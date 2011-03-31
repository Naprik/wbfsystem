// FormExamRollingAnswer.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormExamRollingAnswer.h"
#include "MainFrm.h"
#include "ExamDetail.h"
#include "ExamApp.h"
#include "Studev.h"

extern CHardCtrlApp theApp;
extern CExamDetail  m_examdeteil;
extern CExamApp m_app;
extern CArray<CStudev, CStudev>  m_arryStud;
extern CExamApp m_app;
IMPLEMENT_DYNCREATE(CFormExamRollingAnswer, CDialog)


CFormExamRollingAnswer::CFormExamRollingAnswer(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormExamRollingAnswer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormExamRollingAnswer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormExamRollingAnswer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamRollingAnswer)
	DDX_Control(pDX, IDC_UP, m_up);
	DDX_Control(pDX, IDC_SAVE, m_save);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_FETCH1, m_fetch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamRollingAnswer, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormExamRollingAnswer)
	ON_BN_CLICKED(IDC_FETCH1, OnFetch1)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_UP, OnUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamRollingAnswer message handlers

BOOL CFormExamRollingAnswer::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();
	
	AddSzControl(m_list,mdRelative,mdRelative);	
	
	AddSzControl(*GetDlgItem(IDC_FETCH1),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_SAVE),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_UP),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC2),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC3),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC4),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC5),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC6),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC7),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC8),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC9),mdRelative,mdRelative);
	// 	AddSzControl(*GetDlgItem(IDC_STATIC10),mdRelative,mdRelative);
	// 	AddSzControl(*GetDlgItem(IDC_STATIC11),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC12),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC13),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC14),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC15),mdRelative,mdRelative);
	// 	AddSzControl(*GetDlgItem(IDC_STATIC16),mdRelative,mdRelative);
	// 	AddSzControl(*GetDlgItem(IDC_STATIC17),mdRelative,mdRelative);
	
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
	
	m_list.SetHeadings("学号,150;姓名,100;班级,100;准考证号,100;开考时间,150;交卷时间,150;学生答案,200");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CFormExamRollingAnswer::SetKind(int kind)
{
	m_kind = kind;
}

void CFormExamRollingAnswer::SetInfo()
{
	// 	GetDlgItem(IDC_STATIC2)->SetWindowText(m_examdeteil.sSubject);
	// 	GetDlgItem(IDC_STATIC4)->SetWindowText(m_examdeteil.sPapername);
	// 	GetDlgItem(IDC_STATIC6)->SetWindowText(m_examdeteil.sStart);
	// 	GetDlgItem(IDC_STATIC8)->SetWindowText(m_examdeteil.sEnd);
	if (m_examdeteil.IsEmpty())
	{
		AfxMessageBox("没有试卷信息请载入！");
	}
	else
	{
		GetDlgItem(IDC_STATIC3)->SetWindowText(m_examdeteil.sPapername);
		GetDlgItem(IDC_STATIC5)->SetWindowText(m_examdeteil.sSubject);
		GetDlgItem(IDC_STATIC7)->SetWindowText(m_examdeteil.sAddress);
		CString sClass = "";
		for(int i = 0; i < m_examdeteil.arryClass.GetSize(); i++)
		{
			sClass += m_examdeteil.arryClass[i] + "\n";
		}
		GetDlgItem(IDC_STATIC9)->SetWindowText(sClass);
		GetDlgItem(IDC_STATIC13)->SetWindowText(m_examdeteil.sStart);
		GetDlgItem(IDC_STATIC15)->SetWindowText(m_examdeteil.sEnd);
	}
}

void CFormExamRollingAnswer::OnFetch1() 
{
	m_list.DeleteAllItems();
	if(m_app.FetchAnswer(m_examdeteil.nStartNO, m_examdeteil.nEndNO))
	{	
		for (int i = 0; i < m_arryStud.GetSize(); i++)
		{
			CString sIndex = "";
			sIndex.Format("%d", m_arryStud[i].m_index);
			CString strStuDev = m_arryStud[i].m_mac;
			strStuDev.TrimLeft();
			strStuDev.TrimRight();
			strStuDev.MakeUpper();
			for (int j = 0; j < m_examdeteil.m_arryStuIfo.GetSize(); j++)
			{
				CString strTempMac = m_examdeteil.m_arryStuIfo[j].sStuDev;
				strTempMac.TrimLeft();
				strTempMac.TrimRight();
				strTempMac.MakeUpper();
				if (strTempMac == strStuDev)
				{
					strcpy(m_arryStud[i].m_stuid, m_examdeteil.m_arryStuIfo[j].sStuID);
					m_list.AddItem(0, m_examdeteil.m_arryStuIfo[j].sStuID, m_examdeteil.m_arryStuIfo[j].sStuName, "", sIndex, m_examdeteil.sStart, m_examdeteil.sEnd, m_arryStud[i].m_answer);
				}
			}
		}
	}
	else
		AfxMessageBox("提取答案失败！");		
}



void CFormExamRollingAnswer::OnSave() 
{
	if (m_arryStud.GetSize() == 0)
	{
		AfxMessageBox("没有提取答案，无法保存！");
		return;
	}
	m_app.GreateExamInfo();	
	
}

void CFormExamRollingAnswer::OnUp() 
{
	// TODO: Add your control notification handler code here
	
}
