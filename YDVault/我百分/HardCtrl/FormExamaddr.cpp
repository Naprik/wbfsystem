#include "stdafx.h"
#include "hardctrl.h"
#include "FormExamaddr.h"
#include "MainFrm.h"
#include "DlgExamaddr.h"

extern CHardCtrlApp theApp;

IMPLEMENT_DYNCREATE(CFormExamaddr, CDialog)

CFormExamaddr::CFormExamaddr(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormExamaddr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormExamaddr)
	//}}AFX_DATA_INIT
}


void CFormExamaddr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamaddr)
	DDX_Control(pDX, IDC_UPDATE, m_update);
	DDX_Control(pDX, IDC_SHOWALL, m_showall);
	DDX_Control(pDX, IDC_DELETE, m_delete);
	DDX_Control(pDX, IDC_ADD, m_add);
	DDX_Control(pDX, IDC_ADDRLIST, m_addrlist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamaddr, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormExamaddr)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_SHOWALL, OnShowall)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamaddr message handlers
BOOL CFormExamaddr::OnInitDialog()
{
	cdxCSizingDialog::OnInitDialog();

// 	CRect btnRect; 
// 	m_add.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_add.SetBitmapId(IDB_ADD,IDB_ADD,IDB_ADD_DOWN,IDB_ADD); 
// 
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
   
	AddSzControl(*GetDlgItem(IDC_ADD),mdNone,mdRepos);	               //重新绘制控件在窗口的位置和大小
	AddSzControl(*GetDlgItem(IDC_DELETE),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_UPDATE),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_SHOWALL),mdNone,mdRepos);
	AddSzControl(m_addrlist,mdResize,mdResize);	

    DWORD dwStyle=GetWindowLong(m_addrlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_addrlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_addrlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_addrlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_addrlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_addrlist.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	m_addrlist.SetHeadings("考场编号,100;考场,150");
	return TRUE;
}

void CFormExamaddr::OnAdd() 
{
	CDlgExamaddr	dlg;
	CoInitialize(NULL);
	CDBExamaddr  m_Examaddrdb;
	if ( !m_Examaddrdb.Connect())
	{
		AfxMessageBox("Examaddrdb: 无法连接到数据库服务器!");
	}
	else
	{
		if(dlg.DoModal() == IDOK)
		{
			CString  strAddrID = dlg.m_addid;
			strAddrID.TrimLeft();
			strAddrID.TrimRight();
			if(m_Examaddrdb.LoadByAddrID(strAddrID))
			{
				AfxMessageBox("该考场编号已存在，请返回确认！");
			}
			else
			{
				m_Examaddrdb.m_ydid = 0;
				m_Examaddrdb.m_addrid = dlg.m_addid;
				m_Examaddrdb.m_examaddr = dlg.m_addr;
				if(m_Examaddrdb.Add())
				{
					AfxMessageBox("添加成功！");
					m_addrlist.AddItem(0, dlg.m_addid,dlg.m_addr);
				}
				else
					AfxMessageBox("添加失败！");
			}
		}
		m_Examaddrdb.Close();
	}
	CoUninitialize();
}

void CFormExamaddr::OnUpdate() 
{
	CDlgExamaddr dlg;
	BOOL bMark = TRUE;
	for(int t = 0; t < m_addrlist.GetItemCount(); t++)
	{
		if(m_addrlist.GetCheck(t))
		{
			bMark = FALSE;
			break;
		}
	}
	if(bMark)
	{
		AfxMessageBox("没有选中考场，请在需要修改的考场前打勾！");
		return;
	}	

	CoInitialize(NULL);	
	CDBExamaddr  m_Examaddrdb;
	if ( !m_Examaddrdb.Connect())
	{
		AfxMessageBox("Examaddrdb:无法连接到数据库服务器，删除失败！");
	}
	else
	{
		for(int i = 0; i < m_addrlist.GetItemCount(); i++)
		{
			if(m_addrlist.GetCheck(i))
			{
				dlg.m_addid = m_addrlist.GetItemText(i,0);
				dlg.m_addr = m_addrlist.GetItemText(i,1);
				if(dlg.DoModal() == IDOK)
				{
					BOOL flag = m_Examaddrdb.LoadByAddrID(m_addrlist.GetItemText(i,0));
					if(flag)
					{
						m_Examaddrdb.m_addrid = dlg.m_addid;
						m_Examaddrdb.m_examaddr = dlg.m_addr;
						if(m_Examaddrdb.Update(m_Examaddrdb.m_ydid))
							AfxMessageBox("修改成功！");
						else
							AfxMessageBox("修改失败！");
					}
				}
				m_addrlist.SetItemText(i, 0, m_Examaddrdb.m_addrid);
				m_addrlist.SetItemText(i, 1, m_Examaddrdb.m_examaddr);
			}
		}
		m_Examaddrdb.Close();
	}
	CoUninitialize();
}

void CFormExamaddr::OnDelete() 
{
	BOOL bMark = TRUE;
	for(int t = 0; t < m_addrlist.GetItemCount(); t++)
	{
		if(m_addrlist.GetCheck(t))
		{
			bMark = FALSE;
			break;
		}
	}
	if(bMark)
	{
		AfxMessageBox("没有选中考场，请在需要删除的考场前打勾！");
		return;
	}	
	CString msg;
	msg.Format("确定要删除选中的内容吗？");
	if ( MessageBox(msg,"确认",MB_OKCANCEL) != IDOK)
		return;	

	CoInitialize(NULL);
	CDBExamaddr  m_Examaddrdb;	
	if ( !m_Examaddrdb.Connect())
	{
		AfxMessageBox("Examaddrdb:无法连接到数据库服务器，删除失败！");
	}
	else
	{
		for(int i = 0; i < m_addrlist.GetItemCount(); i++)
		{
			if(m_addrlist.GetCheck(i))
			{
				CString strAddrID = m_addrlist.GetItemText(i, 0);
				strAddrID.TrimLeft();
				strAddrID.TrimRight();
				BOOL flag = m_Examaddrdb.LoadByAddrID(strAddrID);
				if(flag)
				{
					if(!m_Examaddrdb.Delete(m_Examaddrdb.m_ydid))
					{
						CString strTemp = "考场编号为："+strAddrID+"删除失败！";
						AfxMessageBox(strTemp);
					}
				}
			}
		}
		AfxMessageBox("删除成功！");
		int iSize = m_addrlist.GetItemCount();
		for(i = 0; i < iSize; i++)
		{
			BOOL flag = m_Examaddrdb.LoadByAddrID(m_addrlist.GetItemText(i, 0));
			if(flag)
			{
				m_addrlist.AddItem(0,m_Examaddrdb.m_addrid,m_Examaddrdb.m_examaddr);
			}
		}
		for(i = 0; i < iSize; i++)
		{
			m_addrlist.DeleteItem(0);
		}
		m_Examaddrdb.Close();
	}
	CoUninitialize();
}

void CFormExamaddr::showdata()
{
	m_addrlist.DeleteAllItems();	

	CoInitialize(NULL);
	CDBExamaddr  m_Examaddrdb;
	if ( !m_Examaddrdb.Connect())
	{
		AfxMessageBox("Examaddrdb：无法连接到数据库服务器！");
	}
	else
	{
		BOOL flag = m_Examaddrdb.LoadByAll();
		while ( flag )
		{
			m_addrlist.AddItem(0, m_Examaddrdb.m_addrid, m_Examaddrdb.m_examaddr);
			flag = m_Examaddrdb.Move();
		}
		m_Examaddrdb.Close();
	}	
	CoUninitialize();
}



void CFormExamaddr::OnShowall() 
{
	showdata();	
}
CString CFormExamaddr::GetExePath()
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
void CFormExamaddr::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
// 	CRect rc;
//     CJahCoverCapture capture;
// 	GetClientRect(&rc);
// 	capture.m_bmpFile =GetExePath()+"formback.bmp" ;
//     if(!capture.LoadBMPImage(capture.m_bmpFile))
//    		return;
//     if (capture.m_bitmap.m_hObject == 0 )
//     	return;
//    capture.DrawBitmap(&dc,rc);
}
