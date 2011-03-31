// FormStuinfo.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormStuinfo.h"
#include "MainFrm.h"
#include "DlgPhoto.h"
#include "DlgStuQuery.h"

extern CHardCtrlApp theApp;

IMPLEMENT_DYNCREATE(CFormStuinfo, CDialog)
/////////////////////////////////////////////////////////////////////////////
// CFormStuinfo dialog


CFormStuinfo::CFormStuinfo(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormStuinfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormStuinfo)
	//}}AFX_DATA_INIT
}


void CFormStuinfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormStuinfo)
	DDX_Control(pDX, IDC_QUERY, m_query);
	DDX_Control(pDX, IDC_STULIST, m_stulist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormStuinfo, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormStuinfo)
	ON_BN_CLICKED(IDC_QUERY, OnQuery)
	ON_NOTIFY(NM_DBLCLK, IDC_STULIST, OnDblclkStulist)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormStuinfo message handlers

BOOL CFormStuinfo::OnInitDialog()
{
	cdxCSizingDialog::OnInitDialog();

	m_Static1.SubclassDlgItem(IDC_STATIC1,this);
	m_Static1.SetCaption("双击列表可查看学生照片");
	m_Static1.SetBackColor(RGB(0,0,0));

	AddSzControl(m_stulist,mdResize,mdResize);
	AddSzControl(*GetDlgItem(IDC_QUERY),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdNone,mdNone);

	DWORD dwStyle=GetWindowLong(m_stulist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_stulist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_stulist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_stulist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_stulist.GetExtendedStyle();
    m_stulist.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE);

	m_stulist.ClearHeadings();
	m_stulist.SetHeadings("学号,100; 姓名,100; 性别,50; 院系,150; 班级,100; 电话,100; 电子邮箱,150; 学生机设备号,100");
							
	return TRUE;
}

void CFormStuinfo::OnQuery() 
{
	CDlgStuQuery dlg;
	if(dlg.DoModal() == IDOK)
	{
// 		CoInitialize(NULL);	
// 		CDBStuInfo  m_StuInfodb;
// 		if ( !m_StuInfodb.Connect())
// 		{
// 			AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 		}
// 		else
// 		{
// 			m_stulist.DeleteAllItems();
// 			if(dlg.m_stuid != "")
// 			{
// 				BOOL flag = m_StuInfodb.LoadByStuid(dlg.m_stuid);
// 				if(flag)
// 				{
// 					m_stulist.AddItem(0,
// 									m_StuInfodb.m_stuid,
// 									m_StuInfodb.m_stuname,
// 									m_StuInfodb.m_Stugender,
// 									m_StuInfodb.m_Studepartment,
// 									m_StuInfodb.m_Stuclass,
// 									m_StuInfodb.m_stutel,
// 									m_StuInfodb.m_stuemail,
// 									m_StuInfodb.m_devid);
// 				}
// 			}
// 			else if(dlg.m_stuname != "")
// 			{
// 				BOOL flag = m_StuInfodb.LoadByStuname(dlg.m_stuname);
// 				if(flag)
// 				{
// 					m_stulist.AddItem(0,
// 									m_StuInfodb.m_stuid,
// 									m_StuInfodb.m_stuname,
// 									m_StuInfodb.m_Stugender,
// 									m_StuInfodb.m_Studepartment,
// 									m_StuInfodb.m_Stuclass,
// 									m_StuInfodb.m_stutel,
// 									m_StuInfodb.m_stuemail,
// 									m_StuInfodb.m_devid);
// 				}
// 			}
// 			else
// 			{
// 				BOOL flag = m_StuInfodb.LoadByClass(dlg.strClass);
// 				while(flag)
// 				{
// 					m_stulist.AddItem(0,
// 									m_StuInfodb.m_stuid,
// 									m_StuInfodb.m_stuname,
// 									m_StuInfodb.m_Stugender,
// 									m_StuInfodb.m_Studepartment,
// 									m_StuInfodb.m_Stuclass,
// 									m_StuInfodb.m_stutel,
// 									m_StuInfodb.m_stuemail,
// 									m_StuInfodb.m_devid);					
// 					flag = m_StuInfodb.Move();
// 				}
// 			}
// 			m_StuInfodb.Close();
// 		}
// 		CoUninitialize();
	}
	if(m_stulist.GetItemCount() == 0)
		AfxMessageBox("没有查询到相应内容！");	
}

void CFormStuinfo::OnDblclkStulist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item =pNMListView->iItem;
	CString strStuID = m_stulist.GetItemText(item,0);
	if(strStuID != "")
	{
// 		CoInitialize(NULL);
// 		
// 		CDBStuInfo  m_StuInfodb;	
// 		if ( !m_StuInfodb.Connect())
// 		{
// 			AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 		}	
// 		else
// 		{
// 			CDlgPhoto  dlg;
// 			BOOL flag = m_StuInfodb.LoadByStuid(strStuID);
// 			if(flag)
// 			{
// 				dlg.m_id = m_StuInfodb.m_stuid;
// 				dlg.m_name = m_StuInfodb.m_stuname;
// 				dlg.m_gender = m_StuInfodb.m_Stugender;
// 				dlg.m_depart = m_StuInfodb.m_Studepartment;
// 				dlg.m_class = m_StuInfodb.m_Stuclass;
// 				dlg.m_tel = m_StuInfodb.m_stutel;
// 				dlg.m_email = m_StuInfodb.m_stuemail;
// 				dlg.m_path = m_StuInfodb.m_stuphoto;
// 			}
// 			m_StuInfodb.Close();
// 			dlg.DoModal();
// 		}
// 		CoUninitialize();
	}
	*pResult = 0;
}
CString CFormStuinfo::GetExePath()
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
void CFormStuinfo::OnPaint() 
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
   capture.DrawBitmap(&dc,rc);*/
}
