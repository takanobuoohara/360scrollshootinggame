// proj_romDoc.cpp : CProj_romDoc �N���X�̓���̒�`���s���܂��B
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
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProj_romDoc �N���X�̍\�z/����

CProj_romDoc::CProj_romDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B

}

CProj_romDoc::~CProj_romDoc()
{
}

BOOL CProj_romDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CProj_romDoc �V���A���C�[�[�V����

void CProj_romDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���̈ʒu�ɕۑ��p�̃R�[�h��ǉ����Ă��������B
	}
	else
	{
		// TODO: ���̈ʒu�ɓǂݍ��ݗp�̃R�[�h��ǉ����Ă��������B
	}
}

/////////////////////////////////////////////////////////////////////////////
// CProj_romDoc �N���X�̐f�f

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
// CProj_romDoc �R�}���h
