// File_Class.cpp: CFile_Class クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "proj_rom.h"
#include "File_Class.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CFile_Class::CFile_Class()
{
	demono=0;
}

CFile_Class::~CFile_Class()
{

}

int CFile_Class::ReplayLoad()
{
	chacha->DoEvent();
	mkdir("replay"); chdir("replay");
	char mydir2[_MAX_DIR];
	_getcwd(mydir2,255);
	CString sl; sl=mydir2; sl+="\\*.rpf";
	CFileDialog dlg(TRUE,"rpf",sl,
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"リプレイファイル(*.rpf)|*.rpf||",
					mf);
	dlg.m_ofn.lpstrTitle="Separate Dimension リプレイファイル読み込み";
	ShowCursor(TRUE);
	if(!chacha->m_bWindow)
		mf->ShowWindow(SW_HIDE);
	int rr = dlg.DoModal();
	chdir("..");
	if(!chacha->m_bWindow){
		mf->ShowWindow(SW_RESTORE);
		chacha->Reset();
	}
	ShowCursor(FALSE);
	if(rr == IDOK)
	{
		CString BMP = dlg.GetPathName();
		CString BMPF = dlg.GetFileName();
		CFile file;
		if(file.Open(BMP,CFile::shareDenyNone | CFile::modeRead,NULL)==TRUE){
			replaysave rsv1;
			file.Read(&rsv1,sizeof replaysave);
			mf->rsv2.nanido=save.nanido;
			mf->rsv2.stage=mf->stage;
			save.nanido=rsv1.nanido;
			mf->stage=rsv1.stage;
			mf->score=rsv1.score;
			for(int ii=0;ii<rsv1.timer;ii++){
				file.Read(&mf->rdata[ii],(sizeof replaydata));
			}
			file.Close();
		}
		return 99;
	}
	return 0;
}

int CFile_Class::EnhLoad()
{
	chacha->DoEvent();
//	mkdir("replay"); 
	chdir("data");
	char mydir2[_MAX_DIR];
	_getcwd(mydir2,255);
	CString sl; sl=mydir2; sl+="\\*.map";
	CFileDialog dlg(TRUE,"map",sl,
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"マップファイル(*.map)|*.map||",
					mf);
	dlg.m_ofn.lpstrTitle="Separate Dimension マップファイル読み込み";
	ShowCursor(TRUE);
	if(!chacha->m_bWindow)
		mf->ShowWindow(SW_HIDE);
	int rr = dlg.DoModal();
	chdir("..");
	if(!chacha->m_bWindow){
		mf->ShowWindow(SW_RESTORE);
		chacha->Reset();
	}
	ShowCursor(FALSE);
	namef="";
	if(rr == IDOK)
	{
		namef=dlg.GetFileName();
		return 99;
	}
	return 0;
}

int CFile_Class::ReplaySave()
{
	chacha->DoEvent();
	mkdir("replay"); chdir("replay");
	char mydir2[_MAX_DIR];
	_getcwd(mydir2,255);
	CString sl;sl.Format("%s\\stage%03d.rpf",mydir2,mf->stage);
	CFileDialog dlg(FALSE,"rpf",sl,
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"リプレイファイル(*.rpf)|*.rpf||",
					mf);
	dlg.m_ofn.lpstrTitle="Separate Dimension リプレイファイル保存";
	dlg.m_ofn.lpstrInitialDir=mydir2;
	ShowCursor(TRUE);
	if(!chacha->m_bWindow)
		mf->ShowWindow(SW_HIDE);
	int rr = dlg.DoModal();
	chdir("..");
	if(!chacha->m_bWindow){
		mf->ShowWindow(SW_RESTORE);
		chacha->Reset();
	}
	ShowCursor(FALSE);
	if(rr == IDOK)
	{
		CString BMP = dlg.GetPathName();
		CString BMPF = dlg.GetFileName();
		CFile file;
		if(file.Open(BMP,CFile::shareDenyNone | CFile::modeWrite | CFile::modeCreate ,NULL)==TRUE){
			replaysave rsv;
			rsv.nanido=save.nanido;
			rsv.stage=mf->stage;
			rsv.timer=mf->timer+2;
			rsv.score=mf->scorerep;
			file.Write(&rsv,sizeof replaysave);
			for(int ii=0;ii<rsv.timer;ii++){
				file.Write(&mf->rdata[ii],(sizeof replaydata));
			}
			file.Close();
		}
	}
	return 0;
}

int CFile_Class::ReplayAutoSave()
{
	if(save.replay){
		mkdir("replay"); chdir("replay");
		char mydir2[_MAX_DIR];
		_getcwd(mydir2,255);
		CString sl;sl.Format("%s\\stage%03d_%s_best.rpf",mydir2,mf->stage,mf->tb[save.nanido]);
		CFile file;
		if(file.Open(sl,CFile::shareDenyNone | CFile::modeWrite | CFile::modeCreate ,NULL)==TRUE){
			replaysave rsv;
			rsv.nanido=save.nanido;
			rsv.stage=mf->stage;
			rsv.timer=mf->timer+2;
			rsv.score=mf->scorerep;
			file.Write(&rsv,sizeof replaysave);
			for(int ii=0;ii<rsv.timer;ii++){
				file.Write(&mf->rdata[ii],(sizeof replaydata));
			}
			file.Close();
		}
		chdir("..");
	}
	return 0;
}

int CFile_Class::Demo()
{
	UnCompress uc;CFile file;
	CString demo;demo.Format("demo%d.rpf",demono);demono++;if(demono==5)demono=0;
	if(file.Open(uc.Open(demo),CFile::shareDenyNone | CFile::modeRead,NULL)==TRUE){
		replaysave rsv1;
		file.Read(&rsv1,sizeof replaysave);
		mf->rsv2.nanido=save.nanido;
		mf->rsv2.stage=mf->stage;
		save.nanido=rsv1.nanido;
		mf->stage=rsv1.stage;
		mf->score=rsv1.score;
		for(int ii=0;ii<rsv1.timer;ii++){
			file.Read(&mf->rdata[ii],(sizeof replaydata));
		}
		file.Close();
		uc.Close();
		mf->demof=1;mf->replay=1;mf->repsf_=1;mf->repf=0;mf->repsf=0;mf->repf_=0;
		mf->GamePlay(mf->stage);
		mf->replay=0;mf->repf=0;mf->repsf=0;mf->repf_=0;
		save.nanido=mf->rsv2.nanido;
		mf->bgm="bgm04.ogg";
		mf->HosiMake();
		mf->demof=0;
		mf->KeyWait();
		return TRUE;
	}
	return FALSE;
}

