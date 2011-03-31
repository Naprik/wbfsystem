// DlgChangeExmaTime.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgChangeExmaTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgChangeExmaTime dialog


CDlgChangeExmaTime::CDlgChangeExmaTime(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChangeExmaTime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgChangeExmaTime)
	m_starttime = COleDateTime::GetCurrentTime();
	m_endtime = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
}


void CDlgChangeExmaTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgChangeExmaTime)
	DDX_DateTimeCtrl(pDX, IDC_START_TIME, m_starttime);
	DDX_DateTimeCtrl(pDX, IDC_END_TIME, m_endtime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgChangeExmaTime, CDialog)
	//{{AFX_MSG_MAP(CDlgChangeExmaTime)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgChangeExmaTime message handlers
