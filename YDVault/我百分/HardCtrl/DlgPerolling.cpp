// DlgPerolling.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgPerolling.h"
#include "Studev.h"
#include "DBStuInfo.h"
#include "ExamApp.h"
#include "DlgPreRolling.h"
#include "softstat.h"

extern CExamApp m_app;

extern CArray<CStudev, CStudev>  arryStudev;

CDlgPerolling::CDlgPerolling(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPerolling::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPerolling)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgPerolling::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPerolling)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPerolling, CDialog)
	//{{AFX_MSG_MAP(CDlgPerolling)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPerolling message handlers
BOOL CDlgPerolling::OnInitDialog()
{
	CDialog::OnInitDialog();
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
	m_list.SetHeadings("学号,80;姓名,100;班级,150;准考证号,100;当前状态,100");
	 return TRUE;
}
ULONG PreSearchThread(LPVOID pParam,CProgressThreadDlg* pProgressDlg)
{
	pProgressDlg->m_flag = FALSE;
	CDlgPerolling *dlg = (CDlgPerolling *)pParam;

	if(!m_app.Search(dlg->m_arryvalid, dlg->m_stulist))
		return 0;
// 	CoInitialize(NULL);
// 	CDBStuInfo StuInfodb;
// 	if(!StuInfodb.Connect())
// 	{
// 		AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		for(int i = 0; i < dlg->m_arryvalid.GetSize(); i++)
// 		{
// 			BOOL flag = StuInfodb.LoadByStuid(dlg->m_arryvalid[i].m_id);
// 			if(flag)
// 			{
// 				if(m_app.Ismac_in(dlg->m_arryvalid[i].m_id))
// 				{
// 					int iRoll,iRolling;
// 					m_app.Get_singlemac_stat(dlg->m_arryvalid[i].m_id, iRoll, iRolling);
// 					if(iRoll == 1 && (iRolling == 0 || iRolling == 2))
// 					{
// 						dlg->m_list.AddItem(0, StuInfodb.m_stuid,StuInfodb.m_stuname,StuInfodb.m_Stuclass,"","检测到！");
// 						dlg->m_list.SetCheck(i);
// 					}
// 				}
// 			}
// 		}
// 		StuInfodb.Close();
// 	}
// 	CoUninitialize();
	return 0;
}

void CDlgPerolling::OnSearch() 
{
	m_arryvalid.RemoveAll();
	m_list.DeleteAllItems();

	DWORD dwFlags=0;
	dwFlags |= PSTAT_CANCEL;
	dwFlags |= PSTAT_CONFIRMCANCEL;
    ExecuteFunctionWithProgressDialog(PreSearchThread, 
		                              _T("搜索学生机"),
	            					  this,
									  dwFlags, 
									  _T("真的要退出检索吗?"));		
}
ULONG PreRollingThread(LPVOID pParam,CProgressThreadDlg* pProgressDlg)
{
	pProgressDlg->m_flag = FALSE;
	CDlgPerolling *dlg = (CDlgPerolling *)pParam;

	int iRoll,iRolling;
	for(int i = 0; i < 2; i++)
	{
		Sleep(8000);
		if(m_app.Get_singlemac_stat(dlg->m_strTemp, iRoll, iRolling))
		{
			if(iRolling == 2 || iRolling == 3)
				break;
		}
	}
	if(iRolling == 0)
		dlg->m_list.SetItemText(dlg->m_iTemp,4,"收卷失败！");
	if(iRolling == 1)
		dlg->m_list.SetItemText(dlg->m_iTemp,4,"收卷成功！");
	if(iRolling == 2)
		dlg->m_list.SetItemText(dlg->m_iTemp,4,"收卷失败！");
	if(iRolling == 3)
	{
		m_app.GetSingleAnswer(dlg->m_strTemp,dlg->iStart,dlg->iEnd,dlg->iPeperid,dlg->strAdd);
		dlg->m_list.SetItemText(dlg->m_iTemp,4,"收卷成功(提前交卷)！");
	}
	return 0;

}
void CDlgPerolling::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item =pNMListView->iItem;
	m_iTemp = item;
	CString stunum = m_list.GetItemText(item,0);
	stunum.TrimLeft();
	stunum.TrimRight();
	if(stunum != "")
	{
		m_strTemp = stunum;
// 		CoInitialize(NULL);
// 		CDBStuInfo StuInfodb;
 		CString path, id, name, sex, depart, tel, email;
// 		if ( !StuInfodb.Connect())
// 		{
// 			AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 			CoUninitialize();
// 			return;
// 		}
// 		else
// 		{
// 			BOOL flag = StuInfodb.LoadByStuid(stunum);
// 			if(flag)
// 			{
// 				path = StuInfodb.m_stuphoto;
// 				name = StuInfodb.m_stuname;
// 				id   = StuInfodb.m_stuid;
// 				sex  = StuInfodb.m_Stugender;
// 				depart = StuInfodb.m_Stuclass;
// 				tel = StuInfodb.m_stutel;
// 				email = StuInfodb.m_stuemail;
// 			}
// 			StuInfodb.Close();
// 		}
// 		CoUninitialize();

		CDlgPreRolling dlg;
		dlg.m_path = path;
		dlg.m_id = id;
		dlg.m_name = name;
		dlg.m_gender = sex;
		dlg.m_depart = depart;
		dlg.m_suject = m_subject;
		dlg.m_addr = m_address;
		COleDateTime currentStart;
		currentStart = COleDateTime::GetCurrentTime();
		dlg.m_end = currentStart.Format();
		dlg.m_start = m_start.Format();
		if(dlg.DoModal() == IDOK)
		{
			if(!m_app.Pre_Rolling(id, currentStart))
				return;
			DWORD dwFlags=0;
			dwFlags |= PSTAT_CANCEL;
			dwFlags |= PSTAT_CONFIRMCANCEL;
			ExecuteFunctionWithProgressDialog(PreRollingThread, 
										  _T("提前交卷"),
	            						  this,
										  dwFlags, 
										  _T("真的要退出检索吗?"));	
		}
	}
	*pResult = 0;
}
