// FormExamInrarOff.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormExamInrarOff.h"
#include "MainFrm.h"
#include "ExamDetail.h"
#include "ExamApp.h"
#include "Studev.h"
#include "DlgExamEdit.h"
#include "Base.h"

extern CHardCtrlApp theApp;
extern CExamApp m_app;
extern CExamDetail  m_examdeteil;
extern CArray<CStudev, CStudev>  m_arryStud;

IMPLEMENT_DYNCREATE(CFormExamInrarOff, CDialog)

CFormExamInrarOff::CFormExamInrarOff(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormExamInrarOff::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormExamInrarOff)
	//}}AFX_DATA_INIT
}


void CFormExamInrarOff::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamInrarOff)
	DDX_Control(pDX, IDC_SENDINFO, m_sendinfo);
	DDX_Control(pDX, IDC_IMPORT, m_import);
	DDX_Control(pDX, IDC_EDIT, m_edit);
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamInrarOff, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormExamInrarOff)
	ON_BN_CLICKED(IDC_IMPORT, OnImport)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_SENDINFO, OnSendinfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamInrarOff message handlers

BOOL CFormExamInrarOff::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();
	
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_IMPORT),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_EDIT),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_SENDINFO),mdRelative,mdRelative);
	AddSzControl(m_list,mdRelative,mdRelative);
	
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
	m_list.SetHeadings("考试信息,100;内容,200");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFormExamInrarOff::OnImport() 
{
 	static char BASED_CODE szFilter[] = "All Files (*.ini)|*.ini||";
 	CFileDialog dlg(TRUE,NULL,"*.ini",NULL,szFilter);
 	CString strPath = "";	
 	if ( dlg.DoModal() == IDOK)
 	{
 		strPath = dlg.GetPathName();
		strPath.Replace("\\", "\\\\");		
 	}
	else
		return;

	m_examdeteil.Initdata();
//	m_stulist.DeleteAllItems();

	CBackUp bk;
	CString strRoll = bk.Load("ROLLMODE",strPath);
 	m_examdeteil.nRoll_pattern = atoi(strRoll);

 	CString strRolling  = bk.Load("ROLLINGMODE", strPath);
	m_examdeteil.nRolling_pattern = atoi(strRolling);

 	CString strExamquality = bk.Load("EXAMQUALITY", strPath); 	
 	m_examdeteil.nExam_quality = atoi(strExamquality);

	int iCount = 0;
	while (true)
	{
		CString strTitle = "";
		strTitle.Format("CLASS%d", iCount);
		CString strClass = bk.Load(strTitle, "CLASS", strPath);
		if (strClass != "")
		{
			m_examdeteil.arryClass.Add(strClass);
		}
		else
			break;
		iCount++;
	}

 	CString strExamAddr = bk.Load("EXAMADDR", strPath);
 	m_examdeteil.sAddress = strExamAddr;
 
 	CString strPaperName = bk.Load("PAPERNAME", strPath);
 	m_examdeteil.sPapername = strPaperName;
 
 	CString strSubject = bk.Load("SUBJECTNAME", strPath);
 	m_examdeteil.sSubject = strSubject;
 
 
 	CString strStartTime = bk.Load("STARTTIME", strPath);
 	m_examdeteil.sStart = strStartTime;
 
 	CString strEndTime = bk.Load("ENDTIME", strPath);
 	m_examdeteil.sEnd = strEndTime;
 
 	CString strStartNO = bk.Load("STARTNO", strPath);
 	m_examdeteil.nStartNO = atoi(strStartNO);
 
 	CString strEndNO = bk.Load("ENDNO", strPath);
 	m_examdeteil.nEndNO = atoi(strEndNO);
 
 	CString strMaxNO = bk.Load("MAXNO", strPath);
 	m_examdeteil.nMaxNO = atoi(strMaxNO);
 
 	CString strListen = bk.Load("LISTEN", strPath);
 	if (strListen == "1")
 	{
 		m_examdeteil.b_listen = TRUE;
 		CString strPart1 = bk.Load("PART1", strPath);
 		if (strPart1 == "1")
 		{
 			m_examdeteil.b_part1 = TRUE;
 			CString strStarttime1 = bk.Load("STARTPART1", strPath);
 			m_examdeteil.m_start1.ParseDateTime(strStarttime1);
			CString strEndTime1 = bk.Load("ENDTPART1", strPath);
 			m_examdeteil.m_end1.ParseDateTime(strEndTime1);
 			
 		}
 		else
 			m_examdeteil.b_part1 = FALSE;
 	
 		CString strPart2 = bk.Load("PART2", strPath);
 		if (strPart2 == "1")
 		{
 			m_examdeteil.b_part2 = TRUE;
 			CString strStarttime2 = bk.Load("STARTPART2", strPath);
 			m_examdeteil.m_start2.ParseDateTime(strStarttime2);
 			CString strEndTime2 = bk.Load("ENDTPART2", strPath);
 			m_examdeteil.m_end2.ParseDateTime(strEndTime2);
 
 		}
 		else
 			m_examdeteil.b_part2 = FALSE;
 
 		CString strPart3 = bk.Load("PART3", strPath);
 		if (strPart3 == "1")
 		{
 			m_examdeteil.b_part3 = TRUE;
 			CString strStarttime3 = bk.Load("STARTPART3", strPath);
 			m_examdeteil.m_start3.ParseDateTime(strStarttime3);
 			CString strEndTime3 = bk.Load("ENDTPART3", strPath);
 			m_examdeteil.m_end3.ParseDateTime(strEndTime3);
 		}
 		else
 			m_examdeteil.b_part3 = FALSE;
 
 	}
 	else
 		m_examdeteil.b_listen = FALSE;
 
 	CString strChanel = bk.Load("CHANEL", strPath);
 	m_examdeteil.m_chanal = strChanel;
 
 	
	iCount = 1;
 
  	while(true)
  	{
		CString strTitle = "";
 		strTitle.Format("STUINFO%d", iCount);
 		CString strStuID = bk.Load(strTitle, "STUID", strPath);
 		if (strStuID != "")
 		{
 			CString strStuName = bk.Load(strTitle, "STUNAME", strPath);
 			CString strStuClass = bk.Load(strTitle, "STUCLASS", strPath);
 			CString strStuDev = bk.Load(strTitle, "STUDEV", strPath);
 			CStuInfo  stu;
 			stu.sStuID = strStuID;
 			stu.sStuName = strStuName;
 			stu.sStuClass = strStuClass;
 			stu.sStuDev = strStuDev;
 			m_examdeteil.m_arryStuIfo.Add(stu);
 		}
 		else
 			break;
 		iCount++;
  	}

	setdetail();

	bk.Remove(TRUE);

	CBase base;
	CJahCoverCapture capture;
	capture.m_bmpFile = base.GetExePath() + "bak.ini";	

	BOOL flag = CopyFile(strPath,capture.m_bmpFile,FALSE);
	
}

void CFormExamInrarOff::setdetail()
{
	m_list.DeleteAllItems();
	if (m_examdeteil.nRoll_pattern == 1)
		m_list.AddItem(0, "点名方式","蓝牙点名");
	else if (m_examdeteil.nRoll_pattern == 2)
		m_list.AddItem(0, "点名方式","红外点名");
	else if (m_examdeteil.nRoll_pattern == 3)
		m_list.AddItem(0, "点名方式","USB点名");
	
	if (m_examdeteil.nRolling_pattern == 1)
		m_list.AddItem(0, "收卷方式","蓝牙收卷");
	else if (m_examdeteil.nRolling_pattern == 2)
		m_list.AddItem(0, "收卷方式","红外收卷");
	else if (m_examdeteil.nRolling_pattern == 3)
		m_list.AddItem(0, "收卷方式","USB收卷");
	
	if (m_examdeteil.nExam_quality == 1)
		m_list.AddItem(0, "考试性质","正规考试");
	else if (m_examdeteil.nExam_quality == 2)
		m_list.AddItem(0, "考试性质","一般测试");
	
	m_list.AddItem(0, "考场", m_examdeteil.sAddress);
	m_list.AddItem(0, "试卷名称", m_examdeteil.sPapername);
	m_list.AddItem(0, "试卷科目", m_examdeteil.sSubject);
	
	m_list.AddItem(0, "开考时间", m_examdeteil.sStart);
	m_list.AddItem(0, "收卷时间",m_examdeteil.sEnd);
	
	CString temp = "";
	temp.Format("%d", m_examdeteil.nStartNO);
	m_list.AddItem(0, "开始题号",temp);
	
	temp = "";
	temp.Format("%d", m_examdeteil.nEndNO);
	m_list.AddItem(0, "结束题号",temp);
	
	temp = "";
	temp.Format("%d", m_examdeteil.nMaxNO);
	m_list.AddItem(0, "最大题号", temp);
	
	if (m_examdeteil.b_listen)
	{
		if (m_examdeteil.b_part1)
		{
			m_list.AddItem(0, "听力1 开始时间",m_examdeteil.m_start1.Format());
			m_list.AddItem(0, "听力1 结束时间",m_examdeteil.m_end1.Format());
		}
		if (m_examdeteil.b_part2)
		{
			m_list.AddItem(0, "听力2 开始时间",m_examdeteil.m_start2.Format());
			m_list.AddItem(0, "听力2 结束时间",m_examdeteil.m_end2.Format());
			
		}
		if (m_examdeteil.b_part3)
		{
			m_list.AddItem(0, "听力3 开始时间",m_examdeteil.m_start3.Format());
			m_list.AddItem(0, "听力3 结束时间",m_examdeteil.m_end3.Format());
		}
		
		m_list.AddItem(0, "听力频段",m_examdeteil.m_chanal);
	}	
}

void CFormExamInrarOff::OnEdit() 
{
	CDlgExamEdit dlg;
	if (dlg.DoModal() == IDOK)
	{
		setdetail();
	}		
}

void CFormExamInrarOff::OnSendinfo() 
{
	if (m_app.Rollcall_infra())
	{
		AfxMessageBox("向教师机发送考试信息成功！");
	}
	
}
