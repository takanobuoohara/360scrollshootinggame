// proj_rom.h : PROJ_ROM �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#if !defined(AFX_PROJ_ROM_H__7BC18ED9_DEE2_461B_BB18_24E0DDDD8E2E__INCLUDED_)
#define AFX_PROJ_ROM_H__7BC18ED9_DEE2_461B_BB18_24E0DDDD8E2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CProj_romApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� proj_rom.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CProj_romApp : public CWinApp
{
public:
	CProj_romApp();
	virtual ~CProj_romApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CProj_romApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL InitApplication();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
	//{{AFX_MSG(CProj_romApp)
//	afx_msg void OnAppAbout();
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_PROJ_ROM_H__7BC18ED9_DEE2_461B_BB18_24E0DDDD8E2E__INCLUDED_)
