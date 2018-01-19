// chacha_sdk8_audio.h: Cchacha_sdk8_audio クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHACHA_SDK8_AUDIO_H__F6C396AF_5083_49C9_9E0D_2E4DF0DB22CE__INCLUDED_)
#define AFX_CHACHA_SDK8_AUDIO_H__F6C396AF_5083_49C9_9E0D_2E4DF0DB22CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class Cchacha_sdk8_audio  
{
public:
	Cchacha_sdk8_audio();
	virtual ~Cchacha_sdk8_audio();

	BOOL InitDXSound(HWND hWnd);
	BOOL ReleaseDXSound(void);
	void WaveSet(CString fname,int no);
	void Play(int n,int pan=0,BOOL m=FALSE);
	void Stop(int n);
	void Pause(BOOL play);
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
	CString fnames;

};

#endif // !defined(AFX_CHACHA_SDK8_AUDIO_H__F6C396AF_5083_49C9_9E0D_2E4DF0DB22CE__INCLUDED_)
