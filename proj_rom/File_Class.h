// File_Class.h: CFile_Class クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILE_CLASS_H__1165E84E_3CE6_4524_B4E4_348C2558AD0E__INCLUDED_)
#define AFX_FILE_CLASS_H__1165E84E_3CE6_4524_B4E4_348C2558AD0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFile_Class  
{
public:
	CFile_Class();
	virtual ~CFile_Class();

	Cchacha_sdk8_graphic* chacha;
	Cchacha_sdk8_audio* chacha_a;
	Cchacha_sdk8_Input* chacha_i;
	CMainFrame * mf;

	int ReplayLoad();
	int EnhLoad();
	int ReplaySave();
	int ReplayAutoSave();
	int Demo();
	int demono;
};

#endif // !defined(AFX_FILE_CLASS_H__1165E84E_3CE6_4524_B4E4_348C2558AD0E__INCLUDED_)
