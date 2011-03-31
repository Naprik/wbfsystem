#include "stdafx.h"
#include "hardctrl.h"
#include "FormExamInfrar.h"
#include "MainFrm.h"
#include "ExamDetail.h"

extern CHardCtrlApp theApp;
extern CExamDetail  m_examdeteil;
IMPLEMENT_DYNCREATE(CFormExamInfrar, CDialog)


CFormExamInfrar::CFormExamInfrar(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormExamInfrar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormExamInfrar)
	m_systime = _T("");
	//}}AFX_DATA_INIT
}


void CFormExamInfrar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamInfrar)
	DDX_Control(pDX, IDC_UP, m_up);
	DDX_Control(pDX, IDC_OK, m_ok);
	DDX_Control(pDX, IDC_STULIST, m_stulist);
	DDX_Text(pDX, IDC_STATIC17, m_systime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamInfrar, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormExamInfrar)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_UP, OnUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamInfrar message handlers

BOOL CFormExamInfrar::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();

	CRect btnRect;  
	m_up.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_up.SetBitmapId(IDB_UP,IDB_UP,IDB_UP_DOWN,IDB_UP); 

	m_ok.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_ok.SetBitmapId(IDB_OK,IDB_OK,IDB_OK_DOWN,IDB_OK); 

	AddSzControl(*GetDlgItem(IDC_STATIC1),mdResize,mdNone);
	AddSzControl(m_stulist,mdResize,mdResize);	
	
	AddSzControl(*GetDlgItem(IDC_OK),mdNone,mdRepos);
	
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

void CFormExamInfrar::OnTimer(UINT nIDEvent) 
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
void CFormExamInfrar::SetInfo()
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

void CFormExamInfrar::SetKind(int kind)
{
	m_kind = kind;
}

void CFormExamInfrar::OnUp() 
{
	m_examdeteil.nSetp = 3;
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;	
	fram->m_dlgMain.ShowDlg(IDD_FORM_LISTEN);		
}
