// DlgExamset.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgExamset.h"
#include "DlgPaperQuery.h"
#include "DlgPaperset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExamset dialog


CDlgExamset::CDlgExamset(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExamset::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExamset)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgExamset::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExamset)
	DDX_Control(pDX, IDC_CLASSLIST, m_classlist);
	DDX_Control(pDX, IDC_ADDR, m_address);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_SHOWALL, m_showall);
	DDX_Control(pDX, IDC_QUERYPAPER, m_query);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_ADDPAPER, m_add);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExamset, CDialog)
	//{{AFX_MSG_MAP(CDlgExamset)
	ON_BN_CLICKED(IDC_SHOWALL, OnShowall)
	ON_BN_CLICKED(IDC_QUERYPAPER, OnQuerypaper)
	ON_BN_CLICKED(IDC_ADDPAPER, OnAddpaper)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExamset message handlers

BOOL CDlgExamset::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_showall.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_showall.SetIcon(IDI_SHOWALL);
	m_showall.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_showall.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));	

	m_query.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_query.SetIcon(IDI_SEARCHPAPER);
	m_query.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_query.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));

	m_add.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_add.SetIcon(IDI_ADDPAPER);
	m_add.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_add.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));

	m_cancel.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_cancel.SetIcon(IDI_CANCEL);
	m_cancel.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_cancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));
	
	m_ok.SetShade(CShadeButtonST::SHS_DIAGSHADE,0,10,0,RGB(0,0,0));
	m_ok.SetIcon(IDI_OK);
	m_ok.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 178, 255));
	m_ok.SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(0, 0, 255));

	m_Static1.SubclassDlgItem(IDC_STATIC1,this);
	m_Static1.SetCaption("����ѡ��");
	m_Static1.SetBackColor(RGB(0,0,0));

	m_Static2.SubclassDlgItem(IDC_STATIC2,this);
	m_Static2.SetCaption("������Ҫѡ����Ծ�ǰ�򹴣�");
	m_Static2.SetBackColor(RGB(0,0,0));

	m_Static3.SubclassDlgItem(IDC_STATIC3,this);
	m_Static3.SetCaption("���ڱ������Եİ༶ǰ��ǰ�򹴣�");
	m_Static3.SetBackColor(RGB(0,0,0));
	
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
	m_list.SetHeadings("��������,100;���Կ�Ŀ,100; �ο�Ժϵ,100; �ο��༶,100;����ʱ��,150;�վ��Ծ�,150;��ʼ���,100;�������,100;������,100"); 	
	

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


    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );

 	m_classlist.ClearHeadings();
	m_classlist.SetHeadings("�༶���,100;�༶����,100; ����Ժϵ,100; ������,100"); 
	


// 	CoInitialize(NULL);
// 	CDBDepartment  m_Departmentdb;
// 	if ( !m_Departmentdb.Connect())
// 	{
// 		AfxMessageBox("Departmentdb: �޷����ӵ����ݿ������");
// 	}
// 	else
// 	{
// 		BOOL flag = m_Departmentdb.LoadByAll();
// 		while(flag)
// 		{
// 			if(m_Departmentdb.m_classid != "")
// 				m_classlist.AddItem(0,m_Departmentdb.m_classid,m_Departmentdb.m_class,m_Departmentdb.m_depart,m_Departmentdb.m_classhead);
// 			flag = m_Departmentdb.Move();
// 		}
// 		m_Departmentdb.Close();
// 	}
// 	CoUninitialize();

// 	CoInitialize(NULL);
// 	CDBExamaddr  m_Examaddrdb;
// 	if ( !m_Examaddrdb.Connect())
// 	{
// 		AfxMessageBox("Examaddrdb: �޷����ӵ����ݿ������");
// 	}
// 	else
// 	{
// 		BOOL flag = m_Examaddrdb.LoadByAll();
// 		while(flag)
// 		{
// 			CString sAddr = m_Examaddrdb.m_addr;
// 			sAddr.TrimLeft();
// 			sAddr.TrimRight();
// 			if(sAddr != "")
// 			{
// 				int temp = m_address.FindStringExact(-1, sAddr);
// 				if(temp == -1)
// 					m_address.AddString(sAddr);
// 			}
// 			flag = m_Examaddrdb.Move();
// 		}
// 		m_Examaddrdb.Close();
// 	}
// 	CoUninitialize();
	return TRUE;  
}

void CDlgExamset::OnShowall() 
{
	m_list.DeleteAllItems();
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
// 			m_list.AddItem(0,m_Paperdb.m_papername,
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

void CDlgExamset::OnQuerypaper() 
{
	CDlgPaperQuery  dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_list.DeleteAllItems();
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
// 				m_list.AddItem(0,m_Paperdb.m_papername,
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

void CDlgExamset::OnAddpaper() 
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
// 				m_list.AddItem(0,dlg.strPaper,
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

void CDlgExamset::OnOK() 
{
	int iCount = 0;
	for(int i = 0; i < m_list.GetItemCount(); i++)
	{
		if(m_list.GetCheck(i))
			iCount++;
	}
	if(iCount == 0)
	{
		AfxMessageBox("û��ѡ���Ծ��뷵��ѡ����Ҫ���Ե��Ծ�");
		return;
	}
	if(iCount > 1)
	{
		AfxMessageBox("ѡ���˶����Ծ��뷵������ѡ��");
		return;
	}
	iCount = 0;
	for(i = 0; i < m_classlist.GetItemCount(); i++)
	{
		if(m_classlist.GetCheck(i))
			iCount++;
	}
	if(iCount == 0)
	{
		AfxMessageBox("û��ѡ�񱾳����Եİ༶���뷵��ѡ����Ҫ���Եİ༶��");
		return;
	}

	for(i = 0; i < m_list.GetItemCount(); i++)
	{
		if(m_list.GetCheck(i))
		{
			sPaper = m_list.GetItemText(i,0);
			sSubject = m_list.GetItemText(i,1);
			sDepart = m_list.GetItemText(i,2);
			sClass = m_list.GetItemText(i,3);
			sStarttime = m_list.GetItemText(i,4);
			sEndtime = m_list.GetItemText(i,5);
			sStartNO = m_list.GetItemText(i,6);
			sEndNO = m_list.GetItemText(i,7);
			sMaxNO = m_list.GetItemText(i,8);
// 			CoInitialize(NULL);
// 			CDBPaper       m_Paperdb;
// 			if ( !m_Paperdb.Connect())
// 			{
// 				AfxMessageBox("Paperdb: �޷����ӵ����ݿ������");
// 			}
// 			else
// 			{
// 				BOOL flag = m_Paperdb.LoadByPaper(sSubject,sClass,sStarttime);
// 				if(flag)
// 				{
// 					paperid = m_Paperdb.m_ydid;
// 				}
// 				m_Paperdb.Close();
// 			}
// 			CoUninitialize();
		}
	}
	int index = m_address.GetCurSel();
	if(index != -1)
		m_address.GetLBText(index,sAddr);
	else
		sAddr = "";
	
	CString msg;
	msg.Format("��ȷ��ѡ����Ծ���\n�Ծ�����:%s\n���Կ�Ŀ��%s\n�ο�Ժϵ��%s\n�ο��༶��%s\n����ʱ�䣺%s\n�վ�ʱ�䣺%s\n��ʼ��ţ�%s\n������ţ�%s\n�����ţ�%s\n������%s",sPaper,sSubject,sDepart,sClass,sStarttime,sEndtime,sStartNO,sEndNO,sMaxNO,sAddr);

	if ( MessageBox(msg,"ȷ��",MB_OKCANCEL) != IDOK)
		return;	
	classarry.RemoveAll();
	for(i = 0; i < m_classlist.GetItemCount(); i++)
	{
		if(m_classlist.GetCheck(i))
			classarry.Add(m_classlist.GetItemText(i,1));
	}
	CDialog::OnOK();
}
CString CDlgExamset::GetExePath()
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
void CDlgExamset::OnPaint() 
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
