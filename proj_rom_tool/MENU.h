#if !defined(AFX_MENU_H__97F1ACAC_E7D0_4BB8_87A1_1741F60924B3__INCLUDED_)
#define AFX_MENU_H__97F1ACAC_E7D0_4BB8_87A1_1741F60924B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MENU.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CMENU �_�C�A���O

class CMENU : public CDialog
{
// �R���X�g���N�V����
public:
	CMENU(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CMENU)
	enum { IDD = IDD_DIALOG1 };
	CEdit	m_4;
	CEdit	m_3;
	CEdit	m_2;
	CEdit	m_1;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMENU)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CMENU)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MENU_H__97F1ACAC_E7D0_4BB8_87A1_1741F60924B3__INCLUDED_)
