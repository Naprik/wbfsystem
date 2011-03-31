#include "stdafx.h"
#include "hardctrl.h"
#include "FormExamMode.h"
#include "MainFrm.h"
#include "ExamDetail.h"
#include "TeacherDev.h"
#include "ExamApp.h"
#include "DBTeacherDev.h"

extern CHardCtrlApp theApp;
CExamDetail  m_examdeteil;
extern CExamApp m_app;
extern CArray<CTeacherDev, CTeacherDev>  arryTeacherdev;

IMPLEMENT_DYNCREATE(CFormExamMode, CDialog)

CFormExamMode::CFormExamMode(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormExamMode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormExamMode)
	//}}AFX_DATA_INIT
}


void CFormExamMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamMode)
	DDX_Control(pDX, IDC_SEARCH, m_search);
	DDX_Control(pDX, IDC_CHOOSE, m_choose);
	DDX_Control(pDX, IDC_DEVLIST, m_devlist);
	DDX_Control(pDX, IDC_NEXT, m_next);
	DDX_Control(pDX, IDC_RADIO_BLUE, m_blue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamMode, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormExamMode)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_BN_CLICKED(IDC_CHOOSE, OnChoose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamMode message handlers

BOOL CFormExamMode::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();
	
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC2),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_STATIC3),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_RADIO_BLUE),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_RADIO_INFRA),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_RADIO_USB),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_NEXT),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_SEARCH),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_CHOOSE),mdRelative,mdRelative);
	AddSzControl(m_devlist,mdRelative,mdRelative);

	CRect btnRect;  
	m_next.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_next.SetBitmapId(IDB_NEXT,IDB_NEXT,IDB_NEXT_DOWN,IDB_NEXT); 

	m_search.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_search.SetBitmapId(IDB_SEARCH,IDB_SEARCH,IDB_SEARCH_DOWN,IDB_SEARCH); 

	m_choose.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_choose.SetBitmapId(IDB_CHOOSE,IDB_CHOOSE,IDB_CHOOSE_DOWN,IDB_CHOOSE); 

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

	m_devlist.SetHeadings("��ʦ�����, 80; ʹ����,150;ʹ�ö˿�,100;��״̬, 150");
	
	return TRUE;  
}

void CFormExamMode::OnNext() 
{	
	if(!(((CButton *)GetDlgItem(IDC_RADIO_BLUE))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_INFRA))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_USB))->GetCheck()))
	{
		AfxMessageBox("û��ѡ�񱾳������վ�ģʽ���޷�������һ����");
		return;
	}

	if(m_app.Get_teach_count() == 0)
	{
		AfxMessageBox("û��ѡ�񱾳�����ʹ�õĽ�ʦ��������ѡ���ʦ����");
		return;		
	}

	m_examdeteil.nSetp = 2;
	CString msg1,msg2;
	
	if(((CButton *)GetDlgItem(IDC_RADIO_BLUE))->GetCheck())
	{
		m_examdeteil.nRolling_pattern = 1;
		msg2 = "�����վ�";
	}
	else if(((CButton *)GetDlgItem(IDC_RADIO_INFRA))->GetCheck())
	{
		m_examdeteil.nRolling_pattern = 2;
		msg2 = "�����վ�";
	}
	else
	{
		m_examdeteil.nRolling_pattern = 3;
		msg2 = "USB�վ�";
	}


/*	CString msg;
	msg.Format("�������Բ��ã�\n����ģʽ��%s \n�վ�ģʽ��%s",msg1,msg2);
	if ( MessageBox(msg,"ȷ��",MB_OKCANCEL) != IDOK)
	{	
		m_examdeteil.nRoll_mode = 0;
		m_examdeteil.nRolling_pattern = 0;
		return;
	}*/

	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	fram->m_dlgMain.m_examset.SetKind(m_kind);
	fram->m_dlgMain.ShowDlg(IDD_FORM_EXAMSET);	

}

void CFormExamMode::SetKind(int kind)
{
	m_kind = kind;
}

void CFormExamMode::OnSearch() 
{
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	if(!m_app.Test())
	{
		AfxMessageBox("û�м�⵽��ʦ����");
		return;
	}
	int iCount = arryTeacherdev.GetSize();

	CString strMessage;
	strMessage.Format("�����ֽ�ʦ�� %d ̨", iCount);
	fram->m_dlgMsg.ShowMsg(strMessage);

	m_devlist.DeleteAllItems();

// 	CoInitialize(NULL);
// 	CDBTeacherDev m_TeacherDevdb;
// 	if ( !m_TeacherDevdb.Connect())
// 	{
// 		AfxMessageBox("TeacherDev �޷����ӵ����ݿ������");
// 	}
// 	else
// 	{
// 		for(int i = 0; i < iCount; i++)
// 		{
// 			CString strMac, strCom;
// 			strMac.Format("%s",arryTeacherdev[i].m_mac);
// 			strCom.Format("%s",arryTeacherdev[i].m_com);
// 			int flag = m_TeacherDevdb.LoadByDevmac(strMac);
// 			if(flag)
// 			{
// 				m_devlist.AddItem(0, m_TeacherDevdb.m_devid, m_TeacherDevdb.m_teachername, strCom ,"��⵽����ʹ��");
// 				m_devlist.SetCheck(i);
// 			}
// 		}
// 		m_TeacherDevdb.Close();
// 	}
// 	CoUninitialize();
	
}

void CFormExamMode::OnChoose() 
{
	if(m_devlist.GetItemCount() == 0)
	{
		AfxMessageBox("û�н�ʦ������ѡ�����ȵ������������ť��������ʦ����");
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
		AfxMessageBox("û��ѡ���ʦ����������Ҫʹ�õĽ�ʦ��ǰ�򹴣�");
		return;
	}	
	CString msg;
	msg.Format("ȷ��Ҫʹ�ý�ʦ�� %s �μӱ������ԣ�", strTemp);
	if ( MessageBox(msg,"ȷ��",MB_OKCANCEL) != IDOK)
		return;	
	
// 	CoInitialize(NULL);
// 	CDBTeacherDev TeacherDevdb;
// 	if ( !TeacherDevdb.Connect())
// 	{
// 		AfxMessageBox("TeacherDev �޷����ӵ����ݿ������");
// 	}
// 	else
// 	{
// 		int iSize = arryTeacherdev.GetSize();
// 		for(i = 0; i < m_devlist.GetItemCount() ; i++)
// 		{
// 			if(m_devlist.GetCheck(i))
// 			{
// 				CString strDevid = m_devlist.GetItemText(i,0);
// 				int flag = TeacherDevdb.LoadByDevid(strDevid);
// 				if(flag)
// 				{
// 					CString strDevmac = TeacherDevdb.m_devmac;
// 					for(int j = 0; j < iSize; j++)
// 					{
// 						strDevmac.TrimLeft();
// 						strDevmac.TrimRight();
// 						CString temp = arryTeacherdev[j].m_mac;
// 						temp.MakeUpper();
// 						strDevmac.MakeUpper();
// 						temp.TrimLeft();
// 						temp.TrimRight();
// 						if(temp == strDevmac)
// 						{
// 							CTeacherDev  TD;
// 							memset(TD.m_com,0,sizeof(TD.m_com));
// 							strcpy(TD.m_com,arryTeacherdev[j].m_com);
// 							memset(TD.m_mac,0,sizeof(TD.m_mac));
// 							strcpy(TD.m_mac,strDevmac);
// 							arryTeacherdev.Add(TD);
// 						}
// 					}
// 				}
// 			}
// 		}
// 		TeacherDevdb.Close();
// 		for(i = 0; i < iSize; i++)
// 			arryTeacherdev.RemoveAt(0);
// 	}
// 	CoUninitialize();

	m_app.Opencom();
	
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	for(i = 0; i < m_devlist.GetItemCount(); i++)
	{
		if(m_app.Get_teach_stat(m_devlist.GetItemText(i,2)))
		{
			CString strMessage;
			strMessage.Format("��ʦ�� %s ȷ�ϱ�ʹ�ã�", m_devlist.GetItemText(i,0));
			fram->m_dlgMsg.ShowMsg(strMessage);			
			m_devlist.SetItemText(i,3,"ʹ���У�");
		}
		else
		{	
			CString strMessage;
			strMessage.Format("��ʦ�� %s δ��ʹ�ã�", m_devlist.GetItemText(i,0));
			fram->m_dlgMsg.ShowMsg(strMessage);				
			m_devlist.SetItemText(i,3,"δ��ʹ�ã�");
		}
	}	
}
