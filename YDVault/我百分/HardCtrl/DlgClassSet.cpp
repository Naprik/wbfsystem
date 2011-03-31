// DlgClassSet.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgClassSet.h"
#include "DBDepartment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgClassSet dialog


CDlgClassSet::CDlgClassSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgClassSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgClassSet)
	m_classid = _T("");
	m_class = _T("");
	m_classheadtel = _T("");
	m_classhead = _T("");
	//}}AFX_DATA_INIT
}


void CDlgClassSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgClassSet)
	DDX_Control(pDX, IDC_DEPART, m_depart);
	DDX_Text(pDX, IDC_CLASSID, m_classid);
	DDX_Text(pDX, IDC_CLASS, m_class);
	DDX_Text(pDX, IDC_CLASSHEADTEL, m_classheadtel);
	DDX_Text(pDX, IDC_CLASSHEAD, m_classhead);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgClassSet, CDialog)
	//{{AFX_MSG_MAP(CDlgClassSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgClassSet message handlers

void CDlgClassSet::OnOK() 
{
	UpdateData(TRUE);
	int index = m_depart.GetCurSel();
	if(index != -1)
		m_depart.GetLBText(index,strDepart);
	else
	{
		AfxMessageBox("没有选择院系，请选择院系！");
		return;
	}
	if(m_classid == "")
	{
		AfxMessageBox("没有输入班级编号，请输入班级编号！");
		return;
	}
	if(m_class == "")
	{

		AfxMessageBox("没有输入班级名称，请输入班级名称！");
		return;
	}
	CDialog::OnOK();
}

BOOL CDlgClassSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CoInitialize(NULL);
	CDBDepartment m_departdb;
	if (!m_departdb.Connect())
	{
		AfxMessageBox("DBDepartmentdb:无法连接到数据库服务器");
	}
	else
	{
		BOOL flag = m_departdb.LoadByAll();
		while (flag)
		{
			CString strDepart = m_departdb.m_departname;
			strDepart.TrimLeft();
			strDepart.TrimRight();
			int temp = m_depart.FindStringExact(-1, strDepart);
			if(temp == -1)
			{
				m_depart.AddString(strDepart);
			}
 			flag = m_departdb.Move();
		}
		m_departdb.Close();
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
// 			CString strDepart = DBDepartmentdb.m_depart;
// 			strDepart.TrimLeft();
// 			strDepart.TrimRight();
// 			int temp = m_depart.FindStringExact(-1, strDepart);
// 			if(temp == -1)
// 			{
// 				m_depart.AddString(strDepart);
// 			}
// 			flag = DBDepartmentdb.Move();
// 		}
// 		DBDepartmentdb.Close();
// 	}
// 	CoUninitialize();
	strDepart = "";
	return TRUE; 
}
