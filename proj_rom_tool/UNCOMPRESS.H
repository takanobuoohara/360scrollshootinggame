// UnCompress.h: UnCompress クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNCOMPRESS_H__5DD17111_20A7_11D2_8933_BD9978B9297D__INCLUDED_)
#define AFX_UNCOMPRESS_H__5DD17111_20A7_11D2_8933_BD9978B9297D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "libarc.h"

class UnCompress : public CObject  
{
public:
	UnCompress();
	virtual ~UnCompress();
	BOOL run(CString fname);
	BOOL run2(CString fname);
	BOOL run3(CString fname);
	CString Open(CString fname,CString flib = "");
	void Close();
	BOOL nom;
	BOOL err;
protected:
	libarc la;
	BYTE buf[60000];
	BYTE buf2[60000];
	UINT buf2cnt;
	BYTE moji,mojicnt;
	BYTE bufout[300];
	CFile file,file2;

	void Write(BYTE b);
	CString _fname;

	//af2形式で追加
	DWORD Outlen,pFlg1,pFlg2,pData;
	UINT ReadFlg1(BYTE s[],int cnt);
	UINT ReadFlg2(BYTE s[],int cnt);
	UINT ReadData(BYTE s[],int cnt);
	int cnt_1,cnt21;
	BYTE s_1[10000];
	int cnt_2,cnt22;
	BYTE s_2[10000];
	int cnt_d,cnt2d;
	BYTE s_d[10000];

	//af3で追加
	long deHuffman(unsigned char *text, long tlen,
               unsigned char *code, long clen);

};

#endif // !defined(AFX_UNCOMPRESS_H__5DD17111_20A7_11D2_8933_BD9978B9297D__INCLUDED_)
