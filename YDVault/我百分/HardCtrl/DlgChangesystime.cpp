// DlgChangesystime.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgChangesystime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgChangesystime dialog


CDlgChangesystime::CDlgChangesystime(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChangesystime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgChangesystime)
	m_date = COleDateTime::GetCurrentTime();
	m_time = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
}


void CDlgChangesystime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgChangesystime)
	DDX_DateTimeCtrl(pDX, IDC_DATE, m_date);
	DDX_DateTimeCtrl(pDX, IDC_TIME, m_time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgChangesystime, CDialog)
	//{{AFX_MSG_MAP(CDlgChangesystime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgChangesystime message handlers

void CDlgChangesystime::OnOK() 
{
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 

   // Get a token for this process. 
	if (!OpenProcessToken(GetCurrentProcess(), 
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
	{
		AfxMessageBox("时间设置失败");	
		CDialog::OnOK();
	}

   // Get the LUID for the shutdown privilege. 
	LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, 
		&tkp.Privileges[0].Luid); 

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	// Get the shutdown privilege for this process. 
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
		(PTOKEN_PRIVILEGES)NULL, 0); 

	if (GetLastError() != ERROR_SUCCESS) 
	{
		AfxMessageBox("时间设置失败");	
		CDialog::OnOK(); 
	}
	//取得必要权限后，设置系统时间
	SYSTEMTIME systm;

	UpdateData(TRUE);
	int nYear = m_date.GetYear();
	int nMonth = m_date.GetMonth();
	int nDay = m_date.GetDay();
	int nHour = m_time.GetHour();
	int nMinute = m_time.GetMinute();
	int nSecond = m_time.GetSecond();

	systm.wYear = nYear;
	systm.wMonth = nMonth;
	systm.wDay = nDay;
	systm.wHour = nHour;
	systm.wMinute = nMinute;
	systm.wSecond = nSecond;

	if(0 != ::SetLocalTime(&systm))
	{
		AfxMessageBox("时间设置成功！");
	
		CDialog::OnOK();
	}
	else
	{
		AfxMessageBox("时间设置失败");
	
		CDialog::OnOK();
	}
}
