// FormExamTime.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormExamTime.h"
#include "MainFrm.h"
#include "ExamDetail.h"
#include "DlgChangeExmaTime.h"

extern CHardCtrlApp theApp;
extern CExamDetail  m_examdeteil;

IMPLEMENT_DYNCREATE(CFormExamTime, CDialog)


CFormExamTime::CFormExamTime(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormExamTime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormExamTime)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormExamTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamTime)
	DDX_Control(pDX, IDC_UP, m_up);
	DDX_Control(pDX, IDC_NEXT, m_next);
	DDX_Control(pDX, IDC_CHANGE, m_change);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamTime, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormExamTime)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamTime message handlers

BOOL CFormExamTime::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();

	AddSzControl(*GetDlgItem(IDC_STATIC1),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC2),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC3),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC4),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC5),mdRelative,mdRelative);

	AddSzControl(*GetDlgItem(IDC_UP),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_NEXT),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_CHANGE),mdRelative,mdRelative);


	CRect btnRect;  
	m_next.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_next.SetBitmapId(IDB_NEXT,IDB_NEXT,IDB_NEXT_DOWN,IDB_NEXT); 
	
	m_up.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_up.SetBitmapId(IDB_UP,IDB_UP,IDB_UP_DOWN,IDB_UP); 

	m_change.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_change.SetBitmapId(IDB_CHANGE_EXAM_TIME,IDB_CHANGE_EXAM_TIME,IDB_CHANGE_EXAM_TIME_DOWN,IDB_CHANGE_EXAM_TIME); 
	
	return TRUE;  
}


void CFormExamTime::OnUp() 
{
	m_examdeteil.nSetp = 2;
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;	
	fram->m_dlgMain.ShowDlg(IDD_FORM_EXAMSET);	
}

void CFormExamTime::OnNext() 
{
	m_examdeteil.nSetp = 4;	
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	fram->m_dlgMain.m_listen.SetKind(m_kind);
	fram->m_dlgMain.ShowDlg(IDD_FORM_LISTEN);
}
void CFormExamTime::SetKind(int kind)
{
	m_kind = kind;
}

void CFormExamTime::SetInfo()
{
	GetDlgItem(IDC_STATIC3)->SetWindowText(m_examdeteil.sStart);
	GetDlgItem(IDC_STATIC5)->SetWindowText(m_examdeteil.sEnd);
}

void CFormExamTime::OnChange() 
{
	CDlgChangeExmaTime	dlg;
	if(dlg.DoModal() == IDOK)
	{
		{
			m_examdeteil.sStart = dlg.m_starttime.Format();
			m_examdeteil.sEnd = dlg.m_endtime.Format();
		}
		GetDlgItem(IDC_STATIC3)->SetWindowText(m_examdeteil.sStart);
		GetDlgItem(IDC_STATIC5)->SetWindowText(m_examdeteil.sEnd);
	}
} 
