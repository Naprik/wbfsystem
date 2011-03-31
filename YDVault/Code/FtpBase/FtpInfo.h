// FtpInfo.h: interface for the CFtpInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPINFO_H__C856D109_680A_49A6_B0AF_24641AB8ACC1__INCLUDED_)
#define AFX_FTPINFO_H__C856D109_680A_49A6_B0AF_24641AB8ACC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFtpInfo  
{
public:
	CFtpInfo();
	virtual ~CFtpInfo();
public:
	CString m_strFtpConnect;
	CString m_strUser;
	CString m_strPwd;
	CString m_strLocalFile;
	CString m_strFtpFile;
	UINT    m_uPort;//¶Ë¿Ú
};

#endif // !defined(AFX_FTPINFO_H__C856D109_680A_49A6_B0AF_24641AB8ACC1__INCLUDED_)
