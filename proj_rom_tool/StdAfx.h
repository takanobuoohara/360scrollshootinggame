// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__E4994E9F_4C54_40A9_A222_CCB27AA2E14F__INCLUDED_)
#define AFX_STDAFX_H__E4994E9F_4C54_40A9_A222_CCB27AA2E14F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。

#define PI 3.141592653589793238462643383279f 
#define PI180 0.017453292520498851324792463240439f
#define PIw180 57.295779513082320876798154814114f
#define		DEGtoRAD(x)			(float)((float)x * PI / 180.0f)
#define		RELEASE(x)			{ if (x != NULL) {x->Release(); x = NULL;} }
#define matdef 			D3DMATRIX mat; \
			mat._11 = mat._22 = mat._33 = mat._44 = 1.0f; \
			mat._12 = mat._13 = mat._14 = mat._41 = 0.0f; \
			mat._21 = mat._23 = mat._24 = mat._42 = 0.0f; \
			mat._31 = mat._32 = mat._34 = mat._43 = 0.0f; 


#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT
#include	<d3d8.h>
#include	<d3dx8.h>
#include <dmusici.h>    // DirectAudioを使用可能にする
#include <MMSystem.h>

struct CUSTOMVERTEX{
      float x, y, z, rhw;
      DWORD color;
	  float    tu, tv;

  };
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE| D3DFVF_TEX1)

#include "chacha_sdk8_graphic.h"
#include "chacha_sdk8_audio.h"
#include "font.h"
#include "UnCompress.h"
struct datasave{
	int mapdata[256][256]; //パーツデータ
	DWORD tekidata[128][8]; ////0:x 1:y 2:倒した後の敵番号 3:倒した後の敵の配置 4,5,6,7:パーツフラグ 
	int tekikazu; //敵の数
	int startx,starty; //自機スタート地点
	int clear; //クリア条件
	int mapx,mapy;
	int data[30];  //リザーブデータ
	float juryokudo;
	float dataf[31];
};
extern int tekipart[][31];

struct comment{
	char c[1024];
};
/*3:倒した後の敵の配置
0:敵消滅
1:倒された位置
2:元データ
3:ミッションクリア条件１
4：ペナルティ+1
9：ゲームオーバー
10～ 60で1秒単位で遅延登場
*/

extern datasave save;
extern comment cm[128];
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__E4994E9F_4C54_40A9_A222_CCB27AA2E14F__INCLUDED_)
