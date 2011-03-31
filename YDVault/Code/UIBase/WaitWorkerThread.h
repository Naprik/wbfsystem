// WaitWorkerThread.h: interface for the CWaitWorkerThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAITWORKERTHREAD_H__F3E56070_5C06_4CBD_8F12_A85B71D3A0F8__INCLUDED_)
#define AFX_WAITWORKERTHREAD_H__F3E56070_5C06_4CBD_8F12_A85B71D3A0F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxMT.h>

class BASE_DLL_API CWaitWorkerThread : public CWinThread 
{
	DECLARE_DYNCREATE(CWaitWorkerThread)
protected:
	CWaitWorkerThread();

public:
	CEvent*         m_pEvent;
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaitWorkerThread)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	virtual ~CWaitWorkerThread();
	
	// Generated message map functions
	//{{AFX_MSG(CWaitWorkerThread)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_WAITWORKERTHREAD_H__F3E56070_5C06_4CBD_8F12_A85B71D3A0F8__INCLUDED_)
