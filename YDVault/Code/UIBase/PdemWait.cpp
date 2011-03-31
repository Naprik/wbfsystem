// PdemWait.cpp: implementation of the CPdemWait class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PdemWait.h"
#include "WaitDlgThread.h"
#include "WaitWorkerThread.h"
#include "WaitDlg.h"
#include "../Include/YDVaultComm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPdemWait::CPdemWait(const CString& strMsg,BOOL bShowCancel,UINT nRange,short iDelaySec)
: m_strMsg(strMsg),m_nRange(nRange),m_bShowCancel(bShowCancel),m_iDelaySecond(iDelaySec)
{
	m_pThread = NULL;
	m_bClosed = FALSE;
}

CPdemWait::~CPdemWait()
{
	Close();
}
void CPdemWait::ResetMessage(const CString& msg)
{
	if(m_pThread != NULL)
	{
		CSingleLock lock(&m_pThread->m_mutex); //·ÀÖ¹¶Ô»°¿òÏú»Ù
		if(m_pThread->m_pDlg != NULL && m_pThread->m_pDlg->GetSafeHwnd()!=NULL)
		{
			CWnd* pWnd = m_pThread->m_pDlg->GetDlgItem(IDC_STATIC_WAIT_MESSAGE);
			if(pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
				pWnd->SetWindowText(msg);
		}
	}
}

void CPdemWait::SetPos(UINT nPos)
{
	if(m_pThread != NULL)
	{
		m_pThread->PostThreadMessage(WM_SETPOS,(WPARAM)nPos,0);
//		if(m_pThread->m_pDlg != NULL && m_pThread->m_pDlg->GetSafeHwnd()!=NULL)
		//		{
		//			m_pThread->m_pDlg->m_prgctrlWait.SetPos(nPos);
		//		}
		
	}
}

void CPdemWait::StepIt()
{
	if(m_pThread != NULL)
	{
		m_pThread->PostThreadMessage(WM_STEPIT,0,0);
//		if(m_pThread->m_pDlg != NULL && m_pThread->m_pDlg->GetSafeHwnd()!=NULL)
//		{
//			m_pThread->m_pDlg->m_prgctrlWait.StepIt();
//		}

	}

}

void CPdemWait::StepIt(BOOL& bCancel)
{
}


BOOL CPdemWait::BeginWait(CWaitWorkerThread* pThread/* = NULL*/)
{
	
	if(m_iDelaySecond <0 && m_iDelaySecond != -1) m_iDelaySecond = -1;
	
	CExtDllState state;

	_ASSERT(m_pThread == NULL);
	m_pThread = (CWaitDlgThread*)AfxBeginThread(RUNTIME_CLASS(CWaitDlgThread),
		THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	if(m_pThread == NULL)
		return FALSE;
	m_pThread->m_bShowCancel = m_bShowCancel;
	m_pThread->m_iDelaySecond = m_iDelaySecond;
	m_pThread->m_nRange = m_nRange;
	m_pThread->m_strMsg = m_strMsg;
	m_pThread->m_pWait = this;
	if(pThread != NULL)
	{
		m_pThread->m_pWorkerThread = pThread;
		pThread->m_pEvent = &(m_pThread->m_event);
		pThread->ResumeThread();
	}
	m_pThread->ResumeThread();
	m_pThread->WaitDlgCompleted();

	return TRUE;
}

BOOL CPdemWait::Close()
{
	m_bClosed = TRUE; 
	if(m_pThread != NULL)
	{
		m_pThread->Stop();
		DWORD dw = WaitForSingleObject(m_pThread->m_hThread,5000);
		switch(dw) {
		case WAIT_OBJECT_0:
			break;
		default:
			break;
		}
		m_pThread = NULL;
		return TRUE;
	}
	return FALSE;
}
