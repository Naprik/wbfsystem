#include "stdafx.h"
#include "hardctrl.h"
#include "FormExamBlue.h"
#include "MainFrm.h"
#include "ExamDetail.h"
#include "softstat.h"
#include "ExamApp.h"
#include "DlgExamstat.h"
#include "BackUp.h"
#include "Base.h"
#include "DlgExamEdit.h"
#include "DlgSingleroll.h"

IMPLEMENT_DYNCREATE(CFormExamBlue, CDialog)
extern CExamDetail  m_examdeteil;
extern CHardCtrlApp theApp;
extern CExamApp m_app;
extern CArray<CStudev, CStudev>  m_arryStud;
CFormExamBlue* pDlg;


CFormExamBlue::CFormExamBlue(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormExamBlue::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormExamBlue)
	m_systime = _T("");
	//}}AFX_DATA_INIT
}


void CFormExamBlue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamBlue)
	DDX_Control(pDX, IDC_UP, m_up);
	DDX_Control(pDX, IDC_IMPORTCLASS, m_importclass);
	DDX_Control(pDX, IDC_IMPORT, m_import);
	DDX_Control(pDX, IDC_EXPORT, m_export);
	DDX_Control(pDX, IDC_EDIT, m_edit);
	DDX_Control(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_ROLLING, m_rolling);
	DDX_Control(pDX, IDC_ROLL, m_roll);
	DDX_Control(pDX, IDC_STULIST, m_stulist);
	DDX_Text(pDX, IDC_STATIC17, m_systime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamBlue, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormExamBlue)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_BN_CLICKED(IDC_ROLL, OnRoll)
	ON_BN_CLICKED(IDC_ROLLING, OnRolling)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	ON_BN_CLICKED(IDC_IMPORT, OnImport)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_IMPORTCLASS, OnImportclass)
	ON_NOTIFY(NM_DBLCLK, IDC_STULIST, OnDblclkStulist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamBlue message handlers

BOOL CFormExamBlue::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();	
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC2),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC3),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC4),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC5),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC6),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC7),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC8),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC9),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC10),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC11),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC12),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC13),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC14),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC15),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC16),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC17),mdRelative,mdRelative);
	AddSzControl(m_stulist,mdRelative,mdRelative);	
	
	AddSzControl(*GetDlgItem(IDC_SEARCH),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_IMPORT),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_EDIT),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_IMPORTCLASS),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_ROLL),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_UP),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_ROLLING),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_EXPORT),mdRelative,mdRelative);
	

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

	m_stulist.SetHeadings("学号,150;姓名,100;班级,100;准考证号,100;当前状态,150;开考时间,150;交卷时间,150;学生蓝牙地址,100");
	SetTimer(1,1000,NULL);

	m_isroll = false;
	m_isrolling = false;

	return TRUE;  
}

void CFormExamBlue::SetInfo()
{
// 	CoInitialize(NULL);
// 	CDBStuInfo m_StuInfodb;
// 	if(!m_StuInfodb.Connect())
// 	{
// 		AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		m_stulist.DeleteAllItems();
// 		for(int j = 0; j < m_examdeteil.arryClass.GetSize(); j++)
// 		{
// 			int flag = m_StuInfodb.LoadByClass(m_examdeteil.arryClass[j]);
// 			while(flag)
// 			{
// 				m_stulist.AddItem(0,m_StuInfodb.m_stuid,m_StuInfodb.m_stuname,m_StuInfodb.m_Stuclass,"","","","",m_StuInfodb.m_devid);
// 				flag = m_StuInfodb.Move();
// 			}
// 
// 		}
// 		m_StuInfodb.Close();
// 	}
// 	CoUninitialize();

	if (m_kind == 3)
	{
		m_rolling.ShowWindow(FALSE);
		m_roll.ShowWindow(TRUE);
		m_search.ShowWindow(TRUE);
		m_up.ShowWindow(TRUE);
		m_export.ShowWindow(FALSE);
		
	}
	if (m_kind == 6)
	{
		m_rolling.ShowWindow(TRUE);
		m_roll.ShowWindow(FALSE);
		m_search.ShowWindow(FALSE);
		m_up.ShowWindow(FALSE);
		m_export.ShowWindow(TRUE);
		m_export.EnableWindow(FALSE);
	}
}


void CFormExamBlue::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 1)
	{
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
		m_systime.Format("%2d:%2d:%2d",sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
		UpdateData(FALSE);
	}
	cdxCSizingDialog::OnTimer(nIDEvent);
}

void CFormExamBlue::OnUp() 
{
	m_examdeteil.nSetp = 4;
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;	
	fram->m_dlgMain.ShowDlg(IDD_FORM_LISTEN);		
}

ULONG SearchThread(LPVOID pParam,CProgressThreadDlg* pProgressDlg)
{
	pProgressDlg->m_flag = FALSE;
	
	CFormExamBlue *dlg = (CFormExamBlue *)pParam;
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CArray<CStusearch, CStusearch> arryvalid;
	arryvalid.RemoveAll();
	CStringArray  arryList;
	for(int i = 0; i < dlg->m_stulist.GetItemCount(); i++)
	{
		arryList.Add(dlg->m_stulist.GetItemText(i,0));
	}
	
	CDlgExamstat eaxmdlg;
	do
	{
		int t1 = arryList.GetSize();
		if(!m_app.Search(arryvalid, arryList))
			return 0;
		int t2 = arryvalid.GetSize();
		CString strMessage;
		strMessage.Format("共搜索到有效学生机%d台！", arryvalid.GetSize());
		fram->m_dlgMsg.ShowMsg(strMessage);	
		
		for(i = 0; i < arryvalid.GetSize(); i++)
		{
			CString strId = arryvalid[i].m_id;
			for(int j = 0; j < dlg->m_stulist.GetItemCount(); j++)
			{
				CString strListid = dlg->m_stulist.GetItemText(j,0);
				strId.TrimRight();
				strListid.TrimRight();
				if(strListid == strId)
				{
					dlg->m_stulist.SetCheck(j);
					dlg->m_stulist.SetItemText(j,4,"检测到！");
					break;
				}
			}
		}
		CString  strReality, atrOriginal;
		strReality.Format("%d 人", arryvalid.GetSize());
		eaxmdlg.m_reality = strReality;
		atrOriginal.Format("%d 人", dlg->m_stulist.GetItemCount());
		eaxmdlg.m_original = atrOriginal;
	}while(eaxmdlg.DoModal() == IDOK);
	if(!m_app.Rest())
		AfxMessageBox("重置蓝牙失败！");
	return 0;
}
void CFormExamBlue::OnSearch() 
{
	DWORD dwFlags=0;
	dwFlags |= PSTAT_CANCEL;
	dwFlags |= PSTAT_CONFIRMCANCEL;
    ExecuteFunctionWithProgressDialog(SearchThread, 
		_T("搜索学生机"),
		this,
		dwFlags, 
									  _T("真的要退出检索吗?"));
}
ULONG RollThread(LPVOID pParam,CProgressThreadDlg* pProgressDlg)   //对学生机点名（发送考试信息）
{
	CFormExamBlue *dlg = (CFormExamBlue *)pParam;
	
	int iCount = m_app.Get_stud_count();
	pProgressDlg->m_pos = 0;
    pProgressDlg->SetRange(iCount);
	pProgressDlg->m_flag = TRUE;
	
	CStringArray arryRoll;
	CStringArray arryRolling;
	int times = 0;
	while(true)
	{
		Sleep(1500);
		int iRoll = 0;
		int iRolling = 0;
		BOOL bRoll = FALSE;
		BOOL bRolling = FALSE;
		
		m_app.Get_stat(arryRoll, arryRolling, bRoll, iRoll, bRolling, iRolling);
		for(int i = 0; i < arryRoll.GetSize(); i++)
		{
			CString strID = arryRoll[i];
			strID.TrimRight();
			for(int j = 0; j < dlg->m_stulist.GetItemCount(); j++)
			{
				if(strID == dlg->m_stulist.GetItemText(j,0))
				{
					dlg->m_stulist.SetItemText(j,4,"进入考试状态！");
					COleDateTime tStart = m_app.Get_start_time(strID);
					dlg->m_stulist.SetItemText(j,5,tStart.Format());
					break;
				}
			}
		}
		pProgressDlg->m_pos = iRoll;
		if(bRoll == TRUE)
		{	times++;
		if(times > 1)
			break;
		}
	}
	return 0;
}

void CFormExamBlue::OnRoll() 
{
	CStringArray  arryvaild;
	for(int i = 0; i < m_stulist.GetItemCount(); i++)
	{
		if(m_stulist.GetCheck(i))
		{
			CString strID = m_stulist.GetItemText(i,0);
			strID.TrimRight();
			arryvaild.Add(strID);
		}
	}
	COleDateTime m_starttime, m_endtime;
	m_starttime.ParseDateTime(m_examdeteil.sStart);
	m_endtime.ParseDateTime(m_examdeteil.sEnd);
	if(arryvaild.GetSize() != 0)
	{
		if(!m_app.Rollcall_all_new(arryvaild,m_starttime,m_endtime,m_examdeteil.nMaxNO))
			return ;
		DWORD dwFlags=0;
		dwFlags |= PSTAT_CANCEL;
		dwFlags |= PSTAT_CONFIRMCANCEL;
		ExecuteFunctionWithProgressDialog(RollThread, 
										  _T("开考"),
										  this,
										  dwFlags, 
										  _T("真的要退出检索吗?"));
		m_examdeteil.bIsRollDone = TRUE;
	}

	m_roll.EnableWindow(FALSE);
//	m_rolling.EnableWindow(FALSE);
	m_search.EnableWindow(FALSE);
	m_import.EnableWindow(FALSE);
	m_up.EnableWindow(FALSE);
	m_importclass.EnableWindow(FALSE);
	m_edit.EnableWindow(FALSE);
	m_isroll = true;

}
ULONG RollingThread(LPVOID pParam,CProgressThreadDlg* pProgressDlg)      // 收卷
{
	CFormExamBlue *dlg = (CFormExamBlue *)pParam;	
	int iCount = m_app.Get_stud_count();
	pProgressDlg->m_pos = 0;
    pProgressDlg->SetRange(iCount);
	
	CStringArray arryRoll;
	CStringArray arryRolling;
	int times = 0;
	while(true)
	{
		Sleep(2000);
		int iRoll = 0;
		int iRolling = 0;
		BOOL bRoll = FALSE;
		BOOL bRolling = FALSE;
		
		m_app.Get_stat(arryRoll, arryRolling, bRoll, iRoll, bRolling, iRolling);
		for(int i = 0; i < arryRolling.GetSize(); i++)
		{
			CString strID = arryRolling[i];
			strID.TrimRight();
			for(int j = 0; j < dlg->m_stulist.GetItemCount(); j++)
			{
				if(strID == dlg->m_stulist.GetItemText(j,0))
				{
					dlg->m_stulist.SetItemText(j,4,"已收卷！");
					COleDateTime tEnd = m_app.Get_end_time(strID);
					dlg->m_stulist.SetItemText(j,6,tEnd.Format());
					break;
				}
			}
		}
		pProgressDlg->m_pos = iRolling;
		if(bRolling == TRUE)
		{	times++;
		if(times > 1)
			break;
		}
	}
	
	Sleep(3000);
	m_app.Get_answer(m_examdeteil.nStartNO, m_examdeteil.nEndNO, m_examdeteil.sSubject, m_examdeteil.nPaperid, m_examdeteil.sAddress);
	
	dlg->m_export.EnableWindow(TRUE);
	
	m_examdeteil.bIsRollingDone = TRUE;
	return 0;
}
void CFormExamBlue::OnRolling() 
{
	COleDateTime m_endtime;
	m_endtime.ParseDateTime(m_examdeteil.sEnd);
	
	if(!m_app.Rolling_all(m_examdeteil.nStartNO, m_examdeteil.nEndNO, m_endtime))
		return;
	DWORD dwFlags=0;
	dwFlags |= PSTAT_CANCEL;
	dwFlags |= PSTAT_CONFIRMCANCEL;
    ExecuteFunctionWithProgressDialog(RollingThread, 
		_T("收卷"),
		this,
		dwFlags, 
									  _T("真的要退出检索吗?"));	
	m_isrolling = true;
}

void  CFormExamBlue::SetKind(int kind)
{
	m_kind = kind;
}

void CFormExamBlue::OnExport() 
{
	m_app.GreateExamInfo();	
}

void CFormExamBlue::OnImport() 
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
	m_stulist.DeleteAllItems();

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

void CFormExamBlue::setdetail()
{
	m_stulist.DeleteAllItems();
	for (int i = 0; i < m_examdeteil.m_arryStuIfo.GetSize(); i++)
	{
		m_stulist.AddItem(0,m_examdeteil.m_arryStuIfo[i].sStuID,m_examdeteil.m_arryStuIfo[i].sStuName,m_examdeteil.m_arryStuIfo[i].sStuClass,"","","","",m_examdeteil.m_arryStuIfo[i].sStuDev);
	}
	
	GetDlgItem(IDC_STATIC3)->SetWindowText(m_examdeteil.sPapername);
	GetDlgItem(IDC_STATIC5)->SetWindowText(m_examdeteil.sSubject);
	GetDlgItem(IDC_STATIC7)->SetWindowText(m_examdeteil.sAddress);
	CString sClass = "";
	for(i = 0; i < m_examdeteil.arryClass.GetSize(); i++)
	{
		sClass += m_examdeteil.arryClass[i] + "\n";
	}
	GetDlgItem(IDC_STATIC9)->SetWindowText(sClass);
	int m_account = m_stulist.GetItemCount();
	CString sCount;
	sCount.Format("%d 人", m_account);
	GetDlgItem(IDC_STATIC11)->SetWindowText(sCount);
	GetDlgItem(IDC_STATIC13)->SetWindowText(m_examdeteil.sStart);
	GetDlgItem(IDC_STATIC15)->SetWindowText(m_examdeteil.sEnd);
}

void CFormExamBlue::OnEdit() 
{
	CDlgExamEdit dlg;
	if (dlg.DoModal() == IDOK)
	{
		setdetail();
	}
}

void CFormExamBlue::OnImportclass() 
{
	CBase base;
	CJahCoverCapture capture;
	capture.m_bmpFile = base.GetExePath() + "bak.ini";
	CString strPath = capture.m_bmpFile;
	
	CBackUp bk;
	int iCount = 1;
	m_examdeteil.m_arryStuIfo.RemoveAll();
	m_stulist.DeleteAllItems();
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
	for (int i = 0; i < m_examdeteil.m_arryStuIfo.GetSize(); i++)
	{
		m_stulist.AddItem(0,m_examdeteil.m_arryStuIfo[i].sStuID,m_examdeteil.m_arryStuIfo[i].sStuName,m_examdeteil.m_arryStuIfo[i].sStuClass,"","","","",m_examdeteil.m_arryStuIfo[i].sStuDev);
	}
	
}

void CFormExamBlue::OnDblclkStulist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item =pNMListView->iItem;	
	CString strStuNO = m_stulist.GetItemText(item,0);
	if(strStuNO != "")
	{
		if (m_isroll && (!m_isrolling))
		{
			CDlgSingleroll dlg;
			dlg.m_id = m_stulist.GetItemText(item,0);
			dlg.m_name = m_stulist.GetItemText(item,1);
			dlg.m_depart = m_stulist.GetItemText(item,2);
			dlg.m_suject = m_examdeteil.sSubject;
			dlg.m_addr = m_examdeteil.sAddress;
			dlg.m_start = m_examdeteil.sStart;
			dlg.m_end = m_examdeteil.sEnd;
			COleDateTime tStart, tEnd;
			tStart.ParseDateTime(m_examdeteil.sStart);
			tEnd.ParseDateTime(m_examdeteil.sEnd);
			if (dlg.DoModal() == IDOK)
			{
				m_app.Rollcall_single_new(strStuNO, tStart, tEnd);
				BOOL bRoll = FALSE;
				BOOL bRolling = FALSE;
				m_app.Get_singlemac_stat(strStuNO,bRoll,bRolling);
				if (bRoll)
				{
					m_stulist.SetItemText(item,4,"进入考试状态！");
				}
			}
		}
		
	}
	
	*pResult = 0;
}
