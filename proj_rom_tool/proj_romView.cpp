// proj_romView.cpp : CProj_romView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "proj_rom.h"

#include "proj_romDoc.h"
#include "proj_romView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProj_romView

IMPLEMENT_DYNCREATE(CProj_romView, CView)

BEGIN_MESSAGE_MAP(CProj_romView, CView)
	//{{AFX_MSG_MAP(CProj_romView)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//    この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProj_romView クラスの構築/消滅

CProj_romView::CProj_romView()
{
	// TODO: この場所に構築用のコードを追加してください。

}

CProj_romView::~CProj_romView()
{
}

BOOL CProj_romView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CProj_romView クラスの描画

void CProj_romView::OnDraw(CDC* pDC)
{
	CProj_romDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}

/////////////////////////////////////////////////////////////////////////////
// CProj_romView クラスの診断

#ifdef _DEBUG
void CProj_romView::AssertValid() const
{
	CView::AssertValid();
}

void CProj_romView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CProj_romDoc* CProj_romView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProj_romDoc)));
	return (CProj_romDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProj_romView クラスのメッセージ ハンドラ
