// FormlLink.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormlLink.h"
#include "MainFrm.h"
#include "DBResult.h"
#include "afxtempl.h"
#include "TeacherDev.h"
#include "ExamApp.h"
#include "DBTeacherDev.h"

extern CHardCtrlApp theApp;
extern CArray<CTeacherDev, CTeacherDev>  arryTeacherdev;
CExamApp m_app;

IMPLEMENT_DYNCREATE(CFormlLink, CDialog)


CFormlLink::CFormlLink(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormlLink::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormlLink)
	//}}AFX_DATA_INIT
}


void CFormlLink::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormlLink)
	DDX_Control(pDX, IDC_TEST, m_test);
	DDX_Control(pDX, IDC_CHOOSE, m_choose);
	DDX_Control(pDX, IDC_TEACHER, m_teacherlist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormlLink, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormlLink)
	ON_BN_CLICKED(IDC_TEST, OnTest)
	ON_BN_CLICKED(IDC_CHOOSE, OnChoose)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormlLink message handlers

BOOL CFormlLink::OnInitDialog()
{
	CDialog::OnInitDialog();

	cdxCSizingDialog::OnInitDialog();
//	HCURSOR hcur=::LoadCursorFromFile("AniCur.ani");           //���ض������
//	::SetClassLong(GetSafeHwnd(),GCL_HCURSOR,(LONG)hcur);

	AddSzControl(*GetDlgItem(IDC_TEST),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_CHOOSE),mdNone,mdRepos);
	AddSzControl(m_teacherlist,mdResize,mdResize);

	DWORD dwStyle=GetWindowLong(m_teacherlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_teacherlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_teacherlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_teacherlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_teacherlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_teacherlist.SetExtendedStyle(dwStyle);

	m_teacherlist.SetHeadings("��ʦ�����, 80; ʹ����,150;ʹ�ö˿�,100;��״̬, 150");

	CRect btnRect;  
	m_test.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_test.SetBitmapId(IDB_SEARCH,IDB_SEARCH,IDB_SEARCH_DOWN,IDB_SEARCH); 

	m_choose.GetWindowRect(btnRect); 
	ScreenToClient(btnRect); 
	m_choose.SetBitmapId(IDB_CHOOSE,IDB_CHOOSE,IDB_CHOOSE_DOWN,IDB_CHOOSE);
	return TRUE;
}

void CFormlLink::OnTest() 
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

	m_teacherlist.DeleteAllItems();

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
// 				m_teacherlist.AddItem(0, m_TeacherDevdb.m_devid, m_TeacherDevdb.m_teachername, strCom ,"��⵽����ʹ��");
// 				m_teacherlist.SetCheck(i);
// 			}
// 		}
// 		m_TeacherDevdb.Close();
// 	}
// 	CoUninitialize();
}


void CFormlLink::OnChoose() 
{	
	if(m_teacherlist.GetItemCount() == 0)
	{
		AfxMessageBox("û�н�ʦ������ѡ�����ȵ������������ť��������ʦ����");
		return;
	}
	
	CString strTemp = "";
	int i = 0;
	int iCount = 0;
	for(; i < m_teacherlist.GetItemCount() ; i++)
	{
		if(m_teacherlist.GetCheck(i))
		{
			CString strTeacherCOM = m_teacherlist.GetItemText(i,0);			
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
// 		for(i = 0; i < m_teacherlist.GetItemCount() ; i++)
// 		{
// 			if(m_teacherlist.GetCheck(i))
// 			{
// 				CString strDevid = m_teacherlist.GetItemText(i,0);
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
	for(i = 0; i < m_teacherlist.GetItemCount(); i++)
	{
		if(m_app.Get_teach_stat(m_teacherlist.GetItemText(i,2)))
		{
			CString strMessage;
			strMessage.Format("��ʦ�� %s ȷ�ϱ�ʹ�ã�", m_teacherlist.GetItemText(i,0));
			fram->m_dlgMsg.ShowMsg(strMessage);			
			m_teacherlist.SetItemText(i,3,"ʹ���У�");
		}
		else
		{	
			CString strMessage;
			strMessage.Format("��ʦ�� %s δ��ʹ�ã�", m_teacherlist.GetItemText(i,0));
			fram->m_dlgMsg.ShowMsg(strMessage);				
			m_teacherlist.SetItemText(i,3,"δ��ʹ�ã�");
		}
	}
}
CString CFormlLink::GetExePath()
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
void CFormlLink::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
/*	CRect rc;
    CJahCoverCapture capture;
	GetClientRect(&rc);
	capture.m_bmpFile =GetExePath()+"formback.bmp" ;
    if(!capture.LoadBMPImage(capture.m_bmpFile))
   		return;
    if (capture.m_bitmap.m_hObject == 0 )
    	return;
   capture.DrawBitmap(&dc,rc);	*/
}
