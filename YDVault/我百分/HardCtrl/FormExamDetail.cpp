// FormExamDetail.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormExamDetail.h"
#include "MainFrm.h"
#include "ExamDetail.h"
#include "BackUp.h"
#include "ExamApp.h"
#include "TeacherDev.h"

extern CHardCtrlApp theApp;
extern CExamDetail  m_examdeteil;
extern CExamApp m_app;
extern CArray<CTeacherDev, CTeacherDev>  arryTeacherdev;
IMPLEMENT_DYNCREATE(CFormExamDetail, CDialog)

CFormExamDetail::CFormExamDetail(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormExamDetail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormExamDetail)
	//}}AFX_DATA_INIT
}


void CFormExamDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamDetail)
	DDX_Control(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_NEXT, m_next);
	DDX_Control(pDX, IDC_CHOOSE, m_choose);
	DDX_Control(pDX, IDC_DEVLIST, m_devlist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamDetail, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormExamDetail)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_BN_CLICKED(IDC_CHOOSE, OnChoose)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamDetail message handlers

BOOL CFormExamDetail::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	AddSzControl(*GetDlgItem(IDC_STATIC2),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_SEARCH),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_CHOOSE),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_NEXT),mdRelative,mdRelative);
	AddSzControl(m_devlist,mdRelative,mdRelative);

	
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
	
	m_devlist.SetHeadings("教师机编号, 80; 使用者,150;使用端口,100;机状态, 150");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFormExamDetail::SetKind(int kind)
{
	m_kind = kind;
}

void CFormExamDetail::OnSearch() 
{
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	if(!m_app.Test())
	{
		AfxMessageBox("没有检测到教师机！");
		return;
	}
	int iCount = arryTeacherdev.GetSize();
	
	CString strMessage;
	strMessage.Format("共发现教师机 %d 台", iCount);
	fram->m_dlgMsg.ShowMsg(strMessage);	
	
	for (int i = 0; i < arryTeacherdev.GetSize(); i++)
	{
		CString strMac, strCom;
		strMac.Format("%s",arryTeacherdev[i].m_mac);
		strCom.Format("%s",arryTeacherdev[i].m_com);
		m_devlist.AddItem(0, strMac, "", strCom ,"检测到，可使用");
		m_devlist.SetCheck(i);
	}	
}

void CFormExamDetail::OnChoose() 
{
	if(m_devlist.GetItemCount() == 0)
	{
		AfxMessageBox("没有教师机可以选择，请先点击“搜索”按钮，搜索教师机！");
		return;
	}
	
	CString strTemp = "";
	int i = 0;
	int iCount = 0;
	for(; i < m_devlist.GetItemCount() ; i++)
	{
		if(m_devlist.GetCheck(i))
		{
			CString strTeacherCOM = m_devlist.GetItemText(i,0);			
			strTemp += strTeacherCOM + " ";
			iCount++;
		}
	}
	if(iCount == 0)
	{
		AfxMessageBox("没有选择教师机，请在在要使用的教师机前打勾！");
		return;
	}	
	CString msg;
	msg.Format("确定要使用教师机 %s 参加本场考试？", strTemp);
	if ( MessageBox(msg,"确认",MB_OKCANCEL) != IDOK)
		return;	
	
	
	
	m_app.Opencom();
	
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	for(i = 0; i < m_devlist.GetItemCount(); i++)
	{
		if(m_app.Get_teach_stat(m_devlist.GetItemText(i,2)))
		{
			CString strMessage;
			strMessage.Format("教师机 %s 确认被使用！", m_devlist.GetItemText(i,0));
			fram->m_dlgMsg.ShowMsg(strMessage);			
			m_devlist.SetItemText(i,3,"使用中！");
		}
		else
		{	
			CString strMessage;
			strMessage.Format("教师机 %s 未被使用！", m_devlist.GetItemText(i,0));
			fram->m_dlgMsg.ShowMsg(strMessage);				
			m_devlist.SetItemText(i,3,"未被使用！");
		}
	}		
	
}

void CFormExamDetail::OnNext() 
{
	if (m_kind == 3)
	{
		
		CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
		fram->m_dlgMain.m_examblue.SetKind(m_kind);
		fram->m_dlgMain.m_examblue.SetInfo();
		fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_BLUE);
//		fram->m_dlgMain.m_examblue.ImportDefault();		
	}
	if (m_kind == 4)
	{
// 		CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
// 		fram->m_dlgMain.m_examinfara.SetKind(m_kind);
// 		fram->m_dlgMain.m_examinfara.SetInfo();
// 		fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_INRARA);
	}
	if (m_kind == 201)
	{
 		CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
 		fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_INRARA_OFF);
	}
	if (m_kind == 301)
	{
// 		CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
// 		fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_INRARA);
		
	}
	if (m_kind == 7)
	{
 		CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
 		fram->m_dlgMain.m_examrollinganswer.SetInfo();
 		fram->m_dlgMain.ShowDlg(IDD_FORM_ROLLING_INRAR_ANSWER);
	}
	
}
