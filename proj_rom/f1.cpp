// f1.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "proj_rom.h"
#include "f1.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Cf1 ダイアログ


Cf1::Cf1(CWnd* pParent /*=NULL*/)
	: CDialog(Cf1::IDD, pParent)
{
	//{{AFX_DATA_INIT(Cf1)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void Cf1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Cf1)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Cf1, CDialog)
	//{{AFX_MSG_MAP(Cf1)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cf1 メッセージ ハンドラ
int ttt;
BOOL Cf1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ttt=0;
	(CMainFrame*)f1->ShowWindow(SW_HIDE);
	// TODO: この位置に初期化の補足処理を追加してください
	ShowWindow(SW_SHOWNORMAL);
//	EndDialog(f->DoModal());
	SetTimer(121,100,NULL);
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void Cf1::OnTimer(UINT nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CDialog::OnTimer(nIDEvent);
}
