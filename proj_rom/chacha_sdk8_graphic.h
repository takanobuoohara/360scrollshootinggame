// chacha_sdk8_graphic.h: Cchacha_sdk8_graphic �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHACHA_SDK8_GRAPHIC_H__5E490128_C83E_4CAE_A760_EE8DBD5D25EC__INCLUDED_)
#define AFX_CHACHA_SDK8_GRAPHIC_H__5E490128_C83E_4CAE_A760_EE8DBD5D25EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern	LPDIRECT3DDEVICE8	g_pd3dDevice;
extern DWORD _ff1,_ff2;
class Cchacha_sdk8_graphic  
{
public:
	// D3D������
	BOOL CreateD3D(HWND hInst);
	BOOL InitGraphic(int);
	BOOL InitSprite(CString s);
	BOOL LoadWorkTex(int no);//0:���S�A1:�^�C�g�� 2:�G���f�B���O
	BOOL Loadbmp(CString f,LPDIRECT3DTEXTURE8& ff);
	void BackCopy(BOOL c=FALSE);
	void TekiDraw(float x,float y,int cx,int cy,float rad,BOOL radf=FALSE,DWORD alpha=255,int l=16);
	void JikiDraw(float x,float y,int cx,int cy,float rad,BOOL radf=FALSE,DWORD alpha=255,int l=16);
	void JikiDraw2(float x,float y,int cx,int cy,float rad,BOOL radf=FALSE,DWORD alpha=255,int l=16);
	void PartDraw(float x,float y,int cx,int cy,float rad,BOOL radf=FALSE,DWORD alpha=255);
	void PartDrawA(float x,float y,int cx,int cy,float lx,float ly,int mp=0,DWORD alpha=255,float rad=0,BOOL radf=FALSE,float x1=0,float y1=0);
	void Begin(BOOL a=TRUE);
	void End(BOOL a=TRUE);
	void DoEvent();
	void Screen();
	BOOL Reset();

	void Moji32(const char* str,int mojisu,int syu,float x,float y,BOOL alpha=FALSE,BOOL alpha2=255,int kaite=-1);
	void Moji16(const char* str,int mojisu,int syu,float x,float y,BOOL alpha=FALSE,BOOL alpha2=255);

	BOOL DoEventChk(BOOL& endf);

	VOID D3DUtil_SetRotateXMatrix( D3DMATRIX& mat, FLOAT fRads );
	VOID D3DUtil_SetRotateYMatrix( D3DMATRIX& mat, FLOAT fRads );
	VOID D3DUtil_SetRotateZMatrix( D3DMATRIX& mat, FLOAT fRads );
	VOID D3DMath_MatrixMultiply( D3DMATRIX& q, D3DMATRIX& a, D3DMATRIX& b );
	float Getsin(float a);
	float Getcos(float a);
	float GetShita(float a1,float a2,float b1,float b2);
	float Getacos(float a);
	void Rote(CUSTOMVERTEX* v, float Rotate);
	void MoveMatrix(CUSTOMVERTEX* v, float x, float y, float z);
	void Text(const char* str,DWORD x,DWORD y,D3DCOLOR c);
	void Fade(DWORD f);

	void init_sqrt_fast();
	float sqrt_fast(float x);
	void GrpSetViewMat(D3DXVECTOR3 eye, D3DXVECTOR3 pos);
	void GrpSetProjMat(float clip_near, float clip_far);
	void GrpVecCnv2d(D3DXVECTOR3* out_vec, D3DXVECTOR3* cnv_vec, D3DXMATRIX* world);
	BOOL TriangleIntersect(D3DXVECTOR3 Orig, D3DXVECTOR3 dir, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2, float *pRetT, float *pRetU, float *pRetV);

	void GetInput();
	void timing1(WORD a,BOOL b = FALSE,BOOL c = TRUE);
	DWORD Timing64(DWORD& Frate,BOOL bl=FALSE);

	LPDIRECT3D8			m_pD3D;				// Direct3DObject
	LPDIRECT3DDEVICE8	m_pd3dDevice;		// D3D�f�o�C�X
	LPDIRECT3DSURFACE8	m_pBackBuffer;		// �o�b�N�o�b�t�@
	LPDIRECT3DSURFACE8	m_pZBuffer;			// Z�o�b�t�@
	LPDIRECT3DSURFACE8	m_pWorkBuffer;
	LPDIRECT3DSURFACE8  m_bs;

	LPDIRECT3DTEXTURE8 m_tex_map;
	LPDIRECT3DTEXTURE8 m_tex_jiki;
	LPDIRECT3DTEXTURE8 m_tex_mini;
	LPDIRECT3DTEXTURE8 m_tex_bar;

	LPDIRECT3DTEXTURE8 m_tex_teki;

	LPDIRECT3DTEXTURE8 m_tex_moji;
	LPDIRECT3DTEXTURE8 m_tex_moji2;
	
	LPDIRECT3DTEXTURE8 m_tex_map0;
	LPDIRECT3DTEXTURE8 m_tex_map1;
	LPDIRECT3DTEXTURE8 m_tex_map2;
	LPDIRECT3DTEXTURE8 m_tex_map3;

	//�^�C�g���A���S�A�G���f�B���O�p 512x512�e�N�X�`��
	LPDIRECT3DTEXTURE8 m_tex_work[10];

	int mooo;
	BOOL runf;
	
	float WHx,WHy;
	DWORD	m_dwScreenW, m_dwScreenH;		// ��ʂ̕��ƍ���
	D3DPRESENT_PARAMETERS	d3dpp; 
	D3DDISPLAYMODE	d3ddm;
	HWND hwnd;
	BOOL	m_bHAL;							// HAL�g�p�X�C�b�`
	BOOL	m_bWindow;						// WindowMode�X�C�b�`
	BOOL	m_bActive;						// �A�N�e�B�u�X�C�b�`
	int maxx,maxy;
	DWORD cl;
	POINT po;
	int stage;
	int playmode;
	Cchacha_sdk8_audio* chacha_a;
	Cchacha_sdk8_Input* chacha_i;
	MSTATE MState;


	D3DXMATRIX	matWorld, matWorld03;
	D3DXMATRIX	matProj;
	D3DXMATRIX	matView;
	D3DVIEWPORT8 g_ViewPort;
	DWORD t1,t2,t3,t4,t5,dt;// t1:���ݎ��� t2:1�t���[���O�̎��� t3:�덷
	BOOL qpf;
	DWORD ntime,rate;
	float	fps;
	DWORD	fpp;
	DWORD fpstiming;

	DWORD ff[2];

	int noo,kaiten;
protected:
	DWORD Timing(DWORD& Frate,BOOL bl = TRUE);
	DWORD timing(BOOL t);

public:
	// �������֐�
	virtual	inline	void	CleanUp()
	{
		for(int i=0;i<10;i++){
			RELEASE(m_tex_work[i]);
		}
		RELEASE(m_tex_moji);
		RELEASE(m_tex_moji2);
		RELEASE(m_tex_teki);
		RELEASE(m_tex_jiki);
		RELEASE(m_tex_map);
		RELEASE(m_tex_map0);
		RELEASE(m_tex_map1);
		RELEASE(m_tex_map2);
		RELEASE(m_tex_map3);
		RELEASE(m_tex_bar);
		RELEASE(m_tex_mini);
		RELEASE(m_bs);
		RELEASE(m_pWorkBuffer);
		RELEASE(m_pZBuffer);
		RELEASE(m_pBackBuffer);
		RELEASE(m_pd3dDevice);
		RELEASE(m_pD3D);
		g_pd3dDevice = NULL;
		m_dwScreenW = 640;
		m_dwScreenH = 480;
		m_bHAL = TRUE;
		m_bWindow = FALSE;
		m_bActive = FALSE;
	}

	// �R���X�g���N�^�E�f�X�g���N�^
	Cchacha_sdk8_graphic()
	{
		for(int i=0;i<10;i++) m_tex_work[i]=NULL;
		m_tex_moji=m_tex_moji2=m_tex_teki=m_tex_jiki=m_tex_map=m_tex_map0=m_tex_map1=m_tex_map2=m_tex_map3=m_tex_bar=m_tex_mini=NULL;
		m_pZBuffer = m_pWorkBuffer=m_bs= NULL;
		m_pBackBuffer = NULL;
		m_pd3dDevice = NULL;
		m_pD3D = NULL;
		m_dwScreenW = 640;
		m_dwScreenH = 480;
		m_bHAL = TRUE;
		m_bWindow = FALSE;
		m_bActive = FALSE;
		t3=0;
		noo=-1;
		kaiten=0;
	}
	virtual	~Cchacha_sdk8_graphic()
	{
		CleanUp();
	}


};

#endif // !defined(AFX_CHACHA_SDK8_GRAPHIC_H__5E490128_C83E_4CAE_A760_EE8DBD5D25EC__INCLUDED_)
