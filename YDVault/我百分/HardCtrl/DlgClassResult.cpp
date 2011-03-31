// DlgClassResult.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgClassResult.h"
#include "DBSubject.h"
#include "DBDepartment.h"
#include "DBPaper.h"
#include "DBClass.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgClassResult dialog


CDlgClassResult::CDlgClassResult(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgClassResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgClassResult)
	//}}AFX_DATA_INIT
}


void CDlgClassResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgClassResult)
	DDX_Control(pDX, IDC_SUBJECT, m_subject);
	DDX_Control(pDX, IDC_PAPER, m_paper);
	DDX_Control(pDX, IDC_DEPART, m_depart);
	DDX_Control(pDX, IDC_CLASS, m_class);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgClassResult, CDialog)
	//{{AFX_MSG_MAP(CDlgClassResult)
	ON_CBN_SELCHANGE(IDC_DEPART, OnSelchangeDepart)
	ON_CBN_SELCHANGE(IDC_SUBJECT, OnSelchangeSubject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgClassResult message handlers
BOOL CDlgClassResult::OnInitDialog()
{
	CDialog::OnInitDialog();
	CoInitialize(NULL);
	CDBDepartment  m_Departmentdb;
	if ( !m_Departmentdb.Connect())
	{
		AfxMessageBox("DBDepartmentdb:无法连接到数据库服务器");
	}
	else
	{
		BOOL flag = m_Departmentdb.LoadByAll();	
		while(flag)
		{
			CString sDepart = m_Departmentdb.m_departname;
			sDepart.TrimLeft();
			sDepart.TrimRight();
			int temp = m_depart.FindStringExact(-1, sDepart);
			if(temp == -1)
				m_depart.AddString(sDepart);
			flag = m_Departmentdb.Move();
		}
		m_Departmentdb.Close();
	}
 
	CDBSubject	   m_Subjectdb;
	if ( !m_Subjectdb.Connect())
	{
		AfxMessageBox("Subjectdb:无法连接到数据库服务器");
	}
	else
	{
		BOOL flag = m_Subjectdb.LoadByAll();	
		while(flag)
		{
			CString sSubject = m_Subjectdb.m_subject;
			sSubject.TrimLeft();
			sSubject.TrimRight();
			int temp = m_subject.FindStringExact(-1, sSubject);
			if(temp == -1)
				m_subject.AddString(sSubject);
			flag = m_Subjectdb.Move();
		}
		m_Subjectdb.Close();
	}
	CoUninitialize();

	return true;
}

void CDlgClassResult::OnOK() 
{
	UpdateData(TRUE);
	strSubject = "";
	strDepart = "";
	strClass = "";
	strPaper = "";
	int index = m_depart.GetCurSel();
	if(index != -1)
		m_depart.GetLBText(index, strDepart);
	
	index = m_class.GetCurSel();
	if(index != -1)
	{
		m_class.GetLBText(index, strClass);
		if(strClass == "全部")
			strClass = "";
	}
	
	index = m_subject.GetCurSel();
	if(index != -1)
		m_subject.GetLBText(index, strSubject);

	index = m_paper.GetCurSel();
	if(index != -1)
	{
		m_paper.GetLBText(index, strPaper);
		if(strPaper == "全部")
			strPaper = "";
	}

	if (strDepart == "")
	{
		AfxMessageBox("请选择需要查询的院系！");
		return;
	}
	if (strClass == "")
	{
		AfxMessageBox("请选择需要查询的班级！");
		return;
	}
	if (strSubject == "")
	{
		AfxMessageBox("请选择需要查询的考试科目！");
		return;
	}
	if(strPaper == "")
	{
		AfxMessageBox("请选择需要查询的考试试卷！");
		return;
	}

	CDialog::OnOK();
}

void CDlgClassResult::OnSelchangeDepart() 
{
	m_class.ResetContent();
	int index = m_depart.GetCurSel();
	if(index != -1)
		m_depart.GetLBText(index,strDepart);

	strDepart.TrimLeft();
	strDepart.TrimRight();

	CString strDepartid = "";
	CoInitialize(NULL);
	CDBDepartment  m_Departmentdb;
	if ( !m_Departmentdb.Connect())
		AfxMessageBox("DBDepartmentdb:无法连接到数据库服务器");
	else
	{
		BOOL flag = m_Departmentdb.LoadByDepart(strDepart);
		if (flag)
		{
			strDepartid = m_Departmentdb.m_departid;
		}
		m_Departmentdb.Close();
	}

	CDBClass m_classdb;
	if (!m_classdb.Connect())
		AfxMessageBox("DBDepartmentdb:无法连接到数据库服务器");
	else
	{
		BOOL flag = m_classdb.LoadByDepartid(strDepartid);
		while (flag)
		{
			CString sClass = m_classdb.m_classname;
			sClass.TrimLeft();
			sClass.TrimRight();
			if(sClass != "")
			{
				int temp = m_class.FindStringExact(-1, sClass);
				if(temp == -1)
					m_class.AddString(sClass);
			}
			flag = m_classdb.Move();
		}
		m_classdb.Close();
	}
	CoUninitialize();	
}

void CDlgClassResult::OnSelchangeSubject() 
{
	m_paper.ResetContent();
	int index = m_subject.GetCurSel();
	if(index != -1)
		m_subject.GetLBText(index,strSubject);

	strSubject.TrimLeft();
	strSubject.TrimRight();

	CString strSubjectid = "";
	CoInitialize(NULL);
	CDBSubject m_subjectdb;
	if (!m_subjectdb.Connect())
		AfxMessageBox("m_subjectdb:无法连接到数据库服务器");
	else
	{
		BOOL flag = m_subjectdb.LoadBySubject(strSubject);
		if (flag)
		{
			strSubjectid = m_subjectdb.m_subjectid;
		}	
		m_subjectdb.Close();
	}

	CDBPaper m_Paperdb;
	if(!m_Paperdb.Connect())
		AfxMessageBox("Paperdb:无法连接到数据库服务器");
	else
	{
		BOOL flag = m_Paperdb.LoadBySubjectid(strSubjectid);
		while(flag)
		{
			CString sPaper = m_Paperdb.m_papername;
			sPaper.TrimLeft();
			sPaper.TrimRight();
			if(sPaper != "")
			{
				int temp = m_paper.FindStringExact(-1, sPaper);
				if(temp == -1)
					m_paper.AddString(sPaper);
			}
			flag = m_Paperdb.Move();
		}
		
		m_Paperdb.Close();
	}
	CoUninitialize();
}
