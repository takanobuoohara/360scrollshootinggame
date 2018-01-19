// chacha_sdk8_graphic.cpp: Cchacha_sdk8_graphic �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "proj_rom.h"
#include "chacha_sdk8_graphic.h"
#include <stdlib.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

LPDIRECT3DDEVICE8	g_pd3dDevice = NULL;

float sintb[32010*2];
float costb[32010*2];
float acostb[20010];
#define BITLEN 8
#define SIZE (1<<BITLEN)
float idxTbl[256];
float sqrtTbl1[SIZE];
float sqrtTbl2[SIZE];

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
	int aa=ftol(a*10000.0f);
	if(aa<0)aa=0;
	if(aa>63999)aa=63999;
	return sintb[aa];
}

float Cchacha_sdk8_graphic::Getcos(float a)
{
	int aa=ftol(a*10000.0f);
	if(aa<0)aa=0;
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
	if(dx<=0.0f) st=180.0f+(180.0f-st);
	return st;
}
float Cchacha_sdk8_graphic::Getacos(float a)
{
	int aa=ftol((a+1.0f)*10000.0f);
	if(aa<0)aa=0;
	if(aa>19999)aa=19999;
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
	if(x>0&&x<0.0001f) x=0;
	if(x<0&&x>-0.0001f) x=0;
	if((ff[1]&0x80000000)&&(save._3dnow)){//3DNow!!
		ALIGNED float k,x1=x;
		__asm{
			femms
			movd     mm0,x1
			pfrsqrt  mm1,mm0             ;mm1     ???        | 1/sqrt(b) 15�r�b�g���x
			movq     mm2,mm1
			pfmul    mm1,mm1
			pfrsqit1 mm1,mm0
			pfrcpit2 mm1,mm2             ;mm1     ???        | 1/sqrt(b)     24�r�b�g���x    
			pfmul    mm0,mm1             ;mm0     ???        | sqrt(b)
			movd     k,mm0
//			movd     kk,mm1
			femms
		}
		return k;
	}else if((ff[0]&0x02000000)&&(save.sse)){//SSE
		ALIGNED float k,x1=x,a1=-0.5f,a2=1.5f;
		__asm{
//			movaps	xmm0, x1
//			rsqrtps	xmm1, xmm0
//			mulps	xmm1, xmm0
//			movaps	k, xmm1

			// 1/��x1 = 3/2 * y - 1/2 * x1 * y3;  y=1/��x1 �̋ߎ��l
			//SSE = a2*xmm3 + (-0.5*xmm0*xmm1*xmm1*xmm1) �ŋt���������̋ߎ��l
			//�������ɖ߂��ɂ�x1�����Ɋ|���� ��x1 = x1 * 1/��x1
			//�������Ȃ�sqrtps������܂���56�T�C�N��������܂����̂��ߋt�����狁�߂܂�
			movaps	xmm0, x1  //���͂���x1��xmm0��   :2:xmm0=x1
			movaps  xmm2, xmm0//                     :1:xmm2=x1
			rsqrtps	xmm1, xmm0//x1���t������������   :2:xmm1=1/��x1(11bit)
			movaps  xmm3, xmm1//                     :1:xmm3=1/��x1(11bit)
			mulps	xmm1, xmm1// y3��                :4:xmm1=xmm1*xmm1  2��
			mulps	xmm1, xmm3//                     :4:xmm1=xmm1*xmm3  3��
			mulps	xmm1, xmm0//a���|����            :4:xmm1=xmm1*xmm0(x1)
			mulps	xmm1, a1  //-0.5                 :4:xmm1=xmm1*(-0.5)
			mulps	xmm3, a2  //1.5                  :4:xmm3=xmm3*1.5
			addps	xmm3, xmm1//                     :3:xmm3=xmm3+xmm1
			mulps	xmm3, xmm2//�t������������߂�   :4:xmm3=xmm3*xmm2
			movaps	k, xmm3   //���� 22bit���x       :3: 36�T�C�N��
		}
		return k;
	}

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
					D3DXToRadian(45.0f),								// ����p
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

//3D��ł̌�������
//Orig      : ���_
//dir       : �p�x
//v0,v1,v2  : �O�p�`�̒��_
//pRetT     : ����
//
//px = dir.x * pRetT + Orig.x;
//py = dir.y * pRetT + Orig.y;
//pz = dir.z * pRetT + Orig.z;
//�ňʒu�����܂�

BOOL Cchacha_sdk8_graphic::TriangleIntersect(D3DXVECTOR3 Orig, D3DXVECTOR3 dir, D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2, 
											 float *pRetT, float *pRetU, float *pRetV)
{
    D3DXVECTOR3 e1, e2, pvec, tvec, qvec;
    float det;
    float t, u, v;
    float inv_det;

    D3DXVec3Subtract(&e1, &v1, &v0);
    D3DXVec3Subtract(&e2, &v2, &v0);

    D3DXVec3Cross(&pvec, &dir, &e2);
    det = D3DXVec3Dot(&e1, &pvec);

    if (det > (1e-3)) {

        D3DXVec3Subtract(&tvec, &Orig, &v0);
        u = D3DXVec3Dot(&tvec, &pvec);
        if (u < 0.0f || u > det) return FALSE;

        D3DXVec3Cross(&qvec, &tvec, &e1);

        v = D3DXVec3Dot(&dir, &qvec);
        if (v < 0.0 || u + v > det) return FALSE;
    } else if (det < -(1e-3)) {
        D3DXVec3Subtract(&tvec, &Orig, &v0);

        u = D3DXVec3Dot(&tvec, &pvec);
        if (u > 0.0 || u < det) return FALSE;

        D3DXVec3Cross(&qvec, &tvec, &e1);

        v = D3DXVec3Dot(&dir, &qvec);
        if (v > 0.0 || u + v < det) return FALSE;

    } else {
        return FALSE;
    }

    inv_det = 1.0f / det;

    t = D3DXVec3Dot(&e2, &qvec);
    t *= inv_det;
    u *= inv_det;
    v *= inv_det;

    if(pRetT) *pRetT = t;
    if(pRetU) *pRetU = u;
    if(pRetV) *pRetV = v;

    return TRUE;    //hit!!
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
DWORD _ff1=0,_ff2=0;
BOOL QP;
LARGE_INTEGER freq;
BOOL Cchacha_sdk8_graphic::CreateD3D(HWND hInst)
{
	m_tex_mini=NULL;
	m_tex_map=m_tex_map0=m_tex_map1=m_tex_map2=m_tex_map3=NULL;
	m_tex_jiki=NULL;
	m_tex_bar=NULL;
	cl=0x00FFFFFF;
	if(QueryPerformanceFrequency(&freq)){QP=TRUE;}else QP=FALSE;
	DWORD ff1,ff2;
	__asm{
		mov eax,1;
		cpuid;
		mov ff1,edx;
	}
	__asm{
		mov eax,0x80000001;
		cpuid;
		mov ff2,edx;
	}
	_ff1=ff[0]=ff1;
	_ff2=ff[1]=ff2;
	init_sqrt_fast();
	int iii=0;
	for(float s=0.0f;s<=6.4f;s+=0.0001f)
	{
		if(iii==64000) break;
		sintb[iii]=(float)sin(s);
		costb[iii]=(float)cos(s);
		iii++;
	}
	iii=0;
	for( s=-1.0f;s<=1.0f;s+=0.0001f)
	{
		if(iii==20000) break;
		acostb[iii]=(float)acos(s);iii++;
	}
	// D3D�I�u�W�F�N�g���쐬����
	if((m_pD3D = ::Direct3DCreate8(D3D_SDK_VERSION)) == NULL){
		::OutputDebugString("Error CreateD3D() : Direct3DCreate8()\n");
		return FALSE;
	}

	// Window���[�h�Ȃ�A���݂̃f�B�X�v���C���[�h���擾����
	if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))){
		::OutputDebugString("Error CreateD3D() : GetAdapterDisplayMode()\n");
		return FALSE;
	}


	// �f�o�C�X�̔\�͂��L�q����
	::ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = m_dwScreenW;
	d3dpp.BackBufferHeight = m_dwScreenH;
	if(m_bWindow)
		d3dpp.BackBufferFormat = d3ddm.Format;
	else
		d3dpp.BackBufferFormat = save.b32?D3DFMT_X8R8G8B8:D3DFMT_R5G6B5;//D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount = 1;
	d3dpp.Windowed = m_bWindow;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;//D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow=hInst;
	if(m_bWindow==FALSE){
		int cc=m_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT);
		if(cc){
			D3DDISPLAYMODE adp;
			for(int i=0;i<cc;i++){
				m_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT,i,&adp);
				if(adp.Format==d3dpp.BackBufferFormat && adp.RefreshRate == 60 && adp.Width ==m_dwScreenW && adp.Height ==m_dwScreenH) break;
			}
			if(cc==i)
				d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
			else
				d3dpp.FullScreen_RefreshRateInHz = 60;
		}
	}
	//d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_ONE;
	D3DDEVTYPE	devType = (save.dev == 0 ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF);

	// D3D�f�o�C�X���쐬����
	// �܂��ATnL���n�[�h�E�F�A�Ɏ��
//	if(devType==D3DDEVTYPE_HAL){
		D3DCAPS8 caps;
		m_pD3D->GetDeviceCaps( 0, D3DDEVTYPE_HAL, &caps );
		DWORD VertexShaderVersion = caps.VertexShaderVersion;
		// ���_�V�F�[�_�[��1.0�͂���̂񂩁H
		if ( VertexShaderVersion >= D3DVS_VERSION(1,0) )
		{
			// HARDWARE T&L
			if FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, devType, hInst, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pd3dDevice ) )
			{
				// SOFTWARE T&L
				if FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, devType, hInst, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pd3dDevice ) )
				{
					// REFERENCE RASTERIZE
					if FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF,hInst, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pd3dDevice ) )
					{               
						// ���߂����ł�
						MessageBox( NULL,"Direct3D�f�o�C�X�̐����Ɏ��s���܂���", "Error" , MB_OK | MB_ICONSTOP );
						return false;
					}
				}
			}
		}
		else
		{
			// HARDWARE&SOFTWARE T&L
			if FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, devType, hInst, D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &m_pd3dDevice ) )
			{
				// SOFTWARE T&L
				if FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, devType, hInst, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pd3dDevice ) )
				{
					// REFERENCE RASTERIZE
					if FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hInst, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pd3dDevice ) )
					{               
						// ���߂����ł�
						MessageBox( NULL,"Direct3D�f�o�C�X�̐����Ɏ��s���܂���", "Error" , MB_OK | MB_ICONSTOP );
						return false;
					}
				}
			}
		}
//	}else{
//		if FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hInst, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pd3dDevice ) )
//		{               
//			::OutputDebugString("Error CreateD3D() : CreateDevice()\n");
//			return FALSE;
//		}
//	}

	// �o�b�N�o�b�t�@��Z�o�b�t�@���擾����
	g_pd3dDevice = m_pd3dDevice;

    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, true);
	g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	if(m_dwScreenW!=1024){
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	}
//	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );
//	float LODBias = 0.0f;
//	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPMAPLODBIAS, *((LPDWORD)(&LODBias)) );
	
//	return TRUE;

//	if(FAILED(m_pd3dDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer))){
//		::OutputDebugString("Error CreateD3D() : GetBackBuffer()\n");
//		return FALSE;
//	}
//	if(FAILED(m_pd3dDevice->GetDepthStencilSurface(&m_pZBuffer))){
//		::OutputDebugString("Error CreateD3D() : GetDepthStencilSurface()\n");
//		return FALSE;
//	}

	SetRect(&MState.moveRect, 0, 0, m_dwScreenW+16, m_dwScreenH+16);	// �}�E�X�J�[�\���̓����͈�
	MState.x = m_dwScreenW/2;	// �}�E�X�J�[�\���̂w���W��������
	MState.y = m_dwScreenH/2-50;	// �}�E�X�J�[�\���̂x���W��������
	MState.lButton = false;	// ���{�^���̏���������
	MState.rButton = false;	// �E�{�^���̏���������
	MState.cButton = false;	// �����{�^���̏���������
	MState.moveAdd = 2;	// �}�E�X�J�[�\���̈ړ��ʂ�ݒ�
	SetRect(&MState.imgRect, 0, 0, 16, 16);	// �}�E�X�J�[�\���摜�̋�`��ݒ�
	MState.imgWidth = MState.imgRect.right - MState.imgRect.left;	// �摜�̕����v�Z
	MState.imgHeight = MState.imgRect.bottom - MState.imgRect.top;	// �摜�̍������v�Z

	// �r���[�|�[�g�̐ݒ�
	g_ViewPort.X		= 0;
	g_ViewPort.Y		= 0;
	g_ViewPort.Width	= m_dwScreenW;
	g_ViewPort.Height	= m_dwScreenH;
	g_ViewPort.MinZ		= 0.0f;
	g_ViewPort.MaxZ		= 1.0f;
	g_pd3dDevice->SetViewport( &g_ViewPort );

	WHx=1024.0f/m_dwScreenW;
	WHy=768.0f/m_dwScreenH;

	m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,	0x00000000, 1.0f, 0L);
	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);

	return TRUE;

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

//	g_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT); 	

//	InitFont(&g_Font1, "�l�r �S�V�b�N", 8, 16);

	return TRUE;

}

void Cchacha_sdk8_graphic::DoEvent()
{
	MSG msg;
	for(;;){
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.hwnd==hwnd)
				TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else return;
	}
}

void Cchacha_sdk8_graphic::Screen()
{
//	return;
	if(maxx<=1024 && maxy<=768) return;
	m_bWindow = ! m_bWindow;
	if(m_bWindow==0){//�t���X�N���[�����[�h
		d3dpp.BackBufferFormat =  save.b32?D3DFMT_X8R8G8B8:D3DFMT_R5G6B5;//D3DFMT_X8R8G8B8;
		d3dpp.Windowed = 0;
		int cc=m_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT);
		if(cc){
			D3DDISPLAYMODE adp;
			CString s;
			for(int i=0;i<cc;i++){
				m_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT,i,&adp);
				if(adp.Format==d3dpp.BackBufferFormat && adp.RefreshRate == 60 && adp.Width ==m_dwScreenW && adp.Height ==m_dwScreenH) break;
			}
			if(cc==i)
				d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
			else
				d3dpp.FullScreen_RefreshRateInHz = 60;
		}
		SetWindowPos(hwnd, HWND_NOTOPMOST,
         0,0,
         m_dwScreenW,
		 m_dwScreenH,
         SWP_SHOWWINDOW);
		SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	}else{//�E�B���h�E���[�h
		d3dpp.BackBufferFormat = d3ddm.Format;
		d3dpp.Windowed = 1;
		d3dpp.FullScreen_RefreshRateInHz = 0;
		SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_VISIBLE|WS_CAPTION);
		SetWindowPos(hwnd, HWND_NOTOPMOST,
         maxx/2-m_dwScreenW/2, maxy/2-m_dwScreenH/2,
         m_dwScreenW+(GetSystemMetrics(SM_CXFIXEDFRAME)+GetSystemMetrics(SM_CXEDGE))*2,
		 m_dwScreenH+(GetSystemMetrics(SM_CYCAPTION)+(GetSystemMetrics(SM_CYFIXEDFRAME)+GetSystemMetrics(SM_CYEDGE))*2),
         SWP_SHOWWINDOW);
	}
	Reset();
	SetRect(&MState.moveRect, 0, 0, m_dwScreenW+16, m_dwScreenH+16);	// �}�E�X�J�[�\���̓����͈�
	MState.x = m_dwScreenW/2;	// �}�E�X�J�[�\���̂w���W��������
	MState.y = m_dwScreenH/2-50;	// �}�E�X�J�[�\���̂x���W��������
	MState.lButton = false;	// ���{�^���̏���������
	MState.rButton = false;	// �E�{�^���̏���������
	MState.cButton = false;	// �����{�^���̏���������
	MState.moveAdd = 2;	// �}�E�X�J�[�\���̈ړ��ʂ�ݒ�
	SetRect(&MState.imgRect, 400, 0, 420, 20);	// �}�E�X�J�[�\���摜�̋�`��ݒ�
	MState.imgWidth = MState.imgRect.right - MState.imgRect.left;	// �摜�̕����v�Z
	MState.imgHeight = MState.imgRect.bottom - MState.imgRect.top;	// �摜�̍������v�Z
}

BOOL Cchacha_sdk8_graphic::Reset()
{
	if(g_pd3dDevice){
			RELEASE(m_pZBuffer);
			RELEASE(m_pBackBuffer);
//			DWORD rr,rr2=D3DERR_DEVICELOST;
			for(int i=0;i<5;i++){
				DoEvent();DoEvent();DoEvent();DoEvent();DoEvent();Sleep(100);
				if(FAILED(g_pd3dDevice->Reset(&d3dpp))){
					continue;
				}else break;
			}
			if(i==5) return FALSE;
	}
//	if(FAILED(m_pd3dDevice->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer))){
//		::OutputDebugString("Error CreateD3D() : GetBackBuffer()\n");
//		return;
//	}
//	if(FAILED(m_pd3dDevice->GetDepthStencilSurface(&m_pZBuffer))){
//		::OutputDebugString("Error CreateD3D() : GetDepthStencilSurface()\n");
//		return;
//	}
//	InitFont(&g_Font1, "�l�r �S�V�b�N", 8, 16);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	if(m_dwScreenW!=1024){
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	}
//	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_LINEAR );
//	float LODBias = 0.0f;
//	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPMAPLODBIAS, *((LPDWORD)(&LODBias)) );
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, true);
    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

	return TRUE;
}
BOOL jk=FALSE,jk2=FALSE;
void Cchacha_sdk8_graphic::GetInput()
{
	//�}�E�X�Ȃǂ̏��𓾂�
	int t,t2,l,l2;
	RECT rect;
	if(m_bWindow){
		GetWindowRect(hwnd,&rect);
		t=rect.top+(GetSystemMetrics(SM_CYCAPTION)+(GetSystemMetrics(SM_CYFIXEDFRAME)+GetSystemMetrics(SM_CYEDGE)));
		l=rect.left+(GetSystemMetrics(SM_CXFIXEDFRAME)+GetSystemMetrics(SM_CXEDGE));
		t2=t+m_dwScreenH+(GetSystemMetrics(SM_CYFIXEDFRAME)+GetSystemMetrics(SM_CYEDGE));
		l2=l+m_dwScreenW+(GetSystemMetrics(SM_CXFIXEDFRAME)+GetSystemMetrics(SM_CXEDGE));
	}else{
		GetWindowRect(hwnd,&rect);
		t=rect.top;
		l=rect.left;
		t2=t+m_dwScreenH;
		l2=l+m_dwScreenW;
	}
	MState.lButton=FALSE; MState.rButton=FALSE;
	GetCursorPos(&po);
	chacha_i->GetMouseState();
	if(g_lpDIJoypad&&save.joypad){
		chacha_i->GetJoypadState();
		po.x +=dijs.lX/70;if(po.x>=l && po.x<=l2){}else po.x-=dijs.lX/70;
		po.y +=dijs.lY/70;if(po.y>=t && po.y<=t2){}else po.y-=dijs.lY/70;
		(dijs.rgbButtons[save.key[0]]& 0x80) ? MState.rButton |= TRUE : MState.rButton |= FALSE;
		(dijs.rgbButtons[save.key[1]]& 0x80) ? MState.lButton |= TRUE : MState.lButton |= FALSE;
		if((jk==FALSE)&&(dijs.rgbButtons[2]&0x80)){MState.wheel=-120; jk=TRUE;}
		if((dijs.rgbButtons[save.key[2]]& 0x80)==0){jk=FALSE;MState.wheel=0;}
		if(po.x>=l && po.x<=l2 && po.y>=t && po.y<=t2)
			SetCursorPos(po.x,po.y);else GetCursorPos(&po);
	}
	if(po.x>=l && po.x<=l2 && po.y>=t && po.y<=t2){MState.x=po.x-l;MState.y=po.y-t;}
	if ( MState.x < MState.moveRect.left ) MState.x = MState.moveRect.left;
	if ( MState.x > MState.moveRect.right - MState.imgWidth ) MState.x = MState.moveRect.right - MState.imgWidth;
	if ( MState.y < MState.moveRect.top ) MState.y = MState.moveRect.top;
	if ( MState.y > MState.moveRect.bottom - MState.imgHeight ) MState.y = MState.moveRect.bottom - MState.imgHeight;
	if(save.mouse[1]==7)
		MState.lButton|=g_diMouseState.lZ;
	else
		MState.lButton|=(g_diMouseState.rgbButtons[save.mouse[1]] & 0x80);
	if(save.mouse[0]==7)
		MState.rButton|=g_diMouseState.lZ;
	else
		MState.rButton|=(g_diMouseState.rgbButtons[save.mouse[0]] & 0x80);
	MState.cButton = FALSE;
	if(jk2==FALSE&&(g_diMouseState.rgbButtons[save.mouse[3]] & 0x80)){
		(g_diMouseState.rgbButtons[save.mouse[3]] & 0x80) ? MState.cButton = TRUE : MState.cButton = FALSE;jk2=TRUE;}
	if((g_diMouseState.rgbButtons[save.mouse[3]] & 0x80)==0)jk2=FALSE;
	if(save.mouse[2]==7){
		MState.wheel+=g_diMouseState.lZ*120;
	}else{
		MState.wheel+=((g_diMouseState.rgbButtons[save.mouse[2]] & 0x80)!=0)*120;
	}
	chacha_i->GetKeyboardState();
}
BOOL Cchacha_sdk8_graphic::DoEventChk(BOOL& endf)
{
	DoEvent();
	if(GetAsyncKeyState(VK_RETURN) < 0&& GetAsyncKeyState(VK_MENU) < 0) Screen();
	if(runf) return TRUE;
//	chacha_a->WaveDAEnd();
	chacha_a->Pause(FALSE);
	for(;;){if(endf) return FALSE;
		if(runf){
			if(Reset()==0){endf=TRUE;return FALSE;}
			g_pd3dDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
			chacha_a->Pause(TRUE);
//			chacha_a->WaveDAStart("");
			Timing64(fpstiming,FALSE);
			return TRUE;
		}
	Sleep(100);
	DoEvent();
	}
}

BOOL Cchacha_sdk8_graphic::InitGraphic(int a)
{
	if(a==1){
		if(FAILED(Loadbmp("���@.bmp"        ,m_tex_jiki)))return FALSE;
		if(FAILED(Loadbmp("MAPCHIP.bmp"     ,m_tex_map)))return FALSE;
		if(FAILED(Loadbmp("BAR.bmp"         ,m_tex_bar)))return FALSE;
		if(FAILED(Loadbmp("enemy&item.bmp"  ,m_tex_teki)))return FALSE;
	}else{
		if(FAILED(Loadbmp("font32.bmp"      ,m_tex_moji)))return FALSE;
		if(FAILED(Loadbmp("font16.bmp"      ,m_tex_moji2)))return FALSE;
	}
	return TRUE;
}

BOOL Cchacha_sdk8_graphic::Loadbmp(CString f,LPDIRECT3DTEXTURE8& ff)
{
	UnCompress uc;
	if(FAILED(D3DXCreateTextureFromFileEx(g_pd3dDevice,
				uc.Open(f), 0/*��*/, 0/*����*/, 0/*�~�b�v���x��*/,
				0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0xff1f3345/*31,51,69�J���[�L�[*/,
				NULL, NULL, &ff))){
		::OutputDebugString("D3DXCreateTextureFromFileEx() Error\n");
		return FALSE;
	}
	uc.Close();
	return TRUE;
}

BOOL Cchacha_sdk8_graphic::LoadWorkTex(int no)
{
	if(no==noo) return FALSE;
	noo=no;
	for(int i=0;i<10;i++){
		RELEASE(m_tex_work[i]);
	}
	UnCompress uc;
	switch(no){
	case 0://���S
		if(FAILED(Loadbmp("title0.bmp"    ,m_tex_work[0])))return FALSE;
		if(FAILED(Loadbmp("title1.bmp"    ,m_tex_work[1])))return FALSE;
		if(FAILED(Loadbmp("tiltemoji.bmp" ,m_tex_work[2])))return FALSE;
		if(FAILED(Loadbmp("logo.bmp"      ,m_tex_work[3])))return FALSE;
		break;
	case 1://�G���f�B���O
		if(FAILED(Loadbmp("ending.bmp"    ,m_tex_work[0])))return FALSE;
		if(FAILED(Loadbmp("ending2.bmp"   ,m_tex_work[1])))return FALSE;
		break;
	}
	return TRUE;
}

BOOL Cchacha_sdk8_graphic::InitSprite(CString s)
{
	RELEASE(m_tex_mini);
	UnCompress uc;
	if(FAILED(D3DXCreateTextureFromFileEx(g_pd3dDevice,
				uc.Open(s), 0/*��*/, 0/*����*/, 0/*�~�b�v���x��*/,
				0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, 0xff1f3345/*31,51,69�J���[�L�[*/,
				NULL, NULL, &m_tex_mini))){
		::OutputDebugString("D3DXCreateTextureFromFileEx() Error\n");
		return FALSE;
	}
	uc.Close();
	return TRUE;
}

void Cchacha_sdk8_graphic::Begin(BOOL a)
{
    // �o�b�N�E�o�b�t�@�̔w�i�F��ݒ肵�ăN���A
	if(g_pd3dDevice){
		if(a)
			g_pd3dDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
		// �V�[�����J�n
		g_pd3dDevice->BeginScene();
	}
}

void Cchacha_sdk8_graphic::End(BOOL a)
{
	if(g_pd3dDevice){
		if(a)
		    g_pd3dDevice->EndScene();
		else{
	    // �V�[���̕`��
		    if(g_pd3dDevice->Present(NULL,NULL,NULL,NULL)==D3DERR_DEVICELOST)
				if(g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		            g_pd3dDevice->Reset(&d3dpp);
		}
	}
}

void Cchacha_sdk8_graphic::TekiDraw(float x,float y,int cx,int cy,float rad,BOOL radf,DWORD alpha,int l)
{
	PartDrawA(x,y,cx,cy,(float)l,(float)l,5,alpha,rad,radf);
}
void Cchacha_sdk8_graphic::JikiDraw(float x,float y,int cx,int cy,float rad,BOOL radf,DWORD alpha,int l)
{
	PartDrawA(x,y,cx,cy,(float)l,(float)l,0,alpha,rad,radf);
}
void Cchacha_sdk8_graphic::JikiDraw2(float x,float y,int cx,int cy,float rad,BOOL radf,DWORD alpha,int l)
{
	PartDrawA(x,y,cx,cy,(float)l,(float)l,30,alpha,rad,radf,16,16);
}
void Cchacha_sdk8_graphic::PartDraw(float x,float y,int cx,int cy,float rad,BOOL radf,DWORD alpha)
{
	PartDrawA(x,y,cx,cy,16,16,1,alpha,rad,radf);
}
//0:���@.1:map 2:mini 3:bar 4:bar 5:teki 6:moji32 7:moji16 8:-- 9:mini2 10-19:work[0]-work[9] 20-23:texback
//31:bar 2
void Cchacha_sdk8_graphic::PartDrawA(float x,float y,//��ʂւ̕\���ʒu
									 int cx,int cy,//�e�N�X�`�����̊J�n�ʒu
									 float lx,float ly,//�`�掞�̂�������
									 int mp,//���(���0�`�@���Q��)
									 DWORD alpha,//���l
									 float rad,BOOL radf,//��]
									 float x1,float y1//�e�N�X�`�����̏I���͈�
									 )
{
	if(!g_pd3dDevice) return;
	float f32=32.0f;
	if((int)alpha < 0) alpha=0;
	if((int)alpha > 255) alpha=255;
	if(mp<20){
		if(m_dwScreenW!=1024){lx= (((float)lx+0.5f)/(WHx)); ly=(((float)ly+0.5f)/(WHy)); f32=31.0f;
		if((cx==4*32&&cy==0&&mp==0)||(cx==5*32&&cy==0&&mp==0)||(cy>=3*32&&mp==3&&cl==1)){}else{x= ((x+0.5f)/WHx); y=((y+0.5f)/(WHy));cl=0x00ffffff;}}
		if(cy>=3*32&&mp==3&&cl==1) cl=0x00ffffff;
	}
	float xd=(float)cx,xd2;
	float yd=(float)cy,yd2;
//	int ptn=0;
	switch(mp){
	case 20:case 21:case 22:case 23:
		break;
	case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:
		xd2=(xd+x1)/512.0f;
		xd = xd    /512.0f;
		yd2=(yd+y1)/512.0f;
		yd = yd    /512.0f;
		break;
	case 30:
		xd2=(xd+x1)/256.0f;
		xd = xd    /256.0f;
		yd2=(yd+y1)/256.0f;
		yd = yd    /256.0f;
		break;
	case 0:case 3:case 5:case 31:
		xd2=(xd+f32)/256.0f;
		xd = xd     /256.0f;
		yd2=(yd+f32)/256.0f;
		yd = yd     /256.0f;
		break;
	case 6:
		xd2=(xd+f32)/256.0f;
		xd = xd     /256.0f;
		yd2=(yd+f32)/256.0f;
		yd =(yd+y1) /256.0f;
		break;
	case 4:
		xd2=(xd+ 4.0f)/256.0f;
		xd = xd       /256.0f;
		yd2=(yd+ 4.0f)/256.0f;
		yd = yd       /256.0f;
		break;
	case 7:
		xd2=(xd+16.0f)/256.0f;
		xd = xd       /256.0f;
		yd2=(yd+16.0f)/256.0f;
		yd = (yd+y1)  /256.0f;
		break;
	case 1:
		xd2=(xd+f32)/512.0f;
		xd = xd       /512.0f;
		yd2=(yd+f32)/512.0f;
		yd = yd       /512.0f;
		break;
	case 9:cl=0;
	case 2:case 8:
		xd2=1.0f;
		xd =0.0f;
		yd2=1.0f;
		yd =0.0f;
	}
	//��{�ƂȂ�2D�|���S���̑傫���ƈʒu�A�F
    CUSTOMVERTEX g_pVerteces [4] = {
        { x-lx, y-ly, 0.0f, 1.0f, cl|alpha<<24, xd , yd },
        { x+lx, y-ly, 0.0f, 1.0f, cl|alpha<<24, xd2, yd },
        { x+lx, y+ly, 0.0f, 1.0f, cl|alpha<<24, xd2, yd2},
        { x-lx, y+ly, 0.0f, 1.0f, cl|alpha<<24, xd , yd2}    };
    CUSTOMVERTEX g_pVerteces1[4] = {
        { x-0.0f, y+0.0f, 0.0f, 1.0f, cl|alpha<<24, xd , yd },
        { x+lx*2, y+0.0f, 0.0f, 1.0f, cl|alpha<<24, xd2, yd },
        { x+lx*2, y+ly*2, 0.0f, 1.0f, cl|alpha<<24, xd2, yd2},
        { x-0.0f, y+ly*2, 0.0f, 1.0f, cl|alpha<<24, xd , yd2}    };
    CUSTOMVERTEX g_pVerteces2[4] = {
        { x-0.0f,y+0.0f,0.0f, 1.0f, cl|alpha<<24, xd , yd },
        { x+lx, y+0.0f, 0.0f, 1.0f, cl|alpha<<24, xd2, yd },
        { x+lx  , y+ly, 0.0f, 1.0f, cl|alpha<<24, xd2, yd2},
        { x-0.0f, y+ly, 0.0f, 1.0f, cl|alpha<<24, xd , yd2}    };
    CUSTOMVERTEX g_pVerteces3[4] = {
        { 0  ,0  , 0.0f, 1.0f, cl|alpha<<24, 0 ,0},
        { 512,0  , 0.0f, 1.0f, cl|alpha<<24, 1, 0},
        { 512,512, 0.0f, 1.0f, cl|alpha<<24, 1, 1},
        { 0  ,512, 0.0f, 1.0f, cl|alpha<<24, 0 ,1}};
	switch(mp){
	case 0:case 30:g_pd3dDevice->SetTexture( 0, m_tex_jiki );break;
	case 1:g_pd3dDevice->SetTexture( 0, m_tex_map );break;
	case 2: g_pd3dDevice->SetTexture( 0, m_tex_mini );break;
	case 3:case 4:case 31:g_pd3dDevice->SetTexture( 0, m_tex_bar );break;
	case 5:g_pd3dDevice->SetTexture( 0, m_tex_teki );break;
	case 6:g_pd3dDevice->SetTexture( 0, m_tex_moji );break;
	case 7:g_pd3dDevice->SetTexture( 0, m_tex_moji2);break;
	case 9:case 8:g_pd3dDevice->SetTexture( 0, NULL );break;
	case 10:g_pd3dDevice->SetTexture( 0, m_tex_work[0] );break;
	case 11:g_pd3dDevice->SetTexture( 0, m_tex_work[1] );break;
	case 12:g_pd3dDevice->SetTexture( 0, m_tex_work[2] );break;
	case 13:g_pd3dDevice->SetTexture( 0, m_tex_work[3] );break;
	case 14:g_pd3dDevice->SetTexture( 0, m_tex_work[4] );break;
	case 15:g_pd3dDevice->SetTexture( 0, m_tex_work[5] );break;
	case 16:g_pd3dDevice->SetTexture( 0, m_tex_work[6] );break;
	case 17:g_pd3dDevice->SetTexture( 0, m_tex_work[7] );break;
	case 18:g_pd3dDevice->SetTexture( 0, m_tex_work[8] );break;
	case 19:g_pd3dDevice->SetTexture( 0, m_tex_work[9] );break;
	}
    g_pd3dDevice->SetVertexShader( D3DFVF_CUSTOMVERTEX );
	switch(mp){
	case 20:case 21:case 22:case 23:g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, g_pVerteces3, sizeof CUSTOMVERTEX );break;
	case 0:case 1:case 5:if(radf==TRUE)	Rote(g_pVerteces,rad);g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, g_pVerteces, sizeof CUSTOMVERTEX );break;
	case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:case 8:case 6:case 7:case 30:case 31:if(radf==TRUE)	Rote(g_pVerteces2,rad);
	g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, g_pVerteces2, sizeof CUSTOMVERTEX );break;
	default:if(radf==TRUE)	Rote(g_pVerteces1,rad);g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, g_pVerteces1, sizeof CUSTOMVERTEX );break;
	}
	cl=0x00FFFFFF;kaiten=0;
}

void Cchacha_sdk8_graphic::Fade(DWORD f)
{
	if(!g_pd3dDevice) return;
	if((int)f < 0) return;
	if((int)f > 255) f=255;

	//��{�ƂȂ�2D�|���S���̑傫���ƈʒu�A�F
    FADEVERTEX g_pVerteces [] = {
        { 0.0f              , 0.0f              , 0.0f, 1.0f, 0x00|f<<24 },
        { (float)m_dwScreenW, 0.0f              , 0.0f, 1.0f, 0x00|f<<24 },
        { (float)m_dwScreenW, (float)m_dwScreenH, 0.0f, 1.0f, 0x00|f<<24 },
        { 0.0f              , (float)m_dwScreenH, 0.0f, 1.0f, 0x00|f<<24 }    };

	g_pd3dDevice->SetTexture(0, NULL);
    g_pd3dDevice->SetVertexShader( D3DFVF_FADEVERTEX );
    g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, g_pVerteces, sizeof FADEVERTEX );
}


void Cchacha_sdk8_graphic::Rote(CUSTOMVERTEX* v, float Rotate)
{
	float PAD = 3.141592f / 180.0f; // ���W�A���̏�����
    D3DXMATRIX PosMatrix, RotateMatrix;
    float x1, y1;
    int i;
	if(Rotate>=360) Rotate-=360;
	if(Rotate<0) Rotate+=360;

    // �|���S���̒��S�����߁A���_�ֈړ�������
    x1 = (v[2].x - v[0].x) / 2.0f + v[0].x;
    y1 = (v[3].y - v[1].y) / 2.0f + v[1].y;
    MoveMatrix(v, -x1, -y1, 0.0f);

    // �s��̏������i�P�ʍs�񐶐��j
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&RotateMatrix);

    // ��]�s��Ɋp�x��ݒ�
	if(kaiten==0)
	    D3DXMatrixRotationZ(&RotateMatrix, (float)Rotate * PAD);
	else
	    D3DXMatrixRotationX(&RotateMatrix, (float)Rotate * PAD);
	kaiten=0;
    // ��]����
    for ( i=0 ; i<4 ; i++ ) {
        // ���݂̒��_���W���i�[
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        // ���Z
        PosMatrix *= RotateMatrix;
        // ���ʂ�߂�
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = 0;//PosMatrix._43;
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
	float f32=32.0f;
	if(m_dwScreenW!=1024){x= ftol((float)x/WHx); y=ftol((float)y/(WHy));}
//	DrawText8(g_Font1, (char*)str, x, y, c);
}

int tb[17]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
void Cchacha_sdk8_graphic::Moji32(const char* str,int mojisu,int syu,float x,float y,BOOL alpha,BOOL alpha2,int kaite)
{
	char a,b,c;int ten;
	int mx,my,rf=0;
	float xx=x,yy=y,yyy=y,r=0;
	for(int j=0;j<alpha+1;j++){
		x=xx;y=yyy;yy=yyy;
		for(int i=0;i<mojisu;i++){
			ten=0;
			a=str[i];
			if(a=='A'){if(syu==0)syu=1;else syu=0;continue;}
			if(a=='\n'){x=xx;yy+=32;y=yy;continue;}
			if(a==' ')
				if(syu){mx=2*32;my=7*32;}else{mx=0;my=0;}
			if(a>='a' && a<='z'){
				a-='a'-1;//�󔒂�+1
				b=(char)(a%8); c=(char)(a/8);
				mx=tb[b]*32; my=tb[c]*32;
			}else if(a>='0' && a<='9'){
				a-='0';
				b=(char)(a%8); c=(char)(a/8);
				mx=tb[b]*32; my=tb[c]*32+4*32+tb[syu]*64;
			}else switch(a){
				case '[':mx=3*32; my=3*32;break;
				case ']':mx=4*32; my=3*32;break;
				case ':':mx=5*32; my=3*32;break;
				case '.':mx=5*32; my=3*32;ten=1;break;
				case '!':mx=6*32; my=3*32;break;
				case '?':mx=7*32; my=3*32;break;
				case '/':mx=2*32; my=5*32;break;
				case '*':mx=3*32; my=5*32;break;
				case '+':mx=4*32; my=5*32;break;
				case '-':mx=5*32; my=5*32;break;
				case '=':mx=6*32; my=5*32;break;
				case '%':mx=7*32; my=5*32;break;
				case 'L':mx=5*32; my=7*32;break;
				case 'I':mx=6*32; my=7*32;break;
				case 'X':mx=7*32; my=7*32;break;
				case '_':mx=3*32; my=7*32;break;
				case 'E':mx=4*32; my=7*32;break;
			}
			if(kaite!=-1){
				kaiten=1;rf=TRUE;r=(float)kaite;}
			if(ten){
				if(alpha&&j==0)
					PartDrawA(x+8,y+8,mx,my,32,16,6,alpha2/3,r,rf,0,16);
				else
					PartDrawA(x   ,y+16   ,mx,my,32,16,6,alpha2,r,rf,0,16);
			}else{
				if(alpha&&j==0)
					PartDrawA(x+8,y+8,mx,my,32,32,6,alpha2/3,r,rf,0,0);
				else
					PartDrawA(x   ,y   ,mx,my,32,32,6,alpha2,r,rf,0,0);
			}
			x+=32;
		}
	}
}

void Cchacha_sdk8_graphic::Moji16(const char* str,int mojisu,int ,float x,float y,BOOL alpha,BOOL alpha2)
{
	char a,b,c;int ten=0;
	int mx,my;
	float xx=x,yy=y,yyy=y;
	for(int j=0;j<alpha+1;j++){
		x=xx;y=yyy;yy=yyy;
		for(int i=0;i<mojisu;i++){
			ten=0;
			a=str[i];
			if(a=='\n'){x=xx;yy+=16;y=yy;continue;}
			if(a==' '){
				mx=0;my=0;}
			if(a>='a' && a<='z'){
				a-='a'-1;//�󔒂�+1
				b=(char)(a%16); c=(char)(a/16);
				mx=tb[b]*16; my=tb[c]*16;
			}else if(a>='0' && a<='9'){
				a-='0';
				b=(char)(a%16); c=(char)(a/16);
				mx=tb[b]*16; my=tb[c]*16+2*16;
			}else switch(a){
				case '[':mx=11*16; my=16;break;
				case ']':mx=12*16; my=16;break;
				case ':':mx=13*16; my=16;break;
				case '.':mx=13*16; my=16;ten=1;break;
				case '!':mx=14*16; my=16;break;
				case '?':mx=15*16; my=16;break;
				case '/':mx=10*16; my=2*16;break;
				case '*':mx=11*16; my=2*16;break;
				case '+':mx=12*16; my=2*16;break;
				case '-':mx=13*16; my=2*16;break;
				case '=':mx=14*16; my=2*16;break;
				case '%':mx=15*16; my=2*16;break;
			}
			if(ten==0){
				if(alpha&&j==0)
					PartDrawA(x+4,y+4,mx,my,16,16,7,alpha2/3,0,0,0,0);
				else
					PartDrawA(x  ,y ,mx,my,16,16,7,alpha2,0,0,0,0);
			}else{
				if(alpha&&j==0)
					PartDrawA(x+4,y+4,mx,my,16,8,7,alpha2/3,0,0,0,8);
				else
					PartDrawA(x  ,y+8  ,mx,my,16,8,7,alpha2,0,0,0,8);
			}
			x+=16;
		}
	}
}


DWORD Cchacha_sdk8_graphic::timing(BOOL t)
{
//60fps�Ȃ炱��Ȋ����i1/60�b=50/3�~���b�j
	if(t) { t2=timeGetTime(); return 0; }
	DoEvent();
	t4=timeGetTime();
	t5=t4-t2;
	for(;;){
		t1 = timeGetTime();
		dt = (t1 - t2) * 3 + t3;
		if(dt >= 249) {//12
			t3 = dt % 250;
			return 5;
		}
		if(dt >= 199) {//15
			t3 = dt % 200;
			return 4;
		}
		if(dt >= 99) {//30
			t3 = dt % 100;
			return 2;
		}
		if(dt >= 49) {//60
			t3 = dt % 50;
			return 1;
		}
		DoEvent();
	}
}
DWORD Cchacha_sdk8_graphic::Timing64(DWORD& Frate,BOOL bl)
{
	LARGE_INTEGER time_before;
	if(QP){
		DWORD ss,FrateL = Frate;
		if(bl)DoEvent();
		QueryPerformanceCounter(&time_before);
		Frate=(DWORD)(time_before.QuadPart*1000/freq.QuadPart);
		ss = (DWORD)((int)Frate-(int)FrateL);return ss;
	}else return Timing(Frate,bl);
}
DWORD Cchacha_sdk8_graphic::Timing(DWORD& Frate,BOOL bl)
{
	DWORD ss,FrateL = Frate;
	if(bl)DoEvent();
//	timeBeginPeriod(1);
	Frate = timeGetTime();
//	timeEndPeriod(1);
	ss = Frate-FrateL;
	return ss;
}

DWORD timetb[3]={16,17,17};
int timec=0;
void Cchacha_sdk8_graphic::timing1(WORD a,BOOL b,BOOL c)
{
//	if(m_bWindow==FALSE) return;
	DWORD fr2=0;
	if(b){
		Timing(fpstiming,FALSE);
	}
	DWORD j=0;
//	if(!a)a=1;
	for(WORD ii=0;ii<a;ii++){
		j+=timetb[timec];timec++;if(timec>2)timec=0;
	}
	Timing64(fr2,FALSE);
	if((int)(fpstiming+j)-fr2 > 10){
//		timeBeginPeriod(1);
		Sleep(5);
//		timeEndPeriod(1);
	}
	for(;;){
		Timing64(fr2,FALSE);
		if((int)fr2 >= (int)(fpstiming+j)) break;
	}
//	Timing(fpstiming,FALSE);
	if(c)DoEvent();

}