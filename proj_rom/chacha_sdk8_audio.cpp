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
#include "afxmt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
int fadede ,fadef,fadef2;

//Ogg WaveDA for DirectX8 DirectSound
//初期化
//InitDXSound(GetSafeHwnd());
//再生
//WaveDAStart("Y3BG17.ogg");



// グローバル変数

#define MAXSOUNDBUF 90
#define WAVDALen      800000
#define WAVDAStartLen 100000
#define BUFSZ			8192
#define HIGHDIV			4
#define BUFSZH			(BUFSZ/HIGHDIV)
#define SQRT_BUFSZ2		64
#define M_PI			3.1415926535897932384
#define ABS(N)			( (N)<0 ? -(N) : (N) )
#define OUTPUT_BUFFER_NUM   10
#define OUTPUT_BUFFER_SIZE  BUFSZ

LPDIRECTSOUND8 m_ds=NULL;
LPDIRECTSOUNDBUFFER8 m_dsb[MAXSOUNDBUF];
LPDIRECTSOUND3DBUFFER8 m_3d;
LPDIRECTSOUNDBUFFER m_p=NULL;
HANDLE hNotifyEvent[2];
LPDIRECTSOUNDNOTIFY dsnf1;
LPDIRECTSOUNDNOTIFY dsnf2;
UINT HandleNotifications(LPVOID lpvoid);
CString wf[MAXSOUNDBUF];
int wavp[MAXSOUNDBUF];
int	wavt[36][5]={{1,0,0,0,0},
		{1,1,1,1,1},{1,2,2,2,2},{1,3,3,3,3},{1,4,4,4,4},{1,5,5,5,5},{1,6,6,6,6},{1,7,7,7,7},{1,8,8,8,8},{1,9,9,9,9},
		{1,10,10,10,10},{1,11,11,11,11},{1,12,12,12,12},{4,13,40,41,42},{4,14,43,44,45},{1,15,15,15,15},
		{1,16,16,16,16},{4,17,46,47,48},{1,18,18,18,18},{2,19,49,0,0},{4,20,50,51,52},{4,21,53,54,55},
		{4,22,56,57,58},{1,23,0,0,0},{4,24,59,60,61},{4,25,62,63,64},{4,26,65,66,67},{4,27,68,69,70},
		{4,28,71,72,73},{4,29,74,75,76},{3,30,77,78,0},{3,31,79,80,0},{4,32,81,82,83},{1,33,0,0,0},
		{4,34,84,85,86},{1,35,0,0,0}
	};

DWORD hw;
HANDLE NOF;
CString WAV;

char bufwav[WAVDALen];
char bufwav2[WAVDALen];
char *ogg;
OggVorbis_File vf;
WAVEFORMATEX p;
CFile cc;
CString filen="     ";
CSemaphore	m_Smp;
MMRESULT    mmRes;
HWAVEOUT    hwo;
WAVEFORMATEX    wfx;
int cc1,t,oggsize,dd,oggsize1,oggsize2,loop1,loop2;
int playl,ru2=0,ru;
int lo,loc,ps,locs;
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
//	CoInitialize(NULL);
	ru=ru2=0;m_ds = NULL;
	MusicVol=wavvol=8;
}

Cchacha_sdk8_audio::~Cchacha_sdk8_audio()
{
	ReleaseDXSound();
//	CoUninitialize();

}


//-----------------------------------------------------------------------------------------
//DirectX Audioの初期化
//-----------------------------------------------------------------------------------------
BOOL Cchacha_sdk8_audio::InitDXSound(HWND hwnd)
{
	DirectSoundCreate8(NULL,&m_ds,NULL);
	if(m_ds==NULL){ ::OutputDebugString("DirectSoundCreate8() Error\n"); return FALSE; }
	if(m_ds->SetCooperativeLevel(hwnd,DSSCL_PRIORITY)!=DS_OK){::OutputDebugString("SetCooperativeLevel() Error\n"); return FALSE; }
//	if(m_ds->SetSpeakerConfig(DSSPEAKER_COMBINED(DSSPEAKER_STEREO, DSSPEAKER_GEOMETRY_WIDE))!=DS_OK){::OutputDebugString("SetSpeakerConfig() Error\n"); return FALSE; }
	hw=DSBCAPS_LOCSOFTWARE;
	DSCAPS d;ZeroMemory(&d,sizeof(d));d.dwSize=sizeof(d);
	HRESULT r =m_ds->GetCaps(&d);
	if(r!=DS_OK){ ::OutputDebugString("GetCaps() Error\n"); return FALSE; }
	if(d.dwFlags & DSCAPS_SECONDARYSTEREO &&
	   d.dwFlags & DSCAPS_PRIMARYSTEREO &&
	   d.dwFlags & DSCAPS_PRIMARY16BIT && d.dwFreeHwMemBytes!=0){
		hw=DSBCAPS_LOCHARDWARE;
	}
	switch(save.sbuf)
	{
	case 0:
		hw=hw;
		break;
	case 1:
		hw=DSBCAPS_LOCHARDWARE;
		break;
	case 2:
		hw=DSBCAPS_LOCSOFTWARE;
		break;
	}
	m_p=NULL;
	DSBUFFERDESC dss;
	ZeroMemory(&dss,sizeof(dss));
	dss.dwSize=sizeof(dss);
	dss.dwFlags=DSBCAPS_CTRLPAN|DSBCAPS_CTRLVOLUME|DSBCAPS_PRIMARYBUFFER|DSBCAPS_CTRL3D|hw;
	dss.lpwfxFormat=NULL;
	dss.dwBufferBytes=0;
	if(m_ds->CreateSoundBuffer(&dss,&m_p,NULL)!=DS_OK){
		dss.dwFlags=DSBCAPS_CTRLPAN|DSBCAPS_CTRLVOLUME|DSBCAPS_PRIMARYBUFFER|DSBCAPS_CTRL3D|DSBCAPS_LOCSOFTWARE;
		hw=DSBCAPS_LOCSOFTWARE;
		if(m_ds->CreateSoundBuffer(&dss,&m_p,NULL)!=DS_OK){
			::OutputDebugString("CreateSoundBuffer() Error\n"); return FALSE; 
		}
	}

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
		wavp[i]=0;
	}
	return TRUE;
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
	if(waves.Create(uc.Open(fname))==FALSE) return;

	DWORD        dwDataLen = waves.GetDataLen();
	WAVEFORMATEX wfFormat;
	waves.GetFormat(wfFormat);

	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd,sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME |DSBCAPS_CTRLPAN |DSBCAPS_CTRLFREQUENCY|hw;//|DSBCAPS_LOCDEFER;

	dsbd.dwBufferBytes = dwDataLen;
	dsbd.lpwfxFormat = &wfFormat;

	LPDIRECTSOUNDBUFFER pDSB=NULL;
	for(int i=0;i<wavt[no][0];i++){
		if(m_dsb[wavt[no][i+1]] != NULL) m_dsb[wavt[no][i+1]]->Release();
		if(i==0){
			if(m_ds->CreateSoundBuffer(&dsbd,&pDSB,NULL)!=DS_OK){
				dsbd.dwFlags = DSBCAPS_CTRLVOLUME |DSBCAPS_CTRLPAN |DSBCAPS_CTRLFREQUENCY|DSBCAPS_LOCSOFTWARE;
				hw=DSBCAPS_LOCSOFTWARE;
				if(m_ds->CreateSoundBuffer(&dsbd,&pDSB,NULL)!=DS_OK){
					return;
				}
			}
			pDSB->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_dsb[wavt[no][i+1]]);
			RELEASE(pDSB);

			BYTE* pdsb;
			m_dsb[wavt[no][i+1]]->Lock(0,dwDataLen,(LPVOID *)&pdsb,&dwDataLen,NULL,0,0);
			dwDataLen = waves.GetData(pdsb,dwDataLen);
			m_dsb[wavt[no][i+1]]->Unlock(pdsb,dwDataLen,NULL,0);
		}else{
			m_ds->DuplicateSoundBuffer(m_dsb[wavt[no][1]],&pDSB);
			pDSB->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_dsb[wavt[no][i+1]]);
			RELEASE(pDSB);
		}
	}

}

void Cchacha_sdk8_audio::Play(int n,int pan,BOOL m)
{
	wavp[n]++;if(wavt[n][0]==wavp[n]) wavp[n]=0;
	n=wavt[n][wavp[n]+1];
	DWORD dwStatus;
	if(m_dsb[n]==NULL)return;
	m_dsb[n]->GetStatus(&dwStatus);
	if((dwStatus & DSBSTATUS_PLAYING)){
		m_dsb[n]->Stop();
		m_dsb[n]->SetCurrentPosition(0);
	}
	m_dsb[n]->SetPan(pan);
	int vl = (8-save.effectvol);
	vl = vl*vl; vl *= 20;
	m_dsb[n]->SetVolume(-vl);
	if(save.effectplayf)
	{
		if(m==FALSE)
			m_dsb[n]->Play(0,0,0);
		else
			m_dsb[n]->Play(0,0,DSBPLAY_LOOPING );
	}
}

void Cchacha_sdk8_audio::Stop(int n)
{
	if(wavp[n]) n+=MAXSOUNDBUF/2;
	if(m_dsb[n] != NULL){
		DWORD dwStatus;
		m_dsb[n]->GetStatus(&dwStatus);
		if((dwStatus & DSBSTATUS_PLAYING)){
			m_dsb[n]->Stop();
			m_dsb[n]->SetCurrentPosition(0);
		}
		m_dsb[n]->SetPan(0);
	}
}

void Cchacha_sdk8_audio::Vol()
{
	int vl;
	vl = (8-save.effectvol);
	vl = vl*vl; vl *= 20;
	for(int y=0;y<MAXSOUNDBUF;y++){
		if(m_dsb[y] != NULL)
			m_dsb[y]->SetVolume(-vl);
	}
	vl = (8-save.bgmvol);
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
	DWORD             nError;         // Return value.

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
	if ((nError = mmioGetInfo(hmmioIn, &mmioinfoIn, 0)) != 0)	goto ERROR_CANNOT_READ;
				
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

char ab[]={"bgm\\"};
void Cchacha_sdk8_audio::WaveDAStart(CString fname)
{
	int no = 9;
	if(!save.bgmplayf) return;
	if(ab+fname==fnames) return;
	if(fname==""){
		fname=fnames;
		if(fname=="") return;
	}else {fname=ab+fname; }
	WaveDAEnd();
	ru = ru2 = 1;fadede = fadef = 0;WAV = fname; fnames=fname;
	//Ogg Read
	ps=0;
	CString s,b;
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
	if(oggsize2<100){oggsize2=oggsize;}
	playl=whsize;

	stf=0;	plf=1;	poss=0;
    //Stereo 16bit 44kHz
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 2;
    wfx.nSamplesPerSec = 44100;
    wfx.wBitsPerSample = 16;
    wfx.nBlockAlign = (unsigned short)(wfx.nChannels * wfx.wBitsPerSample / 8);
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.cbSize = 0;

	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd,sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME |DSBCAPS_CTRLPAN |DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPOSITIONNOTIFY| DSBCAPS_GLOBALFOCUS|hw;//|DSBCAPS_LOCDEFER;;//| DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS  | 	  ;
	dsbd.dwBufferBytes = WAVDALen;///////////////////
	dsbd.lpwfxFormat = &wfx;

	int n=no;
	if(m_dsb[no] != NULL){ m_dsb[no]->Release(); }
	m_dsb[no] = NULL;
	LPDIRECTSOUNDBUFFER pDSB=NULL;
	if(m_ds->CreateSoundBuffer(&dsbd,&pDSB,NULL)!=DS_OK){
		hw=DSBCAPS_LOCSOFTWARE;
		dsbd.dwFlags = DSBCAPS_CTRLVOLUME |DSBCAPS_CTRLPAN |DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPOSITIONNOTIFY| DSBCAPS_GLOBALFOCUS|hw;//|DSBCAPS_LOCDEFER;;//| DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS  | 	  ;
		if(m_ds->CreateSoundBuffer(&dsbd,&pDSB,NULL)!=DS_OK){
			return;
		}
	}
//	pDSB->QueryInterface(IID_IDirectSound3DBuffer, (LPVOID*)&m_3d);
	pDSB->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_dsb[no]);
    RELEASE(pDSB);
	if(m_dsb[no] == NULL){return;}
	char* pdsb;
	DWORD le=WAVDAStartLen;
	m_dsb[no]->Lock(0,le/**/,(LPVOID *)&pdsb,&le,NULL,0,0);
//    WaveReadFile(hmmio, (UINT)le/**/, pdsb, &mmck, &uActualBytesWritten );
	playwav(pdsb,le);
	m_dsb[no]->Unlock(pdsb,le/**/,NULL,0);
	m_dsb[n]->QueryInterface(IID_IDirectSoundNotify,(LPVOID *)&dsnf1);
	DSBPOSITIONNOTIFY dsn[70];

	hNotifyEvent[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
	hNotifyEvent[1] = CreateEvent(NULL, FALSE, FALSE, NULL);

	// setup the first one.
	for(int y =0;y<32;y++){
		dsn[y].dwOffset = WAVDALen/40+y*(WAVDALen/32);
		dsn[y].hEventNotify = hNotifyEvent[0];
	}
	dsn[y].dwOffset = DSBPN_OFFSETSTOP;
	dsn[y].hEventNotify = hNotifyEvent[1];

	t = WAVDAStartLen-WAVDALen/32;
	// Now create the thread to wait on the events created.
	AfxBeginThread(HandleNotifications,(LPVOID)this);
//	DWORD dwThreadId;
//	NOF = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HandleNotifications, NULL, 0, &dwThreadId) ;
//	SetThreadPriority(NOF,THREAD_PRIORITY_ABOVE_NORMAL/*THREAD_PRIORITY_BELOW_NORMAL*/);
	dsnf1->SetNotificationPositions(y+1,dsn);

	DWORD dwStatus;
	m_dsb[n]->GetStatus(&dwStatus);
	if((dwStatus & DSBSTATUS_PLAYING)){
		m_dsb[n]->Stop();
		m_dsb[n]->SetCurrentPosition(0);
	}
	m_dsb[n]->SetPan(0);
	int vl = (8-save.bgmvol);
	vl = vl*vl; vl *= 20;
	m_dsb[n]->SetVolume(-vl);
	fadede = -vl;
//	fadede=0;
	m_dsb[n]->Play(0,0,DSBPLAY_LOOPING );
}

DWORD popopo;
void Cchacha_sdk8_audio::Pause(BOOL play)
{
	if(m_dsb[9]){
		if(play){
			int vl = (8-save.bgmvol);
			vl = vl*vl; vl *= 20;
			m_dsb[9]->SetVolume(-vl);
		}else{
			m_dsb[9]->SetVolume(-10000);
		}
	}
}

void Cchacha_sdk8_audio::WaveDAEnd()
{
	if(m_dsb[9] == NULL){return;}
	if(ru == 0) return;
	m_dsb[9]->Stop();
	m_dsb[9]->SetCurrentPosition(0);
	for(;ru !=0;)
		DoEvent();
	dsnf1->Release();
	if(m_dsb[9] != NULL){m_dsb[9]->Release();}
	m_dsb[9] = NULL;
	stop();
	ru = ru2 = 0;
}


//スレッド
UINT HandleNotifications(LPVOID)
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
				DWORD  dwDataLen = WAVDALen/32;

				t = t + (int)dwDataLen;
				if( t > WAVDALen-(int)dwDataLen) t = 0;
				if(fadef == 1){
					fadede -= 200;
					if(fadede <= -5000) {
						fadede = -10000;
						m_dsb[no]->SetVolume(fadede);
						fadef2=1;
//						m_dsb[9]->Stop();
						break;
					}
					m_dsb[no]->SetVolume(fadede);
				}
				if(fadede > -5000){
					char* pdsb;
					playwav(bufwav2,dwDataLen);
					m_dsb[no]->Lock(t,dwDataLen,(LPVOID *)&pdsb,&dwDataLen,NULL,0,0);
//					WaveReadFile(hmmio, (UINT)dwDataLen, pdsb, &mmck, &cbActual );
//					playwav(pdsb,dwDataLen);
					memcpy(pdsb,bufwav2,dwDataLen);
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
	return 0;
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
	wh.WaveFmt.nChannels       = (unsigned short)vi->channels;
	wh.WaveFmt.nSamplesPerSec  = vi->rate;
	wh.WaveFmt.nAvgBytesPerSec = vi->rate * vi->channels * word;
	wh.WaveFmt.nBlockAlign     = (unsigned short)(vi->channels * word);
	wh.WaveFmt.wBitsPerSample  = (unsigned short)(word * 8);

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
	int rrr=mcopy(pdsb,le);
	if((int)le != rrr){
		playl=whsize+oggsize1;
		mcopy(pdsb+rrr,(int)le-rrr,TRUE,(ogg_int64_t)loop1);
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
	if(playl+len > oggsize2) lenl=oggsize2 - playl;
	for(;;){
		ret = ov_read(&vf, (bufwav+poss), 4096, 0, 2, 1, &current_section);
		poss+=ret;
		if(ret==0) break;
		if(lenl<=poss)	break;
	}
	memcpy(a,bufwav,lenl);
	if(lenl<=poss){
		poss-=lenl;
		if(poss!=0)	memcpy(bufwav,bufwav+lenl,poss);
	}
	playl+=lenl;
	return lenl;	
}