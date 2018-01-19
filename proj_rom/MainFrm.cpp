// MainFrm.cpp : CMainFrame �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "proj_rom.h"
#include "MainFrm.h"
commonsave save;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CFile_Class fc;
CString namef;
int enhf;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CMainFrame �N���X�̍\�z/����
CMainFrame::CMainFrame()
{
	// TODO: ���̈ʒu�Ƀ����o�̏����������R�[�h��ǉ����Ă��������B
	ZeroMemory(&save,sizeof commonsave);
	save.dev=0;
	save.full=0;
	save.star=1;
	save.bgmvol=8;
	save.bgmplayf=1;
	save.effectvol=8;
	save.effectplayf=1;
	save.joypad=0;save.sbuf=0;
	save.nanido=1;
	save.gengo=0;
	save.clearstage=1;
	save.sse=TRUE;
	save._3dnow=TRUE;
	save.key[0]=0; save.key[1]=1; save.key[2]=2; save.key[3]=3;
	save.mouse[0]=1;save.mouse[1]=0;save.mouse[2]=7;save.mouse[3]=2;
	save.maxnanido=2;
	for(int i=0;i<20;i++){
		save.score[19-i][0]=(i+1)*10000;save.score[19-i][1]=1;
		save.scorec[19-i][0]=save.scorec[19-i][1]=save.scorec[19-i][2]=save.scorec[19-i][3]=save.scorec[19-i][4]=save.scorec[19-i][5]='a';
	}
	for(i=0;i<100;i++){
		save.stagetime[i][0]=-1;
		save.stagetime[i][1]=-1;
		save.stagetime[i][2]=-1;
		save.stagetime[i][3]=-1;
		save.stagetime[i][4]=-1;
	}
	CFile f;
	if(f.Open("common.dat",CFile::shareDenyNone | CFile::modeRead ,NULL)){
		f.Read(&save,sizeof commonsave);
		f.Close();
	}
	if(save.clearstage==0)	save.clearstage=1;


	fs=FALSE;
	mousex=mousey=100;xx=yy=100;xxs=yys=0;
	movef=endf=FALSE;
	switch(save.kaizou){
	case 0:gamenx=1024; gameny=768;break;
	case 1:gamenx=800; gameny=600;break;
	case 2:gamenx=640; gameny=480;break;
	case 3:gamenx=320; gameny=240;break;
	}
	kasoku=0.16f;
	gensoku=(float)1.002;
	juryoku=(float)1.02;
	limit=5;
	srand( (unsigned)time( NULL ) );
	chacha.m_bWindow=save.full^1;
	int xm=GetSystemMetrics(SM_CXSCREEN),ym=GetSystemMetrics(SM_CYSCREEN);
	if(xm==1024 && ym==768 && gamenx==1024 && gameny==768){chacha.m_bWindow=FALSE;gamenx=1024; gameny=768;}
	if(xm==800 && ym==600 && gamenx>=800 && gameny>=600){ chacha.m_bWindow=FALSE;gamenx=800; gameny=600;}
	if(xm==640 && ym==480 && gamenx>=640 && gameny>=480){ chacha.m_bWindow=FALSE;gamenx=640; gameny=480;}
	fs=chacha.m_bWindow^1;
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
	if( !CFrameWnd::PreCreateWindow(cs) )	return FALSE;
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C�����āAWindow �N���X��X�^�C����
	//       �C�����Ă��������B
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame �N���X�̐f�f
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
// CMainFrame ���b�Z�[�W �n���h��
#include "tekipart.h"

static float setti[4][2]={{0,-1},{1,0},{0,1},{-1,0}};
static float jumpt[17]={-5,-4,-3,-2,-1,-0.5f,-0.25f,-0.125f,0,0.125f,0.25f,0.5f,1,2,3,4,5};

/////////////////////////////////////////////////////////////////////////////
void CMainFrame::Idou()
{
	int i,ii,jj,p;float xy,a,b,tss2,tss;float ii3,jj3,ii4,jj4,ii5,jj5;
	if(yys>limit) yys=limit;		if(yys<-limit) yys=-limit;	if(xxs>limit) xxs=limit;		if(xxs<-limit) xxs=-limit;
	if(movef&&hp&&gameover==0){if((mistartmode==5||mistartf==0)&&mistartmodeA<6&&(romrom==0))hp--;//�ړ��v�Z
		tss=chacha.GetShita((float)mousex,gamenx2_,(float)mousey,gameny2_);tss= tss*PI180;
		a=chacha.Getsin(tss);	b=chacha.Getcos(tss);
		xxs+=a*kasoku;			yys+=b*kasoku+(((yys==0.0f)&&(atari==TRUE))?-0.3f:0.0f);
		mot[motsu][0]=ftol(xx-a*10.0f);		mot[motsu][1]=ftol(yy-b*10.0f);
		mot[motsu][2]=4;						mot[motsu][3]=-1;
		mot[motsu][4]=-ftol(a*2.0f);				mot[motsu][5]=-ftol(b*2.0f);mot[motsu][6]=0;
		motsu++;}
	if(atari && xxs<0.09 && yys<0.09 && xxs>-0.09 && yys>-0.09) { } else	atari=FALSE;//�����蔻������H
	if(!atari && gameover==0){//�����蔻��
		for(int h=0;h<3;h++){a=xxs;b=yys;
			if(h==0){ii=ftol(xx)/32;	jj=ftol(yy+b*4)/32;	}//����
			if(h==1){ii=ftol(xx+a*4)/32;jj=ftol(yy)/32;		}//�悱
			if(h==2){ii=ftol(xx+a*4)/32;jj=ftol(yy+b*4)/32;	}//�ȂȂ�
			if(ii<0) {ii=ds.mapx+ii;}	if(ii>=(ds.mapx)) {ii=ii-ds.mapx;}
			if(jj<0) {jj=ds.mapy+jj;}	if(jj>=(ds.mapy)) {jj=jj-ds.mapy;}
			p=ds.mapdata[ii][jj];	switch(p){
			case  0:case  4:case  7:case 52:case 55:case 64:case 75:case 76:case 93:case 94:case  8:case 59:case 11:case 56:case 12:case 63:case 15:case 60:	case 78:case 92:case 128:case 129:case 130:case 144:case 145:case 146:break;
			case  3:case  6:case 23:case 33:case 36:case 53:case 67:case 69:case 83:case 85:case 137:case 167:	case 96:case  99:case 115:case 101:case 117:// ���΁^
				if(atari==FALSE){
					xy=xxs; xxs=yys+0.5f*(yys>0?1.0f:-1.0f); yys=xy+0.2f*(xy>0?1.0f:-1.0f);
					float ptt[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
					for(p=0;p<4;p++){
						if(CheckMap(ii+ptt[p][0],jj+ptt[p][1],TRUE)==0){
							xx+=ptt[p][0];yy+=ptt[p][1];
						}
					}
				}	atari=TRUE;	break;
			case  1:case  5:case 20:case 35:case 39:case 54:case 68:case 70:case 84:case 86:case 135:case 169:	case 97:case 100:case 116:case 102:case 118://�^
				if(atari==FALSE){
					xy=xxs; xxs=-yys-0.5f*(yys>0?1.0f:-1.0f); yys=-xy-0.2f*(xy>0?1.0f:-1.0f);
					float ptt[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
					for(p=0;p<4;p++){
						if(CheckMap(ii+ptt[p][0],jj+ptt[p][1],TRUE)==0){
							xx+=ptt[p][0];yy+=ptt[p][1];
						}
					}
				}	atari=TRUE;	break;
			case  9:case 24:case 25:case 42:case 43:case 58://���^�^
				if(atari==FALSE){
					if(fabs(xxs)>fabs(yys)){
						if(xxs<0){//������Ԃ���
							xy=xxs;
							xxs=-(float)fabs(yys)+xy/16-0.5f*(yys>0?1:-1);
							yys=-xy-0.5f*(xy>0?1:-1);
							xx+=2;yy+=2;
						}else{
							xy=xxs;
							xxs=(float)fabs(yys)+xy/16-0.5f*(yys>0?1:-1);
							yys=-xy-0.5f*(xy>0?1:-1);
							xx-=2;yy-=2;
						}
					}else{
						if(yys<0&&CheckMap_(xx/32,yy/32-1)){//������Ԃ���
							xy=xxs;
							xxs=(float)fabs(yys)-0.5f*(yys>0?1:-1);
							yys=(float)fabs(xy)+((float)fabs(yys))/16-0.5f*(xy>0?1:-1);
							xx+=2;yy+=2;
						}else{
							xy=xxs;
							xxs=-(float)fabs(yys)-0.5f*(yys>0?1:-1);
							yys=-(float)fabs(xy)-((float)fabs(yys))/16-0.5f*(xy>0?1:-1);
							xx-=2;yy-=2;
						}
					}//�㉺���E�ɒʂ��Ƃ��낪���邩�`�F�b�N������΂�������
					float ptt[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
					for(p=0;p<4;p++){
						if(CheckMap(ii+ptt[p][0],jj+ptt[p][1],TRUE)==0){
							xx+=ptt[p][0];yy+=ptt[p][1];
						}
					}
				}atari=TRUE;	break;
			case 10:case 26:case 27:case 40:case 41:case 57://���_�_
				if(atari==FALSE){
					if(fabs(xxs)>fabs(yys)){
						if(xxs<0){//������Ԃ���
							xy=xxs;
							xxs=(float)fabs(yys)+xy/16+0.5f*(yys>0?1:-1);
							yys=xy+0.5f*(xy>0?2:-2);
							xx+=2;yy-=2;
						}else{
							xy=xxs;
							xxs=-(float)fabs(yys)+xy/16+0.5f*(yys>0?1:-1);
							yys=xy-0.5f*(xy>0?2:-2);
							xx-=2;yy+=2;
						}
					}else{
						if(yys<0){//������Ԃ���
							xy=xxs;
							xxs=-(float)fabs(yys)+0.5f*(yys>0?1:-1);
							yys=xy+((float)fabs(yys))/16-0.5f*(xy>0?2:-2);
							xx-=2;yy+=2;
						}else{
							xy=xxs;
							xxs=(float)fabs(yys)+0.5f*(yys>0?1:-1); 
							yys=xy-((float)fabs(yys))/16-0.5f*(xy>0?2:-2);
							xx+=2;yy-=2;
						}
					}
					float ptt[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
					for(p=0;p<4;p++){
						if(CheckMap(ii+ptt[p][0],jj+ptt[p][1],TRUE)==0){
							xx+=ptt[p][0];yy+=ptt[p][1];
						}
					}
				}atari=TRUE;	break;
			case 13:case 28:case 29:case 46:case 47:case 62://���^
				if(atari==FALSE){
					if(fabs(xxs)>fabs(yys)){
						if(xxs<0){//������Ԃ���
							xy=xxs;
							xxs=-yys-xy/10;
							yys=-xy-0.5f*(yys>0?1:-1);
							xx+=2;yy+=2;
						}else{
							xy=xxs;
							xxs=-yys-xy/10;
							yys=-xy-0.5f*(yys>0?1:-1);
							xx-=2;yy-=2;
						}
					}else{
						if((yys<0&&(CheckMap_(xx/32,yy/32-1)))||xxs<0){//������Ԃ���
							xy=xxs;
							xxs=-yys;
							yys=-xy+(yys)/10-0.5f*(xy>0?1:-1);
							xx+=2;yy+=2;
						}else{
							xy=xxs;
							xxs=-yys;
							yys=-xy+(yys)/10-0.5f*(xy>0?1:-1);
							xx-=2;yy-=2;
						}
					}
					float ptt[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
					for(p=0;p<4;p++){
						if(CheckMap(ii+ptt[p][0],jj+ptt[p][1],TRUE)==0){
							xx+=ptt[p][0];yy+=ptt[p][1];
						}
					}
				}atari=TRUE;	break;
			case 14:case 30:case 31:case 44:case 45:case 61://��锽�΁_
				if(atari==FALSE){
					if(fabs(xxs)>fabs(yys)){
						if(xxs<0){//������Ԃ���
							xy=xxs;
							xxs=yys+xy/16;
							yys=xy-0.5f*(yys>0?1:-1);
							xx+=2;yy-=2;
						}else{
							xy=xxs;
							xxs=yys+xy/16;
							yys=xy-0.5f*(yys>0?1:-1);
							xx-=2;yy+=2;
						}
					}else{
						if(yys<0){//������Ԃ���
							xy=xxs;
							xxs=yys;
							yys=xy+(yys)/16-0.5f*(xy>0?1:-1);
							xx-=2;yy+=2;
						}else{
							xy=xxs;
							xxs=yys;
							yys=xy+(yys)/16-0.5f*(xy>0?1:-1);
							xx+=2;yy-=2;
						}
					}
					float ptt[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
					for(p=0;p<4;p++){
						if(CheckMap(ii+ptt[p][0],jj+ptt[p][1],TRUE)==0){
							xx+=ptt[p][0];yy+=ptt[p][1];
						}
					}
				}	atari=TRUE;	break;
			default:
				if(h==0&&atari==FALSE)	if(yys>0.18 || yys<-0.18) yys=-yys; else yys=0;
				if(h==1)xxs=-xxs;
				if(h==2&&atari==FALSE){	if(yys>0.15 || yys<-0.15) yys=-yys; else yys=0;	if(xxs>0.15 || xxs<-0.15) xxs=-xxs; else xxs=0;	}
				atari=TRUE;	break;
			}//switch
		}//for
	}//if
	if(atari){ sabun=0.15f;//�Ԃ������Ƃ��̌�������
		if((chacha.sqrt_fast(xxs*xxs+yys*yys)>4.0f||chacha.sqrt_fast(xxs*xxs+yys*yys)<-4.0f)&&(mistartf==0)){
			if((damenow==FALSE)&&(gameover==0)&&(muteki==FALSE)){damenow=TRUE; dame--; damecnt=damecnt2=0;chacha_a.Play(15);
				mot[motsu][0]=ftol(xx);		mot[motsu][1]=ftol(yy);	mot[motsu][2]=0;	mot[motsu][3]=-1;
				mot[motsu][4]=ftol(xxs);	mot[motsu][5]=ftol(yys);mot[motsu][6]=0;
				motsu++;}
			mot[motsu][0]=ftol(xx);			mot[motsu][1]=ftol(yy);
			mot[motsu][2]=2;				mot[motsu][3]=-1;
			mot[motsu][4]=0;				mot[motsu][5]=0;mot[motsu][6]=0;
			motsu++;}
	}
	//xx,yy ���ۂ̍��W	xxs,yys �ړ�����
	xx+=xxs;yy+=yys;	yys/=(gensoku+sabun);	xxs/=(gensoku+sabun);
	if(sabun>0.0f)	sabun-=0.01f; else sabun=0.0f;
	if(!atari){/*�d�͏���*/if(yys>0.1) yys*=juryoku;	else if(yys<-0.1) yys/=juryoku;	else if(movef) yys+=0.01f; else yys+=0.1f;	}
	if(yys>limit) yys=limit;		if(yys<-limit) yys=-limit;	if(xxs>limit) xxs=limit;		if(xxs<-limit) xxs=-limit;
	if(xx<0) { xx=mapx32+xx;}	if(xx>=(mapx32)) {xx=xx-mapx32;}
	if(yy<0) {yy=mapy32+yy-1;}	if(yy>=(mapy32)) {yy=yy-mapy32;}

	//���b�N�I��
	for(i=0;i<tekisu;i++){//�}�E�X�ƈ�v���Ă邩
		ii5=xx+mousex_-gamenx2;jj5=yy+mousey_-gameny2;
		if(ii5<0) ii5+=mapx32; if(ii5>mapx32) ii5-=mapx32;
		if(jj5<0) jj5+=mapy32; if(jj5>mapy32) jj5-=mapy32;
		ii4=tekidata[i][0];jj4=tekidata[i][1];
		if((ii4-16)<ii5 && (ii4+16)>ii5 &&(jj4-16)<jj5 && (jj4+16)>jj5 && tekidata[i][34]==0&&(mistartmode==5||mistartf==0)&&mistartmodeA<6)//�G�ƃJ�[�\���������Ă邩�H
			if(lockonsu<jikitama[1]||jikitama[1]==-1){//�z�[�~���O�̒e���܂�����
				for(ii=0;ii<lockonsu;ii++)//���łɃ��b�N�I�����Ă邩�H
					if(lockon[ii]==i) break;
				if(ii==lockonsu||lockonsu==0){
					ii4=tekidata[i][0]-xx;jj4=tekidata[i][1]-yy;
					if(gamenx2 > xx)if(mapx32+(xx-(gamenx2))<tekidata[i][0]){ii4=(tekidata[i][0]-(mapx32+(xx-gamenx2)))-gamenx2;}
					if(gameny2 > yy)if(mapy32+(yy-(gameny2))<tekidata[i][1]){jj4=(tekidata[i][1]-(mapy32+(yy-gameny2)))-gameny2;}
					if(mapx32-gamenx2 < xx)if(gamenx2-(mapx32-xx) > tekidata[i][0]){ii4=(mapx32-xx)+tekidata[i][0];}
					if(mapy32-gameny2 < yy)if(gameny2-(mapy32-yy) > tekidata[i][1]){jj4=(mapy32-yy)+tekidata[i][1];}
					lockon[lockonsu]=i;	lockonsu++;chacha_a.Play(30,ftol(ii4*10));
				}
			}
	}

	//-----------�e����-------------------------------------------------------------------------------------------------------------
	if(tamaf&&(mistartmode==5||mistartf==0)&&mistartmodeA<6){tamacnt++;if(renmax)tamacnt+=10;//�e
		switch(buki){case 0:p=7;break;case 1:p=30;break;case 2:p=14;break;case 3:p=20;break;case 4:p=7;break;}
		if(tamacnt>=p){	tamacnt=0;	if(tamasu<256){
				tss=chacha.GetShita((float)mousex,gamenx2_,(float)mousey,gameny2_);tss2=tama[tamasu][5]=tss;tss= tss*PI180;
				a=chacha.Getsin(tss);				b=chacha.Getcos(tss);tama[tamasu][8]=-1;tama[tamasu][9]=0;
				switch(buki){
				case 0:chacha_a.Play(10);hitp[buki][0]++;
					tama[tamasu][0]=xx;					tama[tamasu][1]=yy;
					tama[tamasu][2]=a*8;				tama[tamasu][3]=b*8;tama[tamasu][7]=-1;
					tama[tamasu][4]=(float)buki;		tama[tamasu][6]=-1; tamasu++; break;
				case 1:
					if(lockonsu){
						for(i=0;i<lockonsu;i++){if(jikitama[buki]){chacha_a.Play(10);tama[tamasu][9]=(float)i*5;hitp[buki][0]++;
						tama[tamasu][0]=xx;					tama[tamasu][1]=yy;tama[tamasu][8]=(float)lockon[i];
						tama[tamasu][2]=a*8;				tama[tamasu][3]=b*8;tama[tamasu][7]=-1;tama[tamasu][5]=tss2;
						tama[tamasu][4]=(float)buki;		tama[tamasu][6]=-1;tamasu++;if(mugen==0)jikitama[buki]--;}else lockonsu=0;
						} /*lockonsu=0;*/break;
					}else{if(jikitama[buki]){chacha_a.Play(10);hitp[buki][0]++;
						tama[tamasu][0]=xx;					tama[tamasu][1]=yy;
						tama[tamasu][2]=a*8;				tama[tamasu][3]=b*8;tama[tamasu][7]=-1;
						tama[tamasu][4]=(float)buki;		tama[tamasu][6]=-1;tamasu++;if(mugen==0)jikitama[buki]--;
						}break;
					}
				case 2:chacha_a.Play(12);
					tama[tamasu][0]=xx;					tama[tamasu][1]=yy;hitp[buki][0]++;
					tama[tamasu][2]=0;					tama[tamasu][3]=0.3f;tama[tamasu][7]=119;
					tama[tamasu][4]=(float)buki;		tama[tamasu][6]=60*15;tama[tamasu][5]=0; tamasu++; break;
				case 3:if(jikitama[buki]){chacha_a.Play(11);
					tama[tamasu][0]=xx;					tama[tamasu][1]=yy;hitp[buki][0]++;
					tama[tamasu][2]=a;					tama[tamasu][3]=b;tama[tamasu][7]=19;
					tama[tamasu][4]=(float)buki;		tama[tamasu][6]=-1;tamasu++;if(mugen==0)jikitama[buki]--;} break;
				case 4:chacha_a.Play(10);
					tama[tamasu][0]=xx;					tama[tamasu][1]=yy;hitp[buki][0]++;
					tama[tamasu][2]=a*8;				tama[tamasu][3]=b*8;tama[tamasu][7]=-1;
					tama[tamasu][4]=(float)buki;		tama[tamasu][6]=-1;tss=tama[tamasu][5]+180; tamasu++; if(tss>=360.0f) tss-=360.0f;
					tama[tamasu][5]=tss;tss= tss*PI180;
					a=chacha.Getsin(tss);				b=chacha.Getcos(tss);
					tama[tamasu][0]=xx;					tama[tamasu][1]= yy; hitp[buki][0]++;tama[tamasu][9]=0;
					tama[tamasu][2]=a*8;				tama[tamasu][3]=b*8;tama[tamasu][7]=-1;
					tama[tamasu][4]=(float)buki;		tama[tamasu][6]=-1;tamasu++; break;
				}
				if(buki!=2){
					mot[motsu][0]=ftol(xx+a*10);	mot[motsu][1]=ftol(yy+b*10);
					mot[motsu][2]=1;					mot[motsu][3]=-1;
					mot[motsu][4]=0;					mot[motsu][5]=0;mot[motsu][6]=0;
					motsu++;
				}
			}
		}
	}else{tamacnt++; if(tamacnt>30) tamacnt=30; }
	for(i=0;i<tamasu;i++){int ijf=0;//�e�ړ��A�����蔻��
		if(tama[i][9]){tama[i][9]--;continue;}
		if(tama[i][7]!=-1) tama[i][7]--;
		if((tama[i][4]==2)&&(tama[i][7]==0)) tama[i][7]=119;
		if((tama[i][4]==3)&&(tama[i][7]==0)) tama[i][7]=19;
		if(tama[i][6]!=-1){tama[i][6]--; if(ftol(tama[i][6])%10==0) tama[i][3]*=juryoku; if(tama[i][6]==0){//�@�����j
			ii=ftol(tama[i][0]-xx);jj=ftol(tama[i][1]-yy);	//��ʊO�����ׂ�
			if(gamenx > xx){if(mapx32+(xx-gamenx)<tama[i][0])ii=ftol((tama[i][0]-(mapx32+(xx-gamenx)))-gamenx);}		if(gameny > yy){if(mapy32+(ftol(yy-gameny))<ftol(tama[i][1]))jj=ftol((tama[i][1]-(mapy32+(yy-gameny)))-gameny);}
			if(mapx32-gamenx < xx){if(gamenx-(mapx32-xx) > tama[i][0])ii=ftol((mapx32-xx)+tama[i][0]);}							if(mapy32-gameny < yy){if(gameny-(mapy32-yy) > tama[i][1])jj=ftol((mapy32-yy)+tama[i][1]);}
			if(gamenx2+ii>-32 && gamenx2+ii<gamenx2+32 && gameny2+jj>-32 && gameny2+jj<gameny+32)chacha_a.Play(13);
			ii=ftol(tama[i][0]);jj=ftol(tama[i][1]);
			for(p=0;p<8;p++){
				mot[motsu][0]=ii+rand()%64-32;			mot[motsu][1]=jj+rand()%64-32;
				mot[motsu][2]=3;			mot[motsu][3]=-1; mot[motsu][6]=p*3;
				mot[motsu][4]=0;			mot[motsu][5]=0;			motsu++;
			}int hi=0;
			for(int ti=0;ti<tekisu;ti++){//�@���̔����ɓG�������������H
				if((tekidata[ti][0]-16-32)<tama[i][0] &&(tekidata[ti][0]+16+32)>tama[i][0] &&(tekidata[ti][1]-16-32)<tama[i][1] &&(tekidata[ti][1]+16+32)>tama[i][1]&&tekidata[ti][34]==0){
					tekidata[ti][5]-=4;if(hi==0){hitp[ftol(tama[i][4])][1]++;hi=1;}
					tekikill(ti);//�G�Ƀ_���[�W��
				}
			}
			for(ti=0;ti<tekitamasu;ti++){//�@���̔����ɒe�������������H
				if((tekitama[ti][0]-16-32)<tama[i][0] &&(tekitama[ti][0]+16+32)>tama[i][0] &&(tekitama[ti][1]-16-32)<tama[i][1] &&(tekitama[ti][1]+16+32)>tama[i][1]){
					Copy_tekitama(ti);
				}
			}
			//�@���̋@���U������
			ijf=0;for(int k=0;k<tamasu;k++){//�����̋@���̎���ɋ@�������邩�`�F�b�N
				if((k!=i)&&(tama[k][6]>10/*�@��*/)&&((tama[i][0]-16-64)<tama[k][0])&&((tama[i][0]+16+64)>tama[k][0])&&((tama[i][1]-16-64)<tama[k][1])&&((tama[i][1]+16+64)>tama[k][1])){
					tama[k][6]=10;ijf=1;}
			}Copy_tama(i);continue;
		}}if(tama[i][4]==3){//�Βn�̉���
			tama[i][2]*=1.020f;tama[i][3]*=1.020f;
			if(tama[i][2]>10.0f||tama[i][3]>10.0f){tama[i][2]/=1.020f;tama[i][3]/=1.020f;}
			if(tama[i][2]<-10.0f||tama[i][3]<-10.0f){tama[i][2]/=1.020f;tama[i][3]/=1.020f;}
			mot[motsu][0]=ftol(tama[i][0]-tama[i][2]*2);		mot[motsu][1]=ftol(tama[i][1]-tama[i][3]*2);
			mot[motsu][2]=1;					mot[motsu][3]=-1;
			mot[motsu][4]=-ftol(tama[i][2]);	mot[motsu][5]=-ftol(tama[i][3]);mot[motsu][6]=0;
			motsu++;
		}
		if(tama[i][4]==1&&tama[i][8]!=-1){//�z�[�~���O����
			ii5=tekidata[ftol(tama[i][8])][0];jj5=tekidata[ftol(tama[i][8])][1];
			ii4=tama[i][0];jj4=tama[i][1];
			if(gamenx > ii4)if(mapx32+(ii4-gamenx)<ii5)ii5=ii5-mapx32;
			if(gameny > jj4)if(mapy32+(jj4-gameny)<jj5)jj5=jj5-mapy32;
			if(mapx32-gamenx < ii4)if(gamenx-(mapx32-ii4) > ii5)	ii5=mapx32+ii5;
			if(mapy32-gameny < jj4)if(gameny-(mapy32-jj4) > jj5)	jj5=mapy32+jj5;
			tss=chacha.GetShita(ii5,ii4,jj5,jj4);tama[i][5]=tss;tss= tss*PI180;ii3=ii4;jj3=jj4;
			a=chacha.Getsin(tss);	b=chacha.Getcos(tss);tama[i][2]*=0.9f;tama[i][3]*=0.9f;tama[i][2]+=a;tama[i][3]+=b; 
			if(tama[i][2]<-4*3)tama[i][2]=-4*3;	if(tama[i][3]<-4*2)tama[i][3]=-4*3;
			if(tama[i][2]>4*3)tama[i][2]=4*3;	if(tama[i][3]>4*2)tama[i][3]=4*3;
			tama[i][0]+=tama[i][2];		tama[i][1]+=tama[i][3];
			ii5=tekidata[ftol(tama[i][8])][0];jj5=tekidata[ftol(tama[i][8])][1];
			ii4=tama[i][0];jj4=tama[i][1];
			if(gamenx > ii4)if(mapx32+(ii4-gamenx)<ii5)ii5=ii5-mapx32;
			if(gameny > jj4)if(mapy32+(jj4-gameny)<jj5)jj5=jj5-mapy32;
			if(mapx32-gamenx < ii4)if(gamenx-(mapx32-ii4) > ii5)	ii5=mapx32+ii5;
			if(mapy32-gameny < jj4)if(gameny-(mapy32-jj4) > jj5)	jj5=mapy32+jj5;
			tss=chacha.GetShita(ii4,ii3,jj4,jj3);tama[i][5]=tss;tss= tss*PI180;
			if(tekidata[ftol(tama[i][8])][5]<=0) tama[i][8]=-1;
		}else{	tama[i][0]+=tama[i][2];		tama[i][1]+=tama[i][3];}
		if(tama[i][0]<0			   ){tama[i][0]=mapx32+tama[i][0];}		if(tama[i][0]>=(mapx32)){tama[i][0]=tama[i][0]-mapx32;}
		if(tama[i][1]<0            ){tama[i][1]=mapy32+tama[i][1]-1;}	if(tama[i][1]>=(mapy32)){tama[i][1]=tama[i][1]-mapy32; }
		ii=ftol(tama[i][0]);jj=ftol(tama[i][1]);
		ijf=0;for(int ij=0;ij<tamasu;ij++){//�@���ɒe�����������H
			if((ij!=i/*�s��v*/)&&(tama[ij][6]!=-1/*�@��*/)&&((tama[i][0]-16-32*(tama[i][4]==3))<tama[ij][0])&&((tama[i][0]+16+32*(tama[i][4]==3))>tama[ij][0])&&((tama[i][1]-16-32*(tama[i][4]==3))<tama[ij][1])&&((tama[i][1]+16+32*(tama[i][4]==3))>tama[ij][1])){
				if(tama[i][6]==-1/*�@������Ȃ�*/){tama[ij][6]=1;ijf=1;}
			}
		}for(ij=0;ij<tekitamasu;ij++){//�G�̋@�������������H & �G���@�������������H
			if((ij!=i/*�s��v*/)&&(tekitama[ij][5]==23/*�@��*/)&&((tama[i][0]-16-32*(tama[i][4]==3))<tekitama[ij][0])&&((tama[i][0]+16+32*(tama[i][4]==3))>tekitama[ij][0])&&((tama[i][1]-16-32*(tama[i][4]==3))<tekitama[ij][1])&&((tama[i][1]+16+32*(tama[i][4]==3))>tekitama[ij][1])){
				if(tama[i][6]==-1/*�@������Ȃ�*/){tekitama[ij][6]=1;ijf=1;}}
			if((tama[i][6]!=-1/*�@��*/)&&((tama[i][0]-16)<tekitama[ij][0]&&(tama[i][0]+16)>tekitama[ij][0]&&(tama[i][1]-16)<tekitama[ij][1]&&(tama[i][1]+16)>tekitama[ij][1])){
				tama[i][6]=1;Copy_tekitama(ij);
			}
		}if(ijf==1){Copy_tama(i);continue;}
		int ata=0,tate=0,tdame=0,hi=0;
		for(int ti=0;ti<tekisu;ti++){//�G�Ƃ̓����蔻��
			if((tekidata[ti][0]-16)<tama[i][0] &&(tekidata[ti][0]+16)>tama[i][0] &&(tekidata[ti][1]-16)<tama[i][1] &&(tekidata[ti][1]+16)>tama[i][1]&&(tekidata[ti][34]==0)&&(mistartmode==5||mistartf==0)&&mistartmodeA<6){
				if(tama[i][4]==2||tama[i][4]==3){
					ii4=tama[i][0]-xx;
					if(gamenx2 > xx)if(mapx32+(xx-(gamenx2))<tama[i][0]){ii4=(tama[i][0]-(mapx32+(xx-gamenx2)))-gamenx2;}
					if(mapx32-gamenx2 < xx)if(gamenx2-(mapx32-xx) > tama[i][0]){ii4=(mapx32-xx)+tama[i][0];}
					chacha_a.Play(13,ftol(ii4*10));
					for(p=0;p<8;p++){
						mot[motsu][0]=ii+rand()%64-32;			mot[motsu][1]=jj+rand()%64-32;
						mot[motsu][2]=3;			mot[motsu][3]=-1; mot[motsu][6]=p*3;
						mot[motsu][4]=0;			mot[motsu][5]=0;			motsu++;
					}for(int ti=0;ti<tekisu;ti++){//�@���̔����ɓG�������������H
						if((tekidata[ti][0]-16-32)<tama[i][0] &&(tekidata[ti][0]+16+32)>tama[i][0] &&(tekidata[ti][1]-16-32)<tama[i][1] &&(tekidata[ti][1]+16+32)>tama[i][1])	tdame-=4;
					}
				}ata=1;if(hi==0){hitp[ftol(tama[i][4])][1]++;hi=1;}
				
				if(tekidata[ti][26]){//������
					for(int kl=0;kl<tekidata[ti][26];kl++){//���ɓ����������ǂ������ׂ�
						ii4=tama[i][5]-180;if(ii4>=360)ii4-=360;if(ii4<0)ii4+=360;
						if(ii4>=tekidata[ti][19+kl*2] && ii4<=tekidata[ti][20+kl*2])tate=1;//���ɓ�������
					}
				}if(tate)if(tekidata[ti][27+ftol(tama[i][4])])tate=1;else tate=0;
				//HIT�̕\��
				int jj_=0;
				ii4=tama[i][0]-xx;jj4=tama[i][1]-yy;
				if(gamenx2 > xx)if(mapx32+(xx-(gamenx2))<tama[i][0]){jj_++;ii4=(tama[i][0]-(mapx32+(xx-gamenx2)))-gamenx2;}
				if(gameny2 > yy)if(mapy32+(yy-(gameny2))<tama[i][1]){jj_++;jj4=(tama[i][1]-(mapy32+(yy-gameny2)))-gameny2;}
				if(mapx32-gamenx2 < xx)if(gamenx2-(mapx32-xx) > tama[i][0]){jj_++;ii4=(mapx32-xx)+tama[i][0];}
				if(mapy32-gameny2 < yy)if(gameny2-(mapy32-yy) > tama[i][1]){jj_++;jj4=(mapy32-yy)+tama[i][1];}
				if(!tate){
					if(gamenx2+ii4>0 && gamenx2+ii4<gamenx && gameny2+jj4>0 && gameny2+jj4<gameny){chacha_a.Play(31,ftol(ii4*10));}else{hit[hitcnt][0]=hit[hitcnt][1]=0; hitcnt++;}
					switch(ftol(tama[i][4])){
					case 0:case 4:tekidata[ti][5]--;break;
					case 1:tekidata[ti][5]-=2;break;
					}
					tekidata[ti][5]+=tdame;
					tekikill(ti);//�G�Ƀ_���[�W��
		
				}else{
					if(gamenx2+ii4>0 && gamenx2+ii4<gamenx && gameny2+jj4>0 && gameny2+jj4<gameny){chacha_a.Play(32,ftol(ii4*10));}else{
//						hit[hitcnt][0]=hit[hitcnt][1]=0; hitcnt++;
					}						
				}
				Copy_tama(i);
				break;
			}
		}
		if(ata) continue;

		if(ii<0) ii=mapx32_+ii;			if(jj<0) jj=mapy32_+jj;
		if(ii>=mapx32_) ii=ii-mapx32_;	if(jj>=mapy32_) jj=jj-mapy32_;
		ijf=0;switch(ds.mapdata[ii/32][jj/32]){
			case  0:case  4:case  7:case 52:case 55:case 64:case 75:case 76:case 93:case 94:case  8:case 59:case 11:case 56:case 12:case 63:case 15:case 60:	case 78:case 92:case 128:case 129:case 130:case 144:case 145:case 146:break;
			default:
				if(tama[i][4]==3||tama[i][4]==2){//�@���ƑΒn�̓����蔻��
					ii=ftol(tama[i][0]-xx);jj=ftol(tama[i][1]-yy);	//��ʊO�����ׂ�
					if(gamenx2 > xx){if(mapx32+(xx-gamenx2)<tama[i][0])ii=ftol((tama[i][0]-(mapx32+(xx-gamenx2)))-gamenx2);}	if(gameny2 > yy){if(mapy32+(yy-gameny2)<tama[i][1])jj=ftol((tama[i][1]-(mapy32+(yy-gameny2)))-gameny2);}
					if(mapx32-gamenx2 < xx){if(gamenx2-(mapx32-xx) > tama[i][0])ii=ftol((mapx32-xx)+tama[i][0]);}						if(mapy32-gameny2 < yy){if(gameny2-(mapy32-yy) > tama[i][1])jj=ftol((mapy32-yy)+tama[i][1]);}
					if(gamenx2+ii>-32 && gamenx2+ii<gamenx+32 && gameny2+jj>-32 && gameny2+jj<gameny+32)chacha_a.Play(13,ii*10);
					ii=ftol(tama[i][0]);jj=ftol(tama[i][1]);
					for(p=0;p<8;p++){
						mot[motsu][0]=ii+rand()%64-32;			mot[motsu][1]=jj+rand()%64-32;
						mot[motsu][2]=3;			mot[motsu][3]=-1; mot[motsu][6]=p*3;
						mot[motsu][4]=0;			mot[motsu][5]=0;			motsu++;
					}hi=0;
					for(int ti=0;ti<tekisu;ti++){//�@���̔����ɓG�������������H
						if((tekidata[ti][0]-16-32)<tama[i][0] &&(tekidata[ti][0]+16+32)>tama[i][0] &&(tekidata[ti][1]-16-32)<tama[i][1] &&(tekidata[ti][1]+16+32)>tama[i][1]&&tekidata[i][34]==0){
							tekidata[ti][5]-=4;if(hi==0){hitp[ftol(tama[i][4])][1]++;hi=1;}
							tekikill(ti);//�G�Ƀ_���[�W��
						}
					}
					for(ti=0;ti<tekitamasu;ti++){//�@���̔����ɒe�������������H
						if((tekitama[ti][0]-16-32)<tama[i][0] &&(tekitama[ti][0]+16+32)>tama[i][0] &&(tekitama[ti][1]-16-32)<tama[i][1] &&(tekitama[ti][1]+16+32)>tama[i][1]){
							Copy_tekitama(ti);
						}
					}
					//�@���U������
					for(int k=0;k<tamasu;k++){//�����̎���ɋ@�������邩�`�F�b�N
						if((k!=i)&&(tama[k][6]!=-1/*�@��*/)&&((tama[i][0]-16-64)<tama[k][0])&&((tama[i][0]+16+64)>tama[k][0])&&((tama[i][1]-16-64)<tama[k][1])&&((tama[i][1]+16+64)>tama[k][1])){
							tama[k][6]=10;ijf=1;
						}
					}
				}else{//���̑��̓����蔻��
					mot[motsu][0]=ii;			mot[motsu][1]=jj;
					mot[motsu][2]=2;			mot[motsu][3]=-1;
					mot[motsu][4]=0;			mot[motsu][5]=0;	mot[motsu][6]=0;		motsu++;
					//�@���U������
					for(int k=0;k<tamasu;k++){//�����̎���ɋ@�������邩�`�F�b�N
						if((k!=i)&&(tama[k][6]!=-1/*�@��*/)&&((tama[i][0]-16)<tama[k][0])&&((tama[i][0]+16)>tama[k][0])&&((tama[i][1]-16)<tama[k][1])&&((tama[i][1]+16)>tama[k][1])){
							tama[k][6]=10;ijf=1;
						}
					}
				}
				Copy_tama(i);break;
		}
	}

	//����I������
	if(bukia>0) bukia--;	if(bukia<0) bukia++;
	if(buki!=bukid&&bukia==0){
		if(buki>bukid){if(buki==4&&bukid==0) bukia=8; else bukia=-8;}
		if(buki<bukid){if(buki==0&&bukid==4) bukia=-8; else bukia=8;}
		bukid=buki;bukib=1;bukic=0;bukif=60;}
	if(bukib==1&&bukif) bukif--;
	if(bukib==1&&(!bukif)) bukic++;
	if(bukic==16&&bukib==1) bukib=0;

	//�G����
	//tekidata[1000][14];
	//tekidata2[1000][4];//0,1,2,3 �p�[�c�`��p
	//0:x 1:y 2:xx 3:yy 4:shita 5:hp 6:�j�� 7:�G�ԍ� 8,9,10,11,12,13:�J�E���g�t���O
	//8:��]���x 9:�O��̕� 10:�ړ����� 11:�ړ�����2 12:�W�����v��
	//14,15:�f�B���C 16:�Ԋu 17:�U��shita 18:��shita
	//19:���J�nshita 20;�I��shita 21:���J�nshita 22;�I��shita 23:���J�nshita 24;�I��shita 25:-- 26:���� 
	//27,28,29,30,31:���� 32:MaxHP 33:shita2 34:���(0:�G 1-4:�A�C�e��) 35:�x���\�� 36:�ǒ��J�E���g 37:�_�� 38:��,�n��
	for(i=0;i<tekisu;i++){
		if(tekidata[i][35]){tekidata[i][35]--; continue; }
		if(tekidata[i][39]) continue;
		//���@�Ƃ̓����蔻��
		if(xx-16<tekidata[i][0] &&xx+16>tekidata[i][0] &&yy-16<tekidata[i][1] &&yy+16>tekidata[i][1]&&(mistartmode==5||mistartf==0)&&mistartmodeA<6){
			int j,km=0;
			if(tekidata[i][34]==0){
				//�G�ɓ������Ă͂������
				ii4=xxs-tekidata[i][2];jj4=yys-tekidata[i][3];
				if(ii4>3.0 || ii4<-3.0 || jj4>3.0 || jj4<-3.0){
					if(tekidata[i][38]==1){//��
						if((tekidata[i][2]>0&&xxs<0)||(tekidata[i][2]<0&&xxs>0)){
							xxs=(-ii4*(ii4>0)?-1.0f:1.0f)*2;tekidata[i][2]=-ii4*(ii4>0)?1.0f:-1.0f;
						}else if((tekidata[i][3]>0&&yys<0)||(tekidata[i][3]<0&&yys>0)){
							yys=(-jj4*(jj4>0)?-1.0f:1.0f)*2;tekidata[i][3]=-jj4*(jj4>0)?1.0f:-1.0f;
						}else{
							xxs=-ii4;yys=-jj4;
						}
					}else{
						xxs=-xxs; yys=-yys;
					}km=5;
				}
				if((damenow==FALSE)&&(muteki==0)){
					dame--;tekidata[i][5]--;tekidata[i][5]-=(float)km; dame-=(km==0)?0:1;
					damenow=TRUE;damecnt=damecnt2=0;chacha_a.Play(16);
					mot[motsu][0]=ftol(xx);		mot[motsu][1]=ftol(yy);	mot[motsu][2]=0;	mot[motsu][3]=-1;
					mot[motsu][4]=ftol(xxs);	mot[motsu][5]=ftol(yys);mot[motsu][6]=0;
					motsu++;
				}
				if(tekidata[i][5]<=0){
					mot[motsu][0]=ftol(tekidata[i][0]);		mot[motsu][1]=ftol(tekidata[i][1]);
					mot[motsu][2]=5;					mot[motsu][3]=-1;
					mot[motsu][4]=1;	mot[motsu][5]=16;mot[motsu][6]=0;	motsu++;
					for(j=0;j<tamasu;j++){//�z�[�~���O�̃^�[�Q�b�g�G�̂��炵
						if(tama[j][8]>i) tama[j][8]--;
					}
					for(j=0;j<lockonsu;j++){
						if(lockon[j]==i){Copy_lockon(j);}
					}
					for(j=0;j<lockonsu;j++){
						if(lockon[j]>i) lockon[j]--;
					}
					for(j=0;j<tekitamasu;j++){
						if(ftol(tekitama[j][10])==i){
							if(ftol(tekitama[j][5])==17){//�������[�U�[
								if(ftol(tekitama[j][7])!=0){//���R��
									Copy_tekitama(j);
								}
							}
						}
					}
					switch(ftol(tekidata[i][6])){
					case 0:	break;
					case 1: Tekihaiti(ftol(tekidata[i][7]),tekisu);tekidata[tekisu][0]=tekidata[i][0];tekidata[tekisu][1]=tekidata[i][1];tekisu++;break;
					case 2: Tekihaiti(ftol(tekidata[i][7]),tekisu);tekisu++;break;
					case 3:	missioncnt--;break;
					case 4: penaltycnt++;break;
					case 9: dame=0;break;
					default:Tekihaiti(ftol(tekidata[i][7]),tekisu);tekidata[tekisu][35]=tekidata[i][6]-10;tekisu++;break;
					}Copy_tekidata(i);chacha_a.Play(17);continue;
				}else{
					mot[motsu][0]=ftol(tekidata[i][0]);			mot[motsu][1]=ftol(tekidata[i][1]);
					mot[motsu][2]=3;			mot[motsu][3]=-1; mot[motsu][6]=0;
					mot[motsu][4]=0;			mot[motsu][5]=0;			motsu++;
				}
			}else{//�A�C�e���l��
				score+=50;chacha_a.Play(19);
				switch(ftol(tekidata[i][34])){
				case 1:jikitama[3]+=50;if(jikitama[3]>=1000)jikitama[3]=999;break;
				case 2:jikitama[1]+=50;if(jikitama[1]>=1000)jikitama[1]=999;break;
				case 3:hp+=1000;if(hp>10000) hp=10000; break;
				case 4:dame+=_HP/4;if(dame>_HP) dame=_HP; break;//
					break;
				}Copy_tekidata(i);continue;
			}
		}
		for(DWORD j=0;j<128;j++){//�p�[�c������
			if(GetTekiNo(i,j)){
				//���ڃf�[�^������s
				//���O���t�B�b�N�֌W
				//���ړ��֌W
				switch(tekipart[j][1]){
				case 0:{//�n��̓G
					float xt,yt,xxt=0,yyt=0;int fl=0,fli=0,flj=0;
					//�W�����v
					if(tekipart[j][15]&&tekidata[i][12]==0||tekidata[i][36]>=22){
						int jn=((tekipart[j][15]-1)%10)*50+49;tekidata[i][36]=0;
						if(timer%(500-jn) == 0){ tekidata[i][12]=1; break; }
					}
					if(tekidata[i][12]>0){tekidata[i][12]++;flj=ftol(tekidata[i][12])-1;tekidata[i][13]=0;
						if(flj%2==0){
							switch(ftol(tekidata[i][9])){
							case 0:tekidata[i][1]-=jumpt[flj/10]*((tekipart[j][15]>10)+1);tekidata[i][0]+=tekidata[i][2];break;
							case 1:tekidata[i][0]+=jumpt[flj/10]*((tekipart[j][15]>10)+1);tekidata[i][1]+=tekidata[i][3];break;
							case 2:tekidata[i][1]+=jumpt[flj/10]*((tekipart[j][15]>10)+1);tekidata[i][0]+=tekidata[i][2];break;
							case 3:tekidata[i][0]-=jumpt[flj/10]*((tekipart[j][15]>10)+1);tekidata[i][1]+=tekidata[i][3];break;
							}
						}
						xt=tekidata[i][0];yt=tekidata[i][1];//�������g���ǂ̒�
						if(CheckMap(xt/32,yt/32)){//�ǂ̒��Ȃ甽�]
							tekidata[i][10]*=-1;tekidata[i][2]*=-1;tekidata[i][3]*=-1;
							switch(ftol(tekidata[i][9])){
							case 0:tekidata[i][0]+=tekidata[i][2]*2;break;
							case 1:tekidata[i][1]+=tekidata[i][3]*2;break;
							case 2:tekidata[i][0]+=tekidata[i][2]*2;break;
							case 3:tekidata[i][1]+=tekidata[i][3]*2;break;
							}
						}
						xt=tekidata[i][0];yt=tekidata[i][1];//�������g���ǂ̒�
//						if(CheckMap2(xt/32,yt/32)){tekidata[i][12]=0; break;}//�΂߃p�[�c�ɖ��܂����甲��
						if(tekidata[i][12]==168){ tekidata[i][12]=-3;break;}//�W�����v���[�`���I��
						if(jumpt[flj/10]>0){//���~��
							xt=tekidata[i][0]+setti[ftol(tekidata[i][9])][0]*8;
							yt=tekidata[i][1]+setti[ftol(tekidata[i][9])][1]*8;
							if(CheckMap(xt/32,yt/32)||CheckMap2(xt/32,yt/32)){//�n�\������
								tekidata[i][12]=0;
							}else break;
						}else{
							if(jumpt[flj/10]<=0){
								int m=ftol(tekidata[i][9]);switch(m){case 0:case 1:m+=2;break;case 2:case 3:m-=2;break;}
								xt=tekidata[i][0]+setti[m][0]*28;
								yt=tekidata[i][1]+setti[m][1]*28;
								if(CheckMap(xt/32,yt/32)){//�n�\������
									tekidata[i][12]=0;tekidata[i][9]=(float)m;
									switch(m){case 0:tekidata[i][33]=0;break;case 1:tekidata[i][33]=270;break;case 2:tekidata[i][33]=180;break;case 3:tekidata[i][33]=90;break;}
								}else break;
							}else break;
						}
					}flj=0;
					//�V�n�\�ړ�����
					//�ڒn�ʂ���ړ������Z�o�Ƒ����`�F�b�N
					if(tekidata[i][13]==0){//��ړ������H
						xt=tekidata[i][0];yt=tekidata[i][1];//�������g���ǂ̒�
						if(CheckMap(xt/32,yt/32)){//�ǂ̒��Ȃ甽�]
//							tekidata[i][36]++;if(tekidata[i][36]>20){
//								tekidata[i][0]+=tekidata[i][2];tekidata[i][1]+=tekidata[i][3];
//							}else{
								tekidata[i][10]*=-1;tekidata[i][2]*=-1;tekidata[i][3]*=-1;
								tekidata[i][0]+=tekidata[i][2];tekidata[i][1]+=tekidata[i][3];
								tekidata[i][0]+=tekidata[i][2];tekidata[i][1]+=tekidata[i][3];
//							}
								break;
						}
						switch(CheckMap2(xt/32,yt/32)){//�������΂߂̒����H
							case 0:xt=tekidata[i][0]+setti[ftol(tekidata[i][9])][0]*28;//�ڒn��
								yt=tekidata[i][1]+setti[ftol(tekidata[i][9])][1]*28;
								if(CheckMap(xt/32,yt/32)){//�n�\������
									tekidata[i][12]=0;
									tekidata[i][36]=0;
									//�n�\����̋����ɂ���Ă͒n�\�߂��ֈړ�������
									switch(ftol(tekidata[i][9])){
									case 0:	tekidata[i][1]=(float)(ftol(tekidata[i][1])/32)*32;break;//�ڒn�ʏ�
									case 1:	tekidata[i][0]=(float)(ftol(tekidata[i][0])/32)*32+30;break;//�ڒn�ʉE
									case 2:	tekidata[i][1]=(float)(ftol(tekidata[i][1])/32)*32+30;break;//�ڒn�ʉ�
									case 3:	tekidata[i][0]=(float)(ftol(tekidata[i][0])/32)*32;break;//�ڒn�ʍ�
									}
									//�n�\�������A�U��
									xxt=tekidata[i][2];yyt=tekidata[i][3];	Kasoku(i,j,xxt,yyt);
									switch(ftol(tekidata[i][9])){//�����ɂ���ĕK�v�Ȃ��ړ�����0�ɂ���
									case 0:case 2:yyt=0;break;//�ڒn�ʏ�//�ڒn�ʉ�
									case 1:case 3:xxt=0;break;//�ڒn�ʉE//�ڒn�ʍ�
									}
									tekidata[i][2]=xxt;tekidata[i][3]=yyt;tekidata[i][0]+=tekidata[i][2];tekidata[i][1]+=tekidata[i][3];
								}else{//�n�\���Ȃ�
									if(tekidata[i][12]<0){
										switch(ftol(tekidata[i][9])){
										case 0:tekidata[i][1]-=5;tekidata[i][0]+=tekidata[i][2];break;
										case 1:tekidata[i][0]+=5;tekidata[i][1]+=tekidata[i][3];break;
										case 2:tekidata[i][1]+=5;tekidata[i][0]+=tekidata[i][2];break;
										case 3:tekidata[i][0]-=5;tekidata[i][1]+=tekidata[i][3];break;
										}
									}else{
										fl=CheckMap2(xt/32,yt/32);//�ڒn�ʂ��₩�H
										switch(fl){
										case 0:tekidata[i][36]++;
											if(tekidata[i][12]<0) tekidata[i][12]++;//�ڒn�ʂ��Ȃ��I(90�x��]���܂���[)
											switch(ftol(tekidata[i][9])){//90�x��]��̐������Z�o
											case 0://�ڒn�ʏ�
												if(tekidata[i][10]==1){
													tekidata[i][9]=3;tekidata[i][0]+=4;tekidata[i][1]-=4;tekidata[i][33]= 90;tekidata[i][10]*=-1;
												}else{
													tekidata[i][9]=1;tekidata[i][0]-=4;tekidata[i][1]-=4;tekidata[i][33]=270;
												}break;
											case 1://�ڒn�ʉE
												if(tekidata[i][10]==1){
													tekidata[i][9]=0;tekidata[i][0]+=4;tekidata[i][1]+=4;tekidata[i][33]=  0;
												}else{
													tekidata[i][9]=2;tekidata[i][0]+=4;tekidata[i][1]-=4;tekidata[i][33]=180;tekidata[i][10]*=-1;
												}break;
											case 2://�ڒn�ʉ�
												if(tekidata[i][10]==1){
													tekidata[i][9]=3;tekidata[i][0]+=4;tekidata[i][1]+=4;tekidata[i][33]= 90;
												}else{
													tekidata[i][9]=1;tekidata[i][0]-=4;tekidata[i][1]+=4;tekidata[i][33]=270;tekidata[i][10]*=-1;
												}break;
											case 3://�ڒn�ʍ�
												if(tekidata[i][10]==1){
													tekidata[i][9]=0;tekidata[i][0]-=4;tekidata[i][1]+=4;tekidata[i][33]=  0;tekidata[i][10]*=-1;
												}else{
													tekidata[i][9]=2;tekidata[i][0]-=4;tekidata[i][0]-=4;tekidata[i][33]=180;
												}break;
											}
											break;
										case 1:if(tekidata[i][12]<0) tekidata[i][12]++;//�₠�����_
											switch(ftol(tekidata[i][9])){
											case 0:	tekidata[i][1]=(float)(ftol(tekidata[i][1])/32)*32;break;//�ڒn�ʏ�
											case 1:	tekidata[i][0]=(float)(ftol(tekidata[i][0])/32)*32+30;break;//�ڒn�ʉE
											case 2:	tekidata[i][1]=(float)(ftol(tekidata[i][1])/32)*32+30;break;//�ڒn�ʉ�
											case 3:	tekidata[i][0]=(float)(ftol(tekidata[i][0])/32)*32;break;//�ڒn�ʍ�
											}
											switch(ftol(tekidata[i][9])){//��̈ړ����v�Z
											case 0://�ڒn�ʏ�
												tekidata[i][2]=-2;tekidata[i][3]=-2;break;
											case 1://�ڒn�ʉE
												tekidata[i][2]=2;tekidata[i][3]=2;break;
											case 2://�ڒn�ʉ�
												tekidata[i][2]=2;tekidata[i][3]=2;break;
											case 3://�ڒn�ʍ�
												tekidata[i][2]=-2;tekidata[i][3]=-2;break;
											}tekidata[i][13]=16;break;
										case 2:if(tekidata[i][12]<0) tekidata[i][12]++;//�₠�����^
											switch(ftol(tekidata[i][9])){
											case 0:	tekidata[i][1]=(float)(ftol(tekidata[i][1])/32)*32;break;//�ڒn�ʏ�
											case 1:	tekidata[i][0]=(float)(ftol(tekidata[i][0])/32)*32+30;break;//�ڒn�ʉE
											case 2:	tekidata[i][1]=(float)(ftol(tekidata[i][1])/32)*32+30;break;//�ڒn�ʉ�
											case 3:	tekidata[i][0]=(float)(ftol(tekidata[i][0])/32)*32;break;//�ڒn�ʍ�
											}
											switch(ftol(tekidata[i][9])){//��̈ړ����v�Z
											case 0://�ڒn�ʏ�
												tekidata[i][2]=2;tekidata[i][3]=-2;break;
											case 1://�ڒn�ʉE
												tekidata[i][2]=2;tekidata[i][3]=-2;break;
											case 2://�ڒn�ʉ�
												tekidata[i][2]=-2;tekidata[i][3]=2;break;
											case 3://�ڒn�ʍ�
												tekidata[i][2]=-2;tekidata[i][3]=2;break;
											}tekidata[i][13]=16;break;
										}
									}
								}break;
								case 1:if(tekidata[i][12]<0) tekidata[i][12]++;//�₠�����_
									switch(ftol(tekidata[i][9])){
									case 0:	tekidata[i][1]=(float)(ftol(tekidata[i][1])/32)*32;break;//�ڒn�ʏ�
									case 1:	tekidata[i][0]=(float)(ftol(tekidata[i][0])/32)*32+30;break;//�ڒn�ʉE
									case 2:	tekidata[i][1]=(float)(ftol(tekidata[i][1])/32)*32+30;break;//�ڒn�ʉ�
									case 3:	tekidata[i][0]=(float)(ftol(tekidata[i][0])/32)*32;break;//�ڒn�ʍ�
									}
									switch(ftol(tekidata[i][9])){//��̈ړ����v�Z
									case 0://�ڒn�ʏ�
										tekidata[i][2]=2;tekidata[i][3]=2;break;
									case 1://�ڒn�ʉE
										tekidata[i][2]=-2;tekidata[i][3]=-2;break;
									case 2://�ڒn�ʉ�
										tekidata[i][2]=-2;tekidata[i][3]=-2;break;
									case 3://�ڒn�ʍ�
										tekidata[i][2]=2;tekidata[i][3]=2;break;
									}tekidata[i][13]=16;break;
								case 2:if(tekidata[i][12]<0) tekidata[i][12]++;//�₠�����^
									switch(ftol(tekidata[i][9])){
									case 0:	tekidata[i][1]=(float)(ftol(tekidata[i][1])/32)*32;break;//�ڒn�ʏ�
									case 1:	tekidata[i][0]=(float)(ftol(tekidata[i][0])/32)*32+30;break;//�ڒn�ʉE
									case 2:	tekidata[i][1]=(float)(ftol(tekidata[i][1])/32)*32+30;break;//�ڒn�ʉ�
									case 3:	tekidata[i][0]=(float)(ftol(tekidata[i][0])/32)*32;break;//�ڒn�ʍ�
									}
									switch(ftol(tekidata[i][9])){//��̈ړ����v�Z
									case 0://�ڒn�ʏ�
										tekidata[i][2]=-2;tekidata[i][3]=2;break;
									case 1://�ڒn�ʉE
										tekidata[i][2]=-2;tekidata[i][3]=2;break;
									case 2://�ڒn�ʉ�
										tekidata[i][2]=2;tekidata[i][3]=-2;break;
									case 3://�ڒn�ʍ�
										tekidata[i][2]=2;tekidata[i][3]=-2;break;
									}tekidata[i][13]=16;break;
								}
					}else{//��ړ�
						tekidata[i][13]--;if(tekidata[i][12]<0) tekidata[i][12]++;
						tekidata[i][0]+=tekidata[i][2];tekidata[i][1]+=tekidata[i][3];
					}
					switch(tekipart[j][4]){//�\������
					case 0:	ii4=tekidata[i][0]-xx;jj4=tekidata[i][1]-yy;
							if(gamenx2 > xx)if(mapx32+(xx-(gamenx2))<tekidata[i][0])ii4=(tekidata[i][0]-(mapx32+(xx-gamenx2)))-gamenx2;
							if(gameny2 > yy)if(mapy32+(yy-(gameny2))<tekidata[i][1])jj4=(tekidata[i][1]-(mapy32+(yy-gameny2)))-gameny2;
							if(mapx32-gamenx2 < xx)if(gamenx2-(mapx32-xx) > tekidata[i][0])	ii4=(mapx32-xx)+tekidata[i][0];
							if(mapy32-gameny2 < yy)if(gameny2-(mapy32-yy) > tekidata[i][1])	jj4=(mapy32-yy)+tekidata[i][1];
							ii3=xx+ii4; jj3=yy+jj4;	tekidata[i][4]=chacha.GetShita(xx,ii3,yy,jj3);break;//���@
					default://���X�ɉ�]
						if(tekidata[i][33]!=tekidata[i][4]){
							float aa=tekidata[i][33]-tekidata[i][4];//�������o�����������։�]
							if(tekidata[i][4]>180&&tekidata[i][33]==0) aa+=360; 
							if(tekidata[i][33]>180&&tekidata[i][4]==0) aa-=360; 
							if(aa<-180){ aa+=360; aa*=-1;}
							if(aa>0) tekidata[i][4]+=8; else tekidata[i][4]-=8;
							if(tekidata[i][4]>=360)tekidata[i][4]-=360; if(tekidata[i][4]<0) tekidata[i][4]+=360;
							if((tekidata[i][33]-8)<=tekidata[i][4] && (tekidata[i][33]+8)>tekidata[i][4]) tekidata[i][4]=tekidata[i][33];
							
						}
					}
					break;
					}
				case 1:{//�󒆂̓G
					switch(tekipart[j][4]){//�\������
					case 0:	ii4=tekidata[i][0]-xx;jj4=tekidata[i][1]-yy;
							if(gamenx2 > xx)if(mapx32+(xx-(gamenx2))<tekidata[i][0])ii4=(tekidata[i][0]-(mapx32+(xx-gamenx2)))-gamenx2;
							if(gameny2 > yy)if(mapy32+(yy-(gameny2))<tekidata[i][1])jj4=(tekidata[i][1]-(mapy32+(yy-gameny2)))-gameny2;
							if(mapx32-gamenx2 < xx)if(gamenx2-(mapx32-xx) > tekidata[i][0])	ii4=(mapx32-xx)+tekidata[i][0];
							if(mapy32-gameny2 < yy)if(gameny2-(mapy32-yy) > tekidata[i][1])	jj4=(mapy32-yy)+tekidata[i][1];
							ii3=xx+ii4; jj3=yy+jj4;	tekidata[i][4]=chacha.GetShita(xx,ii3,yy,jj3);break;//���@
					case 1:tekidata[i][4]=360-45; break;//����
					case 2:tekidata[i][4]=0     ; break;//��
					case 3:tekidata[i][4]=45    ; break;//�E��
					case 4:tekidata[i][4]=360-90; break;//��
					case 5:tekidata[i][4]-=tekipart[j][8]*0.6f;if(tekidata[i][4]<0)tekidata[i][4]+=360;break;//�E��]
					case 6:tekidata[i][4]=90    ; break;//�E
					case 7:tekidata[i][4]=360-135;break;//����
					case 8:tekidata[i][4]=360-180;break;//��
					case 9:tekidata[i][4]=135   ; break;//�E��
					case 10:tekidata[i][4]+=tekipart[j][8]*0.6f;if(tekidata[i][4]>=360)tekidata[i][4]-=360;break;//����]
					}
					Kasoku(i,j,tekidata[i][2],tekidata[i][3]);
					if(CheckMap((tekidata[i][0]+tekidata[i][2])/32,(tekidata[i][1]+tekidata[i][3])/32,TRUE)){
						switch(tekipart[j][10]){
						case 1:tekidata[i][2]=-tekidata[i][2];tekidata[i][3]=-tekidata[i][3];{
							float ptt[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
							for(p=0;p<4;p++){
								if(CheckMap((tekidata[i][0]+tekidata[i][2])/32+ptt[p][0],(tekidata[i][1]+tekidata[i][3])/32+ptt[p][1],TRUE)==0){
									tekidata[i][0]+=ptt[p][0];tekidata[i][1]+=ptt[p][1];
								}
							   }}
							break;
						case 2:tekidata[i][2]=0;tekidata[i][3]=0;
						}
					}
					tekidata[i][0]+=tekidata[i][2];tekidata[i][1]+=tekidata[i][3];
					   }break;
				case 2:{//�U���p�[�c
					switch(tekipart[j][21]){//�\������
					case 0:	ii4=tekidata[i][0]-xx;jj4=tekidata[i][1]-yy;
							if(gamenx2 > xx)if(mapx32+(xx-(gamenx2))<tekidata[i][0]+64)ii4=(tekidata[i][0]-(mapx32+(xx-gamenx2)))-gamenx2;
							if(gameny2 > yy)if(mapy32+(yy-(gameny2))<tekidata[i][1]+64)jj4=(tekidata[i][1]-(mapy32+(yy-gameny2)))-gameny2;
							if(mapx32-gamenx2 < xx)if(gamenx2-(mapx32-xx) > tekidata[i][0]-64)	ii4=(mapx32-xx)+tekidata[i][0];
							if(mapy32-gameny2 < yy)if(gameny2-(mapy32-yy) > tekidata[i][1]-64)	jj4=(mapy32-yy)+tekidata[i][1];
							ii3=xx+ii4; jj3=yy+jj4;	tekidata[i][17]=chacha.GetShita(xx,ii3,yy,jj3);break;//���@
					case 9:tekidata[i][17]=tekidata[i][4]-45; break;//����
					case 8:tekidata[i][17]=tekidata[i][4]+0     ; break;//��
					case 7:tekidata[i][17]=tekidata[i][4]+45    ; break;//�E��
					case 6:tekidata[i][17]=tekidata[i][4]-90; break;//��
					case 5:tekidata[i][17]-=tekipart[j][8]*0.6f;if(tekidata[i][17]<0)tekidata[i][17]+=360;break;//�E��]
					case 4:tekidata[i][17]=tekidata[i][4]+90    ; break;//�E
					case 3:tekidata[i][17]=tekidata[i][4]-135;break;//����
					case 2:tekidata[i][17]=tekidata[i][4]-180;break;//��
					case 1:tekidata[i][17]=tekidata[i][4]+135   ; break;//�E��
					case 10:tekidata[i][17]+=tekipart[j][8]*0.6f;if(tekidata[i][17]>=360)tekidata[i][17]-=360;break;//����]
					}
					if(tekidata[i][17]>=360)tekidata[i][17]-=360;
					if(tekidata[i][17]<0)tekidata[i][17]+=360;
					int ll=4,lll=4;
					switch(save.nanido){
					case 0:case 1:ll=4;lll=4;break;
					case 2:ll=3;lll=4;break;
					case 3:ll=2;lll=3;break;
					case 4:ll=2;lll=2;break;
					}
					if(tekidata[i][16]>tekipart[j][24]*ll){//�Ԋu
						if(tekidata[i][15]>tekipart[j][23]*lll){//�f�B���C
							tss= tekidata[i][17]*PI180;	a=chacha.Getsin(tss);	b=chacha.Getcos(tss);
							Tekitama(tekipart[j][20],tekidata[i][0]+a*8,tekidata[i][1]+b*8,tekidata[i][17],i);//�e���ˁA�f�B���C
							tekidata[i][15]=0;tekidata[i][14]++;
							if(tekidata[i][14]>=tekipart[j][22]){//�e��
								tekidata[i][14]=0;tekidata[i][16]=0;
							}
						}else tekidata[i][15]++;
					}else tekidata[i][16]++;
					   }break;
				case 3:{//�h��p�[�c17:�U��shita 18:��shita
					//19:���J�nshita 20;�I��shita 21:���J�nshita 22;�I��shita 23:���J�nshita 24;�I��shita 25:-- 26:���� 
					//27,28,29,30,31:����
					switch(tekipart[j][26]){//�\������
					case 0:	ii4=tekidata[i][0]-xx;jj4=tekidata[i][1]-yy;
							if(gamenx2 > xx)if(mapx32+(xx-(gamenx2))<tekidata[i][0])ii4=(tekidata[i][0]-(mapx32+(xx-gamenx2)))-gamenx2;
							if(gameny2 > yy)if(mapy32+(yy-(gameny2))<tekidata[i][1])jj4=(tekidata[i][1]-(mapy32+(yy-gameny2)))-gameny2;
							if(mapx32-gamenx2 < xx)if(gamenx2-(mapx32-xx) > tekidata[i][0])	ii4=(mapx32-xx)+tekidata[i][0];
							if(mapy32-gameny2 < yy)if(gameny2-(mapy32-yy) > tekidata[i][1])	jj4=(mapy32-yy)+tekidata[i][1];
							ii3=xx+ii4; jj3=yy+jj4;	tekidata[i][18]=chacha.GetShita(xx,ii3,yy,jj3);break;//���@
					case 1:tekidata[i][18]=360-45; break;//����
					case 2:tekidata[i][18]=0     ; break;//��
					case 3:tekidata[i][18]=45    ; break;//�E��
					case 4:tekidata[i][18]=360-90; break;//��
					case 5:tekidata[i][18]-=tekipart[j][8]*0.6f;if(tekidata[i][18]<0)tekidata[i][18]+=360;break;//�E��]
					case 6:tekidata[i][18]=90    ; break;//�E
					case 7:tekidata[i][18]=360-135;break;//����
					case 8:tekidata[i][18]=360-180;break;//��
					case 9:tekidata[i][18]=135   ; break;//�E��
					case 10:tekidata[i][18]+=tekipart[j][8]*0.6f;if(tekidata[i][18]>=360)tekidata[i][18]-=360;break;//����]
					}
					switch(tekipart[j][27]){//�h��ϐ�
					case 0:tekidata[i][27]=tekidata[i][28]=tekidata[i][29]=tekidata[i][30]=tekidata[i][31]=0;break;//�O�F-
					case 1:tekidata[i][27]=1;tekidata[i][28]=tekidata[i][29]=tekidata[i][30]=0;tekidata[i][31]=1;break;//�P�F�}�V���K�����QWAY
					case 2:tekidata[i][28]=1;tekidata[i][27]=tekidata[i][29]=tekidata[i][30]=tekidata[i][31]=0;break;//�Q�F�z�[�~���O
					case 3:tekidata[i][27]=tekidata[i][28]=tekidata[i][31]=1;tekidata[i][30]=tekidata[i][29]=0;break;//�R�F�}�V���K�����QWAY�@���@�z�[�~���O	
					case 4:tekidata[i][27]=tekidata[i][28]=tekidata[i][31]=0;tekidata[i][29]=tekidata[i][30]=1;break;//�S�F�~�T�C�����@���i�����j
					case 5:tekidata[i][28]=0;tekidata[i][27]=tekidata[i][29]=tekidata[i][30]=tekidata[i][31]=0;break;//�T�F�}�V���K�����QWAY�@���@�~�T�C�����@���i�����j
					case 6:tekidata[i][27]=tekidata[i][31]=0;tekidata[i][29]=tekidata[i][30]=tekidata[i][28]=1;break;//�U�F�z�[�~���O�@���@�~�T�C�����@���i�����j
					case 7:tekidata[i][27]=tekidata[i][28]=tekidata[i][29]=tekidata[i][30]=tekidata[i][31]=1;break;//�V�F�S��
					}
					switch(tekipart[j][25]){//�����
					case 0://��
						if(tekidata[i][18]<45){
							tekidata[i][19]=0;tekidata[i][20]=tekidata[i][18]+45;
							tekidata[i][21]=360+tekidata[i][18]-45;tekidata[i][22]=360;
							tekidata[i][26]=2;
						}else if(tekidata[i][18]>(360-45)){
							tekidata[i][19]=0;tekidata[i][20]=tekidata[i][18]+45-360;
							tekidata[i][21]=tekidata[i][18]-45;tekidata[i][22]=360;
							tekidata[i][26]=2;
						}else{
							tekidata[i][19]=tekidata[i][18]-45;tekidata[i][20]=tekidata[i][18]+45;
							tekidata[i][26]=1;
						}break;
					case 1://�㉺
						if(tekidata[i][18]<45){
							tekidata[i][19]=0;tekidata[i][20]=tekidata[i][18]+45;
							tekidata[i][21]=360+tekidata[i][18]-45;tekidata[i][22]=360;
							tekidata[i][23]=tekidata[i][18]+135;tekidata[i][24]=tekidata[i][18]+225;
							if(tekidata[i][23]>=360)tekidata[i][23]-=360;if(tekidata[i][24]>=360)tekidata[i][24]-=360;
							tekidata[i][26]=3;
						}else if(tekidata[i][18]>(360-45)){
							tekidata[i][19]=0;tekidata[i][20]=tekidata[i][18]+45-360;
							tekidata[i][21]=tekidata[i][18]-45;tekidata[i][22]=360;
							tekidata[i][23]=tekidata[i][18]+135;tekidata[i][24]=tekidata[i][18]+225;
							if(tekidata[i][23]>=360)tekidata[i][23]-=360;if(tekidata[i][24]>=360)tekidata[i][24]-=360;
							tekidata[i][26]=3;
						}else{
							tekidata[i][19]=tekidata[i][18]-45;tekidata[i][20]=tekidata[i][18]+45;
							tekidata[i][21]=tekidata[i][18]+135;tekidata[i][22]=tekidata[i][18]+225;
							if(tekidata[i][21]>=360)tekidata[i][21]-=360;if(tekidata[i][22]>=360)tekidata[i][22]-=360;
							tekidata[i][26]=2;
							if(tekidata[i][21]>tekidata[i][22]){
								tekidata[i][23]=tekidata[i][21];tekidata[i][24]=360;
								tekidata[i][21]=0;tekidata[i][26]++;
							}
						}break;
					case 2://�㍶�E
						if(tekidata[i][18]<135){
							tekidata[i][19]=0;tekidata[i][20]=tekidata[i][18]+135;
							tekidata[i][21]=360+tekidata[i][18]-135;tekidata[i][22]=360;
							tekidata[i][26]=2;
						}else if(tekidata[i][18]>(360-135)){
							tekidata[i][19]=0;tekidata[i][20]=tekidata[i][18]+135-360;
							tekidata[i][21]=tekidata[i][18]-135;tekidata[i][22]=360;
							tekidata[i][26]=2;
						}else{
							tekidata[i][19]=tekidata[i][18]-135;tekidata[i][20]=tekidata[i][18]+135;
							tekidata[i][26]=1;
						}break;
					case 3://�S����
							tekidata[i][19]=0;tekidata[i][20]=360;
							tekidata[i][26]=1;
					   }break;
					   }
				case 4://
				case 5://�T�F�z�[�~���O�⋋
				case 6://�U�F�G�l���M�[�⋋
				case 7://�V�F�_���[�W�����i���y�A�j	
					break;
				}//switch
			}//GetTekiNo
		}//�p�[�c
		if(tekidata[i][0]<0) tekidata[i][0]+=mapx32;
		if(tekidata[i][1]<0) tekidata[i][1]+=mapy32;
		if(tekidata[i][0]>=mapx32) tekidata[i][0]-=mapx32;
		if(tekidata[i][1]>=mapy32) tekidata[i][1]-=mapy32;
	}

	//-------------------------------------------------------------------------------------------------------------
	//�G�e�ړ������A�Ǔ����蔻��A���@�����蔻��
	//-------------------------------------------------------------------------------------------------------------
	for(i=0;i<tekitamasu;i++){//0:x 1:y 2:xx 3:yy 4:�������� 5:�e��� 6:�x�� 7:�J�E���g 8:shita 9:alpha
		int eff=0;
		ii4=tekitama[i][0]-xx;jj4=tekitama[i][1]-yy;
		if(gamenx2 > xx)if(mapx32+(xx-(gamenx2))<tekitama[i][0])ii4=(tekitama[i][0]-(mapx32+(xx-gamenx2)))-gamenx2;
		if(gameny2 > yy)if(mapy32+(yy-(gameny2))<tekitama[i][1])jj4=(tekitama[i][1]-(mapy32+(yy-gameny2)))-gameny2;
		if(mapx32-gamenx2 < xx)if(gamenx2-(mapx32-xx) > tekitama[i][0])	ii4=(mapx32-xx)+tekitama[i][0];
		if(mapy32-gameny2 < yy)if(gameny2-(mapy32-yy) > tekitama[i][1])	jj4=(mapy32-yy)+tekitama[i][1];
		if(gamenx2+ii4>0 && gamenx2+ii4<gamenx && gameny2+jj4>0 && gameny2+jj4<gameny){eff=1;}
		switch(ftol(tekitama[i][5])){
		case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:case 11:case 12:case 27:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 43:case 44:case 45:case 46:
			tekitama[i][0]+=tekitama[i][2];		tekitama[i][1]+=tekitama[i][3]; break;
		case 13:case 14:case 15:case 16://���
			tekitama[i][7]++;	if(tekitama[i][7]==60){ tekitama[i][4]=1;if(eff&&tekitama[i][6])chacha_a.Play(14,ftol(ii4*10));}	if(tekitama[i][7]>260) tekitama[i][4]=2;
			tekitama[i][9]=128;	if(tekitama[i][7]>9)tekitama[i][9]=0;//if(tekitama[i][7]>0) tekitama[i][9]=128-(tekitama[i][7])*4;
			if(tekitama[i][4]==1) tekitama[i][9]=255;	if(tekitama[i][7]>200) tekitama[i][9]=128-(tekitama[i][7]-240)*2;
			if(tekitama[i][4]==2){Copy_tekitama(i);continue;
			}break;
		case 17://�������[�U�[ 7:0�ɂȂ����玩�R 6:�����o���܂ł̎��� 4:�G�L��������̋��� 10:�������G�ԍ�
			ii4=tekidata[ftol(tekitama[i][10])][0];jj4=tekidata[ftol(tekitama[i][10])][1];
			tss=tekidata[ftol(tekitama[i][10])][17]*PI180;a=chacha.Getsin(tss);	b=chacha.Getcos(tss);
			ii3=ii4+a*16;	jj3=jj4+b*16;
			if(!tekitama[i][6]){
				if(tekitama[i][7]!=0){tekitama[i][7]--;
					if(gamenx           >ii4)if(mapx32+(ii4-gamenx)<ii3)ii3=ii3-mapx32;
					if(gameny           >jj4)if(mapy32+(jj4-gameny)<jj3)jj3=jj3-mapy32;
					if(mapx32-gamenx<ii4)if(gamenx-(mapx32-ii4)>ii3)ii3=mapx32+ii3;
					if(mapy32-gameny<jj4)if(gameny-(mapy32-jj4)>jj3)jj3=mapy32+jj3;
					tss=chacha.GetShita(ii3,ii4,jj3,jj4);tekitama[i][8]=tss;tss= tss*PI180;
					a=chacha.Getsin(tss);	b=chacha.Getcos(tss);
					tekitama[i][2]=a*4;	tekitama[i][3]=b*4;	tekitama[i][0]=ii4+a*tekitama[i][4];tekitama[i][1]=jj4+b*tekitama[i][4];
					if(tekitama[i][0]<0) { tekitama[i][0]=mapx32+tekitama[i][0];}	if(tekitama[i][0]>=(mapx32)) {tekitama[i][0]=tekitama[i][0]-mapx32;}
					if(tekitama[i][1]<0) {tekitama[i][1]=mapy32+tekitama[i][1]-1;}	if(tekitama[i][1]>=(mapy32)) {tekitama[i][1]=tekitama[i][1]-mapy32;}
				tekitama[i][4]+=4;}else{	tekitama[i][0]+=tekitama[i][2];		tekitama[i][1]+=tekitama[i][3]; break;	}
			}else{tekitama[i][6]--;
				if(gamenx           >ii4)if(mapx32+(ii4-gamenx)<ii3)ii3=ii3-mapx32;
				if(gameny           >jj4)if(mapy32+(jj4-gameny)<jj3)jj3=jj3-mapy32;
				if(mapx32-gamenx<ii4)if(gamenx-(mapx32-ii4)>ii3)ii3=mapx32+ii3;
				if(mapy32-gameny<jj4)if(gameny-(mapy32-jj4)>jj3)jj3=mapy32+jj3;
				tss=chacha.GetShita(ii3,ii4,jj3,jj4);tekitama[i][8]=tss;tss= tss*PI180;
				a=chacha.Getsin(tss);	b=chacha.Getcos(tss);
				tekitama[i][2]=a*4;	tekitama[i][3]=b*4;	tekitama[i][0]=ii4+a*tekitama[i][4];tekitama[i][1]=jj4+b*tekitama[i][4];
				if(tekitama[i][0]<0) { tekitama[i][0]=mapx32+tekitama[i][0];}	if(tekitama[i][0]>=(mapx32)) {tekitama[i][0]=tekitama[i][0]-mapx32;}
				if(tekitama[i][1]<0) {tekitama[i][1]=mapy32+tekitama[i][1]-1;}	if(tekitama[i][1]>=(mapy32)) {tekitama[i][1]=tekitama[i][1]-mapy32;}
			}break;
		case 18://�����~�T�C��
			tekitama[i][2]*=1.020f;tekitama[i][3]*=1.020f;
			if(tekitama[i][2]>10.0f||tekitama[i][3]>10.0f){tekitama[i][2]/=1.020f;tekitama[i][3]/=1.020f;}
			if(tekitama[i][2]<-10.0f||tekitama[i][3]<-10.0f){tekitama[i][2]/=1.020f;tekitama[i][3]/=1.020f;}
			mot[motsu][0]=ftol(tekitama[i][0]);		mot[motsu][1]=ftol(tekitama[i][1]);	mot[motsu][2]=1;	mot[motsu][3]=-1;
			mot[motsu][4]=-ftol(tekitama[i][2]);	mot[motsu][5]=-ftol(tekitama[i][3]);mot[motsu][6]=0;
			motsu++;	tekitama[i][0]+=tekitama[i][2];		tekitama[i][1]+=tekitama[i][3]; break;
		case 19:case 20://�z�[�~���O
			ii4=tekitama[i][0]-xx;jj4=tekitama[i][1]-yy;
			if(gamenx > xx)if(mapx32+(xx-(gamenx))<tekitama[i][0])ii4=(tekitama[i][0]-(mapx32+(xx-gamenx)))-gamenx;
			if(gameny > yy)if(mapy32+(yy-(gameny))<tekitama[i][1])jj4=(tekitama[i][1]-(mapy32+(yy-gameny)))-gameny;
			if(mapx32-gamenx < xx)if(gamenx-(mapx32-xx) > tekitama[i][0])	ii4=(mapx32-xx)+tekitama[i][0];
			if(mapy32-gameny < yy)if(gameny-(mapy32-yy) > tekitama[i][1])	jj4=(mapy32-yy)+tekitama[i][1];
			ii3=xx+ii4; jj3=yy+jj4;	tss=chacha.GetShita(xx,ii3,yy,jj3);tekitama[i][7]=tss;	tekitama[i][8]=tss;tss= tss*PI180;
			a=chacha.Getsin(tss);	b=chacha.Getcos(tss);tekitama[i][2]+=a/5;	tekitama[i][3]+=b/5; 
			if(tekitama[i][2]<-4*tekitama[i][6])tekitama[i][2]=-4*tekitama[i][6];			if(tekitama[i][3]<-4*tekitama[i][6])tekitama[i][3]=-4*tekitama[i][6];
			if(tekitama[i][2]>4*tekitama[i][6])tekitama[i][2]=4*tekitama[i][6];			if(tekitama[i][3]>4*tekitama[i][6])tekitama[i][3]=4*tekitama[i][6];
			tekitama[i][0]+=tekitama[i][2];		tekitama[i][1]+=tekitama[i][3];
			ii4=tekitama[i][0]-xx;jj4=tekitama[i][1]-yy;
			if(gamenx > xx)if(mapx32+(xx-(gamenx))<tekitama[i][0])ii4=(tekitama[i][0]-(mapx32+(xx-gamenx)))-gamenx;
			if(gameny > yy)if(mapy32+(yy-(gameny))<tekitama[i][1])jj4=(tekitama[i][1]-(mapy32+(yy-gameny)))-gameny;
			if(mapx32-gamenx < xx)if(gamenx-(mapx32-xx) > tekitama[i][0])	ii4=(mapx32-xx)+tekitama[i][0];
			if(mapy32-gameny < yy)if(gameny-(mapy32-yy) > tekitama[i][1])	jj4=(mapy32-yy)+tekitama[i][1];
			tss=chacha.GetShita(xx+ii4,ii3,yy+jj4,jj3);tekitama[i][8]=tss;tss= tss*PI180;
			if(tekitama[i][5]==20){
				mot[motsu][0]=ftol(tekitama[i][0]);		mot[motsu][1]=ftol(tekitama[i][1]);	mot[motsu][2]=1;	mot[motsu][3]=-1;
				mot[motsu][4]=-ftol(tekitama[i][2]);	mot[motsu][5]=-ftol(tekitama[i][3]);mot[motsu][6]=0;
				motsu++;}
			break;
		case 22://��]
			if(tekitama[i][3]>0.1) tekitama[i][3]*=juryoku;	else if(tekitama[i][3]<-0.1) tekitama[i][3]/=juryoku;	else tekitama[i][3]+=0.1f;	
			tekitama[i][8]+=4;if(tekitama[i][8]>=360)tekitama[i][8]-=360;
			tekitama[i][0]+=tekitama[i][2];		tekitama[i][1]+=tekitama[i][3]; 
			if(tekitama[i][3]>4) tekitama[i][3]=4;	if(tekitama[i][3]<-4) tekitama[i][3]=-4;
			if(tekitama[i][2]>4) tekitama[i][2]=4;	if(tekitama[i][2]<-4) tekitama[i][2]=-4;
			break;
		case 21:
			tekitama[i][8]+=4;if(tekitama[i][8]>=360)tekitama[i][8]-=360;
			tekitama[i][0]+=tekitama[i][2];		tekitama[i][1]+=tekitama[i][3]; 
			if(tekitama[i][3]>4) tekitama[i][3]=4;	if(tekitama[i][3]<-4) tekitama[i][3]=-4;
			if(tekitama[i][2]>4) tekitama[i][2]=4;	if(tekitama[i][2]<-4) tekitama[i][2]=-4;
			break;
		case 23://�ړ�����
			if(tekitama[i][6]==1){
				for(p=0;p<16;p++){
					mot[motsu][0]=ftol(tekitama[i][0])+rand()%96-32;			mot[motsu][1]=ftol(tekitama[i][1])+rand()%96-32;
					mot[motsu][2]=3;			mot[motsu][3]=-1; mot[motsu][6]=p*3;
					mot[motsu][4]=0;			mot[motsu][5]=0;			motsu++;
				}
				for(int ti=0;ti<tamasu;ti++){//�@���̔����ɒe�������������H
					if((tama[ti][0]-16-64)<tekitama[i][0] &&(tama[ti][0]+16+64)>tekitama[i][0] &&(tama[ti][1]-16-64)<tekitama[i][1] &&(tama[ti][1]+16+64)>tekitama[i][1]){
						Copy_tama(ti);
					}
				}
				if(xx-80 < tekitama[i][0] && xx+80 > tekitama[i][0] && yy-80 < tekitama[i][1] && yy+80 > tekitama[i][1] && (damenow==FALSE)&&(gameover==0)&&(muteki==0)){
					damenow=TRUE; dame--;switch(save.nanido){case 3:dame--;break;case 4:dame-=2;break;} damecnt=damecnt2=0;chacha_a.Play(16);
					mot[motsu][0]=ftol(xx);		mot[motsu][1]=ftol(yy);	mot[motsu][2]=0;	mot[motsu][3]=-1;
					mot[motsu][4]=ftol(xxs);	mot[motsu][5]=ftol(yys);mot[motsu][6]=0;
					motsu++;
				}Copy_tekitama(i);
				continue;
			}
			if(tekitama[i][7]==10){tekitama[i][7]=0;tekitama[i][3]*=juryoku;}else tekitama[i][7]++;
			tekitama[i][8]+=4;if(tekitama[i][8]>=360)tekitama[i][8]-=360;
			if(tekitama[i][3]>4) tekitama[i][3]=4;	if(tekitama[i][3]<-4) tekitama[i][3]=-4;
			if(tekitama[i][2]>4) tekitama[i][2]=4;	if(tekitama[i][2]<-4) tekitama[i][2]=-4;
			tekitama[i][0]+=tekitama[i][2];		tekitama[i][1]+=tekitama[i][3]; break;
		case 24:case 25:case 26://�T�C���g
			if(tekitama[i][7]>=160){tekitama[i][7]=10;if(tekitama[i][6]==1)tekitama[i][6]=0;else tekitama[i][6]=1;} else tekitama[i][7]+=tekitama[i][4]/4;
			if(!tekitama[i][6]){	tss= (tekitama[i][8]+tekitama[i][7]-90);if(tss>=360) tss-=360; if(tss<0) tss+=360; tss*=PI180;
			}else{				tss= (tekitama[i][8]+90-tekitama[i][7]);if(tss>=360) tss-=360; if(tss<0) tss+=360; tss*=PI180;	}
			a=chacha.Getsin(tss);b=chacha.Getcos(tss);	tekitama[i][2]=a*6;	tekitama[i][3]=b*6;
			tekitama[i][0]+=tekitama[i][2];		tekitama[i][1]+=tekitama[i][3]; break;	
		}//switch
		if(tekitama[i][0]<0) { tekitama[i][0]=mapx32+tekitama[i][0];}	if(tekitama[i][0]>=(mapx32)) {tekitama[i][0]=tekitama[i][0]-mapx32;}
		if(tekitama[i][1]<0) {tekitama[i][1]=mapy32+tekitama[i][1]-1;}	if(tekitama[i][1]>=(mapy32)) {tekitama[i][1]=tekitama[i][1]-mapy32;}
		//�L�����Ƃ̓����蔻��
		int hantei=16;
		switch(ftol(tekitama[i][5])){
		case 1:case 2:case 3:case 4:case 17:case 19:case 21:case 22:case 23:case 27:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 43:case 44:case 45:case 46:
			hantei=16;break;
		case 5:case 6:case 7:case 8:
			hantei=32-4*ftol(tekitama[i][7]); break;
		case 9:case 10:case 11:case 12:case 18:case 20:case 24:case 25:case 26:
			hantei=24;break;
		case 13:case 14:case 15:case 16:
			hantei=0;if(tekitama[i][4]==1) hantei=32;
		}
		if(hantei){
			if(xx-hantei/2 < tekitama[i][0] && xx+hantei/2 > tekitama[i][0] && yy-hantei/2 < tekitama[i][1] && yy+hantei/2 > tekitama[i][1] && (damenow==FALSE)&&(gameover==0)&&(mistartf==0)&&(muteki==0)){
				damenow=TRUE;switch(ftol(tekitama[i][5])){
				case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 27:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 43:case 44:case 45:case 46:
					dame--;switch(save.nanido){case 4:dame-=1;break;}break;
				case 9:case 10:case 11:case 12:case 24:case 25:case 26:
					dame-=2;switch(save.nanido){case 3:dame-=1;break;case 4:dame-=1;break;}break;
				case 18:case 19:case 20:case 21:case 22:case 23:
					dame-=8;switch(save.nanido){case 3:dame-=1;break;case 4:dame-=2;break;}break;
				case 13:case 14:case 15:case 16:case 17:
					dame-=16;switch(save.nanido){case 3:dame-=2;break;case 4:dame-=4;break;}break;
				}
				damecnt=damecnt2=0;chacha_a.Play(16);
				mot[motsu][0]=ftol(xx);		mot[motsu][1]=ftol(yy);	mot[motsu][2]=0;	mot[motsu][3]=-1;
				mot[motsu][4]=ftol(xxs);	mot[motsu][5]=ftol(yys);mot[motsu][6]=0;
				motsu++;//��΂���[�[�B���[�[��[�[�[�B
				switch(ftol(tekitama[i][5])){
					case 18:case 20://����3x3
						ii=abs(ftol(xx-tekitama[i][0]));	jj=abs(ftol(yy-tekitama[i][1]));
						tss=chacha.GetShita(tekitama[i][0],xx,tekitama[i][1],yy); ;tss= tss*PI180;
						a=chacha.Getsin(tss)*((ii/10)+1);	b=chacha.Getcos(tss)*((ii/10)+1); 
						xxs-=a;	yys-=b;{
						for(int ti=0;ti<tamasu;ti++){//�@���̔����ɒe�������������H
							if((tama[ti][0]-16-32)<tekitama[i][0] &&(tama[ti][0]+16+32)>tekitama[i][0] &&(tama[ti][1]-16-32)<tekitama[i][1] &&(tama[ti][1]+16+32)>tekitama[i][1]){
								Copy_tama(ti);
						}}
						}
						for(p=0;p<8;p++){
							mot[motsu][0]=ftol(tekitama[i][0])+rand()%64-32;			mot[motsu][1]=ftol(tekitama[i][1])+rand()%64-32;
							mot[motsu][2]=3;			mot[motsu][3]=-1; mot[motsu][6]=p*3;
							mot[motsu][4]=0;			mot[motsu][5]=0;			motsu++;
						}break;
					case 21:case 22:case 23://����5x5
						ii=abs(ftol(xx-tekitama[i][0]));	jj=abs(ftol(yy-tekitama[i][1]));
						tss=chacha.GetShita(tekitama[i][0],xx,tekitama[i][1],yy); ;tss= tss*PI180;
						a=chacha.Getsin(tss)*((ii/10)+1);	b=chacha.Getcos(tss)*((ii/10)+1); 
						xxs-=a;	yys-=b;{
						for(int ti=0;ti<tamasu;ti++){//�@���̔����ɒe�������������H
							if((tama[ti][0]-16-64)<tekitama[i][0] &&(tama[ti][0]+16+64)>tekitama[i][0] &&(tama[ti][1]-16-64)<tekitama[i][1] &&(tama[ti][1]+16+64)>tekitama[i][1]){
								Copy_tama(ti);
							}
						}}
						for(p=0;p<16;p++){
							mot[motsu][0]=ftol(tekitama[i][0])+rand()%96-32;			mot[motsu][1]=ftol(tekitama[i][1])+rand()%96-32;
							mot[motsu][2]=3;			mot[motsu][3]=-1; mot[motsu][6]=p*3;
							mot[motsu][4]=0;			mot[motsu][5]=0;			motsu++;
						}break;
					default:
						xxs+=tekitama[i][2]/2;	yys+=tekitama[i][3]/2;
				}
				Copy_tekitama(i);
				continue;
			}
		}
		//�Ǔ����蔻��
		int h,at=0;float x1,y1;
		for(int hh=0;hh<3;hh++){at=0;
			if(hh==0){ii=ftol(tekitama[i][0])/32;jj=ftol(tekitama[i][1]+tekitama[i][3]*4)/32;	}//����
			if(hh==1){ii=ftol(tekitama[i][0]+tekitama[i][2]*4)/32;	jj=ftol(tekitama[i][1])/32;		}//�悱
			if(hh==2){ii=ftol(tekitama[i][0]+tekitama[i][2]*4)/32;	jj=ftol(tekitama[i][1]+tekitama[i][3]*4)/32;	}//�ȂȂ�
			if(ii<0) {ii=ds.mapx+ii;}	if(ii>=(ds.mapx)) {ii=ii-ds.mapx;}
			if(jj<0) {jj=ds.mapy+jj;}	if(jj>=(ds.mapy)) {jj=jj-ds.mapy;}
			switch(ds.mapdata[ii][jj]){
			case  0:case  4:case  7:case 52:case 55:case 64:case 75:case 76:case 93:case 94:case  8:case 59:case 11:case 56:case 12:case 63:case 15:case 60:	case 78:case 92:case 128:case 129:case 130:case 144:case 145:case 146:break;
			default:if(hh==2) at=1;
				switch(ftol(tekitama[i][5])){	case 5:case 6:case 7:case 8: at=1; break;	}
			}if(at){
				switch(ftol(tekitama[i][5])){
				case 1:case 2:case 3:case 4:case 19:case 27:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 43:case 44:case 45:case 46://����
						mot[motsu][0]=ftol(tekitama[i][0]);			mot[motsu][1]=ftol(tekitama[i][1]);		mot[motsu][2]=2;			mot[motsu][3]=-1;
						mot[motsu][4]=0;			mot[motsu][5]=0;	mot[motsu][6]=0;		motsu++;
						Copy_tekitama(i);break;
				case 5:case 6:case 7:case 8://����
					if(tekitama[i][7]!=3){tekitama[i][7]++;	at=0;x1=tekitama[i][2];y1=tekitama[i][3];for(h=0;h<3;h++){
							if(h==0){ii=ftol(tekitama[i][0])/32;jj=ftol(tekitama[i][1]+tekitama[i][3]*4)/32;	}//����
							if(h==1){ii=ftol(tekitama[i][0]+tekitama[i][2]*4)/32;	jj=ftol(tekitama[i][1])/32;		}//�悱
							if(h==2){ii=ftol(tekitama[i][0]+tekitama[i][2]*4)/32;	jj=ftol(tekitama[i][1]+tekitama[i][3]*4)/32;	}//�ȂȂ�
							if(ii<0) ii=ds.mapx+ii;			if(jj<0) jj=ds.mapy+jj;
							if(ii>=ds.mapx) ii=ii-ds.mapx;	if(jj>=ds.mapy) jj=jj-ds.mapy;
							p=ds.mapdata[ii][jj];	switch(p){
							case  0:case  4:case  7:case 52:case 55:case 64:case 75:case 76:case 93:case 94:case  8:case 59:case 11:case 56:case 12:case 63:case 15:case 60:	case 78:case 92:case 128:case 129:case 130:case 144:case 145:case 146:break;
							case  3:case  6:case 23:case 33:case 36:case 53:case 67:case 69:case 83:case 85:case 137:case 167:	case 96:case  99:case 115:case 101:case 117:// ���΁^
								if(at)break;xy=tekitama[i][2]; tekitama[i][2]=tekitama[i][3]+0.5f*(tekitama[i][3]>0?1:-1); tekitama[i][3]=xy+0.5f*(tekitama[i][3]>0?1:-1);break;
							case  1:case  5:case 20:case 35:case 39:case 54:case 68:case 70:case 84:case 86:case 135:case 169:	case 97:case 100:case 116:case 102:case 108://�^
								if(at)break;xy=tekitama[i][2]; tekitama[i][2]=-tekitama[i][3]-0.5f*(tekitama[i][3]>0?1:-1); tekitama[i][3]=-xy-0.5f*(tekitama[i][3]>0?1:-1);break;
							case  9:case 24:case 25:case 42:case 43:case 58://���^�^
								if(at)break;if(fabs(tekitama[i][2])>fabs(tekitama[i][3])){xy=tekitama[i][2]; tekitama[i][2]=-tekitama[i][3]+xy/16-0.5f*(tekitama[i][3]>0?1:-1); tekitama[i][3]=-xy-0.5f*(tekitama[i][3]>0?1:-1);}else{xy=tekitama[i][2]; tekitama[i][2]=-tekitama[i][3]-0.5f*(tekitama[i][3]>0?1:-1); tekitama[i][3]=-xy-(tekitama[i][3])/16-0.5f*(tekitama[i][3]>0?1:-1);} break;
							case 10:case 26:case 27:case 40:case 41:case 57://��锽�΁^�^
								if(at)break;if(fabs(tekitama[i][2])>fabs(tekitama[i][3])){	xy=tekitama[i][2]; tekitama[i][2]=tekitama[i][3]-xy/16+0.5f*(tekitama[i][3]>0?1:-1); tekitama[i][3]=xy-0.5f*(tekitama[i][3]>0?1:-1);}else{xy=tekitama[i][2]; tekitama[i][2]=tekitama[i][3]+0.5f*(tekitama[i][3]>0?1:-1); tekitama[i][3]=xy-(tekitama[i][3])/16-0.5f*(tekitama[i][3]>0?1:-1);} break;
							case 13:case 28:case 29:case 46:case 47:case 62://���^
								if(at)break;if(fabs(tekitama[i][2])>fabs(tekitama[i][3])){xy=tekitama[i][2]; tekitama[i][2]=-tekitama[i][3]-xy/16-0.5f*(tekitama[i][3]>0?1:-1); tekitama[i][3]=-xy-0.5f*(tekitama[i][3]>0?1:-1);}else{xy=tekitama[i][2]; tekitama[i][2]=-tekitama[i][3]-0.5f*(tekitama[i][3]>0?1:-1); tekitama[i][3]=-xy+(tekitama[i][3])/16-0.5f*(tekitama[i][3]>0?1:-1);} break;
							case 14:case 30:case 31:case 44:case 45:case 61://��锽�΁^�^
								if(at)break;if(fabs(tekitama[i][2])>fabs(tekitama[i][3])){xy=tekitama[i][2]; tekitama[i][2]=tekitama[i][3]+xy/16; tekitama[i][3]=xy-0.5f*(tekitama[i][3]>0?1:-1);}else{xy=tekitama[i][2]; tekitama[i][2]=tekitama[i][3]; tekitama[i][3]=xy+(tekitama[i][3])/16-0.5f*(tekitama[i][3]>0?1:-1);} break;
							default:
								if(h==0)tekitama[i][3]=-tekitama[i][3];
								if(h==1)tekitama[i][2]=-tekitama[i][2];
								if(h==2&&at==0){if(tekitama[i][3]>0.15 || tekitama[i][3]<-0.15)tekitama[i][3]=-tekitama[i][3];	if(tekitama[i][2]>0.15 || tekitama[i][2]<-0.15)tekitama[i][2]=-tekitama[i][2];}
								at=1;break;
							}//switch
						}//for
						break;
					}
				case 9:case 10:case 11:case 12:case 17:case 24:case 25:case 26://����
					Copy_tekitama(i);break;
				case 13:case 14:case 15:case 16://�ђ�
					break;
				case 18:case 20://����3x3
					for(p=0;p<8;p++){
						mot[motsu][0]=ftol(tekitama[i][0])+rand()%64-32;			mot[motsu][1]=ftol(tekitama[i][1])+rand()%64-32;
						mot[motsu][2]=3;			mot[motsu][3]=-1; mot[motsu][6]=p*3;
						mot[motsu][4]=0;			mot[motsu][5]=0;			motsu++;
					}
					if(xx-48 < tekitama[i][0] && xx+48 > tekitama[i][0] && yy-48 < tekitama[i][1] && yy+48 > tekitama[i][1] && (damenow==FALSE)&&(gameover==0)&&(mistartf==0)&&(muteki==0)){
						damenow=TRUE; dame--;switch(save.nanido){case 3:dame--;break;case 4:dame-=2;break;} damecnt=damecnt2=0;chacha_a.Play(16);
						mot[motsu][0]=ftol(xx);		mot[motsu][1]=ftol(yy);	mot[motsu][2]=0;	mot[motsu][3]=-1;
						mot[motsu][4]=ftol(xxs);	mot[motsu][5]=ftol(yys);mot[motsu][6]=0;
						motsu++;
						ii=abs(ftol(xx-tekitama[i][0]));	jj=abs(ftol(yy-tekitama[i][1]));
						tss=chacha.GetShita(tekitama[i][0],xx,tekitama[i][1],yy); ;tss= tss*PI180;
						a=chacha.Getsin(tss)*((ii/10)+1);	b=chacha.Getcos(tss)*((ii/10)+1); 
						xxs-=a;	yys-=b;
					}
					Copy_tekitama(i);break;
				case 21:case 22:case 23://����5x5
					for(p=0;p<16;p++){
						mot[motsu][0]=ftol(tekitama[i][0])+rand()%96-32;			mot[motsu][1]=ftol(tekitama[i][1])+rand()%96-32;
						mot[motsu][2]=3;			mot[motsu][3]=-1; mot[motsu][6]=p*3;
						mot[motsu][4]=0;			mot[motsu][5]=0;			motsu++;
					}
					if(xx-80 < tekitama[i][0] && xx+80 > tekitama[i][0] && yy-80 < tekitama[i][1] && yy+80 > tekitama[i][1] && (damenow==FALSE)&&(gameover==0)&&(mistartf==0)&&(muteki==0)){
						damenow=TRUE; dame--;switch(save.nanido){case 3:dame--;break;case 4:dame-=2;break;} damecnt=damecnt2=0;chacha_a.Play(16);
						mot[motsu][0]=ftol(xx);		mot[motsu][1]=ftol(yy);	mot[motsu][2]=0;	mot[motsu][3]=-1;
						mot[motsu][4]=ftol(xxs);	mot[motsu][5]=ftol(yys);mot[motsu][6]=0;
						motsu++;
						ii=abs(ftol(xx-tekitama[i][0]));	jj=abs(ftol(yy-tekitama[i][1]));
						tss=chacha.GetShita(tekitama[i][0],xx,tekitama[i][1],yy); ;tss= tss*PI180;
						a=chacha.Getsin(tss)*((ii/10)+1);	b=chacha.Getcos(tss)*((ii/10)+1); 
						xxs-=a;	yys-=b;
					}
					Copy_tekitama(i);break;
				}
				if(at) break;
			}
		}
	}
	//HIT��������
	for(i=0;i<hitcnt;i++){
		hit[i][1]++;if(hit[i][1]>4){hit[i][1]=0;hit[i][0]++;}
		if(hit[i][0]>64){
			for(int j=i;j<hitcnt-1;j++){
				hit[j][0]=hit[j+1][0];
				hit[j][1]=hit[j+1][1];
			}hitcnt--; i--; continue;
		}
	}
	//�_���[�W����
	if(damenow){
		damecnt++;if(damecnt==3){damecnt=0;damecnt2++;if(damecnt2==10) damenow=FALSE; }
	}
	if(dame<0&&gameover==0&&romrom==0){ gameover=1; dame=0; gameovercnt=0;chacha_a.Play(18);
	//            321098765432101234568790123            
	//            123456789012345678901234567
	InitMiStart2("missions failed !!         \n"
		         "pursuit plane was destroyed\n\n"
				 "       --game over--       \n\n"
				 "                  try again",TRUE);
		mot[motsu][0]=0;			mot[motsu][1]=0;		mot[motsu][2]=5;			mot[motsu][3]=-1;
		mot[motsu][4]=0;			mot[motsu][5]=0;	mot[motsu][6]=0;		motsu++;
	}
	if(gameover){ gameovercnt++;dame=0;}
	if(gameovercnt==120){ }

	//�N���A����

	//-----------�G�t�F�N�g����-------------------------------------------------------------------------------------------------------------
	for(i=0;i<motsu;i++){//�G�t�F�N�g����
		if(mot[i][6]>0){ mot[i][6]--; continue;}
		if(mot[i][3]==-1) mot[i][3]=0;
		if(mot[i][2]==4&&mot[i][3]>5){
			for(int j=0;j<tekisu;j++){
				if(mot[i][0]-8<tekidata[j][0]&&mot[i][0]+8>tekidata[j][0]&&mot[i][1]-8<tekidata[j][1]&&mot[i][1]+8>tekidata[j][1]){
					tekidata[j][5]-=0.01f;score++;
					if(tekidata[j][5]<=0){
						mot[motsu][0]=ftol(tekidata[j][0]);		mot[motsu][1]=ftol(tekidata[j][1]);
						mot[motsu][2]=5;					mot[motsu][3]=-1;
						mot[motsu][4]=1;	mot[motsu][5]=16;mot[motsu][6]=0;	motsu++;
						for(int jj=0;jj<tamasu;jj++){//�z�[�~���O�̃^�[�Q�b�g�G�̂��炵
							if(tama[jj][8]>j) tama[jj][8]--;
						}for(jj=0;jj<lockonsu;jj++){
							if(lockon[jj]==j){Copy_lockon(jj);}
						}for(jj=0;jj<lockonsu;jj++){
							if(lockon[jj]>j) lockon[jj]--;
						}
						for(jj=0;j<tekitamasu;jj++){
							if(tekitama[jj][10]==(float)j && tekitama[jj][5]==17 && tekitama[jj][7]!=0){//�������[�U�[
								Copy_tekitama(jj);
							}
						}
						Copy_tekidata(j);chacha_a.Play(17); continue;
					}else{
						mot[motsu][0]=ftol(tekidata[j][0]);			mot[motsu][1]=ftol(tekidata[j][1]);
						mot[motsu][2]=3;			mot[motsu][3]=-1; mot[motsu][6]=0;
						mot[motsu][4]=0;			mot[motsu][5]=0;			motsu++;
					}
				}
			}
		}
		if(mot[i][2]!=5){
			mot[i][0]+=mot[i][4];		mot[i][1]+=mot[i][5];
		}
		if(mot[i][6]==-(2+(mot[i][2]==0||mot[i][2]==5)*4)-(mot[i][2]==5&&mot[i][4]==1)*8){	mot[i][3]++;if(mot[i][2]==5) mot[i][5]+=2; mot[i][6]=0;}
		mot[i][6]--;
		if(mot[i][0]<0            ) {mot[i][0]=mapx32_+mot[i][0];}
		if(mot[i][0]>=(mapx32)) {mot[i][0]=mot[i][0]-mapx32_;}
		if(mot[i][1]<0            ) {mot[i][1]=mapy32_+mot[i][1];}
		if(mot[i][1]>=(mapy32)) {mot[i][1]=mot[i][1]-mapy32_;}
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
	if(tamasu>4000) tamasu=4000;
	if(motsu>4000) motsu=4000;
	if(tekitamasu>4000) tekitamasu=4000;

}

BOOL CMainFrame::GetTekiNo(int teki,DWORD no)//�p�[�c�ԍ��L��
{
	if( tekidata2[teki][no>>5]&(1L<<(no&0x1f)) ) return TRUE;
	return FALSE;
}

BOOL CMainFrame::CheckMap(float x,float y,BOOL f)
{
	if(x<0) {x=ds.mapx+x;}	if(x>=(ds.mapx)) {x=x-ds.mapx;}
	if(y<0) {y=ds.mapy+y;}	if(y>=(ds.mapy)) {y=y-ds.mapy;}
	switch(ds.mapdata[ftol(x)][ftol(y)]){
	case  0:case  4:case  7:case 52:case 55:case 64:case 75:case 76:case 93:case 94:case  8:case 59:case 11:case 56:case 12:case 63:case 15:case 60:case 78:case 92:case 128:case 129:case 130:case 144:case 145:case 146:
		if(f) return FALSE;
	case  3:case  6:case 23:case 33:case 36:case 53:case 67:case 69:case 83:case 85:case 137:case 167:	case 96:case  99:case 115:case 101:case 117:// �_
	case  1:case  5:case 20:case 35:case 39:case 54:case 68:case 70:case 84:case 86:case 135:case 169:	case 97:case 100:case 116:case 102:case 118:// �^
	case  9:/*case 24:*/case 25:case 42:/*case 43:*/case 58://���^�^
	case 10:case 26:/*case 27:case 40:*/case 41:case 57://���_�_
	/*case 13:*/case 28:case 29:case 46:case 47://case 62://���^
	/*case 14:*/case 30:case 31:case 44:case 45://case 61://���_
		if(f) return TRUE; else return FALSE;
	}
	return TRUE;
}

BOOL CMainFrame::CheckMap_(float x,float y,BOOL f)
{
	if(x<0) {x=ds.mapx+x;}	if(x>=(ds.mapx)) {x=x-ds.mapx;}
	if(y<0) {y=ds.mapy+y;}	if(y>=(ds.mapy)) {y=y-ds.mapy;}
	switch(ds.mapdata[ftol(x)][ftol(y)]){
	case  0:case  4:case  7:case 52:case 55:case 64:case 75:case 76:case 93:case 94:case  8:case 59:case 11:case 56:case 12:case 63:case 15:case 60:case 78:case 92:case 128:case 129:case 130:case 144:case 145:case 146:
		if(f) return FALSE;
	case  3:case  6:case 23:case 33:case 36:case 53:case 67:case 69:case 83:case 85:case 137:case 167:	case 96:case  99:case 115:case 101:case 117:// �_
	case  1:case  5:case 20:case 35:case 39:case 54:case 68:case 70:case 84:case 86:case 135:case 169:	case 97:case 100:case 116:case 102:case 118:// �^
	case  9:case 24:case 25:case 42:case 43:case 58://���^�^
	case 10:case 26:case 27:case 40:case 41:case 57://���_�_
	case 13:case 28:case 29:case 46:case 47:case 62://���^
	case 14:case 30:case 31:case 44:case 45:case 61://���_
		if(f) return TRUE; else return FALSE;
	}
	return TRUE;
}

int CMainFrame::CheckMap2(float x,float y)
{
	if(x<0) {x=ds.mapx+x;}	if(x>=(ds.mapx)) {x=x-ds.mapx;}
	if(y<0) {y=ds.mapy+y;}	if(y>=(ds.mapy)) {y=y-ds.mapy;}
	switch(ds.mapdata[ftol(x)][ftol(y)]){
	case  3:case  6:case 23:case 33:case 36:case 53:case 67:case 69:case 83:case 85:case 137:case 167:	case 96:case  99:case 115:case 101:case 117:// �_
		return 1;
	case  1:case  5:case 20:case 35:case 39:case 54:case 68:case 70:case 84:case 86:case 135:case 169:	case 97:case 100:case 116:case 102:case 108:case 118:// �^
		return 2;
	case  9:/*case 24:*/case 25:case 42:/*case 43:*/case 58://���^�^
		return 2;
	case 10:case 26:/*case 27:case 40:*/case 41:case 57://���_�_
		return 1;
	/*case 13:*/case 28:case 29:case 46:case 47://case 62://���^
		return 2;
	/*case 14:*/case 30:case 31:case 44:case 45://case 61://���_
		return 1;
	}
		return 0;
}

BOOL CMainFrame::tekikill(int& ti)
{
	if(tekidata[ti][34]==0){
		score+=10*(ftol(tekidata[ti][6])==3?2:1);
		if(tekidata[ti][5]<=0){
			mot[motsu][0]=ftol(tekidata[ti][0]);		mot[motsu][1]=ftol(tekidata[ti][1]);
			mot[motsu][2]=5;					mot[motsu][3]=-1;
			mot[motsu][4]=1;	mot[motsu][5]=16;mot[motsu][6]=0;	motsu++;
			for(int j=0;j<tamasu;j++)//�z�[�~���O�̃^�[�Q�b�g�G�̂��炵
				if(tama[j][8]>ti) tama[j][8]--;
			for(j=0;j<lockonsu;j++){
				if(lockon[j]==ti)Copy_lockon(j);
			}
			for(j=0;j<lockonsu;j++){
				if(lockon[j]>ti) lockon[j]--;
			}
			for(j=0;j<tekitamasu;j++){
				if(ftol(tekitama[j][10])==ti){
					if(ftol(tekitama[j][5])==17){//�������[�U�[
						if(ftol(tekitama[j][7])!=0){//���R��
							Copy_tekitama(j);
						}
					}
				}
			}
			killsec+=10;
			score+=ftol(tekidata[ti][37])*10*(ftol(tekidata[ti][6])==3?2:1);
			switch(ftol(tekidata[ti][6])){
			case 0:	break;
			case 1: Tekihaiti(ftol(tekidata[ti][7]),tekisu);tekidata[tekisu][39]=0;tekidata[tekisu][0]=tekidata[ti][0];tekidata[tekisu][1]=tekidata[ti][1];tekisu++;break;
			case 2: Tekihaiti(ftol(tekidata[ti][7]),tekisu);tekidata[tekisu][39]=0;tekisu++;break;
			case 3:	missioncnt--;killsec+=20;break;
			case 4: penaltycnt++;break;
			case 9: dame=0;break;
			default:Tekihaiti(ftol(tekidata[ti][7]),tekisu);tekidata[tekisu][39]=0;tekidata[tekisu][35]=(tekidata[ti][6]-10)*30;tekisu++;break;
			}float ii4,jj4;
			ii4=tekidata[ti][0]-xx;jj4=tekidata[ti][1]-yy;
			if(gamenx2 > xx)if(mapx32+(xx-(gamenx2))<tekidata[ti][0])ii4=(tekidata[ti][0]-(mapx32+(xx-gamenx2)))-gamenx2;
			if(gameny2 > yy)if(mapy32+(yy-(gameny2))<tekidata[ti][1])jj4=(tekidata[ti][1]-(mapy32+(yy-gameny2)))-gameny2;
			if(mapx32-gamenx2 < xx)if(gamenx2-(mapx32-xx) > tekidata[ti][0])	ii4=(mapx32-xx)+tekidata[ti][0];
			if(mapy32-gameny2 < yy)if(gameny2-(mapy32-yy) > tekidata[ti][1])	jj4=(mapy32-yy)+tekidata[ti][1];
			if(ii4+gamenx2>0 && ii4+gamenx2<gamenx && jj4+gameny2>0 && jj4+gameny2<gameny)
				if(tekidata[ti][6]==3)chacha_a.Play(34,ftol(ii4*10)); else chacha_a.Play(17,ftol(ii4*10));
			Copy_tekidata(ti);
		}else{
			mot[motsu][0]=ftol(tekidata[ti][0]);			mot[motsu][1]=ftol(tekidata[ti][1]);
			mot[motsu][2]=3;			mot[motsu][3]=-1; mot[motsu][6]=0;
			mot[motsu][4]=0;			mot[motsu][5]=0;			motsu++;
		}
		return TRUE;
	}
	return FALSE;
}

void CMainFrame::Copy_lockon(int &j)
{
	for(int k=j;k<lockonsu-1;k++){
		lockon[k]=lockon[k+1];
//						memcpy(&lockon[k],&lockon[k+1],sizeof(int));
	}lockonsu--;
}

void CMainFrame::Copy_tama(int &i)
{
	for(int j=i;j<tamasu-1;j++){
					tama[j][0]=tama[j+1][0];
					tama[j][1]=tama[j+1][1];
					tama[j][2]=tama[j+1][2];
					tama[j][3]=tama[j+1][3];
					tama[j][4]=tama[j+1][4];
					tama[j][5]=tama[j+1][5];
					tama[j][6]=tama[j+1][6];
					tama[j][7]=tama[j+1][7];
					tama[j][8]=tama[j+1][8];
					tama[j][9]=tama[j+1][9];
	//				memcpy(tama[j],tama[j+1],sizeof(float)*10);
				}	tamasu--;i--; 
}

void CMainFrame::Copy_tekitama(int &i)
{
	for(int j=i;j<tekitamasu-1;j++){
		tekitama[j][0]=tekitama[j+1][0];
		tekitama[j][1]=tekitama[j+1][1];
		tekitama[j][2]=tekitama[j+1][2];
		tekitama[j][3]=tekitama[j+1][3];
		tekitama[j][4]=tekitama[j+1][4];
		tekitama[j][5]=tekitama[j+1][5];
		tekitama[j][6]=tekitama[j+1][6];
		tekitama[j][7]=tekitama[j+1][7];
		tekitama[j][8]=tekitama[j+1][8];
		tekitama[j][9]=tekitama[j+1][9];
		tekitama[j][10]=tekitama[j+1][10];
//		memcpy(tekitama[j],tekitama[j+1],sizeof(float)*11);
	}
	tekitamasu--;i--;	
}
void CMainFrame::Copy_tekidata(int &ti)
{
			for(int j=ti;j<tekisu-1;j++){//�G�f�[�^����
				tekidata[j][0]=tekidata[j+1][0];
				tekidata[j][1]=tekidata[j+1][1];
				tekidata[j][2]=tekidata[j+1][2];
				tekidata[j][3]=tekidata[j+1][3];
				tekidata[j][4]=tekidata[j+1][4];
				tekidata[j][5]=tekidata[j+1][5];
				tekidata[j][6]=tekidata[j+1][6];
				tekidata[j][7]=tekidata[j+1][7];
				tekidata[j][8]=tekidata[j+1][8];
				tekidata[j][9]=tekidata[j+1][9];
				tekidata[j][10]=tekidata[j+1][10];
				tekidata[j][11]=tekidata[j+1][11];
				tekidata[j][12]=tekidata[j+1][12];
				tekidata[j][13]=tekidata[j+1][13];
				tekidata[j][14]=tekidata[j+1][14];
				tekidata[j][15]=tekidata[j+1][15];
				tekidata[j][16]=tekidata[j+1][16];
				tekidata[j][17]=tekidata[j+1][17];
				tekidata[j][18]=tekidata[j+1][18];
				tekidata[j][19]=tekidata[j+1][19];
				tekidata[j][20]=tekidata[j+1][20];
				tekidata[j][21]=tekidata[j+1][21];
				tekidata[j][22]=tekidata[j+1][22];
				tekidata[j][23]=tekidata[j+1][23];
				tekidata[j][24]=tekidata[j+1][24];
				tekidata[j][25]=tekidata[j+1][25];
				tekidata[j][26]=tekidata[j+1][26];
				tekidata[j][27]=tekidata[j+1][27];
				tekidata[j][28]=tekidata[j+1][28];
				tekidata[j][29]=tekidata[j+1][29];
				tekidata[j][30]=tekidata[j+1][30];
				tekidata[j][31]=tekidata[j+1][31];
				tekidata[j][32]=tekidata[j+1][32];
				tekidata[j][33]=tekidata[j+1][33];
				tekidata[j][34]=tekidata[j+1][34];
				tekidata[j][35]=tekidata[j+1][35];
				tekidata[j][36]=tekidata[j+1][36];
				tekidata[j][37]=tekidata[j+1][37];
				tekidata[j][38]=tekidata[j+1][38];
				tekidata[j][39]=tekidata[j+1][39];
				tekidata2[j][0]=tekidata2[j+1][0];
				tekidata2[j][1]=tekidata2[j+1][1];
				tekidata2[j][2]=tekidata2[j+1][2];
				tekidata2[j][3]=tekidata2[j+1][3];
//				memcpy(tekidata[j],tekidata[j+1],sizeof(float)*40);
//				memcpy(tekidata2[j],tekidata2[j+1],sizeof(DWORD)*4);
			}
			tekisu--;ti--;
}

void CMainFrame::Kasoku(int i,int j,float& x,float& y)
{
	float ii3,ii4,jj3,jj4,tss,mi,mj;
	mi=(float)mapx32;mj=(float)mapx32;
	switch(tekipart[j][13]){//���@�ǔ�/���x/����/�ō���	
	case 0://�O�F���@��ǔ����Ȃ��B
		if(tekipart[j][18]==0){
			x=(float)tekipart[j][17]*x;
			y=(float)tekipart[j][17]*y;
		}else{
			x+=x*tekipart[j][18]*kasoku+tekidata[i][10]/2;//�����ƈړ�
			y+=y*tekipart[j][18]*kasoku+tekidata[i][10]/2;
		}

		if((x<-tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
			x=-(float)tekipart[j][19];
		if((x> tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
			x=(float)tekipart[j][19];
		if((y<-tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
			y=-(float)tekipart[j][19];
		if((y> tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
			y=(float)tekipart[j][19];
		break;
	case 1://�P�F���@��ǔ�����B
		ii4=tekidata[i][0]-xx;jj4=tekidata[i][1]-yy;
		if(gamenx > xx)if(mi+(xx-(gamenx))<tekidata[i][0])ii4=(tekidata[i][0]-(mi+(xx-gamenx)))-gamenx;
		if(gameny > yy)if(mj+(yy-(gameny))<tekidata[i][1])jj4=(tekidata[i][1]-(mj+(yy-gameny)))-gameny;
		if(mi-gamenx < xx)if(gamenx-(mi-xx) > tekidata[i][0])	ii4=(mi-xx)+tekidata[i][0];
		if(mj-gameny < yy)if(gameny-(mj-yy) > tekidata[i][1])	jj4=(mj-yy)+tekidata[i][1];
		ii3=xx+ii4; jj3=yy+jj4;	
		tss=chacha.GetShita(xx,ii3,yy,jj3);tss= tss*PI180;
		//�����Ƒ��x�ƍō����x
		if(tekipart[j][18]==0){
			x=chacha.Getsin(tss)*tekipart[j][17];
			y=chacha.Getcos(tss)*tekipart[j][17];
		}else{
			x+=chacha.Getsin(tss)*tekipart[j][17]*tekipart[j][18]*(kasoku);
			y+=chacha.Getcos(tss)*tekipart[j][17]*tekipart[j][18]*(kasoku);
		}
		if((x<-tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
			x=-(float)tekipart[j][19];
		if((x> tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
			x=(float)tekipart[j][19];
		if((y<-tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
			y=-(float)tekipart[j][19];
		if((y> tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
			y=(float)tekipart[j][19];
		break;
	case 2://�Q�F���@���瓦����B
		tekidata[i][4]=tekidata[i][4]+180;if(tekidata[i][4]<0) tekidata[i][4]+=360; if(tekidata[i][4]>=360)tekidata[i][4]-=360;
		ii4=tekidata[i][0]-xx;jj4=tekidata[i][1]-yy;
		if(gamenx > xx)if(mi+(xx-(gamenx))<tekidata[i][0]+256)ii4=(tekidata[i][0]-(mi+(xx-gamenx)))-gamenx;
		if(gameny > yy)if(mj+(yy-(gameny))<tekidata[i][1]+256)jj4=(tekidata[i][1]-(mj+(yy-gameny)))-gameny;
		if(mi-gamenx < xx)if(gamenx-(mi-xx) > tekidata[i][0]-256)	ii4=(mi-xx)+tekidata[i][0];
		if(mj-gameny < yy)if(gameny-(mj-yy) > tekidata[i][1]-256)	jj4=(mj-yy)+tekidata[i][1];
		ii3=xx+ii4; jj3=yy+jj4;	
		tss=chacha.GetShita(ii3,xx,jj3,yy);tss= tss*PI180;
		//�����Ƒ��x�ƍō����x
		if(tekipart[j][18]==0){
			x=chacha.Getsin(tss)*tekipart[j][17];
			y=chacha.Getcos(tss)*tekipart[j][17];
		}else{
			x+=chacha.Getsin(tss)*tekipart[j][17]*tekipart[j][18]*(kasoku);
			y+=chacha.Getcos(tss)*tekipart[j][17]*tekipart[j][18]*(kasoku);
		}
		if((x<-tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
			x=-(float)tekipart[j][19]*0.9f;
		if((x> tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
			x=(float)tekipart[j][19]*0.9f;
		if((y<-tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
			y=-(float)tekipart[j][19]*0.9f;
		if((y> tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
			y=(float)tekipart[j][19]*0.9f;
		break;
	case 3://�R�F���@�����苗����ۂB�i�����W�O�Ȃ�ǔ��A�����W���Ȃ瓦����j15�L������
		ii4=tekidata[i][0]-xx;jj4=tekidata[i][1]-yy;
		if(gamenx > xx)if(mi+(xx-(gamenx))<tekidata[i][0])ii4=(tekidata[i][0]-(mi+(xx-gamenx)))-gamenx;
		if(gameny > yy)if(mj+(yy-(gameny))<tekidata[i][1])jj4=(tekidata[i][1]-(mj+(yy-gameny)))-gameny;
		if(mi-gamenx < xx)if(gamenx-(mi-xx) > tekidata[i][0])	ii4=(mi-xx)+tekidata[i][0];
		if(mj-gameny < yy)if(gameny-(mj-yy) > tekidata[i][1])	jj4=(mj-yy)+tekidata[i][1];
		ii3=xx+ii4; jj3=yy+jj4;	
		if(chacha.sqrt_fast((ii3-xx)*(ii3-xx)+(jj3-yy)*(jj3-yy))>15*16){//�����Z�o
			tss=chacha.GetShita(xx,ii3,yy,jj3);tss= tss*PI180;
			//�����Ƒ��x�ƍō����x
			if(tekipart[j][18]==0){
				x=chacha.Getsin(tss)*tekipart[j][17];
				y=chacha.Getcos(tss)*tekipart[j][17];
			}else{
				x+=chacha.Getsin(tss)*tekipart[j][17]*tekipart[j][18]*(kasoku);
				y+=chacha.Getcos(tss)*tekipart[j][17]*tekipart[j][18]*(kasoku);
			}
			if((x<-tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
				x=-(float)tekipart[j][19];
			if((x> tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
				x=(float)tekipart[j][19];
			if((y<-tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
				y=-(float)tekipart[j][19];
			if((y> tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
				y=(float)tekipart[j][19];
		}else{
//			tekidata[i][4]=tekidata[i][4]+180;if(tekidata[i][4]<0) tekidata[i][4]+=360; if(tekidata[i][4]>=360)tekidata[i][4]-=360;
			tss=chacha.GetShita(ii3,xx,jj3,yy);tss= tss*PI180;
			//�����Ƒ��x�ƍō����x
			if(tekipart[j][18]==0){
				x=chacha.Getsin(tss)*tekipart[j][17];
				y=chacha.Getcos(tss)*tekipart[j][17];
			}else{
				x+=chacha.Getsin(tss)*tekipart[j][17]*tekipart[j][18]*(kasoku);
				y+=chacha.Getcos(tss)*tekipart[j][17]*tekipart[j][18]*(kasoku);
			}
			if((x<-tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
				x=-(float)tekipart[j][19];
			if((x> tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
				x=(float)tekipart[j][19];
			if((y<-tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
				y=-(float)tekipart[j][19];
			if((y> tekipart[j][19]) && tekipart[j][19]!=0)//�ō����`�F�b�N
				y=(float)tekipart[j][19];
		}
	}
}

void CMainFrame::Tekitama(int no,float xx1,float yy1,float tss,int t)
{
	float xt,yt,tssb=tss,a,b,ii4,jj4;
	int ii,eff=0;
		tekitama[tekitamasu][8]=tss;tekitama[tekitamasu][10]=(float)t;
		tss= tss*PI180;
		a=chacha.Getsin(tss);	b=chacha.Getcos(tss); tekitama[tekitamasu][9]=255;
		xt=xx1+a*10; yt=yy1+b*10;
		ii4=xx1-xx;jj4=yy1-yy;
		if(gamenx2 > xx)if(mapx32+(xx-(gamenx2))<xx1)ii4=(xx1-(mapx32+(xx-gamenx2)))-gamenx2;
		if(gameny2 > yy)if(mapy32+(yy-(gameny2))<yy1)jj4=(yy1-(mapy32+(yy-gameny2)))-gameny2;
		if(mapx32-gamenx2 < xx)if(gamenx2-(mapx32-xx) > xx1)	ii4=(mapx32-xx)+xx1;
		if(mapy32-gameny2 < yy)if(gameny2-(mapy32-yy) > yy1)	jj4=(mapy32-yy)+yy1;
		if(gamenx2+ii4>-32 && gamenx2+ii4<gamenx+32 && gameny2+jj4>-32 && gameny2+jj4<gameny+32){eff=1;}
		switch(no){
		case 1:case 2:case 3:case 4:if(eff)chacha_a.Play(20,ftol(ii4*10));
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;
			tekitama[tekitamasu][2]=a*no*2;	tekitama[tekitamasu][3]=b*no*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++; break;
		case 5:case 6:case 7:case 8:if(eff)chacha_a.Play(21,ftol(ii4*10));
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;
			tekitama[tekitamasu][2]=a*(no-4)*2;	tekitama[tekitamasu][3]=b*(no-4)*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++; break;
		case 9:case 10:case 11:case 12:if(eff)chacha_a.Play(22,ftol(ii4*10));
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;
			tekitama[tekitamasu][2]=a*(no-8)*2;	tekitama[tekitamasu][3]=b*(no-8)*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++; break;
		case 13:case 14:case 15:case 16:
			{for(ii=0;ii<(no-12)*4;ii++){
				tekitama[tekitamasu][0]=xt+a*32*(float)ii;	tekitama[tekitamasu][1]=yt+b*32*(float)ii;tekitama[tekitamasu][10]=(float)t;
				tekitama[tekitamasu][2]=0;				tekitama[tekitamasu][3]=0; tekitama[tekitamasu][8]=tssb;
				tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;tekitama[tekitamasu][9]=255;
				tekitama[tekitamasu][6]=(ii==0);			tekitama[tekitamasu][7]=0; tekitamasu++; } break;}
		case 17:if(eff)chacha_a.Play(24,ftol(ii4*10));
			{for(ii=0;ii<8;ii++){
				tekitama[tekitamasu][0]=xt+a*32*(float)ii;	tekitama[tekitamasu][1]=yt+b*32*(float)ii;tekitama[tekitamasu][10]=(float)t;
				tekitama[tekitamasu][2]=a*4;				tekitama[tekitamasu][3]=a*4; tekitama[tekitamasu][8]=tssb;
				tekitama[tekitamasu][4]=(float)0;	tekitama[tekitamasu][5]=(float)no;tekitama[tekitamasu][9]=255;
				tekitama[tekitamasu][6]=(float)ii*8;	tekitama[tekitamasu][7]=(8-(float)ii)*8; tekitamasu++; } break;}
		case 18:if(eff)chacha_a.Play(25,ftol(ii4*10));
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;
			tekitama[tekitamasu][2]=a;				tekitama[tekitamasu][3]=b; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++; break;
		case 19:case 20:if(eff)chacha_a.Play(25,ftol(ii4*10));
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;
			tekitama[tekitamasu][2]=a*2;			tekitama[tekitamasu][3]=b*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=(float)no-18;	tekitama[tekitamasu][7]=tssb; tekitamasu++; break;
		case 21:if(eff)chacha_a.Play(26,ftol(ii4*10));
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;
			tekitama[tekitamasu][2]=a*3;			tekitama[tekitamasu][3]=b*3; tekitama[tekitamasu][8]=0;
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++; break;
		case 22:if(eff)chacha_a.Play(26,ftol(ii4*10));
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;
			tekitama[tekitamasu][2]=a*3;			tekitama[tekitamasu][3]=b*3; tekitama[tekitamasu][8]=0;
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++; break;
		case 23:if(eff)chacha_a.Play(26,ftol(ii4*10));
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;
			tekitama[tekitamasu][2]=0;				tekitama[tekitamasu][3]=0.25f; tekitama[tekitamasu][8]=0;
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++; break;
		case 24:case 25:case 26:if(eff)chacha_a.Play(27,ftol(ii4*10));
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;
			tekitama[tekitamasu][2]=a*3;			tekitama[tekitamasu][3]=b*3;tss=tekitama[tekitamasu][8];
			tekitama[tekitamasu][4]=18/(float)(no-23);	tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=10; tekitamasu++; 
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;tekitama[tekitamasu][9]=255;tekitama[tekitamasu][10]=(float)t;
			tekitama[tekitamasu][2]=a*3;			tekitama[tekitamasu][3]=b*3;tekitama[tekitamasu][8]=tss;
			tekitama[tekitamasu][4]=18/(float)(no-23);	tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=1;				tekitama[tekitamasu][7]=10; tekitamasu++; break;
		case 27:case 28:case 29:case 30:if(eff)chacha_a.Play(20,ftol(ii4*10));
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;
			tekitama[tekitamasu][2]=a*(no-26)*2;	tekitama[tekitamasu][3]=b*(no-26)*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++;
			tss=tssb+10; if(tss>=360) tss-=360;tekitama[tekitamasu][8]=tss; tss= tss*PI180;	a=chacha.Getsin(tss);	b=chacha.Getcos(tss);		
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;tekitama[tekitamasu][9]=255;tekitama[tekitamasu][10]=(float)t;
			tekitama[tekitamasu][2]=a*(no-26)*2;	tekitama[tekitamasu][3]=b*(no-26)*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++;
			tss=tssb-10; if(tss<0) tss+=360; tekitama[tekitamasu][8]=tss;tss= tss*PI180;	a=chacha.Getsin(tss);	b=chacha.Getcos(tss);		
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;tekitama[tekitamasu][9]=255;tekitama[tekitamasu][10]=(float)t;
			tekitama[tekitamasu][2]=a*(no-26)*2;	tekitama[tekitamasu][3]=b*(no-26)*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++; break;
		case 31:case 32:case 33:case 34:if(eff)chacha_a.Play(20,ftol(ii4*10));
			tss=tssb+5; if(tss>=360) tss-=360;tekitama[tekitamasu][8]=tss; tss= tss*PI180;	a=chacha.Getsin(tss);	b=chacha.Getcos(tss);		
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;
			tekitama[tekitamasu][2]=a*(no-30)*2;	tekitama[tekitamasu][3]=b*(no-30)*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++;
			tss=tssb+15; if(tss>=360) tss-=360;tekitama[tekitamasu][8]=tss; tss= tss*PI180;	a=chacha.Getsin(tss);	b=chacha.Getcos(tss);		
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;tekitama[tekitamasu][9]=255;tekitama[tekitamasu][10]=(float)t;
			tekitama[tekitamasu][2]=a*(no-30)*2;	tekitama[tekitamasu][3]=b*(no-30)*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++;
			tss=tssb-5; if(tss<0) tss+=360;tekitama[tekitamasu][8]=tss; tss= tss*PI180;	a=chacha.Getsin(tss);	b=chacha.Getcos(tss);		
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;tekitama[tekitamasu][9]=255;tekitama[tekitamasu][10]=(float)t;
			tekitama[tekitamasu][2]=a*(no-30)*2;	tekitama[tekitamasu][3]=b*(no-30)*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++;
			tss=tssb-15; if(tss<0) tss+=360; tekitama[tekitamasu][8]=tss;tss= tss*PI180;	a=chacha.Getsin(tss);	b=chacha.Getcos(tss);		
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;tekitama[tekitamasu][9]=255;
			tekitama[tekitamasu][2]=a*(no-30)*2;	tekitama[tekitamasu][3]=b*(no-30)*2; tekitama[tekitamasu][10]=(float)t;
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++; break;
		case 35:case 36:case 37:case 38:if(eff)chacha_a.Play(20,ftol(ii4*10));
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;
			tekitama[tekitamasu][2]=a*(no-34)*2;	tekitama[tekitamasu][3]=b*(no-34)*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++;
			tss=tssb+10; if(tss>=360) tss-=360;tekitama[tekitamasu][8]=tss; tss= tss*PI180;	a=chacha.Getsin(tss);	b=chacha.Getcos(tss);		
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;tekitama[tekitamasu][9]=255;tekitama[tekitamasu][10]=(float)t;
			tekitama[tekitamasu][2]=a*(no-34)*2;	tekitama[tekitamasu][3]=b*(no-34)*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++;
			tss=tssb+20; if(tss>=360) tss-=360;tekitama[tekitamasu][8]=tss; tss= tss*PI180;	a=chacha.Getsin(tss);	b=chacha.Getcos(tss);		
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;tekitama[tekitamasu][9]=255;tekitama[tekitamasu][10]=(float)t;
			tekitama[tekitamasu][2]=a*(no-34)*2;	tekitama[tekitamasu][3]=b*(no-34)*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++;
			tss=tssb-10; if(tss<0) tss+=360;tekitama[tekitamasu][8]=tss; tss= tss*PI180;	a=chacha.Getsin(tss);	b=chacha.Getcos(tss);		
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;tekitama[tekitamasu][9]=255;tekitama[tekitamasu][10]=(float)t;
			tekitama[tekitamasu][2]=a*(no-34)*2;	tekitama[tekitamasu][3]=b*(no-34)*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++;
			tss=tssb-20; if(tss<0) tss+=360; tekitama[tekitamasu][8]=tss;tss= tss*PI180;	a=chacha.Getsin(tss);	b=chacha.Getcos(tss);		
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;tekitama[tekitamasu][9]=255;tekitama[tekitamasu][10]=(float)t;
			tekitama[tekitamasu][2]=a*(no-34)*2;	tekitama[tekitamasu][3]=b*(no-34)*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++; break;
		case 39:case 40:case 41:case 42:if(eff)chacha_a.Play(28,ftol(ii4*10));
			{for(ii=0;ii<8;ii++){
			tss=tssb+ii*45; if(tss>=360) tss-=360;tekitama[tekitamasu][8]=tss; tss= tss*PI180;	a=chacha.Getsin(tss);	b=chacha.Getcos(tss);		
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;tekitama[tekitamasu][9]=255;tekitama[tekitamasu][10]=(float)t;
			tekitama[tekitamasu][2]=a*(no-38)*2;	tekitama[tekitamasu][3]=b*(no-38)*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++;}break;}
		case 43:case 44:case 45:case 46:if(eff)chacha_a.Play(20,ftol(ii4*10));
			tss=tssb+(timer%20)-10; if(tss>=360) tss-=360;if(tss<0) tss+=360;tekitama[tekitamasu][8]=tss; tss= tss*PI180;	a=chacha.Getsin(tss);	b=chacha.Getcos(tss);		
			tekitama[tekitamasu][0]=xt;		tekitama[tekitamasu][1]=yt;
			tekitama[tekitamasu][2]=a*(no-42)*2;	tekitama[tekitamasu][3]=b*(no-42)*2; 
			tekitama[tekitamasu][4]=0;				tekitama[tekitamasu][5]=(float)no;
			tekitama[tekitamasu][6]=0;				tekitama[tekitamasu][7]=0; tekitamasu++;break;
		}
}

void CMainFrame::LoadStage(int& no)
{
	CFile file;
	UnCompress uc;
	CString fn;
rrr:
	if(no==999)
		fn=namef;
	else
		fn.Format("stage%02d.map",no);
	if(file.Open(uc.Open(fn),CFile::shareDenyNone | CFile::modeRead ,NULL)==TRUE)
	{
		file.Read(&ds,sizeof ds);
		file.Close();
		if(no==999)
			fn=namef.Left(namef.GetLength()-3)+"bmp";
		else
			fn.Format("stage%02d.bmp",no);
		chacha.InitSprite(fn);
		xx=(float)ds.startx*32+16;
		yy=(float)ds.starty*32+31;
		atari=FALSE;
	}else{
		if(no==999) no=1;
		no--; stage=no; goto rrr;}
	mapx32=(float)ds.mapx*32;
	mapy32=(float)ds.mapy*32;
	mapx32_=ds.mapx*32;
	mapy32_=ds.mapy*32;
	tekisu=ds.tekikazu;
	//�����ݒ�
	//0:x 1:y 2:xx 3:yy 4:shita 5:hp 6:�j�� 7:�G�ԍ� 8,9,10,11,12,13:�J�E���g�t���O
	//8:��]���x 9:�O��̕� 10:�ړ����� 11:�ړ�����2 12:�W�����v��
	//14,15:�f�B���C 16:�Ԋu 17:�U��shita 18:��shita
	//19:���J�nshita 20;�I��shita 21:���J�nshita 22;�I��shita 23:���J�nshita 24;�I��shita 25:-- 26:���� 
	//27,28,29,30,31:����
	for(int i=0;i<tekisu;i++){
		Tekihaiti(i,i,FALSE);
	}
}

void CMainFrame::Tekihaiti(int tekino,int tekidatano,BOOL yomi)
{
	float ii4,jj4,ii3,jj3;int hh=0,jj=0;
//	float tg[4][2]={{0,-1},{1,0},{0,1},{-1,0}};
	float tg[4][2]={{0,1},{-1,0},{0,-1},{1,0}};
	ZeroMemory(tekidata[tekidatano],sizeof(float)*40);
	tekidata[tekidatano][0]=(float)ds.tekidata[tekino][0]*32+16;
	tekidata[tekidatano][1]=(float)ds.tekidata[tekino][1]*32+16;
	tekidata[tekidatano][9]=2;tekidata[tekidatano][33]=tekidata[tekidatano][4]=180;
	tekidata[tekidatano][2]=0.0f;
	tekidata[tekidatano][3]=0.0f;
	tekidata[tekidatano][6]=(float)(ds.tekidata[tekino][2]&0x7fff);
	if(yomi==0&&tekidata[tekidatano][6]==3) missioncnt++;
	tekidata[tekidatano][39]=(float)((ds.tekidata[tekino][2]&0x8000)?1:0);
	if(yomi)tekidata[tekidatano][39]=0;
	tekidata[tekidatano][7]=(float)ds.tekidata[tekino][3];
	tekidata2[tekidatano][0]=ds.tekidata[tekino][4];
	tekidata2[tekidatano][1]=ds.tekidata[tekino][5];
	tekidata2[tekidatano][2]=ds.tekidata[tekino][6];
	tekidata2[tekidatano][3]=ds.tekidata[tekino][7];
	tekidata[tekidatano][32]=tekidata[tekidatano][5]=30;
	
	for(DWORD j=0;j<128;j++){//�p�[�c������
		if(GetTekiNo(tekidatano,j)){
			tekidata[tekidatano][34]=(tekipart[j][1]>=5)?(float)tekipart[j][1]-4:0;
			switch(tekipart[j][1]){
			case 1:tekidata[tekidatano][37]+=10;tekidata[tekidatano][38]=1;
				switch(tekipart[j][16]){
				case 0:	ii4=tekidata[tekidatano][0]-xx;jj4=tekidata[tekidatano][1]-yy;
						if(gamenx2 > xx)if(mapx32+(xx-(gamenx2))<tekidata[tekidatano][0])ii4=(tekidata[tekidatano][0]-(mapx32+(xx-gamenx2)))-gamenx2;
						if(gameny2 > yy)if(mapy32+(yy-(gameny2))<tekidata[tekidatano][1])jj4=(tekidata[tekidatano][1]-(mapy32+(yy-gameny2)))-gameny2;
						if(mapx32-gamenx2 < xx)if(gamenx2-(mapx32-xx) > tekidata[tekidatano][0])	ii4=(mapx32-xx)+tekidata[tekidatano][0];
						if(mapy32-gameny2 < yy)if(gameny2-(mapy32-yy) > tekidata[tekidatano][1])	jj4=(mapy32-yy)+tekidata[tekidatano][1];
						ii3=xx+ii4; jj3=yy+jj4;	ii4=chacha.GetShita(xx,ii3,yy,jj3)*PI180;
					   tekidata[tekidatano][2]=chacha.Getsin(ii4);tekidata[tekidatano][3]=chacha.Getcos(ii4);break;//���@
				case 1:tekidata[tekidatano][2]=-1; tekidata[tekidatano][3]= 1; break;//����
				case 2:tekidata[tekidatano][2]= 0; tekidata[tekidatano][3]= 1; break;//��
				case 3:tekidata[tekidatano][2]= 1; tekidata[tekidatano][3]= 1; break;//�E��
				case 4:tekidata[tekidatano][2]=-1; tekidata[tekidatano][3]= 0; break;//��
				case 5:tekidata[tekidatano][2]= 0; tekidata[tekidatano][3]= 0; break;//�E��]
				case 6:tekidata[tekidatano][2]= 1; tekidata[tekidatano][3]= 0; break;//�E
				case 7:tekidata[tekidatano][2]=-1; tekidata[tekidatano][3]=-1; break;//����
				case 8:tekidata[tekidatano][2]= 0; tekidata[tekidatano][3]=-1; break;//��
				case 9:tekidata[tekidatano][2]= 1; tekidata[tekidatano][3]=-1; break;//�E��
				case 10:tekidata[tekidatano][2]=0; tekidata[tekidatano][3]= 0;break;//����]
				}
				break;
			case 0:tekidata[tekidatano][37]+=20;tekidata[tekidatano][38]=0;
				for(jj=0;jj<4;jj++){
					if(hh) break;
					if(CheckMap((float)ds.tekidata[tekino][0]+tg[jj][0],(float)ds.tekidata[tekino][1]+tg[jj][1])){
						switch(jj){
						case 2:hh=1;
							tekidata[tekidatano][0]=(float)ds.tekidata[tekino][0]*32+16;tekidata[tekidatano][9]=0;tekidata[tekidatano][33]=tekidata[tekidatano][4]=0;
							tekidata[tekidatano][1]=(float)ds.tekidata[tekino][1]*32;break;
						case 3:hh=1;
							tekidata[tekidatano][0]=(float)ds.tekidata[tekino][0]*32+32-2;tekidata[tekidatano][9]=1;tekidata[tekidatano][33]=tekidata[tekidatano][4]=270;
							tekidata[tekidatano][1]=(float)ds.tekidata[tekino][1]*32+16;break;
						case 0:hh=1;
							tekidata[tekidatano][0]=(float)ds.tekidata[tekino][0]*32+16;tekidata[tekidatano][9]=2;tekidata[tekidatano][33]=tekidata[tekidatano][4]=180;
							tekidata[tekidatano][1]=(float)ds.tekidata[tekino][1]*32+32-2;break;
						case 1:hh=1;
							tekidata[tekidatano][0]=(float)ds.tekidata[tekino][0]*32;tekidata[tekidatano][9]=3;tekidata[tekidatano][33]=tekidata[tekidatano][4]=90;
							tekidata[tekidatano][1]=(float)ds.tekidata[tekino][1]*32+16;break;
						}
					}
				}
				switch(tekipart[j][16]){
				case 0:	ii4=tekidata[tekidatano][0]-xx;jj4=tekidata[tekidatano][1]-yy;
						if(gamenx2 > xx)if(mapx32+(xx-(gamenx2))<tekidata[tekidatano][0])ii4=(tekidata[tekidatano][0]-(mapx32+(xx-gamenx2)))-gamenx2;
						if(gameny2 > yy)if(mapy32+(yy-(gameny2))<tekidata[tekidatano][1])jj4=(tekidata[tekidatano][1]-(mapy32+(yy-gameny2)))-gameny2;
						if(mapx32-gamenx2 < xx)if(gamenx2-(mapx32-xx) > tekidata[tekidatano][0])	ii4=(mapx32-xx)+tekidata[tekidatano][0];
						if(mapy32-gameny2 < yy)if(gameny2-(mapy32-yy) > tekidata[tekidatano][1])	jj4=(mapy32-yy)+tekidata[tekidatano][1];
						ii3=xx+ii4; jj3=yy+jj4;	ii4=chacha.GetShita(xx,ii3,yy,jj3)*PI180;
					   tekidata[tekidatano][2]=chacha.Getsin(ii4);tekidata[tekidatano][3]=chacha.Getcos(ii4);break;//���@
				case 1:tekidata[tekidatano][2]=-1; tekidata[tekidatano][3]= 1; break;//����
				case 2:tekidata[tekidatano][2]= 0; tekidata[tekidatano][3]= 1; break;//��
				case 3:tekidata[tekidatano][2]= 1; tekidata[tekidatano][3]= 1; break;//�E��
				case 4:tekidata[tekidatano][2]=-1; tekidata[tekidatano][3]= 0; break;//��
				case 5:tekidata[tekidatano][2]= 0; tekidata[tekidatano][3]= 0; break;//�E��]
				case 6:tekidata[tekidatano][2]= 1; tekidata[tekidatano][3]= 0; break;//�E
				case 7:tekidata[tekidatano][2]=-1; tekidata[tekidatano][3]=-1; break;//����
				case 8:tekidata[tekidatano][2]= 0; tekidata[tekidatano][3]=-1; break;//��
				case 9:tekidata[tekidatano][2]= 1; tekidata[tekidatano][3]=-1; break;//�E��
				case 10:tekidata[tekidatano][2]=0; tekidata[tekidatano][3]= 0;break;//����]
				}
				switch(tekipart[j][14]){
				case 1:tekidata[tekidatano][10]=1;tekidata[tekidatano][2]= 1; tekidata[tekidatano][3]=1;
					switch(ftol(tekidata[tekidatano][9])){case 0:case 1:tekidata[tekidatano][10]*=-1;tekidata[tekidatano][2]*=-1; tekidata[tekidatano][3]*=-1;break;}
					break;
				case 2:tekidata[tekidatano][10]=-1;tekidata[tekidatano][2]= -1; tekidata[tekidatano][3]=-1;
					switch(ftol(tekidata[tekidatano][9])){case 0:case 1:tekidata[tekidatano][10]*=-1;tekidata[tekidatano][2]*=-1; tekidata[tekidatano][3]*=-1;break;}
					break;
				}break;
			case 3:tekidata[tekidatano][37]+=30;
				break;
			case 4:
				switch(tekipart[j][28]){
				case 0:tekidata[tekidatano][5]=30;break;
				case 1:tekidata[tekidatano][5]=15;break;
				case 2:tekidata[tekidatano][5]=60;break;
				case 3:tekidata[tekidatano][5]=90;break;
				case 4:tekidata[tekidatano][5]=120;break;
				case 5:tekidata[tekidatano][5]=150;break;
				case 6:tekidata[tekidatano][5]=180;break;
				case 7:tekidata[tekidatano][5]=210;break;
				case 8:tekidata[tekidatano][5]=240;break;
				case 9:tekidata[tekidatano][5]=270;break;
				}tekidata[tekidatano][32]=tekidata[tekidatano][5];break;
			}
		}
	}
	if(tekidata[tekidatano][0]==0&&tekidata[tekidatano][1]==0){
		tekidata[tekidatano][0]=(float)ds.tekidata[tekino][0]*32;
		tekidata[tekidatano][1]=(float)ds.tekidata[tekino][1]*32;
	}
}

void CMainFrame::InitMiStart(CString s,BOOL f)
{
	int k=0,kk=0,kkk=0,kkkk=0;
	mistarts=s;
	mistartmode3=f;if(f){clearf=1; bgm="bgm05.ogg";}//chacha_a.WaveDAStart("bgm05.ogg");
	mistartsu=s.GetLength();
	for(int i=0;i<mistartsu;i++){
		mistartc[i]=s.GetAt(i);
		if(mistartc[i]=='A') kkkk^=1;
		if(mistartc[i]=='\n'){k++;if(kkk<kk) kkk=kk;kk=0;}
		kk++;
		mistart[i][0]=512;
		mistart[i][1]=(float)((i*90)%360);
		mistart[i][2]=(float)i;
		mistartc2[i]=kkkk;
	}
	if(kkk==0)kkk=kk;
	mistartx=-((kkk-1)*16)+512+32;
	mistarty=384-(k/2)*32-16;
	mistartf=TRUE;
	mistartmode=0;mistartmode2=0;mistartmodef=0;
}

void CMainFrame::AnimeMiStart()
{
	if(mistartf==0) return;
	int i;
	float msx=(float)mistartx,msy=(float)mistarty,msxx=msx,msyy=msy;
	switch(mistartmode){
	case 0://���邭���]�\��
		for(i=0;i<mistartsu;i++){
			mistart[i][2]--;
			if(mistart[i][2]>0) continue;
			if(mistartc[i]=='A') continue;
			if(mistartc[i]=='\n'){ msx=msxx;msyy+=32;msy=msyy;continue; }
			if(i%2){
				mistart[i][1]+=5;if(mistart[i][1]>=360)mistart[i][1]-=360;
			}else{
				mistart[i][1]-=5;if(mistart[i][1]<0)mistart[i][1]+=360;
			}
			mistart[i][0]-=5;if(mistart[i][0]<0)mistart[i][0]=0;
			mistart[i][3]=chacha.Getsin(mistart[i][1]*PI180)*mistart[i][0]+msx;
			mistart[i][4]=chacha.Getcos(mistart[i][1]*PI180)*mistart[i][0]+msy;
			msx+=32;
		}
		mistartmodef++;
		if(mistart[mistartsu-1][0]==0){ mistartmode=1;mistartmode2=0;}
		break;
	case 1://���̂܂ܑҋ@
		mistartmode2++;mistartmodef++;if(mistartmode2>20){
			if(mistartmode3==TRUE){
				mistartmode=6;mistartmode2=0;mistartmodef=0;
			}else{
				mistartmode=2;mistartmode2=0;
			}
		}
		break;
	case 2://�����ď�����
		mistartmode2++;mistartmodef++;if(mistartmode2>20){ mistartmode=3;}
		break;
	case 3://�����ď�����
		mistartmode2--;mistartmodef++;if(mistartmode2<1){ mistartmode=4;mistartmode2=0;}
		break;
	case 4://���̂܂ܑҋ@
		mistartmode2++;mistartmodef++;if(mistartmode2>20){ mistartmode=5;mistartmode2=0;}
		break;
	case 5://�����ď�����
		mistartmode2++;if(mistartmode2>60){ mistartf=0;}
		break;
	case 6://���̂܂܏I����
		mistartmodef++;if(mistartmode2>260){ mistartf=2;if(demof)mistartfA=2;}else mistartmode2++;
		if(mistartmode2==100&&demof==0){
			CString s;
			b1=0,b2=0,b3=0,b4=0,b5=0,m1=0,m2=0,m3=0;
			if(hitp[0][0]==0){ s1="no use";b1=0; }else{ s1.Format("  A%3d%%A",hitp[0][1]*100/hitp[0][0]);b1=hitp[0][1]*20000/hitp[0][0];}
			if(hitp[1][0]==0){ s2="no use";b2=0; }else{ s2.Format("  A%3d%%A",hitp[1][1]*100/hitp[1][0]);b2=hitp[1][1]*20000/hitp[1][0];}
			if(hitp[2][0]==0){ s3="no use";b3=0; }else{ s3.Format("  A%3d%%A",hitp[2][1]*100/hitp[2][0]);b3=hitp[2][1]*20000/hitp[2][0];}
			if(hitp[3][0]==0){ s4="no use";b4=0; }else{ s4.Format("  A%3d%%A",hitp[3][1]*100/hitp[3][0]);b4=hitp[3][1]*20000/hitp[3][0];}
			if(hitp[4][0]==0){ s5="no use";b5=0; }else{ s5.Format("  A%3d%%A",hitp[4][1]*100/hitp[4][0]);b5=hitp[4][1]*20000/hitp[4][0];}
			if(killsec*60 < timer) m1=0; else	m1=((abs(killsec*60-timer))*30000)/(killsec*60);
			if(m1<0)m1=30000;
			if(hp==10000) m2=20000; else	m2=((hp)*20000)/10000;
			if(dame==_HP) m3=20000; else	m3=((dame)*20000)/_HP;
			//                 1         2         3
			//        12345678901234567890123456789012
			if(chacha.stage==999)
					s.Format("[mission enhanced result]      \n\n\n"
				     "               I        I      \n"
				     "weapon summary Ihit rateIscore\n"
					 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
				     "machine gun    I  %sI%+7d\n"
				     "homing gun     I  %sI%+7d\n"
				     "space mine     I  %sI%+7d\n"
				     "a g missile    I  %sI%+7d\n"
					 "2-way gun      I  %sI%+7d\n"
					 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
					 "mission time   I%02d:%02d.%02dI%+7d\n"
					 "energy bonus   I    A%3d%%AI%+7d\n"
					 "damage bonus   I    A%3d%%AI%+7d\n"
					 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
					 "bonus total    I        I%+7d\n"
					 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
					 "score          I      %10d\n\n ",
					 s1,b1,s2,b2,s3,b3,s4,b4,s5,b5,
					 timer/(60*60),(timer/60)%60,((timer%60)*100)/60,m1,
					 hp*100/10000,m2,dame*100/_HP,m3,b1+b2+b3+b4+b5+m1+m2+m3,score);
			else
			s.Format("[mission A%03dA result]           \n\n\n"
				     "               I        I      \n"
				     "weapon summary Ihit rateIscore\n"
					 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
				     "machine gun    I  %sI%+7d\n"
				     "homing gun     I  %sI%+7d\n"
				     "space mine     I  %sI%+7d\n"
				     "a g missile    I  %sI%+7d\n"
					 "2-way gun      I  %sI%+7d\n"
					 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
					 "mission time   I%02d:%02d.%02dI%+7d\n"
					 "energy bonus   I    A%3d%%AI%+7d\n"
					 "damage bonus   I    A%3d%%AI%+7d\n"
					 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
					 "bonus total    I        I%+7d\n"
					 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
					 "score          I      %10d\n\n ",chacha.stage,
					 s1,b1,s2,b2,s3,b3,s4,b4,s5,b5,
					 timer/(60*60),(timer/60)%60,((timer%60)*100)/60,m1,
					 hp*100/10000,m2,dame*100/_HP,m3,b1+b2+b3+b4+b5+m1+m2+m3,score);
			replay=-1;
			InitMiStart2(s);
		}
		break;
	}

}

void CMainFrame::DrawMiStart()
{
	if(mistartf==0) return;
	char str[2];int i;
	switch(mistartmode){
	case 0://���邭���]�\��
		if(mistartmode3==FALSE){
			chacha.Fade(255-mistartmodef*2);
		}
		for(i=0;i<mistartsu;i++){
			if(mistart[i][2]>0||mistartc[i]=='\n'||mistartc[i]==' '||mistartc[i]=='A') continue;
			str[0]=mistartc[i];
			chacha.Moji32(str,1,mistartc2[i],mistart[i][3],mistart[i][4],TRUE,255);
		}break;
	case 1:
	case 4:
		if(mistartmode3==FALSE){
			chacha.Fade(255-mistartmodef*2);
		}
		chacha.Moji32(mistarts,mistarts.GetLength(),0,(float)mistartx,(float)mistarty,TRUE,255);
		break;
	case 6:
		if(mistartmode3==TRUE){
			chacha.Fade(mistartmodef);
		}
		chacha.Moji32(mistarts,mistarts.GetLength(),0,(float)mistartx,(float)mistarty,TRUE,255-((mistartmode2>100)?(mistartmode2-100)*2:0));
		break;
	case 2:
	case 3:
		if(mistartmode3==FALSE){
			chacha.Fade(255-mistartmodef*2);
		}
		chacha.Moji32(mistarts,mistarts.GetLength(),0,(float)mistartx,(float)mistarty,TRUE,255);
		g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE );
		for(i=0;i<mistartmode2;i++)
			chacha.Moji32(mistarts,mistarts.GetLength(),0,(float)mistartx,(float)mistarty,FALSE,1);
		g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		break;
	case 5:
		chacha.Moji32(mistarts,mistarts.GetLength(),0,(float)mistartx,mistarty-(float)mistartmode2*10,TRUE,255-mistartmode2*7);
		chacha.Moji32(mistarts,mistarts.GetLength(),0,(float)mistartx,mistarty+(float)mistartmode2*10,TRUE,255-mistartmode2*7);
		break;
	}
}

void CMainFrame::InitMiStart2(CString s,BOOL f)
{
	int k=0,kk=0,kkk=0,kkkk=0;
	mistartsA=s;
	mistartmode3A=f;if(f) bgm="bgm02.ogg";
	mistartsuA=s.GetLength();
	for(int i=0;i<mistartsuA;i++){
		mistartcA[i]=s.GetAt(i);
		if(mistartcA[i]=='A') kkkk^=1;
		if(mistartcA[i]=='\n'){k++;if(kkk<kk) kkk=kk;kk=0;}
		kk++;
		mistartA[i][0]=512;
		mistartA[i][1]=(float)((i*30)%360);
		mistartA[i][2]=(float)i/2;
		mistartc2A[i]=kkkk;
	}
	if(kkk==0)kkk=kk;
	mistartxA=-((kkk-1)*16)+512+32;
	mistartyA=384-(k/2)*32-16;
	mistartfA=TRUE;
	mistartmodeA=0;mistartmode2A=0;mistartmodefA=0;
	mistartmode4A=0;
}
CWinThread *th1;
void CMainFrame::AnimeMiStart2()
{
	if(mistartfA==0) return;
	int i;
	float msx=(float)mistartxA,msy=(float)mistartyA,msxx=msx,msyy=msy;
	switch(mistartmodeA){
	case 0://���邭���]�\��
		for(i=0;i<mistartsuA;i++){
			mistartA[i][2]--;
			if(mistartA[i][2]>0) continue;
			if(mistartcA[i]=='A') continue;
			if(mistartcA[i]=='\n'){ msx=msxx;msyy+=32;msy=msyy;continue; }
			if(i%2){
				mistartA[i][1]+=5;if(mistartA[i][1]>=360)mistartA[i][1]-=360;
			}else{
				mistartA[i][1]-=5;if(mistartA[i][1]<0)mistartA[i][1]+=360;
			}
			mistartA[i][0]-=5;if(mistartA[i][0]<0)mistartA[i][0]=0;
			mistartA[i][3]=chacha.Getsin(mistartA[i][1]*PI180)*mistartA[i][0]+msx;
			mistartA[i][4]=chacha.Getcos(mistartA[i][1]*PI180)*mistartA[i][0]+msy;
			msx+=32;
		}
		mistartmodefA++;
		if(mistartA[mistartsuA-1][0]==0){ mistartmodeA=1;mistartmode2A=0;}
		break;
	case 1://���̂܂ܑҋ@
		mistartmode2A++;mistartmodefA++;if(mistartmode2A>20){
			if(mistartmode3A==TRUE){
				mistartmodeA=6;mistartmode2A=0;mistartmodefA=0;
			}else{
				mistartmodeA=2;mistartmode2A=0;if(save.stagetime[stage-1][save.nanido]>timer || save.stagetime[stage-1][save.nanido]==-1){
					chacha_a.Play(4);
					th1=AfxBeginThread(ReplaySave,(LPVOID)this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
					th1->m_bAutoDelete=TRUE;
					th1->ResumeThread();
				}
			}
		}
		break;
	case 2://�����ď�����
		mistartmode2A++;mistartmodefA++;if(mistartmode2A>20){ mistartmodeA=3;}
		mistartmode4A+=2;if(mistartmode4A>255)mistartmode4A=0;
		break;
	case 3://�����ď�����
		mistartmode2A--;mistartmodefA++;if(mistartmode2A<1&&tamaf){ mistartmodeA=4;mistartmode2A=0;/*HosiOkuMake();*/}
		mistartmode4A+=2;if(mistartmode4A>255)mistartmode4A=0;
		tamaf=0;
		if(mistartmode2A<1){
			if(chacha.stage==999)
					mistartsA.Format("[mission enhanced result]      \n\n\n"
					 "               I        I      \n"
					 "weapon summary Ihit rateI score\n"
					 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
					 "machine gun    I  %sI%+7d\n"
					 "homing gun     I  %sI%+7d\n"
					 "space mine     I  %sI%+7d\n"
					 "a g missile    I  %sI%+7d\n"
					 "2-way gun      I  %sI%+7d\n"
					 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
					 "mission time   I%02d:%02d.%02dI%+7d\n"
					 "energy bonus   I    A%3d%%AI%+7d\n"
					 "damage bonus   I    A%3d%%AI%+7d\n"
					 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
					 "bonus total    I        I%+7d\n"
					 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
					 "score          I      %10d\n\n"
					 "                    next scene \n"
					 "                    push button",
					 s1,b1,s2,b2,s3,b3,s4,b4,s5,b5,
					 timer/(60*60),(timer/60)%60,((timer%60)*100)/60,m1,
					 hp*100/10000,m2,dame*100/_HP,m3,b1+b2+b3+b4+b5+m1+m2+m3,score);
			else
				mistartsA.Format("[mission A%03dA result]           \n\n\n"
					 "               I        I      \n"
					 "weapon summary Ihit rateI score\n"
					 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
					 "machine gun    I  %sI%+7d\n"
					 "homing gun     I  %sI%+7d\n"
					 "space mine     I  %sI%+7d\n"
					 "a g missile    I  %sI%+7d\n"
					 "2-way gun      I  %sI%+7d\n"
					 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
					 "mission time   I%02d:%02d.%02dI%+7d\n"
					 "energy bonus   I    A%3d%%AI%+7d\n"
					 "damage bonus   I    A%3d%%AI%+7d\n"
					 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
					 "bonus total    I        I%+7d\n"
					 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
					 "score          I      %10d\n\n"
					 "                    next scene \n"
					 "                    push button",chacha.stage,
					 s1,b1,s2,b2,s3,b3,s4,b4,s5,b5,
					 timer/(60*60),(timer/60)%60,((timer%60)*100)/60,m1,
					 hp*100/10000,m2,dame*100/_HP,m3,b1+b2+b3+b4+b5+m1+m2+m3,score);
		}
		break;
	case 4://���̂܂ܑҋ@
		{
			mistartmode4A+=2;if(mistartmode4A>255)mistartmode4A=0;
			//HosiOkuProc();
			if(b1>0){b1-=500; if(b1<0){score+=(500+b1);b1=0;}else{score+=500;}}
			else if(b2>0){b2-=500; if(b2<0){score+=(500+b2);b2=0;}else{score+=500;}}
			else if(b3>0){b3-=500; if(b3<0){score+=(500+b3);b3=0;}else{score+=500;}}
			else if(b4>0){b4-=500; if(b4<0){score+=(500+b4);b4=0;}else{score+=500;}}
			else if(b5>0){b5-=500; if(b5<0){score+=(500+b5);b5=0;}else{score+=500;}}
			else if(m1>0){m1-=500; if(m1<0){score+=(500+m1);m1=0;}else{score+=500;}}
			else if(m2>0){m2-=500;m1=0; if(m2<0){score+=(500+m2);m2=0;}else{score+=500;}}
			else if(m3>0){m3-=500; if(m3<0){score+=(500+m3);m3=0;}else{score+=500;}}
			//                 1         2         3
			//        12345678901234567890123456789012
			if(b1+b2+b3+b4+b5+m1+m2+m3==0){
				if(chacha.stage==999)
					mistartsA.Format("[mission enhanced result]      \n\n\n"
						 "               I        I      \n"
						 "weapon summary Ihit rateI score\n"
						 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
						 "machine gun    I  %sI%+7d\n"
						 "homing gun     I  %sI%+7d\n"
						 "space mine     I  %sI%+7d\n"
						 "a g missile    I  %sI%+7d\n"
						 "2-way gun      I  %sI%+7d\n"
						 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
						 "mission time   I%02d:%02d.%02dI%+7d\n"
						 "energy bonus   I    A%3d%%AI%+7d\n"
						 "damage bonus   I    A%3d%%AI%+7d\n"
						 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
						 "bonus total    I        I%+7d\n"
						 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
						 "score          I      %10d\n\n"
						 "                    next mission\n"
						 "                    push button",
						 s1,b1,s2,b2,s3,b3,s4,b4,s5,b5,
						 timer/(60*60),(timer/60)%60,((timer%60)*100)/60,m1,
						 hp*100/10000,m2,dame*100/_HP,m3,b1+b2+b3+b4+b5+m1+m2+m3,score);
				else
						mistartsA.Format("[mission A%03dA result]           \n\n\n"
						 "               I        I      \n"
						 "weapon summary Ihit rateI score\n"
						 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
						 "machine gun    I  %sI%+7d\n"
						 "homing gun     I  %sI%+7d\n"
						 "space mine     I  %sI%+7d\n"
						 "a g missile    I  %sI%+7d\n"
						 "2-way gun      I  %sI%+7d\n"
						 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
						 "mission time   I%02d:%02d.%02dI%+7d\n"
						 "energy bonus   I    A%3d%%AI%+7d\n"
						 "damage bonus   I    A%3d%%AI%+7d\n"
						 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
						 "bonus total    I        I%+7d\n"
						 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
						 "score          I      %10d\n\n"
						 "replay      r key   next mission\n"
						 "replay save s key   push button",chacha.stage,
						 s1,b1,s2,b2,s3,b3,s4,b4,s5,b5,
						 timer/(60*60),(timer/60)%60,((timer%60)*100)/60,m1,
						 hp*100/10000,m2,dame*100/_HP,m3,b1+b2+b3+b4+b5+m1+m2+m3,score);

			}else{
				if(chacha.stage==999)
					mistartsA.Format("[mission enhanced result]      \n\n\n"
						 "               I        I      \n"
						 "weapon summary Ihit rateI score\n"
						 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
						 "machine gun    I  %sI%+7d\n"
						 "homing gun     I  %sI%+7d\n"
						 "space mine     I  %sI%+7d\n"
						 "a g missile    I  %sI%+7d\n"
						 "2-way gun      I  %sI%+7d\n"
						 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
						 "mission time   I%02d:%02d.%02dI%+7d\n"
						 "energy bonus   I    A%3d%%AI%+7d\n"
						 "damage bonus   I    A%3d%%AI%+7d\n"
						 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
						 "bonus total    I        I%+7d\n"
						 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
						 "score          I      %10d",
						 s1,b1,s2,b2,s3,b3,s4,b4,s5,b5,
						 timer/(60*60),(timer/60)%60,((timer%60)*100)/60,m1,
						 hp*100/10000,m2,dame*100/_HP,m3,b1+b2+b3+b4+b5+m1+m2+m3,score);
				else
					mistartsA.Format("[mission A%03dA result]           \n\n\n"
						 "               I        I      \n"
						 "weapon summary Ihit rateI score\n"
						 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
						 "machine gun    I  %sI%+7d\n"
						 "homing gun     I  %sI%+7d\n"
						 "space mine     I  %sI%+7d\n"
						 "a g missile    I  %sI%+7d\n"
						 "2-way gun      I  %sI%+7d\n"
						 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
						 "mission time   I%02d:%02d.%02dI%+7d\n"
						 "energy bonus   I    A%3d%%AI%+7d\n"
						 "damage bonus   I    A%3d%%AI%+7d\n"
						 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
						 "bonus total    I        I%+7d\n"
						 "LLLLLLLLLLLLLLLXLLLLLLLLXLLLLLLL\n"
						 "score          I      %10d",chacha.stage,
						 s1,b1,s2,b2,s3,b3,s4,b4,s5,b5,
						 timer/(60*60),(timer/60)%60,((timer%60)*100)/60,m1,
						 hp*100/10000,m2,dame*100/_HP,m3,b1+b2+b3+b4+b5+m1+m2+m3,score);
			}
		}
		mistartmode2A++;if(mistartmode2A>5&&m3!=0){mistartmode2A=0;chacha_a.Play(33); }
		if((b1+b2+b3+b4+b5+m1+m2+m3)==0&&tamaf){mistartmodeA=5;mistartmode2A=0;chacha_a.Play(1);}
		if((b1+b2+b3+b4+b5+m1+m2+m3)==0&&repf&&chacha.stage!=999){mistartmodeA=5;mistartmode2A=0;chacha_a.Play(1);repf_=1;}
		if((b1+b2+b3+b4+b5+m1+m2+m3)==0&&repsf&&chacha.stage!=999){repsf=0;fc.ReplaySave();Sleep(100);KeyWait();	}
//		mistartmode2A++;mistartmodefA++;if(mistartmode2A>20){ mistartmodeA=5;mistartmode2A=0;}
		repf=0;repsf=0;
//		}
		break;
	case 5://�����ď�����
		mistartmode2A++;if(mistartmode2A>260/2){ mistartfA=2;if(repf_)mistartmodeA=99;}
		mistartmode4A+=2;if(mistartmode4A>255)mistartmode4A=0;
		break;
	case 6://���̂܂܏I����
		if(tamaf){mistartmodeA=7;chacha_a.Play(1);
			for(int i=0;i<20;i++){
				if(save.score[i][0]<score){//�����L���O�ɓ�����
					break;
				}
			}
			if(i==20)//�����L���O�O
				fadef=TRUE;
		}break;
	case 7://���̂܂܏I����
			mistartmodefA++;mistartmode2A++;if(mistartmode2A>160){ mistartfA=2;}
		break;
	}

}

void CMainFrame::DrawMiStart2()
{
	if(mistartfA==0) return;
	char str[2];int i;
	switch(mistartmodeA){
	case 0://���邭���]�\��
		if(mistartmode3A==TRUE)chacha.Fade(5);
		for(i=0;i<mistartsuA;i++){
			if(mistartA[i][2]>=0||mistartcA[i]=='\n'||mistartcA[i]==' '||mistartcA[i]=='A') continue;
			str[0]=mistartcA[i];
			chacha.Moji32(str,1,mistartc2A[i],mistartA[i][3],mistartA[i][4],FALSE,255);
		}break;
	case 1:
	case 4:
		//if(mistartmodeA==4) HosiOkuDraw();
		if(mistartmode3A==TRUE)chacha.Fade(5);
		chacha.Moji32(mistartsA,mistartsA.GetLength(),0,(float)mistartxA,(float)mistartyA,TRUE,255);
		if(save.stagetime[stage-1][save.nanido]>timer || save.stagetime[stage-1][save.nanido]==-1)
			chacha.Moji16("misson time\nnew record !!",25,0,17*32-8,(float)mistartyA+16*32+3,TRUE,(mistartmode4A>127?255-mistartmode4A:mistartmode4A)*2);
		break;
	case 6:
		if(mistartmode3A==TRUE){
			chacha.Fade(mistartmodefA*3);
		}
		chacha.Moji32(mistartsA,mistartsA.GetLength(),0,(float)mistartxA,(float)mistartyA,FALSE,255-((mistartmode2A>80)?(mistartmode2A-80)*3:0));
		break;
	case 7:
		if(mistartmode3A==TRUE){
			chacha.Fade(mistartmodefA*3);
		}
		chacha.Moji32(mistartsA,mistartsA.GetLength(),0,(float)mistartxA,(float)mistartyA,TRUE,255-((mistartmode2A>80)?(mistartmode2A-80)*3:0));
		break;
	case 2:
	case 3:
		chacha.Moji32(mistartsA,mistartsA.GetLength(),0,(float)mistartxA,(float)mistartyA,TRUE,255);
		g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
		for(i=0;i<mistartmode2A;i++)
			chacha.Moji32(mistartsA,mistartsA.GetLength(),0,(float)mistartxA,(float)mistartyA,FALSE,1);
		g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
//		if(mistartmode2A<1){
		if(save.stagetime[stage-1][save.nanido]>timer || save.stagetime[stage-1][save.nanido]==-1)
			chacha.Moji16("misson time\nnew record !!",25,0,17*32-8,(float)mistartyA+16*32+3,TRUE,(mistartmode4A>127?255-mistartmode4A:mistartmode4A)*2);
//		}
		break;
	case 5:
		chacha.Moji32(mistartsA,mistartsA.GetLength(),0,(float)mistartxA,(float)mistartyA,TRUE,255);
		if(save.stagetime[stage-1][save.nanido]>timer || save.stagetime[stage-1][save.nanido]==-1)
			chacha.Moji16("misson time\nnew record !!",25,0,17*32-8,(float)mistartyA+16*32+3,TRUE,(mistartmode4A>127?255-mistartmode4A:mistartmode4A)*2);
		if(mistartmode3A==FALSE){
			chacha.Fade(mistartmode2A*2);
		}
		break;
	}
}

void CMainFrame::KeyWait()
{
	for(;;){
		tekitamaf=0;tamaf=0;movef=0;
		chacha.GetInput();
		input();
		if(tekitamaf==0&&tamaf==0&&movef==0) break;
	}
	tekitamaf=0;tamaf=0;movef=0;
}

BOOL CMainFrame::GamePlay(int no)
{
	//�ϐ�������
	sabun=0.0f;bukia=0;bukib=0;bukic=0;bukid=0;bukif=0;tekitamaf=0;tekitamasu=0;tekitamano=16;
	DWORD f1=0,f2=0,f3=0;xxs=yys=0;mistartmodeA=0;mistartmode=0;tekisu=0; tamasu=0;motsu=0;
	timer=0;gameover=0;emptyanime=hpanime=dameanime=0;
	atari=FALSE;jikitama[1]=jikitama[3]=300/(save.nanido*2+1)+100/(save.nanido+1);
	jikitama[0]=jikitama[2]=jikitama[4]=9999999;lockonsu=0;
	tamasu=0;tamaf=0;tamacnt=7;motsu=0;motcnt=0;damenow=0;hitcnt=0;missioncnt=0;
	_HP=500/(save.nanido+1);if(_HP<170) _HP=170;
	DWORD fadef=255;timen=1;hp=10000;;dame=_HP;killsec=0;
	DWORD timen2=timen;buki=0;
	CString s;yys=0.01f;
	urawaza=FALSE;
	clearf=FALSE;
//	lockten=0;lockten2=156;
	if(no==26)
		bgm="bgm06.ogg";
	else if(no<9)
		bgm="bgm01.ogg";
	else if(no<18)
		bgm="bgm07.ogg";
	else
		bgm="bgm08.ogg";
	LoadStage(no);
	chacha.stage=no;
//	chacha_a.WaveDAStart("bgm01.ogg");
	mistartf=0;mistartfA=0;
	//�~�b�V�����X�^�[�g�̕���
	if(chacha.stage==999)
		s.Format("mission\nenhance\n start ");
	else
		s.Format("mission\n  A%03dA  \n start ",chacha.stage);
	InitMiStart(s);	
	//��
	for(int i=0;i<5;i++){ hitp[i][0]=0;hitp[i][1]=0;}
	for(i=0;i<(ds.mapx+ds.mapy)/2*6;i++){
		haikei[i][0]=(rand()%(ds.mapx))*32+rand()%32;	haikei[i][1]=(rand()%(ds.mapy))*32+rand()%32;
		haikei[i][2]=rand()%6+2;
		haikei[i][0]/=haikei[i][2];						haikei[i][1]/=haikei[i][2];
		haikei[i][3]=rand()%8;
	}
	for(i=0;i<(ds.mapx+ds.mapy)/2*6-1;i++){
		for(int j=i;j<(ds.mapx+ds.mapy)/2*6-1;j++){
			if(haikei[j][2]<haikei[j+1][2]){
				for(int k=0;k<4;k++){int l=haikei[j][k]; haikei[j][k]=haikei[j+1][k]; haikei[j+1][k]=l;}
			}
		}
	}
	//���C�����[�v
	tekitamaf=0;tamaf=0;movef=0;
	if(chacha.DoEventChk(endf)==FALSE){endf=TRUE; return TRUE; }
	chacha.Timing64(f2);
	chacha.Timing64(chacha.fpstiming,FALSE);
	for(;;){
		chacha.GetInput();
		input();
		mousex_=(float)mousex*gamenx/gamenx_;
		mousey_=(float)mousey*gameny/gameny_;
		if(endf) return TRUE;
		if(tekitamaf&&demof) return TRUE;
		if(tekitamaf&&(gameover==0)){
			int repb=replay; replay=-1;
			KeyWait();
			int r=PausePlay();
			switch(r){
			case 0://�Q�[�����A
				KeyWait();
				chacha.Timing64(f2);
				chacha.Timing64(chacha.fpstiming,FALSE);
				replay=repb;
				break;
			case 1://���^�C�A�@������Q�[���I�[�o�[
				KeyWait();
				gameover=1; gameovercnt=0;chacha_a.Play(18);
				//            321098765432101234568790123            
				//            123456789012345678901234567
				InitMiStart2("missions failed !!         \n"
							 "retire-selects purposely   \n\n"
							 "       --game over--       \n\n"
							 "                  try again",TRUE);
					mot[motsu][0]=0;			mot[motsu][1]=0;		mot[motsu][2]=5;			mot[motsu][3]=-1;
					mot[motsu][4]=0;			mot[motsu][5]=0;	mot[motsu][6]=0;		motsu++;
				replay=repb;
				break;
			case 2://�^�C�g����
				mistartmodeA=500;
				replay=repb;
				return FALSE;
			case 3://�I��
				mistartmodeA=0;
				replay=repb;
				return FALSE;
			}
		}
		if(timer>=60*60*99-2){
			gameover=1; gameovercnt=0;chacha_a.Play(18);
			InitMiStart2("missions failed !!         \n"
						 "specified time was over    \n\n"
						 "       --game over--       \n\n"
						 "                  try again",TRUE);
				mot[motsu][0]=0;			mot[motsu][1]=0;		mot[motsu][2]=5;			mot[motsu][3]=-1;
				mot[motsu][4]=0;			mot[motsu][5]=0;	mot[motsu][6]=0;		motsu++;
		}
		if((mistartmode==5||mistartf==0)&&mistartmodeA<6)
			timer++;
		if(mistartmode==5||mistartf==0||mistartmode3==TRUE){
			if(mistartmodeA<6&&mistartf!=2)
				Idou();//���@�ړ��A�ʈړ��A�G�ړ�
		}
		if(missioncnt==0&&mistartf==0){
			if(chacha.stage==999)
				s.Format(" mission \n enhance\ncomplete!!");
			else
				s.Format(" mission \n   A%03dA   \ncomplete!!",chacha.stage);
			InitMiStart(s,TRUE);	
		}
		AnimeMiStart();
		AnimeMiStart2();
		if(mistartfA==2){return FALSE;
		}
		if(dame<_HP/4){ colof++;hpanime++; }else{colof=0;hpanime=0;}
		if(colof==8) colof=0;
		if(hpanime==60) hpanime=0;
		if(hp<10000/4){dameanime++;}else{dameanime=0;}
		if(dameanime==60) dameanime=0;
		if(emptyanime==60) emptyanime=0;else emptyanime++;
//		lockten+=3; lockten=(lockten%(256-70)); lockten_=(lockten>127-35)?255-70-lockten:lockten;
//		lockten2+=3; lockten2=(lockten2%(256-70)); lockten2_=(lockten2>127-35)?255-70-lockten2:lockten2;
		//�ȉ��`��B�@�`�撆�ɂ͕`��ȊO�̌v�Z���͓���Ȃ�����
		if(timen==1){//DirectX�`��J�n
			chacha.Begin(gameover^1);
			Draw();
				//�������
			if(fps){
				CString s1;s1="table use";
				if((chacha.ff[1]&0x80000000)&&save._3dnow)s1="3d now!! use";//3DNow!!
				else if((chacha.ff[0]&0x02000000)&&save.sse)s1="sse use";//SSE
				s.Format("sqrt/int:%s\n%3d[%3d]ms %2dfps",s1,f1,f3,60/timen2);
				chacha.Moji16(s,s.GetLength(),0,20,80,TRUE,200);
			}
				if(muteki){
					s="muteki on";
					chacha.Moji16(s,s.GetLength(),0,20,120,TRUE,200);
				}
				if(mugen){
					s="mugen on";
					chacha.Moji16(s,s.GetLength(),0,20,140,TRUE,200);
				}
				if(renmax){
					s="rensha max on";
					chacha.Moji16(s,s.GetLength(),0,20,160,TRUE,200);
				}
				if(romrom){
					s="rom debug mode";
					chacha.Moji16(s,s.GetLength(),0,20,180,TRUE,200);
				}

			DrawMiStart();
			DrawMiStart2();

			//�`��I���
			chacha.End();
			if(!chacha.DoEventChk(endf)){endf=TRUE; break; }
			chacha.End(FALSE);//�`��I��
			f1=chacha.Timing64(f2,FALSE);
			timen=GetTiming(f1);
			chacha.timing1((WORD)timen,FALSE,FALSE);
			timen2=timen;
			chacha.Timing64(f2,FALSE);
			f3=chacha.Timing64(chacha.fpstiming,FALSE);
		}else timen--;
	}
	return TRUE;
}

void CMainFrame::Draw()
{
	CString s;
		int ii,jj,ii2,jj2,hix,hiy,ij3,i,j,k,hi;
		float iif,jjf,ii4,ii5,jj4,jj5,jkjk,ii6,jj6,ii7,jj7;
			//�w�i
			//if(mistartmodeA<3)
			{
				for(hi=0;hi<(ds.mapx+ds.mapy)/2*(2*(3-save.star));hi++){
						hix=haikei[hi][0];
						hiy=haikei[hi][1];
						//��ʊO�ŕ\���\�����ׂ�
						ii6=hix-xx/haikei[hi][2];jj6=hiy-yy/haikei[hi][2];
						ii4=xx/(float)haikei[hi][2];jj4=yy/(float)haikei[hi][2];
						ii7=(float)(mapx32)/haikei[hi][2];jj7=(float)(mapy32)/haikei[hi][2];
						if(gamenx2     > ii4)if(ii7+(ftol(ii4)-(ftol(gamenx2)))<hix)ii6=(hix-(ii7+(ii4-gamenx2)))-gamenx2;
						if(gameny2     > jj4)if(jj7+(ftol(jj4)-(ftol(gameny2)))<hiy)jj6=(hiy-(jj7+(jj4-gameny2)))-gameny2;
						if(ii7-gamenx2 < ii4)if(gamenx2-(ii7-ii4)              >hix)ii6=(ii7-ii4)+hix;
						if(jj7-gameny2 < jj4)if(gameny2-(jj7-jj4)              >hiy)jj6=(jj7-jj4)+hiy;
						if(gamenx2+ii6<-10 || gamenx2+ii6 > gamenx +10 || gameny2+jj6<-10 || gameny2+jj6>gameny+10){	}else{
							for(i=-4;i<5;i++){
								for(j=-4;j<5;j++){
									ii4=gamenx2+ii6+ii7*i; jj4=gameny2+jj6+jj7*j;
									if(ii4>=0 && ii4<gamenx && jj4 >=0 && jj4<gameny)
										chacha.PartDrawA(ii4,jj4,haikei[hi][3]*32,2*32,2,2,4,160);
								}
							}
						}
				}
				//�}�b�v�\��
				ii2=ftol(xx)/32;jj2=ftol(yy)/32;
				for(i=-16;i<17;i++){for(j=-12;j<13;j++){
						jj=j+jj2;ii=i+ii2;
						if(ii<0) ii=ds.mapx+ii;			if(jj<0) jj=ds.mapy+jj;
						if(ii>=ds.mapx) ii=ii-ds.mapx;	if(jj>=ds.mapy) jj=jj-ds.mapy;
						if(ds.mapdata[ii][jj]!=0)	chacha.PartDraw(gamenx2+i*32+16-(xx-ii2*32),gameny2+j*32+16-(yy-jj2*32),(ds.mapdata[ii][jj]%16)*32,(ds.mapdata[ii][jj]/16)*32,0,FALSE,255);
					}
				}

				//�G�\��
				for(i=0;i<tekisu;i++){
					if(tekidata[i][39])continue;
					iif=tekidata[i][0]-xx;jjf=tekidata[i][1]-yy;			//��ʊO�ŕ\���\�����ׂ�
					if(gamenx2 > xx)if(mapx32+(xx-(gamenx2))<tekidata[i][0]+32)	iif=(tekidata[i][0]-(mapx32+(xx-gamenx2)))-gamenx2;
					if(gameny2 > yy)if(mapy32+(yy-(gameny2))<tekidata[i][1]+32)	jjf=(tekidata[i][1]-(mapy32+(yy-gameny2)))-gameny2+1;
					if(mapx32-gamenx2 < xx)	if(gamenx2-(mapx32-xx) > tekidata[i][0]-32)iif=(mapx32-xx)+tekidata[i][0];
					if(mapy32-gameny2 < yy)	if(gameny2-(mapy32-yy) > tekidata[i][1]-32)jjf=(mapy32-yy)+tekidata[i][1]-1;
					for(j=0;j<128;j++){
						if(GetTekiNo(i,j)){
							if(gamenx2+iif>-64 && gamenx2+iif<gamenx+64 && gameny2+jjf>-64 && gameny2+jjf<gameny+64){
								switch(tekipart[j][1]){
								case 2://�U��
									chacha.TekiDraw(gamenx2+iif,gameny2+jjf,tekipart[j][2]*32,tekipart[j][3]*32,(360-tekidata[i][17])+180,TRUE);break;
								case 3:case 4://��
									chacha.TekiDraw(gamenx2+iif,gameny2+jjf,tekipart[j][2]*32,tekipart[j][3]*32,(360-tekidata[i][18])+180,TRUE);break;
								case 0:case 1://�ړ�
								default:
									chacha.TekiDraw(gamenx2+iif,gameny2+jjf,tekipart[j][2]*32,tekipart[j][3]*32,(360-tekidata[i][4])+180,TRUE);break;
								}
							}
						}
					}
				}
				//�G�t�F�N�g�\��
				for(i=0;i<motsu;i++){
					if(mot[i][6]>0||mot[i][2]==0||(mot[i][2]==5&&mot[i][4]!=1)){  continue;}
					iif=mot[i][0]-xx;jjf=mot[i][1]-yy;			//��ʊO�ŕ\���\�����ׂ�
					if(gamenx2 > xx)if(mapx32+(xx-gamenx2)<mot[i][0]+32)iif=(mot[i][0]-(mapx32+(xx-gamenx2)))-gamenx2;
					if(gameny2 > yy)if(mapy32+(yy-gameny2)<mot[i][1]+32)jjf=(mot[i][1]-(mapy32+(yy-gameny2)))-gameny2+1;
					if(mapx32-gamenx2 < xx)	if(gamenx2-(mapx32-xx) > mot[i][0]-32)iif=(mapx32-xx)+mot[i][0];
					if(mapy32-gameny2 < yy)	if(gameny2-(mapy32-yy) > mot[i][1]-32)jjf=(mapy32-yy)+mot[i][1]-1;
					if(gamenx2+iif>-32 && gamenx2+iif<gamenx+32 && gameny2+jjf>-32 && gameny2+jjf<gameny+32){
						if(mot[i][2]==5&&mot[i][4]==1){//�G���j
							chacha.JikiDraw(gamenx2+iif,gameny2+jjf,mot[i][3]*32,(mot[i][2]+2)*32,0,FALSE,250-mot[i][5]*5,mot[i][5]);
						}else
							chacha.JikiDraw(gamenx2+iif,gameny2+jjf,mot[i][3]*32,(mot[i][2]+2)*32,0,FALSE,150);
					}
				}//�G�e�\��
				for(i=0;i<tekitamasu;i++){
					iif=tekitama[i][0]-xx;jjf=tekitama[i][1]-yy;
					//��ʊO�ŕ\���\�����ׂ�
					if(gamenx2 > xx)if(mapx32+(xx-(gamenx2))<=tekitama[i][0]+32)	iif=(tekitama[i][0]-(mapx32+(xx-gamenx2)))-gamenx2;
					if(gameny2 > yy)if(mapy32+(yy-(gameny2))<=tekitama[i][1]+32)	jjf=(tekitama[i][1]-(mapy32+(yy-gameny2)))-gameny2+1;
					if(mapx32-gamenx2 < xx)	if(gamenx2-(mapx32-xx) >= tekitama[i][0]-32)	iif=(mapx32-xx)+tekitama[i][0];
					if(mapy32-gameny2 < yy)	if(gameny2-(mapy32-yy) >= tekitama[i][1]-32)	jjf=(mapy32-yy)+tekitama[i][1]-1;
					ij3=16;	switch(ftol(tekitama[i][5])){
					case 1:case 2:case 3:case 4:case 19:case 27:case 28:case 29:case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 37:case 38:case 39:case 40:case 41:case 42:case 43:case 44:case 45:case 46:ii2=0; jj2=32; break;
					case 5:case 6:case 7:case 8:ii2=32; jj2=32; ij3-=ftol(tekitama[i][7]*2); break;
					case 13:case 14:case 15:case 16:ii2=2*32; jj2=32; break;
					case 9:case 10:case 11:case 12:case 24:case 25:case 26:ii2=3*32; jj2=32; break;
					case 17:ii2=4*32; jj2=32; break;
					case 18:case 20:ii2=5*32; jj2=32; break;
					case 21:case 22:case 23:ii2=6*32; jj2=32; break;
					}
					if(gamenx2+iif>-32 && gamenx2+iif<gamenx+32 && gameny2+jjf>-32 && gameny2+jjf<gameny+32)
						chacha.TekiDraw(gamenx2+iif,gameny2+jjf,ii2,jj2,(360-tekitama[i][8])+180,TRUE,ftol(tekitama[i][9]),ij3);
				}//�ʕ\��
				for(i=0;i<tamasu;i++){
					iif=tama[i][0]-xx;jjf=tama[i][1]-yy;			//��ʊO�ŕ\���\�����ׂ�
					if(gamenx2 > xx)if(mapx32+(xx-(gamenx2))<tama[i][0]+32)	iif=(tama[i][0]-(mapx32+(xx-gamenx2)))-gamenx2;
					if(gameny2 > yy)if(mapy32+(yy-(gameny2))<tama[i][1]+32)	jjf=(tama[i][1]-(mapy32+(yy-gameny2)))-gameny2+1;
					if(mapx32-gamenx2 < xx)	if(gamenx2-(mapx32-xx) > tama[i][0]-32)iif=(mapx32-xx)+tama[i][0];
					if(mapy32-gameny2 < yy)	if(gameny2-(mapy32-yy) > tama[i][1]-32)jjf=(mapy32-yy)+tama[i][1]-1;
					switch(ftol(tama[i][4])){
					case 0:ii2=2*32; jj2=0; break;
					case 1:ii2=3*32; jj2=0; break;
					case 2:ii2=(ftol(tama[i][7])/60)*32; jj2=32; break;
					case 3:ii2=(2+(ftol(tama[i][7])/10))*32; jj2=32; break;
					case 4:ii2=2*32; jj2=0; break;
					}
					if(gamenx2+iif>-32 && gamenx2+iif<gamenx+32 && gameny2+jjf>-32 && gameny2+jjf<gameny+32)
						chacha.JikiDraw(gamenx2+iif,gameny2+jjf,ii2,jj2,(360-tama[i][5])+180,TRUE);
				}
				for(i=0;i<tekisu;i++){
					if(tekidata[i][39])continue;
					k=0;
					for(j=0;j<lockonsu;j++){
						if(lockon[j]==i) k=1;
					}
					iif=tekidata[i][0]-xx;jjf=tekidata[i][1]-yy;			//��ʊO�ŕ\���\�����ׂ�
					if(gamenx2 > xx)if(mapx32+(xx-(gamenx2))<tekidata[i][0]+64)	iif=(tekidata[i][0]-(mapx32+(xx-gamenx2)))-gamenx2;
					if(gameny2 > yy)if(mapy32+(yy-(gameny2))<tekidata[i][1]+64)	jjf=(tekidata[i][1]-(mapy32+(yy-gameny2)))-gameny2+1;
					if(mapx32-gamenx2 < xx)	if(gamenx2-(mapx32-xx) > tekidata[i][0]-64)iif=(mapx32-xx)+tekidata[i][0];
					if(mapy32-gameny2 < yy)	if(gameny2-(mapy32-yy) > tekidata[i][1]-64)jjf=(mapy32-yy)+tekidata[i][1]-1;
					if(gamenx2+iif>-128 && gamenx2+iif<gamenx+128 && gameny2+jjf>-128 && gameny2+jjf<gameny+128){
						if(k){//���b�N�I��
							chacha.kaiten=1;
//							chacha.JikiDraw2(gamenx2+iif,gameny2+jjf,1*32,16,180,1,255,16);
//							chacha.JikiDraw(gamenx2+iif+32,gameny2+jjf,7*32,0,0,FALSE,255);
							chacha.JikiDraw(gamenx2+iif+16,gameny2+jjf+16,1*32,0,180,1,255);
							chacha.JikiDraw(gamenx2+iif+32+16,gameny2+jjf+16,7*32,0,0,FALSE,255);
						}
						if(tekidata[i][6]==3){//TARGET
//							chacha.JikiDraw2(gamenx2+iif,gameny2+jjf-16,1*32,16,0,FALSE,255,16);
//							chacha.JikiDraw(gamenx2+iif+32,gameny2+jjf-32,6*32,0,0,FALSE,255);
							chacha.JikiDraw(gamenx2+iif+16,gameny2+jjf-16,1*32,0,0,FALSE,255);
							chacha.JikiDraw(gamenx2+iif+32+16,gameny2+jjf-16-32,6*32,0,0,FALSE,255);
						}

						ii=ftol(tekidata[i][5]*100/tekidata[i][32]);ii=100-ii;ii/=25; if(ii<0) ii=0; if(ii>3)ii=3;
						if(tekidata[i][34]==0)chacha.TekiDraw(gamenx2+iif,gameny2+jjf,ii*32+32*4,0,0,FALSE);
					}
				}
				//���@�\��(���S)
				if(!gameover)
					chacha.JikiDraw(gamenx2,gameny2,0,0,(360-chacha.GetShita((float)mousex,gamenx2_,(float)mousey,gameny2_))+180,TRUE);
				for(i=0;i<motsu;i++){
					if(mot[i][2]==0||(mot[i][2]==5&&mot[i][4]!=1))chacha.JikiDraw(gamenx2,gameny2,mot[i][3]*32,(mot[i][2]+2)*32,0,FALSE,200);
				}
				for(i=0;i<hitcnt;i++){
					chacha.JikiDraw(gamenx2,gameny2-hit[i][0]-10,4*32,32,0,FALSE,255-(DWORD)hit[i][0]*5);
				}
				jkjk=0;
				if(hp<10000/4){
					chacha.JikiDraw(gamenx2,gameny2+32,6*32,32,0,FALSE,255-((dameanime>30)?dameanime-30:30-dameanime)*8);
					jkjk=1;
				}
				if(dame<_HP/4){
					chacha.JikiDraw(gamenx2,gameny2+32+jkjk*20,5*32,32,0,FALSE,255-((hpanime>30)?hpanime-30:30-hpanime)*8);
				}

				//�e���Ȃ�
				if(replay==1&&demof==0){
					s="replay";
					chacha.Moji32(s,s.GetLength(),1,512-32*3,150,TRUE,150);
				}
				if(replay==1&&demof==1){
					s="demo play";
					chacha.Moji32(s,s.GetLength(),0,512-32*4-16,150,TRUE,150);
				}
				s.Format("score %10d",score);
				chacha.Moji32(s,s.GetLength(),1,20,10,TRUE,200);
				s.Format("%3d\n\n%3d",jikitama[1],jikitama[3]);
				chacha.Moji32(s,s.GetLength(),1,32,gameny-32*3-16,TRUE,200);
				if((timer%60)>=30){
					s.Format("target%02dA\n\n%02d:%02d %02dA",missioncnt,timer/(60*60),(timer/60)%60,((timer%60)*100)/60);
				}else{
					s.Format("target%02dA\n\n%02d:%02d.%02dA",missioncnt,timer/(60*60),(timer/60)%60,((timer%60)*100)/60);
				}
				chacha.Moji32(s,s.GetLength(),1,gamenx-32*8,gameny-32*3-16,TRUE,200);
				if(save.stagetime[stage-1][save.nanido]==-1)
					s="99:99.99";
				else
					s.Format("%02d:%02d.%02d",save.stagetime[stage-1][save.nanido]/(60*60),
					(save.stagetime[stage-1][save.nanido]/60)%60,((save.stagetime[stage-1][save.nanido]%60)*100)/60);
				chacha.Moji16(s,s.GetLength(),1,gamenx-16*9,gameny-32*2,TRUE,200);
				if(stage==999){
					s.Format("mission %s\nenhance",tb[save.nanido]);
				}else{
					s.Format("mission %s\n  %03d",tb[save.nanido],stage);
				}
				chacha.Moji16(s,s.GetLength(),1,gamenx-32*8-16,gameny-32*2-16,TRUE,200);
				chacha.PartDrawA(0 ,gameny-32*4-10,4*32   ,3*32,16,16,3,200);
				chacha.PartDrawA(32,gameny-32*4-10,4*32+32,3*32,16,16,3,200);
				chacha.PartDrawA(64,gameny-32*4-10,4*32+64,3*32,16,16,3,200);
				chacha.PartDrawA(96,gameny-32*4-10,4*32+96,3*32,16,16,3,200);
				chacha.PartDrawA(0 ,gameny-32*2-10,4*32   ,4*32,16,16,3,200);
				chacha.PartDrawA(32,gameny-32*2-10,4*32+32,4*32,16,16,3,200);
				chacha.PartDrawA(64,gameny-32*2-10,4*32+64,4*32,16,16,3,200);
				chacha.PartDrawA(96,gameny-32*2-10,4*32+96,4*32,16,16,3,200);



				//�o�[�\��
				int colo=6;		if(dame>50) colo=7;
				chacha.PartDrawA(gamenx5+2*32,gameny-32*3,32*5,0,14*16,16,3,160);
				chacha.PartDrawA(gamenx5+2*32,gameny-32*2,32*3,32,14*16,16,3,160);
				if(colof<3)chacha.PartDrawA(gamenx5+2*32,gameny-32*2+1,32*colo,0,(14*16*2)*(float)((_HP-dame)/(float)_HP),30,31,160);
				colo=4;	if(hp>2500) colo=5;if(hp>5000) colo=6;	if(hp>7500) colo=7;
				chacha.PartDrawA(gamenx5+2*32,gameny-32*3+2,32*colo,33,(14*16)*(float)(hp/10000.0f),15,3,160);

				chacha.PartDrawA(gamenx5+16*32,gameny-32*3,32*4,0,16,16,3,160);
				chacha.PartDrawA(gamenx5-32   ,gameny-32*3,0   ,0,16,16,3,160);
				chacha.PartDrawA(gamenx5-32   ,gameny-32*3,32*1,0,16,16,3,160);
				chacha.PartDrawA(gamenx5      ,gameny-32*3,32*2,0,16,16,3,160);
				chacha.PartDrawA(gamenx5+1*32,gameny-32*3,32*3,0,16,16,3,160);
				chacha.PartDrawA(gamenx5-64  ,gameny-32*2,0   ,0,16,16,3,160);
				chacha.PartDrawA(gamenx5-32  ,gameny-32*2,0   ,32,16,16,3,160);
				chacha.PartDrawA(gamenx5+0*32,gameny-32*2,32*1,32,16,16,3,160);
				chacha.PartDrawA(gamenx5+1*32,gameny-32*2,32*2,32,16,16,3,160);
				chacha.PartDrawA(gamenx5+16*32,gameny-32*2,32*4,0,16,16,3,160);

				//�~�j�}�b�v�\��
				chacha.PartDrawA(gamenx-ds.mapx,0,0,0,(float)ds.mapx/2,(float)ds.mapy/2,9,128,0,FALSE);
				chacha.PartDrawA(gamenx-ds.mapx,0,0,0,128,128,2,200,0,FALSE);
				for(i=0;i<tekisu;i++){
					if(tekidata[i][39])continue;
					if(tekidata[i][6]==3)
						chacha.PartDrawA(gamenx-ds.mapx+tekidata[i][0]/32-1,tekidata[i][1]/32-1,2*32,32,2,2,5,255,(360-tekidata[i][4])+180,TRUE);
					else
						chacha.PartDrawA(gamenx-ds.mapx+tekidata[i][0]/32-1,tekidata[i][1]/32-1,2*32,32,1.5f,1.5f,5,150,(360-tekidata[i][4])+180,TRUE);
				}
				chacha.PartDrawA(gamenx-ds.mapx+xx/32-2,yy/32-2,0,0,4,4,0,255,(360-chacha.GetShita((float)mousex,gamenx2,(float)mousey,gameny2))+180,TRUE);
	//			g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
	//			chacha.PartDrawA(gamenx-ds.mapx+xx/32-2,yy/32-2,0,0,3,3,0,255,(360-chacha.GetShita((float)mousex,gamenx2,(float)mousey,gameny2))+180,TRUE);
	//			g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
				//����I��
				if(bukib){
					int bukie[5]={-2,2,-1,1,0};
					for(ii2=0;ii2<5;ii2++){//bukia:0-16�ړ����� bukib=����I����ʊJ�� bukic=�t�F�[�h bukid=���݂̕���
						jj2=bukid+bukie[ii2];	if(jj2<0)jj2+=5; if(jj2>4)jj2-=5;
						switch(jj2){
						case 0:ii=0;jj=3*32;break;
						case 1:ii=4*32;jj=3*32;break;
						case 2:ii=0;jj=4*32;break;
						case 3:ii=4*32;jj=4*32;break;
						case 4:ii=0;jj=5*32;break;
						}
						int ii3=ii2;	ii2=bukie[ii2];
						if(abs(ii2)==2&&(abs(bukia)>3||(!bukia))){}else{
							chacha.PartDrawA(gamenx2-2*32,gameny2+64+ii2*32-bukia*4,ii   ,jj,16,16,3,200-abs((ii2)*2)*50-abs(bukia)*4-bukic*12);
							chacha.PartDrawA(gamenx2-1*32,gameny2+64+ii2*32-bukia*4,ii+32,jj,16,16,3,200-abs((ii2)*2)*50-abs(bukia)*4-bukic*12);
							chacha.PartDrawA(gamenx2+0*32,gameny2+64+ii2*32-bukia*4,ii+64,jj,16,16,3,200-abs((ii2)*2)*50-abs(bukia)*4-bukic*12);
							chacha.PartDrawA(gamenx2+1*32,gameny2+64+ii2*32-bukia*4,ii+96,jj,16,16,3,200-abs((ii2)*2)*50-abs(bukia)*4-bukic*12);
							if(jikitama[jj2]==0){
								chacha.JikiDraw(gamenx2-2*32+16,gameny2+64+ii2*32-bukia*4+16,7*32,32,0,FALSE,200-abs((ii2)*2)*50-abs(bukia)*4-bukic*12);
							}
						}ii2=ii3;
					}
				}
				//�}�E�X�\��
				ii=0;
				for(i=0;i<tekisu;i++){
					ii5=xx+mousex_-gamenx2;jj5=yy+mousey_-gameny2;
					if(ii5<0) ii5+=mapx32; if(ii5>mapx32) ii5-=mapx32;
					if(jj5<0) jj5+=mapy32; if(jj5>mapy32) jj5-=mapy32;
					ii4=tekidata[i][0];jj4=tekidata[i][1];
					if((ii4-16)<ii5 && (ii4+16)>ii5
					 &&(jj4-16)<jj5 && (jj4+16)>jj5 && tekidata[i][34]==0)//�G�ƃJ�[�\���������Ă邩�H
						ii=1;
				}
				if(mistartmodeA<6){
					if(ii){
						chacha.JikiDraw((float)mousex,(float)mousey,5*32,0,0,FALSE);
					}else{
						chacha.JikiDraw((float)mousex,(float)mousey,4*32,0,0,FALSE);
					}
					//�I�𒆕���
					switch(buki){
					case 0:ii=0;jj=3*32;break;
					case 1:ii=4*32;jj=3*32;break;
					case 2:ii=0;jj=4*32;break;
					case 3:ii=4*32;jj=4*32;break;
					case 4:ii=0;jj=5*32;break;
					}chacha.cl=1;
					chacha.PartDrawA((float)mousex-8,(float)mousey+8,ii,jj,8,8,3,160);
					if(jikitama[buki]==0){
						chacha.JikiDraw((float)mousex,(float)mousey,7*32,32,0,FALSE,255-((hpanime>30)?hpanime-30:30-hpanime)*8);
					}
				}
			}

}

void CMainFrame::PostNcDestroy() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	delete this;
	CFrameWnd::PostNcDestroy();
}

BOOL CMainFrame::DestroyWindow() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	if(endf)
		return CFrameWnd::DestroyWindow();
	else { endf=TRUE; return FALSE; }
}
BYTE g_diKeyState1[256];
void CMainFrame::input()
{
	BOOL sppp=FALSE;
	mousex=chacha.MState.x;
	mousey=chacha.MState.y;
	tamaf=chacha.MState.lButton;
	movef=chacha.MState.rButton;
	if(!tekitamaf)tekitamaf=chacha.MState.cButton;
	int zd=chacha.MState.wheel/120;chacha.MState.wheel=0;
	if(zd<0) buki++; if(buki== 5) buki=0;
	if(zd>0) buki--; if(buki==-1) buki=4;

	if((g_diKeyState[DIK_ESCAPE ]&0x80)&&!(g_diKeyState1[DIK_ESCAPE]&0x80)&&endff) tekitamaf=TRUE;
	if((g_diKeyState[DIK_L ]&0x80)&&!(g_diKeyState1[DIK_L]&0x80)) save.gengo^=1;
	if((g_diKeyState[DIK_R]&0x80)&&!(g_diKeyState1[DIK_R]&0x80)) repf=1;
	if((g_diKeyState[DIK_E]&0x80)&&!(g_diKeyState1[DIK_E]&0x80)) enhf=1;
	if((g_diKeyState[DIK_S]&0x80)&&!(g_diKeyState1[DIK_S]&0x80)) repsf=1;

	if((g_diKeyState[DIK_A]&0x80)&&!(g_diKeyState1[DIK_A]&0x80)) uraura+="A";
	if((g_diKeyState[DIK_B]&0x80)&&!(g_diKeyState1[DIK_B]&0x80)) uraura+="B";
	if((g_diKeyState[DIK_C]&0x80)&&!(g_diKeyState1[DIK_C]&0x80)) uraura+="C";
	if((g_diKeyState[DIK_D]&0x80)&&!(g_diKeyState1[DIK_D]&0x80)) uraura+="D";
	if((g_diKeyState[DIK_E]&0x80)&&!(g_diKeyState1[DIK_E]&0x80)) uraura+="E";
	if((g_diKeyState[DIK_F]&0x80)&&!(g_diKeyState1[DIK_F]&0x80)) uraura+="F";
	if((g_diKeyState[DIK_G]&0x80)&&!(g_diKeyState1[DIK_G]&0x80)) uraura+="G";
	if((g_diKeyState[DIK_H]&0x80)&&!(g_diKeyState1[DIK_H]&0x80)) uraura+="H";
	if((g_diKeyState[DIK_I]&0x80)&&!(g_diKeyState1[DIK_I]&0x80)) uraura+="I";
	if((g_diKeyState[DIK_J]&0x80)&&!(g_diKeyState1[DIK_J]&0x80)) uraura+="J";
	if((g_diKeyState[DIK_K]&0x80)&&!(g_diKeyState1[DIK_K]&0x80)) uraura+="K";
	if((g_diKeyState[DIK_L]&0x80)&&!(g_diKeyState1[DIK_L]&0x80)) uraura+="L";
	if((g_diKeyState[DIK_M]&0x80)&&!(g_diKeyState1[DIK_M]&0x80)) uraura+="M";
	if((g_diKeyState[DIK_N]&0x80)&&!(g_diKeyState1[DIK_N]&0x80)) uraura+="N";
	if((g_diKeyState[DIK_O]&0x80)&&!(g_diKeyState1[DIK_O]&0x80)) uraura+="O";
	if((g_diKeyState[DIK_P]&0x80)&&!(g_diKeyState1[DIK_P]&0x80)) uraura+="P";
	if((g_diKeyState[DIK_Q]&0x80)&&!(g_diKeyState1[DIK_Q]&0x80)) uraura+="Q";
	if((g_diKeyState[DIK_R]&0x80)&&!(g_diKeyState1[DIK_R]&0x80)) uraura+="R";
	if((g_diKeyState[DIK_S]&0x80)&&!(g_diKeyState1[DIK_S]&0x80)) uraura+="S";
	if((g_diKeyState[DIK_T]&0x80)&&!(g_diKeyState1[DIK_T]&0x80)) uraura+="T";
	if((g_diKeyState[DIK_U]&0x80)&&!(g_diKeyState1[DIK_U]&0x80)) uraura+="U";
	if((g_diKeyState[DIK_V]&0x80)&&!(g_diKeyState1[DIK_V]&0x80)) uraura+="V";
	if((g_diKeyState[DIK_W]&0x80)&&!(g_diKeyState1[DIK_W]&0x80)) uraura+="W";
	if((g_diKeyState[DIK_X]&0x80)&&!(g_diKeyState1[DIK_X]&0x80)) uraura+="X";
	if((g_diKeyState[DIK_Y]&0x80)&&!(g_diKeyState1[DIK_Y]&0x80)) uraura+="Y";
	if((g_diKeyState[DIK_Z]&0x80)&&!(g_diKeyState1[DIK_Z]&0x80)) uraura+="Z";
	if((g_diKeyState[DIK_SPACE]&0x80)&&!(g_diKeyState1[DIK_SPACE]&0x80)){  sppp=TRUE; }
	if((g_diKeyState[DIK_F12]&0x80)&&!(g_diKeyState1[DIK_F12]&0x80)){
		if(uraura=="FPS") fps^=TRUE;
		if(uraura=="MUTEKI") muteki^=TRUE;
		if(uraura=="MUGEN"){mugen^=TRUE; jikitama[1]=999; jikitama[3]=999;}
		if(uraura=="RENMAX") renmax^=TRUE;
		if(uraura=="NANMAX") save.maxnanido=4;
		if(uraura=="ALLPOWER"){ renmax=TRUE;muteki=TRUE;mugen=TRUE; jikitama[1]=999; jikitama[3]=999;}
		if(uraura=="ALLOFF"){ renmax=0;muteki=0;mugen=0;}
		if(uraura=="STAGEMAX") save.clearstage=STAGEMAX;
		if(uraura=="ROMROM"){romrom=TRUE;}
		uraura="";
	}
	memcpy(g_diKeyState1,g_diKeyState,sizeof(BYTE)*256);
	if(demof){
		tekitamaf|=movef|tamaf;
	}
	if(replay==0){//0:���v���C�ۑ� 1:���v���C�Đ�
		WORD f=0;
		mousex_=(float)mousex*gamenx/gamenx_;
		mousey_=(float)mousey*gameny/gameny_;
		rdata[timer].mx=mousex_;
		rdata[timer].my=mousey_;
		if((mistartmode==5||mistartf==0)&&mistartmodeA<6){
			rdata[timer].xx=xx;
			rdata[timer].yy=yy;
		}
		//FEDC BA98 7654 3210
		//0:tamaf
		//1:movef
		//2:muteki
		//3:mugen
		//4:renmax
		//5:endf
		//6,7,8,9:buki
		//A:space
		f = (tamaf )?1:0;
		f|=((movef )?1:0)<<1;
		f|=((muteki)?1:0)<<2;
		f|=((mugen )?1:0)<<3;
		f|=((renmax)?1:0)<<4;
		f|=((endf  )?1:0)<<5;
		f|=((WORD)buki)  <<6;
		f|=((sppp  )?1:0)<<10;
		if(sppp)lockonsu=0;
		rdata[timer].flag=f;
	}else if(replay==1&&gameover==0){
		mousex_=rdata[timer].mx;
		mousey_=rdata[timer].my;
		if((mistartmode==5||mistartf==0)&&mistartmodeA<6){
			xx=rdata[timer].xx;
			yy=rdata[timer].yy;
		}
		mousex=ftol(mousex_/(gamenx/gamenx_));
		mousey=ftol(mousey_/(gameny/gameny_));
//		rdata[timer].mx=mousex_;
//		rdata[timer].my=mousey_;
//		rdata[timer].xx=xx;
//		rdata[timer].yy=yy;
//		rdata[timer].flag=rdata[timer].flag;
		WORD f=0;
		f=rdata[timer].flag;
		tamaf= f&0x01;
		movef= f&0x02;
		muteki=f&0x04;
		mugen= f&0x08;
		renmax=f&0x10;
		endf=  f&0x20;
		buki=(int)((f>>6)&0x00000f);
		if(f&0x0400)lockonsu=0; 
	}


}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	if(nState!=WA_INACTIVE) chacha.runf=1; else chacha.runf=0;
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	
}
