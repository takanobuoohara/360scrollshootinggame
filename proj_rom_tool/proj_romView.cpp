// proj_romView.cpp : CProj_romView �N���X�̓���̒�`���s���܂��B
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
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//    ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProj_romView �N���X�̍\�z/����

CProj_romView::CProj_romView()
{
	// TODO: ���̏ꏊ�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B

}

CProj_romView::~CProj_romView()
{
}

BOOL CProj_romView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CProj_romView �N���X�̕`��

void CProj_romView::OnDraw(CDC* pDC)
{
	CProj_romDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
}

/////////////////////////////////////////////////////////////////////////////
// CProj_romView �N���X�̐f�f

#ifdef _DEBUG
void CProj_romView::AssertValid() const
{
	CView::AssertValid();
}

void CProj_romView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CProj_romDoc* CProj_romView::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProj_romDoc)));
	return (CProj_romDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProj_romView �N���X�̃��b�Z�[�W �n���h��
