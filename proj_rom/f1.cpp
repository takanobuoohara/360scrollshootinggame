// f1.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "proj_rom.h"
#include "f1.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Cf1 �_�C�A���O


Cf1::Cf1(CWnd* pParent /*=NULL*/)
	: CDialog(Cf1::IDD, pParent)
{
	//{{AFX_DATA_INIT(Cf1)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void Cf1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Cf1)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Cf1, CDialog)
	//{{AFX_MSG_MAP(Cf1)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cf1 ���b�Z�[�W �n���h��
int ttt;
BOOL Cf1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ttt=0;
	(CMainFrame*)f1->ShowWindow(SW_HIDE);
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	ShowWindow(SW_SHOWNORMAL);
//	EndDialog(f->DoModal());
	SetTimer(121,100,NULL);
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void Cf1::OnTimer(UINT nIDEvent) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	CDialog::OnTimer(nIDEvent);
}
