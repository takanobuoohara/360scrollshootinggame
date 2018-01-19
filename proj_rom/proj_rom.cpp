// proj_rom.cpp : アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"
#include "proj_rom.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProj_romApp

BEGIN_MESSAGE_MAP(CProj_romApp, CWinApp)
	//{{AFX_MSG_MAP(CProj_romApp)
//	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
	// 標準のファイル基本ドキュメント コマンド
//	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
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
void *Mutex;
BOOL CProj_romApp::InitInstance()
{
//_CrtSetBreakAlloc(1815);
	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さく
	// したければ以下の特定の初期化ルーチンの中から不必要なもの
	// を削除してください。

	// 設定が保存される下のレジストリ キーを変更します。
	// TODO: この文字列を、会社名または所属など適切なものに
	// 変更してください。
//	SetRegistryKey(_T("proj_rom"));

//	LoadStdProfileSettings();  // 標準の INI ファイルのオプションをローﾄﾞします (MRU を含む)
//	AfxEnableControlContainer();
#ifdef _AFXDLL
	Enable3dControls();			// 共有 DLL の中で MFC を使用する場合にはここを呼び出してください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクしている場合にはここを呼び出してください。
#endif

	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。
	Mutex=NULL;
	Mutex = CreateMutex(NULL,FALSE,"sd_chachamutex");
            //????????はユニークな名前を指定する 
        DWORD Status = GetLastError(); 
        if(Mutex == NULL){ 
            //CleateMutex失敗 
            exit(-1); 
        } 
        if(Status == ERROR_ALREADY_EXISTS){ 
            //すでに起動されている
            ReleaseMutex(Mutex); 
	      ::MessageBox(NULL,"Separete Dimension設定またはゲーム本体が起動してます。\n両方起動することは出来ませんので先に閉じてください。","システム情報",MB_ICONERROR);
            exit(-1); 
        } 

	CString s;
#if SD_BETA
	s="Separate Dimension ～遙かなる時の陽炎～ β-2版 2005.08.28 1:29";
#else
	s="Separate Dimension ～遙かなる時の陽炎～ + 独自マッププレイ版 2007.02.06 Version 1.00e PrePrayer Power Soft";
#endif
	if(m_pMainWnd==NULL){
		m_pMainWnd = new CMainFrame;
		theMain = (CMainFrame*)m_pMainWnd;
		if(!((CMainFrame*)m_pMainWnd)->Create(s))return FALSE;
		((CMainFrame*)m_pMainWnd)->ShowWindow(m_nCmdShow);
		((CMainFrame*)m_pMainWnd)->ValidateRect(0);
		((CMainFrame*)m_pMainWnd)->UpdateWindow();
		if(	((CMainFrame*)m_pMainWnd)->Init() )
			((CMainFrame*)m_pMainWnd)->GameProc();
		((CMainFrame*)m_pMainWnd)->Close();
	}
	if(m_pMainWnd!=NULL){//
//		((CMainFrame*)m_pMainWnd)->OnDestroy();
		delete m_pMainWnd;
		m_pMainWnd=NULL;
	}

	ReleaseMutex(Mutex);
	Mutex=NULL;
//	exit(0); //終わり
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ
/*
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

*/

int CProj_romApp::ExitInstance() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if(Mutex)
		ReleaseMutex(Mutex);
	Mutex=NULL;
	
	return CWinApp::ExitInstance();
}

BOOL CProj_romApp::InitApplication() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	return CWinApp::InitApplication();
}
