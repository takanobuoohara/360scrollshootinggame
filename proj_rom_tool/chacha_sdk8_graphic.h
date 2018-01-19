// chacha_sdk8_graphic.h: Cchacha_sdk8_graphic クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHACHA_SDK8_GRAPHIC_H__5E490128_C83E_4CAE_A760_EE8DBD5D25EC__INCLUDED_)
#define AFX_CHACHA_SDK8_GRAPHIC_H__5E490128_C83E_4CAE_A760_EE8DBD5D25EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

extern	LPDIRECT3DDEVICE8	g_pd3dDevice;

class Cchacha_sdk8_graphic  
{
public:

	LPDIRECT3D8			m_pD3D;				// Direct3DObject
	LPDIRECT3DDEVICE8	m_pd3dDevice;		// D3Dデバイス
	LPDIRECT3DSURFACE8	m_pBackBuffer;		// バックバッファ
	LPDIRECT3DSURFACE8	m_pZBuffer;			// Zバッファ
	ID3DXFont* g_Font1;   // フォントオブジェクト
LPDIRECT3DTEXTURE8 m_tex_map;
LPDIRECT3DTEXTURE8 m_tex_jiki;
LPDIRECT3DTEXTURE8 m_tex_teki;

	DWORD	m_dwScreenW, m_dwScreenH;		// 画面の幅と高さ

	BOOL	m_bHAL;							// HAL使用スイッチ
	BOOL	m_bWindow;						// WindowModeスイッチ
	BOOL	m_bActive;						// アクティブスイッチ

	// D3D初期化
	virtual	BOOL	CreateD3D(HWND hInst);
	BOOL InitGraphic();
	BOOL InitSprite();
	void PartDraw(int x,int y,int cx,int cy,float rad=0,BOOL radf=FALSE,BOOL a=FALSE);
	void JikiDraw(int x,int y,int cx,int cy,float rad=0,BOOL radf=FALSE,BOOL a=FALSE);
	void TekiDraw(int x,int y,int cx,int cy,float rad,BOOL radf=0,BOOL a=FALSE,DWORD alpha=255);
	void Begin();
	void End();
	void DoEvent();

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

	void init_sqrt_fast();
	float sqrt_fast(float x);
	void GrpSetViewMat(D3DXVECTOR3 eye, D3DXVECTOR3 pos);
	void GrpSetProjMat(float clip_near, float clip_far);
	void GrpVecCnv2d(D3DXVECTOR3* out_vec, D3DXVECTOR3* cnv_vec, D3DXMATRIX* world);

	D3DXMATRIX	matWorld, matWorld03;
	D3DXMATRIX	matProj;
	D3DXMATRIX	matView;
	D3DVIEWPORT8 g_ViewPort;

public:
	// 初期化関数
	virtual	inline	void	CleanUp()
	{
		RELEASE(m_tex_jiki);
		RELEASE(m_tex_teki);
		RELEASE(m_tex_map);
		RELEASE(g_Font1);
		RELEASE(m_pZBuffer);
		RELEASE(m_pBackBuffer);
		RELEASE(m_pd3dDevice);
		RELEASE(m_pD3D);
		g_pd3dDevice = NULL;
		m_dwScreenW = 640;
		m_dwScreenH = 480;
		m_bHAL = TRUE;
		m_bWindow = TRUE;
		m_bActive = FALSE;
	}

	// コンストラクタ・デストラクタ
	Cchacha_sdk8_graphic()
	{
		m_pZBuffer = NULL;
		m_pBackBuffer = NULL;
		m_pd3dDevice = NULL;
		g_Font1=NULL;
		m_pD3D = NULL;
		m_dwScreenW = 640;
		m_dwScreenH = 480;
		m_bHAL = TRUE;
		m_bWindow = TRUE;
		m_bActive = FALSE;
	}
	virtual	~Cchacha_sdk8_graphic()
	{
		CleanUp();
	}


};

#endif // !defined(AFX_CHACHA_SDK8_GRAPHIC_H__5E490128_C83E_4CAE_A760_EE8DBD5D25EC__INCLUDED_)
