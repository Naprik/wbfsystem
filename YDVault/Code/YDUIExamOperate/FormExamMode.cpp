#include "stdafx.h"
#include "FormExamMode.h"
#include "../ydcom/ExamDetail.h"
#include "../ydcom/ExamApp.h"
#include "../ObjHelper\StaticObjHelper.h"
#include "../\Base\AutoClean.h"
#include "../ObjRef\YDObjectRef.h"
#include "../UIBase\ListBoxEx.h"




IMPLEMENT_DYNCREATE(CFormExamMode, CYdFormView)

CFormExamMode::CFormExamMode()
	: CYdFormView(CFormExamMode::IDD)
{
	//{{AFX_DATA_INIT(CFormExamMode)
	//}}AFX_DATA_INIT
}


void CFormExamMode::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamMode)
	DDX_Control(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_CHOOSE, m_choose);
	DDX_Control(pDX, IDC_DEVLIST, m_devlist);
	DDX_Control(pDX, IDC_NEXT, m_next);
	DDX_Control(pDX, IDC_RADIO_BLUE, m_blue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamMode, CYdFormView)
	//{{AFX_MSG_MAP(CFormExamMode)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_BN_CLICKED(IDC_CHOOSE, OnChoose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamMode message handlers

void CFormExamMode::OnInitialUpdate() 
{
	CYdFormView::OnInitialUpdate();
	
// 	AddSzControl(*GetDlgItem(IDC_STATIC1),mdRelative,mdRelative);
// 	AddSzControl(*GetDlgItem(IDC_STATIC2),mdRelative,mdRelative);
// 	AddSzControl(*GetDlgItem(IDC_STATIC3),mdRelative,mdRelative);
// 	AddSzControl(*GetDlgItem(IDC_RADIO_BLUE),mdRelative,mdRelative);
// 	AddSzControl(*GetDlgItem(IDC_RADIO_INFRA),mdRelative,mdRelative);
// 	AddSzControl(*GetDlgItem(IDC_RADIO_USB),mdRelative,mdRelative);
// 	AddSzControl(*GetDlgItem(IDC_NEXT),mdRelative,mdRelative);
// 	AddSzControl(*GetDlgItem(IDC_SEARCH),mdRelative,mdRelative);
// 	AddSzControl(*GetDlgItem(IDC_CHOOSE),mdRelative,mdRelative);
// 	AddSzControl(m_devlist,mdRelative,mdRelative);

	CRect btnRect;  
	m_next.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_next.SetBitmapId(IDB_NEXT,IDB_NEXT,IDB_NEXT_DOWN,IDB_NEXT); 

	m_search.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_search.SetBitmapId(IDB_SEARCH,IDB_SEARCH,IDB_SEARCH_DOWN,IDB_SEARCH); 

	m_choose.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_choose.SetBitmapId(IDB_CHOOSE,IDB_CHOOSE,IDB_CHOOSE_DOWN,IDB_CHOOSE); 

	DWORD dwStyle=GetWindowLong(m_devlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_devlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_devlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_devlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_devlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_devlist.SetExtendedStyle(dwStyle);

	m_devlist.SetHeadings(_T("教师机编号, 80; 使用者,150;使用端口,100;机状态, 150"));
	
	return ;  
}

void CFormExamMode::OnNext() 
{	
	HRESULT hr = E_FAIL;
// 	CGExamStruct* pGExamStruct = NULL;
// 	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
// 		return ;
// 	}
// 	if(!(((CButton *)GetDlgItem(IDC_RADIO_BLUE))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_INFRA))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_USB))->GetCheck()))
// 	{
// 		AfxMessageBox(_T("没有选择本场考试收卷模式，无法进入下一步！"));
// 		return;
// 	}
// 	int iCount = 0;
// 	hr = pGExamStruct->m_pExamApp->Get_teach_count(iCount);
// 	if(FAILED(hr))
// 	{
// 		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
// 		return ;
// 	}
// 	if(iCount == 0)
// 	{
// 		AfxMessageBox(_T("没有选择本场考试使用的教师机，请先选择教师机！"));
// 		return;		
// 	}
// 
// 	pGExamStruct->m_pExamDetail->nSetp = 2;
// 	CString msg1,msg2;
// 	
// 	if(((CButton *)GetDlgItem(IDC_RADIO_BLUE))->GetCheck())
// 	{
// 		pGExamStruct->m_pExamDetail->m_uRollingType = ROLLING_TYPE_BLUE;
// 		msg2 = "蓝牙收卷";
// 	}
// 	else if(((CButton *)GetDlgItem(IDC_RADIO_INFRA))->GetCheck())
// 	{
// 		pGExamStruct->m_pExamDetail->m_uRollingType = ROLLING_TYPE_INFRAR;
// 		msg2 = "红外收卷";
// 	}
// 	else
// 	{
// 		pGExamStruct->m_pExamDetail->m_uRollingType = ROLLING_TYPE_USB;
// 		msg2 = "USB收卷";
// 	}
// 
// 
/*	CString msg;
	msg.Format("本场考试采用：\n点名模式：%s \n收卷模式：%s",msg1,msg2);
	if ( MessageBox(msg,"确认",MB_OKCANCEL) != IDOK)
	{	
		pGExamStruct->m_pExamDetail->nRoll_mode = 0;
		pGExamStruct->m_pExamDetail->nRolling_pattern = 0;
		return;
	}*/

	//fram->m_dlgMain.m_examset.SetKind(m_kind);
	//fram->m_dlgMain.ShowDlg(IDD_FORM_EXAMSET);	

}

void CFormExamMode::SetKind(int kind)
{
	m_kind = kind;
}

void CFormExamMode::OnSearch() 
{
	HRESULT hr = E_FAIL;
}

void CFormExamMode::OnChoose() 
{
	
}

BOOL CFormExamMode::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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