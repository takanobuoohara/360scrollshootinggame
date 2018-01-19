// proj_rom.cpp : アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"
#include "proj_rom.h"

#include "MainFrm.h"
#include "proj_romDoc.h"
#include "proj_romView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProj_romApp

BEGIN_MESSAGE_MAP(CProj_romApp, CWinApp)
	//{{AFX_MSG_MAP(CProj_romApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProj_romApp クラスの構築

CProj_romApp::CProj_romApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
	m_pMainWnd=NULL;
}

CProj_romApp::~CProj_romApp()
{
}
/////////////////////////////////////////////////////////////////////////////
// 唯一の CProj_romApp オブジェクト

CProj_romApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CProj_romApp クラスの初期化
CMainFrame *theMain;
BOOL CProj_romApp::InitInstance()
{
//_CrtSetBreakAlloc(49);
	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さく
	// したければ以下の特定の初期化ルーチンの中から不必要なもの
	// を削除してください。

	// 設定が保存される下のレジストリ キーを変更します。
	// TODO: この文字列を、会社名または所属など適切なものに
	// 変更してください。
	SetRegistryKey(_T("proj_rom"));

//	LoadStdProfileSettings();  // 標準の INI ファイルのオプションをローﾄﾞします (MRU を含む)

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。

	if(m_pMainWnd==NULL){
		m_pMainWnd = new CMainFrame;
//		CMainFrame* pMainFrame = new CMainFrame;
//		m_pMainWnd = pMainFrame;
		theMain = (CMainFrame*)m_pMainWnd;
		if(!((CMainFrame*)m_pMainWnd)->Create("Separate Dimension ～遙かなる時の陽炎～ マップツール"))return FALSE;
		((CMainFrame*)m_pMainWnd)->nn.LoadMenu(IDR_MENU1);
		((CMainFrame*)m_pMainWnd)->SetMenu(&((CMainFrame*)m_pMainWnd)->nn);
		((CMainFrame*)m_pMainWnd)->ShowWindow(m_nCmdShow);
		((CMainFrame*)m_pMainWnd)->UpdateWindow();
		((CMainFrame*)m_pMainWnd)->Init();
		((CMainFrame*)m_pMainWnd)->GameProc();
	//	((CMainFrame*)m_pMainWnd)->Close();
	}
	if(m_pMainWnd!=NULL){
		m_pMainWnd->DestroyWindow();
		delete m_pMainWnd;
		m_pMainWnd=NULL;
	}

	exit(0); //終わり
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard 仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
		// メッセージ ハンドラはありません。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// メッセージ ハンドラはありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CProj_romApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CProj_romApp メッセージ ハンドラ

