#if !defined(AFX_WAITDLGTHREAD_H__1B5DE1A4_36DA_446D_860B_68F643FEC8BC__INCLUDED_)
#define AFX_WAITDLGTHREAD_H__1B5DE1A4_36DA_446D_860B_68F643FEC8BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaitDlgThread.h : header file
//

#include <afxMT.h>
/////////////////////////////////////////////////////////////////////////////
// CWaitDlgThread thread
#define		WM_STEPIT		WM_USER+1
#define		WM_SETPOS		WM_USER+2

class CWaitDlg;
class CWaitWorkerThread;
class CPdemWait;

class CWaitDlgThread : public CWinThread
{
	DECLARE_DYNCREATE(CWaitDlgThread)
protected:
	CWaitDlgThread();           // protected constructor used by dynamic creation

// Attributes
private:
	BOOL              m_bDlgCompleted;
	UINT              m_nRange;
	CString           m_strMsg;
	short             m_iDelaySecond;
	BOOL			  m_bShowCancel;
	CWaitDlg         *m_pDlg;
	CMutex            m_mutex;  //对话框销毁和ResetMessage锁
	CPdemWait*        m_pWait;
	friend class CWaitDlg;
	friend class CPdemWait;		
public:
	CEvent            m_event;  //
	CWaitWorkerThread*m_pWorkerThread;

// Operations
public:
	void Stop();
private:
	void WaitDlgCompleted();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaitDlgThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CWaitDlgThread();

	// Generated message map functions
	//{{AFX_MSG(CWaitDlgThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAITDLGTHREAD_H__1B5DE1A4_36DA_446D_860B_68F643FEC8BC__INCLUDED_)
