// DlgDpartment.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgDpartment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDpartment dialog


CDlgDpartment::CDlgDpartment(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDpartment::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDpartment)
	m_head = _T("");
	m_tel = _T("");
	m_depart = _T("");
	m_departid = _T("");
	//}}AFX_DATA_INIT
}


void CDlgDpartment::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDpartment)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, IDC_HEAD, m_head);
	DDX_Text(pDX, IDC_TEL, m_tel);
	DDX_Text(pDX, IDC_DEPART, m_depart);
	DDX_Text(pDX, IDC_DEPARTID, m_departid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDpartment, CDialog)
	//{{AFX_MSG_MAP(CDlgDpartment)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDpartment message handlers


BOOL CDlgDpartment::OnInitDialog()
{
	CDialog::OnInitDialog();


	return TRUE;
}


void CDlgDpartment::OnOK() 
{
	// TODO: Add extra validation here
/*	UpdateData(TRUE);

	if(m_depart != "")
	{	
		if(m_head == "" || m_tel == "")
		{
			CString msg;
			msg.Format("基本信息未填写完整是否继续？");
			if ( MessageBox(msg,"确认",MB_OKCANCEL) == IDOK)
				CDialog::OnOK();
		}
		else
			CDialog::OnOK();
	}
	else
		AfxMessageBox("系别名称不能为空！");*/
	UpdateData(TRUE);
	if(m_depart != "" && m_departid != "")
	{
		if(m_head == "" || m_tel == "")
		{
			CString msg;
			msg.Format("基本信息未填写完整是否继续？");
			if ( MessageBox(msg,"确认",MB_OKCANCEL) == IDOK)
				CDialog::OnOK();
		}
		else
			CDialog::OnOK();
	}
	else
	{
		AfxMessageBox("院系名称或院系编号不能为空！");	
		return;
	}
	//CDialog::OnOK();
}
