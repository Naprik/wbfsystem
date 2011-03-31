// Base.h: interface for the CBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASE_H__7CDB6F4F_A18A_4967_A1FD_08A7744033BB__INCLUDED_)
#define AFX_BASE_H__7CDB6F4F_A18A_4967_A1FD_08A7744033BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBase  
{
public:
	CBase();
	virtual ~CBase();

	CString GetExePath();

};

#endif // !defined(AFX_BASE_H__7CDB6F4F_A18A_4967_A1FD_08A7744033BB__INCLUDED_)
