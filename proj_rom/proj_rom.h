// proj_rom.h : PROJ_ROM アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_PROJ_ROM_H__7BC18ED9_DEE2_461B_BB18_24E0DDDD8E2E__INCLUDED_)
#define AFX_PROJ_ROM_H__7BC18ED9_DEE2_461B_BB18_24E0DDDD8E2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CProj_romApp:
// このクラスの動作の定義に関しては proj_rom.cpp ファイルを参照してください。
//

class CProj_romApp : public CWinApp
{
public:
	CProj_romApp();
	virtual ~CProj_romApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CProj_romApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL InitApplication();
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(CProj_romApp)
//	afx_msg void OnAppAbout();
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PROJ_ROM_H__7BC18ED9_DEE2_461B_BB18_24E0DDDD8E2E__INCLUDED_)
