// TekiSelect.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "proj_rom.h"
#include "TekiSelect.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTekiSelect ダイアログ


CTekiSelect::CTekiSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CTekiSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTekiSelect)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	input=-1; ret=-1;
}


void CTekiSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTekiSelect)
	DDX_Control(pDX, IDC_BUTTON1, m_sinki);
	DDX_Control(pDX, IDC_LIST1, m_tn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTekiSelect, CDialog)
	//{{AFX_MSG_MAP(CTekiSelect)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1New)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTekiSelect メッセージ ハンドラ
extern int xxx,yyy;

BOOL CTekiSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ret=-1;
	x=xxx;y=yyy;
	// TODO: この位置に初期化の補足処理を追加してください
	m_tn.ModifyStyle ( 0, LVS_REPORT );
	m_tn.InsertColumn ( 0, "番号", LVCFMT_LEFT, 40, 0 );
	m_tn.InsertColumn ( 1, "x", LVCFMT_LEFT, 30, 0 );
	m_tn.InsertColumn ( 2, "y", LVCFMT_LEFT, 30, 0 );
	m_tn.InsertColumn ( 3, "敵出現", LVCFMT_LEFT, 50, 0 );
	m_tn.InsertColumn ( 4, "遅延", LVCFMT_LEFT, 50, 0 );
	m_tn.InsertColumn ( 5, "パーツ", LVCFMT_LEFT, 60, 0 );
	m_tn.InsertColumn ( 6, "コメント", LVCFMT_LEFT, 500, 0 );
	DWORD dwExStyle = ListView_GetExtendedListViewStyle( m_tn.m_hWnd );
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_INFOTIP;
	ListView_SetExtendedListViewStyle( m_tn.m_hWnd , dwExStyle );
	char buf[512]; LV_ITEM LvItem;	int idItem;CString sfl;

	for(int i=0;i<save.tekikazu;i++){if(i==input) continue;
		sfl.Format("%03d",i);	strcpy(buf,sfl);	LvItem.pszText = buf;
		LvItem.iItem = m_tn.GetItemCount( );	LvItem.mask = LVIF_TEXT | LVIF_STATE;	LvItem.stateMask = LVIS_FOCUSED | LVIS_SELECTED;LvItem.state = 0;	LvItem.iSubItem = 0;	LvItem.cchTextMax = 512;//strlen(buf);
		idItem = m_tn.InsertItem(&LvItem);
		LvItem.iItem = idItem;
		sfl.Format("%2d",save.tekidata[i][0]);	strcpy(buf,sfl);LvItem.iSubItem = 1;
		LvItem.pszText = buf;	m_tn.SetItem(&LvItem);
		sfl.Format("%2d",save.tekidata[i][1]);	strcpy(buf,sfl);LvItem.iSubItem = 2;
		LvItem.pszText = buf;	m_tn.SetItem(&LvItem);
		sfl.Format("%2d",save.tekidata[i][2]);	strcpy(buf,sfl);LvItem.iSubItem = 3;
		LvItem.pszText = buf;	m_tn.SetItem(&LvItem);
		sfl.Format("%2d",save.tekidata[i][3]);	strcpy(buf,sfl);LvItem.iSubItem = 4;
		LvItem.pszText = buf;	m_tn.SetItem(&LvItem);
		sfl.Format("%02X%02X%02X%02X",save.tekidata[i][7],save.tekidata[i][6],save.tekidata[i][5],save.tekidata[i][4]);	strcpy(buf,sfl);LvItem.iSubItem = 5;
		LvItem.pszText = buf;	m_tn.SetItem(&LvItem);
		strcpy(buf,cm[i].c);	LvItem.iSubItem = 6;
		LvItem.pszText = buf;	m_tn.SetItem(&LvItem);
	}

	if(input==-1) sfl="敵配置";
	else sfl="敵番号取得";
	SetWindowText(sfl);

	if(save.tekikazu==128){m_sinki.EnableWindow(FALSE);
	}
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->Draw();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CTekiSelect::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	int Lindex=-1,ff=0;
	Lindex=m_tn.GetNextItem(Lindex,LVNI_ALL |LVNI_SELECTED);
	if(Lindex==-1) return;
	
	ret=atoi(m_tn.GetItemText(Lindex,0));
	
	CDialog::OnOK();
}

void CTekiSelect::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int Lindex=-1,ff=0;
	Lindex=m_tn.GetNextItem(Lindex,LVNI_ALL |LVNI_SELECTED);
	if(Lindex==-1) return;
	
	ret=atoi(m_tn.GetItemText(Lindex,0));
	EndDialog(IDOK);
	*pResult = 0;
}

void CTekiSelect::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int Lindex=-1,ff=0;
	Lindex=m_tn.GetNextItem(Lindex,LVNI_ALL |LVNI_SELECTED);
	if(Lindex==-1) return;
	int aa=atoi(m_tn.GetItemText(Lindex,0));
	xxx=atoi(m_tn.GetItemText(Lindex,1))-24;
	yyy=atoi(m_tn.GetItemText(Lindex,2))-24;
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->ttee[0]=save.tekidata[Lindex][4];
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->ttee[1]=save.tekidata[Lindex][5];
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->ttee[2]=save.tekidata[Lindex][6];
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->ttee[3]=save.tekidata[Lindex][7];
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->Draw();	
	*pResult = 0;
}

BOOL CTekiSelect::DestroyWindow() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	xxx=x;yyy=y;
	return CDialog::DestroyWindow();
}

void CTekiSelect::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->Draw();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
}

void CTekiSelect::OnButton1New() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
		EndDialog(15951);

}
