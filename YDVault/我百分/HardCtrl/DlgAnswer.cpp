// DlgAnswer.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgAnswer.h"
#include "DBSubject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAnswer dialog


CDlgAnswer::CDlgAnswer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnswer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAnswer)
	m_end = 0;
	m_start = 0;
	//}}AFX_DATA_INIT
}


void CDlgAnswer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAnswer)
	DDX_Control(pDX, IDC_SUBJECT, m_subject);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, IDC_END, m_end);
	DDX_Text(pDX, IDC_START, m_start);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAnswer, CDialog)
	//{{AFX_MSG_MAP(CDlgAnswer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAnswer message handlers

void CDlgAnswer::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	index = m_subject.GetCurSel();
	if(index != -1)
	{
		m_subject.GetLBText(index,strSubject);
	}	
	
	int t1 = m_start;
	int t2 = m_end;
	if((m_start == 0) || (m_end == 0) ||(m_start > m_end))
	{
		AfxMessageBox("题号设置错误！");
	}
	else if(strSubject == "")
	{
		AfxMessageBox("请选择科目！");
	}
	else
		CDialog::OnOK();
}

BOOL CDlgAnswer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
// 	CoInitialize(NULL);
// 	CDBSubject Subjectdb;
// 	if ( !Subjectdb.Connect())
// 	{
// 		AfxMessageBox("无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		int flag = Subjectdb.LoadByAll();	
// 		while ( flag )
// 		{
// 			m_subject.AddString(Subjectdb.m_subject);
// 			flag = Subjectdb.Move();
// 		}
// 
// 		Subjectdb.Close();
// 	}
// 	CoUninitialize();
	
	return TRUE;  
}
