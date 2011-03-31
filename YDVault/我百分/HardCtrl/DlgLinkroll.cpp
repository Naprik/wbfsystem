// DlgLinkroll.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgLinkroll.h"
#include "ExamApp.h"
#include "TeacherDev.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CArray<CTeacherDev, CTeacherDev>  arryTeacherdev;
/////////////////////////////////////////////////////////////////////////////
// CDlgLinkroll dialog

extern CExamApp m_app;

CDlgLinkroll::CDlgLinkroll(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLinkroll::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLinkroll)
	//}}AFX_DATA_INIT
}


void CDlgLinkroll::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLinkroll)
	DDX_Control(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_EXAMLIST, m_examlist);
	DDX_Control(pDX, IDC_LIST, m_stulist);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_LINKROLL, m_linkroll);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLinkroll, CDialog)
	//{{AFX_MSG_MAP(CDlgLinkroll)
	ON_BN_CLICKED(IDC_LINKROLL, OnLinkroll)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLinkroll message handlers

BOOL CDlgLinkroll::OnInitDialog()
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

	dwStyle=GetWindowLong(m_examlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_examlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_examlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_examlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_examlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_examlist.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	
	m_examlist.ClearHeadings();
	m_examlist.SetHeadings("学号,150;姓名,100;班级,100;准考证号,100;当前状态,150;开考时间,150;交卷时间,150;使用端口,100");

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

	m_linkroll.EnableWindow(FALSE);
	return true;
}

void CDlgLinkroll::OnLinkroll() 
{
	if(m_examlist.GetCheck(0))
	{
		CString stuid = m_examlist.GetItemText(0,0);
		m_app.OpenStucom();
		if(m_app.Roll_link(stuid,m_starttime,m_endtime,m_maxNO))
		{
			AfxMessageBox("开考成功");
			COleDateTime tStart;
			tStart = m_app.Get_start_time(stuid);
			m_examlist.SetItemText(0,5,tStart.Format());
			m_examlist.SetItemText(0,4,"开考成功(联机)");
			m_app.CloseStucom();
		}
		else
			AfxMessageBox("开考失败");
	}
	else
		AfxMessageBox("没有选择学生机！");
}

void CDlgLinkroll::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item =pNMListView->iItem;
	CString strStuID = m_stulist.GetItemText(item,0);
	if(strStuID != "")
	{
		m_examlist.DeleteAllItems();
		m_examlist.AddItem(0,
							m_stulist.GetItemText(item,0),
							m_stulist.GetItemText(item,1),
							m_stulist.GetItemText(item,2),
							m_stulist.GetItemText(item,3),
							m_stulist.GetItemText(item,4),
							m_stulist.GetItemText(item,5),
							m_stulist.GetItemText(item,6),
							"");
	}
	
	*pResult = 0;
}

void CDlgLinkroll::OnSearch() 
{
	m_examlist.DeleteAllItems();	
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
// 					m_examlist.AddItem(0,
// 										m_StuInfodb.m_stuid,
// 										m_StuInfodb.m_stuname,
// 										m_StuInfodb.m_Stuclass,
// 										"",
// 										"联机成功",
// 										"",
// 										"",
// 										strCom);
// 					m_examlist.SetCheck(0);
// 				}
// 				m_linkroll.EnableWindow(TRUE);
// 
// 				m_StuInfodb.Close();
// 			}
// 			CoUninitialize();
		}
	}
	else
		AfxMessageBox("没有搜索到学生机！");
}

void CDlgLinkroll::OnOK() 
{
	sStuid = m_examlist.GetItemText(0,0);
	sStartTime = m_examlist.GetItemText(0,5);
	sStat = m_examlist.GetItemText(0,4);
	
	CDialog::OnOK();
}
