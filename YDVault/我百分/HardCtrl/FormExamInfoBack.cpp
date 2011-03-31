// FormExamInfoBack.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormExamInfoBack.h"
#include "MainFrm.h"
#include "ExamDetail.h"

extern CExamDetail  m_examdeteil;

IMPLEMENT_DYNCREATE(CFormExamInfoBack, CDialog)


CFormExamInfoBack::CFormExamInfoBack(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormExamInfoBack::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormExamInfoBack)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormExamInfoBack::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamInfoBack)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamInfoBack, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormExamInfoBack)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamInfoBack message handlers

BOOL CFormExamInfoBack::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();
	
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_RADIO_BLUE),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_RADIO_INFRA),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_RADIO_USB),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC2),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_RADIO_BLUE2),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_RADIO_INFRA2),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_RADIO_USB2),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_NEXT),mdRelative,mdRelative);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFormExamInfoBack::SetKind(int kind)
{
	m_kind = kind;
}


void CFormExamInfoBack::OnNext() 
{
	if(!(((CButton *)GetDlgItem(IDC_RADIO_BLUE2))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_INFRA2))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_USB2))->GetCheck()))
	{
		AfxMessageBox("没有选择本场考试开考模式，无法进入下一步！");
		return;
	}
	
	if(!(((CButton *)GetDlgItem(IDC_RADIO_BLUE))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_INFRA))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_USB))->GetCheck()))
	{
		AfxMessageBox("没有选择本场考试收卷模式，无法进入下一步！");
		return;
	}
	
	
	if(((CButton *)GetDlgItem(IDC_RADIO_BLUE2))->GetCheck())
	{
		m_examdeteil.nRoll_pattern = 1;
		//msg2 = "蓝牙收卷";
	}
	else if(((CButton *)GetDlgItem(IDC_RADIO_INFRA2))->GetCheck())
	{
		m_examdeteil.nRoll_pattern = 2;
		//msg2 = "红外收卷";
	}
	else
	{
		m_examdeteil.nRoll_pattern = 3;
		//msg2 = "USB收卷";
	}	
	
	
	if(((CButton *)GetDlgItem(IDC_RADIO_BLUE))->GetCheck())
	{
		m_examdeteil.nRolling_pattern = 1;
		//msg2 = "蓝牙收卷";
	}
	else if(((CButton *)GetDlgItem(IDC_RADIO_INFRA))->GetCheck())
	{
		m_examdeteil.nRolling_pattern = 2;
		//msg2 = "红外收卷";
	}
	else
	{
		m_examdeteil.nRolling_pattern = 3;
		//msg2 = "USB收卷";
	}
	
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	
	fram->m_dlgMain.m_examset.SetInfo();
	fram->m_dlgMain.ShowDlg(IDD_FORM_EXAMSET);	
	
}
