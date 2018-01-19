// chacha_sdk8_graphic.cpp: Cchacha_sdk8_graphic クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "proj_rom.h"
#include "chacha_sdk8_graphic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

LPDIRECT3DDEVICE8	g_pd3dDevice = NULL;

float sintb[32000*2];
float costb[32000*2];
float acostb[30000];
#define BITLEN 8
#define SIZE (1<<BITLEN)
static float idxTbl[256];
static float sqrtTbl1[SIZE];
static float sqrtTbl2[SIZE];

//-----------------------------------------------------------------------------
//3Dライブラリ
//-----------------------------------------------------------------------------
inline VOID D3DUtil_SetIdentityMatrix( D3DMATRIX& m )
{
    m._12 = m._13 = m._14 = m._21 = m._23 = m._24 = 0.0f;
    m._31 = m._32 = m._34 = m._41 = m._42 = m._43 = 0.0f;
    m._11 = m._22 = m._33 = m._44 = 1.0f;
}

VOID Cchacha_sdk8_graphic::D3DUtil_SetRotateXMatrix( D3DMATRIX& mat, FLOAT fRads )
{
    D3DUtil_SetIdentityMatrix( mat );
    mat._22 =  Getcos( fRads );
    mat._23 =  Getsin( fRads );
    mat._32 = -Getsin( fRads );
    mat._33 =  Getcos( fRads );
}

VOID Cchacha_sdk8_graphic::D3DUtil_SetRotateYMatrix( D3DMATRIX& mat, FLOAT fRads )
{
    D3DUtil_SetIdentityMatrix( mat );
    mat._11 =  Getcos( fRads );
    mat._13 = -Getsin( fRads );
    mat._31 =  Getsin( fRads );
    mat._33 =  Getcos( fRads );
}

VOID Cchacha_sdk8_graphic::D3DUtil_SetRotateZMatrix( D3DMATRIX& mat, FLOAT fRads )
{
    D3DUtil_SetIdentityMatrix( mat );
    mat._11  =  Getcos( fRads );
    mat._12  =  Getsin( fRads );
    mat._21  = -Getsin( fRads );
    mat._22  =  Getcos( fRads );
}

VOID Cchacha_sdk8_graphic::D3DMath_MatrixMultiply( D3DMATRIX& q, D3DMATRIX& a, D3DMATRIX& b )
{
    FLOAT* pA = (FLOAT*)&a;
    FLOAT* pB = (FLOAT*)&b;
    FLOAT  pM[16];

    ZeroMemory( pM, sizeof(D3DMATRIX) );

    for( WORD i=0; i<4; i++ ) 
        for( WORD j=0; j<4; j++ ) 
            for( WORD k=0; k<4; k++ ) 
                pM[4*i+j] += pA[4*k+j] * pB[4*i+k];

    memcpy( &q, pM, sizeof(D3DMATRIX) );
}

float Cchacha_sdk8_graphic::Getsin(float a)
{
	int aa=(int)(a*10000.0f);
	if(aa>63999)aa=63999;
	return sintb[aa];
}

float Cchacha_sdk8_graphic::Getcos(float a)
{
	int aa=(int)(a*10000.0f);
	if(aa>63999)aa=63999;
	return costb[aa];
}

float Cchacha_sdk8_graphic::GetShita(float a1,float a2,float b1,float b2)
{
//	return get_l2d(a1,a2,b1,b2);
	float dx,dy,st;
	dx=a1-a2;//自機と敵の位置を観測
	dy=b1-b2;
	st=Getacos(dy/sqrt_fast(dx*dx+dy*dy))*PIw180;
	if(dx<0.0f) st=180.0f+(180.0f-st);
	return st;
}
float Cchacha_sdk8_graphic::Getacos(float a)
{
	int aa=(int)((a+1.0f)*10000.0f);
	if(aa>29999)aa=29999;
	return acostb[aa];

}

void Cchacha_sdk8_graphic::init_sqrt_fast(void){
	int i;
	for(i = 0; i < 256; i++){
		idxTbl[i] = (float)pow(2, (i - 127) * 0.5);
	}
	for(i = 0; i < SIZE; i++){
		sqrtTbl1[i] = (float)sqrt(1 + i / (double)SIZE);
	}
	for(i = 0; i < SIZE-1; i++){
		sqrtTbl2[i] = (sqrtTbl1[i+1] - sqrtTbl1[i]) / (float)SIZE;
	}
	sqrtTbl2[SIZE-1] = ((float)sqrt(2) - sqrtTbl1[SIZE-1]) / (float)SIZE;
}

float Cchacha_sdk8_graphic::sqrt_fast(float x){
	int s, t, u;
	s = t = u = *(int *)&x;
	s >>= 23;
	t &= ((1<<23)-1);
	u &= ((1<<(23-BITLEN))-1);
	t >>= (23-BITLEN);
	u >>= (23-BITLEN*2);
	return idxTbl[s] * (sqrtTbl1[t] + sqrtTbl2[t] * u);
}

////////////////////////////////////////////////////////////////////////////
//	関数名	：	GrpVecCnv2d
//	処理概要：	３Ｄ座標　→　２Ｄ座標変換
//	入力	：	out_vec		２Ｄスクリーン座標変換結果、格納先ベクトル
//			：	cnv_vec		座標変換元ベクトル
//			：	world		ワールド座標変換行列
//	出力	：	
////////////////////////////////////////////////////////////////////////////
void Cchacha_sdk8_graphic::GrpVecCnv2d(D3DXVECTOR3* out_vec, D3DXVECTOR3* cnv_vec, D3DXMATRIX* world)
{
	D3DXVec3Project(out_vec,						// ２Ｄスクリーン座標変換結果、格納先ベクトル
					cnv_vec,						// 座標変換元ベクトル
					&g_ViewPort,					// ビューポート
					&matProj,						// 透視変換マトリックス
					&matView,						// カメラマトリックス
					world);							// ワールド座標変換行列
}
////////////////////////////////////////////////////////////////////////////
//	関数名	：	GrpSetProjMat
//	処理概要：	透視変換行列の設定
//	入力	：	clip_near		Near クリップ値
//			：	clip_far		far	 クリップ値
//	出力	：	
////////////////////////////////////////////////////////////////////////////
void Cchacha_sdk8_graphic::GrpSetProjMat(float clip_near, float clip_far)
{
	// 透視変換行列作成（左手系）
	D3DXMatrixPerspectiveFovLH(&matProj,							// 作成プロジェクションマトリックスへのポインタ
					D3DXToRadian(45.0),								// 視野角
					640.0f / 480.0f,								// アスペクト比（縦、横比率）
					clip_near,										// Near クリップ
					clip_far);										// Far  クリップ
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);		// 透視変換行列を設定
}

////////////////////////////////////////////////////////////////////////////
//	関数名	：	GrpSetViewMat
//	処理概要：	カメラ行列の設定
//	入力	：	eye		注視点
//			：	pos		カメラ位置
//	出力	：	
////////////////////////////////////////////////////////////////////////////
void Cchacha_sdk8_graphic::GrpSetViewMat(D3DXVECTOR3 eye, D3DXVECTOR3 pos)
{
	// カメラ行列作成（左手系）	
	D3DXMatrixLookAtLH(&matView,									// 作成ビューマトリックスのポインタ
					   &pos,										// カメラ位置
					   &eye,										// カメラ凝視点
					   &D3DXVECTOR3(0,1,0));						// ワールド空間で上を示す３次元ベクトル指定	

	// カメラ行列を設定
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

/*
60fpsならこんな感じ（1/60秒=50/3ミリ秒）

DWORD t1, t2, t3, dt;　　// t1:現在時刻 t2:1フレーム前の時刻 t3:誤差

t1 = timeGetTime();
dt = (t1 - t2) * 3 + t3;
if(dt >= 50) {
　　t3 = dt % 50;
　　t2 = t1;
　　UpdateFrame();
}
*/
BOOL Cchacha_sdk8_graphic::CreateD3D(HWND hInst)
{
	m_tex_jiki=m_tex_map=m_tex_teki=NULL;
	D3DPRESENT_PARAMETERS	d3dpp; 
	init_sqrt_fast();
	int iii=0;
	for(float s=0.0f;s<=6.3f;s+=0.0001f)
	{
		sintb[iii]=(float)sin(s);
		costb[iii]=(float)cos(s);
		iii++;
	}
	iii=0;
	for( s=-1.0f;s<=1.0f;s+=0.0001f)
	{
		acostb[iii]=(float)acos(s);iii++;
	}
	// D3Dオブジェクトを作成する
	if((m_pD3D = ::Direct3DCreate8(D3D_SDK_VERSION)) == NULL){
		::OutputDebugString("Error CreateD3D() : Direct3DCreate8()\n");
		return FALSE;
	}

	// Windowモードなら、現在のディスプレイモードを取得する
	D3DDISPLAYMODE	d3ddm;
	if(m_bWindow){
		if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))){
			::OutputDebugString("Error CreateD3D() : GetAdapterDisplayMode()\n");
			return FALSE;
		}
	}
	else{
		d3ddm.Format = D3DFMT_A8R8G8B8;
	}


	// デバイスの能力を記述する
	::ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = m_dwScreenW;
	d3dpp.BackBufferHeight = m_dwScreenH;
	d3dpp.BackBufferFormat = d3ddm.Format;
    d3dpp.BackBufferCount = 1;
	d3dpp.Windowed = m_bWindow;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	D3DDEVTYPE	devType = (m_bHAL == TRUE ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF);

	// D3Dデバイスを作成する
	// まず、TnLをハードウェアに取る
	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, devType, hInst,
								  D3DCREATE_HARDWARE_VERTEXPROCESSING,
								  &d3dpp, &m_pd3dDevice))){
		// 駄目ならソフトウェアで処理する
		if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, devType, hInst,
									  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									  &d3dpp, &m_pd3dDevice))){
			::OutputDebugString("Error CreateD3D() : CreateDevice()\n");
			return FALSE;
		}
	}

	// バックバッファとZバッファを取得する
	if(FAILED(m_pd3dDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer))){
		::OutputDebugString("Error CreateD3D() : GetBackBuffer()\n");
		return FALSE;
	}
	if(FAILED(m_pd3dDevice->GetDepthStencilSurface(&m_pZBuffer))){
		::OutputDebugString("Error CreateD3D() : GetDepthStencilSurface()\n");
		return FALSE;
	}

	g_pd3dDevice = m_pd3dDevice;

	// ビューポートの設定
	g_ViewPort.X		= 0;
	g_ViewPort.Y		= 0;
	g_ViewPort.Width	= m_dwScreenW;
	g_ViewPort.Height	= m_dwScreenH;
	g_ViewPort.MinZ		= 0.0f;
	g_ViewPort.MaxZ		= 1.0f;
	g_pd3dDevice->SetViewport( &g_ViewPort );

	// ワールド行列
	static	float	angle = 0.0f;
	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 10.0f);
	D3DXMatrixRotationY(&matWorld03, DEGtoRAD(angle));
	angle += 1.0f;
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// ビュー行列
	D3DXVECTOR3	pos(0.0f, 0.0f, -8.0f);
	D3DXVECTOR3	pnt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3	up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &pos, &pnt, &up);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// 投影行列
	D3DXMatrixPerspectiveFovLH(&matProj, DEGtoRAD(45.0f), (float)m_dwScreenW / (float)m_dwScreenH, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);


	m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,	0x00000000, 1.0f, 0L);
	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	InitFont(&g_Font1, "ＭＳ Ｐゴシック", 8, 16);

	return TRUE;

}

void Cchacha_sdk8_graphic::DoEvent()
{
	MSG msg;
	for(;;)
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg); 
		}else return;
}

BOOL Cchacha_sdk8_graphic::InitGraphic()
{
	RELEASE(m_tex_jiki);
	RELEASE(m_tex_teki);
	RELEASE(m_tex_map);
	UnCompress uc;
	if(FAILED(D3DXCreateTextureFromFileEx(g_pd3dDevice,
				uc.Open("自機.bmp"), 0/*幅*/, 0/*高さ*/, 0/*ミップレベル*/,
				0, D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,D3DX_FILTER_LINEAR,D3DX_FILTER_LINEAR, 0xff1f3345/*31,51,69カラーキー*/,
				NULL, NULL, &m_tex_jiki))){
		::OutputDebugString("D3DXCreateTextureFromFileEx() Error\n");
		return FALSE;
	}
	uc.Close();
	if(FAILED(D3DXCreateTextureFromFileEx(g_pd3dDevice,
				uc.Open("MAPCHIP.bmp"), 0/*幅*/, 0/*高さ*/, 0/*ミップレベル*/,
				0, D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,D3DX_FILTER_LINEAR,D3DX_FILTER_LINEAR, 0xff1f3345/*31,51,69カラーキー*/,
//				0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0xff1f3345/*31,51,69カラーキー*/,
				NULL, NULL, &m_tex_map))){
		::OutputDebugString("D3DXCreateTextureFromFileEx() Error\n");
		return FALSE;
	}
	if(FAILED(D3DXCreateTextureFromFileEx(g_pd3dDevice,
				uc.Open("enemy&item.bmp"), 0/*幅*/, 0/*高さ*/, 0/*ミップレベル*/,
				0, D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,D3DX_FILTER_LINEAR,D3DX_FILTER_LINEAR, 0xff1f3345/*31,51,69カラーキー*/,
				NULL, NULL, &m_tex_teki))){
		::OutputDebugString("D3DXCreateTextureFromFileEx() Error\n");
		return FALSE;
	}
	uc.Close();
	return TRUE;
}

BOOL Cchacha_sdk8_graphic::InitSprite()
{
	return TRUE;
}

void Cchacha_sdk8_graphic::Begin()
{
    // バック・バッファの背景色を設定してクリア
	if(g_pd3dDevice){
		g_pd3dDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
		// シーンを開始
		g_pd3dDevice->BeginScene();
	}
}

void Cchacha_sdk8_graphic::End()
{
	if(g_pd3dDevice){
	    g_pd3dDevice->EndScene();
	    // シーンの描画
	    g_pd3dDevice->Present(NULL,NULL,NULL,NULL);
	}
}


void Cchacha_sdk8_graphic::PartDraw(int x,int y,int cx,int cy,float rad,BOOL radf,BOOL a)
{
	if(g_pd3dDevice==NULL) return;

	float xd=(float)cx,xd2;
	float yd=(float)cy,yd2;
	if(a){
		xd2=1.0f;
		xd =0.0f;
		yd2=1.0f;
		yd =0.0f;
	}else{
		xd2=((float)xd+(float)31.0)/(float)512.0;
		xd = (float)xd             /(float)512.0;
		yd2=((float)yd+(float)31.0)/(float)512.0;
		yd = (float)yd             /(float)512.0;
	}
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, true);
    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if(!a){
		CUSTOMVERTEX g_pVerteces [] = {
			{ x-8.0f, y-8.0f, 0.0f, 1.0f, 0xFFFFFFFF, xd , yd },
			{ x+8.0f, y-8.0f, 0.0f, 1.0f, 0xFFFFFFFF, xd2, yd },
			{ x+8.0f, y+8.0f, 0.0f, 1.0f, 0xFFFFFFFF, xd2, yd2},
			{ x-8.0f, y+8.0f, 0.0f, 1.0f, 0xFFFFFFFF, xd , yd2}
		};

		if(radf==TRUE){
			Rote(g_pVerteces,rad);
		}
		g_pd3dDevice->SetTexture( 0, m_tex_map );
		g_pd3dDevice->SetVertexShader( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, g_pVerteces, sizeof CUSTOMVERTEX );
	}else{
		CUSTOMVERTEX g_pVerteces [] = {
			{ x+0.0f  , y+0.0f  , 0.0f, 1.0f, 0xFFFFFFFF, xd , yd },
			{ x+255.0f, y+0.0f  , 0.0f, 1.0f, 0xFFFFFFFF, xd2, yd },
			{ x+255.0f, y+255.0f, 0.0f, 1.0f, 0xFFFFFFFF, xd2, yd2},
			{ x+0.0f  , y+255.0f, 0.0f, 1.0f, 0xFFFFFFFF, xd , yd2}
		};

		if(radf==TRUE){
			Rote(g_pVerteces,rad);
		}
		g_pd3dDevice->SetTexture( 0, m_tex_map );
		g_pd3dDevice->SetVertexShader( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, g_pVerteces, sizeof CUSTOMVERTEX );
	}
}

void Cchacha_sdk8_graphic::TekiDraw(int x,int y,int cx,int cy,float rad,BOOL radf,BOOL a,DWORD alpha)
{
	if(g_pd3dDevice==NULL) return;

	float xd=(float)cx,xd2;
	float yd=(float)cy,yd2;
	xd2=((float)xd+(float)31.0)/(float)256.0;
	xd = (float)xd             /(float)256.0;
	yd2=((float)yd+(float)31.0)/(float)256.0;
	yd = (float)yd             /(float)256.0;

    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, true);
    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
   g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	if(a){
		CUSTOMVERTEX g_pVerteces [] = {
			{ x-8.0f, y-8.0f, 0.0f, 1.0f, 0x00FFFFFF|alpha<<24, xd , yd },
			{ x+8.0f, y-8.0f, 0.0f, 1.0f, 0x00FFFFFF|alpha<<24, xd2, yd },
			{ x+8.0f, y+8.0f, 0.0f, 1.0f, 0x00FFFFFF|alpha<<24, xd2, yd2},
			{ x-8.0f, y+8.0f, 0.0f, 1.0f, 0x00FFFFFF|alpha<<24, xd , yd2}
		};

		if(radf==TRUE){
			Rote(g_pVerteces,rad);
		}
		g_pd3dDevice->SetTexture( 0, m_tex_teki );
		g_pd3dDevice->SetVertexShader( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, g_pVerteces, sizeof CUSTOMVERTEX );
	}else{
		CUSTOMVERTEX g_pVerteces [] = {
			{ x-16.0f, y-16.0f, 0.0f, 1.0f, 0x00FFFFFF|alpha<<24, xd , yd },
			{ x+16.0f, y-16.0f, 0.0f, 1.0f, 0x00FFFFFF|alpha<<24, xd2, yd },
			{ x+16.0f, y+16.0f, 0.0f, 1.0f, 0x00FFFFFF|alpha<<24, xd2, yd2},
			{ x-16.0f, y+16.0f, 0.0f, 1.0f, 0x00FFFFFF|alpha<<24, xd , yd2}
		};

		if(radf==TRUE){
			Rote(g_pVerteces,rad);
		}
		g_pd3dDevice->SetTexture( 0, m_tex_teki );
		g_pd3dDevice->SetVertexShader( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, g_pVerteces, sizeof CUSTOMVERTEX );
	}
}


void Cchacha_sdk8_graphic::JikiDraw(int x,int y,int cx,int cy,float rad,BOOL radf,BOOL a)
{
	if(g_pd3dDevice==NULL) return;

	float xd=(float)cx,xd2;
	float yd=(float)cy,yd2;
	if(a){
		xd2=1.0f;
		xd =0.0f;
		yd2=1.0f;
		yd =0.0f;
	}else{
		xd2=((float)xd+(float)31.0)/(float)256.0;
		xd = (float)xd             /(float)256.0;
		yd2=((float)yd+(float)31.0)/(float)256.0;
		yd = (float)yd             /(float)256.0;
	}
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, true);
    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if(!a){
		CUSTOMVERTEX g_pVerteces [] = {
			{ x-8.0f, y-8.0f, 0.0f, 1.0f, 0xFFFFFFFF, xd , yd },
			{ x+8.0f, y-8.0f, 0.0f, 1.0f, 0xFFFFFFFF, xd2, yd },
			{ x+8.0f, y+8.0f, 0.0f, 1.0f, 0xFFFFFFFF, xd2, yd2},
			{ x-8.0f, y+8.0f, 0.0f, 1.0f, 0xFFFFFFFF, xd , yd2}
		};

		if(radf==TRUE){
			Rote(g_pVerteces,rad);
		}
		g_pd3dDevice->SetTexture( 0, m_tex_jiki );
		g_pd3dDevice->SetVertexShader( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, g_pVerteces, sizeof CUSTOMVERTEX );
	}else{
		CUSTOMVERTEX g_pVerteces [] = {
			{ x+0.0f  , y+0.0f  , 0.0f, 1.0f, 0xFFFFFFFF, xd , yd },
			{ x+511.0f, y+0.0f  , 0.0f, 1.0f, 0xFFFFFFFF, xd2, yd },
			{ x+511.0f, y+511.0f, 0.0f, 1.0f, 0xFFFFFFFF, xd2, yd2},
			{ x+0.0f  , y+511.0f, 0.0f, 1.0f, 0xFFFFFFFF, xd , yd2}
		};

		if(radf==TRUE){
			Rote(g_pVerteces,rad);
		}
		g_pd3dDevice->SetTexture( 0, m_tex_jiki );
		g_pd3dDevice->SetVertexShader( D3DFVF_CUSTOMVERTEX );
		g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, g_pVerteces, sizeof CUSTOMVERTEX );
	}
}



void Cchacha_sdk8_graphic::Rote(CUSTOMVERTEX* v, float Rotate)
{
	float PAD = 3.141592f / 180.0f; // ラジアンの初期化
    D3DXMATRIX PosMatrix, RotateMatrix;
    float x1, y1;
    int i;

    // ポリゴンの中心を求め、原点へ移動させる
    x1 = (v[2].x - v[0].x) / 2.0f + v[0].x;
    y1 = (v[3].y - v[1].y) / 2.0f + v[1].y;
    MoveMatrix(v, -x1, -y1, 0.0f);

    // 行列の初期化（単位行列生成）
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&RotateMatrix);

    // 回転行列に角度を設定
    D3DXMatrixRotationZ(&RotateMatrix, (float)Rotate * PAD);

    // 回転処理
    for ( i=0 ; i<4 ; i++ ) {
        // 現在の頂点座標を格納
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        // 演算
        PosMatrix *= RotateMatrix;
        // 結果を戻す
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = PosMatrix._43;
    }

    // ポリゴンを元の位置へ戻す
    MoveMatrix(v, x1, y1, 0.0f);

}

void Cchacha_sdk8_graphic::MoveMatrix(CUSTOMVERTEX* v, float x, float y, float z)
{
    D3DXMATRIX PosMatrix, MoveMatrix;
    int i;

    // 行列の初期化（単位行列生成）
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&MoveMatrix);

    // 移動量設定
    D3DXMatrixTranslation(&MoveMatrix, x, y, z);

    // 移動処理
    for ( i=0 ; i<4 ; i++ ) {
        // 現在の頂点座標を格納
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        // 演算
        PosMatrix *= MoveMatrix;
        // 結果を戻す
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = PosMatrix._43;
    }

}

void Cchacha_sdk8_graphic::Text(const char* str,DWORD x,DWORD y,D3DCOLOR c)
{
	DrawText8(g_Font1, (char*)str, x, y, c);
}
