// proj_romDoc.h : CProj_romDoc クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJ_ROMDOC_H__D5111819_20D6_42BC_91D9_597DACFCFF63__INCLUDED_)
#define AFX_PROJ_ROMDOC_H__D5111819_20D6_42BC_91D9_597DACFCFF63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CProj_romDoc : public CDocument
{
protected: // シリアライズ機能のみから作成します。
	CProj_romDoc();
	DECLARE_DYNCREATE(CProj_romDoc)

// アトリビュート
public:

// オペレーション
public:

//オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CProj_romDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CProj_romDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CProj_romDoc)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PROJ_ROMDOC_H__D5111819_20D6_42BC_91D9_597DACFCFF63__INCLUDED_)
