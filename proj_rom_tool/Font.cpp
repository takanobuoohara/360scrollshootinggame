//=============================================================================
//  フォント・オブジェクト関係の自作関数群
//  Copyright NKC Game Staff(←自分の名前)　
//-----------------------------------------------------------------------------
//#include "NKC_Common.h"
#include "stdafx.h"
extern	LPDIRECT3DDEVICE8	g_pd3dDevice;
// グローバル変数
/* 他のソースからも利用されるもの */

/* 自ソースでのみ利用するもの */

// プロトタイプ宣言
/* 自ソースでのみ使用するもの */

//-----------------------------------------------------------------------------
// 関数名　：　InitFont()
// 機能概要：　フォント・オブジェクトの初期化
//-----------------------------------------------------------------------------
HRESULT InitFont(ID3DXFont** pFont, char* FontName, int Width, int Height)
{
	LOGFONT LogFont;	// フォントスタイル構造体

	// フォントスタイルの設定
	ZeroMemory(&LogFont, sizeof(LogFont));	// 構造体初期化
	LogFont.lfWidth     = Width;
	LogFont.lfHeight    = Height;
	LogFont.lfItalic    = FALSE;
	LogFont.lfUnderline = FALSE;
	LogFont.lfStrikeOut = FALSE;
	LogFont.lfCharSet   = SHIFTJIS_CHARSET;
	lstrcpy(LogFont.lfFaceName, FontName);

	// フォントオブジェクトを作成
	if( FAILED(D3DXCreateFontIndirect(g_pd3dDevice, &LogFont, pFont)) )
		return E_FAIL;

	return S_OK;
}

//-----------------------------------------------------------------------------
// 関数名　：　DrawText
// 機能概要：　フォント・オブジェクトの描画
//-----------------------------------------------------------------------------
VOID DrawText8(ID3DXFont* pFont, char* text, DWORD x, DWORD y, D3DCOLOR Color)
{
	RECT Rect;

	SetRect(&Rect, x, y, CW_USEDEFAULT, CW_USEDEFAULT);
	pFont->DrawTextA(text, -1, &Rect, DT_LEFT | DT_NOCLIP, Color);

}
