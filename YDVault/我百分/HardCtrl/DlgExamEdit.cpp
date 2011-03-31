// DlgExamEdit.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgExamEdit.h"
#include "ExamDetail.h"
#include "DlgChangeExmaTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExamEdit dialog
extern CExamDetail  m_examdeteil;

CDlgExamEdit::CDlgExamEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExamEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExamEdit)
	m_channel = _T("");
	m_endno = 0;
	m_papername = _T("");
	m_startno = 0;
	//}}AFX_DATA_INIT
}


void CDlgExamEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExamEdit)
	DDX_Control(pDX, IDC_FM3, m_fm3);
	DDX_Control(pDX, IDC_FM2, m_fm2);
	DDX_Control(pDX, IDC_FM1, m_fm1);
	DDX_Text(pDX, IDC_CHANNLE, m_channel);
	DDX_Text(pDX, IDC_ENDNO, m_endno);
	DDX_Text(pDX, IDC_PAPERNAME, m_papername);
	DDX_Text(pDX, IDC_START_NO, m_startno);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExamEdit, CDialog)
	//{{AFX_MSG_MAP(CDlgExamEdit)
	ON_BN_CLICKED(IDC_CHANGE_TIME, OnChangeTime)
	ON_BN_CLICKED(IDC_RADIO_YES, OnRadioYes)
	ON_BN_CLICKED(IDC_RADIO_NO, OnRadioNo)
	ON_BN_CLICKED(IDC_FM1, OnFm1)
	ON_BN_CLICKED(IDC_FM2, OnFm2)
	ON_BN_CLICKED(IDC_FM3, OnFm3)
	ON_BN_CLICKED(IDC_CHANGE_FM1, OnChangeFm1)
	ON_BN_CLICKED(IDC_CHANGE_FM2, OnChangeFm2)
	ON_BN_CLICKED(IDC_CHANGE_FM3, OnChangeFm3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExamEdit message handlers

BOOL CDlgExamEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	init();
	
	UpdateData(FALSE);
	return TRUE;  
}

void CDlgExamEdit::init()
{
	if (m_examdeteil.nRoll_pattern == 1)
	{
		((CButton *)GetDlgItem(IDC_RADIO_ROLL_BLUE))->SetCheck(TRUE);
	}
	else if (m_examdeteil.nRoll_pattern == 2)
	{
		((CButton *)GetDlgItem(IDC_RADIO_ROLL_INFRAR))->SetCheck(TRUE);
	}
	else if (m_examdeteil.nRoll_pattern == 2)
	{
		((CButton *)GetDlgItem(IDC_RADIO_ROLL_USB))->SetCheck(TRUE);
	}
	
	if (m_examdeteil.nRolling_pattern == 1)
	{
		((CButton *)GetDlgItem(IDC_RADIO_ROLLING_BLUE))->SetCheck(TRUE);
	}
	else if (m_examdeteil.nRolling_pattern == 2)
	{
		((CButton *)GetDlgItem(IDC_RADIO_ROLLING_INFRAR))->SetCheck(TRUE);
	}
	else if (m_examdeteil.nRolling_pattern == 2)
	{
		((CButton *)GetDlgItem(IDC_RADIO_ROLLING_USB))->SetCheck(TRUE);
	}
	
	if (m_examdeteil.nExam_quality == 1)
	{
		((CButton *)GetDlgItem(IDC_RADIO_STANDARD))->SetCheck(TRUE);
	}
	else if (m_examdeteil.nExam_quality == 2)
	{
		((CButton *)GetDlgItem(IDC_RADIO_NORMAL))->SetCheck(TRUE);
	}
	
	if (m_examdeteil.sStart != "")
	{
		GetDlgItem(IDC_STATIC1)->SetWindowText(m_examdeteil.sStart);
	}
	if (m_examdeteil.sEnd != "")
	{
		GetDlgItem(IDC_STATIC2)->SetWindowText(m_examdeteil.sEnd);
	}
	
	if (m_examdeteil.sPapername != "")
	{
		m_papername = m_examdeteil.sPapername;
		m_startno = m_examdeteil.nStartNO;
		m_endno = m_examdeteil.nEndNO;
	}
	
	if (m_examdeteil.b_listen)
	{
		((CButton *)GetDlgItem(IDC_RADIO_YES))->SetCheck(TRUE);
		if (m_examdeteil.b_part1)
		{
			((CButton *)GetDlgItem(IDC_FM1))->SetCheck(TRUE);
			GetDlgItem(IDC_STATIC3)->SetWindowText(m_examdeteil.m_start1.Format());
			GetDlgItem(IDC_STATIC4)->SetWindowText(m_examdeteil.m_end1.Format());
		}
		if (m_examdeteil.b_part2)
		{
			((CButton *)GetDlgItem(IDC_FM2))->SetCheck(TRUE);
			GetDlgItem(IDC_STATIC5)->SetWindowText(m_examdeteil.m_start2.Format());
			GetDlgItem(IDC_STATIC6)->SetWindowText(m_examdeteil.m_end2.Format());
		}
		if (m_examdeteil.b_part3)
		{
			((CButton *)GetDlgItem(IDC_FM3))->SetCheck(TRUE);
			GetDlgItem(IDC_STATIC7)->SetWindowText(m_examdeteil.m_start3.Format());
			GetDlgItem(IDC_STATIC8)->SetWindowText(m_examdeteil.m_end3.Format());
		}
		
		m_channel = m_examdeteil.m_chanal;
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RADIO_NO))->SetCheck(TRUE);
		((CButton *)GetDlgItem(IDC_FM1))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_FM2))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_FM3))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_CHANNLE))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_CHANGE_FM1))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_CHANGE_FM2))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_CHANGE_FM3))->EnableWindow(FALSE);
	}
	
}

void CDlgExamEdit::OnChangeTime() 
{
	CDlgChangeExmaTime dlg;
	if (dlg.DoModal() == IDOK)
	{
		GetDlgItem(IDC_STATIC1)->SetWindowText(dlg.m_starttime.Format());
		GetDlgItem(IDC_STATIC2)->SetWindowText(dlg.m_endtime.Format());
	}	
	
}

void CDlgExamEdit::OnRadioYes() 
{
	((CButton *)GetDlgItem(IDC_FM1))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_FM2))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_FM3))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_CHANNLE))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_CHANGE_FM1))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_CHANGE_FM2))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_CHANGE_FM3))->EnableWindow(FALSE);	
}

void CDlgExamEdit::OnRadioNo() 
{
	((CButton *)GetDlgItem(IDC_FM1))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_FM2))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_FM3))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_CHANNLE))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_CHANGE_FM1))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_CHANGE_FM2))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_CHANGE_FM3))->EnableWindow(FALSE);	
}

void CDlgExamEdit::OnFm1() 
{
	if(m_fm1.GetCheck())
	{
		
		((CButton *)GetDlgItem(IDC_CHANGE_FM1))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHANGE_FM1))->EnableWindow(FALSE);
	}		
	
}

void CDlgExamEdit::OnFm2() 
{
	if(m_fm2.GetCheck())
	{
		
		((CButton *)GetDlgItem(IDC_CHANGE_FM2))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHANGE_FM2))->EnableWindow(FALSE);
	}
	
}

void CDlgExamEdit::OnFm3() 
{
	if(m_fm3.GetCheck())
	{
		
		((CButton *)GetDlgItem(IDC_CHANGE_FM3))->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHANGE_FM3))->EnableWindow(FALSE);
	}	
}

void CDlgExamEdit::OnChangeFm1() 
{
	CDlgChangeExmaTime dlg;
	if (dlg.DoModal() == IDOK)
	{
		GetDlgItem(IDC_STATIC3)->SetWindowText(dlg.m_starttime.Format());
		GetDlgItem(IDC_STATIC4)->SetWindowText(dlg.m_endtime.Format());
	}			
	
}

void CDlgExamEdit::OnChangeFm2() 
{
	CDlgChangeExmaTime dlg;
	if (dlg.DoModal() == IDOK)
	{
		GetDlgItem(IDC_STATIC5)->SetWindowText(dlg.m_starttime.Format());
		GetDlgItem(IDC_STATIC6)->SetWindowText(dlg.m_endtime.Format());
	}		
}

void CDlgExamEdit::OnChangeFm3() 
{
	CDlgChangeExmaTime dlg;
	if (dlg.DoModal() == IDOK)
	{
		GetDlgItem(IDC_STATIC7)->SetWindowText(dlg.m_starttime.Format());
		GetDlgItem(IDC_STATIC8)->SetWindowText(dlg.m_endtime.Format());
	}
	
}

BOOL CDlgExamEdit::decodeFM(CString &strFM)
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

void CDlgExamEdit::OnOK() 
{
	COleDateTime	m_start1;
	COleDateTime	m_start2;
	COleDateTime	m_start3;
	COleDateTime	m_end1;
	COleDateTime	m_end2;
	COleDateTime	m_end3;

	COleDateTime	m_start;
	COleDateTime	m_end;

	UpdateData(TRUE);
	CString sStart = "";
	GetDlgItem(IDC_STATIC1)->GetWindowText(sStart);
	m_start.ParseDateTime(sStart);

	CString sEnd = "";
	GetDlgItem(IDC_STATIC2)->GetWindowText(sEnd);
	m_end.ParseDateTime(sEnd);

	if(!(((CButton *)GetDlgItem(IDC_RADIO_ROLL_BLUE))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_ROLL_INFRAR))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_ROLL_USB))->GetCheck()))
	{
		AfxMessageBox("没有选择本场考试开考模式，无法进入下一步！");
		return;
	}

	if(!(((CButton *)GetDlgItem(IDC_RADIO_ROLLING_BLUE))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_ROLLING_INFRAR))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_ROLLING_INFRAR))->GetCheck()))
	{
		AfxMessageBox("没有选择本场考试收卷模式，无法进入下一步！");
		return;
	}
	
	if(!(((CButton *)GetDlgItem(IDC_RADIO_STANDARD))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_NORMAL))->GetCheck()) )
	{
		AfxMessageBox("没有选择本场考试性质，无法进入下一步！");
		return;
	}

	COleDateTimeSpan timeSpan; 
	timeSpan = m_end - m_start;
	int span = (int)timeSpan.GetTotalSeconds();
	if(span <= 0)
	{
		AfxMessageBox("考试时间设置错误，结束时间必须大于开始时间！");
		return;
	}
	
	if (m_papername == "")
	{
		AfxMessageBox("没有输入试卷名称，请重新输入！");
		return;
	}

	if (m_startno <= 0 || m_endno <= 0)
	{
		AfxMessageBox("试卷题号设置错误，请重新输入！");
		return;
	}
	if (m_startno >= m_endno)
	{
		AfxMessageBox("试卷结束题号必须大于开始题号，请重新输入！");
		return;
	}


	if (((CButton *)GetDlgItem(IDC_RADIO_YES))->GetCheck())
	{
		if (m_channel == "")
		{
			AfxMessageBox("没有设置频道，请输入听力频段！");
			return;
		}
		if(!decodeFM(m_channel))
		{
			AfxMessageBox("您输入的频段有误，请正确输入！");
			return;
		}
		if(m_fm1.GetCheck())
		{
			CString sStart1 = "";
		 	GetDlgItem(IDC_STATIC3)->GetWindowText(sStart1);
			m_start1.ParseDateTime(sStart1);
		 	
		 	CString sEnd1 = "";
		 	GetDlgItem(IDC_STATIC4)->GetWindowText(sEnd1);
		 	m_end1.ParseDateTime(sEnd1);

			//COleDateTimeSpan timeSpan; 
			timeSpan = m_end1 - m_start1;
			span = (int)timeSpan.GetTotalSeconds();
			if(span <= 0)
			{
				AfxMessageBox("听力1时间设置错误，结束时间必须大于开始时间！");
				return;
			}
			
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
		if(m_fm2.GetCheck())
		{
			CString sStart2 = "";
			GetDlgItem(IDC_STATIC5)->GetWindowText(sStart2);
			m_start2.ParseDateTime(sStart2);
			
			CString sEnd2 = "";
			GetDlgItem(IDC_STATIC6)->GetWindowText(sEnd2);
			m_end2.ParseDateTime(sEnd2);
			
			//COleDateTimeSpan timeSpan; 
			timeSpan = m_end2 - m_start2;
			span = (int)timeSpan.GetTotalSeconds();
			if(span <= 0)
			{
				AfxMessageBox("听力2时间设置错误，结束时间必须大于开始时间！");
				return;
			}
			
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
		if(m_fm3.GetCheck())
		{
			CString sStart3 = "";
			GetDlgItem(IDC_STATIC7)->GetWindowText(sStart3);
			m_start3.ParseDateTime(sStart3);
			
			CString sEnd3 = "";
			GetDlgItem(IDC_STATIC8)->GetWindowText(sEnd3);
			m_end3.ParseDateTime(sEnd3);
			
			//COleDateTimeSpan timeSpan; 
			timeSpan = m_end3 - m_start3;
			span = (int)timeSpan.GetTotalSeconds();
			if(span <= 0)
			{
				AfxMessageBox("听力3时间设置错误，结束时间必须大于开始时间！");
				return;
			}
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

	if (((CButton *)GetDlgItem(IDC_RADIO_ROLL_BLUE))->GetCheck())
	{
		m_examdeteil.nRoll_pattern = 1;
	}
	else if (((CButton *)GetDlgItem(IDC_RADIO_ROLL_INFRAR))->GetCheck())
	{
		m_examdeteil.nRoll_pattern = 2;
	}
	else
		m_examdeteil.nRoll_pattern = 3;

	if (((CButton *)GetDlgItem(IDC_RADIO_ROLLING_BLUE))->GetCheck())
	{
		m_examdeteil.nRolling_pattern = 1;
	}
	else if (((CButton *)GetDlgItem(IDC_RADIO_ROLLING_INFRAR))->GetCheck())
	{
		m_examdeteil.nRolling_pattern = 2;
	}
	else
		m_examdeteil.nRolling_pattern = 3;

	if (((CButton *)GetDlgItem(IDC_RADIO_STANDARD))->GetCheck())
	{
		m_examdeteil.nExam_quality = 1;
	}
	else
		m_examdeteil.nExam_quality = 2;

	m_examdeteil.sStart = m_start.Format();
	m_examdeteil.sEnd = m_end.Format();
	m_examdeteil.sPapername = m_papername;
	m_examdeteil.nStartNO = m_startno;
	m_examdeteil.nEndNO = m_endno;
	m_examdeteil.nMaxNO = m_endno;

	if (((CButton *)GetDlgItem(IDC_RADIO_YES))->GetCheck())
	{
		m_examdeteil.b_listen = TRUE;
		if (m_fm1.GetCheck())
		{
			m_examdeteil.b_part1 = TRUE;
			m_examdeteil.m_start1 = m_start1;
			m_examdeteil.m_end1 = m_end1;
		}
		else
			m_examdeteil.b_part1 = FALSE;

		if (m_fm2.GetCheck())
		{
			m_examdeteil.b_part2 = TRUE;
			m_examdeteil.m_start2 = m_start2;
			m_examdeteil.m_end2 = m_end2;
		}
		else
			m_examdeteil.b_part2 = FALSE;

		if (m_fm3.GetCheck())
		{
			m_examdeteil.b_part3 = TRUE;
			m_examdeteil.m_start3 = m_start3;
			m_examdeteil.m_end3 = m_end3;
		}
		else
			m_examdeteil.b_part3 = FALSE;
		m_examdeteil.m_chanal = m_channel;
	}
	else
		m_examdeteil.b_listen = FALSE;

	CDialog::OnOK();
}
