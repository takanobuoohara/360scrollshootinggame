// TekiConfig.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "proj_rom.h"
#include "TekiConfig.h"
#include "TekiSelect.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTekiConfig �_�C�A���O


CTekiConfig::CTekiConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CTekiConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTekiConfig)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CTekiConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTekiConfig)
	DDX_Control(pDX, IDC_CHECK1, m_haiti);
	DDX_Control(pDX, IDC_BUTTON3, m_del);
	DDX_Control(pDX, IDC_BUTTON2, m_ido);
	DDX_Control(pDX, IDC_BUTTON1, m_tnn);
	DDX_Control(pDX, IDC_COMBO1, m_syurui);
	DDX_Control(pDX, IDC_EDIT5, m_memo);
	DDX_Control(pDX, IDC_LIST2, m_litiran);
	DDX_Control(pDX, IDC_LIST1, m_lsettei);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTekiConfig, CDialog)
	//{{AFX_MSG_MAP(CTekiConfig)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON2, OnIdo)
	ON_BN_CLICKED(IDC_BUTTON3, OnDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTekiConfig ���b�Z�[�W �n���h��
#include "tekipart.h"


BOOL CTekiConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	CString s;s="�����ɂ͓G�̖��O�␫���Ȃǂ������������闓�Ńc�[����݂̂̏��ł�";
	m_memo.SetWindowText(s);

	m_syurui.AddString("0:�G����");
	m_syurui.AddString("1:�|���ꂽ�ʒu");
	m_syurui.AddString("2:���f�[�^");
	m_syurui.AddString("3:�~�b�V�����N���A�����P");
	m_syurui.AddString("4�F�y�i���e�B+1");
	m_syurui.AddString("5�F");
	m_syurui.AddString("6�F");
	m_syurui.AddString("7�F");
	m_syurui.AddString("8�F");
	m_syurui.AddString("9�F�Q�[���I�[�o�[");
	for(int i=10;i<410;i++){
		s.Format("%d:%3.2f�b��ɕ\��",i,((float)i-10)/2);
		m_syurui.AddString(s);
	}
	if(newf){
		m_syurui.SetCurSel(0);
		m_ido.EnableWindow(FALSE);
		m_del.EnableWindow(FALSE);
		m_haiti.SetCheck(FALSE);
	}else{
		m_syurui.SetCurSel(save.tekidata[settei][2]&0x7fff);
		m_haiti.SetCheck(save.tekidata[settei][2]>>15);
	}
	m_litiran.ModifyStyle ( 0, LVS_REPORT );
	m_litiran.InsertColumn ( 0, "�ԍ�", LVCFMT_LEFT, 40, 0 );
	m_litiran.InsertColumn ( 1, "�G���",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 2, "�R�����g",     LVCFMT_LEFT  , 200, 0 );
	m_litiran.InsertColumn ( 3, "x,y",     LVCFMT_LEFT  , 30, 0 );
	m_litiran.InsertColumn ( 4, "�\����",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 5, "�T�C�Y",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 6, "���蔻��",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 7, "--",     LVCFMT_LEFT  , 0, 0 );
	m_litiran.InsertColumn ( 8, "��]���x",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 9, "��",     LVCFMT_LEFT  , 20, 0 );
	m_litiran.InsertColumn ( 10, "--",     LVCFMT_LEFT  , 0, 0 );
	m_litiran.InsertColumn ( 11, "�n�`",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 12, "--",     LVCFMT_LEFT  , 0, 0 );
	m_litiran.InsertColumn ( 13, "--",     LVCFMT_LEFT  , 0, 0 );
	m_litiran.InsertColumn ( 14, "���@�ǔ�",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 15, "�n�\�ړ�",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 16, "�ެ���",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 17, "�����ڕ�",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 18, "���x",     LVCFMT_LEFT  , 30, 0 );
	m_litiran.InsertColumn ( 19, "����",     LVCFMT_LEFT  , 30, 0 );
	m_litiran.InsertColumn ( 20, "�ō����x",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 21, "��",     LVCFMT_LEFT  , 20, 0 );
	m_litiran.InsertColumn ( 22, "�e�ԍ�",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 23, "�U������",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 24, "�A�ː�",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 25, "�A�ˊԊu",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 26, "�U���Ԋu",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 27, "��",     LVCFMT_LEFT  , 20, 0 );
	m_litiran.InsertColumn ( 28, "�����",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 29, "������",     LVCFMT_LEFT  , 50, 0 );
	m_litiran.InsertColumn ( 30, "�h��ϐ�",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 31, "�ϋv�{��",     LVCFMT_LEFT  , 60, 0 );
	m_litiran.InsertColumn ( 32, "��",     LVCFMT_LEFT  , 20, 0 );
	m_litiran.InsertColumn ( 33, "���ʉ�",     LVCFMT_LEFT  , 50, 0 );

	DWORD dwExStyle = ListView_GetExtendedListViewStyle( m_litiran.m_hWnd );
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_INFOTIP;
	ListView_SetExtendedListViewStyle( m_litiran.m_hWnd , dwExStyle );

	m_lsettei.ModifyStyle ( 0, LVS_REPORT );
	m_lsettei.InsertColumn ( 0, "�ԍ�", LVCFMT_LEFT, 40, 0 );
	m_lsettei.InsertColumn ( 1, "�G���",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 2, "�R�����g",     LVCFMT_LEFT  , 200, 0 );
	m_lsettei.InsertColumn ( 3, "x,y",     LVCFMT_LEFT  , 30, 0 );
	m_lsettei.InsertColumn ( 4, "�\����",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 5, "�T�C�Y",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 6, "���蔻��",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 7, "�ڒn���]",     LVCFMT_LEFT  , 0, 0 );
	m_lsettei.InsertColumn ( 8, "��]���x",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 9, "��",     LVCFMT_LEFT  , 20, 0 );
	m_lsettei.InsertColumn ( 10, "�n�`��",     LVCFMT_LEFT  , 0, 0 );
	m_lsettei.InsertColumn ( 11, "�n�`",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 12, "�d��",     LVCFMT_LEFT  , 0, 0 );
	m_lsettei.InsertColumn ( 13, "�ڒn�ړ�",     LVCFMT_LEFT  , 0, 0 );
	m_lsettei.InsertColumn ( 14, "���@�ǔ�",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 15, "�n�\�ړ�",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 16, "�ެ���",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 17, "�����ڕ�",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 18, "���x",     LVCFMT_LEFT  , 30, 0 );
	m_lsettei.InsertColumn ( 19, "����",     LVCFMT_LEFT  , 30, 0 );
	m_lsettei.InsertColumn ( 20, "�ō����x",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 21, "��",     LVCFMT_LEFT  , 20, 0 );
	m_lsettei.InsertColumn ( 22, "�e�ԍ�",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 23, "�U������",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 24, "�A�ː�",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 25, "�A�ˊԊu",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 26, "�U���Ԋu",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 27, "��",     LVCFMT_LEFT  , 20, 0 );
	m_lsettei.InsertColumn ( 28, "�����",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 29, "������",     LVCFMT_LEFT  , 50, 0 );
	m_lsettei.InsertColumn ( 30, "�h��ϐ�",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 31, "�ϋv�{��",     LVCFMT_LEFT  , 60, 0 );
	m_lsettei.InsertColumn ( 32, "��",     LVCFMT_LEFT  , 20, 0 );
	m_lsettei.InsertColumn ( 33, "���ʉ�",     LVCFMT_LEFT  , 50, 0 );

	dwExStyle = ListView_GetExtendedListViewStyle( m_lsettei.m_hWnd );
	dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_INFOTIP;
	ListView_SetExtendedListViewStyle( m_lsettei.m_hWnd , dwExStyle );
	if(newf){
		selectflag[0]=selectflag[1]=selectflag[2]=selectflag[3]=0;
		itiranflag[0]=itiranflag[1]=itiranflag[2]=itiranflag[3]=0xffffffff;
		tekino=settei;
		s.Format("�ĂԓG�ԍ�:�Ȃ�",tekino); m_tnn.SetWindowText(s);
	}else{
		selectflag[0]=save.tekidata[settei][4];itiranflag[0]=selectflag[0]^0xffffffff;
		selectflag[1]=save.tekidata[settei][5];itiranflag[1]=selectflag[1]^0xffffffff;
		selectflag[2]=save.tekidata[settei][6];itiranflag[2]=selectflag[2]^0xffffffff;
		selectflag[3]=save.tekidata[settei][7];itiranflag[3]=selectflag[3]^0xffffffff;
		tekino=save.tekidata[settei][3];
		if(tekino){
			s.Format("�ĂԓG�ԍ�:%d",tekino); m_tnn.SetWindowText(s);
		}else{
			s.Format("�ĂԓG�ԍ�:�Ȃ�",tekino); m_tnn.SetWindowText(s);
		}
	}
	ListReDraw();
	if(newf)
		s.Format("�G�f�[�^�쐬:�G�ԍ�:%3d",tekino);
	else
		s.Format("�G�f�[�^�ҏW:�G�ԍ�:%3d",tekino);
	SetWindowText(s);
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

BOOL CTekiConfig::GetTekiNo(int no,DWORD*tt)
{
//	DWORD pp=1;
	int n=no/32;
	DWORD nn=(DWORD)1L<<((DWORD)no%32);
	if( tt[n]&nn ) return TRUE;
	return FALSE;
}

void CTekiConfig::SetTekiNo(int no,DWORD*tt,BOOL a)
{
	DWORD pp=1;
	int n=no/32;
	if(a)
		tt[n]&=0xffffffff^(pp<<((DWORD)no%32));
	else
		tt[n]|=(pp<<((DWORD)no%32));
}

char koumoku01[][200]={{"�n��p"},{"��p"},{"�U��p"},{"�h��p"},{"�~�T�C���⋋"},{"�z�[�~���O�⋋"},{"�G�l���M�[�⋋"},{"�_���[�W�����i���y�A�j"},{"���g�p"}};
char koumoku04[][200]={{"���@"},{"����"},{"��"},{"�E��"},{"��"},{"�E��"},{"�E"},{"����"},{"��"},{"�E��"},{"����"}};
char koumoku07[][200]={{"���]�Ȃ�"},{"�X�^�[�g���ɏ�ɒn�`������ƁA�L�������㉺���]����B"},{"�X�^�[�g���ɍ��ɒn�`������ƁA�L���������E���]����B"}};
//char koumoku09[][200]={{"�n�`�Ɋ֌W�Ȃ��ړ��i�ђʁj"},{"���ւ̈ړ���ɒn�`������ƈړ������𔽓]"},{"���ւ̈ړ���ɒn�`������ƈړ������𔽓]�{�L���������E���]"},{"�ړ���ɒn�`������ƁA����ȏ�i�߂Ȃ�"}};
char koumoku10[][200]={{"�ђ�"},{"���]"},{"��~"}};
//char koumoku11[][200]={{"�d�͂������Ȃ�"},{"��������ɒn�`���Ȃ��ƁA�d�͂̉e���Œn�`������܂ŗ�����B�����Ă�Ԃ́A�㉺���E�����Ȃ��B�n�`�ɗ��������ɍ���"},{"��������ɒn�`���Ȃ��ƁA�d�͂̉e���Œn�`������܂ŗ�����B�㉺�͓����Ȃ������E�ɂ͓�����B�n�`�ɗ��������ɍ���"},{"�����ɒn�`���Ȃ��ƁA�d�͂̉e���Œn�`������܂ŗ�����B�����Ă�Ԃ́A�㉺���E�����Ȃ��B�n�`�ɗ��������ɍ���"},{"�����ɒn�`���Ȃ��ƁA�d�͂̉e���Œn�`������܂ŗ�����B�㉺�͓����Ȃ������E�ɂ͓�����B�n�`�ɗ��������ɍ���"}};
//char koumoku12[][200]={{"���ꂪ�����Ă��ړ��ł���"},{"���E�ɐڒn�ł���n�`���Ȃ��ƈړ��ł��Ȃ�"},{"�㉺�ɐڒn�ł���n�`���Ȃ��ƈړ��ł��Ȃ�"}};
char koumoku13[][200]={{"���Ȃ�"},{"����"},{"������"},{"��苗��"}};
char koumoku14[][200]={{"���Ȃ�"},{"�n�\�E"},{"�n�\��"}};
char koumoku15[][200]={{"���Ȃ�"},{"10%"},{"20%"},{"30%"},{"40%"},{"50%"},{"60%"},{"70%"},{"80%"},{"90%"},{"��"},{"10%�{"},{"20%�{"},{"30%�{"},{"40%�{"},{"50%�{"},{"60%�{"},{"70%�{"},{"80%�{"},{"90%�{"},{"��{"}};
char koumoku16[][200]={{"���@"},{"����"},{"��"},{"�E��"},{"��"},{"-"},{"�E"},{"����"},{"��"},{"�E��"},{"-"}};
char koumoku18[][200]={{"���Ȃ�"},{"1�{����"},{"2�{����"},{"3�{����"}};
char koumoku19[][200]={{"������"},{"1.000"},{"2.000"},{"3.000"},{"4.000"},{"5.000"},{"6.000"},{"7.000"},{"8.000"},{"9.000"}};
char koumoku21[][200]={{"���@"},{"����"},{"��"},{"�E��"},{"��"},{"�E��"},{"�E"},{"����"},{"��"},{"�E��"},{"����"}};
char koumoku25[][200]={{"��"},{"�㉺"},{"�㍶�E"},{"�S����"}};
char koumoku26[][200]={{"���@"},{"����"},{"��"},{"�E��"},{"��"},{"�E��"},{"�E"},{"����"},{"��"},{"�E��"},{"����"}};
char koumoku27[][200]={{"-"},{"�}�V���K�����QWAY"},{"�z�[�~���O"},{"�}�V���K�����QWAY�@���@�z�[�~���O"},{"�~�T�C�����@���i����"},{"�}�V���K�����QWAY�@���@�~�T�C�����@���i�����j"},{"�z�[�~���O�@���@�~�T�C�����@���i�����j"},{"�S��"}};
char koumoku28[][200]={{"1�{"},{"����"},{"2�{"},{"3�{"},{"4�{"},{"5�{"},{"6�{"},{"7�{"},{"8�{"},{"9�{"}};

void CTekiConfig::ListReDraw(BOOL a)
{
	char buf[512]; LV_ITEM LvItem;	int idItem;CString sfl;
	m_lsettei.DeleteAllItems();
	if(a) m_litiran.DeleteAllItems();
//	for(int i=0;i<5;i++){
		for(int j=0;j<128;j++){
			if(GetTekiNo(j,itiranflag)){
				if(a){
					sfl.Format("%03d",tekipart[j][0]);	strcpy(buf,sfl);	LvItem.pszText = buf;
					LvItem.iItem = m_litiran.GetItemCount( );	LvItem.mask = LVIF_TEXT | LVIF_STATE;	LvItem.stateMask = LVIS_FOCUSED | LVIS_SELECTED;LvItem.state = 0;	LvItem.iSubItem = 0;	LvItem.cchTextMax = 512;//strlen(buf);
					idItem = m_litiran.InsertItem(&LvItem);
					LvItem.iItem = idItem;
					strcpy(buf,koumoku01[tekipart[j][1]]);	LvItem.iSubItem = 1;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%s",partcomment[j]);	strcpy(buf,sfl);LvItem.iSubItem = 2;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%d,%d",tekipart[j][2],tekipart[j][3]);	strcpy(buf,sfl);LvItem.iSubItem = 3;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][4],koumoku04[tekipart[j][4]]);strcpy(buf,sfl);	LvItem.iSubItem = 4;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][5]);	strcpy(buf,sfl);LvItem.iSubItem = 5;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][6]);	strcpy(buf,sfl);LvItem.iSubItem = 6;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][7],koumoku07[tekipart[j][7]]);strcpy(buf,sfl);	LvItem.iSubItem = 7;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][8]);	strcpy(buf,sfl);LvItem.iSubItem = 8;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
//					sfl.Format("%02d:%s",tekipart[j][9],koumoku07[tekipart[j][9]]);strcpy(buf,sfl);LvItem.iSubItem = 10;
//					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][10],koumoku10[tekipart[j][10]]);strcpy(buf,sfl);LvItem.iSubItem = 11;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
//					sfl.Format("%02d:%s",tekipart[j][11],koumoku11[tekipart[j][11]]);strcpy(buf,sfl);LvItem.iSubItem = 12;
//					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
//					sfl.Format("%02d:%s",tekipart[j][12],koumoku12[tekipart[j][12]]);strcpy(buf,sfl);LvItem.iSubItem = 13;
//					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][13],koumoku13[tekipart[j][13]]);strcpy(buf,sfl);LvItem.iSubItem = 14;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][14],koumoku14[tekipart[j][14]]);strcpy(buf,sfl);LvItem.iSubItem = 15;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][15],koumoku15[tekipart[j][15]]);strcpy(buf,sfl);LvItem.iSubItem = 16;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][16],koumoku16[tekipart[j][16]]);strcpy(buf,sfl);LvItem.iSubItem = 17;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][17]);	strcpy(buf,sfl);LvItem.iSubItem = 18;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][18],koumoku18[tekipart[j][18]]);strcpy(buf,sfl);LvItem.iSubItem = 19;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][19],koumoku19[tekipart[j][19]]);strcpy(buf,sfl);LvItem.iSubItem = 20;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][20]);	strcpy(buf,sfl);LvItem.iSubItem = 22;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][21],koumoku21[tekipart[j][21]]);strcpy(buf,sfl);LvItem.iSubItem = 23;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][22]);	strcpy(buf,sfl);LvItem.iSubItem = 24;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][23]);	strcpy(buf,sfl);LvItem.iSubItem = 25;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][24]);	strcpy(buf,sfl);LvItem.iSubItem = 26;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][25],koumoku25[tekipart[j][25]]);strcpy(buf,sfl);LvItem.iSubItem = 28;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][26],koumoku26[tekipart[j][26]]);strcpy(buf,sfl);LvItem.iSubItem = 29;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][27],koumoku27[tekipart[j][27]]);strcpy(buf,sfl);LvItem.iSubItem = 30;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d:%s",tekipart[j][28],koumoku28[tekipart[j][28]]);strcpy(buf,sfl);LvItem.iSubItem = 31;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
					sfl.Format("%02d",tekipart[j][29]);	strcpy(buf,sfl);LvItem.iSubItem = 33;
					LvItem.pszText = buf;	m_litiran.SetItem(&LvItem);
				}
			}else{
				sfl.Format("%03d",tekipart[j][0]);	strcpy(buf,sfl);	LvItem.pszText = buf;
				LvItem.iItem = m_lsettei.GetItemCount( );	LvItem.mask = LVIF_TEXT | LVIF_STATE;	LvItem.stateMask = LVIS_FOCUSED | LVIS_SELECTED;LvItem.state = 0;	LvItem.iSubItem = 0;	LvItem.cchTextMax = 512;//strlen(buf);
				idItem = m_lsettei.InsertItem(&LvItem);
				LvItem.iItem = idItem;
				strcpy(buf,koumoku01[tekipart[j][1]]);	LvItem.iSubItem = 1;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%s",partcomment[j]);	strcpy(buf,sfl);LvItem.iSubItem = 2;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%d,%d",tekipart[j][2],tekipart[j][3]);	strcpy(buf,sfl);LvItem.iSubItem = 3;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][4],koumoku04[tekipart[j][4]]);strcpy(buf,sfl);	LvItem.iSubItem = 4;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][5]);	strcpy(buf,sfl);LvItem.iSubItem = 5;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][6]);	strcpy(buf,sfl);LvItem.iSubItem = 6;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][7],koumoku07[tekipart[j][7]]);strcpy(buf,sfl);	LvItem.iSubItem = 7;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][8]);	strcpy(buf,sfl);LvItem.iSubItem = 8;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
//				sfl.Format("%02d:%s",tekipart[j][9],koumoku07[tekipart[j][9]]);strcpy(buf,sfl);LvItem.iSubItem = 10;
//				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][10],koumoku10[tekipart[j][10]]);strcpy(buf,sfl);LvItem.iSubItem = 11;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
//				sfl.Format("%02d:%s",tekipart[j][11],koumoku11[tekipart[j][11]]);strcpy(buf,sfl);LvItem.iSubItem = 12;
//				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
//				sfl.Format("%02d:%s",tekipart[j][12],koumoku12[tekipart[j][12]]);strcpy(buf,sfl);LvItem.iSubItem = 13;
//				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][13],koumoku13[tekipart[j][13]]);strcpy(buf,sfl);LvItem.iSubItem = 14;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][14],koumoku14[tekipart[j][14]]);strcpy(buf,sfl);LvItem.iSubItem = 15;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][15],koumoku15[tekipart[j][15]]);strcpy(buf,sfl);LvItem.iSubItem = 16;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][16],koumoku16[tekipart[j][16]]);strcpy(buf,sfl);LvItem.iSubItem = 17;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][17]);	strcpy(buf,sfl);LvItem.iSubItem = 18;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][18],koumoku18[tekipart[j][18]]);strcpy(buf,sfl);LvItem.iSubItem = 19;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][19],koumoku19[tekipart[j][19]]);strcpy(buf,sfl);LvItem.iSubItem = 20;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][20]);	strcpy(buf,sfl);LvItem.iSubItem = 22;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][21],koumoku21[tekipart[j][21]]);strcpy(buf,sfl);LvItem.iSubItem = 23;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][22]);	strcpy(buf,sfl);LvItem.iSubItem = 24;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][23]);	strcpy(buf,sfl);LvItem.iSubItem = 25;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][24]);	strcpy(buf,sfl);LvItem.iSubItem = 26;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][25],koumoku25[tekipart[j][25]]);strcpy(buf,sfl);LvItem.iSubItem = 28;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][26],koumoku26[tekipart[j][26]]);strcpy(buf,sfl);LvItem.iSubItem = 29;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][27],koumoku27[tekipart[j][27]]);strcpy(buf,sfl);LvItem.iSubItem = 30;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d:%s",tekipart[j][28],koumoku28[tekipart[j][28]]);strcpy(buf,sfl);LvItem.iSubItem = 31;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
				sfl.Format("%02d",tekipart[j][29]);	strcpy(buf,sfl);LvItem.iSubItem = 33;
				LvItem.pszText = buf;	m_lsettei.SetItem(&LvItem);
			}
		}
//	}

	//�`��
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->Draw();
}

void CTekiConfig::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	save.tekidata[settei][0]=x;
	save.tekidata[settei][1]=y;
	save.tekidata[settei][3]=tekino;
	save.tekidata[settei][2]=(DWORD)(m_syurui.GetCurSel())+((DWORD)m_haiti.GetCheck()<<15);
	save.tekidata[settei][4]=selectflag[0];
	save.tekidata[settei][5]=selectflag[1];
	save.tekidata[settei][6]=selectflag[2];
	save.tekidata[settei][7]=selectflag[3];
	CString s;m_memo.GetWindowText(s);
	strcpy(cm[settei].c,s);
	if(newf) save.tekikazu++;
	CDialog::OnOK();
}

void CTekiConfig::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	int Lindex=-1,ff=0;
	Lindex=m_litiran.GetNextItem(Lindex,LVNI_ALL |LVNI_SELECTED);
	if(Lindex==-1) return;
	
	SetTekiNo(atoi(m_litiran.GetItemText(Lindex,0)),selectflag,FALSE);
	SetTekiNo(atoi(m_litiran.GetItemText(Lindex,0)),itiranflag,TRUE);
	m_litiran.DeleteItem(Lindex);
	ttee[0]=selectflag[0];
	ttee[1]=selectflag[1];
	ttee[2]=selectflag[2];
	ttee[3]=selectflag[3];
	ListReDraw(FALSE);	
	*pResult = 0;
}

void CTekiConfig::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	int Lindex=-1,ff=0;
	Lindex=m_lsettei.GetNextItem(Lindex,LVNI_ALL |LVNI_SELECTED);
	if(Lindex==-1) return;
	
	SetTekiNo(atoi(m_lsettei.GetItemText(Lindex,0)),selectflag,TRUE);
	SetTekiNo(atoi(m_lsettei.GetItemText(Lindex,0)),itiranflag,FALSE);
//	m_litiran.DeleteItem(Lindex);
	ttee[0]=selectflag[0];
	ttee[1]=selectflag[1];
	ttee[2]=selectflag[2];
	ttee[3]=selectflag[3];
	ListReDraw();	
	
	*pResult = 0;
}

void CTekiConfig::OnButton1() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CTekiSelect a;
	a.input=settei;
	ShowWindow(SW_HIDE);
	if(a.DoModal()==IDOK){
		tekino=a.ret;
		CString s;s.Format("�ĂԓG�ԍ�:%d",tekino); m_tnn.SetWindowText(s);
	}
	ShowWindow(SW_SHOW);
}

void CTekiConfig::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->Draw();
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	
}

void CTekiConfig::OnIdo() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	EndDialog(155);	
}

void CTekiConfig::OnDel() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	EndDialog(444);	
	
}
