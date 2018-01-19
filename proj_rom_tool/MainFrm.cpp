// MainFrm.cpp : CMainFrame クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "proj_rom.h"
#include "MENU.h"
#include "MainFrm.h"
#include "TekiConfig.h"
#include "TekiSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_COMMAND(IDM_END, OnEnd)
	ON_COMMAND(IDM_LOAD, OnLoad)
	ON_COMMAND(IDM_SAVE, OnSave)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_syoki, Onsyoki)
	ON_COMMAND(IDM_MENU, OnMenu)
	ON_COMMAND(IDM_1, On1)
	ON_COMMAND(IDM_2, On2)
	ON_COMMAND(IDM_4, On4)
	ON_COMMAND(IDM_8, On8)
	ON_COMMAND(IDM_16, On16)
	ON_COMMAND(IDM_TEKI1, OnTeki1)
//	ON_COMMAND(IDM_TEKI2, OnTeki2)
	ON_COMMAND(IDM_CLEAR, OnClear)
	ON_WM_MBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの構築/消滅

CMainFrame::CMainFrame()
{
	// TODO: この位置にメンバの初期化処理コードを追加してください。
	fs=FALSE;
	mousex=mousey=100;
	xx=yy=100;xxs=yys=0;
	movef=endf=FALSE;
	gamenx=1024; gameny=768;
	kasoku=1.5;
	gensoku=(float)1.05;
	juryoku=(float)1.10;
	limit=10;
	save.startx=0;
	save.starty=0;
	save.mapx=256;
	save.mapy=256;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: この位置で CREATESTRUCT cs を修正して、Window クラスやスタイルを
	//       修正してください。

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メッセージ ハンドラ

//筆数表記
BOOL CMainFrame::Create(const CString& sTitle)
{
	CString sClassName;

	sClassName = AfxRegisterWndClass(NULL ,
    LoadCursor(NULL, IDC_ARROW),
    (HBRUSH)::GetStockObject(BLACK_BRUSH),
    LoadIcon(AfxGetInstanceHandle(),
    MAKEINTRESOURCE(IDR_MAINFRAME)));

	int aa;
	aa=GetSystemMetrics(SM_CXFIXEDFRAME);//3
	aa=GetSystemMetrics(SM_CYCAPTION);//19
	aa=GetSystemMetrics(SM_CYMENU);//19
	aa=GetSystemMetrics(SM_CXEDGE);//2

 return /*CFrameWnd::*/CreateEx(WS_EX_APPWINDOW ,sClassName, sTitle,
	  (WS_VISIBLE | WS_POPUP | WS_SYSMENU|WS_OVERLAPPED | (fs==FALSE)*WS_CAPTION),
	  0,0,
	  (int)gamenx+(GetSystemMetrics(SM_CXFIXEDFRAME)+GetSystemMetrics(SM_CXEDGE))*2,
	  (int)gameny+(fs==FALSE)*(
	   GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYMENU)+(GetSystemMetrics(SM_CYFIXEDFRAME)+GetSystemMetrics(SM_CYEDGE))*2
	   )
	  ,NULL,NULL,NULL);
	
}

void CMainFrame::Init()
{
/*	if(fs==FALSE)
	{
		CRect gr;
		int gx=0,gy=0;
		for(;;)
		{
			SetWindowPos(NULL,0,0,(int)gamenx+gx,(int)gameny+gy,SWP_NOZORDER|SWP_NOMOVE);
			GetClientRect(&gr);
			if(gr.right<(int)gamenx)gx++;
			if(gr.bottom<(int)gameny)gy++;
			if(gr.right==(int)gamenx&&gr.bottom==(int)gameny) break;
		}
//		chacha.timing(30,TRUE);
	}
*/
	chacha.m_dwScreenW=(int)gamenx;
	chacha.m_dwScreenH=(int)gameny;
	chacha.CreateD3D(GetSafeHwnd());
	chacha_a.InitDXSound(GetSafeHwnd());
	mode=0;
	ttee[0]=ttee[1]=ttee[2]=ttee[3]=0;
}

datasave save;
comment cm[128];
int xxx=0,yyy=0,spp2=1;
int spp[16][16];

void CMainFrame::Idou()
{
	if(movef){//移動計算
		int x=mousex/16+xxx;
		int y=mousey/16+yyy;
		for(int i=0;i<masu;i++) for(int j=0;j<masu;j++){
			if(x+i<256 && y+j<256)
				save.mapdata[x+i][y+j]=spp[i][j];
		}
	}
}
void CMainFrame::GameProc()
{
	chacha.InitGraphic();
//	chacha_a.WaveDAStart("Y3BG17.ogg");
	ZeroMemory(&save,sizeof save);
	save.mapx=256;
	save.mapy=256;
	for(int i=0;i<256;i++)
		for(int j=0;j<256;j++)
			save.mapdata[i][j]=0;
	for(i=0;i<16;i++)
		for(int j=0;j<16;j++)
			spp[i][j]=1;
	masu=1;
	for(;;){
		Idou();
		if(endf) break;
		Draw();
		chacha.DoEvent();
		Sleep(5);
	}
}

char ssss[][50]={{"0:敵消滅"},{"1:倒された位置"},{"2:元データ"},{"3:ミッションクリア条件１"},{"4：ペナルティ+1"},{"5："},{"6："},{"7："},{"8："},{"9：ゲームオーバー"}};

void CMainFrame::Draw()
{

		CString s;int i;
		chacha.Begin();
		for(i=0;i<32+16;i++){
			for(int j=0;j<24+24;j++){
				if(i+xxx>=0 && j+yyy>=0 && i+xxx<256 && j+yyy<256){
					chacha.PartDraw((i)*16+8,(j)*16+8,15*32,15*32,0,FALSE);
					chacha.PartDraw((i)*16+8,(j)*16+8,(save.mapdata[i+xxx][j+yyy]%16)*32,(save.mapdata[i+xxx][j+yyy]/16)*32,0,FALSE);
				}
			}
		}
		chacha.PartDraw(512+256,0,0,0,0,FALSE,TRUE);
		for(i=0;i<masu;i++) for(int j=0;j<masu;j++)
			chacha.PartDraw(930-15-16*8+i*16,420+j*16,(spp[i][j]%16)*32,(spp[i][j]/16)*32,0,FALSE);
		if(mousex>767){
			chacha.PartDraw(930-10,400,(spp2%16)*32,(spp2/16)*32,0,FALSE);
		}else{
			s.Format("X:%3d Y:%3d MAP位置",mousex/16+xxx,mousey/16+yyy);
			chacha.Text(s,780,540   ,D3DCOLOR_XRGB(255,255,255));
			s.Format("X:%3d(～%3d) Y:%3d(～%3d)",xxx,xxx+31+16,yyy,yyy+23+24);
			chacha.Text(s,780,560   ,D3DCOLOR_XRGB(255,255,255));
		}
		{
			int ii=save.startx-xxx,jj=save.starty-yyy;
			if(ii<32+16 && jj<24+24)
				chacha.JikiDraw(ii*16+8,jj*16+8,0,0);
		}

		for(i=0;i<save.tekikazu;i++){
			int ii=(int)save.tekidata[i][0]-xxx,jj=(int)save.tekidata[i][1]-yyy;
			if(ii<32+16 && jj<24+24){//敵描画
				if(mode==2||GetAsyncKeyState(VK_CONTROL) < 0||GetAsyncKeyState(VK_SHIFT) < 0){
					chacha.Text("○",ii*16,jj*16,D3DCOLOR_XRGB(255,255,255));
				}
				for(int j=0;j<128;j++){
					if(GetTekiNo(i,j)){
						chacha.TekiDraw(ii*16+8,jj*16+8,(int)tekipart[j][2]*32,(int)tekipart[j][3]*32,0,FALSE,TRUE,255-(save.tekidata[i][2]>>15)*128);
					}
				}
//				chacha.TekiDraw(ii*16+8,jj*16+8,0,0);
			}
		}
		s="Ctrl+で敵配置・編集\nShift+で敵削除";
		chacha.Text(s,780,500,D3DCOLOR_XRGB(255,255,255));
		s.Format("スタート位置:X:%3d Y:%3d",save.startx,save.starty);
		chacha.Text(s,780,590+20,D3DCOLOR_XRGB(255,255,255));
		s.Format("マップサイズ:X:1-%d Y:1-%d",save.mapx,save.mapy);
		chacha.Text(s,780,590+40,D3DCOLOR_XRGB(255,255,255));
		s.Format("選択中のパーツ番号 %3d",spp[0][0]);
		chacha.Text(s,780,590+60,D3DCOLOR_XRGB(255,255,255));
		s.Format("編集単位 %dx%dマス単位",masu,masu);
		chacha.Text(s,780,590+80,D3DCOLOR_XRGB(255,255,255));
		if(mode==1){
			s.Format("スタート地点となる場所をクリックしてください");
			chacha.Text(s,mousex,mousey+32,D3DCOLOR_XRGB(255,255,255));
		}else
		if(mode==2||GetAsyncKeyState(VK_CONTROL) < 0){
			s.Format("敵を配置する位置または編集する敵をクリックしてください");
			chacha.Text(s,mousex,mousey+32,D3DCOLOR_XRGB(255,255,255));
		}else
		if(mode==4){
			s.Format("新しく配置し直す敵の位置をクリックしてください");
			chacha.Text(s,mousex,mousey+32,D3DCOLOR_XRGB(255,255,255));
		}else
		if(GetAsyncKeyState(VK_SHIFT) < 0){
			s.Format("削除したい敵をクリックしてください");
			chacha.Text(s,mousex,mousey+32,D3DCOLOR_XRGB(255,255,255));
		}
		if(mode==3){
			//敵確認表示
			for(int j=0;j<128;j++){
				if(GetTekiNo2(j)){
					chacha.TekiDraw(960,720,(int)tekipart[j][2]*32,(int)tekipart[j][3]*32,0,FALSE);
				}
			}
		}else
			for(i=0;i<save.tekikazu;i++){
				int ii=xxx+mousex/16,jj=yyy+mousey/16;
				if((int)save.tekidata[i][0]==ii && (int)save.tekidata[i][1]==jj){
					s.Format("敵番号:%d[%s]\nコメント:%s",i,ssss[save.tekidata[i][2]&0x7fff],cm[i].c);
					chacha.Text(s,mousex,mousey+64,D3DCOLOR_ARGB(255-(save.tekidata[i][2]>>15)*128,255,255,255));
				}
			}
		chacha.End();
}


void CMainFrame::PostNcDestroy() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	CFrameWnd::PostNcDestroy();
	((CProj_romApp*)AfxGetApp())->m_pMainWnd=NULL;
//	delete this;
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	mousex=point.x;
	mousey=point.y;
	if(point.x<768)	{}
	else{
		spp2=((point.x-768)/16)+(point.y/16)*16;
	}
	CFrameWnd::OnMouseMove(nFlags, point);
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if(point.x<768){
		if(mode==0){
			if(GetAsyncKeyState(VK_CONTROL) < 0)
				mode=2;
			else if(GetAsyncKeyState(VK_SHIFT) < 0)
				mode=5;
			else
				movef=TRUE;
		}
		if(mode==1){
			save.startx=mousex/16+xxx; save.starty=mousey/16+yyy;
			mode=0;
		}
		if(mode==2){
			mode=3;
			int i;
			for(i=0;i<save.tekikazu;i++){
				int ii=xxx+mousex/16,jj=yyy+mousey/16;
				if((int)save.tekidata[i][0]==ii && (int)save.tekidata[i][1]==jj){
					break;
				}
			}
			if(i==save.tekikazu){
				CTekiSelect b;
				b.input=-1;
				if(b.DoModal()==15951){//New
					CTekiConfig a;
					a.settei=save.tekikazu;
					a.x=mousex/16+xxx;
					a.y=mousey/16+yyy;
					a.newf=TRUE;
					ttee[0]=ttee[1]=ttee[2]=ttee[3]=0x0;
					a.ttee=ttee;
					a.DoModal();
				}else{
					if(b.ret!=-1){//既存の敵を使う
						memcpy(save.tekidata[save.tekikazu],save.tekidata[b.ret],sizeof(DWORD)*8);
						memcpy(cm[save.tekikazu].c,cm[b.ret].c,1024);
						save.tekidata[save.tekikazu][0]=mousex/16+xxx;
						save.tekidata[save.tekikazu][1]=mousey/16+yyy;
						save.tekikazu++;					
					}
				}
				mode=0;
			}else{//編集
				CTekiConfig a;
				a.settei=i;
				a.x=mousex/16+xxx;
				a.y=mousey/16+yyy;
				a.newf=FALSE;
				ttee[0]=save.tekidata[i][4];
				ttee[1]=save.tekidata[i][5];
				ttee[2]=save.tekidata[i][6];
				ttee[3]=save.tekidata[i][7];
				a.ttee=ttee;
				int r=a.DoModal();
				iti=a.settei;
				mode=0;
				if(r==155){//移動
					mode=4;
				}
				if(r==444){//削除
					mode=0;
					for(int i=0;i<save.tekikazu;i++){
						if((int)save.tekidata[i][3]>iti)save.tekidata[i][3]--;
					}
//					for(i=iti;i<save.tekikazu;i++){
						for(int j=iti;j<save.tekikazu-1;j++){
							memcpy(save.tekidata[j],save.tekidata[j+1],sizeof(DWORD)*8);
							memcpy(cm[j].c,cm[j+1].c,1024);
						}
			//		}
					save.tekikazu--;
				}
			}
		}else if(mode==4){
			save.tekidata[iti][0]=mousex/16+xxx;
			save.tekidata[iti][1]=mousey/16+yyy;
			mode=0;
		}
		if(mode==5){
			mode=0;
			int i;
			for(i=0;i<save.tekikazu;i++){
				int ii=xxx+mousex/16,jj=yyy+mousey/16;
				if((int)save.tekidata[i][0]==ii && (int)save.tekidata[i][1]==jj){
					break;
				}
			}
			if(i!=save.tekikazu){iti=i;
				for(int i=0;i<save.tekikazu;i++){
					if((int)save.tekidata[i][3]>iti)save.tekidata[i][3]--;
				}
//				for(i=iti;i<save.tekikazu;i++){
					for(int j=iti;j<save.tekikazu-1;j++){
						memcpy(save.tekidata[j],save.tekidata[j+1],sizeof(DWORD)*8);
						memcpy(cm[j].c,cm[j+1].c,1024);
					}
//				}
				save.tekikazu--;
			}
		}
	}else{
		int x=(point.x-768)/16;
		int y=(point.y/16);
		for(int i=0;i<masu;i++) for(int j=0;j<masu;j++){
			if(x+i<16 && y+j<16)
				spp[i][j]=x+i+(y+j)*16;
		}
	}
	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	movef=FALSE;
	
	CFrameWnd::OnLButtonUp(nFlags, point);
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if(nChar==VK_ESCAPE) endf=TRUE;
	if(nChar==VK_UP) yyy--;
	if(nChar==VK_DOWN) yyy++;
	if(nChar==VK_LEFT) xxx--;
	if(nChar==VK_RIGHT) xxx++;
	if(nChar=='1') masu=1;
	if(nChar=='2') masu=2;
	if(nChar=='3') masu=4;
	if(nChar=='4') masu=8;
	if(nChar=='5') masu=16;
	if(xxx<0) xxx=0;
	if(yyy<0) yyy=0;
	if(xxx>(save.mapx-32-16)) xxx=save.mapx-32-16;
	if(yyy>(save.mapy-24-24)) yyy=save.mapy-24-24;
	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnEnd() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	endf=TRUE;
}

void CMainFrame::OnLoad() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CFileDialog dlg(TRUE,"*.map","*.map",
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"面ﾌｧｲﾙ(*.map)|*.map||",
					this);
	int rr = dlg.DoModal();
	if(rr == IDOK)
	{
		CString BMP = dlg.GetPathName();
		CString BMPF = dlg.GetFileName();
		CFile file;
		ZeroMemory(&save,sizeof save);
		if(file.Open(BMP,CFile::shareDenyNone | CFile::modeRead ,NULL)==TRUE){
			file.Read(&save,sizeof save);
			file.Close();
			xxx=yyy=0;
		}
		ZeroMemory(&cm,sizeof (comment)*128);
		if(file.Open(BMP+".comment",CFile::shareDenyNone | CFile::modeRead ,NULL)==TRUE){
			file.Read(&cm,sizeof(comment)*128);
			file.Close();
		}
	}
	
}

void CMainFrame::OnSave() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CFileDialog dlg(FALSE,"*.map","*.map",
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"面ﾌｧｲﾙ(*.map)|*.map||",
					this);
	int rr = dlg.DoModal();
	if(rr == IDOK)
	{
		CString BMP = dlg.GetPathName();
		CString BMPF = dlg.GetFileName();
		CFile file;
		if(file.Open(BMP,CFile::shareDenyNone | CFile::modeWrite | CFile::modeCreate ,NULL)==TRUE){
			file.Write(&save,sizeof save);
			file.Close();
			OnSaveBMP(BMP);
		}
		if(file.Open(BMP+".comment",CFile::shareDenyNone | CFile::modeWrite | CFile::modeCreate ,NULL)==TRUE){
			file.Write(&cm,sizeof(comment)*128);
			file.Close();
		}
	}
}

void CMainFrame::OnSaveBMP(CString s0) 
{
	BITMAPFILEHEADER a;
	BITMAPINFOHEADER b;
	a.bfType='M'*256+'B';
	a.bfSize=256*256*4+sizeof a + sizeof b;
	a.bfReserved1=0;
	a.bfReserved2=0;
	a.bfOffBits=sizeof a + sizeof b;
	b.biSize=40;
	b.biWidth=256;
	b.biHeight=256;
	b.biPlanes=1;
	b.biBitCount=32;
	b.biCompression=0;
	b.biSizeImage=3780;
	b.biXPelsPerMeter=3780;
	b.biYPelsPerMeter=3780;
	b.biClrImportant=0;
	b.biClrUsed=0;
	CFile file;
	CString s; s=s0.Left(s0.GetLength()-3); s+="bmp";
	if(file.Open(s,CFile::shareDenyNone | CFile::modeWrite | CFile::modeCreate ,NULL)==TRUE){
		file.Write(&a,sizeof a);
		file.Write(&b,sizeof b);
		for(int i=0;i<256;i++) for(int j=0;j<256;j++){
			BYTE d[4];
			int k=255-i;
			if(save.mapdata[j][k]==0||!(j<=save.mapx&&k<=save.mapy)){
				d[0]=69;d[1]=51;d[2]=31;d[3]=0;
//				d[0]=0;d[1]=0;d[2]=0;d[3]=0;
			}else{
				d[0]=15;d[1]=200;d[2]=30;d[3]=0;
			}
			file.Write(&d,4);
		}
		file.Close();
	}
	
}

void CMainFrame::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if(mode!=0) {mode=0;} else{
	if(point.x<768){
//		spp=((point.x)/64)+(point.y/32)*16;
		int x=mousex/16+xxx;
		int y=mousey/16+yyy;
		for(int i=0;i<masu;i++) for(int j=0;j<masu;j++){
			if(x+i<256 && y+j<256)
				spp[i][j]=save.mapdata[x+i][y+j];
		}
	}	
	}
	CFrameWnd::OnRButtonDown(nFlags, point);
}

void CMainFrame::Onsyoki() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	mode=1;
}

void CMainFrame::OnMenu() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CMENU a;
	a.DoModal();
}

void CMainFrame::On1() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	masu=1;
}

void CMainFrame::On2() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	masu=2;	
}

void CMainFrame::On4() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	masu=4;
}

void CMainFrame::On8() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	masu=8;	
}

void CMainFrame::On16() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	masu=16;
}

void CMainFrame::OnTeki1() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	mode=2;
}

void CMainFrame::OnTeki2() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
}

void CMainFrame::OnClear() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	
}

void CMainFrame::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	CFrameWnd::OnMButtonDown(nFlags, point);
}
BOOL CMainFrame::GetTekiNo(int teki,int no)
{
	DWORD pp=1;
	DWORD n=no/32;
	if( save.tekidata[teki][4+n]&(pp<<(no%32)) ) return TRUE;
	return FALSE;
}
BOOL CMainFrame::GetTekiNo2(int no)
{
	DWORD pp=1;
	int n=no/32;
	if( ttee[n]&(pp<<(no%32)) ) return TRUE;
	return FALSE;
}

BOOL CMainFrame::DestroyWindow() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if(endf)
		return CFrameWnd::DestroyWindow();
	else { endf=TRUE; return FALSE; }
	
	return CFrameWnd::DestroyWindow();
}
