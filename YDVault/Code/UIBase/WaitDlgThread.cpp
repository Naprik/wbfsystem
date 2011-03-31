// WaitDlgThread.cpp : implementation file
//

#include "stdafx.h"
#include "WaitDlgThread.h"
#include "WaitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

/////////////////////////////////////////////////////////////////////////////
// CWaitDlgThread

IMPLEMENT_DYNCREATE(CWaitDlgThread, CWinThread)

CWaitDlgThread::CWaitDlgThread()
:m_event(FALSE,TRUE)
{
	m_nRange = 100;
	m_strMsg = _T("ÇëÉÔºò...");
	m_iDelaySecond = -1;
	m_bShowCancel = FALSE;
	m_pDlg = NULL;
	m_pWorkerThread = NULL;
	m_pWait = NULL;	
	m_bDlgCompleted = FALSE;
}

CWaitDlgThread::~CWaitDlgThread()
{
}

BOOL CWaitDlgThread::InitInstance()
{
	m_bDlgCompleted = FALSE;
	CSingleLock lock(&m_mutex);
	m_pDlg = new CWaitDlg;
	m_pMainWnd = m_pDlg;
	m_pDlg->m_strWaitMsg = m_strMsg;
	m_pDlg->m_nRange = m_nRange;
	m_pDlg->m_iDelaySecond =m_iDelaySecond;
	m_pDlg->m_bShowCancel = m_bShowCancel;
	m_pDlg->m_pWait = m_pWait;	
	//m_pDlg->DoModal();

	m_pDlg->Create(CWaitDlg::IDD);
	m_pDlg->ShowWindow(SW_SHOWNORMAL);
	m_bDlgCompleted = TRUE;
	//delete m_pDlg;
	//m_pDlg = NULL;

	return TRUE;
}

int CWaitDlgThread::ExitInstance()
{
	if (m_pDlg != NULL) {
		CSingleLock lock(&m_mutex);
		m_pDlg->DestroyWindow();
		delete m_pDlg;
		m_pDlg = NULL;
	}
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CWaitDlgThread, CWinThread)
	//{{AFX_MSG_MAP(CWaitDlgThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaitDlgThread message handlers

void CWaitDlgThread::Stop()
{
	m_event.SetEvent();
	this->PostThreadMessage(WM_QUIT,0,0);
}

void CWaitDlgThread::WaitDlgCompleted()
{
	while(!m_bDlgCompleted) Sleep(10);
}

BOOL CWaitDlgThread::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_STEPIT) {
		if(m_pDlg != NULL && m_pDlg->GetSafeHwnd()!=NULL)
		{
			m_pDlg->m_prgctrlWait.StepIt();
		}
	}
	else if(pMsg->message == WM_SETPOS)
	{
		if(m_pDlg != NULL && m_pDlg->GetSafeHwnd()!=NULL)
		{
			UINT nPos = pMsg->wParam;
			m_pDlg->m_prgctrlWait.SetPos(nPos);
		}
		
	}
	return CWinThread::PreTranslateMessage(pMsg);
}
