// FtpUpload.h: interface for the CFtpUpload class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPUPLOAD_H__04EBED1F_2513_4BAA_8FB3_524925363B24__INCLUDED_)
#define AFX_FTPUPLOAD_H__04EBED1F_2513_4BAA_8FB3_524925363B24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxinet.h>
#include "ProgDlg.h"

class CFtpInfo;
class CFtpUpload  
{
public:
	CFtpUpload(CFtpInfo *_pFtpInfo, DWORD ctout = 0);
	virtual ~CFtpUpload();
	unsigned int m_thread_finished_flag;
	unsigned int m_abort_flag;
	CString m_result_str;
	CProgressDlg m_cpd;
	
	CFtpInfo *m_pFtpInfo;
	DWORD m_connection_timeout;
public:
	HRESULT Upload();
};

#endif // !defined(AFX_FTPUPLOAD_H__04EBED1F_2513_4BAA_8FB3_524925363B24__INCLUDED_)
