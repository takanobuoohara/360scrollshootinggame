// MENU.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "proj_rom.h"
#include "MENU.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMENU ダイアログ


CMENU::CMENU(CWnd* pParent /*=NULL*/)
	: CDialog(CMENU::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMENU)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CMENU::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMENU)
	DDX_Control(pDX, IDC_EDIT4, m_4);
	DDX_Control(pDX, IDC_EDIT3, m_3);
	DDX_Control(pDX, IDC_EDIT2, m_2);
	DDX_Control(pDX, IDC_EDIT1, m_1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMENU, CDialog)
	//{{AFX_MSG_MAP(CMENU)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMENU メッセージ ハンドラ

BOOL CMENU::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	CString s;
	s.Format("%d",save.startx);
	m_1.SetWindowText(s);
	s.Format("%d",save.starty);
	m_2.SetWindowText(s);
	s.Format("%d",save.mapx);
	m_3.SetWindowText(s);
	s.Format("%d",save.mapy);
	m_4.SetWindowText(s);
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CMENU::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CString s;
	m_1.GetWindowText(s);
	save.startx=atoi(s);
	m_2.GetWindowText(s);
	save.starty=atoi(s);
	m_3.GetWindowText(s);
	save.mapx=atoi(s);
	m_4.GetWindowText(s);
	save.mapy=atoi(s);

	CDialog::OnOK();
}
