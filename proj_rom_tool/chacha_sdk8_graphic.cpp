// chacha_sdk8_graphic.cpp: Cchacha_sdk8_graphic �N���X�̃C���v�������e�[�V����
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
//3D���C�u����
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
	dx=a1-a2;//���@�ƓG�̈ʒu���ϑ�
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
//	�֐���	�F	GrpVecCnv2d
//	�����T�v�F	�R�c���W�@���@�Q�c���W�ϊ�
//	����	�F	out_vec		�Q�c�X�N���[�����W�ϊ����ʁA�i�[��x�N�g��
//			�F	cnv_vec		���W�ϊ����x�N�g��
//			�F	world		���[���h���W�ϊ��s��
//	�o��	�F	
////////////////////////////////////////////////////////////////////////////
void Cchacha_sdk8_graphic::GrpVecCnv2d(D3DXVECTOR3* out_vec, D3DXVECTOR3* cnv_vec, D3DXMATRIX* world)
{
	D3DXVec3Project(out_vec,						// �Q�c�X�N���[�����W�ϊ����ʁA�i�[��x�N�g��
					cnv_vec,						// ���W�ϊ����x�N�g��
					&g_ViewPort,					// �r���[�|�[�g
					&matProj,						// �����ϊ��}�g���b�N�X
					&matView,						// �J�����}�g���b�N�X
					world);							// ���[���h���W�ϊ��s��
}
////////////////////////////////////////////////////////////////////////////
//	�֐���	�F	GrpSetProjMat
//	�����T�v�F	�����ϊ��s��̐ݒ�
//	����	�F	clip_near		Near �N���b�v�l
//			�F	clip_far		far	 �N���b�v�l
//	�o��	�F	
////////////////////////////////////////////////////////////////////////////
void Cchacha_sdk8_graphic::GrpSetProjMat(float clip_near, float clip_far)
{
	// �����ϊ��s��쐬�i����n�j
	D3DXMatrixPerspectiveFovLH(&matProj,							// �쐬�v���W�F�N�V�����}�g���b�N�X�ւ̃|�C���^
					D3DXToRadian(45.0),								// ����p
					640.0f / 480.0f,								// �A�X�y�N�g��i�c�A���䗦�j
					clip_near,										// Near �N���b�v
					clip_far);										// Far  �N���b�v
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);		// �����ϊ��s���ݒ�
}

////////////////////////////////////////////////////////////////////////////
//	�֐���	�F	GrpSetViewMat
//	�����T�v�F	�J�����s��̐ݒ�
//	����	�F	eye		�����_
//			�F	pos		�J�����ʒu
//	�o��	�F	
////////////////////////////////////////////////////////////////////////////
void Cchacha_sdk8_graphic::GrpSetViewMat(D3DXVECTOR3 eye, D3DXVECTOR3 pos)
{
	// �J�����s��쐬�i����n�j	
	D3DXMatrixLookAtLH(&matView,									// �쐬�r���[�}�g���b�N�X�̃|�C���^
					   &pos,										// �J�����ʒu
					   &eye,										// �J�����Î��_
					   &D3DXVECTOR3(0,1,0));						// ���[���h��Ԃŏ�������R�����x�N�g���w��	

	// �J�����s���ݒ�
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

/*
60fps�Ȃ炱��Ȋ����i1/60�b=50/3�~���b�j

DWORD t1, t2, t3, dt;�@�@// t1:���ݎ��� t2:1�t���[���O�̎��� t3:�덷

t1 = timeGetTime();
dt = (t1 - t2) * 3 + t3;
if(dt >= 50) {
�@�@t3 = dt % 50;
�@�@t2 = t1;
�@�@UpdateFrame();
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
	// D3D�I�u�W�F�N�g���쐬����
	if((m_pD3D = ::Direct3DCreate8(D3D_SDK_VERSION)) == NULL){
		::OutputDebugString("Error CreateD3D() : Direct3DCreate8()\n");
		return FALSE;
	}

	// Window���[�h�Ȃ�A���݂̃f�B�X�v���C���[�h���擾����
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


	// �f�o�C�X�̔\�͂��L�q����
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

	// D3D�f�o�C�X���쐬����
	// �܂��ATnL���n�[�h�E�F�A�Ɏ��
	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, devType, hInst,
								  D3DCREATE_HARDWARE_VERTEXPROCESSING,
								  &d3dpp, &m_pd3dDevice))){
		// �ʖڂȂ�\�t�g�E�F�A�ŏ�������
		if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, devType, hInst,
									  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									  &d3dpp, &m_pd3dDevice))){
			::OutputDebugString("Error CreateD3D() : CreateDevice()\n");
			return FALSE;
		}
	}

	// �o�b�N�o�b�t�@��Z�o�b�t�@���擾����
	if(FAILED(m_pd3dDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer))){
		::OutputDebugString("Error CreateD3D() : GetBackBuffer()\n");
		return FALSE;
	}
	if(FAILED(m_pd3dDevice->GetDepthStencilSurface(&m_pZBuffer))){
		::OutputDebugString("Error CreateD3D() : GetDepthStencilSurface()\n");
		return FALSE;
	}

	g_pd3dDevice = m_pd3dDevice;

	// �r���[�|�[�g�̐ݒ�
	g_ViewPort.X		= 0;
	g_ViewPort.Y		= 0;
	g_ViewPort.Width	= m_dwScreenW;
	g_ViewPort.Height	= m_dwScreenH;
	g_ViewPort.MinZ		= 0.0f;
	g_ViewPort.MaxZ		= 1.0f;
	g_pd3dDevice->SetViewport( &g_ViewPort );

	// ���[���h�s��
	static	float	angle = 0.0f;
	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 10.0f);
	D3DXMatrixRotationY(&matWorld03, DEGtoRAD(angle));
	angle += 1.0f;
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// �r���[�s��
	D3DXVECTOR3	pos(0.0f, 0.0f, -8.0f);
	D3DXVECTOR3	pnt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3	up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &pos, &pnt, &up);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// ���e�s��
	D3DXMatrixPerspectiveFovLH(&matProj, DEGtoRAD(45.0f), (float)m_dwScreenW / (float)m_dwScreenH, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);


	m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,	0x00000000, 1.0f, 0L);
	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	InitFont(&g_Font1, "�l�r �o�S�V�b�N", 8, 16);

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
				uc.Open("���@.bmp"), 0/*��*/, 0/*����*/, 0/*�~�b�v���x��*/,
				0, D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,D3DX_FILTER_LINEAR,D3DX_FILTER_LINEAR, 0xff1f3345/*31,51,69�J���[�L�[*/,
				NULL, NULL, &m_tex_jiki))){
		::OutputDebugString("D3DXCreateTextureFromFileEx() Error\n");
		return FALSE;
	}
	uc.Close();
	if(FAILED(D3DXCreateTextureFromFileEx(g_pd3dDevice,
				uc.Open("MAPCHIP.bmp"), 0/*��*/, 0/*����*/, 0/*�~�b�v���x��*/,
				0, D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,D3DX_FILTER_LINEAR,D3DX_FILTER_LINEAR, 0xff1f3345/*31,51,69�J���[�L�[*/,
//				0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0xff1f3345/*31,51,69�J���[�L�[*/,
				NULL, NULL, &m_tex_map))){
		::OutputDebugString("D3DXCreateTextureFromFileEx() Error\n");
		return FALSE;
	}
	if(FAILED(D3DXCreateTextureFromFileEx(g_pd3dDevice,
				uc.Open("enemy&item.bmp"), 0/*��*/, 0/*����*/, 0/*�~�b�v���x��*/,
				0, D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,D3DX_FILTER_LINEAR,D3DX_FILTER_LINEAR, 0xff1f3345/*31,51,69�J���[�L�[*/,
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
    // �o�b�N�E�o�b�t�@�̔w�i�F��ݒ肵�ăN���A
	if(g_pd3dDevice){
		g_pd3dDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
		// �V�[�����J�n
		g_pd3dDevice->BeginScene();
	}
}

void Cchacha_sdk8_graphic::End()
{
	if(g_pd3dDevice){
	    g_pd3dDevice->EndScene();
	    // �V�[���̕`��
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
	float PAD = 3.141592f / 180.0f; // ���W�A���̏�����
    D3DXMATRIX PosMatrix, RotateMatrix;
    float x1, y1;
    int i;

    // �|���S���̒��S�����߁A���_�ֈړ�������
    x1 = (v[2].x - v[0].x) / 2.0f + v[0].x;
    y1 = (v[3].y - v[1].y) / 2.0f + v[1].y;
    MoveMatrix(v, -x1, -y1, 0.0f);

    // �s��̏������i�P�ʍs�񐶐��j
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&RotateMatrix);

    // ��]�s��Ɋp�x��ݒ�
    D3DXMatrixRotationZ(&RotateMatrix, (float)Rotate * PAD);

    // ��]����
    for ( i=0 ; i<4 ; i++ ) {
        // ���݂̒��_���W���i�[
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        // ���Z
        PosMatrix *= RotateMatrix;
        // ���ʂ�߂�
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = PosMatrix._43;
    }

    // �|���S�������̈ʒu�֖߂�
    MoveMatrix(v, x1, y1, 0.0f);

}

void Cchacha_sdk8_graphic::MoveMatrix(CUSTOMVERTEX* v, float x, float y, float z)
{
    D3DXMATRIX PosMatrix, MoveMatrix;
    int i;

    // �s��̏������i�P�ʍs�񐶐��j
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&MoveMatrix);

    // �ړ��ʐݒ�
    D3DXMatrixTranslation(&MoveMatrix, x, y, z);

    // �ړ�����
    for ( i=0 ; i<4 ; i++ ) {
        // ���݂̒��_���W���i�[
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        // ���Z
        PosMatrix *= MoveMatrix;
        // ���ʂ�߂�
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = PosMatrix._43;
    }

}

void Cchacha_sdk8_graphic::Text(const char* str,DWORD x,DWORD y,D3DCOLOR c)
{
	DrawText8(g_Font1, (char*)str, x, y, c);
}
