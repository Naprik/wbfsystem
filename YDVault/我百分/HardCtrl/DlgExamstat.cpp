// DlgExamstat.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgExamstat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExamstat dialog


CDlgExamstat::CDlgExamstat(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExamstat::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExamstat)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgExamstat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExamstat)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExamstat, CDialog)
	//{{AFX_MSG_MAP(CDlgExamstat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExamstat message handlers

BOOL CDlgExamstat::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_STATIC1)->SetWindowText(m_subject);
	GetDlgItem(IDC_STATIC2)->SetWindowText(m_original);
	GetDlgItem(IDC_STATIC3)->SetWindowText(m_reality);
	return true;
}

void CDlgExamstat::OnOK() 
{

			//GetDlgItem(IDC_STATIC7)->SetWindowText(temp);
	CDialog::OnOK();
}

void CDlgExamstat::OnCancel() 
{
	
	CString msg;
	msg.Format("本场考试应道到%s，实际搜索到%s，确认结束搜索？",m_original,m_reality);
	if ( MessageBox(msg,"确认",MB_OKCANCEL) != IDOK)
	{	
		return;	
	}
	
	CDialog::OnCancel();
}
