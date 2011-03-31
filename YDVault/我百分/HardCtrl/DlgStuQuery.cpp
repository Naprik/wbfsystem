// DlgStuQuery.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgStuQuery.h"
#include "DBDepartment.h"
#include "DBClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStuQuery dialog


CDlgStuQuery::CDlgStuQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStuQuery::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStuQuery)
	m_stuid = _T("");
	m_stuname = _T("");
	//}}AFX_DATA_INIT
}


void CDlgStuQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStuQuery)
	DDX_Control(pDX, IDC_CLASS, m_class);
	DDX_Control(pDX, IDC_STUDNAME, m_name);
	DDX_Control(pDX, IDC_STUDID, m_id);
	DDX_Text(pDX, IDC_STUDID, m_stuid);
	DDX_Text(pDX, IDC_STUDNAME, m_stuname);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStuQuery, CDialog)
	//{{AFX_MSG_MAP(CDlgStuQuery)
	ON_BN_CLICKED(IDC_RADIOSTUID, OnRadiostuid)
	ON_BN_CLICKED(IDC_RADIOSTUNAME, OnRadiostuname)
	ON_BN_CLICKED(IDC_RADIOCLASS, OnRadioclass)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStuQuery message handlers

void CDlgStuQuery::OnRadiostuid() 
{
	GetDlgItem(IDC_STATIC1)->SetWindowText("学号");
	m_id.ShowWindow(TRUE);
	m_name.ShowWindow(FALSE);
	m_class.ShowWindow(FALSE);
}

void CDlgStuQuery::OnRadiostuname() 
{
	GetDlgItem(IDC_STATIC1)->SetWindowText("姓名");
	m_id.ShowWindow(FALSE);
	m_name.ShowWindow(TRUE);
	m_class.ShowWindow(FALSE);	
	
}

void CDlgStuQuery::OnRadioclass() 
{
	GetDlgItem(IDC_STATIC1)->SetWindowText("班级");
	m_id.ShowWindow(FALSE);
	m_name.ShowWindow(FALSE);
	m_class.ShowWindow(TRUE);
}

BOOL CDlgStuQuery::OnInitDialog() 
{
	CDialog::OnInitDialog();
	((CButton *)GetDlgItem(IDC_RADIOSTUID))->SetCheck(TRUE);
	GetDlgItem(IDC_STATIC1)->SetWindowText("学号");
	m_id.ShowWindow(TRUE);
	m_name.ShowWindow(FALSE);
	m_class.ShowWindow(FALSE);

	CoInitialize(NULL);
	CDBClass m_classdb;
	if (!m_classdb.Connect())
	{
		AfxMessageBox("DBDepartmentdb:无法连接到数据库服务器");
	}
	else
	{
		BOOL flag = m_classdb.LoadByAll();
		while (flag)
		{
			CString strClass = m_classdb.m_classname;
			strClass.TrimLeft();
			strClass.TrimRight();
			int temp = m_class.FindStringExact(-1, strClass);
			if(temp == -1 && strClass != "")
			{
				m_class.AddString(strClass);
 			}
			flag = m_classdb.Move();
		}

		m_classdb.Close();
	}

	CoUninitialize();

// 	CoInitialize(NULL);
// 	CDBDepartment DBDepartmentdb;
// 	if ( !DBDepartmentdb.Connect())
// 	{
// 		AfxMessageBox("DBDepartmentdb:无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		BOOL flag = DBDepartmentdb.LoadByAll();
// 		while(flag)
// 		{
// 			CString strClass = DBDepartmentdb.m_class;
// 			strClass.TrimLeft();
// 			strClass.TrimRight();
// 			int temp = m_class.FindStringExact(-1, strClass);
// 			if(temp == -1 && strClass != "")
// 			{
// 				m_class.AddString(strClass);
// 			}
// 			flag = DBDepartmentdb.Move();
// 		}
// 		DBDepartmentdb.Close();
// 	}
// 	CoUninitialize();
	
	return TRUE;  
}

void CDlgStuQuery::OnOK() 
{
	UpdateData(TRUE);
	if(((CButton *)GetDlgItem(IDC_RADIOSTUID))->GetCheck())
	{
		m_stuid.TrimLeft();
		m_stuid.TrimRight();
		if(m_stuid == "")
		{
			AfxMessageBox("请输入要查询的学生学号！");
			return;
		}
	}
	else if(((CButton *)GetDlgItem(IDC_RADIOSTUNAME))->GetCheck())
	{
		m_stuname.TrimLeft();
		m_stuname.TrimRight();
		if(m_stuname == "")
		{
			AfxMessageBox("请输入要查询的学生姓名！");
			return;
		}
	}
	else
	{
		int index = m_class.GetCurSel();
		if(index != -1)
			m_class.GetLBText(index,strClass);
		else
		{
			AfxMessageBox("请选择要查询的班级！");
			return;			
		}
	}
	
	CDialog::OnOK();
}
