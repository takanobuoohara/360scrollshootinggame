#include "MainFrm.h"
#if !defined(AFX_F1_H__A9D8796E_234F_4619_BFAC_EED0D9ED0B3C__INCLUDED_)
#define AFX_F1_H__A9D8796E_234F_4619_BFAC_EED0D9ED0B3C__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// f1.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// Cf1 �_�C�A���O

class Cf1 : public CDialog
{
// �R���X�g���N�V����
public:
	Cf1(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(Cf1)
	enum { IDD = IDD_DIALOG1 };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA

	CFileDialog *f;
	CMainFrame *f1;
// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(Cf1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(Cf1)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_F1_H__A9D8796E_234F_4619_BFAC_EED0D9ED0B3C__INCLUDED_)
