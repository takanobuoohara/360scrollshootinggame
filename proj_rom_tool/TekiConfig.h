#if !defined(AFX_TEKICONFIG_H__3A3A1818_4480_4C52_999C_EC01053B9D72__INCLUDED_)
#define AFX_TEKICONFIG_H__3A3A1818_4480_4C52_999C_EC01053B9D72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TekiConfig.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CTekiConfig ダイアログ

class CTekiConfig : public CDialog
{
// コンストラクション
public:
	CTekiConfig(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
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

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTekiConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
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
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TEKICONFIG_H__3A3A1818_4480_4C52_999C_EC01053B9D72__INCLUDED_)
