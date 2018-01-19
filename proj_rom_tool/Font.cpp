//=============================================================================
//  �t�H���g�E�I�u�W�F�N�g�֌W�̎���֐��Q
//  Copyright NKC Game Staff(�������̖��O)�@
//-----------------------------------------------------------------------------
//#include "NKC_Common.h"
#include "stdafx.h"
extern	LPDIRECT3DDEVICE8	g_pd3dDevice;
// �O���[�o���ϐ�
/* ���̃\�[�X��������p�������� */

/* ���\�[�X�ł̂ݗ��p������� */

// �v���g�^�C�v�錾
/* ���\�[�X�ł̂ݎg�p������� */

//-----------------------------------------------------------------------------
// �֐����@�F�@InitFont()
// �@�\�T�v�F�@�t�H���g�E�I�u�W�F�N�g�̏�����
//-----------------------------------------------------------------------------
HRESULT InitFont(ID3DXFont** pFont, char* FontName, int Width, int Height)
{
	LOGFONT LogFont;	// �t�H���g�X�^�C���\����

	// �t�H���g�X�^�C���̐ݒ�
	ZeroMemory(&LogFont, sizeof(LogFont));	// �\���̏�����
	LogFont.lfWidth     = Width;
	LogFont.lfHeight    = Height;
	LogFont.lfItalic    = FALSE;
	LogFont.lfUnderline = FALSE;
	LogFont.lfStrikeOut = FALSE;
	LogFont.lfCharSet   = SHIFTJIS_CHARSET;
	lstrcpy(LogFont.lfFaceName, FontName);

	// �t�H���g�I�u�W�F�N�g���쐬
	if( FAILED(D3DXCreateFontIndirect(g_pd3dDevice, &LogFont, pFont)) )
		return E_FAIL;

	return S_OK;
}

//-----------------------------------------------------------------------------
// �֐����@�F�@DrawText
// �@�\�T�v�F�@�t�H���g�E�I�u�W�F�N�g�̕`��
//-----------------------------------------------------------------------------
VOID DrawText8(ID3DXFont* pFont, char* text, DWORD x, DWORD y, D3DCOLOR Color)
{
	RECT Rect;

	SetRect(&Rect, x, y, CW_USEDEFAULT, CW_USEDEFAULT);
	pFont->DrawTextA(text, -1, &Rect, DT_LEFT | DT_NOCLIP, Color);

}
