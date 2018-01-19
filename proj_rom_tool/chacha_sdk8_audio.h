// chacha_sdk8_audio.h: Cchacha_sdk8_audio クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHACHA_SDK8_AUDIO_H__F6C396AF_5083_49C9_9E0D_2E4DF0DB22CE__INCLUDED_)
#define AFX_CHACHA_SDK8_AUDIO_H__F6C396AF_5083_49C9_9E0D_2E4DF0DB22CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VOLUME_MAX 1000
#define VOLUME_MIN -10000
#define MAXSOUNDBUF 35
extern IDirectMusicPerformance8* g_pPerformance;// パフォーマンス
extern IDirectMusicLoader8* g_pLoader;			// ローダー
extern IDirectMusicAudioPath8* g_pDefAudioPath;	// デフォルト・オーディオパス
extern IDirectMusicAudioPath8* g_pAudioPath1;	// オーディオ・パス１（必要に応じて作成）
extern IDirectMusicAudioPath8* g_pAudioPath2;	// オーディオ・パス２（必要に応じて作成）
extern IDirectMusicSegment8* g_pSeg[32];		// セグメント
extern IDirectMusicSegmentState8* g_pSegState[32]; // セグメントの状態
extern MUSIC_TIME g_pSegLength[32];				// セグメントの長さ
extern long g_nMasterVolume;					// マスター・ボリューム
extern long g_nVolume1;							// オーディオ・パス１用のボリューム（必要に応じて利用）
extern long g_nVolume2;							// オーディオ・パス２用のボリューム（必要に応じて利用）



class Cchacha_sdk8_audio  
{
public:
	Cchacha_sdk8_audio();
	virtual ~Cchacha_sdk8_audio();

	BOOL InitDXAudio(HWND hWnd);
	BOOL InitDXSound(HWND hWnd);
	BOOL LoadSeg(int seg, char* FileName, bool flgMIDI=false);
	BOOL ReleaseDMSegment(void);
	BOOL ReleaseDXAudio(void);
	BOOL ReleaseDXSound(void);
	void WaveSet(CString fname,int no);
	void Play(int n,BOOL m);
	void Stop(int n);
	void WaveDAStart(CString fname);
	void WaveDAEnd();
	void DoEvent();
	void Vol();
	void Vol(int wavvol2);
	void stop();

	int wavvol;
	int wavvol2_;
	int MusicVol;
	int MusicOn_Effect;

};

#endif // !defined(AFX_CHACHA_SDK8_AUDIO_H__F6C396AF_5083_49C9_9E0D_2E4DF0DB22CE__INCLUDED_)
