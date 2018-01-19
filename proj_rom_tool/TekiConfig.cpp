// TekiConfig.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "proj_rom.h"
#include "TekiConfig.h"
#include "TekiSelect.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTekiConfig ダイアログ


CTekiConfig::CTekiConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CTekiConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTekiConfig)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CTekiConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTekiConfig)
	DDX_Control(pDX, IDC_CHECK1, m_haiti);
	DDX_Control(pDX, IDC_BUTTON3, m_del);
	DDX_Control(pDX, IDC_BUTTON2, m_ido);
	DDX_Control(pDX, IDC_BUTTON1, m_tnn);
	DDX_Control(pDX, IDC_COMBO1, m_syurui);
	DDX_Control(pDX, IDC_EDIT5, m_memo);
	DDX_Control(pDX, IDC_LIST2, m_litiran);
	DDX_Control(pDX, IDC_LIST1, m_lsettei);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTekiConfig, CDialog)
	//{{AFX_MSG_MAP(CTekiConfig)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON2, OnIdo)
	ON_BN_CLICKED(IDC_BUTTON3, OnDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTekiConfig メッセージ ハンドラ
#include "tekipart.h"


BOOL CTekiConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	CString s;s="ここには敵の名前や性質などをメモ書きする欄でツール上のみの情報です";
	m_memo.SetWindowText(s);

	m_syurui.AddString("0:敵消滅");
	m_syurui.AddString("1:倒された位置");
	m_syurui.AddString("2:元データ");
	m_syurui.AddString("3:ミッションクリア条件１");
	m_syurui.AddString("4：ペナルティ+1");
	m_syurui.AddString("5：");
	m_syurui.AddString("6：");
	m_syurui.AddString("7：");
	m_syurui.AddString("8：");
	m_syurui.AddString("9：ゲームオーバー");
	for(int i=10;i<410;i++){
		s.Format("%d:%3.2f秒後に表示",i,((float)i-10)/2);
		m_syurui.AddString(s);
	}
	if(newf){
		m_syurui.SetCurSel(0);
		m_ido.EnableWindow(FALSE);
		m_del.EnableWindow(FALSE);
		m_haiti.SetCheck(FALSE);
	}else{
		m_syurui.SetCurSel(save.tekidata[settei][2]&0x7fff);
		m_haiti.SetCheck(save.tekidata[settei][2]>>15);
	}
	m_litiran.ModifyStyle ( 0, LVS_REPORT );
	m_litiran.InsertColumn ( 0, "番号", LVCFMT_LEFT, 40, 0 );
	m_litiran.InsertColumn ( 1, "敵種別",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 2, "コメント",     LVCFMT_LEFT  , 200, 0 );
	m_litiran.InsertColumn ( 3, "x,y",     LVCFMT_LEFT  , 30, 0 );
	m_litiran.InsertColumn ( 4, "表示向",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 5, "サイズ",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 6, "当り判定",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 7, "--",     LVCFMT_LEFT  , 0, 0 );
	m_litiran.InsertColumn ( 8, "回転速度",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 9, "◆",     LVCFMT_LEFT  , 20, 0 );
	m_litiran.InsertColumn ( 10, "--",     LVCFMT_LEFT  , 0, 0 );
	m_litiran.InsertColumn ( 11, "地形",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 12, "--",     LVCFMT_LEFT  , 0, 0 );
	m_litiran.InsertColumn ( 13, "--",     LVCFMT_LEFT  , 0, 0 );
	m_litiran.InsertColumn ( 14, "自機追尾",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 15, "地表移動",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 16, "ｼﾞｬﾝﾌﾟ",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 17, "初期移方",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 18, "速度",     LVCFMT_LEFT  , 30, 0 );
	m_litiran.InsertColumn ( 19, "加速",     LVCFMT_LEFT  , 30, 0 );
	m_litiran.InsertColumn ( 20, "最高速度",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 21, "◆",     LVCFMT_LEFT  , 20, 0 );
	m_litiran.InsertColumn ( 22, "弾番号",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 23, "攻撃方向",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 24, "連射数",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 25, "連射間隔",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 26, "攻撃間隔",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 27, "◆",     LVCFMT_LEFT  , 20, 0 );
	m_litiran.InsertColumn ( 28, "盾種類",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 29, "盾方向",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 30, "防御耐性",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 31, "耐久倍率",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 32, "◆",     LVCFMT_LEFT  , 20, 0 );
	m_litiran.InsertColumn ( 33, "効果音",     LVCFMT_LEFT  , 50, 0 );

	DWORD dwExStyle = ListView_GetExtendedListViewStyle( m_litiran.m_hWnd );
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_INFOTIP;
	ListView_SetExtendedListViewStyle( m_litiran.m_hWnd , dwExStyle );

	m_lsettei.ModifyStyle ( 0, LVS_REPORT );
	m_lsettei.InsertColumn ( 0, "番号", LVCFMT_LEFT, 40, 0 );
	m_lsettei.InsertColumn ( 1, "敵種別",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 2, "コメント",     LVCFMT_LEFT  , 200, 0 );
	m_lsettei.InsertColumn ( 3, "x,y",     LVCFMT_LEFT  , 30, 0 );
	m_lsettei.InsertColumn ( 4, "表示向",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 5, "サイズ",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 6, "当り判定",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 7, "接地反転",     LVCFMT_LEFT  , 0, 0 );
	m_lsettei.InsertColumn ( 8, "回転速度",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 9, "◆",     LVCFMT_LEFT  , 20, 0 );
	m_lsettei.InsertColumn ( 10, "地形横",     LVCFMT_LEFT  , 0, 0 );
	m_lsettei.InsertColumn ( 11, "地形",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 12, "重力",     LVCFMT_LEFT  , 0, 0 );
	m_lsettei.InsertColumn ( 13, "接地移動",     LVCFMT_LEFT  , 0, 0 );
	m_lsettei.InsertColumn ( 14, "自機追尾",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 15, "地表移動",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 16, "ｼﾞｬﾝﾌﾟ",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 17, "初期移方",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 18, "速度",     LVCFMT_LEFT  , 30, 0 );
	m_lsettei.InsertColumn ( 19, "加速",     LVCFMT_LEFT  , 30, 0 );
	m_lsettei.InsertColumn ( 20, "最高速度",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 21, "◆",     LVCFMT_LEFT  , 20, 0 );
	m_lsettei.InsertColumn ( 22, "弾番号",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 23, "攻撃方向",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 24, "連射数",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 25, "連射間隔",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 26, "攻撃間隔",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 27, "◆",     LVCFMT_LEFT  , 20, 0 );
	m_lsettei.InsertColumn ( 28, "盾種類",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 29, "盾方向",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 30, "防御耐性",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 31, "耐久倍率",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 32, "◆",     LVCFMT_LEFT  , 20, 0 );
	m_lsettei.InsertColumn ( 33, "効果音",     LVCFMT_LEFT  , 50, 0 );

	dwExStyle = ListView_GetExtendedListViewStyle( m_lsettei.m_hWnd );
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_INFOTIP;
	ListView_SetExtendedListViewStyle( m_lsettei.m_hWnd , dwExStyle );
	if(newf){
		selectflag[0]=selectflag[1]=selectflag[2]=selectflag[3]=0;
		itiranflag[0]=itiranflag[1]=itiranflag[2]=itiranflag[3]=0xffffffff;
		tekino=settei;
		s.Format("呼ぶ敵番号:なし",tekino); m_tnn.SetWindowText(s);
	}else{
		selectflag[0]=save.tekidata[settei][4];itiranflag[0]=selectflag[0]^0xffffffff;
		selectflag[1]=save.tekidata[settei][5];itiranflag[1]=selectflag[1]^0xffffffff;
		selectflag[2]=save.tekidata[settei][6];itiranflag[2]=selectflag[2]^0xffffffff;
		selectflag[3]=save.tekidata[settei][7];itiranflag[3]=selectflag[3]^0xffffffff;
		tekino=save.tekidata[settei][3];
		if(tekino){
			s.Format("呼ぶ敵番号:%d",tekino); m_tnn.SetWindowText(s);
		}else{
			s.Format("呼ぶ敵番号:なし",tekino); m_tnn.SetWindowText(s);
		}
	}
	ListReDraw();
	if(newf)
		s.Format("敵データ作成:敵番号:%3d",tekino);
	else
		s.Format("敵データ編集:敵番号:%3d",tekino);
	SetWindowText(s);
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

BOOL CTekiConfig::GetTekiNo(int no,DWORD*tt)
{
//	DWORD pp=1;
	int n=no/32;
	DWORD nn=(DWORD)1L<<((DWORD)no%32);
	if( tt[n]&nn ) return TRUE;
	return FALSE;
}

void CTekiConfig::SetTekiNo(int no,DWORD*tt,BOOL a)
{
	DWORD pp=1;
	int n=no/32;
	if(a)
		tt[n]&=0xffffffff^(pp<<((DWORD)no%32));
	else
		tt[n]|=(pp<<((DWORD)no%32));
}

char koumoku01[][200]={{"地上p"},{"空中p"},{"攻撃p"},{"防御p"},{"ミサイル補給"},{"ホーミング補給"},{"エネルギー補給"},{"ダメージ減少（リペア）"},{"未使用"}};
char koumoku04[][200]={{"自機"},{"左下"},{"下"},{"右下"},{"左"},{"右回"},{"右"},{"左上"},{"上"},{"右上"},{"左回"}};
char koumoku07[][200]={{"反転なし"},{"スタート時に上に地形があると、キャラを上下反転する。"},{"スタート時に左に地形があると、キャラを左右反転する。"}};
//char koumoku09[][200]={{"地形に関係なく移動（貫通）"},{"横への移動先に地形があると移動方向を反転"},{"横への移動先に地形があると移動方向を反転＋キャラも左右反転"},{"移動先に地形があると、それ以上進めない"}};
char koumoku10[][200]={{"貫通"},{"反転"},{"停止"}};
//char koumoku11[][200]={{"重力が働かない"},{"足元か上に地形がないと、重力の影響で地形があるまで落ちる。落ちてる間は、上下左右動けない。地形に落ちた時に砂煙"},{"足元か上に地形がないと、重力の影響で地形があるまで落ちる。上下は動けないが左右には動ける。地形に落ちた時に砂煙"},{"足元に地形がないと、重力の影響で地形があるまで落ちる。落ちてる間は、上下左右動けない。地形に落ちた時に砂煙"},{"足元に地形がないと、重力の影響で地形があるまで落ちる。上下は動けないが左右には動ける。地形に落ちた時に砂煙"}};
//char koumoku12[][200]={{"足場が無くても移動できる"},{"左右に接地できる地形がないと移動できない"},{"上下に接地できる地形がないと移動できない"}};
char koumoku13[][200]={{"しない"},{"する"},{"逃げる"},{"一定距離"}};
char koumoku14[][200]={{"しない"},{"地表右"},{"地表左"}};
char koumoku15[][200]={{"しない"},{"10%"},{"20%"},{"30%"},{"40%"},{"50%"},{"60%"},{"70%"},{"80%"},{"90%"},{"常"},{"10%倍"},{"20%倍"},{"30%倍"},{"40%倍"},{"50%倍"},{"60%倍"},{"70%倍"},{"80%倍"},{"90%倍"},{"常倍"}};
char koumoku16[][200]={{"自機"},{"左下"},{"下"},{"右下"},{"左"},{"-"},{"右"},{"左上"},{"上"},{"右上"},{"-"}};
char koumoku18[][200]={{"しない"},{"1倍加速"},{"2倍加速"},{"3倍加速"}};
char koumoku19[][200]={{"無制限"},{"1.000"},{"2.000"},{"3.000"},{"4.000"},{"5.000"},{"6.000"},{"7.000"},{"8.000"},{"9.000"}};
char koumoku21[][200]={{"自機"},{"左下"},{"下"},{"右下"},{"左"},{"右回"},{"右"},{"左上"},{"上"},{"右上"},{"左回"}};
char koumoku25[][200]={{"上"},{"上下"},{"上左右"},{"全方向"}};
char koumoku26[][200]={{"自機"},{"左下"},{"下"},{"右下"},{"左"},{"右回"},{"右"},{"左上"},{"上"},{"右上"},{"左回"}};
char koumoku27[][200]={{"-"},{"マシンガン＆２WAY"},{"ホーミング"},{"マシンガン＆２WAY　＆　ホーミング"},{"ミサイル＆機雷（爆発"},{"マシンガン＆２WAY　＆　ミサイル＆機雷（爆発）"},{"ホーミング　＆　ミサイル＆機雷（爆発）"},{"全部"}};
char koumoku28[][200]={{"1倍"},{"半分"},{"2倍"},{"3倍"},{"4倍"},{"5倍"},{"6倍"},{"7倍"},{"8倍"},{"9倍"}};

void CTekiConfig::ListReDraw(BOOL a)
{
	char buf[512]; LV_ITEM LvItem;	int idItem;CString sfl;
	m_lsettei.DeleteAllItems();
	if(a) m_litiran.DeleteAllItems();
//	for(int i=0;i<5;i++){
		for(int j=0;j<128;j++){
			if(GetTekiNo(j,itiranflag)){
				if(a){
					sfl.Format("%03d",tekipart[j][0]);	strcpy(buf,sfl);	LvItem.pszText = buf;
					LvItem.iItem = m_litiran.GetItemCount( );	LvItem.mask = LVIF_TEXT | LVIF_STATE;	LvItem.stateMask = LVIS_FOCUSED | LVIS_SELECTED;LvItem.state = 0;	LvItem.iSubItem = 0;	LvItem.cchTextMax = 512;//strlen(buf);
					idItem = m_litiran.InsertItem(&LvItem);
					LvItem.iItem = idItem;
					strcpy(buf,koumoku01[tekipart[j][1]]);	LvItem.iSubItem = 1;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%s",partcomment[j]);	strcpy(buf,sfl);LvItem.iSubItem = 2;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%d,%d",tekipart[j][2],tekipart[j][3]);	strcpy(buf,sfl);LvItem.iSubItem = 3;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][4],koumoku04[tekipart[j][4]]);strcpy(buf,sfl);	LvItem.iSubItem = 4;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][5]);	strcpy(buf,sfl);LvItem.iSubItem = 5;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][6]);	strcpy(buf,sfl);LvItem.iSubItem = 6;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][7],koumoku07[tekipart[j][7]]);strcpy(buf,sfl);	LvItem.iSubItem = 7;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][8]);	strcpy(buf,sfl);LvItem.iSubItem = 8;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
//					sfl.Format("%02d:%s",tekipart[j][9],koumoku07[tekipart[j][9]]);strcpy(buf,sfl);LvItem.iSubItem = 10;
//					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][10],koumoku10[tekipart[j][10]]);strcpy(buf,sfl);LvItem.iSubItem = 11;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
//					sfl.Format("%02d:%s",tekipart[j][11],koumoku11[tekipart[j][11]]);strcpy(buf,sfl);LvItem.iSubItem = 12;
//					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
//					sfl.Format("%02d:%s",tekipart[j][12],koumoku12[tekipart[j][12]]);strcpy(buf,sfl);LvItem.iSubItem = 13;
//					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][13],koumoku13[tekipart[j][13]]);strcpy(buf,sfl);LvItem.iSubItem = 14;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][14],koumoku14[tekipart[j][14]]);strcpy(buf,sfl);LvItem.iSubItem = 15;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][15],koumoku15[tekipart[j][15]]);strcpy(buf,sfl);LvItem.iSubItem = 16;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][16],koumoku16[tekipart[j][16]]);strcpy(buf,sfl);LvItem.iSubItem = 17;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][17]);	strcpy(buf,sfl);LvItem.iSubItem = 18;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][18],koumoku18[tekipart[j][18]]);strcpy(buf,sfl);LvItem.iSubItem = 19;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][19],koumoku19[tekipart[j][19]]);strcpy(buf,sfl);LvItem.iSubItem = 20;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][20]);	strcpy(buf,sfl);LvItem.iSubItem = 22;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][21],koumoku21[tekipart[j][21]]);strcpy(buf,sfl);LvItem.iSubItem = 23;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][22]);	strcpy(buf,sfl);LvItem.iSubItem = 24;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][23]);	strcpy(buf,sfl);LvItem.iSubItem = 25;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][24]);	strcpy(buf,sfl);LvItem.iSubItem = 26;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][25],koumoku25[tekipart[j][25]]);strcpy(buf,sfl);LvItem.iSubItem = 28;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][26],koumoku26[tekipart[j][26]]);strcpy(buf,sfl);LvItem.iSubItem = 29;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][27],koumoku27[tekipart[j][27]]);strcpy(buf,sfl);LvItem.iSubItem = 30;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][28],koumoku28[tekipart[j][28]]);strcpy(buf,sfl);LvItem.iSubItem = 31;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][29]);	strcpy(buf,sfl);LvItem.iSubItem = 33;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
				}
			}else{
				sfl.Format("%03d",tekipart[j][0]);	strcpy(buf,sfl);	LvItem.pszText = buf;
				LvItem.iItem = m_lsettei.GetItemCount( );	LvItem.mask = LVIF_TEXT | LVIF_STATE;	LvItem.stateMask = LVIS_FOCUSED | LVIS_SELECTED;LvItem.state = 0;	LvItem.iSubItem = 0;	LvItem.cchTextMax = 512;//strlen(buf);
				idItem = m_lsettei.InsertItem(&LvItem);
				LvItem.iItem = idItem;
				strcpy(buf,koumoku01[tekipart[j][1]]);	LvItem.iSubItem = 1;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%s",partcomment[j]);	strcpy(buf,sfl);LvItem.iSubItem = 2;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%d,%d",tekipart[j][2],tekipart[j][3]);	strcpy(buf,sfl);LvItem.iSubItem = 3;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][4],koumoku04[tekipart[j][4]]);strcpy(buf,sfl);	LvItem.iSubItem = 4;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][5]);	strcpy(buf,sfl);LvItem.iSubItem = 5;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][6]);	strcpy(buf,sfl);LvItem.iSubItem = 6;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][7],koumoku07[tekipart[j][7]]);strcpy(buf,sfl);	LvItem.iSubItem = 7;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][8]);	strcpy(buf,sfl);LvItem.iSubItem = 8;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
//				sfl.Format("%02d:%s",tekipart[j][9],koumoku07[tekipart[j][9]]);strcpy(buf,sfl);LvItem.iSubItem = 10;
//				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][10],koumoku10[tekipart[j][10]]);strcpy(buf,sfl);LvItem.iSubItem = 11;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
//				sfl.Format("%02d:%s",tekipart[j][11],koumoku11[tekipart[j][11]]);strcpy(buf,sfl);LvItem.iSubItem = 12;
//				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
//				sfl.Format("%02d:%s",tekipart[j][12],koumoku12[tekipart[j][12]]);strcpy(buf,sfl);LvItem.iSubItem = 13;
//				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][13],koumoku13[tekipart[j][13]]);strcpy(buf,sfl);LvItem.iSubItem = 14;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][14],koumoku14[tekipart[j][14]]);strcpy(buf,sfl);LvItem.iSubItem = 15;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][15],koumoku15[tekipart[j][15]]);strcpy(buf,sfl);LvItem.iSubItem = 16;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][16],koumoku16[tekipart[j][16]]);strcpy(buf,sfl);LvItem.iSubItem = 17;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][17]);	strcpy(buf,sfl);LvItem.iSubItem = 18;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][18],koumoku18[tekipart[j][18]]);strcpy(buf,sfl);LvItem.iSubItem = 19;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][19],koumoku19[tekipart[j][19]]);strcpy(buf,sfl);LvItem.iSubItem = 20;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][20]);	strcpy(buf,sfl);LvItem.iSubItem = 22;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][21],koumoku21[tekipart[j][21]]);strcpy(buf,sfl);LvItem.iSubItem = 23;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][22]);	strcpy(buf,sfl);LvItem.iSubItem = 24;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][23]);	strcpy(buf,sfl);LvItem.iSubItem = 25;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][24]);	strcpy(buf,sfl);LvItem.iSubItem = 26;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][25],koumoku25[tekipart[j][25]]);strcpy(buf,sfl);LvItem.iSubItem = 28;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][26],koumoku26[tekipart[j][26]]);strcpy(buf,sfl);LvItem.iSubItem = 29;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][27],koumoku27[tekipart[j][27]]);strcpy(buf,sfl);LvItem.iSubItem = 30;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][28],koumoku28[tekipart[j][28]]);strcpy(buf,sfl);LvItem.iSubItem = 31;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][29]);	strcpy(buf,sfl);LvItem.iSubItem = 33;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
			}
		}
//	}

	//描画
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->Draw();
}

void CTekiConfig::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	save.tekidata[settei][0]=x;
	save.tekidata[settei][1]=y;
	save.tekidata[settei][3]=tekino;
	save.tekidata[settei][2]=(DWORD)(m_syurui.GetCurSel())+((DWORD)m_haiti.GetCheck()<<15);
	save.tekidata[settei][4]=selectflag[0];
	save.tekidata[settei][5]=selectflag[1];
	save.tekidata[settei][6]=selectflag[2];
	save.tekidata[settei][7]=selectflag[3];
	CString s;m_memo.GetWindowText(s);
	strcpy(cm[settei].c,s);
	if(newf) save.tekikazu++;
	CDialog::OnOK();
}

void CTekiConfig::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int Lindex=-1,ff=0;
	Lindex=m_litiran.GetNextItem(Lindex,LVNI_ALL |LVNI_SELECTED);
	if(Lindex==-1) return;
	
	SetTekiNo(atoi(m_litiran.GetItemText(Lindex,0)),selectflag,FALSE);
	SetTekiNo(atoi(m_litiran.GetItemText(Lindex,0)),itiranflag,TRUE);
	m_litiran.DeleteItem(Lindex);
	ttee[0]=selectflag[0];
	ttee[1]=selectflag[1];
	ttee[2]=selectflag[2];
	ttee[3]=selectflag[3];
	ListReDraw(FALSE);	
	*pResult = 0;
}

void CTekiConfig::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int Lindex=-1,ff=0;
	Lindex=m_lsettei.GetNextItem(Lindex,LVNI_ALL |LVNI_SELECTED);
	if(Lindex==-1) return;
	
	SetTekiNo(atoi(m_lsettei.GetItemText(Lindex,0)),selectflag,TRUE);
	SetTekiNo(atoi(m_lsettei.GetItemText(Lindex,0)),itiranflag,FALSE);
//	m_litiran.DeleteItem(Lindex);
	ttee[0]=selectflag[0];
	ttee[1]=selectflag[1];
	ttee[2]=selectflag[2];
	ttee[3]=selectflag[3];
	ListReDraw();	
	
	*pResult = 0;
}

void CTekiConfig::OnButton1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CTekiSelect a;
	a.input=settei;
	ShowWindow(SW_HIDE);
	if(a.DoModal()==IDOK){
		tekino=a.ret;
		CString s;s.Format("呼ぶ敵番号:%d",tekino); m_tnn.SetWindowText(s);
	}
	ShowWindow(SW_SHOW);
}

void CTekiConfig::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->Draw();
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
}

void CTekiConfig::OnIdo() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	EndDialog(155);	
}

void CTekiConfig::OnDel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	EndDialog(444);	
	
}
