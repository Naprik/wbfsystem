// DlgSubject.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgSubject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubject dialog


CDlgSubject::CDlgSubject(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSubject::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSubject)
	m_subject = _T("");
	m_subjectid = _T("");
	//}}AFX_DATA_INIT
}


void CDlgSubject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubject)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, IDC_SUBJECT, m_subject);
	DDX_Text(pDX, IDC_SUBJECTID, m_subjectid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSubject, CDialog)
	//{{AFX_MSG_MAP(CDlgSubject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubject message handlers

void CDlgSubject::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if(m_subjectid == "")
	{	
		AfxMessageBox("科目编号不能为空！");
		return;
	}
	if(m_subject == "")
	{
		AfxMessageBox("科目名称不能为空！");
		return;
	}
		CDialog::OnOK();
}
