// DlgLate.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgLate.h"
#include "ExamApp.h"
#include "Studev.h"
#include "DBStuInfo.h"
#include "DlgSingleroll.h"
#include "softstat.h"

extern CExamApp m_app;
extern CArray<CStudev, CStudev>  m_arryStud;

CDlgLate::CDlgLate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLate)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgLate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLate)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_LATESTART, m_latestart);
	DDX_Control(pDX, IDC_LATESEARCH, m_latesearch);
	DDX_Control(pDX, IDC_LATELIST, m_latelist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLate, CDialog)
	//{{AFX_MSG_MAP(CDlgLate)
	ON_BN_CLICKED(IDC_LATESEARCH, OnLatesearch)
	ON_BN_CLICKED(IDC_LATESTART, OnLatestart)
	ON_NOTIFY(NM_DBLCLK, IDC_LATELIST, OnDblclkLatelist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLate message handlers

BOOL CDlgLate::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwStyle=GetWindowLong(m_latelist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_latelist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_latelist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_latelist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_latelist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_latelist.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	
	m_latelist.ClearHeadings();
	m_latelist.SetHeadings("学号,80;姓名,100;班级,150;准考证号,100;当前状态,100");

	return true;
}
ULONG LateSearchThread(LPVOID pParam,CProgressThreadDlg* pProgressDlg)
{
	pProgressDlg->m_flag = FALSE;

	CDlgLate *dlg = (CDlgLate *)pParam;
	if(!m_app.Search(dlg->m_arryvalid, dlg->m_list))
		return 0;
	if(dlg->m_arryvalid.GetSize() == 0)
		return 0;
// 	CoInitialize(NULL);
// 	CDBStuInfo StuInfodb;
// 	if(!StuInfodb.Connect())
// 	{
// 		AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		BOOL flag = TRUE;
// 		for(int i = 0; i < dlg->m_arryvalid.GetSize(); i++)
// 		{
// 			CString strID = dlg->m_arryvalid[i].m_id;
// 			strID.TrimRight();
// 			for(int j = 0; j < m_arryStud.GetSize(); j++)
// 			{
// 				CString strTemp = m_arryStud[j].m_stuid;
// 				int iTemp = m_arryStud[j].m_rollcall;
// 				if(strID == m_arryStud[j].m_stuid && m_arryStud[j].m_rollcall == 1)
// 				{
// 					flag = FALSE;
// 				}
// 			}
// 			if(flag)
// 			{
// 				BOOL flag = StuInfodb.LoadByStuid(dlg->m_arryvalid[i].m_id);
// 				if(flag)
// 				{
// 					dlg->m_latelist.AddItem(0, StuInfodb.m_stuid,StuInfodb.m_stuname,StuInfodb.m_Stuclass,"","检测到（迟到）！");
// 					dlg->m_latelist.SetCheck(i);
// 				}
// 			}
// 			flag = TRUE;
// 		}
// 
// 		StuInfodb.Close();
// 	}
// 	CoUninitialize();

	return 0;
}
void CDlgLate::OnLatesearch() 
{
	m_arryvalid.RemoveAll();
	m_latelist.DeleteAllItems();

	DWORD dwFlags=0;
	dwFlags |= PSTAT_CANCEL;
	dwFlags |= PSTAT_CONFIRMCANCEL;
    ExecuteFunctionWithProgressDialog(LateSearchThread, 
		                              _T("搜索学生机"),
	            					  this,
									  dwFlags, 
									  _T("真的要退出检索吗?"));	
}

void CDlgLate::OnLatestart() 
{

}
ULONG LateStartThread(LPVOID pParam,CProgressThreadDlg* pProgressDlg)
{
	pProgressDlg->m_flag = FALSE;

	CDlgLate *dlg = (CDlgLate *)pParam;
	BOOL falg = m_app.Rollcall_single(dlg->m_strTemp,dlg->m_start,dlg->m_endtime, dlg->m_maxNo, TRUE);

	if(falg)
	{
		dlg->m_latelist.SetItemText(dlg->m_iTemp,4,"进入考试状态(迟到)！");
	}
	else
		dlg->m_latelist.SetItemText(dlg->m_iTemp,4,"开考失败！");


	return 0;
}
void CDlgLate::OnDblclkLatelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item =pNMListView->iItem;
	m_iTemp = item;
	CString stunum = m_latelist.GetItemText(item,0);
	stunum.TrimLeft();
	stunum.TrimRight();
	if(stunum != "")
	{
		m_strTemp = stunum;
// 		CoInitialize(NULL);
// 		CDBStuInfo m_StuInfodb;
 		CString path, id, name, sex, depart, tel, email;
// 		if ( !m_StuInfodb.Connect())
// 		{
// 			AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 			CoUninitialize();
// 			return;
// 		}
// 		else
// 		{
// 			BOOL flag = m_StuInfodb.LoadByStuid(stunum);
// 			if(flag)
// 			{
// 				path = m_StuInfodb.m_stuphoto;
// 				name = m_StuInfodb.m_stuname;
// 				id   = m_StuInfodb.m_stuid;
// 				sex  = m_StuInfodb.m_Stugender;
// 				depart = m_StuInfodb.m_Stuclass;
// 				tel = m_StuInfodb.m_stutel;
// 				email = m_StuInfodb.m_stuemail;
// 			}
// 			m_StuInfodb.Close();	
// 		}
// 		CoUninitialize();

		CDlgSingleroll dlg;
		dlg.m_path = path;
		dlg.m_id = id;
		dlg.m_name = name;
		dlg.m_gender = sex;
		dlg.m_depart = depart;
		dlg.m_suject = m_subject;
		dlg.m_addr = m_address;
		COleDateTime currentStart;
		currentStart = COleDateTime::GetCurrentTime();		
		dlg.m_start = currentStart.Format();
		dlg.m_end = m_endtime.Format();
		if(dlg.DoModal() == IDOK)
		{
			m_start = currentStart;
			int iRoll,iRolling;
			if(m_latelist.GetItemText(item, 4) == "")
				AfxMessageBox("没有搜索到该学生机，无法开考！");
			else
			{
				BOOL flag = m_app.Get_singlemac_stat(id, iRoll,iRolling);
				if(flag && iRoll == 1)
					AfxMessageBox("该考考生已经开考！");
				else
				{
					DWORD dwFlags=0;
					dwFlags |= PSTAT_CANCEL;
					dwFlags |= PSTAT_CONFIRMCANCEL;
					ExecuteFunctionWithProgressDialog(LateStartThread, 
												  _T("迟到开考"),
	            								  this,
												  dwFlags, 
												  _T("真的要退出检索吗?"));	
				}
			}
		}

	}

/*	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item =pNMListView->iItem;
	m_iTemp = item;
	CString stunum = m_latelist.GetItemText(item,0);
	if(stunum != "")
	{
		m_strTemp = stunum;
		CString strMac;
		CoInitialize(NULL);
		CDBDev Devdb;
		if ( !Devdb.Connect() )
		{
			AfxMessageBox("Devdb:无法连接到数据库服务器");
			return;
		}
		else
		{
			BOOL flag = Devdb.LoadByStuid(stunum);
			if(flag)
			{
				strMac = Devdb.m_devid;
			}
			Devdb.Close();
		}

		CDBStuInfo StuInfodb;
		CString path, id, name, sex, depart, tel, email;
		if ( !StuInfodb.Connect())
		{
			AfxMessageBox("StuInfodb:无法连接到数据库服务器");
			CoUninitialize();
			return;
		}
		else
		{
			BOOL flag = StuInfodb.LoadByStuid(stunum);
			if(flag)
			{
				path = StuInfodb.m_stuphoto;
				name = StuInfodb.m_stuname;
				id   = StuInfodb.m_stuid;
				sex  = StuInfodb.m_Stugender;
				depart = StuInfodb.m_Studepartment;
				tel = StuInfodb.m_stutel;
				email = StuInfodb.m_stuemail;
			}
			StuInfodb.Close();
		}
		CoUninitialize();

		CDlgSingleroll dlg;
		dlg.m_path = path;
		dlg.m_id = id;
		dlg.m_name = name;
		dlg.m_gender = sex;
		dlg.m_depart = depart;
		dlg.m_suject = m_subject;
		dlg.m_addr = m_address;
		COleDateTime currentStart;
		currentStart = COleDateTime::GetCurrentTime();		
		dlg.m_start = currentStart.Format();
		dlg.m_end = m_endtime.Format();
		if(dlg.DoModal() == IDOK)
		{
			m_start = currentStart;
			int iRoll,iRolling;
			if(m_latelist.GetItemText(item, 4) == "")
				AfxMessageBox("没有搜索到该学生机，无法开考！");
			else
			{
				BOOL flag = m_app.Get_singlemac_stat(id, iRoll,iRolling);
				if(flag && iRoll == 1)
					AfxMessageBox("该考考生已经开考！");
				else
				{
					DWORD dwFlags=0;
					dwFlags |= PSTAT_CANCEL;
					dwFlags |= PSTAT_CONFIRMCANCEL;
					ExecuteFunctionWithProgressDialog(LateStartThread, 
												  _T("迟到开考"),
	            								  this,
												  dwFlags, 
												  _T("真的要退出检索吗?"));	
				}
			}
		}
	}*/
}

void CDlgLate::OnOK() 
{
	

	CDialog::OnOK();
}
