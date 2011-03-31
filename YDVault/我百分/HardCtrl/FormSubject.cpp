// FormSubject.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormSubject.h"
#include "MainFrm.h"
#include "DBSubject.h"
#include "DlgSubject.h"

extern CHardCtrlApp theApp;
IMPLEMENT_DYNCREATE(CFormSubject, CDialog)


CFormSubject::CFormSubject(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormSubject::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormSubject)
	//}}AFX_DATA_INIT
}


void CFormSubject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormSubject)
	DDX_Control(pDX, IDC_UPDATE, m_update);
	DDX_Control(pDX, IDC_SHOWALL, m_showall);
	DDX_Control(pDX, IDC_DELETE, m_delete);
	DDX_Control(pDX, IDC_ADD, m_add);
	DDX_Control(pDX, IDC_SUBJECTLIST, m_subjectlist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormSubject, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormSubject)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_SHOWALL, OnShowall)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormSubject message handlers
BOOL CFormSubject::OnInitDialog()
{
	cdxCSizingDialog::OnInitDialog();

// 	CRect btnRect;  
// 	m_showall.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_showall.SetBitmapId(IDB_SHOWALL,IDB_SHOWALL,IDB_SHOWALL_DOWN,IDB_SHOWALL); 	
// 
// 	m_update.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_update.SetBitmapId(IDB_UPDATE,IDB_UPDATE,IDB_UPDATE_DOWN,IDB_UPDATE); 
// 	
// 	m_delete.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_delete.SetBitmapId(IDB_DELETE,IDB_DELETE,IDB_DELETE_DOWN,IDB_DELETE); 
// 
// 	m_add.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_add.SetBitmapId(IDB_ADD,IDB_ADD,IDB_ADD_DOWN,IDB_ADD); 


	AddSzControl(*GetDlgItem(IDC_UPDATE),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_DELETE),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_SHOWALL),mdNone,mdRepos);
    AddSzControl(*GetDlgItem(IDC_ADD),mdNone,mdRepos);
	AddSzControl(m_subjectlist,mdResize,mdResize);	

    DWORD dwStyle=GetWindowLong(m_subjectlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_subjectlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_subjectlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_subjectlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_subjectlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_subjectlist.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	
	m_subjectlist.ClearHeadings();
	m_subjectlist.SetHeadings("科目编号,100;科目名称,150");

	return TRUE;
}

void CFormSubject::OnAdd() 
{
	CDlgSubject	dlg;
	CoInitialize(NULL);
	CDBSubject   m_Subjectdb;
	if ( !m_Subjectdb.Connect())
	{
		AfxMessageBox("Subjectdb: 无法连接到数据库服务器!");
	}
	else
	{
		if(dlg.DoModal() == IDOK)
		{
			CString  strSubjectID = dlg.m_subjectid;
			strSubjectID.TrimLeft();
			strSubjectID.TrimRight();
			if(m_Subjectdb.LoadBySubjectid(strSubjectID))
			{
				AfxMessageBox("该科目编号已存在，请返回确认！");
			}
			else
			{
				m_Subjectdb.m_ydid = 0;
				m_Subjectdb.m_subjectid = dlg.m_subjectid;
				m_Subjectdb.m_subject = dlg.m_subject;
				if(m_Subjectdb.Add())
				{
					AfxMessageBox("添加成功！");
					m_subjectlist.AddItem(0, dlg.m_subjectid, dlg.m_subject);
				}
				else
					AfxMessageBox("添加失败！");
			}
		}
		m_Subjectdb.Close();
	}
	CoUninitialize();
}

void CFormSubject::OnUpdate() 
{
	CDlgSubject	dlg;
	BOOL bMark = TRUE;
	for(int t = 0; t < m_subjectlist.GetItemCount(); t++)
	{
		if(m_subjectlist.GetCheck(t))
		{
			bMark = FALSE;
			break;
		}
	}
	if(bMark)
	{
		AfxMessageBox("没有选中学生，请在需要修改的学生前打勾！");
		return;
	}	

	CoInitialize(NULL);
	CDBSubject   m_Subjectdb;	
	if ( !m_Subjectdb.Connect())
	{
		AfxMessageBox("Subjectdb:无法连接到数据库服务器，删除失败！");
	}
	else
	{
		for(int i = 0; i < m_subjectlist.GetItemCount(); i++)
		{
			if(m_subjectlist.GetCheck(i))
			{
				dlg.m_subjectid = m_subjectlist.GetItemText(i,0);
				dlg.m_subject = m_subjectlist.GetItemText(i,1);
				if(dlg.DoModal() == IDOK)
				{
					BOOL flag = m_Subjectdb.LoadBySubjectid(m_subjectlist.GetItemText(i,0));
					if(flag)
					{
						m_Subjectdb.m_subjectid = dlg.m_subjectid;
						m_Subjectdb.m_subject = dlg.m_subject;
						if(m_Subjectdb.Update(m_Subjectdb.m_ydid))
							AfxMessageBox("修改成功！");
						else
							AfxMessageBox("修改失败！");
					}
				}
				m_subjectlist.SetItemText(i, 0, m_Subjectdb.m_subjectid);
				m_subjectlist.SetItemText(i, 1, m_Subjectdb.m_subject);
			}
		}
		m_Subjectdb.Close();
	}
	CoUninitialize();
}

void CFormSubject::OnDelete() 
{
	BOOL bMark = TRUE;
	for(int t = 0; t < m_subjectlist.GetItemCount(); t++)
	{
		if(m_subjectlist.GetCheck(t))
		{
			bMark = FALSE;
			break;
		}
	}
	if(bMark)
	{
		AfxMessageBox("没有选中科目，请在需要删除的科目前打勾！");
		return;
	}	
	CString msg;
	msg.Format("确定要删除选中的内容吗？");
	if ( MessageBox(msg,"确认",MB_OKCANCEL) != IDOK)
		return;	

	CoInitialize(NULL);	
	CDBSubject   m_Subjectdb;
	if ( !m_Subjectdb.Connect())
	{
		AfxMessageBox("Subjectdb:无法连接到数据库服务器，删除失败！");
	}
	else
	{
		for(int i = 0; i < m_subjectlist.GetItemCount(); i++)
		{
			if(m_subjectlist.GetCheck(i))
			{
				CString strSubjectID = m_subjectlist.GetItemText(i, 0);
				strSubjectID.TrimLeft();
				strSubjectID.TrimRight();
				BOOL flag = m_Subjectdb.LoadBySubjectid(strSubjectID);
				if(flag)
				{
					if(!m_Subjectdb.Delete(m_Subjectdb.m_ydid))
					{
						CString strTemp = "科目编号为："+strSubjectID+"删除失败！";
						AfxMessageBox(strTemp);
					}
				}
			}
		}
		AfxMessageBox("删除成功！");
		int iSize = m_subjectlist.GetItemCount();
		for(i = 0; i < iSize; i++)
		{
			BOOL flag = m_Subjectdb.LoadBySubjectid(m_subjectlist.GetItemText(i, 0));
			if(flag)
			{
				m_subjectlist.AddItem(0,m_Subjectdb.m_subjectid,m_Subjectdb.m_subject);
			}
		}
		for(i = 0; i < iSize; i++)
		{
			m_subjectlist.DeleteItem(0);
		}
		m_Subjectdb.Close();
	}
	CoUninitialize();
}

void CFormSubject::showdata()
{
	m_subjectlist.DeleteAllItems();	

	CoInitialize(NULL);
	CDBSubject   m_Subjectdb;
	if ( !m_Subjectdb.Connect())
	{
		AfxMessageBox("Subjectdb：无法连接到数据库服务器！");
	}
	else
	{
		BOOL flag = m_Subjectdb.LoadByAll();
		while ( flag )
		{
			m_subjectlist.AddItem(0, m_Subjectdb.m_subjectid, m_Subjectdb.m_subject);
			flag = m_Subjectdb.Move();
		}
		m_Subjectdb.Close();
	}	
	CoUninitialize();
}

void CFormSubject::OnShowall() 
{
	showdata();	
}
CString CFormSubject::GetExePath()
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
void CFormSubject::OnPaint() 
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
