#include "stdafx.h"
#include "hardctrl.h"
#include "FormPCoffRoll.h"
#include "MainFrm.h"
#include "DlgClassQuery.h"
#include "DlgPaperQuery.h"
#include "DlgPaperset.h"
#include "TeacherDev.h"
#include "ExamApp.h"
#include "Studev.h"

extern CHardCtrlApp theApp;
extern CArray<CTeacherDev, CTeacherDev>  arryTeacherdev;
extern CExamApp m_app;

IMPLEMENT_DYNCREATE(CFormPCoffRoll, CDialog)


CFormPCoffRoll::CFormPCoffRoll(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormPCoffRoll::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormPCoffRoll)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormPCoffRoll::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormPCoffRoll)
	DDX_Control(pDX, IDC_NEXT, m_next);
	DDX_Control(pDX, IDC_BACK, m_back);
	DDX_Control(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_CHOOSE, m_choose);
	DDX_Control(pDX, IDC_TEACHLIST, m_teachlist);
	DDX_Control(pDX, IDC_CLASSQUERY, m_classquery);
	DDX_Control(pDX, IDC_SHOWALL, m_showall);
	DDX_Control(pDX, IDC_PAPERQUERY, m_paperquery);
	DDX_Control(pDX, IDC_PAPERLIST, m_paperlist);
	DDX_Control(pDX, IDC_OK, m_ok);
	DDX_Control(pDX, IDC_CLASSLIST, m_classlist);
	DDX_Control(pDX, IDC_ADD, m_add);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormPCoffRoll, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormPCoffRoll)
	ON_BN_CLICKED(IDC_CLASSQUERY, OnClassquery)
	ON_BN_CLICKED(IDC_SHOWALL, OnShowall)
	ON_BN_CLICKED(IDC_PAPERQUERY, OnPaperquery)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_BN_CLICKED(IDC_CHOOSE, OnChoose)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormPCoffRoll message handlers

BOOL CFormPCoffRoll::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();

	AddSzControl(*GetDlgItem(IDC_STATIC1),mdResize,mdResize);
	AddSzControl(*GetDlgItem(IDC_STATIC2),mdResize,mdResize);
	AddSzControl(*GetDlgItem(IDC_STATIC5),mdResize,mdResize);
	AddSzControl(*GetDlgItem(IDC_SHOWALL),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_PAPERQUERY),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_ADD),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_BACK),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_NEXT),mdNone,mdRepos);
	
	AddSzControl(*GetDlgItem(IDC_SEARCH),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_CHOOSE),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_OK),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_CLASSQUERY ),mdNone,mdRepos);
	
	AddSzControl(m_paperlist,mdResize,mdResize);
	AddSzControl(m_classlist,mdResize,mdResize);
	AddSzControl(m_teachlist,mdResize,mdResize);

	DWORD dwStyle=GetWindowLong(m_paperlist.GetSafeHwnd(),GWL_STYLE);
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
	m_paperlist.SetHeadings("��������,100;���Կ�Ŀ,100; �ο�Ժϵ,100; �ο��༶,100;����ʱ��,150;�վ��Ծ�,150;��ʼ���,100;�������,100;������,100"); 	

	dwStyle=GetWindowLong(m_classlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_classlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_classlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_classlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_classlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_classlist.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE);

	m_classlist.ClearHeadings();
	m_classlist.SetHeadings("ѧ��,100; ����,100; �Ա�,50; Ժϵ,150; �༶,100; �绰,100; ��������,150; ѧ�����豸��,100");
	

	dwStyle=GetWindowLong(m_teachlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_teachlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_teachlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_teachlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_teachlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_teachlist.SetExtendedStyle(dwStyle);

	m_teachlist.SetHeadings("��ʦ�����, 80; ʹ����,150;ʹ�ö˿�,100;��״̬, 150");	

	m_step = 1;
	m_back.EnableWindow(FALSE);
	m_next.EnableWindow(TRUE);
	m_search.ShowWindow(TRUE);
	m_choose.ShowWindow(TRUE);
	m_teachlist.ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC5)->ShowWindow(TRUE);

	m_showall.ShowWindow(FALSE);
	m_paperquery.ShowWindow(FALSE);
	m_add.ShowWindow(FALSE);
	m_paperlist.ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC4)->ShowWindow(FALSE);

	m_classquery.ShowWindow(FALSE);
	m_ok.ShowWindow(FALSE);
	m_classlist.ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC3)->ShowWindow(FALSE);

	b_isOpencom = FALSE;

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFormPCoffRoll::OnClassquery() 
{
	CDlgClassQuery  dlg;
	if(dlg.DoModal() == IDOK)
	{
// 		CoInitialize(NULL);
// 		CDBStuInfo m_StuInfodb;
// 		if(!m_StuInfodb.Connect())
// 		{
// 			AfxMessageBox("StuInfodb:�޷����ӵ����ݿ������");
// 		}
// 		else
// 		{
// 			m_classlist.DeleteAllItems();
// 			for(int j = 0; j < dlg.classarry.GetSize(); j++)
// 			{
// 				int flag = m_StuInfodb.LoadByClass(dlg.classarry[j]);
// 				while(flag)
// 				{
// 					m_classlist.AddItem(0,m_StuInfodb.m_stuid,m_StuInfodb.m_stuname,m_StuInfodb.m_Stugender,m_StuInfodb.m_Studepartment,m_StuInfodb.m_Stuclass,m_StuInfodb.m_stutel,m_StuInfodb.m_stuemail,m_StuInfodb.m_devid);
// 					flag = m_StuInfodb.Move();
// 				}
// 
// 			}
// 			m_StuInfodb.Close();
// 		}
// 		CoUninitialize();
	}
//	for(int i = 0; i < m_classlist.GetItemCount(); i++)
//	{
//		m_classlist.SetCheck(i);
//	}
	
}

void CFormPCoffRoll::OnShowall() 
{
	m_paperlist.DeleteAllItems();
// 	CoInitialize(NULL);
// 	CDBPaper  m_Paperdb;
// 	if ( !m_Paperdb.Connect())
// 	{
// 		AfxMessageBox("Paperdb: �޷����ӵ����ݿ������");
// 	}	
// 	else
// 	{
// 		BOOL flag = m_Paperdb.LoadByAll();
// 		while(flag)
// 		{
// 			CString tep1,tep2,tep3;
// 			tep1.Format("%d",m_Paperdb.m_startNO);
// 			tep2.Format("%d",m_Paperdb.m_endNO);
// 			tep3.Format("%d",m_Paperdb.m_maxNO);
// 			m_paperlist.AddItem(0,m_Paperdb.m_papername,
// 							m_Paperdb.m_subject,
// 							m_Paperdb.m_depart,
// 							m_Paperdb.m_class,
// 							m_Paperdb.m_starttime.Format(),
// 							m_Paperdb.m_endtime.Format(),
// 							tep1,tep2,tep3);
// 			flag = m_Paperdb.Move();
// 		}
// 		m_Paperdb.Close();
// 	}
// 	CoUninitialize();	
}

void CFormPCoffRoll::OnPaperquery() 
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
// 			AfxMessageBox("Paperdb: �޷����ӵ����ݿ������");
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
// 				CString tep1,tep2,tep3;
// 				tep1.Format("%d",m_Paperdb.m_startNO);
// 				tep2.Format("%d",m_Paperdb.m_endNO);
// 				tep3.Format("%d",m_Paperdb.m_maxNO);
// 				m_paperlist.AddItem(0,m_Paperdb.m_papername,
// 								m_Paperdb.m_subject,
// 								m_Paperdb.m_depart,
// 								m_Paperdb.m_class,
// 								m_Paperdb.m_starttime.Format(),
// 								m_Paperdb.m_endtime.Format(),
// 								tep1,tep2,tep3);
// 				flag = m_Paperdb.Move();
// 			}
// 			m_Paperdb.Close();
// 		}
// 		CoUninitialize();
	}	
}

void CFormPCoffRoll::OnAdd() 
{
	CDlgPaperset dlg;
	if(dlg.DoModal() == IDOK)
	{
// 		CoInitialize(NULL);
// 		CDBPaper       m_Paperdb;
// 		if ( !m_Paperdb.Connect())
// 		{
// 			AfxMessageBox("Paperdb: �޷����ӵ����ݿ������");
// 		}
// 		else
// 		{
// 			m_Paperdb.m_ydid = 0;
// 			m_Paperdb.m_papername = dlg.strPaper;
// 			m_Paperdb.m_subject = dlg.strSubject;
// 			m_Paperdb.m_depart = dlg.strDepart;
// 			m_Paperdb.m_class = dlg.strClass;
// 			m_Paperdb.m_starttime = dlg.tStart;
// 			m_Paperdb.m_endtime = dlg.tEnd;
// 			m_Paperdb.m_startNO = dlg.m_startNO;
// 			m_Paperdb.m_endNO = dlg.m_endNO;
// 			m_Paperdb.m_maxNO = dlg.m_maxNO;
// 			if(m_Paperdb.Add())
// 			{
// 				AfxMessageBox("��ӳɹ���");
// 				CString tep1,tep2,tep3;
// 				tep1.Format("%d",dlg.m_startNO);
// 				tep2.Format("%d",dlg.m_endNO);
// 				tep3.Format("%d",dlg.m_maxNO);
// 				m_paperlist.AddItem(0,dlg.strPaper,
// 								dlg.strSubject,
// 								dlg.strDepart,
// 								dlg.strClass,
// 								dlg.tStart.Format(),
// 								dlg.tEnd.Format(),
// 								tep1,tep2,tep3);
// 			}
// 			else
// 				AfxMessageBox("���ʧ�ܣ�");
// 	
// 			m_Paperdb.Close();
// 		}
// 		CoUninitialize();
	}	
}

void CFormPCoffRoll::OnSearch() 
{
	m_teachlist.DeleteAllItems();	
	if(m_app.TestStu())
	{
		int i = arryTeacherdev.GetSize();
		if(i <= 0)
			AfxMessageBox("û����������ʦ����");
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
// 				AfxMessageBox("TeacherDev �޷����ӵ����ݿ������");
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
// 						m_teachlist.AddItem(0, m_TeacherDevdb.m_devid, m_TeacherDevdb.m_teachername, strCom ,"��⵽����ʹ��");
// 						m_teachlist.SetCheck(i);
// 					}
// 				}
// 				m_TeacherDevdb.Close();
// 			}
// 			CoUninitialize();
		}
	}
	else
		AfxMessageBox("û����������ʦ����");
	
}

void CFormPCoffRoll::OnChoose() 
{
	if(m_teachlist.GetItemCount() == 0)
	{
		AfxMessageBox("û�н�ʦ������ѡ�����ȵ������������ť��������ʦ����");
		return;
	}
	if(m_app.OpenStucom())
	{
		AfxMessageBox("�򿪳ɹ���");
		b_isOpencom = TRUE;
	}
	else
	{
		AfxMessageBox("��ʧ�ܣ�");
		b_isOpencom = FALSE;
	}
}

void CFormPCoffRoll::OnOk() 
{
	BOOL flag = FALSE;
	if(b_isOpencom == FALSE)
	{
		AfxMessageBox("��ʦ���˿��ѹرգ��뷵����һ���򿪽�ʦ����");
		return;
	}

	for(int i = 0; i < m_paperlist.GetItemCount(); i++)
	{
		if(m_paperlist.GetCheck(i))
		{
			CString tStart = m_paperlist.GetItemText(i,4);
			CString tEnd = m_paperlist.GetItemText(i,5);
			int iStartNO = atoi(m_paperlist.GetItemText(i,6));
			int iEndNO = atoi(m_paperlist.GetItemText(i,7));
			int iMaxNO = atoi(m_paperlist.GetItemText(i,8));
			COleDateTime 	m_starttime;
			COleDateTime 	m_endtime;
			m_starttime.ParseDateTime(tStart);
			m_endtime.ParseDateTime(tEnd);
			if(!m_app.Offexaminfo(m_starttime,m_endtime,iMaxNO,iStartNO,iEndNO))
			{
				AfxMessageBox("���Ϳ�����Ϣʧ�ܣ�");
				flag = FALSE;
			}
			else
				flag = TRUE;
			break;
		}
	}
	Sleep(500);
	CArray<CStudev, CStudev>  stuarry;
	stuarry.RemoveAll();
	int index = 0;
	for(i = 0; i < m_classlist.GetItemCount();i++)
	{
		if(m_classlist.GetCheck(i))
		{
			CStudev sd;
			sd.m_index = index;
			memset(sd.m_mac,0,sizeof(sd.m_mac));
			strcpy(sd.m_mac,m_classlist.GetItemText(i,7));
			stuarry.Add(sd);
			index++;
		}
	}
	if(!m_app.Offsendmac(stuarry))
	{
		AfxMessageBox("����ѧ����Ϣʧ�ܣ�");
		flag = FALSE;
	}
	if(flag)
	{
		AfxMessageBox("��Ϣ��������ʦ���ɹ���");
		m_app.CloseStucom();
		b_isOpencom = FALSE;

	}
	else
	{
		AfxMessageBox("��Ϣ��������ʦ��ʧ�ܣ�");
	}
}

void CFormPCoffRoll::OnBack() 
{
	m_step--;
	if(m_step == 1)
	{
		m_back.EnableWindow(FALSE);
		m_next.EnableWindow(TRUE);

		m_search.ShowWindow(TRUE);
		m_choose.ShowWindow(TRUE);
		m_teachlist.ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC5)->ShowWindow(TRUE);

		m_showall.ShowWindow(FALSE);
		m_paperquery.ShowWindow(FALSE);
		m_add.ShowWindow(FALSE);
		m_paperlist.ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(FALSE);

		m_classquery.ShowWindow(FALSE);
		m_ok.ShowWindow(FALSE);
		m_classlist.ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC3)->ShowWindow(FALSE);
	}
	else if(m_step == 2)
	{
		m_back.EnableWindow(TRUE);
		m_next.EnableWindow(TRUE);

		m_search.ShowWindow(FALSE);
		m_choose.ShowWindow(FALSE);
		m_teachlist.ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC5)->ShowWindow(FALSE);

		m_showall.ShowWindow(TRUE);
		m_paperquery.ShowWindow(TRUE);
		m_add.ShowWindow(TRUE);
		m_paperlist.ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC1)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(TRUE);

		m_classquery.ShowWindow(FALSE);
		m_ok.ShowWindow(FALSE);
		m_classlist.ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC3)->ShowWindow(FALSE);
	}
	else
	{
		m_back.EnableWindow(TRUE);
		m_next.EnableWindow(FALSE);

		m_search.ShowWindow(FALSE);
		m_choose.ShowWindow(FALSE);
		m_teachlist.ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC5)->ShowWindow(FALSE);

		m_showall.ShowWindow(FALSE);
		m_paperquery.ShowWindow(FALSE);
		m_add.ShowWindow(FALSE);
		m_paperlist.ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(FALSE);


		m_classquery.ShowWindow(TRUE);
		m_ok.ShowWindow(TRUE);
		m_classlist.ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
	}
	
}

void CFormPCoffRoll::OnNext() 
{
	m_step++;
	if(m_step == 1)
	{
		if(b_isOpencom == TRUE)
		{
			m_back.EnableWindow(FALSE);
			m_next.EnableWindow(TRUE);

			m_search.ShowWindow(TRUE);
			m_choose.ShowWindow(TRUE);
			m_teachlist.ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC5)->ShowWindow(TRUE);

			m_showall.ShowWindow(FALSE);
			m_paperquery.ShowWindow(FALSE);
			m_add.ShowWindow(FALSE);
			m_paperlist.ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC4)->ShowWindow(FALSE);

			m_classquery.ShowWindow(FALSE);
			m_ok.ShowWindow(FALSE);
			m_classlist.ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC3)->ShowWindow(FALSE);
		}
		else
		{
			AfxMessageBox("û��ѡ���ʦ���޷�������һ��������");
		}
	}
	else if(m_step == 2)
	{
		if(b_isOpencom == TRUE)
		{
			m_back.EnableWindow(TRUE);
			m_next.EnableWindow(TRUE);

			m_search.ShowWindow(FALSE);
			m_choose.ShowWindow(FALSE);
			m_teachlist.ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC5)->ShowWindow(FALSE);

			m_showall.ShowWindow(TRUE);
			m_paperquery.ShowWindow(TRUE);
			m_add.ShowWindow(TRUE);
			m_paperlist.ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC1)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC4)->ShowWindow(TRUE);

			m_classquery.ShowWindow(FALSE);
			m_ok.ShowWindow(FALSE);
			m_classlist.ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC2)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC3)->ShowWindow(FALSE);
		}
		else
		{
			AfxMessageBox("û��ѡ���ʦ���޷�������һ��������");
			m_step--;
		}
	}
	else
	{
		int iCount = 0;
		for(int i = 0; i < m_paperlist.GetItemCount(); i++)
		{
			if(m_paperlist.GetCheck(i))
			{
				iCount++;
			}
		}
		if(iCount == 1)
		{
			m_back.EnableWindow(TRUE);
			m_next.EnableWindow(FALSE);

			m_search.ShowWindow(FALSE);
			m_choose.ShowWindow(FALSE);
			m_teachlist.ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC5)->ShowWindow(FALSE);

			m_showall.ShowWindow(FALSE);
			m_paperquery.ShowWindow(FALSE);
			m_add.ShowWindow(FALSE);
			m_paperlist.ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);
			GetDlgItem(IDC_STATIC4)->ShowWindow(FALSE);


			m_classquery.ShowWindow(TRUE);
			m_ok.ShowWindow(TRUE);
			m_classlist.ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC2)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
		}
		else if(iCount == 0)
		{
			AfxMessageBox("��û��ѡ���Ծ���ѡ���Ծ�");
			m_step--;
		}
		else
		{
			AfxMessageBox("��ѡ���˶����Ծ���ѡ������һ���Ծ�");
			m_step--;
		}

	}
	
}
