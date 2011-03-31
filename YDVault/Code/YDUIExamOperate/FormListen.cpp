#include "stdafx.h"
#include "FormListen.h"
#include "DlgChangesystime.h"
#include "../ydcom/ExamDetail.h"



IMPLEMENT_DYNCREATE(CFormListen, CYdFormView)


CFormListen::CFormListen()
	: CYdFormView(CFormListen::IDD)
{
	//{{AFX_DATA_INIT(CFormListen)
	m_end1 = COleDateTime::GetCurrentTime();
	m_end2 = COleDateTime::GetCurrentTime();
	m_end3 = COleDateTime::GetCurrentTime();
	m_systime = _T("");
	m_start1 = COleDateTime::GetCurrentTime();
	m_start2 = COleDateTime::GetCurrentTime();
	m_start3 = COleDateTime::GetCurrentTime();
	m_fm = _T("");
	//}}AFX_DATA_INIT
}


void CFormListen::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormListen)
	DDX_Control(pDX, IDC_START3, m_start3ctl);
	DDX_Control(pDX, IDC_START2, m_start2ctl);
	DDX_Control(pDX, IDC_START1, m_start1ctl);
	DDX_Control(pDX, IDC_UP, m_up);
	DDX_Control(pDX, IDC_NEXT, m_next);
	DDX_Control(pDX, IDC_END3, m_end3ctl);
	DDX_Control(pDX, IDC_END2, m_end2ctl);
	DDX_Control(pDX, IDC_END1, m_end1ctl);
	DDX_Control(pDX, IDC_CHECK3, m_check3);
	DDX_Control(pDX, IDC_CHECK2, m_check2);
	DDX_Control(pDX, IDC_CHECK1, m_check1);
	DDX_Control(pDX, IDC_CHANGESYSTIME, m_change);
	DDX_DateTimeCtrl(pDX, IDC_END1, m_end1);
	DDX_DateTimeCtrl(pDX, IDC_END2, m_end2);
	DDX_DateTimeCtrl(pDX, IDC_END3, m_end3);
	DDX_Text(pDX, IDC_STATIC3, m_systime);
	DDX_DateTimeCtrl(pDX, IDC_START1, m_start1);
	DDX_DateTimeCtrl(pDX, IDC_START2, m_start2);
	DDX_DateTimeCtrl(pDX, IDC_START3, m_start3);
	DDX_Text(pDX, IDC_FM, m_fm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormListen, CYdFormView)
	//{{AFX_MSG_MAP(CFormListen)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIOYES, OnRadioyes)
	ON_BN_CLICKED(IDC_RADIONO, OnRadiono)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHANGESYSTIME, OnChangesystime)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormListen message handlers

void CFormListen::OnInitialUpdate() 
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
	m_change.SetBitmapId(IDB_CHANGESYSTIME,IDB_CHANGESYSTIME,IDB_CHANGESYSTIME_DOWN,IDB_CHANGESYSTIME); 

	SetTimer(1,1000,NULL);
//	SetTimer(2,1000,NULL);
//	SetTimer(3,1,NULL);	
/*	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	m_systime.Format("%2d:%2d",sysTime.wHour,sysTime.wMinute);*/


	((CButton *)GetDlgItem(IDC_RADIONO))->SetCheck(TRUE);

	m_check1.EnableWindow(false);
	m_check2.EnableWindow(false);
	m_check3.EnableWindow(false);
	
	m_end1ctl.EnableWindow(false);
	m_end2ctl.EnableWindow(false);
	m_end3ctl.EnableWindow(false);
	
	m_start1ctl.EnableWindow(false);
	m_start2ctl.EnableWindow(false);
	m_start3ctl.EnableWindow(false);
	
	((CButton *)GetDlgItem(IDC_FM))->EnableWindow(false);

	
	return ; 
}

void CFormListen::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 1)
	{
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
	//	UpdateData();
		m_systime.Format(_T("%2d:%2d:%2d"),sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
		GetDlgItem(IDC_STATIC3)->SetWindowText(m_systime);
	//	UpdateData(FALSE);
	}
	CYdFormView::OnTimer(nIDEvent);
}

void CFormListen::OnRadioyes() 
{
	((CButton *)GetDlgItem(IDC_FM))->EnableWindow(true);
	m_check1.EnableWindow(true);
	m_check2.EnableWindow(true);
	m_check3.EnableWindow(true);
	
	m_end1ctl.EnableWindow(false);
	m_end2ctl.EnableWindow(false);
	m_end3ctl.EnableWindow(false);
	
	m_start1ctl.EnableWindow(false);
	m_start2ctl.EnableWindow(false);
	m_start3ctl.EnableWindow(false);	
}

void CFormListen::OnRadiono() 
{
	((CButton *)GetDlgItem(IDC_FM))->EnableWindow(false);
	m_check1.EnableWindow(false);
	m_check2.EnableWindow(false);
	m_check3.EnableWindow(false);
	
	m_end1ctl.EnableWindow(false);
	m_end2ctl.EnableWindow(false);
	m_end3ctl.EnableWindow(false);
	
	m_start1ctl.EnableWindow(false);
	m_start2ctl.EnableWindow(false);
	m_start3ctl.EnableWindow(false);	
}

void CFormListen::OnCheck1() 
{
	if(m_check1.GetCheck())
	{
		m_end1ctl.EnableWindow(true);

		m_start1ctl.EnableWindow(true);
	}
	else
	{
		m_end1ctl.EnableWindow(false);
		
		m_start1ctl.EnableWindow(false);
	}		
}

void CFormListen::OnCheck2() 
{
	if(m_check2.GetCheck())
	{
		m_end2ctl.EnableWindow(true);

		m_start2ctl.EnableWindow(true);
	}
	else
	{
		m_end2ctl.EnableWindow(false);
		
		m_start2ctl.EnableWindow(false);
	}		
}

void CFormListen::OnCheck3() 
{
	if(m_check3.GetCheck())
	{
		m_end3ctl.EnableWindow(true);

		m_start3ctl.EnableWindow(true);
	}
	else
	{
		m_end3ctl.EnableWindow(false);
		
		m_start3ctl.EnableWindow(false);
	}		
}

void CFormListen::OnChangesystime() 
{
	CDlgChangesystime  dlg;
	if(dlg.DoModal() == IDOK)
	{

	}
}

void CFormListen::OnUp() 
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
// 	pGExamStruct->m_pExamDetail->nSetp = 3;
// 	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;	
// 	fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_TIME);
}

void CFormListen::OnNext() 
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
// 	pGExamStruct->m_pExamDetail->nSetp = 5;
// 	UpdateData(TRUE);
// 
// 
// 	if(((CButton *)GetDlgItem(IDC_RADIOYES))->GetCheck())
// 	{
// 		if(m_fm == "")
// 		{
// 			AfxMessageBox(_T("没有设置频道，请输入听力频段！"));
// 			return;
// 		}
// 
// 		if(decodeFM(m_fm))
// 		{
// 			pGExamStruct->m_pExamDetail->m_chanal =  m_fm;
// 		}
// 		else
// 		{
// 			AfxMessageBox(_T("您输入的频段有误，请正确输入！"));
// 			return;
// 		}
// 
// 
// 		pGExamStruct->m_pExamDetail->b_listen = true;
// 		if(m_check1.GetCheck())
// 		{
// 			
// 			UpdateData(TRUE);
// 			COleDateTimeSpan timeSpan; 
// 			timeSpan = m_end1 - m_start1;
// 			int span = (int)timeSpan.GetTotalSeconds();
// 			if(span <= 0)
// 			{
// 				AfxMessageBox(_T("听力1时间设置错误，结束时间必须大于开始时间！"));
// 				return;
// 			}	
// 			pGExamStruct->m_pExamDetail->b_part1 = true;
// 			pGExamStruct->m_pExamDetail->m_start1 = m_start1;
// 			pGExamStruct->m_pExamDetail->m_end1 = m_end1;
// 		}
// 		if(m_check2.GetCheck())
// 		{
// 			UpdateData(TRUE);
// 			COleDateTimeSpan timeSpan; 
// 			timeSpan = m_end2 - m_start2;
// 			int span = (int)timeSpan.GetTotalSeconds();
// 			if(span <= 0)
// 			{
// 				AfxMessageBox(_T("听力2时间设置错误，结束时间必须大于开始时间！"));
// 				return;
// 			}	
// 			pGExamStruct->m_pExamDetail->b_part2 = true;
// 			pGExamStruct->m_pExamDetail->m_start2 = m_start2;
// 			pGExamStruct->m_pExamDetail->m_end2 = m_end2;
// 		}
// 		if(m_check3.GetCheck())
// 		{
// 			UpdateData(TRUE);
// 			COleDateTimeSpan timeSpan;
// 			timeSpan = m_end3 - m_start3;
// 			int span = (int)timeSpan.GetTotalSeconds();
// 			if(span <= 0)
// 			{
// 				AfxMessageBox(_T("听力3时间设置错误，结束时间必须大于开始时间！"));
// 				return;
// 			}
// 			pGExamStruct->m_pExamDetail->b_part3 = true;
// 			pGExamStruct->m_pExamDetail->m_start3 = m_start3;
// 			pGExamStruct->m_pExamDetail->m_end3 = m_end3;
// 		}
// 		COleDateTime  m_start, m_end;
// 		m_start.ParseDateTime(pGExamStruct->m_pExamDetail->sStart);
// 		m_end.ParseDateTime(pGExamStruct->m_pExamDetail->sEnd);
// 		if(pGExamStruct->m_pExamDetail->b_part1)
// 		{
// 			COleDateTimeSpan timeSpan_start, timeSpan_end;
// 			timeSpan_start = m_start - m_start1;
// 			int span1 = (int)timeSpan_start.GetTotalSeconds();
// 			if(span1 > 0)
// 			{
// 				AfxMessageBox(_T("听力1开始时间设置错误,设置的听力1开始时间小于本场考试时间！"));
// 				return;
// 			}
// 			timeSpan_end = 	m_end - m_end1;
// 			int span2 = (int)timeSpan_end.GetTotalSeconds();
// 			if(span2 < 0)
// 			{
// 				AfxMessageBox(_T("听力1结束时间设置错误,设置的听力1结束时间大于本场考试时间！"));
// 				return;				
// 			}
// 		}
// 
// 		if(pGExamStruct->m_pExamDetail->b_part2)
// 		{
// 			COleDateTimeSpan timeSpan_start, timeSpan_end;
// 			timeSpan_start = m_start - m_start2;
// 			int span1 = (int)timeSpan_start.GetTotalSeconds();
// 			if(span1 > 0)
// 			{
// 				AfxMessageBox(_T("听力2开始时间设置错误,设置的听力2开始时间小于本场考试时间！"));
// 				return;
// 			}
// 			timeSpan_end = 	m_end - m_end2;
// 			int span2 = (int)timeSpan_end.GetTotalSeconds();
// 			if(span2 < 0)
// 			{
// 				AfxMessageBox(_T("听力2结束时间设置错误,设置的听力2结束时间大于本场考试时间！"));
// 				return;				
// 			}
// 		}
// 		if(pGExamStruct->m_pExamDetail->b_part3)
// 		{
// 			COleDateTimeSpan timeSpan_start, timeSpan_end;
// 			timeSpan_start = m_start - m_start3;
// 			int span1 = (int)timeSpan_start.GetTotalSeconds();
// 			if(span1 > 0)
// 			{
// 				AfxMessageBox(_T("听力3开始时间设置错误,设置的听力3开始时间小于本场考试时间！"));
// 				return;
// 			}
// 			timeSpan_end = 	m_end - m_end3;
// 			int span2 = (int)timeSpan_end.GetTotalSeconds();
// 			if(span2 < 0)
// 			{
// 				AfxMessageBox(_T("听力3结束时间设置错误,设置的听力3结束时间大于本场考试时间！"));
// 				return;				
// 			}
// 		}
// 
// 	}
// 	else
// 	{
// 		pGExamStruct->m_pExamDetail->b_listen = false;
// 	}
// 
// 	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
// 	if(m_kind == 3)
// 	{
// 		fram->m_dlgMain.m_examblue.SetKind(m_kind);
// 		fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_BLUE);
// 		fram->m_dlgMain.m_examblue.SetInfo();
// 	}
// 	if(m_kind == 4)
// 	{
// 		fram->m_dlgMain.m_examinfrar.SetKind(m_kind);
// 		fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_INFRAR);
// 		fram->m_dlgMain.m_examinfrar.SetInfo();
// 	}
// 	if(m_kind == 5)
// 	{
// 		fram->m_dlgMain.m_examusb.SetKind(m_kind);
// 		fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_USB);
// 		fram->m_dlgMain.m_examusb.SetInfo();
// 	}
// 	if(m_kind == 17)
// 	{
// 		fram->m_dlgMain.m_creatfile.SetKind(m_kind);
// 		fram->m_dlgMain.ShowDlg(IDD_CREAT_FILE);
// 		fram->m_dlgMain.m_creatfile.SetInfo();	
// 	}
}
void CFormListen::SetKind(int kind)
{
	m_kind = kind;
}

BOOL CFormListen::decodeFM(CString &strFM)
{
	strFM.Remove('.');

	BOOL bIsDigit = TRUE;
	int nCount = strFM.GetLength(); // 获得字符个数
	if(nCount > 4)
		return FALSE;
	for ( int i = 0; i < nCount; i ++ )
	{
		if ( 0 == isdigit( strFM.GetAt(i) ) ) // 不是数字就置标志位
		{
				 bIsDigit = FALSE;
				 break;   // 退出
		}
	 }
	
	return bIsDigit;
}
BOOL CFormListen::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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