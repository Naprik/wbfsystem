#include "stdafx.h"
#include "hardctrl.h"
#include "FormListen.h"
#include "MainFrm.h"
#include "DlgChangesystime.h"
#include "ExamDetail.h"

extern CHardCtrlApp theApp;
extern CExamDetail  m_examdeteil;

IMPLEMENT_DYNCREATE(CFormListen, CDialog)


CFormListen::CFormListen(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormListen::IDD, pParent)
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
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormListen)
	DDX_Control(pDX, IDC_NEXT, m_next);
	DDX_Control(pDX, IDC_UP, m_up);
	DDX_Control(pDX, IDC_CHANGESYSTIME, m_change);
	DDX_Control(pDX, IDC_START3, m_start3ctl);
	DDX_Control(pDX, IDC_START2, m_start2ctl);
	DDX_Control(pDX, IDC_START1, m_start1ctl);
	DDX_Control(pDX, IDC_END3, m_end3ctl);
	DDX_Control(pDX, IDC_END2, m_end2ctl);
	DDX_Control(pDX, IDC_END1, m_end1ctl);
	DDX_Control(pDX, IDC_CHECK3, m_check3);
	DDX_Control(pDX, IDC_CHECK2, m_check2);
	DDX_Control(pDX, IDC_CHECK1, m_check1);
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


BEGIN_MESSAGE_MAP(CFormListen, CDialog)
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

BOOL CFormListen::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();
	
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_STATIC2),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_STATIC3),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_STATIC10),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC4),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_STATIC5),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_STATIC6),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_STATIC7),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_STATIC8),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_STATIC9),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_STATIC11),mdRelative,mdRelative);		
	
	
	AddSzControl(*GetDlgItem(IDC_RADIOYES),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_RADIONO),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_FM),mdRelative,mdRelative);	
	
	AddSzControl(*GetDlgItem(IDC_CHECK1),mdRelative,mdRelative);	
	
	AddSzControl(*GetDlgItem(IDC_CHECK2),mdRelative,mdRelative);	
	
	AddSzControl(*GetDlgItem(IDC_CHECK3),mdRelative,mdRelative);	
	
	AddSzControl(*GetDlgItem(IDC_START1),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_END1),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_START2),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_END2),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_START3),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_END3),mdRelative,mdRelative);	
	
	AddSzControl(*GetDlgItem(IDC_NEXT),mdRelative,mdRelative);		
	AddSzControl(*GetDlgItem(IDC_UP),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_CHANGESYSTIME),mdRelative,mdRelative);	
	
	SetTimer(1,1000,NULL);
	
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


	
	return TRUE; 
}

void CFormListen::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 1)
	{
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
	//	UpdateData();
		m_systime.Format("%2d:%2d:%2d",sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
		GetDlgItem(IDC_STATIC3)->SetWindowText(m_systime);
	//	UpdateData(FALSE);
	}
	cdxCSizingDialog::OnTimer(nIDEvent);
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
	m_examdeteil.nSetp = 3;
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;	
	fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_TIME);
}

void CFormListen::OnNext() 
{
	UpdateData(TRUE);


	if(((CButton *)GetDlgItem(IDC_RADIOYES))->GetCheck())
	{
		if(m_fm == "")
		{
			AfxMessageBox("没有设置频道，请输入听力频段！");
			return;
		}

		if(decodeFM(m_fm))
		{
			m_examdeteil.m_chanal =  m_fm;
		}
		else
		{
			AfxMessageBox("您输入的频段有误，请正确输入！");
			return;
		}


		m_examdeteil.b_listen = true;
		if(m_check1.GetCheck())
		{
			
			UpdateData(TRUE);
			COleDateTimeSpan timeSpan; 
			timeSpan = m_end1 - m_start1;
			int span = (int)timeSpan.GetTotalSeconds();
			if(span <= 0)
			{
				AfxMessageBox("听力1时间设置错误，结束时间必须大于开始时间！");
				return;
			}	
			m_examdeteil.b_part1 = true;
			m_examdeteil.m_start1 = m_start1;
			m_examdeteil.m_end1 = m_end1;
		}
		if(m_check2.GetCheck())
		{
			UpdateData(TRUE);
			COleDateTimeSpan timeSpan; 
			timeSpan = m_end2 - m_start2;
			int span = (int)timeSpan.GetTotalSeconds();
			if(span <= 0)
			{
				AfxMessageBox("听力2时间设置错误，结束时间必须大于开始时间！");
				return;
			}	
			m_examdeteil.b_part2 = true;
			m_examdeteil.m_start2 = m_start2;
			m_examdeteil.m_end2 = m_end2;
		}
		if(m_check3.GetCheck())
		{
			UpdateData(TRUE);
			COleDateTimeSpan timeSpan;
			timeSpan = m_end3 - m_start3;
			int span = (int)timeSpan.GetTotalSeconds();
			if(span <= 0)
			{
				AfxMessageBox("听力3时间设置错误，结束时间必须大于开始时间！");
				return;
			}
			m_examdeteil.b_part3 = true;
			m_examdeteil.m_start3 = m_start3;
			m_examdeteil.m_end3 = m_end3;
		}
		COleDateTime  m_start, m_end;
		m_start.ParseDateTime(m_examdeteil.sStart);
		m_end.ParseDateTime(m_examdeteil.sEnd);
		if(m_examdeteil.b_part1)
		{
			COleDateTimeSpan timeSpan_start, timeSpan_end;
			timeSpan_start = m_start - m_start1;
			int span1 = (int)timeSpan_start.GetTotalSeconds();
			if(span1 > 0)
			{
				AfxMessageBox("听力1开始时间设置错误,设置的听力1开始时间小于本场考试时间！");
				return;
			}
			timeSpan_end = 	m_end - m_end1;
			int span2 = (int)timeSpan_end.GetTotalSeconds();
			if(span2 < 0)
			{
				AfxMessageBox("听力1结束时间设置错误,设置的听力1结束时间大于本场考试时间！");
				return;				
			}
		}

		if(m_examdeteil.b_part2)
		{
			COleDateTimeSpan timeSpan_start, timeSpan_end;
			timeSpan_start = m_start - m_start2;
			int span1 = (int)timeSpan_start.GetTotalSeconds();
			if(span1 > 0)
			{
				AfxMessageBox("听力2开始时间设置错误,设置的听力2开始时间小于本场考试时间！");
				return;
			}
			timeSpan_end = 	m_end - m_end2;
			int span2 = (int)timeSpan_end.GetTotalSeconds();
			if(span2 < 0)
			{
				AfxMessageBox("听力2结束时间设置错误,设置的听力2结束时间大于本场考试时间！");
				return;				
			}
		}
		if(m_examdeteil.b_part3)
		{
			COleDateTimeSpan timeSpan_start, timeSpan_end;
			timeSpan_start = m_start - m_start3;
			int span1 = (int)timeSpan_start.GetTotalSeconds();
			if(span1 > 0)
			{
				AfxMessageBox("听力3开始时间设置错误,设置的听力3开始时间小于本场考试时间！");
				return;
			}
			timeSpan_end = 	m_end - m_end3;
			int span2 = (int)timeSpan_end.GetTotalSeconds();
			if(span2 < 0)
			{
				AfxMessageBox("听力3结束时间设置错误,设置的听力3结束时间大于本场考试时间！");
				return;				
			}
		}

	}
	else
	{
		m_examdeteil.b_listen = false;
	}	
	
	
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	fram->m_dlgMain.m_creatfile.SetInfo();
	fram->m_dlgMain.ShowDlg(IDD_FORM_CREAT_FILE);
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