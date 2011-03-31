#include "stdafx.h"
#include "FormExamInfrar.h"
#include "../ydcom/ExamDetail.h"
#include "../ydcom/ExamApp.h"

IMPLEMENT_DYNCREATE(CFormExamInfrar, CYdFormView)


CFormExamInfrar::CFormExamInfrar()
	: CYdFormView(CFormExamInfrar::IDD)
{
	//{{AFX_DATA_INIT(CFormExamInfrar)
	m_systime = _T("");
	//}}AFX_DATA_INIT
}


void CFormExamInfrar::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamInfrar)
	DDX_Control(pDX, IDC_UP, m_up);
	DDX_Control(pDX, IDC_OK, m_ok);
	DDX_Control(pDX, IDC_STULIST, m_stulist);
	DDX_Text(pDX, IDC_STATIC17, m_systime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamInfrar, CYdFormView)
	//{{AFX_MSG_MAP(CFormExamInfrar)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_OK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamInfrar message handlers

void CFormExamInfrar::OnInitialUpdate() 
{
	CYdFormView::OnInitialUpdate();

	CRect btnRect;  
	m_up.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_up.SetBitmapId(IDB_UP,IDB_UP,IDB_UP_DOWN,IDB_UP); 

	m_ok.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_ok.SetBitmapId(IDB_OK,IDB_OK,IDB_OK_DOWN,IDB_OK); 

// 	AddSzControl(*GetDlgItem(IDC_STATIC1),mdResize,mdNone);
// 	AddSzControl(m_stulist,mdResize,mdResize);	
// 	
// 	AddSzControl(*GetDlgItem(IDC_OK),mdNone,mdRepos);
	
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

void CFormExamInfrar::OnTimer(UINT nIDEvent) 
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
void CFormExamInfrar::SetInfo()
{
// 	HRESULT hr = E_FAIL;
// 	CGExamStruct* pGExamStruct = NULL;
// 	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
// 		return ;
// 	}
// 	GetDlgItem(IDC_STATIC3)->SetWindowText(pGExamStruct->m_pExamDetail->sPapername);
// 	GetDlgItem(IDC_STATIC5)->SetWindowText(pGExamStruct->m_pExamDetail->sSubject);
// 	GetDlgItem(IDC_STATIC7)->SetWindowText(pGExamStruct->m_pExamDetail->sAddress);
// // 	CString sClass = _T("");
// // 	for(int i = 0; i < pGExamStruct->m_pExamDetail->arryClass.GetSize(); i++)
// // 	{
// // 		sClass += pGExamStruct->m_pExamDetail->arryClass[i] + _T("\n");
// // 	}
// 	//GetDlgItem(IDC_STATIC9)->SetWindowText(sClass);
// 	int m_account = m_stulist.GetItemCount();
// 	CString sCount;
// 	sCount.Format(_T("%d 人"), m_account);
// 	GetDlgItem(IDC_STATIC11)->SetWindowText(sCount);
// 	GetDlgItem(IDC_STATIC13)->SetWindowText(pGExamStruct->m_pExamDetail->sStart);
// 	GetDlgItem(IDC_STATIC15)->SetWindowText(pGExamStruct->m_pExamDetail->sEnd);
// 
// 	CString temp = _T("");
// 	temp.Format(_T("点名模式：%d, 收卷模式：%d, 考试性质：%d"),
// 				pGExamStruct->m_pExamDetail->m_uRollCallType,
// 				pGExamStruct->m_pExamDetail->m_uRollingType,
// 				pGExamStruct->m_pExamDetail->m_uExamKind);
// 	AfxMessageBox(temp);
// // 	for(int i = 0; i < pGExamStruct->m_pExamDetail->arryClass.GetSize(); i++)
// // 	{
// // 		AfxMessageBox(pGExamStruct->m_pExamDetail->arryClass[i]);
// // 	}
// 	AfxMessageBox(pGExamStruct->m_pExamDetail->sPapername);
// 	AfxMessageBox(pGExamStruct->m_pExamDetail->sSubject);
// 	AfxMessageBox(pGExamStruct->m_pExamDetail->sStart);
// 	AfxMessageBox(pGExamStruct->m_pExamDetail->sEnd);
// 	temp = _T("");
// 	temp.Format(_T("开始题号：%d， 结束题号：%d， 最大题号：%d"),
// 				pGExamStruct->m_pExamDetail->nStartNO,
// 				pGExamStruct->m_pExamDetail->nEndNO,
// 				pGExamStruct->m_pExamDetail->nMaxNO);
// 	AfxMessageBox(temp);
// 	if(pGExamStruct->m_pExamDetail->b_listen)
// 	{
// 		AfxMessageBox(pGExamStruct->m_pExamDetail->m_chanal);
// 		if(pGExamStruct->m_pExamDetail->b_part1)
// 		{
// 			AfxMessageBox(pGExamStruct->m_pExamDetail->m_start1.Format());
// 			AfxMessageBox(pGExamStruct->m_pExamDetail->m_end1.Format());
// 		}
// 		if(pGExamStruct->m_pExamDetail->b_part2)
// 		{
// 			AfxMessageBox(pGExamStruct->m_pExamDetail->m_start2.Format());
// 			AfxMessageBox(pGExamStruct->m_pExamDetail->m_end2.Format());
// 		}
// 		if(pGExamStruct->m_pExamDetail->b_part3)
// 		{
// 			AfxMessageBox(pGExamStruct->m_pExamDetail->m_start3.Format());
// 			AfxMessageBox(pGExamStruct->m_pExamDetail->m_end3.Format());
// 		}
// 	}
}

void CFormExamInfrar::SetKind(int kind)
{
	m_kind = kind;
}

void CFormExamInfrar::OnUp() 
{
// 	HRESULT hr = E_FAIL;
// 	CGExamStruct* pGExamStruct = NULL;
// 	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
// 		return ;
// 	}
// 	pGExamStruct->m_pExamDetail->nSetp = 4;
// 	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;	
// 	fram->m_dlgMain.ShowDlg(IDD_FORM_LISTEN);		
}

void CFormExamInfrar::OnOk() 
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
}

BOOL CFormExamInfrar::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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
