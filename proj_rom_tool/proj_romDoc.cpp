// proj_romDoc.cpp : CProj_romDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "proj_rom.h"

#include "proj_romDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProj_romDoc

IMPLEMENT_DYNCREATE(CProj_romDoc, CDocument)

BEGIN_MESSAGE_MAP(CProj_romDoc, CDocument)
	//{{AFX_MSG_MAP(CProj_romDoc)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProj_romDoc クラスの構築/消滅

CProj_romDoc::CProj_romDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。

}

CProj_romDoc::~CProj_romDoc()
{
}

BOOL CProj_romDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CProj_romDoc シリアライゼーション

void CProj_romDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: この位置に保存用のコードを追加してください。
	}
	else
	{
		// TODO: この位置に読み込み用のコードを追加してください。
	}
}

/////////////////////////////////////////////////////////////////////////////
// CProj_romDoc クラスの診断

#ifdef _DEBUG
void CProj_romDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CProj_romDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProj_romDoc コマンド
