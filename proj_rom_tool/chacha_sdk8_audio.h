// chacha_sdk8_audio.h: Cchacha_sdk8_audio �N���X�̃C���^�[�t�F�C�X
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
extern IDirectMusicPerformance8* g_pPerformance;// �p�t�H�[�}���X
extern IDirectMusicLoader8* g_pLoader;			// ���[�_�[
extern IDirectMusicAudioPath8* g_pDefAudioPath;	// �f�t�H���g�E�I�[�f�B�I�p�X
extern IDirectMusicAudioPath8* g_pAudioPath1;	// �I�[�f�B�I�E�p�X�P�i�K�v�ɉ����č쐬�j
extern IDirectMusicAudioPath8* g_pAudioPath2;	// �I�[�f�B�I�E�p�X�Q�i�K�v�ɉ����č쐬�j
extern IDirectMusicSegment8* g_pSeg[32];		// �Z�O�����g
extern IDirectMusicSegmentState8* g_pSegState[32]; // �Z�O�����g�̏��
extern MUSIC_TIME g_pSegLength[32];				// �Z�O�����g�̒���
extern long g_nMasterVolume;					// �}�X�^�[�E�{�����[��
extern long g_nVolume1;							// �I�[�f�B�I�E�p�X�P�p�̃{�����[���i�K�v�ɉ����ė��p�j
extern long g_nVolume2;							// �I�[�f�B�I�E�p�X�Q�p�̃{�����[���i�K�v�ɉ����ė��p�j



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
