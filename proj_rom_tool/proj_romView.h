// proj_romView.h : CProj_romView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJ_ROMVIEW_H__0A48516B_BA98_4922_B608_CB455F3559F2__INCLUDED_)
#define AFX_PROJ_ROMVIEW_H__0A48516B_BA98_4922_B608_CB455F3559F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CProj_romView : public CView
{
protected: // シリアライズ機能のみから作成します。
	CProj_romView();
	DECLARE_DYNCREATE(CProj_romView)

// アトリビュート
public:
	CProj_romDoc* GetDocument();

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CProj_romView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CProj_romView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CProj_romView)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // proj_romView.cpp ファイルがデバッグ環境の時使用されます。
inline CProj_romDoc* CProj_romView::GetDocument()
   { return (CProj_romDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PROJ_ROMVIEW_H__0A48516B_BA98_4922_B608_CB455F3559F2__INCLUDED_)
