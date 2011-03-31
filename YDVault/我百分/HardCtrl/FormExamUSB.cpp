#include "stdafx.h"
#include "hardctrl.h"
#include "FormExamUSB.h"
#include "MainFrm.h"
#include "ExamDetail.h"
#include "ExamApp.h"
#include "TeacherDev.h"

extern CHardCtrlApp theApp;
extern CExamDetail  m_examdeteil;
extern CExamApp m_app;
extern CArray<CTeacherDev, CTeacherDev>  arryTeacherdev;
IMPLEMENT_DYNCREATE(CFormExamUSB, CDialog)


CFormExamUSB::CFormExamUSB(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormExamUSB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormExamUSB)
	m_systime = _T("");
	//}}AFX_DATA_INIT
}


void CFormExamUSB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamUSB)
	DDX_Control(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_ROLLCALL, m_roll);
	DDX_Control(pDX, IDC_STULIST, m_stulist);
	DDX_Text(pDX, IDC_STATIC17, m_systime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamUSB, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormExamUSB)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_BN_CLICKED(IDC_ROLLCALL, OnRollcall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamUSB message handlers

BOOL CFormExamUSB::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();

	AddSzControl(*GetDlgItem(IDC_STATIC1),mdResize,mdNone);
	AddSzControl(*GetDlgItem(IDC_STATIC18),mdResize,mdNone);
	AddSzControl(m_stulist,mdResize,mdResize);	
	
	AddSzControl(*GetDlgItem(IDC_SEARCH),mdRepos,mdRepos);
	AddSzControl(*GetDlgItem(IDC_ROLLCALL),mdRepos,mdRepos);

	CRect btnRect;  
	m_search.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_search.SetBitmapId(IDB_SEARCH,IDB_SEARCH,IDB_SEARCH_DOWN,IDB_SEARCH); 	

	m_roll.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_roll.SetBitmapId(IDB_ROLL,IDB_ROLL,IDB_ROLL_DOWN,IDB_ROLL); 

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

	m_stulist.SetHeadings("学号,150;姓名,100;班级,100;准考证号,100;当前状态,150;开考时间,150;交卷时间,150;学生蓝牙地址,100");
	SetTimer(1,1000,NULL);
	
	return TRUE;  
}

void CFormExamUSB::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 1)
	{
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
		m_systime.Format("%2d:%2d:%2d",sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
		UpdateData(FALSE);
	}
	cdxCSizingDialog::OnTimer(nIDEvent);
}

void  CFormExamUSB::SetKind(int kind)
{
	m_kind = kind;
}

void CFormExamUSB::SetInfo()
{
// 	CoInitialize(NULL);
// 	CDBStuInfo m_StuInfodb;
// 	if(!m_StuInfodb.Connect())
// 	{
// 		AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		m_stulist.DeleteAllItems();
// 		for(int j = 0; j < m_examdeteil.arryClass.GetSize(); j++)
// 		{
// 			int flag = m_StuInfodb.LoadByClass(m_examdeteil.arryClass[j]);
// 			while(flag)
// 			{
// 				m_stulist.AddItem(0,m_StuInfodb.m_stuid,m_StuInfodb.m_stuname,m_StuInfodb.m_Stuclass,"","","","",m_StuInfodb.m_devid);
// 				flag = m_StuInfodb.Move();
// 			}
// 
// 		}
// 		m_StuInfodb.Close();
// 	}
// 	CoUninitialize();

	GetDlgItem(IDC_STATIC3)->SetWindowText(m_examdeteil.sPapername);
	GetDlgItem(IDC_STATIC5)->SetWindowText(m_examdeteil.sSubject);
	GetDlgItem(IDC_STATIC7)->SetWindowText(m_examdeteil.sAddress);
	CString sClass = "";
	for(int i = 0; i < m_examdeteil.arryClass.GetSize(); i++)
	{
		sClass += m_examdeteil.arryClass[i] + "\n";
	}
	GetDlgItem(IDC_STATIC9)->SetWindowText(sClass);
	int m_account = m_stulist.GetItemCount();
	CString sCount;
	sCount.Format("%d 人", m_account);
	GetDlgItem(IDC_STATIC11)->SetWindowText(sCount);
	GetDlgItem(IDC_STATIC13)->SetWindowText(m_examdeteil.sStart);
	GetDlgItem(IDC_STATIC15)->SetWindowText(m_examdeteil.sEnd);
}

void CFormExamUSB::OnSearch() 
{
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
// 					GetDlgItem(IDC_STATIC20)->SetWindowText(m_StuInfodb.m_stuid);
// 					GetDlgItem(IDC_STATIC22)->SetWindowText(m_StuInfodb.m_stuname);
// 					GetDlgItem(IDC_STATIC24)->SetWindowText(m_StuInfodb.m_Stugender);
// 					GetDlgItem(IDC_STATIC26)->SetWindowText(m_StuInfodb.m_Stuclass);
// 				}
// 				m_StuInfodb.Close();
// 			}
// 			CoUninitialize();
		}
	}
	else
		AfxMessageBox("没有搜索到学生机！");	
}

void CFormExamUSB::OnRollcall() 
{
	
}
