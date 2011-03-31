// DlgPaperQuery.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgPaperQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPaperQuery dialog


CDlgPaperQuery::CDlgPaperQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPaperQuery::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPaperQuery)
	m_time = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
}


void CDlgPaperQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPaperQuery)
	DDX_Control(pDX, IDC_PAPERNAME, m_paper);
	DDX_Control(pDX, IDC_CHECKTIME, m_checktime);
	DDX_Control(pDX, IDC_CHECKDEPART, m_checkdepart);
	DDX_Control(pDX, IDC_DATETIMEPICKER, m_timectl);
	DDX_Control(pDX, IDC_CHECKSUBJECT, m_checksub);
	DDX_Control(pDX, IDC_SUBJECT, m_subject);
	DDX_Control(pDX, IDC_DEPART, m_depart);
	DDX_Control(pDX, IDC_CLASS, m_class);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER, m_time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPaperQuery, CDialog)
	//{{AFX_MSG_MAP(CDlgPaperQuery)
	ON_BN_CLICKED(IDC_CHECKSUBJECT, OnChecksubject)
	ON_BN_CLICKED(IDC_CHECKDEPART, OnCheckdepart)
	ON_BN_CLICKED(IDC_CHECKTIME, OnChecktime)
	ON_CBN_SELCHANGE(IDC_DEPART, OnSelchangeDepart)
	ON_CBN_SELCHANGE(IDC_SUBJECT, OnSelchangeSubject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPaperQuery message handlers

void CDlgPaperQuery::OnChecksubject() 
{
	if(m_checksub.GetCheck())
	{
		m_subject.EnableWindow(TRUE);
		m_paper.EnableWindow(TRUE);
	}
	else
	{
		m_subject.EnableWindow(FALSE);
		m_paper.EnableWindow(FALSE);
	}
}

BOOL CDlgPaperQuery::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_subject.EnableWindow(FALSE);
	m_paper.EnableWindow(FALSE);
	m_class.EnableWindow(FALSE);
	m_depart.EnableWindow(FALSE);
	m_timectl.EnableWindow(FALSE);

// 	CoInitialize(NULL);
// 	CDBDepartment  m_Departmentdb;
// 	if ( !m_Departmentdb.Connect())
// 	{
// 		AfxMessageBox("DBDepartmentdb:无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		BOOL flag = m_Departmentdb.LoadByAll();	
// 		while(flag)
// 		{
// 			CString sDepart = m_Departmentdb.m_depart;
// 			sDepart.TrimLeft();
// 			sDepart.TrimRight();
// 			int temp = m_depart.FindStringExact(-1, sDepart);
// 			if(temp == -1)
// 				m_depart.AddString(sDepart);
// 			flag = m_Departmentdb.Move();
// 		}
// 		m_Departmentdb.Close();
// 	}
// 
// 	
// 	CDBSubject	   m_Subjectdb;
// 	if ( !m_Subjectdb.Connect())
// 	{
// 		AfxMessageBox("Subjectdb:无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		BOOL flag = m_Subjectdb.LoadByAll();	
// 		while(flag)
// 		{
// 			CString sSubject = m_Subjectdb.m_subject;
// 			sSubject.TrimLeft();
// 			sSubject.TrimRight();
// 			int temp = m_subject.FindStringExact(-1, sSubject);
// 			if(temp == -1)
// 				m_subject.AddString(sSubject);
// 			flag = m_Subjectdb.Move();
// 		}
// 		m_Subjectdb.Close();
// 	}
// 	CoUninitialize();
	
	
	return TRUE;  
}

void CDlgPaperQuery::OnCheckdepart() 
{
	if(m_checkdepart.GetCheck())
	{
		m_depart.EnableWindow(TRUE);
		m_class.EnableWindow(TRUE);
	}
	else
	{
		m_depart.EnableWindow(FALSE);
		m_class.EnableWindow(FALSE);
	}
}

void CDlgPaperQuery::OnChecktime() 
{
	if(m_checktime.GetCheck())
		m_timectl.EnableWindow(TRUE);
	else
		m_timectl.EnableWindow(FALSE);
}

void CDlgPaperQuery::OnSelchangeDepart() 
{
	m_class.ResetContent();
	int index = m_depart.GetCurSel();
	if(index != -1)
		m_depart.GetLBText(index,strDepart);

	strDepart.TrimLeft();
	strDepart.TrimRight();
	
// 	CoInitialize(NULL);
// 	CDBDepartment  m_Departmentdb;
// 	if ( !m_Departmentdb.Connect())
// 		AfxMessageBox("DBDepartmentdb:无法连接到数据库服务器");
// 	else
// 	{
// 		BOOL flag = m_Departmentdb.LoadByDepart(strDepart);
// 		m_class.AddString("全部");
// 		while(flag)
// 		{
// 			CString sClass = m_Departmentdb.m_class;
// 			sClass.TrimLeft();
// 			sClass.TrimRight();
// 			if(sClass != "")
// 			{
// 				int temp = m_class.FindStringExact(-1, sClass);
// 				if(temp == -1)
// 					m_class.AddString(sClass);
// 			}
// 			flag = m_Departmentdb.Move();
// 		}
// 		m_Departmentdb.Close();
// 	}
// 	CoUninitialize();	
}

void CDlgPaperQuery::OnOK() 
{
	UpdateData(TRUE);
	strSubject = "";
	strDepart = "";
	strClass = "";
	strPaper = "";

	if(!m_checktime.GetCheck())
		bIstime = FALSE;
	else
		bIstime = TRUE;
	int index = m_subject.GetCurSel();
	if(index != -1)
		m_subject.GetLBText(index,strSubject);

	index = m_paper.GetCurSel();
	if(index != -1)
	{
		m_paper.GetLBText(index,strPaper);
		if(strPaper == "全部")
			strPaper = "";
	}
	
	index = m_depart.GetCurSel();
	if(index != -1)
		m_depart.GetLBText(index,strDepart);

	index = m_class.GetCurSel();
	if(index != -1)
	{
		m_class.GetLBText(index,strClass);
//		if(strClass == "全部")
//			strClass = "";
	}

//	AfxMessageBox(m_time.Format());
	
	CDialog::OnOK();
}

void CDlgPaperQuery::OnSelchangeSubject() 
{
	m_paper.ResetContent();
	int index = m_subject.GetCurSel();
	if(index != -1)
		m_subject.GetLBText(index,strSubject);

	strSubject.TrimLeft();
	strSubject.TrimRight();
	
// 	CoInitialize(NULL);
// 	CDBPaper	   m_Paperdb;
// 	if ( !m_Paperdb.Connect())
// 		AfxMessageBox("Paperdb:无法连接到数据库服务器");
// 	else
// 	{
// 		BOOL flag = m_Paperdb.LoadBySubject(strSubject);
// 		m_paper.AddString("全部");
// 		while(flag)
// 		{
// 			CString sPaper = m_Paperdb.m_papername;
// 			sPaper.TrimLeft();
// 			sPaper.TrimRight();
// 			if(sPaper != "")
// 			{
// 				int temp = m_paper.FindStringExact(-1, sPaper);
// 				if(temp == -1)
// 					m_paper.AddString(sPaper);
// 			}
// 			flag = m_Paperdb.Move();
// 		}
// 		m_Paperdb.Close();
// 	}
// 	CoUninitialize();		
	
}
