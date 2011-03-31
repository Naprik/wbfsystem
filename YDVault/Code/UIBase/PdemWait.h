// PdemWait.h: interface for the CPdemWait class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDEMWAIT_H__696D2E0C_E11D_45B1_841E_CA5BEF08A221__INCLUDED_)
#define AFX_PDEMWAIT_H__696D2E0C_E11D_45B1_841E_CA5BEF08A221__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProgressBar.h"

class CWaitDlgThread;
class CWaitWorkerThread;

class AFX_EXT_CLASS CPdemWait  
{
public:
	CPdemWait(const CString& strMsg = _T("«Î…‘∫Ú..."),BOOL bShowCancel = FALSE,UINT nRange = 100,short iDelaySec = -1);	
	virtual ~CPdemWait();

	virtual void ResetMessage(const CString& msg);
	virtual void SetPos(UINT nPos);
	virtual void StepIt();
	virtual void StepIt(BOOL& bCancel);

	virtual BOOL BeginWait(CWaitWorkerThread* pThread = NULL);
	virtual BOOL Close();

	friend class CWaitDlg;
protected:
	CWaitDlgThread*   m_pThread;
	UINT              m_nRange;
	UINT              m_nCurPos;
	CString           m_strMsg;
	short             m_iDelaySecond;
	BOOL			  m_bShowCancel;

	CProgressBar      m_cProgressBar;
	BOOL              m_bClosed;
};

#endif // !defined(AFX_PDEMWAIT_H__696D2E0C_E11D_45B1_841E_CA5BEF08A221__INCLUDED_)
