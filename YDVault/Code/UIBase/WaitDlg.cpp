// WaitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WaitDlg.h"
#include <AfxMT.h>
#include "WaitDlgThread.h"
#include "PdemWait.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaitDlg dialog


CWaitDlg::CWaitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaitDlg)
	m_strWaitMsg = _T("");
	//}}AFX_DATA_INIT
	m_pWait = NULL;
	m_nRange = 100;
	m_iDelaySecond = -1;
	m_bShowCancel = FALSE;
	m_nCount = 0;
	m_nIDTimer = -1;
}


void CWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaitDlg)
	DDX_Control(pDX, IDC_PROGRESS_WAIT, m_prgctrlWait);
	DDX_Text(pDX, IDC_STATIC_WAIT_MESSAGE, m_strWaitMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaitDlg, CDialog)
	//{{AFX_MSG_MAP(CWaitDlg)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaitDlg message handlers

void CWaitDlg::OnCancel() 
{
	if(!GetDlgItem(IDCANCEL)->IsWindowVisible())
		return; //cancel按钮不可见不容许关闭
	if(m_pWait != NULL)
	{
		m_pWait->Close();
	}
	
	CDialog::OnCancel();
}

void CWaitDlg::OnOK() 
{
	//CDialog::OnOK();
}

BOOL CWaitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDCANCEL)->ShowWindow(m_bShowCancel ? SW_SHOW : SW_HIDE);
	if(m_iDelaySecond > 0)
	{
		m_nCount = m_iDelaySecond*10;
		m_prgctrlWait.SetRange32(0,m_nCount);
	}
	else
	{
		m_prgctrlWait.SetRange32(0,m_nRange);
	}
	m_prgctrlWait.SetPos(0);
	m_prgctrlWait.SetStep(1);
	
	if(m_iDelaySecond >= 0)
	{
		m_nIDTimer = SetTimer(1,100,NULL);
	}
	
	BringWindowToTop();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWaitDlg::OnTimer(UINT nIDEvent) 
{
	if(m_iDelaySecond > 0)
	{
		m_prgctrlWait.StepIt();
		m_nCount--;
		if(m_nCount <= 0)
		{
			KillTimer(m_nIDTimer);
			m_nIDTimer = -1;

			if(m_pWait != NULL && m_pWait->m_pThread != NULL)
			{
				m_pWait->m_pThread->Stop();
			}
			//CDialog::OnOK();
		}
	}
	else if(m_iDelaySecond == 0)
	{
		m_prgctrlWait.StepIt();
		if(m_nRange == (UINT)m_prgctrlWait.GetPos())
			m_prgctrlWait.SetPos(0);
	}
	
	if(m_pWait != NULL && m_pWait->m_bClosed)
	{
		if(m_pWait->m_pThread != NULL) //PostThreadMessage无效的处理
		{   
			m_pWait->m_pThread->Stop();
		}
	}
//	CDialog::OnOK();
}

int CWaitDlg::DoModal() 
{
	return CDialog::DoModal();
}

void CWaitDlg::OnClose() 
{
	if(m_nIDTimer>0)
	{
		KillTimer(m_nIDTimer);
		m_nIDTimer = -1;
	}
	CDialog::OnClose();
}
