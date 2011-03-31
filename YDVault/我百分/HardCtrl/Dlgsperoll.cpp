// Dlgsperoll.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "Dlgsperoll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgsperoll dialog


CDlgsperoll::CDlgsperoll(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgsperoll::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgsperoll)
	m_stuid = _T("");
	m_dev = _T("");
	//}}AFX_DATA_INIT
}


void CDlgsperoll::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgsperoll)
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_SPEROLL, m_speroll);
	DDX_Control(pDX, IDC_SPESTART, m_spestart);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Text(pDX, IDC_ID, m_stuid);
	DDX_Text(pDX, IDC_DEV, m_dev);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgsperoll, CDialog)
	//{{AFX_MSG_MAP(CDlgsperoll)
	ON_BN_CLICKED(IDC_SPEROLL, OnSperoll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgsperoll message handlers
BOOL CDlgsperoll::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwStyle=GetWindowLong(m_list.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_list.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_list.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_list.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	
	m_list.ClearHeadings();
	m_list.SetHeadings("学号,80;姓名,100;班级,150;准考证号,100;当前状态,100");

	return true;
}


void CDlgsperoll::OnSperoll() 
{
	// TODO: Add your control notification handler code here
	
}
