// MainFrm.h : CMainFrame �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__CB74BAD3_A130_4BC8_BC2C_571FF49FA2E7__INCLUDED_)
#define AFX_MAINFRM_H__CB74BAD3_A130_4BC8_BC2C_571FF49FA2E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
public:	
//protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
public:	
	//����
	BOOL Create(const CString& sTitle);
	BOOL Init();
	void Close();
	//�Q�[�����C��
	void GameProc();
	//�Q�[���J�n
	BOOL GamePlay(int no);
	int PausePlay();
	//���S
	void LogoPlay();
	static UINT oggplay(LPVOID pParam );
	static UINT ReplaySave(LPVOID pParam );
	static UINT Loading(LPVOID pParam );
	void Loadingmain();
	BOOL loadf,loadf2;
	void HosiOkuMake();
	void HosiOkuProc();
	void HosiOkuDraw();
	//�^�C�g��
	BOOL TitlePlay(int ka);
	void HosiMake();
	//�����L���O
	void RankigPlay();
	void motanime();
	void motDraw();
	//���Z�\��
	void UraInfo();
	//�G���f�B���O
	void EndPlay();

	DWORD GetTiming(DWORD);
	void Naido(int);
	
	void KeyWait();
	CString tb[5];
	int demof;

	CString bgm;
	BOOL bgmth;
	//�L�����G�ړ�����
	void Idou();
	void Draw();
	BOOL GetTekiNo(int teki,DWORD no);
	BOOL CheckMap(float x,float y,BOOL f=FALSE);
	BOOL CheckMap_(float x,float y,BOOL f=TRUE);
	int CheckMap2(float x,float y);
	void Kasoku(int i,int j,float& x,float& y);
	void Tekitama(int no,float xx,float yy,float tss,int t);
	void Tekihaiti(int tekino,int tekidatano,BOOL yomi=TRUE);
	//�X�e�[�W�f�[�^�ǂݍ���
	void LoadStage(int& no);
	void InitMiStart(CString s,BOOL f=FALSE);
	void AnimeMiStart();
	void DrawMiStart();
	void InitMiStart2(CString s,BOOL f=FALSE);
	void AnimeMiStart2();
	void DrawMiStart2();
	BOOL tekikill(int& ti);
	void input();

	void Copy_tekidata(int &ti);
	void Copy_tekitama(int &ti);
	void Copy_tama(int &ti);
	void Copy_lockon(int &ti);



// �A�g���r���[�g
public:

// �I�y���[�V����
public:
	datasave ds;
	Cchacha_sdk8_graphic chacha;
	Cchacha_sdk8_audio chacha_a;
	Cchacha_sdk8_Input chacha_i;

	BOOL fs;
	int mousex,mousey;
	float gamenx,gameny;
	float gamenx2,gameny2;
	float gamenx5,gameny5;
	float gamenx_,gameny_;
	float gamenx2_,gameny2_;
	float gamenx5_,gameny5_;
	float xx,yy,xxs,yys;
	BOOL movef,endf,endff;
	float kasoku,gensoku,juryoku,limit;
	BOOL atari;

	int tamacnt;
	BOOL tamaf;
	int tamasu;

	int lockonsu;//,lockten,lockten2,lockten_,lockten2_;
	int motsu,motcnt;
	int lockon[1000];
	float tama[5000][10];//0:x 1:y 2:xx 3:yy 4:��� 5:shita 6:�^�C�}�[ 7:�A�j���^�C�}�[ 8:���b�N�I���̓G�ԍ� 9:�x��
	int mot[5000][7]; //0:x 1:y 2:no 3:no2 4:xx 5:yy 6:�x��
	float tekitama[5000][11];//0:x 1:y 2:xx 3:yy 4:�������� 5:�e��� 6:�x�� 7:�J�E���g 8:shita 9:alpha 10:�G�ԍ�
	float tekidata[1000][40];//0:x 1:y 2:xx 3:yy 4:shita 5:hp 6:�j�� 7:�G�ԍ� 8:��]�@9,10,11,12,13,14:�J�E���g�t���O 15:�f�B���C 16:�Ԋu
	DWORD tekidata2[1000][4];//0,1,2,3 �p�[�c�`��p
	float hit[3000][2];//0:��ւ̈ʒu 1:�t�F�[�h�̃��l
	int haikei[256*8+1][5]; //1:x 2:y 3:�� 4:�F
	int hitcnt;

	//�e�֘A,�V�X�e���֘A
	int jikitama[5];//��:-1�̎�������  0:�}�V���K�� 1:�z�[�~���O 2:�@�� 3:�~�T�C�� 4:2WAY
	int hitp[5][2];//���������A����������
	int timer,gameover,gameovercnt;
	int score,killsec;//���_
	int missioncnt,penaltycnt,emptyanime;//�~�b�V�����J�E���g
	float sabun;
	DWORD timen;
	float mousex_,mousey_;
	int stage;
	float mapx32,mapy32;
	int  mapx32_,mapy32_;

	//���Z�֘A
	int fps;
	int muteki;
	int mugen;
	int renmax;
	int stagemax;
	int urawaza;
	CString uraura;
	int clearf;
	int romrom;

	//MISSION xxx START COMPLETE�̕����̃A�j���֘A
	float mistart[800][5];//0:���S����̈ʒu 1:�� 2:�x�� 3:x 4:y
	char mistartc[800];
	int mistartc2[800];
	int mistartf,mistartsu,mistartx,mistarty;
	int mistartmode,mistartmode2,mistartmode3,mistartmodef;
	CString mistarts;
	float mistartA[800][5];//0:���S����̈ʒu 1:�� 2:�x�� 3:x 4:y
	char mistartcA[800];
	int mistartc2A[800];
	int mistartfA,mistartsuA,mistartxA,mistartyA;
	int mistartmodeA,mistartmode2A,mistartmode3A,mistartmodefA,mistartmode4A;
	CString mistartsA;
	int b1,b2,b3,b4,b5,m1,m2,m3;
	CString s1,s2,s3,s4,s5;

	replaysave rsv1,rsv2;
//	replaydata_ rdata_[60*60*99+2];
	replaydata rdata[60*60*99+2];
	int replay,scorerep,repf,repsf,repf_,repsf_;
	//�G,�e�֘A
	int tekitamasu,tekitamaf,tekitamano;
	int tekisu;
	//0:x 1:y 2:xx 3:yy 4:shita 5:hp 6:�j�� 7:�G�ԍ� 8,9,10,11,12,13:�J�E���g�t���O
	//8:��]���x 9:�O��̕� 10:�ړ����� 11:�ړ�����2 12:�W�����v��
	//14,15:�f�B���C 16:�Ԋu 17:�U��shita 18:��shita
	//19:���J�nshita 20;�I��shita 21:���J�nshita 22;�I��shita 23:���J�nshita 24;�I��shita 25:-- 26:���� 
	//27,28,29,30,31:���� 32:MaxHP 33:shita2 34:���(0:�G 1-4:�A�C�e��) 35:�x���\�� 36:�ǒ��J�E���g 37:�_�� 38:�U���p�[�c�̌���no
	int hp,_HP;
	int dame,damenow,damecnt,damecnt2;
	int dameanime,hpanime;
	int colof;

	int buki,bukia,bukib,bukic,bukid,bukif;



// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// �������ꂽ���b�Z�[�W �}�b�v�֐�
//protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MAINFRM_H__CB74BAD3_A130_4BC8_BC2C_571FF49FA2E7__INCLUDED_)
