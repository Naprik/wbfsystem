// DlgClassQuery.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgClassQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgClassQuery dialog


CDlgClassQuery::CDlgClassQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgClassQuery::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgClassQuery)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgClassQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgClassQuery)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_CLASSLIST, m_classlist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgClassQuery, CDialog)
	//{{AFX_MSG_MAP(CDlgClassQuery)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgClassQuery message handlers

BOOL CDlgClassQuery::OnInitDialog() 
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
	m_classlist.SetHeadings("班级编号,100;班级名称,100; 所属院系,100; 班主任,100"); 
	

// 	CoInitialize(NULL);
// 	CDBDepartment  m_Departmentdb;
// 	if ( !m_Departmentdb.Connect())
// 	{
// 		AfxMessageBox("Departmentdb: 无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		BOOL flag = m_Departmentdb.LoadByAll();
// 		while(flag)
// 		{
// 			if(m_Departmentdb.m_classid != "")
// 				m_classlist.AddItem(0,m_Departmentdb.m_classid,m_Departmentdb.m_class,m_Departmentdb.m_depart,m_Departmentdb.m_classhead);
// 			flag = m_Departmentdb.Move();
// 		}
// 		m_Departmentdb.Close();
// 	}
// 	CoUninitialize();
	
	return TRUE; 
}

void CDlgClassQuery::OnOK() 
{
	// TODO: Add extra validation here
	classarry.RemoveAll();
	for(int i = 0; i < m_classlist.GetItemCount(); i++)
	{
		if(m_classlist.GetCheck(i))
			classarry.Add(m_classlist.GetItemText(i,1));
	}	
	CDialog::OnOK();
}
