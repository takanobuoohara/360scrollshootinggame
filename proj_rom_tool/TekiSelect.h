#if !defined(AFX_TEKISELECT_H__58241D8D_413D_4421_BD01_6954709F3C5E__INCLUDED_)
#define AFX_TEKISELECT_H__58241D8D_413D_4421_BD01_6954709F3C5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TekiSelect.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CTekiSelect ダイアログ

class CTekiSelect : public CDialog
{
// コンストラクション
public:
	CTekiSelect(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTekiSelect)
	enum { IDD = IDD_DIALOG3 };
	CButton	m_sinki;
	CListCtrl	m_tn;
	//}}AFX_DATA

	int ret;
	int input;
	int x,y;
	DWORD *ttee;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTekiSelect)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
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
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TEKISELECT_H__58241D8D_413D_4421_BD01_6954709F3C5E__INCLUDED_)
