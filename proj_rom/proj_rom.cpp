// proj_rom.cpp : �A�v���P�[�V�����p�N���X�̋@�\��`���s���܂��B
//

#include "stdafx.h"
#include "proj_rom.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProj_romApp

BEGIN_MESSAGE_MAP(CProj_romApp, CWinApp)
	//{{AFX_MSG_MAP(CProj_romApp)
//	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
	// �W���̃t�@�C����{�h�L�������g �R�}���h
//	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProj_romApp �N���X�̍\�z

CProj_romApp::CProj_romApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
	m_pMainWnd=NULL;
}

CProj_romApp::~CProj_romApp()
{
}
/////////////////////////////////////////////////////////////////////////////
// �B��� CProj_romApp �I�u�W�F�N�g

CProj_romApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CProj_romApp �N���X�̏�����
CMainFrame *theMain;
void *Mutex;
BOOL CProj_romApp::InitInstance()
{
//_CrtSetBreakAlloc(1815);
	// �W���I�ȏ���������
	// ���������̋@�\���g�p�����A���s�t�@�C���̃T�C�Y��������
	// ��������Έȉ��̓���̏��������[�`���̒�����s�K�v�Ȃ���
	// ���폜���Ă��������B

	// �ݒ肪�ۑ�����鉺�̃��W�X�g�� �L�[��ύX���܂��B
	// TODO: ���̕�������A��Ж��܂��͏����ȂǓK�؂Ȃ��̂�
	// �ύX���Ă��������B
//	SetRegistryKey(_T("proj_rom"));

//	LoadStdProfileSettings();  // �W���� INI �t�@�C���̃I�v�V���������[�ނ��܂� (MRU ���܂�)
//	AfxEnableControlContainer();
#ifdef _AFXDLL
	Enable3dControls();			// ���L DLL �̒��� MFC ���g�p����ꍇ�ɂ͂������Ăяo���Ă��������B
#else
	Enable3dControlsStatic();	// MFC �ƐÓI�Ƀ����N���Ă���ꍇ�ɂ͂������Ăяo���Ă��������B
#endif

	// �A�v���P�[�V�����p�̃h�L�������g �e���v���[�g��o�^���܂��B�h�L�������g �e���v���[�g
	//  �̓h�L�������g�A�t���[�� �E�B���h�E�ƃr���[���������邽�߂ɋ@�\���܂��B
	Mutex=NULL;
	Mutex = CreateMutex(NULL,FALSE,"sd_chachamutex");
            //????????�̓��j�[�N�Ȗ��O���w�肷�� 
        DWORD Status = GetLastError(); 
        if(Mutex == NULL){ 
            //CleateMutex���s 
            exit(-1); 
        } 
        if(Status == ERROR_ALREADY_EXISTS){ 
            //���łɋN������Ă���
            ReleaseMutex(Mutex); 
	      ::MessageBox(NULL,"Separete Dimension�ݒ�܂��̓Q�[���{�̂��N�����Ă܂��B\n�����N�����邱�Ƃ͏o���܂���̂Ő�ɕ��Ă��������B","�V�X�e�����",MB_ICONERROR);
            exit(-1); 
        } 

	CString s;
#if SD_BETA
	s="Separate Dimension �`ꡂ��Ȃ鎞�̗z���` ��-2�� 2005.08.28 1:29";
#else
	s="Separate Dimension �`ꡂ��Ȃ鎞�̗z���` + �Ǝ��}�b�v�v���C�� 2007.02.06 Version 1.00e PrePrayer Power Soft";
#endif
	if(m_pMainWnd==NULL){
		m_pMainWnd = new CMainFrame;
		theMain = (CMainFrame*)m_pMainWnd;
		if(!((CMainFrame*)m_pMainWnd)->Create(s))return FALSE;
		((CMainFrame*)m_pMainWnd)->ShowWindow(m_nCmdShow);
		((CMainFrame*)m_pMainWnd)->ValidateRect(0);
		((CMainFrame*)m_pMainWnd)->UpdateWindow();
		if(	((CMainFrame*)m_pMainWnd)->Init() )
			((CMainFrame*)m_pMainWnd)->GameProc();
		((CMainFrame*)m_pMainWnd)->Close();
	}
	if(m_pMainWnd!=NULL){//
//		((CMainFrame*)m_pMainWnd)->OnDestroy();
		delete m_pMainWnd;
		m_pMainWnd=NULL;
	}

	ReleaseMutex(Mutex);
	Mutex=NULL;
//	exit(0); //�I���
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�����̃o�[�W�������Ŏg���� CAboutDlg �_�C�A���O
/*
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard ���z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	//{{AFX_MSG(CAboutDlg)
		// ���b�Z�[�W �n���h���͂���܂���B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// ���b�Z�[�W �n���h���͂���܂���B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// �_�C�A���O�����s���邽�߂̃A�v���P�[�V���� �R�}���h
void CProj_romApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CProj_romApp ���b�Z�[�W �n���h��

*/

int CProj_romApp::ExitInstance() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	if(Mutex)
		ReleaseMutex(Mutex);
	Mutex=NULL;
	
	return CWinApp::ExitInstance();
}

BOOL CProj_romApp::InitApplication() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	
	return CWinApp::InitApplication();
}
