// DlgLinkrolling.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgLinkrolling.h"
#include "ExamApp.h"
#include "TeacherDev.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CArray<CTeacherDev, CTeacherDev>  arryTeacherdev;
extern CExamApp m_app;
/////////////////////////////////////////////////////////////////////////////
// CDlgLinkrolling dialog


CDlgLinkrolling::CDlgLinkrolling(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLinkrolling::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLinkrolling)
	//}}AFX_DATA_INIT
}


void CDlgLinkrolling::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLinkrolling)
	DDX_Control(pDX, IDC_STULIST, m_stulist);
	DDX_Control(pDX, IDC_SEACH, m_search);
	DDX_Control(pDX, IDC_LINKLIST, m_linklist);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_LINKROLLING, m_linkrolling);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLinkrolling, CDialog)
	//{{AFX_MSG_MAP(CDlgLinkrolling)
	ON_BN_CLICKED(IDC_LINKROLLING, OnLinkrolling)
	ON_BN_CLICKED(IDC_SEACH, OnSeach)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLinkrolling message handlers
BOOL CDlgLinkrolling::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwStyle=GetWindowLong(m_stulist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_stulist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_stulist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_stulist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_stulist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_stulist.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	
	m_stulist.ClearHeadings();
	m_stulist.SetHeadings("学号,150;姓名,100;班级,100;准考证号,100;当前状态,150;开考时间,150;交卷时间,150");

	dwStyle=GetWindowLong(m_linklist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_linklist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_linklist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_linklist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_linklist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_linklist.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	
	m_linklist.ClearHeadings();
	m_linklist.SetHeadings("学号,150;姓名,100;班级,100;准考证号,100;当前状态,150;开考时间,150;交卷时间,150;使用端口,100");

// 	CoInitialize(NULL);
// 	CDBStuInfo m_StuInfodb;
// 	if(!m_StuInfodb.Connect())
// 	{
// 		AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		m_stulist.DeleteAllItems();
// 		for(int j = 0; j < classarry.GetSize(); j++)
// 		{
// 			int flag = m_StuInfodb.LoadByClass(classarry[j]);
// 			while(flag)
// 			{
// 				m_stulist.AddItem(0,m_StuInfodb.m_stuid,m_StuInfodb.m_stuname,m_StuInfodb.m_Stuclass,"","","","");
// 				flag = m_StuInfodb.Move();
// 			}
// 
// 		}
// 		m_StuInfodb.Close();
// 	}
// 	CoUninitialize();

	return true;

	m_linkrolling.EnableWindow(FALSE);
}

void CDlgLinkrolling::OnLinkrolling() 
{
	if(m_linklist.GetCheck(0))
	{
		m_app.OpenStucom();
		CString stuid = m_linklist.GetItemText(0,0);
		CString strAnswer = m_app.LinkRolling(stuid,iStart,iEnd,iPaper,sAddr);
		if(strAnswer != "")
		{
		//	AfxMessageBox("收卷成功");
			COleDateTime current;
			current = COleDateTime::GetCurrentTime();
			m_linklist.SetItemText(0,6,current.Format());
			m_linklist.SetItemText(0,4,"收卷成功(联机)");
/*			CoInitialize(NULL);
			CDBResult m_Resultdb;
			if(!m_Resultdb.Connect())
			{
				AfxMessageBox("Resultdb:无法连接到数据库服务器");
			}
			else
			{
				m_Resultdb.m_ydid = 0;
				m_Resultdb.m_stuid = m_linklist.GetItemText(0,0);
				m_Resultdb.m_paperid = iPaper;
				m_Resultdb.m_starttime = m_starttime;
				m_Resultdb.m_endtime = current;
				m_Resultdb.m_stuanswer = strAnswer;
				m_Resultdb.m_addr = sAddr;
				if(m_Resultdb.Add())
				{
					m_app.LinkSuccess();
					AfxMessageBox("收卷成功");
				}
				else
					AfxMessageBox("收卷失败！");
					
				m_Resultdb.Close();
			}
			CoUninitialize();*/
//			m_app.LinkSuccess();
		}
	}
	else
		AfxMessageBox("没有选择学生机！");
}

void CDlgLinkrolling::OnSeach() 
{
	m_linklist.DeleteAllItems();	
	if(m_app.TestStu())
	{
		int i = arryTeacherdev.GetSize();
		if(i <= 0)
			AfxMessageBox("没有搜索到学生机！");
		else
		{
			CString strCom = arryTeacherdev[0].m_com;
			strCom.TrimLeft();
			strCom.TrimRight();
			CString strMac = arryTeacherdev[0].m_mac;
			strMac.TrimLeft();
			strMac.TrimRight();
// 			CoInitialize(NULL);
// 			CDBStuInfo m_StuInfodb;
// 			if(!m_StuInfodb.Connect())
// 			{
// 				AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 			}
// 			else
// 			{
// 				strMac.MakeLower();
// 				BOOL flag = m_StuInfodb.LoadByDev(strMac);
// 				if(flag)
// 				{
// 					m_linklist.AddItem(0,
// 										m_StuInfodb.m_stuid,
// 										m_StuInfodb.m_stuname,
// 										m_StuInfodb.m_Stuclass,
// 										"",
// 										"联机成功",
// 										"",
// 										"",
// 										strCom);
// 					m_linklist.SetCheck(0);
// 				}
// 				m_linkrolling.EnableWindow(TRUE);
// 				m_StuInfodb.Close();
// 			}
// 			CoUninitialize();
		}
	}
	else
		AfxMessageBox("没有搜索到学生机！");	
}

void CDlgLinkrolling::OnOK() 
{
	sStuid = m_linklist.GetItemText(0,0);
	sEndTime = m_linklist.GetItemText(0,6);
	sStat = m_linklist.GetItemText(0,4);	
	CDialog::OnOK();
}
