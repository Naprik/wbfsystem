// DlgExamaddr.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgExamaddr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExamaddr dialog


CDlgExamaddr::CDlgExamaddr(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExamaddr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExamaddr)
	m_addr = _T("");
	m_addid = _T("");
	//}}AFX_DATA_INIT
}


void CDlgExamaddr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExamaddr)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, IDC_ADDR, m_addr);
	DDX_Text(pDX, IDC_ADDRID, m_addid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExamaddr, CDialog)
	//{{AFX_MSG_MAP(CDlgExamaddr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExamaddr message handlers

void CDlgExamaddr::OnOK() 
{
	UpdateData(TRUE);

	if(m_addid == "")
	{	
		AfxMessageBox("考场编号不能为空！");
		return;
	}
	if(m_addr == "")
	{
		AfxMessageBox("考场不能为空！");
		return;
	}

	CDialog::OnOK();
}
