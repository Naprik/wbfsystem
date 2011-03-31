#include "stdafx.h"
#include "hardctrl.h"
#include "FormPCoffRolling.h"
#include "MainFrm.h"
#include "TeacherDev.h"
#include "ExamApp.h"
#include "DlgPaperQuery.h"

extern CHardCtrlApp theApp;
extern CArray<CTeacherDev, CTeacherDev>  arryTeacherdev;
extern CExamApp m_app;

IMPLEMENT_DYNCREATE(CFormPCoffRolling, CDialog)

CFormPCoffRolling::CFormPCoffRolling(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormPCoffRolling::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormPCoffRolling)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormPCoffRolling::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormPCoffRolling)
	DDX_Control(pDX, IDC_UP, m_up);
	DDX_Control(pDX, IDC_DOWN, m_next);
	DDX_Control(pDX, IDC_TEACHLIST, m_teachlist);
	DDX_Control(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_CHOSE, m_chose);
	DDX_Control(pDX, IDC_SHOW, m_show);
	DDX_Control(pDX, IDC_SAVE, m_save);
	DDX_Control(pDX, IDC_QUERY, m_query);
	DDX_Control(pDX, IDC_FETCH, m_fetch);
	DDX_Control(pDX, IDC_PAPERLIST, m_paperlist);
	DDX_Control(pDX, IDC_ANSWERLIST, m_answerlist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormPCoffRolling, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormPCoffRolling)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_BN_CLICKED(IDC_CHOSE, OnChose)
	ON_BN_CLICKED(IDC_QUERY, OnQuery)
	ON_BN_CLICKED(IDC_SHOW, OnShow)
	ON_BN_CLICKED(IDC_FETCH, OnFetch)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormPCoffRolling message handlers

BOOL CFormPCoffRolling::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();
	
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdResize,mdResize);
	AddSzControl(*GetDlgItem(IDC_STATIC2),mdResize,mdResize);
	AddSzControl(*GetDlgItem(IDC_STATIC4),mdResize,mdResize);
	AddSzControl(*GetDlgItem(IDC_SHOW),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_QUERY),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_FETCH),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_UP),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_DOWN),mdNone,mdRepos);

	
	AddSzControl(*GetDlgItem(IDC_SAVE ),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_CHOSE),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_SEARCH ),mdNone,mdRepos);

	
	AddSzControl(m_paperlist,mdResize,mdResize);
	AddSzControl(m_answerlist,mdResize,mdResize);
	AddSzControl(m_teachlist,mdResize,mdResize);	

	DWORD dwStyle=GetWindowLong(m_teachlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_teachlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_teachlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_teachlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_teachlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_teachlist.SetExtendedStyle(dwStyle);

	m_teachlist.SetHeadings("教师机编号, 80; 使用者,150;使用端口,100;机状态, 150");	

	dwStyle=GetWindowLong(m_paperlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_paperlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  

	m_paperlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_paperlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_paperlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_paperlist.SetExtendedStyle(dwStyle);


    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );

 	m_paperlist.ClearHeadings();
	m_paperlist.SetHeadings("考卷名称,100;考试科目,100; 参考院系,100; 参考班级,100;开考时间,150;收卷试卷,150;开始题号,100;结束题号,100;最大题号,100;试卷编号,0"); 	

	dwStyle=GetWindowLong(m_answerlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_answerlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  

	m_answerlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_answerlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_answerlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_answerlist.SetExtendedStyle(dwStyle);


    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );

 	m_answerlist.ClearHeadings();
	m_answerlist.SetHeadings("学生学号,100;学生姓名,100; 答案,300"); 
	
	m_step = 1;
	b_isOpencom = FALSE;
	m_up.EnableWindow(FALSE);
	m_next.EnableWindow(TRUE);

	m_search.ShowWindow(TRUE);
	m_chose.ShowWindow(TRUE);
	m_teachlist.ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC4)->ShowWindow(TRUE);

	m_query.ShowWindow(FALSE);
	m_show.ShowWindow(FALSE);
	m_paperlist.ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC3)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);

	m_fetch.ShowWindow(FALSE);
	m_save.ShowWindow(FALSE);
	m_answerlist.ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);


	
	return TRUE;
}

void CFormPCoffRolling::OnSearch() 
{
	m_teachlist.DeleteAllItems();	
	if(m_app.TestStu())
	{
		int i = arryTeacherdev.GetSize();
		if(i <= 0)
			AfxMessageBox("没有搜索到教师机！");
		else
		{
			CString strCom = arryTeacherdev[0].m_com;
			strCom.TrimLeft();
			strCom.TrimRight();
			CString strMac = arryTeacherdev[0].m_mac;
			strMac.TrimLeft();
			strMac.TrimRight();
// 			CoInitialize(NULL);
// 			CDBTeacherDev m_TeacherDevdb;
// 			if ( !m_TeacherDevdb.Connect())
// 			{
// 				AfxMessageBox("TeacherDev 无法连接到数据库服务器");
// 			}
// 			else
// 			{
// 				for(int i = 0; i < arryTeacherdev.GetSize(); i++)
// 				{
// 					CString strMac, strCom;
// 					strMac.Format("%s",arryTeacherdev[i].m_mac);
// 					strCom.Format("%s",arryTeacherdev[i].m_com);
// 					int flag = m_TeacherDevdb.LoadByDevmac(strMac);
// 					if(flag)
// 					{
// 						m_teachlist.AddItem(0, m_TeacherDevdb.m_devid, m_TeacherDevdb.m_teachername, strCom ,"检测到，可使用");
// 						m_teachlist.SetCheck(i);
// 					}
// 				}
// 				m_TeacherDevdb.Close();
// 			}
// 			CoUninitialize();
		}
	}
	else
		AfxMessageBox("没有搜索到教师机！");
}

void CFormPCoffRolling::OnChose() 
{
	if(m_teachlist.GetItemCount() == 0)
	{
		AfxMessageBox("没有教师机可以选择，请先点击“搜索”按钮，搜索教师机！");
		return;
	}
	if(m_app.OpenStucom())
	{
		AfxMessageBox("打开成功！");
		b_isOpencom = TRUE;
	}
	else
	{
		AfxMessageBox("打开失败！");
		b_isOpencom = FALSE;
	}
}

void CFormPCoffRolling::OnQuery() 
{
	CDlgPaperQuery  dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_paperlist.DeleteAllItems();
		COleDateTime time;
		time.SetDate(dlg.m_time.GetYear(),dlg.m_time.GetMonth(),dlg.m_time.GetDay());
// 		CoInitialize(NULL);
// 		CDBPaper       m_Paperdb;
// 		if ( !m_Paperdb.Connect())
// 		{
// 			AfxMessageBox("Paperdb: 无法连接到数据库服务器");
// 		}
// 		else
// 		{
// 			BOOL flag = m_Paperdb.LoadByQuery(dlg.strPaper,
// 											  dlg.strSubject,
// 											  dlg.strDepart,
// 											  dlg.strClass,
// 											  time,dlg.bIstime);
// 			while(flag)
// 			{
// 				CString tep1,tep2,tep3,strID;
// 				tep1.Format("%d",m_Paperdb.m_startNO);
// 				tep2.Format("%d",m_Paperdb.m_endNO);
// 				tep3.Format("%d",m_Paperdb.m_maxNO);
// 				strID.Format("%d",m_Paperdb.m_ydid);
// 				m_paperlist.AddItem(0,m_Paperdb.m_papername,
// 								m_Paperdb.m_subject,
// 								m_Paperdb.m_depart,
// 								m_Paperdb.m_class,
// 								m_Paperdb.m_starttime.Format(),
// 								m_Paperdb.m_endtime.Format(),
// 								tep1,tep2,tep3,strID);
// 				flag = m_Paperdb.Move();
// 			}
// 			m_Paperdb.Close();
// 		}
// 		CoUninitialize();
	}	
}

void CFormPCoffRolling::OnShow() 
{
	m_paperlist.DeleteAllItems();
// 	CoInitialize(NULL);
// 	CDBPaper  m_Paperdb;
// 	if ( !m_Paperdb.Connect())
// 	{
// 		AfxMessageBox("Paperdb: 无法连接到数据库服务器");
// 	}	
// 	else
// 	{
// 		BOOL flag = m_Paperdb.LoadByAll();
// 		while(flag)
// 		{
// 			CString tep1,tep2,tep3,strID;
// 			tep1.Format("%d",m_Paperdb.m_startNO);
// 			tep2.Format("%d",m_Paperdb.m_endNO);
// 			tep3.Format("%d",m_Paperdb.m_maxNO);
// 			strID.Format("%d",m_Paperdb.m_ydid);
// 			m_paperlist.AddItem(0,m_Paperdb.m_papername,
// 							m_Paperdb.m_subject,
// 							m_Paperdb.m_depart,
// 							m_Paperdb.m_class,
// 							m_Paperdb.m_starttime.Format(),
// 							m_Paperdb.m_endtime.Format(),
// 							tep1,tep2,tep3,strID);
// 			flag = m_Paperdb.Move();
// 		}
// 		m_Paperdb.Close();
// 	}
// 	CoUninitialize();	
	
}

void CFormPCoffRolling::OnFetch() 
{
	int iStartNO, iEndNO, iPaperID;
	CString addr = "";
	CString starttime,endtime;
	for(int i = 0; i < m_paperlist.GetItemCount(); i++)
	{
		if(m_paperlist.GetCheck(i))
		{
			CString strStartNO = m_paperlist.GetItemText(i,6);
			CString strEndN0 = m_paperlist.GetItemText(i,7);
			iStartNO = atoi(strStartNO);
			iEndNO = atoi(strEndN0);
			iPaperID = atoi(m_paperlist.GetItemText(i,9));
			starttime =m_paperlist.GetItemText(i,4);
			endtime = m_paperlist.GetItemText(i,5);
			break;
		}
	}
	COleDateTime tstart,tend;
	tstart.ParseDateTime(starttime);
	tend.ParseDateTime(endtime);
	CStringArray answerlist,maclist;
	answerlist.RemoveAll();
	maclist.RemoveAll();
	m_app.Getoffanswer(answerlist,iStartNO,iEndNO);
	Sleep(1000);
	m_app.Getrelationmap(maclist);
	if(answerlist.GetSize() != maclist.GetSize())
	{
		AfxMessageBox("答案提取失败！无法对应答案关系！");
		return;
	}

// 	CoInitialize(NULL);
// 	CDBStuInfo  m_StuInfodb;
// 	if ( !m_StuInfodb.Connect())
// 	{
// 		AfxMessageBox("StuInfodb: 无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		for(i = 0; i < answerlist.GetSize(); i++)
// 		{
// 			CString mac = maclist[i];
// 			mac.TrimLeft();
// 			mac.TrimRight();
// 			mac.MakeUpper();
// 			BOOL flag = m_StuInfodb.LoadByDev(mac);
// 			if(flag)
// 			{
// 				CString stuid = m_StuInfodb.m_stuid;
// 				CString stuname = m_StuInfodb.m_stuname;
// 				m_answerlist.AddItem(0,stuid,stuname,answerlist[i]);
// 			}
// 		}
// 		m_StuInfodb.Close();
// 	}
// 	CoUninitialize();	
}

void CFormPCoffRolling::OnSave() 
{
	int iPaperID;
	CString addr = "";
	CString starttime,endtime;
	for(int i = 0; i < m_paperlist.GetItemCount(); i++)
	{
		if(m_paperlist.GetCheck(i))
		{
			iPaperID = atoi(m_paperlist.GetItemText(i,9));
			starttime =m_paperlist.GetItemText(i,4);
			endtime = m_paperlist.GetItemText(i,5);
			break;
		}
	}
	COleDateTime tstart,tend;
	tstart.ParseDateTime(starttime);
	tend.ParseDateTime(endtime);
	
// 	CoInitialize(NULL);
// 	CDBResult	m_Resultdb;	
// 	if ( !m_Resultdb.Connect())
// 	{
// 		AfxMessageBox("Resultdb: 无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		for(int i = 0; i < m_answerlist.GetItemCount(); i++)
// 		{
// 			CString stuid = m_answerlist.GetItemText(i,0);
// 			CString strAnwer = m_answerlist.GetItemText(i,2);
// 
// 			m_Resultdb.m_ydid = 0;
// 			m_Resultdb.m_stuid = stuid;
// 			m_Resultdb.m_paperid = iPaperID;
// 			m_Resultdb.m_starttime = tstart;
// 			m_Resultdb.m_endtime = tend;
// 			m_Resultdb.m_stuanswer = strAnwer;
// 
// 			if(!m_Resultdb.Add())
// 			{
// 				CString msg;
// 				msg.Format("学号为：%s答案添加失败！", stuid);
// 				AfxMessageBox(msg);
// 			}
// 		}
// 		AfxMessageBox("保存成功！");
// 		m_Resultdb.Close();
// 	}
// 	CoUninitialize();	
}

void CFormPCoffRolling::OnUp() 
{
	m_step--;
	if(m_step == 1)
	{
		m_up.EnableWindow(FALSE);
		m_next.EnableWindow(TRUE);

		m_search.ShowWindow(TRUE);
		m_chose.ShowWindow(TRUE);
		m_teachlist.ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(TRUE);

		m_query.ShowWindow(FALSE);
		m_show.ShowWindow(FALSE);
		m_paperlist.ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC3)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);

		m_fetch.ShowWindow(FALSE);
		m_save.ShowWindow(FALSE);
		m_answerlist.ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);
	}
	else
	{
		m_up.EnableWindow(TRUE);
		m_next.EnableWindow(TRUE);

		m_search.ShowWindow(FALSE);
		m_chose.ShowWindow(FALSE);
		m_teachlist.ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(FALSE);

		m_query.ShowWindow(TRUE);
		m_show.ShowWindow(TRUE);
		m_paperlist.ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC1)->ShowWindow(TRUE);

		m_fetch.ShowWindow(FALSE);
		m_save.ShowWindow(FALSE);
		m_answerlist.ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);
	}
	
}

void CFormPCoffRolling::OnDown() 
{
	m_step++;
	if(m_step == 2)
	{
		if(b_isOpencom == TRUE)
		{
			m_up.EnableWindow(TRUE);
			m_next.EnableWindow(TRUE);

			m_search.ShowWindow(FALSE);
			m_chose.ShowWindow(FALSE);
			m_teachlist.ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC4)->ShowWindow(FALSE);

			m_query.ShowWindow(TRUE);
			m_show.ShowWindow(TRUE);
			m_paperlist.ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC1)->ShowWindow(TRUE);

			m_fetch.ShowWindow(FALSE);
			m_save.ShowWindow(FALSE);
			m_answerlist.ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);
		}
		else
		{
			AfxMessageBox("没有选择教师，无法进行下一步操作！");
			m_step--;
		}
	}
	else
	{
		int iCount = 0;
		for(int i = 0; i < m_paperlist.GetItemCount(); i++)
		{
			if(m_paperlist.GetCheck(i))
				iCount++;
		}
		if(iCount == 1)
		{
			m_up.EnableWindow(TRUE);
			m_next.EnableWindow(FALSE);

			m_search.ShowWindow(FALSE);
			m_chose.ShowWindow(FALSE);
			m_teachlist.ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC4)->ShowWindow(FALSE);

			m_query.ShowWindow(FALSE);
			m_show.ShowWindow(FALSE);
			m_paperlist.ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC3)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);

			m_fetch.ShowWindow(TRUE);
			m_save.ShowWindow(TRUE);
			m_answerlist.ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC2)->ShowWindow(TRUE);
		}
		else if(iCount == 0)
		{
			AfxMessageBox("您没有选择试卷，请选择试卷！");
			m_step--;
		}
		else
		{
			AfxMessageBox("您选择了多张试卷，请选择其中一张试卷！");
			m_step--;
		}
	}
}
