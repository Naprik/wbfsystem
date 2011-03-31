// FormMenu.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormMenu.h"
#include "MainFrm.h"

IMPLEMENT_DYNCREATE(CFormMenu, CDialog)


CFormMenu::CFormMenu(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormMenu::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormMenu)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormMenu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormMenu)
	DDX_Control(pDX, IDC_QUERY, m_query);
	DDX_Control(pDX, IDC_EXIT, m_exit);
	DDX_Control(pDX, IDC_EXAM, m_exam);
	DDX_Control(pDX, IDC_DATE, m_date);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormMenu, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormMenu)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXAM, OnExam)
	ON_BN_CLICKED(IDC_DATE, OnDate)
	ON_BN_CLICKED(IDC_QUERY, OnQuery)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormMenu message handlers


void CFormMenu::OnPaint() 
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	CDC dcMem; 
	dcMem.CreateCompatibleDC(&dc); 
	BITMAP bitMap;
	m_bmpBackground.GetBitmap(&bitMap);
	CBitmap *pbmpOld=dcMem.SelectObject(&m_bmpBackground);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitMap.bmWidth,bitMap.bmHeight,SRCCOPY);
}

void CFormMenu::OnSize(UINT nType, int cx, int cy) 
{
	cdxCSizingDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

BOOL CFormMenu::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();
	
	m_bmpBackground.LoadBitmap(IDB_MENU2);	

	AddSzControl(*GetDlgItem(IDC_EXAM),mdRepos,mdRepos);
	AddSzControl(*GetDlgItem(IDC_DATE),mdRepos,mdRepos);
	AddSzControl(*GetDlgItem(IDC_QUERY),mdRepos,mdRepos);
	AddSzControl(*GetDlgItem(IDC_EXIT),mdRepos,mdRepos);

	CRect btnRect;
	m_date.GetWindowRect(btnRect); 
	ScreenToClient(btnRect);
	m_date.SetBitmapId(IDB_DATE,IDB_DATE,IDB_DATE_DOWN,IDB_DATE); 

	m_exam.GetWindowRect(btnRect);
	ScreenToClient(btnRect);
	m_exam.SetBitmapId(IDB_EXAM,IDB_EXAM,IDB_EXAM_DOWN,IDB_EXAM); 

	m_query.GetWindowRect(btnRect);
	ScreenToClient(btnRect);
	m_query.SetBitmapId(IDB_QUERY,IDB_QUERY,IDB_QUERY_DOWN,IDB_QUERY); 

	m_exit.GetWindowRect(btnRect);
	ScreenToClient(btnRect);
	m_exit.SetBitmapId(IDB_EXIT_BN,IDB_EXIT_BN,IDB_EXIT_DOWN,IDB_EXIT_BN); 

	return TRUE;  
}

void CFormMenu::OnExam() 
{
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;	
	fram->m_TabCtrl.SetActivePage(0);			
}

void CFormMenu::OnDate() 
{
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;	
	fram->m_TabCtrl.SetActivePage(1);
}

void CFormMenu::OnQuery() 
{
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;	
	fram->m_TabCtrl.SetActivePage(2);
}

void CFormMenu::OnExit() 
{
	//CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CString msg;
	msg.Format("确定退出系统？");
	if ( MessageBox(msg,"确认",MB_OKCANCEL) == IDOK)
		PostQuitMessage(0);;
}
