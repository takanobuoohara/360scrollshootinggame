// libarc.h: libarc クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIBARC_H__7AC552CE_9683_11D2_8933_9E0E2F5B3577__INCLUDED_)
#define AFX_LIBARC_H__7AC552CE_9683_11D2_8933_9E0E2F5B3577__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class libarc  : public CObject
{
public:
	libarc();
	virtual ~libarc();
	
	CString openfile,opendir;

	BOOL add(CString flib,CString fname);
	BOOL sub(CString flib,CString fname);
	BOOL test(CString flib,CString fname);
	BOOL testnext(CString flib,CString fname);
	BOOL Open(CString flib,CString fname);
	BOOL Close();

};

#endif // !defined(AFX_LIBARC_H__7AC552CE_9683_11D2_8933_9E0E2F5B3577__INCLUDED_)
