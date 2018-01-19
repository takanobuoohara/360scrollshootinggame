// libarc.cpp: libarc クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "libarc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

libarc::libarc()
{
	openfile = "";
}

libarc::~libarc()
{
	if(openfile != "") Close();
}

BOOL libarc::add(CString flib,CString fname)
{
	char buf[200];
	CString f;
	//データ書き込み
	f = "data\\" + flib + ".afd";
	CFile file,file2;
	if(file2.Open(fname,CFile::shareDenyNone | CFile::modeRead | CFile::typeBinary,NULL)==FALSE)
		return FALSE;
	if(file.Open(f,CFile::shareDenyNone | CFile::modeRead | CFile::typeBinary,NULL)==FALSE)
	{
		if(file.Open(f,CFile::shareDenyNone | CFile::modeCreate | CFile::modeWrite | CFile::typeBinary,NULL)==FALSE) 
			return FALSE;
		file.Close();
	}

	if(file.Open(f,CFile::shareDenyNone | CFile::modeReadWrite | CFile::typeBinary,NULL)==FALSE) 
		return FALSE;

	file.SeekToEnd();
	DWORD start = file.GetPosition();
	for(;;)
	{
		UINT b = file2.Read(buf,200); //転送元からデータを得る
		if(b != 0)
			file.Write(buf,b);
		if(b != 200) break;
	}
	DWORD end = file.GetPosition();

	file2.Close();
	file.Close();

	//インデックス書き込み
	f = "data\\" + flib + ".afl";

	if(file.Open(f,CFile::shareDenyNone | CFile::modeRead | CFile::typeBinary,NULL)==FALSE)
	{
		if(file.Open(f,CFile::shareDenyNone | CFile::modeCreate | CFile::modeWrite | CFile::typeBinary,NULL)==FALSE) return FALSE;
			file.Close();
	}

	if(file.Open(f,CFile::shareDenyNone | CFile::modeReadWrite | CFile::typeBinary,NULL)==FALSE) return FALSE;

	file.SeekToEnd();
	int i,j;
	j = fname.ReverseFind('\\');
	j++;
	for(i=1;;i++,j++)
	{
		if(i > 100) return FALSE;
		buf[i] = fname[j];
		if(buf[i] == 0) break;
	}
	i++;
	buf[i] = (BYTE)((start / 0x1000000) % 256); i++;
	buf[i] = (BYTE)((start / 0x10000  ) % 256); i++;
	buf[i] = (BYTE)((start / 0x100    ) % 256); i++;
	buf[i] = (BYTE)((start / 0x1      ) % 256); i++;

	buf[i] = (BYTE)((end   / 0x1000000) % 256); i++;
	buf[i] = (BYTE)((end   / 0x10000  ) % 256); i++;
	buf[i] = (BYTE)((end   / 0x100    ) % 256); i++;
	buf[i] = (BYTE)((end   / 0x1      ) % 256); i++;

	buf[0] = (char)i;	
	file.Write(buf,i+1);
	file.Close();

	return TRUE;
}

BOOL libarc::sub(CString flib,CString fname)
{
	return TRUE;
}

BOOL libarc::test(CString flib,CString fname)
{
	return TRUE;
}

BOOL libarc::testnext(CString flib,CString fname)
{
	return TRUE;
}

BOOL libarc::Open(CString flib,CString fname)
{
	if(flib == "") //ルートファイル(アーカイブ無視)
	{
		openfile = "";
		opendir = "";
	}
	else //アーカイブ指定
	{
		BYTE buf[10000];
		CString f,f2;
		CFile file;
		f = flib + ".afl";
		f2= fname;
		if(file.Open(f,CFile::shareDenyNone | CFile::modeRead | CFile::typeBinary,NULL)==FALSE)
			return FALSE;

		int i = 0,b;
		DWORD start,end;
		for(;;)
		{
			b = file.Read(buf,1);
			if(b == 0) //アーカイブ内に無い
			{
				i = -1;
				break;
			}
			b = (UINT)buf[0]; //ファイルヘッダバイト数獲得
			file.Read(buf,b-1/*start*/-8/*end*/); //ファイル内容読み込み
			if(fname.CompareNoCase((CString)buf) == 0) //アーカイブ内にあった
			{
				file.Read(buf,9); //ファイル長獲得
				start = (UINT)buf[0] * 0x1000000 +
					    (UINT)buf[1] * 0x10000   +
					    (UINT)buf[2] * 0x100     +
					    (UINT)buf[3] * 0x1        ;
				end   = (UINT)buf[4] * 0x1000000 +
					    (UINT)buf[5] * 0x10000   +
					    (UINT)buf[6] * 0x100     +
					    (UINT)buf[7] * 0x1        ;
				file.Close();
				f = flib + ".afd";
				if(file.Open(f,CFile::shareDenyNone | CFile::modeRead | CFile::typeBinary,NULL)==FALSE)
					return FALSE;
				int sa = end - start;
				file.SeekToBegin();
				file.Seek(start,CFile::begin);
				CFile file2;
				if(file2.Open(f2,CFile::shareDenyNone | CFile::modeCreate | CFile::modeWrite | CFile::typeBinary,NULL)==FALSE) 
					return FALSE;
				file2.Close();

				if(file2.Open(f2,CFile::shareDenyNone | CFile::modeReadWrite | CFile::typeBinary,NULL)==FALSE) 
					return FALSE;

				for(;;)
				{
					DWORD l = file.Read(buf,10000);
					sa -= 10000;
					if( sa <= 0)
					{
						sa += 10000;
						file2.Write(buf,sa);
						break;
					}
					file2.Write(buf,l);
				}
				openfile = f2;
				opendir = "";
				break;
			}
			else
			{
				file.Read(buf,9); //ファイル長獲得
			}
			i ++;
		}
		if(i == -1)
			return FALSE;
	}
	return TRUE;
}

BOOL libarc::Close()
{
	if(openfile != "")
	{
		CFile::Remove(openfile);
		openfile = "";
		opendir = "";
	}
	return TRUE;
}

