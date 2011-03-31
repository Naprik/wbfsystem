// DlgStunfo.cpp : implementation file
//
#pragma once
#include "stdafx.h"
#include "hardctrl.h"
#include "DlgStunfo.h"
#include "DBDepartment.h"
#include "DBClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStunfo dialog


CDlgStunfo::CDlgStunfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStunfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStunfo)
	m_email = _T("");
	m_name = _T("");
	m_num = _T("");
	m_tel = _T("");
	m_blueteeth = _T("");
	//}}AFX_DATA_INIT
}


void CDlgStunfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStunfo)
	DDX_Control(pDX, IDC_DEPARTMENT, m_department);
	DDX_Control(pDX, IDC_CLASS, m_class);
	DDX_Control(pDX, IDC_BUTTON1, m_button);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_GENDER, m_gender);
	DDX_Text(pDX, IDC_EMIAL, m_email);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_NUM, m_num);
	DDX_Text(pDX, IDC_TEL, m_tel);
	DDX_Text(pDX, IDC_BLUETEETH, m_blueteeth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStunfo, CDialog)
	//{{AFX_MSG_MAP(CDlgStunfo)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_DEPARTMENT, OnSelchangeDepartment)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStunfo message handlers

BOOL CDlgStunfo::OnInitDialog()
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
			int temp = m_department.FindStringExact(-1, sDepart);
			if(temp == -1)
				m_department.AddString(sDepart);
			flag = m_Departmentdb.Move();
		}
		m_Departmentdb.Close();
	}
	CoUninitialize();

	m_gender.AddString("男");
	m_gender.AddString("女");

	if(strDepart != "" )
	{	
		int departindex = m_department.FindStringExact(0, strDepart);
		m_department.SetCurSel(departindex);
	}

	if(strSex != "")
	{
		int sexindex = m_gender.FindStringExact(0, strSex);
		m_gender.SetCurSel(sexindex);
	}
	if(strClass != "")
	{
		int classindex = m_class.FindStringExact(0, strClass);
		m_class.SetCurSel(classindex);
	}
	strPath.Replace("\\", "\\\\");
	CButton   *aa=(CButton*)GetDlgItem(IDC_PHOTO);  
	CString	  path = strPath;
	HBITMAP   hbmp=(HBITMAP)::LoadImage(NULL,path,IMAGE_BITMAP,180,200,LR_LOADFROMFILE);   
	 aa->SetBitmap(hbmp);
	return TRUE;
}

void CDlgStunfo::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	int index = m_class.GetCurSel();
	if(index != -1)
		m_class.GetLBText(index,strClass);
	index = m_gender.GetCurSel();
	if(index != -1)
		m_gender.GetLBText(index,strSex);

	if(m_num == "")
	{
		AfxMessageBox("没有输入学生学号，请输入学生学号！");
		return;
	}
	if(m_name == "")
	{
		AfxMessageBox("没有输入学生姓名，请输入学生姓名！");
		return;
	}
	if(m_name == "")
	{
		AfxMessageBox("没有输入学生姓名，请输入学生姓名！");
		return;
	}
	if(strSex == "")
	{
		AfxMessageBox("没有选择学生性别，请选择学生性别！");
		return;
	}
	if(strDepart == "")
	{
		AfxMessageBox("没有选择学生院系，请选择学生院系！");
		return;
	}
	if(strClass == "")
	{
		AfxMessageBox("没有选择学生班级，请选择学生班级！");
		return;
	}

	CDialog::OnOK();
}
extern CString GetExePath();


void CDlgStunfo::OnButton1() 
{
	static char BASED_CODE szFilter[] = "All Files (*.bmp)|*.bmp||";
  	CFileDialog dlg(TRUE,NULL,"*.bmp",NULL,szFilter);

	if ( dlg.DoModal() == IDOK)
	{
		strPath = dlg.GetPathName();
		strPath.Replace("\\", "\\\\");
		CButton   *aa=(CButton*)GetDlgItem(IDC_PHOTO);  
		CString	  path = strPath;
		HBITMAP   hbmp=(HBITMAP)::LoadImage(NULL,path,IMAGE_BITMAP,180,200,LR_LOADFROMFILE);   
		 aa->SetBitmap(hbmp); 
	}	
}


void CDlgStunfo::OnSelchangeDepartment() 
{
	m_class.ResetContent();
	int index = m_department.GetCurSel();
	if(index != -1)
		m_department.GetLBText(index,strDepart);

	strDepart.TrimLeft();
	strDepart.TrimRight();
	CString strDepartid = "";
	CoInitialize(NULL);
	CDBDepartment  m_Departmentdb;
	if (!m_Departmentdb.Connect())
	{
		AfxMessageBox("DBDepartmentdb:无法连接到数据库服务器");
	}
	else
	{
		BOOL flag = m_Departmentdb.LoadByDepart(strDepart);
		if (flag)
		{
			strDepartid = m_Departmentdb.m_departid;
		}
		m_Departmentdb.Close();
	}

	if (strDepartid != "")
	{
		CDBClass m_classdb;
		if (!m_classdb.Connect())
		{
			AfxMessageBox("m_classdb:无法连接到数据库服务器");
		}
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
	}

	CoUninitialize();
// 	CoInitialize(NULL);
// 	CDBDepartment  m_Departmentdb;
// 	if ( !m_Departmentdb.Connect())
// 		AfxMessageBox("DBDepartmentdb:无法连接到数据库服务器");
// 	else
// 	{
// 		BOOL flag = m_Departmentdb.LoadByDepart(strDepart);
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
