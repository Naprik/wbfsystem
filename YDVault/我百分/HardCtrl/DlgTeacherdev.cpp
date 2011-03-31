// DlgTeacherdev.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgTeacherdev.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTeacherdev dialog


CDlgTeacherdev::CDlgTeacherdev(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTeacherdev::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTeacherdev)
	m_devmac = _T("");
	m_devid = _T("");
	m_teachername = _T("");
	//}}AFX_DATA_INIT
}


void CDlgTeacherdev::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTeacherdev)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, IDC_TEACHERDEV, m_devmac);
	DDX_Text(pDX, IDC_TEACHERID, m_devid);
	DDX_Text(pDX, IDC_TEACHERNAME, m_teachername);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTeacherdev, CDialog)
	//{{AFX_MSG_MAP(CDlgTeacherdev)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTeacherdev message handlers

void CDlgTeacherdev::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if((m_devid == "")||(m_teachername == "")||(m_devmac == ""))
	{	
		AfxMessageBox("必要信息不完整，请填写完整！");
	}
	else
		CDialog::OnOK();
}
