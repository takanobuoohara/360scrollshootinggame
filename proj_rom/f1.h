#include "MainFrm.h"
#if !defined(AFX_F1_H__A9D8796E_234F_4619_BFAC_EED0D9ED0B3C__INCLUDED_)
#define AFX_F1_H__A9D8796E_234F_4619_BFAC_EED0D9ED0B3C__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// f1.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// Cf1 ダイアログ

class Cf1 : public CDialog
{
// コンストラクション
public:
	Cf1(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(Cf1)
	enum { IDD = IDD_DIALOG1 };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

	CFileDialog *f;
	CMainFrame *f1;
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(Cf1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(Cf1)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_F1_H__A9D8796E_234F_4619_BFAC_EED0D9ED0B3C__INCLUDED_)
