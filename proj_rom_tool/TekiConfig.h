#if !defined(AFX_TEKICONFIG_H__3A3A1818_4480_4C52_999C_EC01053B9D72__INCLUDED_)
#define AFX_TEKICONFIG_H__3A3A1818_4480_4C52_999C_EC01053B9D72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TekiConfig.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CTekiConfig �_�C�A���O

class CTekiConfig : public CDialog
{
// �R���X�g���N�V����
public:
	CTekiConfig(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CTekiConfig)
	enum { IDD = IDD_DIALOG2 };
	CButton	m_haiti;
	CButton	m_del;
	CButton	m_ido;
	CButton	m_tnn;
	CComboBox	m_syurui;
	CEdit	m_memo;
	CListCtrl	m_litiran;
	CListCtrl	m_lsettei;
	//}}AFX_DATA
	int settei,x,y,tekino;
	BOOL newf;
	void ListReDraw(BOOL a=TRUE);
	DWORD *ttee;

	DWORD selectflag[4],itiranflag[4];
	BOOL GetTekiNo(int no,DWORD*tt);
	void SetTekiNo(int no,DWORD*tt,BOOL a);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTekiConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTekiConfig)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton1();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnIdo();
	afx_msg void OnDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TEKICONFIG_H__3A3A1818_4480_4C52_999C_EC01053B9D72__INCLUDED_)
