// DlgExmOption.cpp : implementation file
//
#include "stdafx.h"
#include "hardctrl.h"
#include "DlgExmOption.h"
#include "DBSubject.h"
#include "DBExamaddr.h"
#include "DBDepartment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExmOption dialog


CDlgExmOption::CDlgExmOption(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExmOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExmOption)
	m_start = COleDateTime::GetCurrentTime();
	m_end = COleDateTime::GetCurrentTime();
	m_supervisor = _T("");
	m_account = 0;
	m_maxno = 0;
	m_startno = 0;
	m_endno = 0;
	//}}AFX_DATA_INIT
}


void CDlgExmOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExmOption)
	DDX_Control(pDX, IDC_CLASSLIST, m_classlist);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_EXAMCLASS, m_examclass);
	DDX_Control(pDX, IDC_ADDRESS, m_address);
	DDX_Control(pDX, IDC_SUBJECT, m_subject);
	DDX_DateTimeCtrl(pDX, IDC_STARTTIME, m_start);
	DDX_DateTimeCtrl(pDX, IDC_ENDTIME, m_end);
	DDX_Text(pDX, IDC_SUPERVISOR, m_supervisor);
	DDX_Text(pDX, IDC_ACCOUNT, m_account);
	DDX_Text(pDX, IDC_MAXNO, m_maxno);
	DDX_Text(pDX, IDC_STARTNO, m_startno);
	DDX_Text(pDX, IDC_ENDNO, m_endno);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExmOption, CDialog)
	//{{AFX_MSG_MAP(CDlgExmOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExmOption message handlers

BOOL CDlgExmOption::OnInitDialog()
{
	CDialog::OnInitDialog();

    DWORD dwStyle=GetWindowLong(m_classlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_classlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_classlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_classlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_classlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_classlist.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	
	m_classlist.ClearHeadings();
	m_classlist.SetHeadings("班级,170");	

// 	CoInitialize(NULL);
// 	CDBDepartment departdb;
// 	if ( !departdb.Connect())
// 	{
// 		CoUninitialize();
// 		AfxMessageBox("无法连接到数据库服务器");
// 	}
// 	int flag2 = departdb.LoadByAll();
// 	while ( flag2 )
// 	{
// 		m_classlist.AddItem(0,departdb.m_depart);
// 		flag2 = departdb.Move();
// 	}
// 	departdb.Close();	
// 	CoUninitialize();


// 	CoInitialize(NULL);
// 	CDBSubject db;
// 	if ( !db.Connect())
// 	{
// 		CoUninitialize();
// 		AfxMessageBox("无法连接到数据库服务器");
// 	}
// 	int flag = db.LoadByAll();
// 	while ( flag )
// 	{
// 		m_subject.AddString(db.m_subject);
// 		subjectarry.Add(db.m_subject);
// 		flag = db.Move();
// 	}
// 	db.Close();	
// 	CoUninitialize();


// 	CoInitialize(NULL);
// 	CDBExamaddr db1;
// 	if ( !db1.Connect())
// 	{
// 		CoUninitialize();
// 		AfxMessageBox("无法连接到数据库服务器");
// 	}
// 	int flag1 = db1.LoadByAll();
// 	while ( flag1 )
// 	{
// 		m_address.AddString(db1.m_addr);
// 		addrarry.Add(db1.m_addr);
// 		flag1 = db1.Move();
// 	}
// 	db1.Close();	
// 	CoUninitialize();
	return true;
}

void CDlgExmOption::OnOK() 
{
	UpdateData(TRUE);
	int index = m_subject.GetCurSel();
	if(index < 0 )
	{
		m_stusubject = "";
	}
	else
		m_stusubject = subjectarry[index];

	int index1 = m_address.GetCurSel();
	if(index1 < 0)
	{
		m_examaddr = "";
	}
	else
		m_examaddr = addrarry[index1];

	COleDateTimeSpan timeSpan; 
	timeSpan = m_end - m_start;
	int span = (int)timeSpan.GetTotalSeconds();
	classarry.RemoveAll();
	for(int i = 0; i < m_classlist.GetItemCount() ; i++)
	{
		if(m_classlist.GetCheck(i))
		{
			classarry.Add(m_classlist.GetItemText(i,0));
		}
	}

	int n = classarry.GetSize();
	if(m_account <= 0)
	{
		AfxMessageBox("参加开考人数设置错误！");
		return;
	}
	if(m_maxno <= 0)
	{
		AfxMessageBox("最大题号设置错误！");
		return;
	}
	if(m_startno <= 0)
	{
		AfxMessageBox("开始题号设置错误！");
		return;
	}
	if(m_endno <= 0)
	{
		AfxMessageBox("结束题号设置错误！");
		return;
	}
	if(n == 0)
	{
		AfxMessageBox("没有选择参加考试的班级！");
		return;
	}
	if(m_startno > m_endno)
	{
		AfxMessageBox("结束题号必须大于开始题号！");
		return;
	}
	if(span <= 0)
	{
		AfxMessageBox("收卷时间必须大于开考时间！");
		return;
	}
		CDialog::OnOK();	
}
