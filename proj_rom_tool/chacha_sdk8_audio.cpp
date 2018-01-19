// chacha_sdk8_audio.cpp: Cchacha_sdk8_audio クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "proj_rom.h"
#include "chacha_sdk8_audio.h"
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <math.h>
#include "Wave.h"
#include "UnCompress.h"
#include "afxmt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//Ogg WaveDA for DirectX8 DirectSound
//初期化
//InitDXSound(GetSafeHwnd());
//再生
//WaveDAStart("Y3BG17.ogg");



// グローバル変数
/* 他のソースからも利用されるもの */
IDirectMusicPerformance8* g_pPerformance = NULL;// パフォーマンス
IDirectMusicLoader8* g_pLoader = NULL;			// ローダー
IDirectMusicAudioPath8* g_pDefAudioPath = NULL;	// デフォルト・オーディオパス
IDirectMusicAudioPath8* g_pAudioPath1;			// オーディオ・パス１（必要に応じて利用）
IDirectMusicAudioPath8* g_pAudioPath2;			// オーディオ・パス２（必要に応じて利用）
IDirectMusicSegment8* g_pSeg[32];				// セグメント
IDirectMusicSegmentState8* g_pSegState[32];		// セグメントの状態
MUSIC_TIME g_pSegLength[32];					// セグメントの長さ
long g_nMasterVolume;							// マスター・ボリューム
long g_nVolume1;								// オーディオ・パス１用のボリューム（必要に応じて利用）
long g_nVolume2;								// オーディオ・パス２用のボリューム（必要に応じて利用）


#define WAVDALen      800000
#define WAVDAStartLen 600000
#define BUFSZ			8192
#define HIGHDIV			4
#define BUFSZH			(BUFSZ/HIGHDIV)
#define SQRT_BUFSZ2		64
#define M_PI			3.1415926535897932384
#define ABS(N)			( (N)<0 ? -(N) : (N) )
#define OUTPUT_BUFFER_NUM   10
#define OUTPUT_BUFFER_SIZE  BUFSZ

LPDIRECTSOUND8 m_ds=NULL;
LPDIRECTSOUNDBUFFER m_dsb[MAXSOUNDBUF];
LPDIRECTSOUNDBUFFER m_p=NULL;
HANDLE hNotifyEvent[2];
LPDIRECTSOUNDNOTIFY dsnf1;
LPDIRECTSOUNDNOTIFY dsnf2;
DWORD WINAPI HandleNotifications(LPVOID lpvoid);
CString wf[MAXSOUNDBUF];
DWORD hw;
int fadede ,fadef;
HANDLE NOF;
CString WAV;

char bufwav[WAVDALen+5000];
char *ogg;
OggVorbis_File vf;
WAVEFORMATEX p;
CFile cc;
CString filen="     ";
CSemaphore	m_Smp;
MMRESULT    mmRes;
HWAVEOUT    hwo;
WAVEFORMATEX    wfx;
int cc1,wl,t,oggsize,dd,oggsize1,oggsize2,loop1,loop2;
int playl,ru2=0,ru;
int lo,loc,endf,ps,locs;
int poss=0,loopcnt,pl_no;
int current_section;
long whsize;
int stf;
int plf;

int WaveOpenFile(const char *  pszFileName,HMMIO *phmmioIn,WAVEFORMATEX **ppwfxInfo,MMCKINFO *pckInRIFF);
int WaveStartDataRead(HMMIO *phmmioIn,MMCKINFO *pckIn,MMCKINFO *pckInRIFF);
int WaveReadFile(HMMIO hmmioIn,UINT cbRead,	BYTE *pbDest,MMCKINFO *pckIn,UINT *cbActualRead);
int WaveCloseReadFile(HMMIO *phmmio,WAVEFORMATEX **ppwfxSrc);

int mcopy(char* a,int len,BOOL seek=FALSE,ogg_int64_t pos=0);
long LoadOggVorbis(const char *file_name, int word, char **ogg);
void ReleaseOggVorbis(char **);
void playwav(char* pdsb,DWORD le);


/* WAVEファイルのヘッダ */
typedef struct{
	char ckidRIFF[4];
	int ckSizeRIFF;
	char fccType[4];
	char ckidFmt[4];
	int ckSizeFmt;
	WAVEFORMATEX WaveFmt;
	char ckidData[4];
	int ckSizeData;
} WAVEFILEHEADER;
WAVEFILEHEADER wh;
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

Cchacha_sdk8_audio::Cchacha_sdk8_audio()
{
	CoInitialize(NULL);
	ru=ru2=0;
}

Cchacha_sdk8_audio::~Cchacha_sdk8_audio()
{
    ReleaseDMSegment();
    // DirectX Audioの開放処理
    ReleaseDXAudio();
	ReleaseDXSound();
	CoUninitialize();

}


//-----------------------------------------------------------------------------------------
//DirectX Audioの初期化
//-----------------------------------------------------------------------------------------
BOOL Cchacha_sdk8_audio::InitDXSound(HWND hwnd)
{
	DirectSoundCreate8(NULL,&m_ds,NULL);
	if(m_ds==NULL){ ::OutputDebugString("DirectSoundCreate8() Error\n"); return FALSE; }
	if(m_ds->SetCooperativeLevel(hwnd,DSSCL_PRIORITY)!=DS_OK){::OutputDebugString("SetCooperativeLevel() Error\n"); return FALSE; }
	if(m_ds->SetSpeakerConfig(DSSPEAKER_STEREO)!=DS_OK){::OutputDebugString("SetSpeakerConfig() Error\n"); return FALSE; }
	hw=DSBCAPS_LOCSOFTWARE;
	DSCAPS d;ZeroMemory(&d,sizeof(d));d.dwSize=sizeof(d);
	HRESULT r =m_ds->GetCaps(&d);
	if(r!=DS_OK){ ::OutputDebugString("GetCaps() Error\n"); return FALSE; }
	if(d.dwFlags & (DSCAPS_SECONDARYSTEREO|DSCAPS_PRIMARYSTEREO |DSCAPS_PRIMARY16BIT) && d.dwFreeHwMemBytes!=0){
		hw=DSBCAPS_LOCHARDWARE;
	}
	m_p=NULL;
	DSBUFFERDESC dss;
	ZeroMemory(&dss,sizeof(dss));
	dss.dwSize=sizeof(dss);
	dss.dwFlags=DSBCAPS_CTRLVOLUME|DSBCAPS_PRIMARYBUFFER|hw;
	dss.lpwfxFormat=NULL;
	dss.dwBufferBytes=0;
	if(m_ds->CreateSoundBuffer(&dss,&m_p,NULL)!=DS_OK){ ::OutputDebugString("CreateSoundBuffer() Error\n"); return FALSE; }
	if(m_p!=NULL){
		WAVEFORMATEX p;
		ZeroMemory(&p,sizeof(p));
		p.wFormatTag=WAVE_FORMAT_PCM;
		p.nChannels=2;
		p.nSamplesPerSec=44100;
		p.nBlockAlign=4;
		p.nAvgBytesPerSec=p.nSamplesPerSec*p.nBlockAlign;
		p.wBitsPerSample=16;
		m_p->SetFormat(&p);
	}
	for(int i=0;i<MAXSOUNDBUF;i++){
		m_dsb[i] = NULL;
		wf[i] = "";
	}
	return TRUE;
}

BOOL Cchacha_sdk8_audio::InitDXAudio(HWND hWnd)
{
	HRESULT hr;

	// パフォーマンスの作成
	hr = CoCreateInstance(
		CLSID_DirectMusicPerformance,
		NULL,
		CLSCTX_INPROC,
		IID_IDirectMusicPerformance8,
		(void**)&g_pPerformance );
	if (FAILED(hr)) {
		::OutputDebugString("DirectMusicPerformance8オブジェクトの作成に失敗\n");
		return false;
	}

	// パフォーマンスの初期化
	hr = g_pPerformance->InitAudio( 
		NULL,								// IDirectMusicインターフェイスは不要
		NULL,								// IDirectSoundインターフェイスは不要
		hWnd,								// ウィンドウのハンドル
		DMUS_APATH_SHARED_STEREOPLUSREVERB,	// デフォルトのオーディオパス・タイプ
		64,									// パフォーマンス・チャンネルの数
		DMUS_AUDIOF_ALL,					// シンセサイザの機能
		NULL);								// オーディオ・パラメータにはデフォルトを使用
	if (FAILED(hr)) {
		::OutputDebugString("DirectMusicPerformance8の初期化に失敗\n");
		return false;
	}

	// デフォルト・オーディオパスの取得
	hr = g_pPerformance->GetDefaultAudioPath(&g_pDefAudioPath);
	if (FAILED(hr)) {
		::OutputDebugString("デフォルト・オーディオパスの取得に失敗\n");
		return false;
	}

	// オーディオ・パスを作成（必要に応じて利用する）
	/* １つ目 */
	hr = g_pPerformance->CreateStandardAudioPath( 
		DMUS_APATH_DYNAMIC_STEREO,	// パスの種類。
		64,							// パフォーマンス チャンネルの数。
		TRUE,						// ここでアクティブになる。
		&g_pAudioPath1 );			// オーディオパスを受け取るポインタ。
	if (FAILED(hr))	::OutputDebugString("オーディオ・パス「g_pAudioPath1」の作成に失敗\n");
	/* ２つ目 */
	hr = g_pPerformance->CreateStandardAudioPath( 
		DMUS_APATH_DYNAMIC_STEREO,	// パスの種類。
		64,							// パフォーマンス チャンネルの数。
		TRUE,						// ここでアクティブになる。
		&g_pAudioPath2 );			// オーディオパスを受け取るポインタ。
	if (FAILED(hr))	::OutputDebugString("オーディオ・パス「g_pAudioPath2」の作成に失敗\n");

	// マスター・ボリュームを取得し、初期値としてセット
	hr = g_pPerformance->SetGlobalParam(GUID_PerfMasterVolume, (void*)&g_nMasterVolume, sizeof(long));
	if (FAILED(hr)) {
		::OutputDebugString("マスター・ボリュームの取得に失敗\n");
		return false;
	}
	g_nVolume1 = g_nVolume2 = g_nMasterVolume;

	// ローダーの作成
	hr = CoCreateInstance(
		CLSID_DirectMusicLoader,
		NULL, 
		CLSCTX_INPROC,
		IID_IDirectMusicLoader8,
		(void**)&g_pLoader );
	if (FAILED(hr)) {
		::OutputDebugString("DirectMusicLoader8オブジェクトの作成に失敗\n");
		return false;
	}

	// ローダーの初期化（検索パスをカレント・ディレクトリに設定）
	CHAR strPath[MAX_PATH];
	DWORD cntPath = GetCurrentDirectory(MAX_PATH, strPath);
	if (cntPath==0 || MAX_PATH<cntPath) {
		::OutputDebugString("カレント・ディレクトリの取得に失敗\n");
		return false;
	}

	// マルチ・バイト文字をUNICODEに変換
	WCHAR wstrSearchPath[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, strPath, -1, wstrSearchPath, MAX_PATH);

	// ローダーに検索パスを設定
	hr = g_pLoader->SetSearchDirectory(GUID_DirectMusicAllTypes, wstrSearchPath, FALSE);
	if (FAILED(hr)) {
		::OutputDebugString("検索パスの設定に失敗\n");
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------
// 関数名：LoadSeg(int seg, char* FileName, bool flgMIDI)
// 引　数：int seg 読み込ませたいセグメント番号
//         char FileName 読み込ませたいファイルの名前
// 機　能：ファイル名とセグメント番号を指定してファイルから読み込ませる。
//-----------------------------------------------------------------------------------------
BOOL Cchacha_sdk8_audio::LoadSeg(int seg, char* FileName, bool flgMIDI) 
{
	HRESULT	hr;

	// セグメントの初期化
	if(g_pSeg[seg] != NULL) g_pSeg[seg] = NULL;

	// セグメントにファイル名を指定してロードする
	WCHAR wstrFileName[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, FileName, -1, wstrFileName, MAX_PATH);
	hr = g_pLoader->LoadObjectFromFile(
		CLSID_DirectMusicSegment,
		IID_IDirectMusicSegment8,
		wstrFileName,
		(LPVOID*)&g_pSeg[seg]);
	if (FAILED(hr)) {
		::OutputDebugString("MIDIファイルのロードに失敗\n");
		return false;
	}

	// MIDIファイルの場合、パラメータを設定する
	// （一度スタートすると、停止が実行されるまでループさせる）
	if (flgMIDI) {
		hr = g_pSeg[seg]->SetParam(GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL);
		if (FAILED(hr)) {
			::OutputDebugString("セグメントのパラメータ設定に失敗\n");
			return false;
		}

		// セグメントの長さを取得
		hr = g_pSeg[seg]->GetLength(&g_pSegLength[seg]);
		if (FAILED(hr))	::OutputDebugString("セグメントの長さの取得に失敗\n");

		// セグメントのループ回数設定
		//（０：ループなし　DMUS_SEG_REPEAT_INFINITE：明示的に止められるまでループ）
		hr = g_pSeg[seg]->SetRepeats(DMUS_SEG_REPEAT_INFINITE);
		if (FAILED(hr))	::OutputDebugString("セグメントのループ回数の設定に失敗\n");
	}

	// バンドのダウンロード
	hr = g_pSeg[seg]->Download(g_pPerformance);
	if (FAILED(hr)) {
		::OutputDebugString("セグメントのバンドダウンロードに失敗\n");
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------
//	セグメントの終了処理
//-----------------------------------------------------------------------------------------
BOOL Cchacha_sdk8_audio::ReleaseDMSegment(void)
{
	HRESULT hr;

	// 演奏停止
	if (g_pPerformance) {
		hr = g_pPerformance->Stop(NULL, NULL, 0, 0);
		if (FAILED(hr))	::OutputDebugString("全セグメントの演奏停止に失敗\n");

		// 音色データのアンロード・セグメントの開放
		for (int i=0 ; i<32 ; i++) {
			if (g_pSeg[i]) {
				hr = g_pSeg[i]->Unload(g_pPerformance);
				if (FAILED(hr))	::OutputDebugString("セグメントの音色データのアンロードに失敗\n");
			}
			RELEASE(g_pSegState[i]);
			RELEASE(g_pSeg[i]);
		}
	}
	return true;
}

//-----------------------------------------------------------------------------------------
//	DirectX Audioの終了処理
//-----------------------------------------------------------------------------------------
BOOL Cchacha_sdk8_audio::ReleaseDXAudio(void)
{
	HRESULT hr;

	// ローダを開放
	RELEASE(g_pLoader);

	// オーディオパスを開放
	RELEASE(g_pDefAudioPath);
	RELEASE(g_pAudioPath1);
	RELEASE(g_pAudioPath2);

	// パフォーマンスを開放
	if (g_pPerformance) {
		hr = g_pPerformance->CloseDown();
		if (FAILED(hr))	::OutputDebugString("IDirectMusicPerformance8::CloseDownに失敗\n");
	}
	RELEASE(g_pPerformance);

	return true;
}

BOOL Cchacha_sdk8_audio::ReleaseDXSound(void)
{
	if(m_ds){
		WaveDAEnd();
		for(int i=0;i<MAXSOUNDBUF;i++){
			if(m_dsb[i] != NULL) {m_dsb[i]->Release();m_dsb[i]=NULL;}
		}
		if(m_p!=NULL){m_p->Release();m_p=NULL;}

		if(m_ds)
		{
			m_ds->Release();
			m_ds = NULL;
		}
	}
	return TRUE;
}

void Cchacha_sdk8_audio::WaveSet(CString fname,int no)
{
	if(wf[no].CompareNoCase(fname) == 0 ) return;
	wf[no] = fname;
	CWave waves;
	UnCompress uc;
	waves.Create(uc.Open(fname));

	DWORD        dwDataLen = waves.GetDataLen();
	WAVEFORMATEX wfFormat;
	waves.GetFormat(wfFormat);

	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd,sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME |DSBCAPS_CTRLPAN |DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS|hw;//|DSBCAPS_LOCDEFER;

	dsbd.dwBufferBytes = dwDataLen;
	dsbd.lpwfxFormat = &wfFormat;

	if(m_dsb[no] != NULL) m_dsb[no]->Release();
	HRESULT r=m_ds->CreateSoundBuffer(&dsbd,&m_dsb[no],NULL);
	if(m_dsb[no] == NULL)
	{
		return;
	}
	BYTE* pdsb;
	m_dsb[no]->Lock(0,dwDataLen,(LPVOID *)&pdsb,&dwDataLen,NULL,0,0);
	dwDataLen = waves.GetData(pdsb,dwDataLen);
	m_dsb[no]->Unlock(pdsb,dwDataLen,NULL,0);
}

void Cchacha_sdk8_audio::Play(int n,BOOL m)
{
	DWORD dwStatus;
	if(m_dsb[n]==NULL)return;
	m_dsb[n]->GetStatus(&dwStatus);
	if((dwStatus & DSBSTATUS_PLAYING))
	{
		m_dsb[n]->Stop();
		m_dsb[n]->SetCurrentPosition(0);
	}
	m_dsb[n]->SetPan(0);
	if(n==28||n==29)
	{
		int vl = (8-wavvol2_);
		vl = vl*vl; vl *= 20;
		m_dsb[n]->SetVolume(-vl);
	}else{
		int vl = (8-wavvol);
		vl = vl*vl; vl *= 20;
		m_dsb[n]->SetVolume(-vl);
	}
//	if(MusicOn_Effect == 1)
	{
		if(m==FALSE)
			m_dsb[n]->Play(0,0,0);
		else
			m_dsb[n]->Play(0,0,DSBPLAY_LOOPING );
	}
}

void Cchacha_sdk8_audio::Stop(int n)
{
	if(m_dsb[n] != NULL)
	{
		DWORD dwStatus;
		m_dsb[n]->GetStatus(&dwStatus);
		if((dwStatus & DSBSTATUS_PLAYING))
		{
			m_dsb[n]->Stop();
			m_dsb[n]->SetCurrentPosition(0);
		}
		m_dsb[n]->SetPan(0);
	}
}

void Cchacha_sdk8_audio::Vol()
{
	int vl;
	vl = (8-wavvol);
	vl = vl*vl; vl *= 20;
	for(int y=0;y<MAXSOUNDBUF;y++)
	{
		if(m_dsb[y] != NULL)
			m_dsb[y]->SetVolume(-vl);
	}
	vl = (8-MusicVol);
	vl = vl*vl; vl *= 20;
	
	if(m_dsb[9] != NULL)
		m_dsb[9]->SetVolume(-vl);

}


void Cchacha_sdk8_audio::Vol(int wavvol2)
{
	int vl;
	wavvol2_=wavvol2;
	vl = (8-wavvol2);
	vl = vl*vl; vl *= 20;
	if(m_dsb[28] != NULL)
		m_dsb[28]->SetVolume(-vl);
	if(m_dsb[29] != NULL)
		m_dsb[29]->SetVolume(-vl);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//----------------Ogg WavDA Programs ---------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------


WAVEFORMATEX         *pwfx;		    /* Wave Format data structure */
HMMIO                hmmio;		    /* MM I/O handle for the WAVE */
MMCKINFO             mmck;		    /* Multimedia RIFF chunk */
MMCKINFO             mmckInRIFF;	    /* Use in opening a WAVE file */
LPDIRECTSOUNDBUFFER  lpDSBStreamBuffer; /* Points to DirectSoundBuffer */
DWORD                dwBufferSize;	    /* Size of the entire buffer */
DWORD				 dwNotifySize;		// size of each notification period.
DWORD                dwNextWriteOffset; /* Offset to next buffer segment */
DWORD                dwProgress;	    /* Used with above to show prog. */
DWORD				 dwLastPos;			// the last play position returned by GetCurrentPos().
//DWORD                dwBytesRemaining;  /* Bytes 'til timer shutdown */
BOOL                 bDonePlaying;	    /* Signals early abort to timer */
BOOL                 bLoopFile;	    /* Should we loop playback? */
BOOL                 bFoundEnd;	    /* Timer found file end */

//WAV-DA
int WaveOpenFile(const char * pszFileName,                              // (IN)
						HMMIO *phmmioIn,                                // (OUT)
						WAVEFORMATEX **ppwfxInfo,                       // (OUT)
						MMCKINFO *pckInRIFF                             // (OUT)
						){
	HMMIO           hmmioIn;
	MMCKINFO        ckIn;           // chunk info. for general use.
	PCMWAVEFORMAT   pcmWaveFormat;  // Temp PCM structure to load in.       
	WORD            cbExtraAlloc;   // Extra bytes for waveformatex 
	int             nError;         // Return value.

	// Initialization...
	*ppwfxInfo = NULL;
	nError = 0;
	hmmioIn = NULL;
	
	if ((hmmioIn = mmioOpen((char*)pszFileName, NULL, MMIO_ALLOCBUF | MMIO_READ|MMIO_COMPAT | MMIO_READ)) == NULL)goto ERROR_READING_WAVE;
	if ((nError = (int)mmioDescend(hmmioIn, pckInRIFF, NULL, 0)) != 0)goto ERROR_READING_WAVE;
	if ((pckInRIFF->ckid != FOURCC_RIFF) || (pckInRIFF->fccType != mmioFOURCC('W', 'A', 'V', 'E')))	goto ERROR_READING_WAVE;
			
	/* Search the input file for for the 'fmt ' chunk.     */
    ckIn.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if ((nError = (int)mmioDescend(hmmioIn, &ckIn, pckInRIFF, MMIO_FINDCHUNK)) != 0)goto ERROR_READING_WAVE;                
					
	/* Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;
    * if there are extra parameters at the end, we'll ignore them */
    if (ckIn.cksize < (long) sizeof(PCMWAVEFORMAT))	goto ERROR_READING_WAVE;
															
	/* Read the 'fmt ' chunk into <pcmWaveFormat>.*/     
    if (mmioRead(hmmioIn, (HPSTR) &pcmWaveFormat, (long) sizeof(pcmWaveFormat)) != (long) sizeof(pcmWaveFormat))goto ERROR_READING_WAVE;

	// Ok, allocate the waveformatex, but if its not pcm
	// format, read the next word, and thats how many extra
	// bytes to allocate.
	if (pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM)	cbExtraAlloc = 0;                               
	else{
		// Read in length of extra bytes.
		if (mmioRead(hmmioIn, (LPSTR) &cbExtraAlloc,
			(long) sizeof(cbExtraAlloc)) != (long) sizeof(cbExtraAlloc))goto ERROR_READING_WAVE;
		}
							
	// Ok, now allocate that waveformatex structure.
	if ((*ppwfxInfo = (WAVEFORMATEX*)GlobalAlloc(GMEM_FIXED, sizeof(WAVEFORMATEX)+cbExtraAlloc)) == NULL)goto ERROR_READING_WAVE;

	// Copy the bytes from the pcm structure to the waveformatex structure
	memcpy(*ppwfxInfo, &pcmWaveFormat, sizeof(pcmWaveFormat));
	(*ppwfxInfo)->cbSize = cbExtraAlloc;

	// Now, read those extra bytes into the structure, if cbExtraAlloc != 0.
	if (cbExtraAlloc != 0){
		if (mmioRead(hmmioIn, (LPSTR) (((BYTE*)&((*ppwfxInfo)->cbSize))+sizeof(cbExtraAlloc)),
			(long) (cbExtraAlloc)) != (long) (cbExtraAlloc))goto ERROR_READING_WAVE;
	}

	/* Ascend the input file out of the 'fmt ' chunk. */                                                            
	if ((nError = mmioAscend(hmmioIn, &ckIn, 0)) != 0)	goto ERROR_READING_WAVE;

	goto TEMPCLEANUP;               

ERROR_READING_WAVE:
	if (*ppwfxInfo != NULL){
		GlobalFree(*ppwfxInfo);
		*ppwfxInfo = NULL;
	}               
	if (hmmioIn != NULL){
		mmioClose(hmmioIn, 0);
		hmmioIn = NULL;
	}
	
TEMPCLEANUP:
	*phmmioIn = hmmioIn;
	return(nError);
}

int WaveStartDataRead(
					HMMIO *phmmioIn,
					MMCKINFO *pckIn,
					MMCKINFO *pckInRIFF
					){
	int nError;	nError = 0;
	
	// Do a nice little seek...
	if ((nError = mmioSeek(*phmmioIn, pckInRIFF->dwDataOffset + sizeof(FOURCC), SEEK_SET)) == -1){	}

	nError = 0;
	//      Search the input file for for the 'data' chunk.
	pckIn->ckid = mmioFOURCC('d', 'a', 't', 'a');
	if ((nError = mmioDescend(*phmmioIn, pckIn, pckInRIFF, MMIO_FINDCHUNK)) != 0)goto ERROR_READING_WAVE;
	goto CLEANUP;

ERROR_READING_WAVE:
CLEANUP:        
	return(nError);
}

/*      This will read wave data from the wave file.  Makre sure we're descended into
	the data chunk, else this will fail bigtime!
	hmmioIn         - Handle to mmio.
	cbRead          - # of bytes to read.   
	pbDest          - Destination buffer to put bytes.
	cbActualRead- # of bytes actually read.*/
int WaveReadFile(
		HMMIO hmmioIn,                          // IN
		UINT cbRead,                            // IN           
		BYTE *pbDest,                           // IN
		MMCKINFO *pckIn,                        // IN.
		UINT *cbActualRead                      // OUT.
		){
	MMIOINFO    mmioinfoIn;         // current status of <hmmioIn>
	int                     nError;
	UINT                    cT, cbDataIn, uCopyLength;
	nError = 0;
	if (nError = mmioGetInfo(hmmioIn, &mmioinfoIn, 0) != 0)	goto ERROR_CANNOT_READ;
				
	cbDataIn = cbRead;
	if (cbDataIn > pckIn->cksize) cbDataIn = pckIn->cksize;       
	pckIn->cksize -= cbDataIn;
	
	for (cT = 0; cT < cbDataIn; ){
		/* Copy the bytes from the io to the buffer. */
		if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead){
			if ((nError = mmioAdvance(hmmioIn, &mmioinfoIn, MMIO_READ)) != 0)goto ERROR_CANNOT_READ;
			if (mmioinfoIn.pchNext == mmioinfoIn.pchEndRead){nError = -1;goto ERROR_CANNOT_READ;}
		}
		// Actual copy.
		uCopyLength = (UINT)(mmioinfoIn.pchEndRead - mmioinfoIn.pchNext);
		if((cbDataIn - cT) < uCopyLength )	uCopyLength = cbDataIn - cT;
		memcpy( (BYTE*)(pbDest+cT), (BYTE*)mmioinfoIn.pchNext, uCopyLength );
		cT += uCopyLength;
		mmioinfoIn.pchNext += uCopyLength;
	}
	if ((nError = mmioSetInfo(hmmioIn, &mmioinfoIn, 0)) != 0)goto ERROR_CANNOT_READ;

	*cbActualRead = cbDataIn;
	goto FINISHED_READING;

ERROR_CANNOT_READ:
	*cbActualRead = 0;

FINISHED_READING:
	if(*cbActualRead == 0 || nError != 0){
		m_dsb[9]->Stop();
		m_dsb[9]->SetCurrentPosition(0);
	
		WaveCloseReadFile( &hmmio, &pwfx );
		dsnf1->Release();
		if(m_dsb[9] != NULL) m_dsb[9]->Release();
		m_dsb[9] = NULL;
		ru = ru2 = 0;
	}
	return(nError);
}

int WaveCloseReadFile(
			HMMIO *phmmio,                                  // IN
			WAVEFORMATEX **ppwfxSrc                 // IN
			){
	if (*ppwfxSrc != NULL){
		GlobalFree(*ppwfxSrc);
		*ppwfxSrc = NULL;
	}
	if (*phmmio != NULL){
		mmioClose(*phmmio, 0);
		*phmmio = NULL;
	}
	return(0);
}

void Cchacha_sdk8_audio::DoEvent()
{
	MSG msg;
	if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg); 
	}
}
void Cchacha_sdk8_audio::WaveDAStart(CString fname)
{
	ru = ru2 = 1;fadede = fadef = 0;int no = 9;WAV = fname;
	UnCompress uc;
	if(m_dsb[9] != NULL){
		m_dsb[9]->Release();
		WaveCloseReadFile( &hmmio, &pwfx );
		dsnf1->Release();
		m_dsb[9] = NULL;
	}

	//Ogg Read
//	WaveOpenFile(uc.Open(fname),&hmmio, &pwfx, &mmckInRIFF );
//	WaveStartDataRead( &hmmio, &mmck, &mmckInRIFF ) ;
	ps=0;
	wl=0;
	CString s,b;
	ReleaseOggVorbis(&ogg);
	CFile f;if(f.Open(fname,CFile::modeRead | CFile::typeBinary,NULL)!=TRUE)return;
	f.Close();		
	oggsize = LoadOggVorbis(fname, 2, &ogg);
	loop1=loop2=0;
	CString cc;
	for(int iii=0;iii<vf.vc->comments;iii++){
		cc=vf.vc->user_comments[iii];
		if(cc.Left(10)=="LOOPSTART=") loop1=atoi(cc.Mid(10));
		if(cc.Left(11)=="LOOPLENGTH=")loop2=atoi(cc.Mid(11));
	}
	oggsize1=(loop1)*4;
	oggsize2=(loop2+loop1)*4+whsize;
	endf=0;	if(oggsize2<100){oggsize2=oggsize;endf=1;}
	playl=whsize;

	stf=0;	plf=1;	poss=0;
    //Stereo 16bit 44kHz
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 2;
    wfx.nSamplesPerSec = 44100;
    wfx.wBitsPerSample = 16;
    wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.cbSize = 0;

	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd,sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME |DSBCAPS_CTRLPAN |DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPOSITIONNOTIFY| DSBCAPS_GLOBALFOCUS|hw;//|DSBCAPS_LOCDEFER;;//| DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS  | 	  ;
	dsbd.dwBufferBytes = WAVDALen;///////////////////
	dsbd.lpwfxFormat = &wfx;

	int n=no;
	if(m_dsb[no] != NULL) m_dsb[no]->Release();
	m_dsb[no] = NULL;
	HRESULT r=m_ds->CreateSoundBuffer(&dsbd,&m_dsb[no],NULL);
	if(m_dsb[no] == NULL){return;}
	char* pdsb;
	DWORD le=WAVDAStartLen;
	m_dsb[no]->Lock(0,le/**/,(LPVOID *)&pdsb,&le,NULL,0,0);
//    WaveReadFile(hmmio, (UINT)le/**/, pdsb, &mmck, &uActualBytesWritten );
	playwav(pdsb,le);
	m_dsb[no]->Unlock(pdsb,le/**/,NULL,0);
	m_dsb[n]->QueryInterface(IID_IDirectSoundNotify,(LPVOID *)&dsnf1);
	DSBPOSITIONNOTIFY dsn[20];

	hNotifyEvent[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
	hNotifyEvent[1] = CreateEvent(NULL, FALSE, FALSE, NULL);

	// setup the first one.
	for(int y =0;y<8;y++){
		dsn[y].dwOffset = WAVDALen/40+y*(WAVDALen/8);
		dsn[y].hEventNotify = hNotifyEvent[0];
	}
	dsn[y].dwOffset = DSBPN_OFFSETSTOP;
	dsn[y].hEventNotify = hNotifyEvent[1];

	t = WAVDAStartLen-WAVDALen/8;
	// Now create the thread to wait on the events created.
	DWORD dwThreadId;
	NOF = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HandleNotifications, NULL, 0, &dwThreadId) ;
	SetThreadPriority(NOF,THREAD_PRIORITY_ABOVE_NORMAL/*THREAD_PRIORITY_BELOW_NORMAL*/);
	dsnf1->SetNotificationPositions(y+1,dsn);

	DWORD dwStatus;
	m_dsb[n]->GetStatus(&dwStatus);
	if((dwStatus & DSBSTATUS_PLAYING)){
		m_dsb[n]->Stop();
		m_dsb[n]->SetCurrentPosition(0);
	}
	m_dsb[n]->SetPan(0);
	int vl = (8-MusicVol);
	vl = vl*vl; vl *= 20;
	m_dsb[n]->SetVolume(-vl);
	fadede = -vl;
	fadede=0;
	m_dsb[n]->Play(0,0,DSBPLAY_LOOPING );
}

void Cchacha_sdk8_audio::WaveDAEnd()
{
	if(m_dsb[9] == NULL){return;}
	if(ru == 0) return;
	m_dsb[9]->Stop();
	m_dsb[9]->SetCurrentPosition(0);
	for(;ru !=0;)
		DoEvent();
	stop();
	dsnf1->Release();
	if(m_dsb[9] != NULL) m_dsb[9]->Release();
	m_dsb[9] = NULL;
	ru = ru2 = 0;
}


//スレッド
DWORD WINAPI HandleNotifications(LPVOID lpvoid)
{
	DWORD hr = DS_OK;
	DWORD hRet = 0;

	while((hRet = WaitForMultipleObjects(2, hNotifyEvent, FALSE, INFINITE))!= WAIT_FAILED)
	{
		rand();
		if(ru2 == 0) break;
		// Deal with the event that got signalled.
		switch(hRet-WAIT_OBJECT_0)
		{
		case 0:
			{	// A play notification has been received.
				int no = 9;

				t = t + WAVDALen/8;
				if( t > WAVDALen-(WAVDALen/8)) t = 000000;
				DWORD        dwDataLen = WAVDALen/8;
				if(fadef == 1){
					fadede -= 200;
					if(fadede <= -5000) {
						fadede = -10000;
						m_dsb[no]->SetVolume(fadede);
//						m_dsb[9]->Stop();
						break;
					}
					m_dsb[no]->SetVolume(fadede);
				}
				if(fadede > -5000){
					char* pdsb;
					m_dsb[no]->Lock(t,dwDataLen,(LPVOID *)&pdsb,&dwDataLen,NULL,0,0);
//					WaveReadFile(hmmio, (UINT)dwDataLen, pdsb, &mmck, &cbActual );
					playwav(pdsb,dwDataLen);
					m_dsb[no]->Unlock(pdsb,dwDataLen,NULL,0);
				}
				break;
			}
		case 1:
			{

				m_dsb[9]->Stop();
				CloseHandle(hNotifyEvent[0]);
				CloseHandle(hNotifyEvent[1]);
				hNotifyEvent[0] = hNotifyEvent[1] = (HANDLE)NULL;
				ru2 = 0;
				break;
			}
		default:
			ASSERT(FALSE);	// we should never fall in here.
			break;

		}// end switch

	} // while

	//UpdateProgressBar();
	ru = 0;
	return TRUE;
} //handlenotifications()

//////////////////////////////////////////////////////////////////////////////
long LoadOggVorbis(const char *file_name, int word, char **ogg)
{
	int eof=0;
	FILE *fp;
	long size = 0;
	vorbis_info *vi;
	long data_size;
	/* 量子化バイト数が正しい値かどうか調べる */
	if(!(word == 1 || word == 2)){
		fprintf(stderr, "word is error");
		return -1;
	}
	/* ファイルを開く */
	fp = fopen(file_name, "rb");
	if(fp == NULL){
		fprintf(stderr, "Open Error : %s", file_name);
		return -1;
	}
	/* OggVorbis初期化 */
	if(ov_open(fp, &vf, NULL, 0) < 0) {
      fprintf(stderr,"Input does not appear to be an Ogg bitstream.\n");
			fclose(fp);
      return -1;
	}
	else{
		vi=ov_info(&vf,-1);
	}

	/* ヘッダサイズの収得 */
	whsize = sizeof(wh.ckidRIFF) + sizeof(wh.ckSizeRIFF) + sizeof(wh.fccType) +
					 sizeof(wh.ckidFmt)  + sizeof(wh.ckSizeFmt)  + sizeof(wh.WaveFmt) +
					 sizeof(wh.ckidData) + sizeof(wh.ckSizeData);

	/* デコード後のデータサイズを求め、メモリ確保 */
	data_size = (long)ceil(vi->channels * vi->rate * ov_time_total(&vf,-1) * word);
	dd=vi->channels * vi->rate*word;
	*ogg = (char *)malloc(whsize);
	if(ogg == NULL){
		free(ogg);
	  ov_clear(&vf);
		fclose(fp);
		fprintf(stderr, "malloc Error\n");
		return -1;
	}
	/* ヘッダの初期化 */
	memcpy(wh.ckidRIFF, "RIFF", 4);
	wh.ckSizeRIFF = whsize + size - 8;
	memcpy(wh.fccType, "WAVE", 4);
	memcpy(wh.ckidFmt, "fmt ", 4);
	wh.ckSizeFmt = sizeof(WAVEFORMATEX);

	wh.WaveFmt.cbSize          = sizeof(WAVEFORMATEX);
	wh.WaveFmt.wFormatTag      = WAVE_FORMAT_PCM;
	wh.WaveFmt.nChannels       = vi->channels;
	wh.WaveFmt.nSamplesPerSec  = vi->rate;
	wh.WaveFmt.nAvgBytesPerSec = vi->rate * vi->channels * word;
	wh.WaveFmt.nBlockAlign     = vi->channels * word;
	wh.WaveFmt.wBitsPerSample  = word * 8;

	memcpy(wh.ckidData, "data", 4);
	wh.ckSizeData = size;

	/* メモリへのヘッダの書き込み */
	int s = 0;
	memcpy(*ogg, &wh.ckidRIFF, sizeof(wh.ckidRIFF));          s += sizeof(wh.ckidRIFF);
	memcpy(*ogg + s, &wh.ckSizeRIFF, sizeof(wh.ckSizeRIFF));  s += sizeof(wh.ckSizeRIFF);
	memcpy(*ogg + s, &wh.fccType, sizeof(wh.fccType));        s += sizeof(wh.fccType);
	memcpy(*ogg + s, &wh.ckidFmt, sizeof(wh.ckidFmt));        s += sizeof(wh.ckidFmt);
	memcpy(*ogg + s, &wh.ckSizeFmt, sizeof(wh.ckSizeFmt));    s += sizeof(wh.ckSizeFmt);
	memcpy(*ogg + s, &wh.WaveFmt, sizeof(wh.WaveFmt));        s += sizeof(wh.WaveFmt);
	memcpy(*ogg + s, &wh.ckidData, sizeof(wh.ckidData));      s += sizeof(wh.ckidData);
	memcpy(*ogg + s, &wh.ckSizeData, sizeof(wh.ckSizeData));

	return data_size + whsize;
}

void ReleaseOggVorbis(char **ogg)
{
    ov_clear(&vf);
	if(ogg!=NULL)
	{
		free(*ogg);
		ogg=NULL;
	}
}

void playwav(char* pdsb,DWORD le)
{
    //データ読み込み
	lo++;if( lo==OUTPUT_BUFFER_NUM) lo=0;
	DWORD dwDataLen = OUTPUT_BUFFER_SIZE;
	int rrr=mcopy(pdsb,le);
	if((int)le != rrr){
		if(endf==1){
			playl=whsize+oggsize1;
			mcopy(pdsb+rrr,(int)le-rrr,TRUE,(ogg_int64_t)loop1);
		}else{
			playl=whsize+oggsize1;
			mcopy(pdsb+rrr,(int)le-rrr,TRUE,(ogg_int64_t)loop1);
		}
	}
}

void Cchacha_sdk8_audio::stop()
{
	if(ogg!=NULL){
		CSingleLock	sema(&m_Smp);
		sema.Lock();
		stf=1;
		sema.Unlock();
		plf=0;
		ReleaseOggVorbis(&ogg);
		ogg=NULL;
	}
}

//oggから実際にデータを獲得する
int mcopy(char* a,int len,BOOL seek,ogg_int64_t pos)
{
	int ret=0;
	int lenl=len;
	if(seek==TRUE){
		ov_pcm_seek(&vf,pos);
		poss=0;
	}
	for(;;){
		ret = ov_read(&vf, (bufwav+poss), 4096, 0, 2, 1, &current_section);
		poss+=ret;
		if(ret==0) break;
		if(len<=poss)	break;
	}
	if(playl+len > oggsize2) lenl=oggsize2 - playl;
	memcpy(a,bufwav,lenl);
	if(len<=poss){
		poss-=len;
		if(poss!=0)	memcpy(bufwav,bufwav+len,poss);
	}
	wl+=lenl;
	playl+=lenl;
	return lenl;	
}