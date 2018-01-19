#if !defined(AFX_MENU_H__97F1ACAC_E7D0_4BB8_87A1_1741F60924B3__INCLUDED_)
#define AFX_MENU_H__97F1ACAC_E7D0_4BB8_87A1_1741F60924B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MENU.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMENU ダイアログ

class CMENU : public CDialog
{
// コンストラクション
public:
	CMENU(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CMENU)
	enum { IDD = IDD_DIALOG1 };
	CEdit	m_4;
	CEdit	m_3;
	CEdit	m_2;
	CEdit	m_1;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMENU)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMENU)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MENU_H__97F1ACAC_E7D0_4BB8_87A1_1741F60924B3__INCLUDED_)
