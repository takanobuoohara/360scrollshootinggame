#include "stdafx.h"
#include "proj_rom.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//筆数表記
BOOL CMainFrame::Create(const CString& sTitle)
{
	CString sClassName;
	sClassName = AfxRegisterWndClass(NULL,LoadCursor(NULL, IDC_ARROW),
    (HBRUSH)::GetStockObject(BLACK_BRUSH),LoadIcon(AfxGetInstanceHandle(),
    MAKEINTRESOURCE(IDR_MAINFRAME)));
	chacha.maxx=GetSystemMetrics(SM_CXSCREEN);
	chacha.maxy=GetSystemMetrics(SM_CYSCREEN);
	fs=FALSE;
	mousex=mousey=100;xx=yy=100;xxs=yys=0;
	movef=endf=FALSE;
	kasoku=0.16f;
	gensoku=(float)1.002;
	juryoku=(float)1.02;
	limit=5;
	srand( (unsigned)time( NULL ) );
 return CFrameWnd::CreateEx(WS_EX_APPWINDOW ,sClassName, sTitle,
	  (WS_VISIBLE | WS_POPUP | (fs==FALSE)*WS_SYSMENU|(fs==FALSE)*WS_CAPTION),GetSystemMetrics(SM_CXSCREEN)/2-ftol(gamenx)/2,GetSystemMetrics(SM_CYSCREEN)/2-ftol(gameny)/2,
	  ftol(gamenx)+(GetSystemMetrics(SM_CXFIXEDFRAME)+GetSystemMetrics(SM_CXEDGE))*2,
	  ftol(gameny)+(fs==FALSE)*(GetSystemMetrics(SM_CYCAPTION)+(GetSystemMetrics(SM_CYFIXEDFRAME)+GetSystemMetrics(SM_CYEDGE))*2)
	  ,NULL,NULL,NULL);
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
CWinThread *thi;
BOOL CMainFrame::Init()
{
	ShowCursor(FALSE);
	chacha.chacha_a = &chacha_a;
	chacha.chacha_i = &chacha_i;
	fc.chacha   = &chacha;
	fc.chacha_a = &chacha_a;
	fc.chacha_i = &chacha_i;
	fc.mf       = this;
	chacha.m_dwScreenW=ftol(gamenx);
	chacha.m_dwScreenH=ftol(gameny);
	chacha.hwnd=GetSafeHwnd();
	if(chacha.CreateD3D(GetSafeHwnd())==FALSE) return FALSE;
//	for(;endf==0;){
//		chacha.DoEvent();
//	}
//	return FALSE;
	if(chacha_a.InitDXSound(GetSafeHwnd())==FALSE) return FALSE;
	if(chacha_i.InitDInput(AfxGetApp()->m_hInstance,GetSafeHwnd())==FALSE) return FALSE;
	if ( FAILED(chacha_i.InitMouse()) ) return (FALSE);
	if ( FAILED(chacha_i.InitKeyboard()) ) return (FALSE);
	chacha_i.InitJoypad();
	if ( FAILED(chacha.InitGraphic(0))) return (FALSE);
	loadf=loadf2=0;
	thi=AfxBeginThread(Loading,(LPVOID)this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	thi->m_bAutoDelete=TRUE;
	thi->ResumeThread();
	if ( FAILED(chacha.InitGraphic(1))){
		loadf=1;
		for(int i=0;i<15;i++){
			chacha.DoEvent();
			if(loadf2){ break;}
			Sleep(100);
		}
		return (FALSE);
	}
	chacha_a.WaveSet("okok.wav",1);
	chacha_a.WaveSet("CANCEL.WAV",2);
	chacha_a.WaveSet("SELECT.WAV",3);
	chacha_a.WaveSet("logo.wav",6);
	chacha_a.WaveSet("s36NewRecord.wav",4);
	chacha_a.WaveSet("バルカン.wav",10);
	chacha_a.WaveSet("ミサイル.wav",11);
	chacha_a.WaveSet("機雷.wav",12);
	chacha_a.WaveSet("爆発1.wav",13);
	chacha_a.WaveSet("thunder.wav",14);
	chacha_a.WaveSet("地表ダメ.wav",15);
	chacha_a.WaveSet("自機ダメ.wav",16);
	chacha_a.WaveSet("敵破壊.wav",17);
	chacha_a.WaveSet("自機破壊.wav",18);
	chacha_a.WaveSet("S19アイテム取得.wav",19);
	chacha_a.WaveSet("S20通常弾.wav",20);
	chacha_a.WaveSet("S21反射弾.wav",21);
	chacha_a.WaveSet("s22ツイン砲.wav",22);
//	chacha_a.WaveSet("s22ツイン砲.wav",23);
	chacha_a.WaveSet("s24レーザー.wav",24);
	chacha_a.WaveSet("s25敵ミサイル.wav",25);
	chacha_a.WaveSet("s26敵機雷.wav",26);
	chacha_a.WaveSet("s27敵ウェーヴ弾.wav",27);
	chacha_a.WaveSet("S28８WAY.wav",28);
	chacha_a.WaveSet("s22ツイン砲.wav",29);
	chacha_a.WaveSet("ロックオン.wav",30);
	chacha_a.WaveSet("敵に当たった.wav",31);
	chacha_a.WaveSet("s32.wav",32);
	chacha_a.WaveSet("s33.wav",33);
	chacha_a.WaveSet("s34ターゲット破壊.wav",34);
	chacha_a.WaveSet("s35自機ダメージ.wav",35);

	//
	loadf=1;
	for(int i=0;i<15;i++){
		chacha.DoEvent();
		if(loadf2){ break;}
		Sleep(100);
	}
	
/*	17：敵破壊
	18：自機破壊
	19：アイテム取得
	20：敵通常弾
	21：敵反射弾
	22：敵ツイン砲
	23：敵極太
	24：敵レーザー
	25：敵ミサイル
	26：敵機雷
	27：敵ウェーヴ弾
	28：敵８WAY弾
	29：敵誘導弾
	30：ホーミング　ロックオン
	31：
	*/
	gamenx_=gamenx; gameny_=gameny;;
	gamenx=1024; gameny=768;
	gamenx2=gamenx/2;	gameny2=gameny/2;
	gamenx5=gamenx/5;	gameny5=gameny/5;
	gamenx2_=gamenx_/2;	gameny2_=gameny_/2;
	gamenx5_=gamenx_/5;	gameny5_=gameny_/5;
	return TRUE;
}
CWinThread *th;

void CMainFrame::GameProc()
{
	tb[0]="easy"; tb[1]="normal"; tb[2]="hard";tb[3]="nightmare";tb[4]="inferno";
	chacha.playmode=GAME_PLAY;
	score=0;
	stage=0;romrom=0;
	replay=-1;repf=0;repsf=0;repf_=0;repsf_=0;demof=0;
	fadef2=0;
	uraura="";
	fps=FALSE;
	muteki=FALSE;
	mugen=FALSE;
	renmax=FALSE;
	stagemax=FALSE;
	HosiMake();
	bgm="";
	th=NULL;
	bgmth=FALSE;
	th=AfxBeginThread(oggplay,(LPVOID)this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	th->m_bAutoDelete=TRUE;
	th->ResumeThread();
	int r=4,ka;
	endff=0;
	KeyWait();
//	EndPlay();
//	score=1234;
//	RankigPlay();
//	goto gameend;
	LogoPlay();
	tamaf=0;
	ka=0;
title:
	replay=-1;repf=0;repsf=0;repf_=0;repsf_=0;
	score=0;
	tamaf=0;
	KeyWait();
	r=TitlePlay(ka);
	ka=1;
	tamaf=0;
	if(r==4) goto gameend;//終了
	if(r==99){//リプレイ
retr0:
		replay=1;repsf_=1;repf=0;repsf=0;repf_=0;
		if(GamePlay(stage)){
			save.nanido=rsv2.nanido;goto gameend;
		}else{
			replay=0;repf=0;repsf=0;repf_=0;
			if(mistartmodeA==99){
				replay=1;score=rsv1.score;goto retr0;
			}
			if(mistartmodeA==0){goto gameend;
			}
			save.nanido=rsv2.nanido;
			HosiMake();goto title;
		}
	}
	stage=1;	score=0;

	if(r<0)stage=-r;// goto gameend;//ステージセレクト -stage となっているので -を消してやること
	if(r==999) stage=999;
retr:
	replay=0;repf=0;repsf=0;repf_=0;repsf_=0;
retrr:
	endff=1;repf_=0;repsf_=0;
	scorerep=score;
	if(GamePlay(stage)){
	}else{
		endff=1;
		if(stage==999){HosiMake(); goto title;}
		//mistartmodefA 7:ゲームオーバー 5:クリア 99:リプレイ
		if(mistartmodeA==99){
			replay=1;score=scorerep;goto retrr;
		}
		if(mistartmodeA==7){
			if(clearf==1 && hitp[0][0]==0 && hitp[1][0]==0 && hitp[2][0]==0 && hitp[3][0]==0 && hitp[4][0]==0){
				UraInfo();HosiMake();goto title;
			}else{
				RankigPlay();	HosiMake();goto title;
			}
		}
		if(mistartmodeA==5){
			if(save.stagetime[stage-1][save.nanido]>timer || save.stagetime[stage-1][save.nanido]==-1) save.stagetime[stage-1][save.nanido]=timer;
			if(stage==STAGEMAX){
#if SD_BETA
#else
				EndPlay();
				for(int i=0;i<STAGEMAX;i++){if(save.stagetime[i][save.nanido]==-1) break;} if(i==STAGEMAX){
					if((save.nanido==2)&&(save.maxnanido<3)){save.maxnanido=3;Naido(0);}
					if((save.nanido==3)&&(save.maxnanido<4)){save.maxnanido=4;Naido(1);}
				}
#endif
				RankigPlay(); HosiMake();goto title;
			}
#if SD_BETA
#else
			for(int i=0;i<STAGEMAX;i++){if(save.stagetime[i][save.nanido]==-1) break;} if(i==STAGEMAX){
				if((save.nanido==2)&&(save.maxnanido<3)){save.maxnanido=3;Naido(0);}
				if((save.nanido==3)&&(save.maxnanido<4)){save.maxnanido=4;Naido(1);}
			}
#endif
			stage++;
			if(save.clearstage<stage)
				save.clearstage=stage;
			goto retr;
		}
		if(mistartmodeA==500){
			HosiMake();
			goto title;
		}
	}

gameend:
	endf=1;
	CString sss;
	::OutputDebugString("BGM Thread End..");
	for(int i=0;i<15;i++){
		chacha.DoEvent();
		sss.Format("%d ",i);
		::OutputDebugString(sss);
		if(th==NULL||bgmth==TRUE){::OutputDebugString("End ok!!"); break;}
		Sleep(100);
	}
	return;
}

UINT CMainFrame::ReplaySave(LPVOID pParam)
{
	CMainFrame* cf=(CMainFrame*)pParam;
	Sleep(10);
	fc.ReplayAutoSave();
	return 0;
}

UINT CMainFrame::Loading(LPVOID pParam)
{
	CMainFrame* cf=(CMainFrame*)pParam;
	cf->Loadingmain();
	cf->loadf2=1;
	return 0;
}

void CMainFrame::Loadingmain()
{
	DWORD f1=0,f2=0,f3=0,za4=0;
	int modeno=0;
	int modeno1=0;
	int modeno2=0;
	int modeno3=0;
	CString s;
	DWORD timen2=timen=1;
	if(chacha.DoEventChk(endf)==FALSE){endf=TRUE; return; }
	chacha.Timing64(f2);
	chacha.Timing64(chacha.fpstiming,FALSE);
	for(;;){
		chacha.GetInput();
		input();
		if(loadf) modeno2=1;
		modeno++;modeno1=modeno>32?64-modeno:modeno; modeno%=64;
		if(modeno2) modeno3++;
		if(modeno3>64) return;
		if(timen==1){//DirectX描画開始
			chacha.Begin();
			s="now loading";
			chacha.Moji32(s,s.GetLength(),0,1024-12*32,768-50,TRUE,modeno1*8-modeno3*4);
			chacha.End();
			if(!chacha.DoEventChk(endf)){endf=TRUE; break; }
			chacha.End(FALSE);//描画終了
			f1=chacha.Timing64(f2,FALSE);
			timen=GetTiming(f1);
			chacha.timing1((WORD)timen,FALSE,FALSE);
			timen2=timen;
			chacha.Timing64(f2,FALSE);
			f3=chacha.Timing64(chacha.fpstiming,FALSE);
		}else timen--;
	}
	return;
}


UINT CMainFrame::oggplay(LPVOID pParam)
{
	CMainFrame* cf=(CMainFrame*)pParam;
	for(;;){
		if(cf->endf) break;
		if(fadef2){
			cf->chacha_a.WaveDAEnd();
			cf->chacha_a.fnames="";
		}
		if(cf->bgm==""){
			Sleep(700); continue;
		}else{
			CString s=cf->bgm;
			cf->bgm="";
			fadef=0;fadede=0;
			cf->chacha_a.WaveDAEnd();
			Sleep(100);
			fadef=0;fadede=0;
			cf->chacha_a.WaveDAStart(s);
		}
	}
	cf->bgmth=TRUE;
	AfxEndThread(0);
	return 0;
}


void CMainFrame::Close()
{
	endf=1;
	chacha.DoEvent();
	chacha_a.ReleaseDXSound();
	chacha_i.ReleaseDInput();
	chacha.CleanUp();
	CFile f;
	if(f.Open("common.dat",CFile::shareDenyNone | CFile::modeWrite | CFile::modeCreate ,NULL)){
		f.Write(&save,sizeof commonsave);
		f.Close();
	}

}
void CMainFrame::HosiOkuMake()
{
	//0:奥からの距離 1:θ 2:速度
	float a,b;
	for(int j=0;j<256*(2*(3-save.star));j++){ haikei[j][0]=-1;}
	for(int k=0;k<800;k++)
		for(int j=0;j<256*(2*(3-save.star));j++){
			if(haikei[j][0]==-1&&(rand()%20==9)){
				haikei[j][0]=(rand()%200)+20;
				haikei[j][1]=(rand()%360);
				haikei[j][2]=(rand()%50)+1;
				haikei[j][4]=rand()%8;
			}else{
				if(haikei[j][0]!=-1){
					haikei[j][0]++;
					a=chacha.Getsin(haikei[j][1]*PI180)*(float)(haikei[j][0]*haikei[j][2])+512;
					b=chacha.Getcos(haikei[j][1]*PI180)*(float)(haikei[j][0]*haikei[j][2])+384;
		//			if(haikei[j][0]==500)haikei[j][0]=-1;
					if(a<0) haikei[j][0]=-1;
					if(a>1024) haikei[j][0]=-1;
					if(b<0) haikei[j][0]=-1;
					if(b>768) haikei[j][0]=-1;
				}
			}
		}
}
void CMainFrame::HosiOkuProc()
{
	float a,b;
	for(int j=0;j<256*(2*(3-save.star));j++){
		if(haikei[j][0]==-1&&(rand()%20==9)){
			haikei[j][0]=(rand()%200)+20;
			haikei[j][1]=(rand()%360);
			haikei[j][2]=(rand()%50)+1;
			haikei[j][4]=rand()%8;
		}else{
			if(haikei[j][0]!=-1){
				haikei[j][0]++;
				a=chacha.Getsin(haikei[j][1]*PI180)*(float)(haikei[j][0]*haikei[j][2])+512;
				b=chacha.Getcos(haikei[j][1]*PI180)*(float)(haikei[j][0]*haikei[j][2])+384;
	//			if(haikei[j][0]==500)haikei[j][0]=-1;
				if(a<0) haikei[j][0]=-1;
				if(a>1024) haikei[j][0]=-1;
				if(b<0) haikei[j][0]=-1;
				if(b>768) haikei[j][0]=-1;
			}
		}
	}
}
void CMainFrame::HosiOkuDraw()
{
	float a,b;
	for(int j=0;j<256*(2*(3-save.star));j++){
		if(haikei[j][0]!=-1){
			a=chacha.Getsin(haikei[j][1]*PI180)*(float)(haikei[j][0]*haikei[j][2])+512;
			b=chacha.Getcos(haikei[j][1]*PI180)*(float)(haikei[j][0]*haikei[j][2])+384;
			chacha.PartDrawA(a,b,haikei[j][4]*32,2*32,2,2,4,160);
		}
	}
}
void CMainFrame::HosiMake()
{
	for(int j=0;j<256*(2*(3-save.star));j++){ haikei[j][0]=haikei[j][1]=-1;haikei[j][2]=haikei[j][3]=haikei[j][4]=0;}
	for(int k=0;k<800;k++)
			for(j=0;j<256*(2*(3-save.star));j++){
			if(haikei[j][0]==-1&&(rand()%20==9)){
				if(rand()%200 > 100){
					haikei[j][0]=0;	haikei[j][1]=(rand()%768);
					haikei[j][2]=haikei[j][3]=(rand()%10)+1;
				}else{
					haikei[j][1]=0;	haikei[j][0]=(rand()%1024);
					haikei[j][2]=haikei[j][3]=(rand()%10)+1;
				}
				haikei[j][4]=rand()%8;
			}else{
				if(haikei[j][0]!=-1){
					haikei[j][0]+=haikei[j][2];haikei[j][1]+=haikei[j][3];
					if(haikei[j][0]>1024 || haikei[j][1]>768) haikei[j][0]=-1;
				}
			}
		}
}
//タイトル表示
BOOL CMainFrame::TitlePlay(int ka)
{
	CString s;
	DWORD f1=0,f2=0,f3=0;
	DWORD timen2=timen=1;
	int tamafb=tamaf,staget=save.clearstage,j;
	chacha.LoadWorkTex(0);tamaf=0;
	bgm="bgm04.ogg";
//	chacha_a.WaveDAStart("bgm04.ogg");
//	AfxBeginThread(oggplay,(LPVOID)this);

	float num,oldnum=-1,num2; int menuno=-1;
	float mox=0,moy=0;
	int ten=0,ten2,democnt=0;
	int za=0,za2=0,za3=255,za4=0,za5=0,za2_=0,za2__=0;
	float newy,oldy;
	if(chacha.DoEventChk(endf)==FALSE){endf=TRUE; return TRUE; }
	chacha.Timing64(f2);
	chacha.Timing64(chacha.fpstiming,FALSE);
	for(;;){
		chacha.GetInput();
		input();
		//マウスの選択範囲を決める
		mousex_=(float)mousex*gamenx/gamenx_;
		mousey_=(float)mousey*gameny/gameny_;
		if(mox!=mousex_ || moy!=mousey_) democnt=0;
		mox=mousex_;moy=mousey_;
		democnt++;
		if(democnt>2000){//35秒経過　デモ開始
			if(menuno==0){
				if(fc.Demo()){
					oldnum=-1,num2;menuno=-1;
					mox=0,moy=0;
					ten=0,ten2,democnt=0;
					za=0,za2=0,za3=255,za4=0,za5=0,za2_=0,za2__=0;
					chacha.Timing64(f2);
					chacha.Timing64(chacha.fpstiming,FALSE);
				}
			}
			democnt=0;
		}
		if(menuno!=0) democnt=0;
//		if(endf) return TRUE;
		//★処理
		for(j=0;j<256*(2*(3-save.star));j++){
			if(haikei[j][0]==-1&&(rand()%20==9)){
				if(rand()%200 > 100){
					haikei[j][0]=0;	haikei[j][1]=(rand()%768);
					haikei[j][2]=haikei[j][3]=(rand()%10)+1;
				}else{
					haikei[j][1]=0;	haikei[j][0]=(rand()%1024);
					haikei[j][2]=haikei[j][3]=(rand()%10)+1;
				}
				haikei[j][4]=rand()%8;
			}else{
				if(haikei[j][0]!=-1){
					haikei[j][0]+=haikei[j][2];haikei[j][1]+=haikei[j][3];
					if(haikei[j][0]>1024 || haikei[j][1]>768) haikei[j][0]=-1;
				}
			}
		}
		num=-1;
		ten++;ten=ten%128;ten2=(ten>64)?128-ten:ten;
		switch(menuno){
		case -4://つづきから
			if(za>64){
				return -staget;
			}else za++;
			break;
		case -3://はじめから
			if(za>64){
				return 0;
			}else za++;
			break;
		case -2://終了
			if(za>64){
				return 4;
			}else za++;
			break;
		case -1://一番最初
			democnt=0;
			if(tamaf){za=0;za2=0;menuno=0;				KeyWait();break;}
			if(za>64){
				if(za2>98){
					za=0;za2=0;menuno=0;break;
				}else za2++;
			}else za++;
			break;
		case 0:
			if(mousex_>384 && mousex_<384+256){
				if(mousey_>350      && mousey_<=350+48  ) num=0;
				if(mousey_>350+48   && mousey_<=350+48*2) num=1;
				if(mousey_>350+48*2 && mousey_<=350+48*3) num=2;
				if(mousey_>350+48*3 && mousey_<=350+48*4) num=3;
				if(mousey_>350+48*4 && mousey_<=350+48*5) num=4;
			}
			if(repf){//リプレイロード
				repf=0;
				if(fc.ReplayLoad()==99) return 99;
				
			}
			if(enhf){//自作ステージロード
				enhf=0;
				if(fc.EnhLoad()==99) return 999;
				
			}
			if(num==0 && tamaf && menuno!=-3){ menuno=-3;za=0;fadef=1;chacha_a.Play(1);break;}
			if(num==1 && tamaf){ menuno=4; za3=255; za4=0; chacha_a.Play(1);break;}
			if(num==2 && tamaf){ menuno=1; za3=255; za4=0; chacha_a.Play(1);break;}
			if(num==3 && tamaf){ menuno=7; za3=255; za4=0; chacha_a.Play(1);break;}
			if(num==4 && tamaf && menuno!=-2){ menuno=-2;za=0;fadef=1;chacha_a.Play(1);break;}
			if(oldnum!=num){
			
				if(za==0){za=3;}
				num2=num;
				oldy=oldnum*48;	newy=num2*48;
				if(num==-1){za=1;za2=200;num2=oldnum;oldy=oldnum*48;newy=oldnum*48;}else
					if(oldnum==-1){za=2;za2=0;oldy=num*48;	newy=num*48;chacha_a.Play(3);} else 	chacha_a.Play(3);
			}
			oldnum=num;
			switch(za){
			case 1:za2-=5;if(za2<=0) za=0;break;
			case 2:za2+=5;if(za2>=200) za=0;break;
			}
			{
				for(int i=0;i<5;i++){
					if(oldy>newy) oldy--;
					if(oldy<newy) oldy++;
				}
			}break;
		case 1://メニューからオプションへ
			za3-=5; za4+=5; if(za2>0) za2-=5; oldnum=-1;
			if(za3<0) menuno=2;num=-1;
			break;
		case 2:
			if(mousex_>460-10 && mousex_<460+105+10 && mousey_>550 && mousey_<=580  ) num=0;
			if((num==0 && tamaf)||(movef)){ menuno=3; za4=255; za3=0;chacha_a.Play(1); break;}
			if(tamafb==0){
				if(mousex_>407 && mousex_<407+75 && mousey_>355 && mousey_<355+30 && tamaf){save.bgmplayf=0;chacha_a.WaveDAEnd();}
				{
					for(int i=0;i<8;i++)
						if(mousex_>483+32.5f*(float)i && mousex_<483+75+32.5f*(float)i && mousey_>355 && mousey_<355+30 && tamaf){
							save.bgmvol=i+1;if(save.bgmplayf==0){save.bgmplayf=1;chacha_a.WaveDAStart("");}save.bgmplayf=1;chacha_a.Vol();}
				}
				if(mousex_>407 && mousex_<407+75 && mousey_>355+50 && mousey_<355+80 && tamaf)save.effectplayf=0;
				{
					for(int i=0;i<8;i++)
						if(mousex_>483+32.5f*(float)i && mousex_<483+75+32.5f*(float)i && mousey_>355+50 && mousey_<355+80 && tamaf){
							save.effectvol=i+1;save.effectplayf=1;}
				}
				if(mousex_>407 && mousex_<407+85  && mousey_>450 && mousey_<=450+30 && tamaf ) save.nanido=0;
				if(mousex_>500 && mousex_<500+120 && mousey_>450 && mousey_<=450+30 && tamaf ) save.nanido=1;
				if(mousex_>630 && mousex_<630+85  && mousey_>450 && mousey_<=450+30 && tamaf ) save.nanido=2;
				if(mousex_>720 && mousex_<720+175 && mousey_>450 && mousey_<=450+30 && tamaf && save.maxnanido>=3) save.nanido=3;
				if(mousex_>900 && mousex_<900+118 && mousey_>450 && mousey_<=450+30 && tamaf && save.maxnanido>=4) save.nanido=4;
				if(mousex_>407 && mousex_<407+113 && mousey_>500 && mousey_<=500+30 && tamaf ) save.gengo=0;
				if(mousex_>530 && mousex_<530+129 && mousey_>500 && mousey_<=500+30 && tamaf ) save.gengo=1;
			}tamafb=tamaf;
			if(oldnum!=num){
			
				if(za==0){za=3;}
				num2=num;
				oldy=oldnum*48;	newy=num2*48;
				if(num==-1){za=1;za2_=200;num2=oldnum;oldy=oldnum*48;newy=oldnum*48;}else
					if(oldnum==-1){za=2;za2_=0;oldy=num*48;	newy=num*48;chacha_a.Play(3);} else 	chacha_a.Play(3);
			}
			oldnum=num;
			switch(za){
			case 1:za2_-=5;if(za2_<=0) za=0;break;
			case 2:za2_+=5;if(za2_>=200) za=0;break;
			}
			break;
		case 3://プションからメニューへ
			za3+=5; za4-=5; if(za2_>0) za2_-=5; oldnum=-1;
			if(za4<0) menuno=0;num=-1;
			break;
		case 4://メニューから選択へ
			za3-=5; za4+=5; if(za2>0) za2-=5; oldnum=-1;
			if(za3<0) menuno=5;num=-1;
			break;
		case 5://選択
			//512-16,390     
			if(mousex_>460-10 && mousex_<460+105+10){
				if(mousey_>390      && mousey_<=390+32  ) num=0;
				if(mousey_>390+32   && mousey_<=390+32*2) num=1;
				if(mousey_>390+32*2 && mousey_<=390+32*3) num=2;
				if(mousey_>390+32*3 && mousey_<=390+32*4) num=3;
			}
			if(tamafb==0){
				if(num==0 && tamaf){ staget--;if(staget==0) staget=save.clearstage;}
				if(num==2 && tamaf){ staget++;if(staget==save.clearstage+1) staget=1;}
				if(num==1 && tamaf){ menuno=-4;za=0;fadef=1;chacha_a.Play(1);break;}
				if((num==3 && tamaf)||movef){ menuno=6;za4=255; za3=0;chacha_a.Play(1);break;}
			}tamafb=tamaf;
			if(oldnum!=num){
			
				if(za==0){za=3;}
				num2=num;
				oldy=oldnum*32;	newy=num2*32;
				if(num==-1){za=1;za2_=200;num2=oldnum;oldy=oldnum*32;newy=oldnum*32;}else
					if(oldnum==-1){za=2;za2_=0;oldy=num*32;	newy=num*32;chacha_a.Play(3);} else 	chacha_a.Play(3);
			}
			oldnum=num;
			switch(za){
			case 1:za2_-=5;if(za2_<=0) za=0;break;
			case 2:za2_+=5;if(za2_>=200) za=0;break;
			}
			{
				for(int i=0;i<5;i++){
					if(oldy>newy) oldy--;
					if(oldy<newy) oldy++;
				}
			}break;
		case 6://選択からメニューへ
			za3+=5; za4-=5; if(za2_>0) za2_-=5; oldnum=-1;
			if(za4<0) menuno=0;num=-1;
			break;
		case 7://メニューからランキングへ
			za3-=5; za4+=5; if(za2>0) za2-=5; oldnum=-1;
			if(za3<0) menuno=8;num=-1;
			break;
		case 8://ランキング
			if(mousex_>460-10 && mousex_<460+105+10){
				if(mousey_>768-48      && mousey_<=768-48+32  ) num=0;
			}
			if(tamafb==0){
				if((num==0 && tamaf)||movef){ menuno=9;za4=255; za3=0;chacha_a.Play(1);break;}
				if(tamaf) {menuno=10;za4=255; za5=0;chacha_a.Play(1);break;}
			}tamafb=tamaf;
			if(oldnum!=num){
				if(za==0){za=3;}
				num2=num;
				oldy=oldnum*32;	newy=num2*32;
				if(num==-1){za=1;za2_=200;num2=oldnum;oldy=oldnum*32;newy=oldnum*32;}else
					if(oldnum==-1){za=2;za2_=0;oldy=num*32;	newy=num*32;chacha_a.Play(3);} else 	chacha_a.Play(3);
			}
			oldnum=num;
			switch(za){
			case 1:za2_-=5;if(za2_<=0) za=0;break;
			case 2:za2_+=5;if(za2_>=200) za=0;break;
			}
			{
				for(int i=0;i<5;i++){
					if(oldy>newy) oldy--;
					if(oldy<newy) oldy++;
				}
			}break;
		case 9://ランキングからメニューへ
			za3+=5; za4-=5; if(za2_>0) za2_-=5; oldnum=-1;
			if(za4<0) menuno=0;num=-1;
			break;
		case 10://ランキングからスコアへ
			za5+=5; za4-=5; if(za2_>0) za2_-=5; oldnum=-1;
			if(za4<0) menuno=11;num=-1;
			break;
		case 11://スコア
			if(mousex_>460-10 && mousex_<460+105+10){
				if(mousey_>768-48      && mousey_<=768-48+32  ) num=0;
			}
			if(tamafb==0){
				if((num==0 && tamaf)||movef){ menuno=12;za5=255; za3=0;chacha_a.Play(1);break;}
				if(tamaf) {menuno=13;za5=255; za4=0;chacha_a.Play(1);break;}
			}tamafb=tamaf;
			if(oldnum!=num){
			
				if(za==0){za=3;}
				num2=num;
				oldy=oldnum*32;	newy=num2*32;
				if(num==-1){za=1;za2_=200;num2=oldnum;oldy=oldnum*32;newy=oldnum*32;}else
					if(oldnum==-1){za=2;za2_=0;oldy=num*32;	newy=num*32;chacha_a.Play(3);} else 	chacha_a.Play(3);
			}
			oldnum=num;
			switch(za){
			case 1:za2_-=5;if(za2_<=0) za=0;break;
			case 2:za2_+=5;if(za2_>=200) za=0;break;
			}
			{
				for(int i=0;i<5;i++){
					if(oldy>newy) oldy--;
					if(oldy<newy) oldy++;
				}
			}break;
		case 12://スコアからメニューへ
			za3+=5; za5-=5; if(za2_>0) za2_-=5; oldnum=-1;
			if(za5<0) menuno=0;num=-1;
			break;
		case 13://スコアからランキングへ
			za4+=5; za5-=5; if(za2_>0) za2_-=5; oldnum=-1;
			if(za5<0) menuno=8;num=-1;
			break;
		}
		if(timen==1){//DirectX描画開始
			chacha.Begin();
			//タイトル表示
			for(j=0;j<256*(2*(3-save.star));j++){
				if(haikei[j][0]!=-1)
					chacha.PartDrawA((float)haikei[j][0],(float)haikei[j][1],haikei[j][4]*32,2*32,2,2,4,160);

			}
			if(menuno==-1){
				if(ka)	chacha.Fade(255-za*4);
				for(int i=0;i<320/8;i++){
					chacha.PartDrawA((float)za2*12-1200     ,(float)i*8  ,0,i*8  ,512,4,10,255,0,0,512,2);
					chacha.PartDrawA((float)za2*12+512-1200 ,(float)i*8  ,0,i*8  ,512,4,11,255,0,0,512,2);
					chacha.PartDrawA((float)-za2*12+1200    ,(float)i*8+4,0,i*8+4,512,4,10,255,0,0,512,2);
					chacha.PartDrawA((float)-za2*12+512+1200,(float)i*8+4,0,i*8+4,512,4,11,255,0,0,512,2);
				}
			}else{
				if(menuno==7||menuno==8||menuno==9||menuno==10||menuno==11||menuno==12||menuno==13){
					chacha.PartDrawA(0,0,0,0,512,320,10,255-za4/2-za5/2,0,0,512,320);
					chacha.PartDrawA(512,0,0,0,512,320,11,255-za4/2-za5/2,0,0,512,320);
					chacha.PartDrawA(100,560,0,312,512,192,10,255-za4/2-za5/2,0,0,512,192);
					chacha.PartDrawA(612,560,0,312,512,192,11,255-za4/2-za5/2,0,0,512,192);
				}else{
					chacha.PartDrawA(0,0,0,0,512,320,10,255,0,0,512,320);
					chacha.PartDrawA(512,0,0,0,512,320,11,255,0,0,512,320);
					chacha.PartDrawA(100,560,0,312,512,192,10,255,0,0,512,192);
					chacha.PartDrawA(612,560,0,312,512,192,11,255,0,0,512,192);
				}
			}
			if(menuno==0||menuno==1||menuno==3||menuno==4||menuno==6||menuno==7||menuno==9||menuno==12||menuno==-2||menuno==-3||menuno==-4){//メインメニュー表示
				if(save.gengo==0){
					chacha.PartDrawA(384,350,0,0,256,250,12,za3,0,0,256,250);
					//選択枠表示
					chacha.PartDrawA(410,355+oldy,0,0,204,33,8,8+ten2*3-(200-za2),0,0);
					chacha.PartDrawA(405,333+oldy,268,90,10,60,12,za2,0,0,10,40);
					chacha.PartDrawA(415,333+oldy,278,90,190,60,12,za2,0,0,12,40);
					chacha.PartDrawA(605,333+oldy,290,90,10,60,12,za2,0,0,10,40);
				}else{
					s="start";
					chacha.Moji32(s,s.GetLength(),0,432,358,FALSE,za3);
					s="select";
					chacha.Moji32(s,s.GetLength(),0,416,358+48,FALSE,za3);
					s="config";
					chacha.Moji32(s,s.GetLength(),0,416,358+48*2,FALSE,za3);
					s="ranking";
					chacha.Moji32(s,s.GetLength(),0,400,358+48*3,FALSE,za3);
					s=" exit";
					chacha.Moji32(s,s.GetLength(),0,416,358+48*4,FALSE,za3);
					//選択枠表示
					chacha.PartDrawA(400,355+oldy,0,0,224,33,8,8+ten2*3-(200-za2),0,0);
					chacha.PartDrawA(395,333+oldy,268,90,10,60,12,za2,0,0,10,40);
					chacha.PartDrawA(405,333+oldy,278,90,210,60,12,za2,0,0,12,40);
					chacha.PartDrawA(615,333+oldy,290,90,10,60,12,za2,0,0,10,40);
				}
				s="replay load r key";
				chacha.Moji16(s,s.GetLength(),0,30,720,FALSE,za3-60);
				s="stage  load e key";
				chacha.Moji16(s,s.GetLength(),0,30,720-20,FALSE,za3-60);
			}
			if(menuno==1||menuno==2||menuno==3){//オプションメニュー表示
				if(save.gengo==0){
					chacha.PartDrawA(185,350,0,250,256,160,12,za4,0,0,256,160);
					chacha.PartDrawA(185,350+160,0,250+160,256,32,12,za4,0,0,256,32);
					chacha.PartDrawA(385,500+50,0,450,256,64,12,za4,0,0,256,64);
				}else{
					s="   bgm";
					chacha.Moji32(s,s.GetLength(),0,200,350,FALSE,za4);
					s="effect";
					chacha.Moji32(s,s.GetLength(),0,200,350+50,FALSE,za4);
					s="defficulty";
					chacha.Moji32(s,s.GetLength(),0,200-32*4,350+50*2,FALSE,za4);
					s="  language";
					chacha.Moji32(s,s.GetLength(),0,200-32*4,350+50*3,FALSE,za4);
					s="exit";
					chacha.Moji32(s,s.GetLength(),0,455,350+50*4,FALSE,za4);
				}
				chacha.PartDrawA(400,350,256,250,96,32,12,za4,0,0,96,32);
				chacha.PartDrawA(480,352,256,300,256,32,12,za4,0,0,256,32);
				chacha.PartDrawA(400,400,256,250,96,32,12,za4,0,0,96,32);
				chacha.PartDrawA(480,402,256,300,256,32,12,za4,0,0,256,32);
				chacha.PartDrawA(400,450   ,256,350   ,128,32,12,za4,0,0,128,32);//easy
				chacha.PartDrawA(400,450+32,256,350+32,128,16,12,za4,0,0,128,16);
				chacha.PartDrawA(490,450,256,400,192,32,12,za4,0,0,192,32);//normal
				chacha.PartDrawA(620,450,256,450,128,32,12,za4,0,0,128,32);//hard
				if(save.maxnanido==3||save.maxnanido==4)
				chacha.PartDrawA(720,450,  0,384,256,32,13,za4,0,0,256,32);//nightmare
				if(save.maxnanido==4)
				chacha.PartDrawA(900,450,  0,446,256,32,13,za4,0,0,256,32);//inferno

				chacha.PartDrawA(410,500,256,256-62,256,64,12,za4,0,0,256,64);

				//オプション項目を設定
				//BGM
				if(save.bgmplayf){
					chacha.PartDrawA(483+32.5f*((float)save.bgmvol-1),355,0,0,25,30,8,30+ten2*3-(255-za4),0,0);
					chacha.PartDrawA(478+32.5f*((float)save.bgmvol-1),337,268,90,10,50,12,za4-55,0,0,10,40);
					chacha.PartDrawA(488+32.5f*((float)save.bgmvol-1),337,278,90,12,50,12,za4-55,0,0,12,40);
					chacha.PartDrawA(500+32.5f*((float)save.bgmvol-1),337,290,90,10,50,12,za4-55,0,0,10,40);
				}else{
					chacha.PartDrawA(407,355,0,0,75,30,8,30+ten2*3-(255-za4),0,0);
					chacha.PartDrawA(402,337,268,90,10,50,12,za4-55,0,0,10,40);
					chacha.PartDrawA(412,337,278,90,60,50,12,za4-55,0,0,12,40);
					chacha.PartDrawA(472,337,290,90,10,50,12,za4-55,0,0,10,40);
				}
				if(save.effectplayf){
					chacha.PartDrawA(483+32.5f*((float)save.effectvol-1),355+50,0,0,25,30,8,30+ten2*3-(255-za4),0,0);
					chacha.PartDrawA(478+32.5f*((float)save.effectvol-1),337+50,268,90,10,50,12,za4-55,0,0,10,40);
					chacha.PartDrawA(488+32.5f*((float)save.effectvol-1),337+50,278,90,12,50,12,za4-55,0,0,12,40);
					chacha.PartDrawA(500+32.5f*((float)save.effectvol-1),337+50,290,90,10,50,12,za4-55,0,0,10,40);
				}else{
					chacha.PartDrawA(407,355+50,0,0,75,30,8,30+ten2*3-(255-za4),0,0);
					chacha.PartDrawA(402,337+50,268,90,10,50,12,za4-55,0,0,10,40);
					chacha.PartDrawA(412,337+50,278,90,60,50,12,za4-55,0,0,12,40);
					chacha.PartDrawA(472,337+50,290,90,10,50,12,za4-55,0,0,10,40);
				}
				switch(save.nanido){
				case 0://EASY
					chacha.PartDrawA(407,350+100,0,0,85,30,8,30+ten2*3-(255-za4),0,0);
					chacha.PartDrawA(402,330+100,268,90,10,52,12,za4-55,0,0,10,40);
					chacha.PartDrawA(412,330+100,278,90,70,52,12,za4-55,0,0,12,40);
					chacha.PartDrawA(482,330+100,290,90,10,52,12,za4-55,0,0,10,40);
					break;
				case 1://NORMAL
					chacha.PartDrawA(500,350+100,0,0,120,30,8,30+ten2*3-(255-za4),0,0);
					chacha.PartDrawA(495,330+100,268,90,10,52,12,za4-55,0,0,10,40);
					chacha.PartDrawA(505,330+100,278,90,105,52,12,za4-55,0,0,12,40);
					chacha.PartDrawA(610,330+100,290,90,10,52,12,za4-55,0,0,10,40);
					break;
				case 2://HARD
					chacha.PartDrawA(630,350+100,0,0,85,30,8,30+ten2*3-(255-za4),0,0);
					chacha.PartDrawA(625,330+100,268,90,10,52,12,za4-55,0,0,10,40);
					chacha.PartDrawA(635,330+100,278,90,70,52,12,za4-55,0,0,12,40);
					chacha.PartDrawA(705,330+100,290,90,10,52,12,za4-55,0,0,10,40);
					break;
				case 3://NIGHTMARE
					chacha.PartDrawA(720,350+100,0,0,175,30,8,30+ten2*3-(255-za4),0,0);
					chacha.PartDrawA(715,330+100,268,90,10,52,12,za4-55,0,0,10,40);
					chacha.PartDrawA(725,330+100,278,90,160,52,12,za4-55,0,0,12,40);
					chacha.PartDrawA(885,330+100,290,90,10,52,12,za4-55,0,0,10,40);
					break;
				case 4://INFERNO
					chacha.PartDrawA(900,350+100,0,0,118,30,8,30+ten2*3-(255-za4),0,0);
					chacha.PartDrawA(895,330+100,268,90,10,52,12,za4-55,0,0,10,40);
					chacha.PartDrawA(905,330+100,278,90,103,52,12,za4-55,0,0,12,40);
					chacha.PartDrawA(1008,330+100,290,90,10,52,12,za4-55,0,0,10,40);
					break;
				}
				switch(save.gengo){
				case 0://日本語
					chacha.PartDrawA(407,350+150,0,0,113,34,8,30+ten2*3-(255-za4),0,0);
					chacha.PartDrawA(402,330+150,268,90,10,56,12,za4-55,0,0,10,40);
					chacha.PartDrawA(412,330+150,278,90,98,56,12,za4-55,0,0,12,40);
					chacha.PartDrawA(502+8,330+150,290,90,10,56,12,za4-55,0,0,10,40);
					break;
				case 1://英語L
					chacha.PartDrawA(530,350+150,0,0,129,38,8,30+ten2*3-(255-za4),0,0);
					chacha.PartDrawA(525,330+150,268,90,10,60,12,za4-55,0,0,10,40);
					chacha.PartDrawA(535,330+150,278,90,114,60,12,za4-55,0,0,12,40);
					chacha.PartDrawA(649,330+150,290,90,10,60,12,za4-55,0,0,10,40);
					break;
				}
				//選択枠表示
				chacha.PartDrawA(450,550,0,0,135,30,8,8+ten2*3-(200-za2_),0,0);
				chacha.PartDrawA(445,530,268,90,10,52,12,za2_,0,0,10,40);
				chacha.PartDrawA(455,530,278,90,120,52,12,za2_,0,0,12,40);
				chacha.PartDrawA(575,530,290,90,10,52,12,za2_,0,0,10,40);
			}
			if(menuno==4||menuno==5||menuno==6){//MISSION選択表示
				if(save.gengo==0){
					chacha.PartDrawA(384,350,0,48,256,48,12,za4,0,0,256,48);
					chacha.PartDrawA(385,390+32*3,0,450,256,64,12,za4,0,0,256,64);
				}else{
					s="mission select";
					chacha.Moji32(s,s.GetLength(),0,512-7*32,350,FALSE,za4);
					s=" exit";
					chacha.Moji32(s,s.GetLength(),0,416,390+32*3,FALSE,za4);
				}
				chacha.PartDrawA(512-16,390+4   ,270,7,32,32,12,za4,0,0,32,32);
				chacha.PartDrawA(512-16,390+32*2-2,270,7+44,32,32,12,za4,0,0,32,32);
				s.Format("[%3d]",staget);
				chacha.Moji32(s,s.GetLength(),1,512-32-16-32,390+32,FALSE,za4);
				//選択枠表示
				chacha.PartDrawA(400,390+oldy,0,0,224,30,8,8+ten2*3-(200-za2_),0,0);
				chacha.PartDrawA(395,370+oldy,268,90,10,52,12,za2_,0,0,10,40);
				chacha.PartDrawA(405,370+oldy,278,90,210,52,12,za2_,0,0,12,40);
				chacha.PartDrawA(615,370+oldy,290,90,10,52,12,za2_,0,0,10,40);
			}
			if(menuno==7||menuno==8||menuno==9||menuno==10||menuno==13){//ランキング表示
				if(save.gengo==0){
					chacha.PartDrawA(384,20,0,48*3,256,48,12,za4,0,0,256,48);
					chacha.PartDrawA(385,768-48,0,450,256,64,12,za4,0,0,256,64);
				}else{
					s="ranking";
					chacha.Moji32(s,s.GetLength(),0,512-3*32-16,20,FALSE,za4);
					s=" exit";
					chacha.Moji32(s,s.GetLength(),0,416,768-48,FALSE,za4);
				}
				for(int l=0;l<20;l++){
					s.Format("A%2d A%10dA %c%c%c%c%c%c %s",l+1,save.score[l][0],
						save.scorec[l][0],
						save.scorec[l][1],
						save.scorec[l][2],
						save.scorec[l][3],
						save.scorec[l][4],
						save.scorec[l][5],tb[save.score[l][1]]);
					chacha.Moji32(s,s.GetLength(),1,512-14*32-16,65+(float)l*32,FALSE,za4);
				}
				//選択枠表示
				chacha.PartDrawA(400,720+oldy,0,0,224,30,8,8+ten2*3-(200-za2_),0,0);
				chacha.PartDrawA(395,700+oldy,268,90,10,52,12,za2_,0,0,10,40);
				chacha.PartDrawA(405,700+oldy,278,90,210,52,12,za2_,0,0,12,40);
				chacha.PartDrawA(615,700+oldy,290,90,10,52,12,za2_,0,0,10,40);
			}
			if(menuno==10||menuno==11||menuno==12||menuno==13){//スコア表示
				if(save.gengo==0){
					chacha.PartDrawA(384,20,0,48*3,256,48,12,za5,0,0,256,48);
					chacha.PartDrawA(385,768-48,0,450,256,64,12,za5,0,0,256,64);
				}else{
					s="ranking";
					chacha.Moji32(s,s.GetLength(),0,512-3*32-16,20,FALSE,za5);
					s=" exit";
					chacha.Moji32(s,s.GetLength(),0,416,768-48,FALSE,za5);
				}
				chacha.PartDrawA(  0, 70,  0,340,256,32,13,za5,0,0,256,32);//mission
				chacha.PartDrawA(135, 70,256,350,128,32,12,za5,0,0,128,32);//easy
				chacha.PartDrawA(135,102,256,382,128,16,12,za5,0,0,128,16);
				chacha.PartDrawA(296, 70,256,400,192,32,12,za5,0,0,192,32);//normal
				chacha.PartDrawA(457, 70,256,450,128,32,12,za5,0,0,128,32);//hard
				if(save.maxnanido==3||save.maxnanido==4)
				chacha.PartDrawA(600, 70,  0,384,256,32,13,za5,0,0,256,32);//nightmare
				if(save.maxnanido==4)
				chacha.PartDrawA(784, 70,  0,446,256,32,13,za5,0,0,256,32);//inferno
				CString s1,s2,s3,s4,s5;

				for(int l=0;l<STAGEMAX;l++){
					s1.Format("%02d:%02d.%02d",save.stagetime[l][0]/(60*60),(save.stagetime[l][0]/60)%60,((save.stagetime[l][0]%60)*100)/60);
					s2.Format("%02d:%02d.%02d",save.stagetime[l][1]/(60*60),(save.stagetime[l][1]/60)%60,((save.stagetime[l][1]%60)*100)/60);
					s3.Format("%02d:%02d.%02d",save.stagetime[l][2]/(60*60),(save.stagetime[l][2]/60)%60,((save.stagetime[l][2]%60)*100)/60);
					s4.Format("%02d:%02d.%02d",save.stagetime[l][3]/(60*60),(save.stagetime[l][3]/60)%60,((save.stagetime[l][3]%60)*100)/60);
					s5.Format("%02d:%02d.%02d",save.stagetime[l][4]/(60*60),(save.stagetime[l][4]/60)%60,((save.stagetime[l][4]%60)*100)/60);
					if(save.stagetime[l][0]==-1) s1="--:--.--";
					if(save.stagetime[l][1]==-1) s2="--:--.--";
					if(save.stagetime[l][2]==-1) s3="--:--.--";
					if(save.stagetime[l][3]==-1) s4="--:--.--";
					if(save.stagetime[l][4]==-1) s5="--:--.--";
					if(save.maxnanido==3)
						s.Format("   %3d   %s  %s  %s  %s",l+1,s1,s2,s3,s4);
					else if(save.maxnanido==4)
						s.Format("   %3d   %s  %s  %s  %s  %s",l+1,s1,s2,s3,s4,s5);
					else
						s.Format("   %3d   %s  %s  %s",l+1,s1,s2,s3);
					chacha.Moji16(s,s.GetLength(),1,0,110+(float)l*23,FALSE,za5);
				}
				//選択枠表示
				chacha.PartDrawA(400,720+oldy,0,0,224,30,8,8+ten2*3-(200-za2_),0,0);
				chacha.PartDrawA(395,700+oldy,268,90,10,52,12,za2_,0,0,10,40);
				chacha.PartDrawA(405,700+oldy,278,90,210,52,12,za2_,0,0,12,40);
				chacha.PartDrawA(615,700+oldy,290,90,10,52,12,za2_,0,0,10,40);
			}
			//カーソル表示
			if(menuno!=-1)
				chacha.JikiDraw((float)mousex,(float)mousey,4*32,0,0,FALSE);
			if(menuno==-2||menuno==-3||menuno==-4){
				chacha.Fade(za*4);
			}

			//描画終わり
			chacha.End();
			if(!chacha.DoEventChk(endf)){endf=TRUE; break; }
			chacha.End(FALSE);//描画終了
			f1=chacha.Timing64(f2,FALSE);
			timen=GetTiming(f1);
//			timen=(DWORD)((float)f1/16.7f)+1;
			chacha.timing1((WORD)timen,FALSE,FALSE);
			timen2=timen;
			chacha.Timing64(f2,FALSE);
			f3=chacha.Timing64(chacha.fpstiming,FALSE);
		}else timen--;
	}
	return TRUE;


}

//ロゴ
void CMainFrame::LogoPlay()
{
	DWORD f1=0,f2=0,f3=0;
	DWORD timen2=timen=1;
	int animecnt=0;
	float fx=64,fy=64;

	int animecnt2,j,jjj=0;
	float animecnt3;
	chacha.LoadWorkTex(0);
	if(chacha.DoEventChk(endf)==FALSE){endf=TRUE; return; }
	chacha.Timing64(f2);
	chacha.Timing64(chacha.fpstiming,FALSE);
	for(;;){
		chacha.GetInput();
		input();
		animecnt++;
		//★処理
		for(j=0;j<256*(2*(3-save.star));j++){
			if(haikei[j][0]==-1&&(rand()%20==9)){
				if(rand()%200 > 100){
					haikei[j][0]=0;	haikei[j][1]=(rand()%768);
					haikei[j][2]=haikei[j][3]=(rand()%10)+1;
				}else{
					haikei[j][1]=0;	haikei[j][0]=(rand()%1024);
					haikei[j][2]=haikei[j][3]=(rand()%10)+1;
				}
				haikei[j][4]=rand()%8;
			}else{
				if(haikei[j][0]!=-1){
					haikei[j][0]+=haikei[j][2];haikei[j][1]+=haikei[j][3];
					if(haikei[j][0]>1024 || haikei[j][1]>768) haikei[j][0]=-1;
				}
			}
		}
		//マウスの選択範囲を決める
		mousex_=(float)mousex*gamenx/gamenx_;
		mousey_=(float)mousey*gameny/gameny_;
		if((endf||tamaf)&&(jjj==0)){jjj=1;animecnt=128+100+30+60+60+90+28*3+110+128+200+128+25;}
		if(animecnt==128+100+30+60+60+90+28*3+110+128)chacha_a.Play(6);
		if(timen==1){//DirectX描画開始
			chacha.Begin();
			for(j=0;j<256*(2*(3-save.star));j++){
				if(haikei[j][0]!=-1)
					chacha.PartDrawA((float)haikei[j][0],(float)haikei[j][1],haikei[j][4]*32,2*32,2,2,4,160);
			}
			if(animecnt<128){
				animecnt2=animecnt;
				chacha.Fade(255-animecnt*2);
			}
			if(animecnt>=128 && animecnt<128+100){
				animecnt2=animecnt-128;
				chacha.PartDrawA((float)animecnt*8-1024-8*20,(float)animecnt*8-1024-8*20,0,0,160,160,13,255,(float)((animecnt*4)%360),1,160,160);
			}
			if(animecnt>=128+100 && animecnt<128+100+30){
				animecnt2=animecnt-(128+100);
				chacha.PartDrawA(640,640,0,0,160,160,13,255,192,1,160,160);
				chacha.PartDrawA(710-(float)animecnt2*3,760-(float)animecnt2*3,0,160,32,32,13,255,192,1,32,32);
				chacha.PartDrawA(710+(float)animecnt2*3,760-(float)animecnt2*3,0,160,32,32,13,255,0,0,32,32);
			}
			if(animecnt>=128+100+30 && animecnt<128+100+30+60){
				animecnt2=animecnt-(128+100+30);
				chacha.PartDrawA(640,640,0,0,160,160,13,255,192,1,160,160);
			}
			if(animecnt>=128+100+30+60 && animecnt<128+100+30+60+60){
				animecnt2=animecnt-(128+100+30+60);
				chacha.PartDrawA(640,640-(float)animecnt2,0,0,160,160,13,255,192,1,160,160);
				chacha.PartDrawA(730,770-(float)animecnt2,256,0,fx,fy,13,255,0,0,fx,fy);
			}
			if(animecnt>=128+100+30+60+60 && animecnt<128+100+30+60+60+90){
				animecnt2=animecnt-(128+100+30+60+60);
				chacha.PartDrawA(640-(float)animecnt2,640-60-(float)animecnt2*10,0,0,160,160,13,255,(float)((192+animecnt2*3)%360),1,160,160);
				chacha.PartDrawA(730-(float)animecnt2*5,770-60-(float)animecnt2*5,256,0,fx,fy,13,255,0,0,fx,fy);
				chacha.PartDrawA(730-(float)animecnt2*4,770-60-(float)animecnt2*5,256,0,fx,fy,13,255,0,0,fx,fy);
				chacha.PartDrawA(730-(float)animecnt2*2,770-60-(float)animecnt2*5,256,0,fx,fy,13,255,0,0,fx,fy);
			}
			if(animecnt>=128+100+30+60+60+90 && animecnt<128+100+30+60+60+90+28*3){
				animecnt2=animecnt-(128+100+30+60+60+90);
				animecnt3=(float)animecnt2/3;
				//r e a y o w f t
				chacha.PartDrawA(730-90*5+animecnt3  ,770-60-90*5,310,60*0,fx,fy,13,255,0,0,fx,fy);//r
				chacha.PartDrawA(730-90*5+animecnt3*2,770-60-90*5,310,60*1,fx,fy,13,255,0,0,fx,fy);//e
				chacha.PartDrawA(730-90*4+animecnt3  ,770-60-90*5,310,60*0,fx,fy,13,255,0,0,fx,fy);//r
				chacha.PartDrawA(730-90*4+animecnt3*2,770-60-90*5,310,60*2,fx,fy,13,255,0,0,fx,fy);//a
				chacha.PartDrawA(730-90*4+animecnt3*3,770-60-90*5,310,60*3,fx,fy,13,255,0,0,fx,fy);//y
				chacha.PartDrawA(730-90*4+animecnt3*4,770-60-90*5,310,60*1,fx,fy,13,255,0,0,fx,fy);//e
				chacha.PartDrawA(730-90*4+animecnt3*5,770-60-90*5,310,60*0,fx,fy,13,255,0,0,fx,fy);//r
				chacha.PartDrawA(730-90*2+animecnt3  ,770-60-90*5,310,60*4,fx,fy,13,255,0,0,fx,fy);//o
				chacha.PartDrawA(730-90*2+animecnt3*2,770-60-90*5,310,60*5,fx,fy,13,255,0,0,fx,fy);//w
				chacha.PartDrawA(730-90*2+animecnt3*3,770-60-90*5,310,60*1,fx,fy,13,255,0,0,fx,fy);//e
				chacha.PartDrawA(730-90*2+animecnt3*4,770-60-90*5,310,60*0,fx,fy,13,255,0,0,fx,fy);//r
				chacha.PartDrawA(730-90*5,770-60-90*5,256,0,fx,fy,13,255,0,0,fx,fy);
				chacha.PartDrawA(730-90*4,770-60-90*5,256,0,fx,fy,13,255,0,0,fx,fy);
				chacha.PartDrawA(730-90*2,770-60-90*5,256,0,fx,fy,13,255,0,0,fx,fy);
			}
			if(animecnt>=128+100+30+60+60+90+28*3 && animecnt<128+100+30+60+60+90+28*3+110){
				animecnt2=animecnt-(128+100+30+60+60+90+28*3);
				animecnt3=28;
				chacha.PartDrawA(730-90*5,770-60-90*5,256,0,fx,fy,13,255,0,0,fx,fy);
				chacha.PartDrawA(730-90*4,770-60-90*5,256,0,fx,fy,13,255,0,0,fx,fy);
				chacha.PartDrawA(730-90*2,770-60-90*5,256,0,fx,fy,13,255,0,0,fx,fy);
				//r e a y o w f t
				chacha.PartDrawA(730-90*5+animecnt3  ,770-60-90*5,310,60*0,fx,fy,13,255,0,0,fx,fy);//r
				chacha.PartDrawA(730-90*5+animecnt3*2,770-60-90*5,310,60*1,fx,fy,13,255,0,0,fx,fy);//e
				chacha.PartDrawA(730-90*4+animecnt3  ,770-60-90*5,310,60*0,fx,fy,13,255,0,0,fx,fy);//r
				chacha.PartDrawA(730-90*4+animecnt3*2,770-60-90*5,310,60*2,fx,fy,13,255,0,0,fx,fy);//a
				chacha.PartDrawA(730-90*4+animecnt3*3,770-60-90*5,310,60*3,fx,fy,13,255,0,0,fx,fy);//y
				chacha.PartDrawA(730-90*4+animecnt3*4,770-60-90*5,310,60*1,fx,fy,13,255,0,0,fx,fy);//e
				chacha.PartDrawA(730-90*4+animecnt3*5,770-60-90*5,310,60*0,fx,fy,13,255,0,0,fx,fy);//r
				chacha.PartDrawA(730-90*2+animecnt3  ,770-60-90*5,310,60*4,fx,fy,13,255,0,0,fx,fy);//o
				chacha.PartDrawA(730-90*2+animecnt3*2,770-60-90*5,310,60*5,fx,fy,13,255,0,0,fx,fy);//w
				chacha.PartDrawA(730-90*2+animecnt3*3,770-60-90*5,310,60*1,fx,fy,13,255,0,0,fx,fy);//e
				chacha.PartDrawA(730-90*2+animecnt3*4,770-60-90*5,310,60*0,fx,fy,13,255,0,0,fx,fy);//r
				if(animecnt2>26){
					animecnt3=(float)(animecnt2-26)/3;
					chacha.PartDrawA(430+animecnt3  ,320,310,60*4,fx,fy,13,255,0,0,fx,fy);//o
					chacha.PartDrawA(430+animecnt3*2,320,310,60*6,fx,fy,13,255,0,0,fx,fy);//f
					chacha.PartDrawA(430+animecnt3*3,320,310,60*7,fx,fy,13,255,0,0,fx,fy);//t
					chacha.PartDrawA(430,320,256,60,fx,fy,13,255,0,0,fx,fy);

				}
				chacha.PartDrawA(250+(float)animecnt2*5,(float)animecnt2*10,0,0,160,160,13,255,(float)((192+animecnt2*3)%360),1,160,160);
			}
			if(animecnt>=128+100+30+60+60+90+28*3+110 && animecnt<128+100+30+60+60+90+28*3+110+128){
				animecnt2=animecnt-(128+100+30+60+60+90+28*3+110);
				animecnt3=28;
				chacha.PartDrawA(730-90*5,770-60-90*5,256,0,fx,fy,13,255,0,0,fx,fy);
				chacha.PartDrawA(730-90*4,770-60-90*5,256,0,fx,fy,13,255,0,0,fx,fy);
				chacha.PartDrawA(730-90*2,770-60-90*5,256,0,fx,fy,13,255,0,0,fx,fy);
				//r e a y o w f t
				chacha.PartDrawA(730-90*5+animecnt3  ,770-60-90*5,310,60*0,fx,fy,13,255,0,0,fx,fy);//r
				chacha.PartDrawA(730-90*5+animecnt3*2,770-60-90*5,310,60*1,fx,fy,13,255,0,0,fx,fy);//e
				chacha.PartDrawA(730-90*4+animecnt3  ,770-60-90*5,310,60*0,fx,fy,13,255,0,0,fx,fy);//r
				chacha.PartDrawA(730-90*4+animecnt3*2,770-60-90*5,310,60*2,fx,fy,13,255,0,0,fx,fy);//a
				chacha.PartDrawA(730-90*4+animecnt3*3,770-60-90*5,310,60*3,fx,fy,13,255,0,0,fx,fy);//y
				chacha.PartDrawA(730-90*4+animecnt3*4,770-60-90*5,310,60*1,fx,fy,13,255,0,0,fx,fy);//e
				chacha.PartDrawA(730-90*4+animecnt3*5,770-60-90*5,310,60*0,fx,fy,13,255,0,0,fx,fy);//r
				chacha.PartDrawA(730-90*2+animecnt3  ,770-60-90*5,310,60*4,fx,fy,13,255,0,0,fx,fy);//o
				chacha.PartDrawA(730-90*2+animecnt3*2,770-60-90*5,310,60*5,fx,fy,13,255,0,0,fx,fy);//w
				chacha.PartDrawA(730-90*2+animecnt3*3,770-60-90*5,310,60*1,fx,fy,13,255,0,0,fx,fy);//e
				chacha.PartDrawA(730-90*2+animecnt3*4,770-60-90*5,310,60*0,fx,fy,13,255,0,0,fx,fy);//r
				chacha.PartDrawA(430+animecnt3  ,320,310,60*4,fx,fy,13,255,0,0,fx,fy);//o
				chacha.PartDrawA(430+animecnt3*2,320,310,60*6,fx,fy,13,255,0,0,fx,fy);//f
				chacha.PartDrawA(430+animecnt3*3,320,310,60*7,fx,fy,13,255,0,0,fx,fy);//t
				chacha.PartDrawA(430,320,256,60,fx,fy,13,255,0,0,fx,fy);
				chacha.PartDrawA(380,380,0,256,256,64,13,animecnt2*2,0,0,256,64);
			}
			if(animecnt>=128+100+30+60+60+90+28*3+110+128 && animecnt<128+100+30+60+60+90+28*3+110+128+200){
				animecnt2=animecnt-(128+100+30+60+60+90+28*3+110+128);
				animecnt3=28;
				chacha.PartDrawA(730-90*5,770-60-90*5,256,0,fx,fy,13,255,0,0,fx,fy);
				chacha.PartDrawA(730-90*4,770-60-90*5,256,0,fx,fy,13,255,0,0,fx,fy);
				chacha.PartDrawA(730-90*2,770-60-90*5,256,0,fx,fy,13,255,0,0,fx,fy);
				//r e a y o w f t
				chacha.PartDrawA(730-90*5+animecnt3  ,770-60-90*5,310,60*0,fx,fy,13,255,0,0,fx,fy);//r
				chacha.PartDrawA(730-90*5+animecnt3*2,770-60-90*5,310,60*1,fx,fy,13,255,0,0,fx,fy);//e
				chacha.PartDrawA(730-90*4+animecnt3  ,770-60-90*5,310,60*0,fx,fy,13,255,0,0,fx,fy);//r
				chacha.PartDrawA(730-90*4+animecnt3*2,770-60-90*5,310,60*2,fx,fy,13,255,0,0,fx,fy);//a
				chacha.PartDrawA(730-90*4+animecnt3*3,770-60-90*5,310,60*3,fx,fy,13,255,0,0,fx,fy);//y
				chacha.PartDrawA(730-90*4+animecnt3*4,770-60-90*5,310,60*1,fx,fy,13,255,0,0,fx,fy);//e
				chacha.PartDrawA(730-90*4+animecnt3*5,770-60-90*5,310,60*0,fx,fy,13,255,0,0,fx,fy);//r
				chacha.PartDrawA(730-90*2+animecnt3  ,770-60-90*5,310,60*4,fx,fy,13,255,0,0,fx,fy);//o
				chacha.PartDrawA(730-90*2+animecnt3*2,770-60-90*5,310,60*5,fx,fy,13,255,0,0,fx,fy);//w
				chacha.PartDrawA(730-90*2+animecnt3*3,770-60-90*5,310,60*1,fx,fy,13,255,0,0,fx,fy);//e
				chacha.PartDrawA(730-90*2+animecnt3*4,770-60-90*5,310,60*0,fx,fy,13,255,0,0,fx,fy);//r
				chacha.PartDrawA(430+animecnt3  ,320,310,60*4,fx,fy,13,255,0,0,fx,fy);//o
				chacha.PartDrawA(430+animecnt3*2,320,310,60*6,fx,fy,13,255,0,0,fx,fy);//f
				chacha.PartDrawA(430+animecnt3*3,320,310,60*7,fx,fy,13,255,0,0,fx,fy);//t
				chacha.PartDrawA(430,320,256,60,fx,fy,13,255,0,0,fx,fy);
				chacha.PartDrawA(380,380,0,256,256,64,13,255,0,0,256,64);
			}
			if(animecnt>=128+100+30+60+60+90+28*3+110+128+200 && animecnt<128+100+30+60+60+90+28*3+110+128+200+128){
				animecnt2=animecnt-(128+100+30+60+60+90+28*3+110+128+200);
				animecnt3=28;
				chacha.PartDrawA(730-90*5,770-60-90*5,256,0,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);
				chacha.PartDrawA(730-90*4,770-60-90*5,256,0,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);
				chacha.PartDrawA(730-90*2,770-60-90*5,256,0,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);
				//r e a y o w f t
				chacha.PartDrawA(730-90*5+animecnt3  ,770-60-90*5,310,60*0,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);//r
				chacha.PartDrawA(730-90*5+animecnt3*2,770-60-90*5,310,60*1,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);//e
				chacha.PartDrawA(730-90*4+animecnt3  ,770-60-90*5,310,60*0,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);//r
				chacha.PartDrawA(730-90*4+animecnt3*2,770-60-90*5,310,60*2,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);//a
				chacha.PartDrawA(730-90*4+animecnt3*3,770-60-90*5,310,60*3,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);//y
				chacha.PartDrawA(730-90*4+animecnt3*4,770-60-90*5,310,60*1,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);//e
				chacha.PartDrawA(730-90*4+animecnt3*5,770-60-90*5,310,60*0,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);//r
				chacha.PartDrawA(730-90*2+animecnt3  ,770-60-90*5,310,60*4,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);//o
				chacha.PartDrawA(730-90*2+animecnt3*2,770-60-90*5,310,60*5,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);//w
				chacha.PartDrawA(730-90*2+animecnt3*3,770-60-90*5,310,60*1,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);//e
				chacha.PartDrawA(730-90*2+animecnt3*4,770-60-90*5,310,60*0,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);//r
				chacha.PartDrawA(430+animecnt3  ,320,310,60*4,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);//o
				chacha.PartDrawA(430+animecnt3*2,320,310,60*6,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);//f
				chacha.PartDrawA(430+animecnt3*3,320,310,60*7,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);//t
				chacha.PartDrawA(430,320,256,60,fx,fy,13,(255-animecnt2*2),0,0,fx,fy);
				chacha.PartDrawA(380,380,0,256,256,64,13,(255-animecnt2*2),0,0,256,64);
			}
			if(animecnt>=128+100+30+60+60+90+28*3+110+128+200+128 && animecnt<128+100+30+60+60+90+28*3+110+128+200+128+30){
			}
			if(animecnt>=128+100+30+60+60+90+28*3+110+128+200+128+30){
				return;
			}
			chacha.End();
			if(!chacha.DoEventChk(endf)){endf=TRUE; break; }
			chacha.End(FALSE);//描画終了
			f1=chacha.Timing64(f2,FALSE);
			timen=GetTiming(f1);
			chacha.timing1((WORD)timen,FALSE,FALSE);
			timen2=timen;
			chacha.Timing64(f2,FALSE);
			f3=chacha.Timing64(chacha.fpstiming,FALSE);
		}else timen--;
	}
	return;
}

//ランキング
void CMainFrame::RankigPlay()
{
	DWORD f1=0,f2=0,f3=0,za4=0;
	chacha.LoadWorkTex(0);
	DWORD timen2=timen=1;
	CString ra,ra2;
	int ranking,al=255,cnt=0;
	int moji=-1;//文字列に表示されている文字
	int mojia=0;//入力を行った文字
	float ax[30]={0},ay[30]={0},axx[30]={0},ayy[30]={0},mx[30]={0},my[30]={0},as[30]={0},ad[30]={0};//ミサイル
	int am[30],ai[30];
	int mis=FALSE,wx,wy,kettei=FALSE,del,deltyu=0;
	int kaiten[6];
	float kaitenf[6][6];
	int kaitensu=0;
	float a,b,tss;
	char scorec[6]={' ',' ',' ',' ',' ',' '};BOOL fl;int sentaku=-1; char sentakuc[6]={' ',' ',' ',' ',' ',' '}; int missu=0;
	char tbl[49]={' ','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o',
		          'p','q','r','s','t','u','v','w','x','y','z','[',']',':','!','?',
				  '0','1','2','3','4','5','6','7','8','9','/','*','+','-','=','%','E'};
	int tamafb=0;
//	score=1212345;
	motsu=0;
	//ここでランキングに入ったかどうかをチェック
	for(int i=0;i<20;i++){
		if(save.score[i][0]<score){//ランキングに入った
			break;
		}
	}if(i==20)//ランキング外
		return;
	ranking=i;
	//★初期化
	HosiOkuMake();
	if(chacha.DoEventChk(endf)==FALSE){endf=TRUE; return; }
	chacha.Timing64(f2);
	chacha.Timing64(chacha.fpstiming,FALSE);
	for(;;){
		chacha.GetInput();
		input();
		HosiOkuProc();
		cnt++;
		//マウスの選択範囲を決める
		mousex_=(float)mousex*gamenx/gamenx_;
		mousey_=(float)mousey*gameny/gameny_;
		if(endf) return;
		if((kettei==2)&&(endf||tamaf)){ kettei=3;cnt=0;	fadef=TRUE;}
		if(kettei==4) return;
		//マウス処理をここへ
		fl=FALSE;del=-1;sentaku=-1;
		int mox,moy;mox=ftol(mousex_);moy=ftol(mousey_);
		for(i=0;i<16;i++){
			if(mox>16+i*64 && mox<48+i*64 && moy>32*7 && moy <32*8 ){
				fl=TRUE;if(tamaf&&tamafb==0){ sentaku=i; wx=i*64+32;wy=32*7+16;}}
			if(mox>16+i*64 && mox<48+i*64 && moy>32*9 && moy <32*10){
				fl=TRUE;if(tamaf&&tamafb==0){ sentaku=i+16; wx=i*64+32;wy=32*9+16;}}
			if(mox>16+i*64 && mox<48+i*64 && moy>32*11&& moy <32*12){
				fl=TRUE;if(tamaf&&tamafb==0){ sentaku=i+32; wx=i*64+32;wy=32*11+16;}}
		}
		if(mox>512-16 && mox<512+16 && moy>32*13&& moy <32*14){
			fl=TRUE;if(tamaf&&tamafb==0){ sentaku=48; wx=512;wy=32*13+16;}}
		for(i=0;i<6;i++){
			if(mojia>i){
				if(mox>512-3*32+i*32 && mox<=512-3*32+i*32+32 && moy>32*5 && moy<32*6){
					fl=TRUE;if(tamaf&&tamafb==0){ del=i; wx=512-3*32+i*32+16;wy=32*5+16;}
				}
			}
		}
		tamafb=tamaf;
		if(kettei==0){
			//文字をクリックした？
			if((sentaku!=-1)&&(deltyu==0)){//した。　クリックしたらミサイルが飛ぶ
				moji++;if(moji==6&&sentaku!=48){moji=5;}
				else{
					sentakuc[moji]=tbl[sentaku];
					mis=TRUE;
					ax[missu]=512;ay[missu]=32*20;
					tss=chacha.GetShita((float)wx,512,(float)wy,32*20);as[missu]=tss;tss*=PI180;a=chacha.Getsin(tss);b=chacha.Getcos(tss);
					axx[missu]=a;ayy[missu]=b;mx[missu]=(float)wx;my[missu]=(float)wy;ad[missu]=0;am[missu]=sentaku;ai[missu]=moji;
					missu++;chacha_a.Play(11);
				}
			}
			if((del!=-1)&&(deltyu==0)){
				if(moji!=-1){
					moji=del-1;deltyu=1;
					mis=TRUE;
					ax[missu]=512;ay[missu]=32*20;
					tss=chacha.GetShita((float)wx,512,(float)wy,32*20);as[missu]=tss;tss*=PI180;a=chacha.Getsin(tss);b=chacha.Getcos(tss);
					axx[missu]=a;ayy[missu]=b;mx[missu]=(float)wx;my[missu]=(float)wy;ad[missu]=1;
					missu++;chacha_a.Play(11);
				}
			}
			for(i=0;i<missu;i++){//ミサイル飛び処理
				axx[i]*=1.020f;ayy[i]*=1.020f;
				if(axx[i]>10.0f||ayy[i]>10.0f){axx[i]/=1.020f;ayy[i]/=1.020f;}
				if(axx[i]<-10.0f||ayy[i]<-10.0f){axx[i]/=1.020f;ayy[i]/=1.020f;}
				ax[i]+=axx[i]; ay[i]+=ayy[i];
				mot[motsu][0]=ftol(ax[i]-axx[i]*2);		mot[motsu][1]=ftol(ay[i]-ayy[i]*2);
				mot[motsu][2]=1;					mot[motsu][3]=-1;
				mot[motsu][4]=-ftol(axx[i]);	mot[motsu][5]=-ftol(ayy[i]);mot[motsu][6]=0;
				motsu++;
				if(mx[i]-16<ax[i] && mx[i]+16>ax[i] && my[i]-16<ay[i] && my[i]+16>ay[i]){//目的の文字に到着
					for(int p=0;p<8;p++){
						mot[motsu][0]=ftol(mx[i])+rand()%64-32;			mot[motsu][1]=ftol(my[i])+rand()%64-32;
						mot[motsu][2]=3;			mot[motsu][3]=-1; mot[motsu][6]=p*3;
						mot[motsu][4]=0;			mot[motsu][5]=0;			motsu++;
					}
					if(ad[i]==0){
						if(tbl[am[i]]=='E'){
							missu=0;
							kettei=TRUE;cnt=0;chacha_a.Play(1);
							for(i=18;i>=ranking;i--){
								save.score[i+1][0]=save.score[i][0];
								save.score[i+1][1]=save.score[i][1];
								save.scorec[i+1][0]=save.scorec[i][0];
								save.scorec[i+1][1]=save.scorec[i][1];
								save.scorec[i+1][2]=save.scorec[i][2];
								save.scorec[i+1][3]=save.scorec[i][3];
								save.scorec[i+1][4]=save.scorec[i][4];
								save.scorec[i+1][5]=save.scorec[i][5];
							}
							save.score[ranking][0]=score;
							save.score[ranking][1]=save.nanido;
							save.scorec[ranking][0]=scorec[0];
							save.scorec[ranking][1]=scorec[1];
							save.scorec[ranking][2]=scorec[2];
							save.scorec[ranking][3]=scorec[3];
							save.scorec[ranking][4]=scorec[4];
							save.scorec[ranking][5]=scorec[5];
						}else{
							kaiten[kaitensu]=tbl[am[i]];
							kaitenf[kaitensu][0]=mx[i];
							kaitenf[kaitensu][1]=my[i];
							tss=chacha.GetShita(512-3*32+(float)ai[i]*32,mx[i],32*5,my[i]);tss*=PI180;a=chacha.Getsin(tss);b=chacha.Getcos(tss);
							kaitenf[kaitensu][2]=a*5;
							kaitenf[kaitensu][3]=b*5;
							kaitenf[kaitensu][4]=512-3*32+(float)ai[i]*32;
							kaitenf[kaitensu][5]=32*5;
							kaitensu++;
	//						scorec[mojia]=sentakuc[mojia];
	//						mojia++;
						}
					}else{
						mojia=moji+1;deltyu=0;
					}
					for(int j=i;j<missu-1;j++){
						ax[j] =ax[j+1];	axx[j]=axx[j+1];	mx[j] =mx[j+1];ad[j] =ad[j+1];am[j] =am[j+1];
						ay[j] =ay[j+1];	ayy[j]=ayy[j+1];	my[j] =my[j+1];as[j] =as[j+1];ai[j] =ai[j+1];
					}
					i--;missu--;chacha_a.Play(13);
				}
			}
			for(i=0;i<kaitensu;i++){
				kaitenf[i][0]+=kaitenf[i][2];kaitenf[i][1]+=kaitenf[i][3];
				if(kaitenf[i][4]-8<kaitenf[i][0] && kaitenf[i][4]+8>kaitenf[i][0] && kaitenf[i][5]-8<kaitenf[i][1] && kaitenf[i][5]+8>kaitenf[i][1]){//目的の文字に到着
					scorec[mojia]=sentakuc[mojia];
					mojia++;
					for(int j=i;j<kaitensu-1;j++){
						kaiten[j]=kaiten[j+1];
						kaitenf[j][0]=kaitenf[j+1][0];
						kaitenf[j][1]=kaitenf[j+1][1];
						kaitenf[j][2]=kaitenf[j+1][2];
						kaitenf[j][3]=kaitenf[j+1][3];
						kaitenf[j][4]=kaitenf[j+1][4];
						kaitenf[j][5]=kaitenf[j+1][5];
					}i--;kaitensu--;
				}
			}
		}
		if((kettei==1)&&(cnt>127)) kettei=2;
		if((cnt==127)&&(kettei==3))	kettei=4;
		motanime();
		if(timen==1){//DirectX描画開始
			chacha.Begin();
			//★
			if(kettei==0)HosiOkuDraw();
			if((kettei==0)||(kettei==1)){
				//ランキング表示
				//               1234567890
				//         1234567890123456789
				ra.Format("ranking %d",ranking+1);
				chacha.Moji32(ra,ra.GetLength(),0,512-5*32,32   ,TRUE,al);
				ra.Format("score %10d",score);
				chacha.Moji32(ra,ra.GetLength(),0,512-8*32,32*3 ,TRUE,al);
				for(i=0;i<6;i++){
					if(mojia>i){
						ra.Format("%c",scorec[i]);
						chacha.Moji32(ra,ra.GetLength(),0,512-3*32+(float)i*32,32*5 ,TRUE,al);
					}else{
						ra="_";
						if(mojia==i){
							chacha.Moji32(ra,ra.GetLength(),0,512-3*32+(float)i*32,32*5 ,TRUE,abs(64-(cnt%128))*4);
						}else{
							chacha.Moji32(ra,ra.GetLength(),0,512-3*32+(float)i*32,32*5 ,TRUE,128);
						}
					}
				}
				//  1234567890123456789012345678901
				if((mojia!=6)&&(deltyu==0)){
					ra="  a b c d e f g h i j k l m n o";
					chacha.Moji32(ra,ra.GetLength(),0,512-15*32-16,32*7 ,TRUE,al);
					ra="p q r s t u v w x y z [ ] : ! ?";
					chacha.Moji32(ra,ra.GetLength(),0,512-15*32-16,32*9 ,TRUE,al);
					ra="0 1 2 3 4 5 6 7 8 9 / * + - = %";
					chacha.Moji32(ra,ra.GetLength(),0,512-15*32-16,32*11 ,TRUE,al);
				}else{
					ra="  a b c d e f g h i j k l m n o";
					chacha.Moji32(ra,ra.GetLength(),0,512-15*32-16,32*7 ,TRUE,96);
					ra="p q r s t u v w x y z [ ] : ! ?";
					chacha.Moji32(ra,ra.GetLength(),0,512-15*32-16,32*9 ,TRUE,96);
					ra="0 1 2 3 4 5 6 7 8 9 / * + - = %";
					chacha.Moji32(ra,ra.GetLength(),0,512-15*32-16,32*11 ,TRUE,96);
				}
				ra="E";
				if(deltyu==0)
					chacha.Moji32(ra,ra.GetLength(),0,512-16      ,32*13,TRUE,al);
				else
					chacha.Moji32(ra,ra.GetLength(),0,512-16      ,32*13,TRUE,96);
				motDraw();
				//ミサイル飛ぶ
				for(i=0;i<missu;i++){
					chacha.JikiDraw(ax[i],ay[i],(2+(cnt%20)/10)*32,32,(360-as[i])+180,TRUE);
				}
				for(i=0;i<kaitensu;i++){
					ra.Format("%c",kaiten[i]);
					chacha.Moji32(ra,ra.GetLength(),0,kaitenf[i][0],kaitenf[i][1],TRUE,al,(cnt*8)%360);

				}
				//照準表示
				chacha.JikiDraw(512,32*20,0,0,(360-chacha.GetShita((float)mousex_,512,(float)mousey_,32*20))+180,TRUE);
				//照準が文字とかさなっているか？
				if(fl)
					chacha.JikiDraw((float)mousex,(float)mousey,5*32,0,0,FALSE);
				else
					chacha.JikiDraw((float)mousex,(float)mousey,4*32,0,0,FALSE);
				if((kettei==0)&&(cnt<128)) chacha.Fade(255-cnt*2);
				if((kettei==1)&&(cnt<128)) chacha.Fade(cnt*2);
			}
			if((kettei==1)||(kettei==2)||(kettei==3)||(kettei==4)){//ランキング確認
				za4=255;
				if((cnt<128)&&(kettei==1)) za4=cnt*2;

				HosiOkuDraw();
				if(save.gengo==0){
					chacha.PartDrawA(384,20,0,48*3,256,48,12,za4,0,0,256,48);
				}else{
					ra="ranking";
					chacha.Moji32(ra,ra.GetLength(),0,512-3*32-16,20,FALSE,za4);
				}
				
				for(int l=0;l<20;l++){
					if(kettei==2){ za4=255;
						if(ranking==l) za4=abs(64-(cnt%128))*2+128;
					}
					ra.Format("A%2d A%10dA %c%c%c%c%c%c %s",l+1,save.score[l][0],
						save.scorec[l][0],
						save.scorec[l][1],
						save.scorec[l][2],
						save.scorec[l][3],
						save.scorec[l][4],
						save.scorec[l][5],tb[save.score[l][1]]);
					chacha.Moji32(ra,ra.GetLength(),1,512-14*32-16,65+(float)l*32,FALSE,za4);
				}
				if((cnt<200)&&(kettei==3||kettei==4)) chacha.Fade(cnt*2);
			}
			chacha.End();
			if(!chacha.DoEventChk(endf)){endf=TRUE; break; }
			chacha.End(FALSE);//描画終了
			f1=chacha.Timing64(f2,FALSE);
			timen=GetTiming(f1);
			chacha.timing1((WORD)timen,FALSE,FALSE);
			timen2=timen;
			chacha.Timing64(f2,FALSE);
			f3=chacha.Timing64(chacha.fpstiming,FALSE);
		}else timen--;
	}
	return;
}

void CMainFrame::motanime()
{
	for(int i=0;i<motsu;i++){//エフェクト処理
		if(mot[i][6]>0){ mot[i][6]--; continue;}
		if(mot[i][3]==-1) mot[i][3]=0;
		if(mot[i][2]!=5){
			mot[i][0]+=mot[i][4];		mot[i][1]+=mot[i][5];
		}
		if(mot[i][6]==-(2+(mot[i][2]==0||mot[i][2]==5)*4)-(mot[i][2]==5&&mot[i][4]==1)*8){	mot[i][3]++;if(mot[i][2]==5) mot[i][5]+=2; mot[i][6]=0;}
		mot[i][6]--;
		if(mot[i][3]==8){
			for(int j=i;j<motsu-1;j++){
				mot[j][0]=mot[j+1][0];
				mot[j][1]=mot[j+1][1];
				mot[j][2]=mot[j+1][2];
				mot[j][3]=mot[j+1][3];
				mot[j][4]=mot[j+1][4];
				mot[j][5]=mot[j+1][5];
				mot[j][6]=mot[j+1][6];
			}	motsu--;i--;
		}
	}
}

void CMainFrame::motDraw()
{
	//エフェクト表示
	for(int i=0;i<motsu;i++){
		if(mot[i][6]>0||mot[i][2]==0||(mot[i][2]==5&&mot[i][4]!=1)){  continue;}
		chacha.JikiDraw((float)mot[i][0],(float)mot[i][1],mot[i][3]*32,(mot[i][2]+2)*32,0,FALSE,150);
	}
}

//ゲーム中のPause処理
int CMainFrame::PausePlay()//戻り値 0:復帰 1:ゲームオーバーで 2:タイトルへ 3:ゲーム終了
{
	DWORD f1=0,f2=0,f3=0;
	chacha.LoadWorkTex(0);
	float num,oldnum=-1,num2; int menuno=0;
	int ten=0,ten2;
	int za=0,za2=0,za3=255,za4=0,za2_=0;
	float newy,oldy;
	DWORD timen2=timen=1;
	CString s;
	if(chacha.DoEventChk(endf)==FALSE){endf=TRUE; return 3; }
	chacha.Timing64(f2);
	chacha.Timing64(chacha.fpstiming,FALSE);
	for(;;){
		chacha.GetInput();
		input();
		if(endf) return 0;
		mousex_=(float)mousex*gamenx/gamenx_;
		mousey_=(float)mousey*gameny/gameny_;

		num=-1;
		ten++;ten=ten%128;ten2=(ten>64)?128-ten:ten;
		switch(menuno){
		case 0:
			if(mousex_>384 && mousex_<384+256){
				if(mousey_>264      && mousey_<=264+48  ) num=0;
				if(mousey_>264+48   && mousey_<=264+48*2) num=1;
				if(mousey_>264+48*2 && mousey_<=264+48*3) num=2;
				if(mousey_>264+48*3 && mousey_<=264+48*4) num=3;
			}
			if(movef||tekitamaf){ chacha_a.Play(2);return 0;}
			if(num==0 && tamaf){ chacha_a.Play(1);return 0;}
			if(num==1 && tamaf){ chacha_a.Play(1);return 1;}
			if(num==2 && tamaf){ chacha_a.Play(1);menuno=1;fadef=1;}
			if(num==3 && tamaf){ chacha_a.Play(1);menuno=2;fadef=1;}
			if(oldnum!=num){
			
				if(za==0){za=3;}
				num2=num;
				oldy=oldnum*48;	newy=num2*48;
				if(num==-1){za=1;za2=200;num2=oldnum;oldy=oldnum*48;newy=oldnum*48;}else
					if(oldnum==-1){za=2;za2=0;oldy=num*48;	newy=num*48;chacha_a.Play(3);} else 	chacha_a.Play(3);
			}
			oldnum=num;
			switch(za){
			case 1:za2-=5;if(za2<=0) za=0;break;
			case 2:za2+=5;if(za2>=200) za=0;break;
			}
			{
				for(int i=0;i<5;i++){
					if(oldy>newy) oldy--;
					if(oldy<newy) oldy++;
				}
			}break;
		case 1://メニュー閉じ(フェード)
			za4++; if(za4>128) return 2;
			break;
		case 2://メニュー閉じ(フェード)
			za4++; if(za4>128) return 3;
			break;
		}

		if(timen==1){//DirectX描画開始
			chacha.Begin();
			Draw();
			chacha.Fade(128);
			if(save.gengo==0){
				chacha.PartDrawA(512-96    ,384-2*60    ,320,0,128,128,12,255,0,0,128,128);
				chacha.PartDrawA(512-96+128,384-2*60    ,320+128,0,64,128,12,255,0,0,64,128);
				chacha.PartDrawA(512-96    ,384-2*60+128,320,128,128,64,12,255,0,0,128,64);
				chacha.PartDrawA(512-96+128,384-2*60+128,320+128,128,64,64,12,255,0,0,64,64);
				//選択枠表示
				chacha.PartDrawA(410-32,268+oldy,0,0,204+64,33,8,8+ten2*3-(200-za2),0,0);
				chacha.PartDrawA(405-32,246+oldy,268,90,10,60,12,za2,0,0,10,40);
				chacha.PartDrawA(415-32,246+oldy,278,90,190+64,60,12,za2,0,0,12,40);
				chacha.PartDrawA(605+32,246+oldy,290,90,10,60,12,za2,0,0,10,40);
			}else{
				s="return";
				chacha.Moji32(s,s.GetLength(),0,512-3*32,367-2*48,FALSE,255);
				s="retire";
				chacha.Moji32(s,s.GetLength(),0,512-3*32,367-1*48,FALSE,255);
				s="title";
				chacha.Moji32(s,s.GetLength(),0,512-2*32-16,367,FALSE,255);
				s="end game";
				chacha.Moji32(s,s.GetLength(),0,512-4*32,367+1*48,FALSE,255);
				//選択枠表示
				chacha.PartDrawA(410-32,268+oldy,0,0,204+64,33,8,8+ten2*3-(200-za2),0,0);
				chacha.PartDrawA(405-32,246+oldy,268,90,10,60,12,za2,0,0,10,40);
				chacha.PartDrawA(415-32,246+oldy,278,90,190+64,60,12,za2,0,0,12,40);
				chacha.PartDrawA(605+32,246+oldy,290,90,10,60,12,za2,0,0,10,40);
			}
			chacha.JikiDraw((float)mousex,(float)mousey,4*32,0,0,FALSE);
			chacha.Fade(za4*2);
			chacha.End();
			if(!chacha.DoEventChk(endf)){endf=TRUE; break; }
			chacha.End(FALSE);//描画終了
			f1=chacha.Timing64(f2,FALSE);
			timen=GetTiming(f1);
			chacha.timing1((WORD)timen,FALSE,FALSE);
			timen2=timen;
			chacha.Timing64(f2,FALSE);
			f3=chacha.Timing64(chacha.fpstiming,FALSE);
		}else timen--;
	}

	return 0;
}

//ending
void CMainFrame::EndPlay()
{
	endff=1;
	DWORD f1=0,f2=0,f3=0,za4=0;
	chacha.LoadWorkTex(1);
	//文字データ
	float tbe[][5]={
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,512,120,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,120,512, 60,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,180,512, 60,10},
		{0,240,512, 60,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,300,512, 60,10},
		{0,360,512, 60,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,420,512, 60,10},
		{0, 00,512, 60,11},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,11},
		{0, 60,512, 60,11},
		{0,120,512, 60,11},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,10},
		{0,  0,  1,  1,11},
		{0,180,512, 60,11},
		{0,240,512, 60,11},
		{0,  0,  1,  1,11},
		{0,  0,  1,  1,11},
		{0,  0,  1,  1,11},
		{1,  0,  1,  1,11},
		{0,  0,  1,  1,11},
		{0,  0,  1,  1,11},
		{0,  0,  1,  1,11},
		{0,  0,  1,  1,11},
		{0,300,512, 60,11},
		{0,360,512, 60,11},
		{0,420,512, 60,11},
		{0,  0,  1,  1,11},
		{0,  0,  1,  1,11},
		{0,  0,  1,  1,11},
		{0,  0,  1,  1,11},
		{0,  0,  1,  1,11},
		{1,  0,  1,  1,11},
		{1,  0,  1,  1,11},
		{1,  0,  1,  1,11},
		{1,  0,  1,  1,11},
		{1,  0,  1,  1,11},
		{1,  0,  1,  1,11},
		{1,  0,  1,  1,11},
		{1,  0,  1,  1,11},
		{1,  0,  1,  1,11},
		{1,  0,  1,  1,11},
		{1,  0,  1,  1,11},
		{1,  0,  1,  1,11},
		{1,  0,  1,  1,11}
	};

	int modeno=0;//最初のフェード用
	float modeno1=0;//文字スクロール用
	int modeno2=0;
	int modeno3=0;//最後のフェード用
	int modeno4=0;
	int modeno5=0;
	DWORD timen2=timen=1;
	HosiOkuMake();
	if(chacha.DoEventChk(endf)==FALSE){endf=TRUE; return; }
	chacha.Timing64(f2);
	chacha.Timing64(chacha.fpstiming,FALSE);
	for(;;){
		chacha.GetInput();
		input();
		if(tekitamaf||tamaf||movef) modeno4=1;
		if(modeno4) modeno5++;
		if(modeno5==128) return;
		mousex_=(float)mousex*gamenx/gamenx_;
		mousey_=(float)mousey*gameny/gameny_;
		modeno++;
		if(modeno>500){
			if(tbe[modeno3][0]!=1){
				modeno2++; if(modeno2==120){modeno3++; modeno2=0;}
			}
		}
		if(modeno==128) bgm="bgm03.ogg";
		HosiOkuProc();
		if(timen==1){//DirectX描画開始
			chacha.Begin();
			HosiOkuDraw();
			if(modeno<128) chacha.Fade(255-modeno*2);
			if(modeno>=128 && modeno<256) chacha.PartDrawA(0,0,0,0,1024,768,8,(modeno-128)*2);
			if(modeno>256&&modeno<384){
				for(int j=0;j<13;j++){
					chacha.PartDrawA(512-tbe[j][2]/2,60*(float)j,ftol(tbe[j][0]),ftol(tbe[j][1]),tbe[j][2],tbe[j][3],
						ftol(tbe[j][4]),255,0,0,(float)tbe[j][2],(float)tbe[j][3]);
				}
			}
			if(modeno>383){
				for(int j=-1;j<14;j++){
					if(j+modeno3<0) continue;
					if(tbe[j+modeno3][2]==1) continue;
					chacha.PartDrawA(512-tbe[j+modeno3][2]/2,60*(float)j-modeno2/2,
						ftol(tbe[j+modeno3][0]),ftol(tbe[j+modeno3][1]),tbe[j+modeno3][2],tbe[j+modeno3][3],
						ftol(tbe[j+modeno3][4]),255,0,0,(float)tbe[j+modeno3][2],(float)tbe[j+modeno3][3]);
				}
			}
			if(modeno>=256 && modeno<384) chacha.PartDrawA(0,0,0,0,1024,768,8,255-(modeno-256)*2);
			if(modeno4) chacha.Fade(modeno5*2);
			chacha.End();
			if(!chacha.DoEventChk(endf)){endf=TRUE; break; }
			chacha.End(FALSE);//描画終了
			f1=chacha.Timing64(f2,FALSE);
			timen=GetTiming(f1);
			chacha.timing1((WORD)timen,FALSE,FALSE);
			timen2=timen;
			chacha.Timing64(f2,FALSE);
			f3=chacha.Timing64(chacha.fpstiming,FALSE);
		}else timen--;
	}

	return;
}

//裏技表示
void CMainFrame::UraInfo()
{
	DWORD f1=0,f2=0,f3=0,za4=0;
	int modeno=0;
	int modeno1=0;
	int modeno2=0;
	int modeno3=0;
	CString s;
	HosiOkuMake();
	DWORD timen2=timen=1;
	if(chacha.DoEventChk(endf)==FALSE){endf=TRUE; return; }
	chacha.Timing64(f2);
	chacha.Timing64(chacha.fpstiming,FALSE);
	for(;;){
		chacha.GetInput();
		input();
		if(endf||tamaf||movef||tekitamaf) modeno1=2;
		if(modeno3>128) return;
		mousex_=(float)mousex*gamenx/gamenx_;
		mousey_=(float)mousey*gameny/gameny_;
		HosiOkuProc();
		modeno++;
		if(modeno==128) modeno1=1;
		if(modeno1==1) modeno2++;
		if(modeno1==2) modeno3++;
		if(timen==1){//DirectX描画開始
			chacha.Begin();
			HosiOkuDraw();
			if(modeno<128) chacha.Fade(255-modeno*2);
			if(modeno2<128){
				// 12345678901234567890123456789012 27 13.5
				s="esoterics summary [urawaza]"; 
				chacha.Moji32(s,s.GetLength(),0,512-13*32-16,50,TRUE,modeno2*2);
				s="invincibility[muteki]"; 
				chacha.Moji32(s,s.GetLength(),0,512-10*32-16,50+32*3,TRUE,modeno2*2);
				s="m u t e k i [f12]";
				chacha.Moji32(s,s.GetLength(),0,512- 8*32-16,50+32*4,TRUE,modeno2*2);
				s="shot infinity[mugen]"; 
				chacha.Moji32(s,s.GetLength(),0,512-10*32   ,50+32*6,TRUE,modeno2*2);
				s="m u g e n [f12]";
				chacha.Moji32(s,s.GetLength(),0,512- 6*32-16,50+32*7,TRUE,modeno2*2);
				s="burst speed up[rensha up]"; 
				chacha.Moji32(s,s.GetLength(),0,512-12*32   ,50+32*9,TRUE,modeno2*2);
				s="r e n m a x [f12]";
				chacha.Moji32(s,s.GetLength(),0,512- 8*32-16,50+32*10,TRUE,modeno2*2);
				s="all power up[muteki+mugen+renup]"; 
				chacha.Moji32(s,s.GetLength(),0,512-16*32   ,50+32*12,TRUE,modeno2*2);
				s="a l l p o w e r [f12]";
				chacha.Moji32(s,s.GetLength(),0,512-10*32-16,50+32*13,TRUE,modeno2*2);
				s="all power off"; 
				chacha.Moji32(s,s.GetLength(),0,512- 6*32-16,50+32*15,TRUE,modeno2*2);
				s="a l l o f f [f12]";
				chacha.Moji32(s,s.GetLength(),0,512- 8*32-16,50+32*16,TRUE,modeno2*2);
			}else{
				s="esoterics summary [urawaza]"; 
				chacha.Moji32(s,s.GetLength(),0,512-13*32-16,50,TRUE,255);
				s="invincibility[muteki]"; 
				chacha.Moji32(s,s.GetLength(),0,512-10*32-16,50+32*3,TRUE,255);
				s="m u t e k i [f12]";
				chacha.Moji32(s,s.GetLength(),0,512- 8*32-16,50+32*4,TRUE,255);
				s="shot infinity[mugen]"; 
				chacha.Moji32(s,s.GetLength(),0,512-10*32   ,50+32*6,TRUE,255);
				s="m u g e n [f12]";
				chacha.Moji32(s,s.GetLength(),0,512- 6*32-16,50+32*7,TRUE,255);
				s="burst speed up[rensha up]"; 
				chacha.Moji32(s,s.GetLength(),0,512-12*32   ,50+32*9,TRUE,255);
				s="r e n m a x [f12]";
				chacha.Moji32(s,s.GetLength(),0,512- 8*32-16,50+32*10,TRUE,255);
				s="all power up[muteki+mugen+renup]"; 
				chacha.Moji32(s,s.GetLength(),0,512-16*32   ,50+32*12,TRUE,255);
				s="a l l p o w e r [f12]";
				chacha.Moji32(s,s.GetLength(),0,512-10*32-16,50+32*13,TRUE,255);
				s="all power off"; 
				chacha.Moji32(s,s.GetLength(),0,512- 6*32-16,50+32*15,TRUE,255);
				s="a l l o f f [f12]";
				chacha.Moji32(s,s.GetLength(),0,512- 8*32-16,50+32*16,TRUE,255);
			}
			chacha.Fade(modeno3*2);
			chacha.End();
			if(!chacha.DoEventChk(endf)){endf=TRUE; break; }
			chacha.End(FALSE);//描画終了
			f1=chacha.Timing64(f2,FALSE);
			timen=GetTiming(f1);
			chacha.timing1((WORD)timen,FALSE,FALSE);
			timen2=timen;
			chacha.Timing64(f2,FALSE);
			f3=chacha.Timing64(chacha.fpstiming,FALSE);
		}else timen--;
	}
	return;
}
void CMainFrame::Naido(int na)
{
	DWORD f1=0,f2=0,f3=0,za4=0;
	int modeno=0;
	int modeno1=0;
	int modeno2=0;
	int modeno3=0;
	CString s;
	HosiOkuMake();
	DWORD timen2=timen=1;
	if(chacha.DoEventChk(endf)==FALSE){endf=TRUE; return; }
	chacha.Timing64(f2);
	chacha.Timing64(chacha.fpstiming,FALSE);
	for(;;){
		chacha.GetInput();
		input();
		if(endf||tamaf||movef||tekitamaf) modeno1=2;
		if(modeno3>128) return;
		mousex_=(float)mousex*gamenx/gamenx_;
		mousey_=(float)mousey*gameny/gameny_;
		HosiOkuProc();
		modeno++;
		if(modeno==128) modeno1=1;
		if(modeno1==1) modeno2++;
		if(modeno1==2) modeno3++;
		if(timen==1){//DirectX描画開始
			chacha.Begin();
			HosiOkuDraw();
			if(modeno<128) chacha.Fade(255-modeno*2);
			if(modeno2<128){
				// 12345678901234567890123456789012 27 13.5
				s="new difficulty was added!!"; 
				chacha.Moji32(s,s.GetLength(),0,512-13*32,384-16-50,TRUE,modeno2*2);
				if(na){
					s=" inferno mode ";
				}else{
					s="nightmare mode"; 
				}
				chacha.Moji32(s,s.GetLength(),0,512- 7*32,384-16,TRUE,modeno2*2);
				s="to the further challenge";
				chacha.Moji32(s,s.GetLength(),0,512-12*32,384-16+50,TRUE,modeno2*2);
			}else{
				s="new difficulty was added!!"; 
				chacha.Moji32(s,s.GetLength(),0,512-13*32,384-16-50,TRUE,255);
				if(na){
					s=" inferno mode ";
				}else{
					s="nightmare mode"; 
				}
				chacha.Moji32(s,s.GetLength(),0,512- 7*32,384-16,TRUE,255);
				s="to the further challenge";
				chacha.Moji32(s,s.GetLength(),0,512-12*32,384-16+50,TRUE,255);
			}

			chacha.Fade(modeno3*2);
			chacha.End();
			if(!chacha.DoEventChk(endf)){endf=TRUE; break; }
			chacha.End(FALSE);//描画終了
			f1=chacha.Timing64(f2,FALSE);
			timen=GetTiming(f1);
			chacha.timing1((WORD)timen,FALSE,FALSE);
			timen2=timen;
			chacha.Timing64(f2,FALSE);
			f3=chacha.Timing64(chacha.fpstiming,FALSE);
		}else timen--;
	}
	return;
}

DWORD CMainFrame::GetTiming(DWORD f)
{
	DWORD frate_=10;
	if(f<10000/60+1) frate_=10;
	if(f<9000/60+1) frate_=9;
	if(f<8000/60+1) frate_=8;
	if(f<7000/60+1) frate_=7;
	if(f<6000/60+1) frate_=6;
	if(f<5000/60+1) frate_=5;
	if(f<67) frate_=4;
	if(f<51) frate_=3;
	if(f<34) frate_=2;
	if(f<17) frate_=1;
	return frate_;

}