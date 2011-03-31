#include "stdafx.h"
#include "SoftStat.h"
#pragma   warning(disable:   4244)   

class CProgressThreadInfo : public CObject                                                   
{
 public:
   DECLARE_DYNAMIC(CProgressThreadInfo)
   CProgressThreadInfo();
   CProgressThreadDlg*     m_pStatusDlg;        //Used to allow the UI to be updated during operation
   FUNCTION_WITH_PROGRESS* m_pfnFunction;       //Function to call to do the work
   void*                   m_pData;             //data to pass to the caller function
};
UINT ProgressDialogWorkerThread(LPVOID pParam);
IMPLEMENT_DYNAMIC(CProgressThreadInfo, CObject)
CProgressThreadInfo::CProgressThreadInfo()
{
	m_pStatusDlg = NULL;
	m_pfnFunction = NULL;
	m_pData = NULL;
}
IMPLEMENT_DYNAMIC(CProgressThreadDlg, CDialog)
CProgressThreadDlg::CProgressThreadDlg(BOOL bShowCancelButton, BOOL bConfirmCancel, CWnd* pParent)
	: CDialog( IDD_PROGRESS, pParent)
{
	//{{AFX_DATA_INIT(CProgressThreadDlg)
	//}}AFX_DATA_INIT
	m_nCurrentPercentage = 0;
	m_bReady = FALSE;
	m_bCancelled = FALSE;
	m_bShowCancelButton = bShowCancelButton;
	if (bConfirmCancel)
		ASSERT(m_bShowCancelButton);
	m_bConfirmCancel = bConfirmCancel;
	m_bOkToClose = FALSE;
	m_pThread = NULL;
	m_pos     = 0;
}
void CProgressThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressThreadDlg)
	DDX_Control(pDX, IDC_READ_ACCOUNT, m_account);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH1, m_falsh);
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CProgressThreadDlg, CDialog)
	//{{AFX_MSG_MAP(CProgressThreadDlg)
	ON_BN_CLICKED(IDCANCEL, OnCancelled)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
CString CProgressThreadDlg::GetExePath()
{
	CString file;
	char process[MAX_PATH];
	GetModuleFileName(NULL, process, MAX_PATH);
    for ( int i = strlen(process);i>0 ;i --)
	if ( process[i] =='\\')
	{
		process[i+1]='\0';
		break;
	}
	file.Format(process);
	return file;
}
BOOL CProgressThreadDlg::OnInitDialog() 
{	
//	HCURSOR hcur=::LoadCursorFromFile("AniCur.ani");           //加载动画广标
//	::SetClassLong(GetSafeHwnd(),GCL_HCURSOR,(LONG)hcur);

	CDialog::OnInitDialog();
	m_ctrlProgress.SetRange(0, 100);
	m_ctrlProgress.SetPos(0);
	m_TimeCreation = CTime::GetCurrentTime();
	m_bReady = TRUE;
	if (!m_sCaption.IsEmpty())
		SetWindowText(m_sCaption);
	m_EventReady.SetEvent();
	if ( m_sConfirmPrompt.IsEmpty())
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE );
    
	SetTimer(NULL,1000,NULL);


	RECT rc;
	this->GetClientRect(&rc);

	m_falsh.MoveWindow( &rc, true );

	TCHAR strCurDrt[500];
	int nLen = ::GetCurrentDirectory(500,strCurDrt);
	if( strCurDrt[nLen]!='\\' )
	{
		strCurDrt[nLen++] = '\\';
		strCurDrt[nLen] = '\0';
	}

	CString strFileName = strCurDrt;
	CJahCoverCapture capture;
	capture.m_bmpFile =GetExePath()+"clock.swf" ;
	strFileName = capture.m_bmpFile;
	m_falsh.LoadMovie(0, strFileName);
	m_falsh.SetScale("%100");   
	m_falsh.Play();
	if(m_flag)	
	{
		m_ctrlProgress.ShowWindow(SW_SHOW);
		m_account.ShowWindow(SW_SHOW);
		m_falsh.ShowWindow(SW_HIDE);
	}
	else
	{
		m_ctrlProgress.ShowWindow(SW_HIDE);
		m_account.ShowWindow(SW_HIDE);
		m_falsh.ShowWindow(SW_SHOW);
	}
	return TRUE;
}
void CProgressThreadDlg::OnCancel() 
{
	if (m_bOkToClose)
	{
		CDialog::OnCancel();
	}
	else if (m_bShowCancelButton)
	{
		if (m_bConfirmCancel)
		{
			ASSERT(m_pThread);
			m_pThread->SuspendThread();
			if (AfxMessageBox(m_sConfirmPrompt, MB_YESNO) == IDYES)
				m_bCancelled = TRUE;
			m_pThread->ResumeThread();
		}
		else
			m_bCancelled = TRUE;
	}
}
void CProgressThreadDlg::Close()
{
	m_bOkToClose = TRUE;
	PostMessage(WM_CLOSE);
}
void CProgressThreadDlg::OnCancelled() 
{
	OnCancel();
}

BOOL ExecuteFunctionWithProgressDialog(FUNCTION_WITH_PROGRESS* pfnFunction, const CString& sProgressTitle,
									   void* pData, DWORD dwFlags, const CString& sConfirmPrompt, int nPriority, CWnd* pParent)
{
	BOOL bShowCancel = (dwFlags & PSTAT_CANCEL);
	BOOL bConfirmCancel = (dwFlags & PSTAT_CONFIRMCANCEL);
	CProgressThreadDlg dlg(bShowCancel, bConfirmCancel, pParent);
	dlg.m_sCaption = sProgressTitle;
	dlg.m_sConfirmPrompt = sConfirmPrompt;
	CProgressThreadInfo Info;
	Info.m_pStatusDlg = &dlg;
	Info.m_pfnFunction = pfnFunction;
	Info.m_pData = pData;
    CWinThread* pThread = AfxBeginThread(ProgressDialogWorkerThread, &Info, THREAD_PRIORITY_LOWEST/*nPriority*/, 0, CREATE_SUSPENDED);
	if (!pThread)	{ ASSERT(FALSE);return FALSE;}
	dlg.m_pThread = pThread;
	pThread->ResumeThread();
	dlg.DoModal();
	return !dlg.HasBeenCancelled();
}
UINT ProgressDialogWorkerThread(LPVOID pParam)
{
	CProgressThreadInfo* pInfo = (CProgressThreadInfo*) pParam;
	ASSERT(pInfo);
	ASSERT(pInfo->IsKindOf(RUNTIME_CLASS(CProgressThreadInfo)));
	pInfo->m_pfnFunction(pInfo->m_pData, pInfo->m_pStatusDlg);
	pInfo->m_pStatusDlg->Close();
	return 0;
}

void CProgressThreadDlg::SetText(CString text)
{
	if (!m_bReady )
		return;
	ASSERT(GetDlgItem(IDC_TIMELEFT));
	GetDlgItem(IDC_TIMELEFT)->SetWindowText(text);
}
void CProgressThreadDlg::SetStep(long pos)
{
  	CString tmp;
	double t = pos ;
	t = t / m_max;
	m_ctrlProgress.SetPos(t*100);
	tmp.Format("已经完成：%d/%d(%0.2f%c)",pos,m_max,t*100,'%' );
	GetDlgItem(IDC_READ_ACCOUNT)->SetWindowText(tmp);
}
void CProgressThreadDlg::SetRange(long max)
{
	m_max = max;
	m_ctrlProgress.SetRange(0, max);
	m_ctrlProgress.SetPos(0);
}

void CProgressThreadDlg::OnTimer(UINT nIDEvent) 
{
	SetStep(m_pos);	

	CDialog::OnTimer(nIDEvent);
}
