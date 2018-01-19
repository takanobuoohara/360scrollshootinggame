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
public:	
	//生成
	BOOL Create(const CString& sTitle);
	BOOL Init();
	void Close();
	//ゲームメイン
	void GameProc();
	//ゲーム開始
	BOOL GamePlay(int no);
	int PausePlay();
	//ロゴ
	void LogoPlay();
	static UINT oggplay(LPVOID pParam );
	static UINT ReplaySave(LPVOID pParam );
	static UINT Loading(LPVOID pParam );
	void Loadingmain();
	BOOL loadf,loadf2;
	void HosiOkuMake();
	void HosiOkuProc();
	void HosiOkuDraw();
	//タイトル
	BOOL TitlePlay(int ka);
	void HosiMake();
	//ランキング
	void RankigPlay();
	void motanime();
	void motDraw();
	//裏技表示
	void UraInfo();
	//エンディング
	void EndPlay();

	DWORD GetTiming(DWORD);
	void Naido(int);
	
	void KeyWait();
	CString tb[5];
	int demof;

	CString bgm;
	BOOL bgmth;
	//キャラ敵移動処理
	void Idou();
	void Draw();
	BOOL GetTekiNo(int teki,DWORD no);
	BOOL CheckMap(float x,float y,BOOL f=FALSE);
	BOOL CheckMap_(float x,float y,BOOL f=TRUE);
	int CheckMap2(float x,float y);
	void Kasoku(int i,int j,float& x,float& y);
	void Tekitama(int no,float xx,float yy,float tss,int t);
	void Tekihaiti(int tekino,int tekidatano,BOOL yomi=TRUE);
	//ステージデータ読み込み
	void LoadStage(int& no);
	void InitMiStart(CString s,BOOL f=FALSE);
	void AnimeMiStart();
	void DrawMiStart();
	void InitMiStart2(CString s,BOOL f=FALSE);
	void AnimeMiStart2();
	void DrawMiStart2();
	BOOL tekikill(int& ti);
	void input();

	void Copy_tekidata(int &ti);
	void Copy_tekitama(int &ti);
	void Copy_tama(int &ti);
	void Copy_lockon(int &ti);



// アトリビュート
public:

// オペレーション
public:
	datasave ds;
	Cchacha_sdk8_graphic chacha;
	Cchacha_sdk8_audio chacha_a;
	Cchacha_sdk8_Input chacha_i;

	BOOL fs;
	int mousex,mousey;
	float gamenx,gameny;
	float gamenx2,gameny2;
	float gamenx5,gameny5;
	float gamenx_,gameny_;
	float gamenx2_,gameny2_;
	float gamenx5_,gameny5_;
	float xx,yy,xxs,yys;
	BOOL movef,endf,endff;
	float kasoku,gensoku,juryoku,limit;
	BOOL atari;

	int tamacnt;
	BOOL tamaf;
	int tamasu;

	int lockonsu;//,lockten,lockten2,lockten_,lockten2_;
	int motsu,motcnt;
	int lockon[1000];
	float tama[5000][10];//0:x 1:y 2:xx 3:yy 4:種類 5:shita 6:タイマー 7:アニメタイマー 8:ロックオンの敵番号 9:遅延
	int mot[5000][7]; //0:x 1:y 2:no 3:no2 4:xx 5:yy 6:遅延
	float tekitama[5000][11];//0:x 1:y 2:xx 3:yy 4:加速減速 5:弾種類 6:遅延 7:カウント 8:shita 9:alpha 10:敵番号
	float tekidata[1000][40];//0:x 1:y 2:xx 3:yy 4:shita 5:hp 6:破壊時 7:敵番号 8:回転　9,10,11,12,13,14:カウントフラグ 15:ディレイ 16:間隔
	DWORD tekidata2[1000][4];//0,1,2,3 パーツ描画用
	float hit[3000][2];//0:上への位置 1:フェードのα値
	int haikei[256*8+1][5]; //1:x 2:y 3:奥 4:色
	int hitcnt;

	//弾関連,システム関連
	int jikitama[5];//数:-1の時無制限  0:マシンガン 1:ホーミング 2:機雷 3:ミサイル 4:2WAY
	int hitp[5][2];//撃った数、当たった数
	int timer,gameover,gameovercnt;
	int score,killsec;//得点
	int missioncnt,penaltycnt,emptyanime;//ミッションカウント
	float sabun;
	DWORD timen;
	float mousex_,mousey_;
	int stage;
	float mapx32,mapy32;
	int  mapx32_,mapy32_;

	//裏技関連
	int fps;
	int muteki;
	int mugen;
	int renmax;
	int stagemax;
	int urawaza;
	CString uraura;
	int clearf;
	int romrom;

	//MISSION xxx START COMPLETEの部分のアニメ関連
	float mistart[800][5];//0:中心からの位置 1:θ 2:遅延 3:x 4:y
	char mistartc[800];
	int mistartc2[800];
	int mistartf,mistartsu,mistartx,mistarty;
	int mistartmode,mistartmode2,mistartmode3,mistartmodef;
	CString mistarts;
	float mistartA[800][5];//0:中心からの位置 1:θ 2:遅延 3:x 4:y
	char mistartcA[800];
	int mistartc2A[800];
	int mistartfA,mistartsuA,mistartxA,mistartyA;
	int mistartmodeA,mistartmode2A,mistartmode3A,mistartmodefA,mistartmode4A;
	CString mistartsA;
	int b1,b2,b3,b4,b5,m1,m2,m3;
	CString s1,s2,s3,s4,s5;

	replaysave rsv1,rsv2;
//	replaydata_ rdata_[60*60*99+2];
	replaydata rdata[60*60*99+2];
	int replay,scorerep,repf,repsf,repf_,repsf_;
	//敵,弾関連
	int tekitamasu,tekitamaf,tekitamano;
	int tekisu;
	//0:x 1:y 2:xx 3:yy 4:shita 5:hp 6:破壊時 7:敵番号 8,9,10,11,12,13:カウントフラグ
	//8:回転速度 9:前回の壁 10:移動向き 11:移動向き2 12:ジャンプ中
	//14,15:ディレイ 16:間隔 17:攻撃shita 18:盾shita
	//19:盾開始shita 20;終了shita 21:盾開始shita 22;終了shita 23:盾開始shita 24;終了shita 25:-- 26:盾数 
	//27,28,29,30,31:武器 32:MaxHP 33:shita2 34:種類(0:敵 1-4:アイテム) 35:遅延表示 36:壁中カウント 37:点数 38:攻撃パーツの向きno
	int hp,_HP;
	int dame,damenow,damecnt,damecnt2;
	int dameanime,hpanime;
	int colof;

	int buki,bukia,bukib,bukic,bukid,bukif;



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
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAINFRM_H__CB74BAD3_A130_4BC8_BC2C_571FF49FA2E7__INCLUDED_)
