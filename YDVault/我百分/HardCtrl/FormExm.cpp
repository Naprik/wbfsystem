#include "stdafx.h"
#include "hardctrl.h"
#include "FormExm.h"
#include "MainFrm.h"
#include "DlgExmOption.h"
#include "YDCom.h"
#include "DlgPhoto.h"
#include "DBResult.h"
#include "softstat.h"
#include "ExamApp.h"
#include "Studev.h"
#include "DlgExamstat.h"
#include "DlgLate.h"
#include "DlgPerolling.h"
#include "Dlgsperoll.h"
#include "DlgLinkroll.h"
#include "DlgLinkrolling.h"
#include "DlgSingleroll.h"
#include "Stusearch.h"
#include "DlgPreRolling.h"
#include "BackUp.h"

#include "DlgExamset.h"

IMPLEMENT_DYNCREATE(CFormExm, CDialog)

extern CHardCtrlApp theApp;
extern CExamApp m_app;
extern CArray<CStudev, CStudev>  m_arryStud;
CFormExm* pDlg;

CFormExm::CFormExm(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormExm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormExm)
	m_systime = _T("");
	//}}AFX_DATA_INIT
}


void CFormExm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExm)
	DDX_Control(pDX, IDC_REMIND, m_remind);
	DDX_Control(pDX, IDC_SPEROLL, m_speroll);
	DDX_Control(pDX, IDC_LATE, m_late);
	DDX_Control(pDX, IDC_PREROLLING, m_prerolling);
	DDX_Control(pDX, IDC_LINKROLLING, m_linkrolling);
	DDX_Control(pDX, IDC_LINKROLL, m_linkroll);
	DDX_Control(pDX, IDC_DALL, m_dall);
	DDX_Control(pDX, IDC_ALL, m_all);
	DDX_Control(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_SETTING, m_setting);
	DDX_Control(pDX, IDC_START, m_start);
	DDX_Control(pDX, IDC_ROLLING, m_rolling);
	DDX_Control(pDX, IDC_ROLL, m_roll);
	DDX_Control(pDX, IDC_DEVLIST, m_DevList);
	DDX_Text(pDX, IDC_STATIC11, m_systime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExm, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormExm)
	ON_BN_CLICKED(IDC_ROLL, OnRoll)
	ON_BN_CLICKED(IDC_SETTING, OnSetting)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_ROLLING, OnRolling)
	ON_NOTIFY(NM_DBLCLK, IDC_DEVLIST, OnDblclkDevlist)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ALL, OnAll)
	ON_BN_CLICKED(IDC_DALL, OnDall)
	ON_BN_CLICKED(IDC_LATE, OnLate)
	ON_BN_CLICKED(IDC_PREROLLING, OnPrerolling)
	ON_BN_CLICKED(IDC_SPEROLL, OnSperoll)
	ON_BN_CLICKED(IDC_LINKROLL, OnLinkroll)
	ON_BN_CLICKED(IDC_LINKROLLING, OnLinkrolling)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CFormExm::OnInitDialog()
{
	cdxCSizingDialog::OnInitDialog();

/*	RECT rc;
	this->GetClientRect(&rc);

	m_flash.MoveWindow( &rc, true );

	CJahCoverCapture capture;
	capture.m_bmpFile =GetExePath()+"default.swf" ;
	CString strFileName = capture.m_bmpFile;
	m_flash.LoadMovie(0, strFileName);
	m_flash.SetScale("%100");   
	m_flash.Play();
	m_flash.ShowWindow(SW_SHOW);*/

	m_Static1.SubclassDlgItem(IDC_STATIC2,this);
	m_Static1.SetCaption("��Ŀ��");
	m_Static1.SetBackColor(RGB(0,0,0));

	m_Static2.SubclassDlgItem(IDC_STATIC4,this);
	m_Static2.SetCaption("�ο��༶��");
	m_Static2.SetBackColor(RGB(0,0,0));

	m_Static3.SubclassDlgItem(IDC_STATIC6,this);
	m_Static3.SetCaption("Ӧ��������");
	m_Static3.SetBackColor(RGB(0,0,0));

	m_Static4.SubclassDlgItem(IDC_STATIC12,this);
	m_Static4.SetCaption("����ʱ�䣺");
	m_Static4.SetBackColor(RGB(0,0,0));

	m_Static5.SubclassDlgItem(IDC_STATIC13,this);
	m_Static5.SetCaption("�վ�ʱ�䣺");
	m_Static5.SetBackColor(RGB(0,0,0));

	m_Static6.SubclassDlgItem(IDC_STATIC14,this);
	m_Static6.SetCaption("������");
	m_Static6.SetBackColor(RGB(0,0,0));

	m_Static7.SubclassDlgItem(IDC_STATIC1,this);
	m_Static7.SetCaption("����������Ϣ��");
	m_Static7.SetBackColor(RGB(0,0,0));

	m_search.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_search.SetIcon(IDI_CHOOSE);
	m_search.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_search.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));	
   
	m_roll.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_roll.SetIcon(IDI_ROLL);
	m_roll.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_roll.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));

	m_rolling.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_rolling.SetIcon(IDI_ICONING);
	m_rolling.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_rolling.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));

	m_late.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_late.SetIcon(IDI_LATE);
	m_late.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_late.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));

	m_prerolling.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_prerolling.SetIcon(IDI_LATE);
	m_prerolling.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_prerolling.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));

	m_linkroll.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_linkroll.SetIcon(IDI_LINKR);
	m_linkroll.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_linkroll.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));

	m_linkrolling.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_linkrolling.SetIcon(IDI_LINKR);
	m_linkrolling.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_linkrolling.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));

	m_speroll.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_speroll.SetIcon(IDI_SP);
	m_speroll.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_speroll.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));


	AddSzControl(*GetDlgItem(IDC_ROLL),mdNone,mdRepos);	               //���»��ƿؼ��ڴ��ڵ�λ�úʹ�С
	AddSzControl(*GetDlgItem(IDC_START),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_ROLLING),mdNone,mdRepos);

	AddSzControl(*GetDlgItem(IDC_SETTING),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_SEARCH),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_ALL),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_DALL),mdNone,mdRepos);

	AddSzControl(*GetDlgItem(IDC_LINKROLLING),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_LINKROLL),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_LATE),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_PREROLLING),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_SPEROLL),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_REMIND),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdResize,mdNone);
	
	AddSzControl(m_DevList,mdResize,mdResize);	

    DWORD dwStyle=GetWindowLong(m_DevList.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_DevList.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_DevList.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_DevList.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_DevList.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_DevList.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	
	m_DevList.ClearHeadings();

	m_DevList.SetHeadings("ѧ��,150;����,100;�༶,100;׼��֤��,100;��ǰ״̬,150;����ʱ��,150;����ʱ��,150;ѧ��������ַ,100");
		
	SetTimer(1,1,NULL);
	SetTimer(2,1000,NULL);
	SetTimer(3,1,NULL);

	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	m_systime.Format("%2d:%2d",sysTime.wHour,sysTime.wMinute);
	bIsRoll = FALSE;
	bIsRolling = FALSE;

	return TRUE;
}

/*UINT Fun(LPVOID param)
{
//	CFormExm* pDlg =(CFormExm*) p;
	COleDateTime t1;
	COleDateTimeSpan timeSpan; 
	int  span=0;
	while(true)
	{
		Sleep(1000);
		t1 = COleDateTime::GetCurrentTime();
		timeSpan = pDlg->m_endtime - t1;
		span = (int)timeSpan.GetTotalSeconds();
		if(span < 0)
			break;
	}
	AfxMessageBox("haha");
	return 0;
}*/
void CFormExm::OnSetting()               //���ÿ�����Ϣ
{
	CDlgExamset dlg;
	if(dlg.DoModal() == IDOK)
	{
		b_isSetexam = TRUE;
		m_papername = dlg.sPaper;
		m_subject = dlg.sSubject;
		m_depart = dlg.sDepart;
		m_class = dlg.sClass;
		m_address = dlg.sAddr;
		m_starttime.ParseDateTime(dlg.sStarttime);
		m_endtime.ParseDateTime(dlg.sEndtime);
		m_startno = atoi(dlg.sStartNO);
		m_endno = atoi(dlg.sEndNO);
		m_maxno = atoi(dlg.sMaxNO);
		m_paperid = dlg.paperid;
		CBackUp bk;
		bk.Remove();
		bk.Backup("�Ծ�����",m_papername);
		bk.Backup("��Ŀ",m_subject);
		bk.Backup("�ο�Ժϵ",m_depart);
		bk.Backup("�ο��༶",m_class);
		bk.Backup("����",m_address);
		bk.Backup("����ʱ��",m_starttime.Format());
		bk.Backup("�վ�ʱ��",m_endtime.Format());
		bk.Backup("��ʼ���",dlg.sStartNO);
		bk.Backup("�������",dlg.sEndNO);
		bk.Backup("������",dlg.sMaxNO);

		m_Static8.SubclassDlgItem(IDC_STATIC3,this);
		m_Static8.SetCaption(m_subject);
		m_Static8.SetTextColor(RGB(255,0,0));
		m_Static8.SetBackColor(RGB(0,0,0));
		
	//	GetDlgItem(IDC_STATIC3)->SetWindowText(m_subject);

//		GetDlgItem(IDC_STATIC10)->SetWindowText(m_address);
		m_Static11.SubclassDlgItem(IDC_STATIC10,this);
		m_Static11.SetCaption(m_address);
		m_Static11.SetTextColor(RGB(255,0,0));
		m_Static11.SetBackColor(RGB(0,0,0));

//		GetDlgItem(IDC_STATIC8)->SetWindowText(m_starttime.Format());
		m_Static12.SubclassDlgItem(IDC_STATIC8,this);
		m_Static12.SetCaption(m_starttime.Format());
		m_Static12.SetTextColor(RGB(255,0,0));
		m_Static12.SetBackColor(RGB(0,0,0));

		GetDlgItem(IDC_STATIC9)->SetWindowText(m_endtime.Format());
		m_Static13.SubclassDlgItem(IDC_STATIC9,this);
		m_Static13.SetCaption(m_endtime.Format());
		m_Static13.SetTextColor(RGB(255,0,0));
		m_Static13.SetBackColor(RGB(0,0,0));

		CString strclass = "";
		int temp1 = dlg.classarry.GetSize();
		m_classarry.RemoveAll();
		for(int i = 0; i < dlg.classarry.GetSize(); i++)
		{
			strclass += dlg.classarry[i] + " ";	
			m_classarry.Add(dlg.classarry[i]);
		}
	//	GetDlgItem(IDC_STATIC5)->SetWindowText(strclass);
		m_Static9.SubclassDlgItem(IDC_STATIC5,this);
		m_Static9.SetCaption(strclass);
		m_Static9.SetTextColor(RGB(255,0,0));
		m_Static9.SetBackColor(RGB(0,0,0));

// 		CoInitialize(NULL);
// 		CDBStuInfo m_StuInfodb;
// 		if(!m_StuInfodb.Connect())
// 		{
// 			AfxMessageBox("StuInfodb:�޷����ӵ����ݿ������");
// 		}
// 		else
// 		{
// 			m_DevList.DeleteAllItems();
// 			for(int j = 0; j < dlg.classarry.GetSize(); j++)
// 			{
// 				int flag = m_StuInfodb.LoadByClass(dlg.classarry[j]);
// 				while(flag)
// 				{
// 					m_DevList.AddItem(0,m_StuInfodb.m_stuid,m_StuInfodb.m_stuname,m_StuInfodb.m_Stuclass,"","","","",m_StuInfodb.m_devid);
// 					flag = m_StuInfodb.Move();
// 				}
// 
// 			}
// 			m_StuInfodb.Close();
// 		}
		m_account = m_DevList.GetItemCount();
		CString temp;
		temp.Format("%d ��", m_account);
	//	GetDlgItem(IDC_STATIC7)->SetWindowText(temp);
		m_Static10.SubclassDlgItem(IDC_STATIC7,this);
		m_Static10.SetCaption(temp);
		m_Static10.SetTextColor(RGB(255,0,0));
		m_Static10.SetBackColor(RGB(0,0,0));

		CoUninitialize();
		m_search.EnableWindow(true);
		m_start.EnableWindow(false);
		m_rolling.EnableWindow(false);
		m_roll.EnableWindow(false);
		m_all.EnableWindow(true);
		m_dall.EnableWindow(true);

	//	AfxBeginThread(Fun,(void*)pDlg,THREAD_PRIORITY_NORMAL);	
	}
	else
	{
		AfxMessageBox("û�����ÿ�����Ϣ�����޷��Կ��Խ��в�����");
		m_search.EnableWindow(false);
		m_start.EnableWindow(false);
		m_rolling.EnableWindow(false);
		m_roll.EnableWindow(false);
		m_all.EnableWindow(false);
		m_dall.EnableWindow(false);
		i_examMode = 0;
	}

}
/*ULONG SearchThread(LPVOID pParam,CProgressThreadDlg* pProgressDlg)
{
	pProgressDlg->m_flag = FALSE;

	CFormExm *dlg = (CFormExm *)pParam;
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CArray<CStusearch, CStusearch> arryvalid;
	arryvalid.RemoveAll();
	CStringArray  arryList;
	for(int i = 0; i < dlg->m_DevList.GetItemCount(); i++)
	{
		arryList.Add(dlg->m_DevList.GetItemText(i,0));
	}
	
	CDlgExamstat eaxmdlg;
	do
	{
		int t1 = arryList.GetSize();
		if(!m_app.Search(arryvalid, arryList))
			return 0;
		int t2 = arryvalid.GetSize();
		CString strMessage;
		strMessage.Format("����������Чѧ����%d̨��", arryvalid.GetSize());
		fram->m_dlgMsg.ShowMsg(strMessage);	
		
		for(i = 0; i < arryvalid.GetSize(); i++)
		{
			CString strId = arryvalid[i].m_id;
			for(int j = 0; j < dlg->m_DevList.GetItemCount(); j++)
			{
				CString strListid = dlg->m_DevList.GetItemText(j,0);
				strId.TrimRight();
				strListid.TrimRight();
				if(strListid == strId)
				{
					dlg->m_DevList.SetCheck(j);
					dlg->m_DevList.SetItemText(j,4,"��⵽��");
					break;
				}
			}
		}
		CString  strReality, atrOriginal;
		strReality.Format("%d ��", arryvalid.GetSize());
		eaxmdlg.m_reality = strReality;
		atrOriginal.Format("%d ��", dlg->m_account);
		eaxmdlg.m_original = atrOriginal;
	}while(eaxmdlg.DoModal() == IDOK);
	if(!m_app.Rest())
		AfxMessageBox("��������ʧ�ܣ�");
	dlg->m_roll.EnableWindow(true);
	dlg->m_rolling.EnableWindow(true);
	return 0;
}*/
void CFormExm::OnSearch()                   //����ѧ����
{	
/*	DWORD dwFlags=0;
	dwFlags |= PSTAT_CANCEL;
	dwFlags |= PSTAT_CONFIRMCANCEL;
    ExecuteFunctionWithProgressDialog(SearchThread, 
		                              _T("����ѧ����"),
	            					  this,
									  dwFlags, 
									  _T("���Ҫ�˳�������?"));*/

}

/*ULONG RollThread(LPVOID pParam,CProgressThreadDlg* pProgressDlg)   //��ѧ�������������Ϳ�����Ϣ��
{
	CFormExm *dlg = (CFormExm *)pParam;

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
			for(int j = 0; j < dlg->m_DevList.GetItemCount(); j++)
			{
				if(strID == dlg->m_DevList.GetItemText(j,0))
				{
					dlg->m_DevList.SetItemText(j,4,"���뿼��״̬��");
					COleDateTime tStart = m_app.Get_start_time(strID);
					dlg->m_DevList.SetItemText(j,5,tStart.Format());
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
}*/
void CFormExm::OnRoll()                                  //����
{
/*	CStringArray  arryvaild;
	for(int i = 0; i < m_DevList.GetItemCount(); i++)
	{
		if(m_DevList.GetCheck(i))
		{
			CString strID = m_DevList.GetItemText(i,0);
			strID.TrimRight();
			arryvaild.Add(strID);
		}
	}
	if(!m_app.Rollcall_all(arryvaild,m_starttime,m_endtime,m_maxno))
		return ;
	bIsRoll = TRUE;
	DWORD dwFlags=0;
	dwFlags |= PSTAT_CANCEL;
	dwFlags |= PSTAT_CONFIRMCANCEL;
    ExecuteFunctionWithProgressDialog(RollThread, 
		                              _T("����"),
	            					  this,
									  dwFlags, 
									  _T("���Ҫ�˳�������?"));

	m_search.EnableWindow(false);
	m_rolling.EnableWindow(true);
	m_roll.EnableWindow(false);
	
	bIsRolling = TRUE;*/
}

void CFormExm::OnStart()								// ȷ�ϵ�������
{

}
/*ULONG RollingThread(LPVOID pParam,CProgressThreadDlg* pProgressDlg)      // �վ�
{
	CFormExm *dlg = (CFormExm *)pParam;	
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
			for(int j = 0; j < dlg->m_DevList.GetItemCount(); j++)
			{
				if(strID == dlg->m_DevList.GetItemText(j,0))
				{
					dlg->m_DevList.SetItemText(j,4,"���վ�");
					COleDateTime tEnd = m_app.Get_end_time(strID);
					dlg->m_DevList.SetItemText(j,6,tEnd.Format());
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
	m_app.Get_answer(dlg->m_startno, dlg->m_endno, dlg->m_subject, dlg->m_paperid, dlg->m_address);
	return 0;
}*/

void CFormExm::OnRolling() 
{	
/*	if(!m_app.Rolling_all(m_startno, m_endno, m_endtime))
		return;
	bIsRolling = TRUE;
	bIsRoll = FALSE;
	DWORD dwFlags=0;
	dwFlags |= PSTAT_CANCEL;
	dwFlags |= PSTAT_CONFIRMCANCEL;
    ExecuteFunctionWithProgressDialog(RollingThread, 
		                              _T("�վ�"),
	            					  this,
									  dwFlags, 
									  _T("���Ҫ�˳�������?"));	*/
}

void CFormExm::SetKind(int kind)       //���ô������ͣ��������������վ��������
{
	m_kind = kind;
}
void CFormExm::Show()                 //���ݲ�ͬform������ʾ��Ӧ��ť
{
	switch(m_kind)     
	{
	case 0:
		m_roll.ShowWindow(SW_SHOW);
		m_rolling.ShowWindow(SW_HIDE);
		m_start.ShowWindow(SW_HIDE);
		m_setting.ShowWindow(SW_HIDE);
		m_search.ShowWindow(SW_SHOW);
		m_late.ShowWindow(SW_HIDE);
		m_prerolling.ShowWindow(SW_HIDE);
		m_linkrolling.ShowWindow(SW_HIDE);
		m_linkroll.ShowWindow(SW_HIDE);
		m_speroll.ShowWindow(SW_HIDE);
		m_dall.ShowWindow(SW_HIDE);
		m_all.ShowWindow(SW_HIDE);
		m_remind.EnableWindow(SW_HIDE);
		break;
	case 9:
		m_roll.ShowWindow(SW_HIDE);
		m_rolling.ShowWindow(SW_HIDE);
		m_start.ShowWindow(SW_HIDE);
		m_setting.ShowWindow(SW_HIDE);
		m_search.ShowWindow(SW_SHOW);
		m_late.ShowWindow(SW_HIDE);
		m_prerolling.ShowWindow(SW_HIDE);
		m_linkrolling.ShowWindow(SW_HIDE);
		m_linkroll.ShowWindow(SW_HIDE);
		m_speroll.ShowWindow(SW_HIDE);
		m_dall.ShowWindow(SW_HIDE);
		m_all.ShowWindow(SW_HIDE);
		m_remind.EnableWindow(SW_SHOW);
		break;
	case 10:
		m_roll.ShowWindow(SW_HIDE);
		m_rolling.ShowWindow(SW_SHOW);
		m_start.ShowWindow(SW_HIDE);
		m_setting.ShowWindow(SW_HIDE);
		m_search.ShowWindow(SW_HIDE);
		m_late.ShowWindow(SW_HIDE);
		m_prerolling.ShowWindow(SW_HIDE);
		m_linkrolling.ShowWindow(SW_HIDE);
		m_linkroll.ShowWindow(SW_HIDE);
		m_speroll.ShowWindow(SW_HIDE);
		m_dall.ShowWindow(SW_HIDE);
		m_all.ShowWindow(SW_HIDE);
		m_remind.EnableWindow(SW_HIDE);
		break;
	case 11:
		m_roll.ShowWindow(SW_HIDE);
		m_rolling.ShowWindow(SW_HIDE);
		m_start.ShowWindow(SW_HIDE);
		m_setting.ShowWindow(SW_HIDE);
		m_search.ShowWindow(SW_HIDE);
		m_late.ShowWindow(SW_SHOW);
		m_prerolling.ShowWindow(SW_SHOW);
		m_linkrolling.ShowWindow(SW_SHOW);
		m_linkroll.ShowWindow(SW_SHOW);
		m_speroll.ShowWindow(SW_SHOW);
		m_dall.ShowWindow(SW_HIDE);
		m_all.ShowWindow(SW_HIDE);
		m_remind.EnableWindow(SW_HIDE);
		break;
	default:
		break;
	}
}

ULONG SingleRollThread(LPVOID pParam,CProgressThreadDlg* pProgressDlg) 
{
	CFormExm *dlg = (CFormExm *)pParam;	
	pProgressDlg->m_flag = FALSE;
	BOOL falg = m_app.Rollcall_single(dlg->m_strTemp,dlg->m_starttime,dlg->m_endtime,dlg->m_maxno, FALSE);
	if(falg)
	{
		COleDateTime tStart = m_app.Get_start_time(dlg->m_strTemp);
		dlg->m_DevList.SetItemText(dlg->m_iTemp,5,tStart.Format());
		dlg->m_DevList.SetItemText(dlg->m_iTemp,4,"���뿼��״̬��");
	}
	else
		dlg->m_DevList.SetItemText(dlg->m_iTemp,4,"����ʧ�ܣ�");	
	return 0;
}
ULONG SingleRollingThread(LPVOID pParam,CProgressThreadDlg* pProgressDlg)
{
	pProgressDlg->m_flag = FALSE;
	CFormExm *dlg = (CFormExm *)pParam;

	int iRoll = 0;
	int iRolling = 0;
	COleDateTime t1, t2;
	t1 = COleDateTime::GetCurrentTime();
	COleDateTimeSpan timeSpan; 
	int  span=0;
	while(true)
	{
		Sleep(2000);
		t2 = COleDateTime::GetCurrentTime();
		timeSpan = t2 - t1;
		span = (int)timeSpan.GetTotalSeconds();
		if(m_app.Get_singlemac_stat(dlg->m_strTemp, iRoll, iRolling))
		{
			if(iRolling == 1 || iRolling == 3 || (span >= 16))
				break;
		}
	}
	Sleep(500);
	m_app.Rest();

	if(iRolling == 0)
		dlg->m_DevList.SetItemText(dlg->m_iTemp,4,"�վ�ʧ�ܣ�");
	if(iRolling == 1)
		dlg->m_DevList.SetItemText(dlg->m_iTemp,4,"�վ�ɹ���");
	if(iRolling == 2)
		dlg->m_DevList.SetItemText(dlg->m_iTemp,4,"�վ�ʧ�ܣ�");
	if(iRolling == 3)
		dlg->m_DevList.SetItemText(dlg->m_iTemp,4,"�վ�ɹ�(��ǰ����)��");
	return 0;
}

void CFormExm::OnDblclkDevlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item =pNMListView->iItem;
	m_iTemp = item;
	CString strID = m_DevList.GetItemText(item,0);
	strID.TrimLeft();
	strID.TrimRight();
	if(strID != "")
	{
		m_strTemp = strID;
// 		CoInitialize(NULL);
// 		CDBStuInfo m_StuInfodb;
 		CString path, id, name, sex, depart, tel, email;
// 		if ( !m_StuInfodb.Connect())
// 		{
// 			AfxMessageBox("StuInfodb:�޷����ӵ����ݿ������");
// 		}
// 		else
// 		{
// 			BOOL flag = m_StuInfodb.LoadByStuid(strID);
// 			if(flag)
// 			{
// 				path = m_StuInfodb.m_stuphoto;
// 				name = m_StuInfodb.m_stuname;
// 				id   = m_StuInfodb.m_stuid;
// 				sex  = m_StuInfodb.m_Stugender;
// 				depart = m_StuInfodb.m_Studepartment;
// 				tel = m_StuInfodb.m_stutel;
// 				email = m_StuInfodb.m_stuemail;
// 			}
// 			m_StuInfodb.Close();
// 		}
// 		CoUninitialize();

		if(m_kind == 9 || bIsRoll)
		{
			CDlgSingleroll dlg;
			dlg.m_path = path;
			dlg.m_id = id;
			dlg.m_name = name;
			dlg.m_gender = sex;
			dlg.m_depart = depart;
			dlg.m_suject = m_subject;
			dlg.m_addr = m_address;
			dlg.m_start = m_starttime.Format();
			dlg.m_end = m_endtime.Format();
			if(dlg.DoModal() == IDOK)
			{

				int iRoll,iRolling;
				if(m_DevList.GetItemText(item, 4) == "")
					AfxMessageBox("û����������ѧ�������޷�������");
				else
				{
					BOOL  flag = m_app.Get_singlemac_stat(id, iRoll,iRolling);
					if(flag && iRoll == 1)
					{
						AfxMessageBox("�ÿ����Ѿ�������");
					}
					else
					{

						DWORD dwFlags=0;
						dwFlags |= PSTAT_CANCEL;
						dwFlags |= PSTAT_CONFIRMCANCEL;
						ExecuteFunctionWithProgressDialog(SingleRollThread, 
														  _T("����"),
	            										  this,
														  dwFlags, 
														  _T("���Ҫ�˳�������?"));	
					}

				}
			}
		}
		else if(bIsRolling)
		{
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
			dlg.m_start = m_starttime.Format();
			if(dlg.DoModal() == IDOK)
			{
				if(!m_app.SingleRolling(id, currentStart))
					return;
				DWORD dwFlags=0;
				dwFlags |= PSTAT_CANCEL;
				dwFlags |= PSTAT_CONFIRMCANCEL;
				ExecuteFunctionWithProgressDialog(SingleRollingThread, 
											  _T("�վ�"),
	            							  this,
											  dwFlags, 
											  _T("���Ҫ�˳�������?"));	
			}
		}
		else
		{
			CDlgPhoto  dlg;
			dlg.m_path = path;
			dlg.m_id = id;
			dlg.m_name = name;
			dlg.m_gender = sex;
			dlg.m_depart = depart;
			dlg.m_tel = tel;
			dlg.m_email = email;

			//if(stunum != "")
			dlg.DoModal();
		}
	}
	*pResult = 0;
}

void CFormExm::OnTimer(UINT nIDEvent) 
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

void CFormExm::OnAll() 
{
	m_DevList.SelectAll();
}


void CFormExm::OnDall() 
{
	m_DevList.DeSelectAll();	
}



void CFormExm::OnLate() 
{
	CDlgLate  dlg;
	dlg.m_endtime = m_endtime;
	dlg.m_maxNo = m_maxno;
	dlg.m_subject = m_subject;
	dlg.m_address = m_address;
	for(int i = 0; i < m_DevList.GetItemCount(); i++)
	{
		dlg.m_list.Add(m_DevList.GetItemText(i,0));
	}	
	if(dlg.DoModal() == IDOK)
	{
		for(int i = 0; i < m_arryStud.GetSize(); i++)
		{
			if(m_arryStud[i].m_islate == 1)
			{
				for(int j = 0; j < m_DevList.GetItemCount(); j++)
				{
					if(m_arryStud[i].m_stuid == m_DevList.GetItemText(j,0))
					{
						m_DevList.SetItemText(j,4,"���뿼��״̬(�ٵ�)��");
						m_DevList.SetItemText(j,5,m_arryStud[i].m_start.Format());
						m_DevList.SetCheck(j);
						break;
					}
				}
			}
		}
	}
}

void CFormExm::OnPrerolling() 
{
	CDlgPerolling dlg;
	dlg.m_start = m_starttime;
	dlg.m_subject = m_subject;
	dlg.m_address = m_address;
	dlg.iPeperid = m_paperid;
	dlg.iStart = m_startno;
	dlg.iEnd = m_endno;
	dlg.strAdd = m_address;
	for(int i = 0; i < m_DevList.GetItemCount(); i++)
	{
		dlg.m_stulist.Add(m_DevList.GetItemText(i,0));
	}	
	if(	dlg.DoModal() == IDOK)
	{
		for(int i = 0; i < m_arryStud.GetSize(); i++)
		{
			if(m_arryStud[i].m_rolling == 3)
			{
				for(int j = 0; j < m_DevList.GetItemCount(); j++)
				{
					if(m_arryStud[i].m_stuid == m_DevList.GetItemText(j,0))
					{
						m_DevList.SetItemText(j,4,"���վ�(��ǰ����)��");
						m_DevList.SetItemText(j,6,m_arryStud[i].m_end.Format());
						m_DevList.SetCheck(j);
						break;
					}
				}
			}
		}
	}
	
}

void CFormExm::OnSperoll() 
{
	CDlgsperoll  dlg;
	dlg.DoModal();
	
}

void CFormExm::OnLinkroll() 
{
	CDlgLinkroll  dlg;
	dlg.classarry.RemoveAll();
	for(int i= 0; i < m_classarry.GetSize();i++)
	{
		dlg.classarry.Add(m_classarry[i]);
	}
	dlg.m_starttime = m_starttime;
	dlg.m_endtime = m_endtime;
	dlg.m_maxNO = m_maxno;
	if(dlg.DoModal() == IDOK)
	{
		for(i = 0; i < m_DevList.GetItemCount(); i++)
		{
			if(m_DevList.GetItemText(i,0) == dlg.sStuid)
			{
				m_DevList.SetItemText(i,5,dlg.sStartTime);
				m_DevList.SetItemText(i,4,dlg.sStat);
				break;
			}
		}
	}
	 
}

void CFormExm::OnLinkrolling() 
{
	CDlgLinkrolling dlg;
	dlg.iEnd = m_endno;
	dlg.iStart = m_startno;
	dlg.iPaper = m_paperid;
	dlg.m_starttime = m_starttime;
	dlg.sAddr = m_address;
	dlg.classarry.RemoveAll();
	if(dlg.DoModal() == IDOK)
	{
		for(int i = 0; i < m_DevList.GetItemCount(); i++)
		{
			if(m_DevList.GetItemText(i,0) == dlg.sStuid)
			{
				m_DevList.SetItemText(i,6,dlg.sEndTime);
				m_DevList.SetItemText(i,4,dlg.sStat);
				break;
			}
		}
	}
	
}
CString CFormExm::GetExePath()
{
	CString file;
	char process[MAX_PATH];
	GetModuleFileName(NULL, process, MAX_PATH);
    for ( int i = strlen(process);i>0 ;i --)
	if ( process[i] =='\\')
	{
		process[i+1]='\0';
		break;
	}
	file.Format(process);
	return file;
}

void CFormExm::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
    CJahCoverCapture capture;
	GetClientRect(&rc);
	capture.m_bmpFile =GetExePath()+"formback.bmp" ;
    if(!capture.LoadBMPImage(capture.m_bmpFile))
   		return;
    if (capture.m_bitmap.m_hObject == 0 )
    	return;
   capture.DrawBitmap(&dc,rc);
}
