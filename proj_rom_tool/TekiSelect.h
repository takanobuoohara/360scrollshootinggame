#if !defined(AFX_TEKISELECT_H__58241D8D_413D_4421_BD01_6954709F3C5E__INCLUDED_)
#define AFX_TEKISELECT_H__58241D8D_413D_4421_BD01_6954709F3C5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TekiSelect.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CTekiSelect �_�C�A���O

class CTekiSelect : public CDialog
{
// �R���X�g���N�V����
public:
	CTekiSelect(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CTekiSelect)
	enum { IDD = IDD_DIALOG3 };
	CButton	m_sinki;
	CListCtrl	m_tn;
	//}}AFX_DATA

	int ret;
	int input;
	int x,y;
	DWORD *ttee;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTekiSelect)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTekiSelect)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnButton1New();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TEKISELECT_H__58241D8D_413D_4421_BD01_6954709F3C5E__INCLUDED_)
