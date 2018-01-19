// compress.h: compress クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPRESS_H__5DD17110_20A7_11D2_8933_BD9978B9297D__INCLUDED_)
#define AFX_COMPRESS_H__5DD17110_20A7_11D2_8933_BD9978B9297D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class compress : public CObject  
{
public:
	compress();
	virtual ~compress();
	BOOL run(CString fname);
	BOOL run2(CString fname);
	BOOL run3(CString fname);

protected:
	BYTE buf[60000];
	BYTE buf2[60000];
	UINT buf2cnt;
	BYTE moji,mojicnt;
	BYTE bufout[10];
	CFile file,file2;

	//af2形式で追加
	DWORD Inlen,Outlen,pFlg1,pFlg2,pData;
	CFile file3,file4;
	UINT Read(BYTE s[],int cnt);
	int cnt_,cnt2;
	BYTE s_[10000];
	void Write(BYTE b);

	//af3形式で追加
	long enHuffman(unsigned char *code, long clen,
               unsigned char *text, long tlen);
};

#endif // !defined(AFX_COMPRESS_H__5DD17110_20A7_11D2_8933_BD9978B9297D__INCLUDED_)
