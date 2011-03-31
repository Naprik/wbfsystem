// FormExamTime.cpp : implementation file
//

#include "stdafx.h"
#include "FormExamTime.h"
#include "../ydcom/ExamDetail.h"
#include "DlgChangeExmaTime.h"


IMPLEMENT_DYNCREATE(CFormExamTime, CYdFormView)


CFormExamTime::CFormExamTime()
	: CYdFormView(CFormExamTime::IDD)
{
	//{{AFX_DATA_INIT(CFormExamTime)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormExamTime::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamTime)
	DDX_Control(pDX, IDC_UP, m_up);
	DDX_Control(pDX, IDC_NEXT, m_next);
	DDX_Control(pDX, IDC_CHANGE, m_change);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamTime, CYdFormView)
	//{{AFX_MSG_MAP(CFormExamTime)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamTime message handlers

void CFormExamTime::OnInitialUpdate() 
{
	CYdFormView::OnInitialUpdate();


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
	
	return ;  
}


void CFormExamTime::OnUp() 
{
	HRESULT hr = E_FAIL;
// 	CGExamStruct* pGExamStruct = NULL;
// 	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
// 		return ;
// 	}
// 	ASSERT(pGExamStruct);
// 	pGExamStruct->m_pExamDetail->nSetp = 2;
	//CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;	
	//fram->m_dlgMain.ShowDlg(IDD_FORM_EXAMSET);	
}

void CFormExamTime::OnNext() 
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	//pGExamStruct->m_pExamDetail->nSetp = 4;	
// 	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
// 	fram->m_dlgMain.m_listen.SetKind(m_kind);
// 	fram->m_dlgMain.ShowDlg(IDD_FORM_LISTEN);
}
void CFormExamTime::SetKind(int kind)
{
	m_kind = kind;
}

void CFormExamTime::SetInfo()
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
// 	GetDlgItem(IDC_STATIC3)->SetWindowText(pGExamStruct->m_pExamDetail->sStart);
// 	GetDlgItem(IDC_STATIC5)->SetWindowText(pGExamStruct->m_pExamDetail->sEnd);
}

void CFormExamTime::OnChange() 
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	CDlgChangeExmaTime	dlg;
// 	if(dlg.DoModal() == IDOK)
// 	{
// 		{
// 			pGExamStruct->m_pExamDetail->sStart = dlg.m_starttime.Format();
// 			pGExamStruct->m_pExamDetail->sEnd = dlg.m_enftime.Format();
// 		}
// 		GetDlgItem(IDC_STATIC3)->SetWindowText(pGExamStruct->m_pExamDetail->sStart);
// 		GetDlgItem(IDC_STATIC5)->SetWindowText(pGExamStruct->m_pExamDetail->sEnd);
// 	}
} 

BOOL CFormExamTime::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
											 DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
											 CCreateContext* pContext)
{
	CExtDllState state;
	return CFormView::Create(lpszClassName,
		lpszWindowName,
		dwRequestedStyle,
		rect,
		pParentWnd,
		nID,
		pContext);
}