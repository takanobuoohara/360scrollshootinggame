// MainFrm.h : CMainFrame クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__CB74BAD3_A130_4BC8_BC2C_571FF49FA2E7__INCLUDED_)
#define AFX_MAINFRM_H__CB74BAD3_A130_4BC8_BC2C_571FF49FA2E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
public:	
//protected: // シリアライズ機能のみから作成します。
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	BOOL Create(const CString& sTitle);
	void Init();
	void GameProc();
	void Idou();
	CMenu nn;
	void OnSaveBMP(CString s0) ;
	int mode,masu;
	void Draw();
	int iti;

	DWORD ttee[4];
// アトリビュート
public:

// オペレーション
public:

	BOOL fs;
	int mousex,mousey;
	float gamenx,gameny;
	float xx,yy,xxs,yys;
	BOOL movef,endf;
	float kasoku,gensoku,juryoku,limit;

	Cchacha_sdk8_graphic chacha;
	Cchacha_sdk8_audio chacha_a;
	int tekisu;

	BOOL GetTekiNo(int teki,int no);
	BOOL GetTekiNo2(int no);
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成されたメッセージ マップ関数
//protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEnd();
	afx_msg void OnLoad();
	afx_msg void OnSave();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void Onsyoki();
	afx_msg void OnMenu();
	afx_msg void On1();
	afx_msg void On2();
	afx_msg void On4();
	afx_msg void On8();
	afx_msg void On16();
	afx_msg void OnTeki1();
	afx_msg void OnTeki2();
	afx_msg void OnClear();
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAINFRM_H__CB74BAD3_A130_4BC8_BC2C_571FF49FA2E7__INCLUDED_)
