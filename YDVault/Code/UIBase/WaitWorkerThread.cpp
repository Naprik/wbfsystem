// WaitWorkerThread.cpp: implementation of the CWaitWorkerThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaitWorkerThread.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CWaitWorkerThread, CWinThread)

CWaitWorkerThread::CWaitWorkerThread()
{
	m_pEvent = NULL;
}

CWaitWorkerThread::~CWaitWorkerThread()
{

}

BOOL CWaitWorkerThread::InitInstance()
{
	return CWinThread::InitInstance();
}

int CWaitWorkerThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CWaitWorkerThread, CWinThread)
//{{AFX_MSG_MAP(CWaitDlgThread)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
