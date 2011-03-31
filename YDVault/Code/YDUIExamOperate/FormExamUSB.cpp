#include "stdafx.h"
#include "FormExamUSB.h"
#include "../ydcom/ExamDetail.h"
#include "../ydcom/ExamApp.h"



IMPLEMENT_DYNCREATE(CFormExamUSB, CYdFormView)


CFormExamUSB::CFormExamUSB()
	: CYdFormView(CFormExamUSB::IDD)
{
	//{{AFX_DATA_INIT(CFormExamUSB)
	m_systime = _T("");
	//}}AFX_DATA_INIT
}


void CFormExamUSB::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamUSB)
	DDX_Control(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_ROLLCALL, m_roll);
	DDX_Control(pDX, IDC_STULIST, m_stulist);
	DDX_Text(pDX, IDC_STATIC17, m_systime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamUSB, CYdFormView)
	//{{AFX_MSG_MAP(CFormExamUSB)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_BN_CLICKED(IDC_ROLLCALL, OnRollcall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamUSB message handlers

void CFormExamUSB::OnInitialUpdate() 
{
	CYdFormView::OnInitialUpdate();

// 	AddSzControl(*GetDlgItem(IDC_STATIC1),mdResize,mdNone);
// 	AddSzControl(*GetDlgItem(IDC_STATIC18),mdResize,mdNone);
// 	AddSzControl(m_stulist,mdResize,mdResize);	
// 	
// 	AddSzControl(*GetDlgItem(IDC_SEARCH),mdRepos,mdRepos);
// 	AddSzControl(*GetDlgItem(IDC_ROLLCALL),mdRepos,mdRepos);

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

	m_stulist.SetHeadings(_T("学号,150;姓名,100;班级,100;准考证号,100;当前状态,150;开考时间,150;交卷时间,150;学生蓝牙地址,100"));
	SetTimer(1,1000,NULL);
	
	return ;  
}

void CFormExamUSB::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 1)
	{
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
		m_systime.Format(_T("%2d:%2d:%2d"),sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
		UpdateData(FALSE);
	}
	CYdFormView::OnTimer(nIDEvent);
}

void  CFormExamUSB::SetKind(int kind)
{
	m_kind = kind;
}

void CFormExamUSB::SetInfo()
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

// 	GetDlgItem(IDC_STATIC3)->SetWindowText(pGExamStruct->m_pExamDetail->sPapername);
// 	GetDlgItem(IDC_STATIC5)->SetWindowText(pGExamStruct->m_pExamDetail->sSubject);
// 	GetDlgItem(IDC_STATIC7)->SetWindowText(pGExamStruct->m_pExamDetail->sAddress);
// 	CString sClass = _T("");
// // 	for(int i = 0; i < pGExamStruct->m_pExamDetail->arryClass.GetSize(); i++)
// // 	{
// // 		sClass += pGExamStruct->m_pExamDetail->arryClass[i] + _T("\n");
// // 	}
// 	GetDlgItem(IDC_STATIC9)->SetWindowText(sClass);
// 	int m_account = m_stulist.GetItemCount();
// 	CString sCount;
// 	sCount.Format(_T("%d 人"), m_account);
// 	GetDlgItem(IDC_STATIC11)->SetWindowText(sCount);
// 	GetDlgItem(IDC_STATIC13)->SetWindowText(pGExamStruct->m_pExamDetail->sStart);
// 	GetDlgItem(IDC_STATIC15)->SetWindowText(pGExamStruct->m_pExamDetail->sEnd);
}

void CFormExamUSB::OnSearch() 
{

}

void CFormExamUSB::OnRollcall() 
{
	
}
BOOL CFormExamUSB::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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