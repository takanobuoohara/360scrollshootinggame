// proj_romView.h : CProj_romView �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJ_ROMVIEW_H__0A48516B_BA98_4922_B608_CB455F3559F2__INCLUDED_)
#define AFX_PROJ_ROMVIEW_H__0A48516B_BA98_4922_B608_CB455F3559F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CProj_romView : public CView
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	CProj_romView();
	DECLARE_DYNCREATE(CProj_romView)

// �A�g���r���[�g
public:
	CProj_romDoc* GetDocument();

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CProj_romView)
	public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷��ۂɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CProj_romView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CProj_romView)
		// ���� -  ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//         ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // proj_romView.cpp �t�@�C�����f�o�b�O���̎��g�p����܂��B
inline CProj_romDoc* CProj_romView::GetDocument()
   { return (CProj_romDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_PROJ_ROMVIEW_H__0A48516B_BA98_4922_B608_CB455F3559F2__INCLUDED_)
