// FormTeacherDev.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormTeacherDev.h"
#include "DBTeacherDev.h"
#include "DlgTeacherdev.h"
	
extern CHardCtrlApp theApp;
IMPLEMENT_DYNCREATE(CFormTeacherDev, CDialog)


CFormTeacherDev::CFormTeacherDev(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormTeacherDev::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormTeacherDev)
	//}}AFX_DATA_INIT
}


void CFormTeacherDev::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormTeacherDev)
	DDX_Control(pDX, IDC_UPDATE, m_update);
	DDX_Control(pDX, IDC_SHOWALL, m_showall);
	DDX_Control(pDX, IDC_DELETE, m_delete);
	DDX_Control(pDX, IDC_ADD, m_add);
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormTeacherDev, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormTeacherDev)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_SHOWALL, OnShowall)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormTeacherDev message handlers
BOOL CFormTeacherDev::OnInitDialog()
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


	AddSzControl(*GetDlgItem(IDC_ADD),mdNone,mdRepos);	
	AddSzControl(*GetDlgItem(IDC_DELETE),mdNone,mdRepos);	
	AddSzControl(*GetDlgItem(IDC_UPDATE),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_SHOWALL),mdNone,mdRepos);	
	AddSzControl(m_list,mdResize,mdResize);	

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
	m_list.SetHeadings("���,150;ʹ�ý�ʦ,100;��ʦ��������ַ,150");

	return TRUE;
}



void CFormTeacherDev::showdata()
{
	m_list.DeleteAllItems();
	
	CoInitialize(NULL);
	CDBTeacherDev   m_TeacherDevdb;
	if ( !m_TeacherDevdb.Connect())
	{
		AfxMessageBox("TeacherDevdb���޷����ӵ����ݿ��������");
	}
	else
	{
		BOOL flag = m_TeacherDevdb.LoadByAll();
		while ( flag )
		{
			m_list.AddItem(0, m_TeacherDevdb.m_teacherid,m_TeacherDevdb.m_teachername,m_TeacherDevdb.m_devmac);
			flag = m_TeacherDevdb.Move();
		}
		m_TeacherDevdb.Close();	
	}
	CoUninitialize();
}

void CFormTeacherDev::OnAdd() 
{
	CDlgTeacherdev	dlg;
	CoInitialize(NULL);
	CDBTeacherDev   m_TeacherDevdb;
	if ( !m_TeacherDevdb.Connect())
	{
		AfxMessageBox("TeacherDev �޷����ӵ����ݿ������");
	}
	else
	{
		if(dlg.DoModal() == IDOK)
		{
			CString  strDevID = dlg.m_devid;
			strDevID.TrimLeft();
			strDevID.TrimRight();
			if(m_TeacherDevdb.LoadByDevid(dlg.m_devid))
			{
				AfxMessageBox("�ñ�Ž�ʦ���Ѵ��ڣ��뷵��ȷ�ϣ�");
			}
			else if(m_TeacherDevdb.LoadByDevmac(dlg.m_devmac))
			{
				AfxMessageBox("��������ַ��ʦ���Ѵ��ڣ��뷵��ȷ�ϣ�");
			}
			else
			{
				m_TeacherDevdb.m_ydid = 0;
				m_TeacherDevdb.m_teacherid = dlg.m_devid;
				m_TeacherDevdb.m_teachername = dlg.m_teachername;
				m_TeacherDevdb.m_devmac = dlg.m_devmac;
				if(m_TeacherDevdb.Add())
				{
					AfxMessageBox("��ӳɹ���");
					m_list.AddItem(0, dlg.m_devid,dlg.m_teachername,dlg.m_devmac);
				}
				else
					AfxMessageBox("���ʧ�ܣ�");
			}
		}
		m_TeacherDevdb.Close();
	}
	CoUninitialize();
}

void CFormTeacherDev::OnDelete() 
{
	BOOL bMark = TRUE;
	for(int t = 0; t < m_list.GetItemCount(); t++)
	{
		if(m_list.GetCheck(t))
		{
			bMark = FALSE;
			break;
		}
	}
	if(bMark)
	{
		AfxMessageBox("û��ѡ��ѧ����������Ҫɾ����ѧ��ǰ�򹴣�");
		return;
	}	
	CString msg;
	msg.Format("���Ҫɾ��ѡ�е�������");
	if ( MessageBox(msg,"ȷ��",MB_OKCANCEL) != IDOK)
		return;	

	CoInitialize(NULL);	
	CDBTeacherDev   m_TeacherDevdb;
	if ( !m_TeacherDevdb.Connect())
	{
		AfxMessageBox("TeacherDevdb:�޷����ӵ����ݿ��������ɾ��ʧ�ܣ�");
	}
	else
	{
		for(int i = 0; i < m_list.GetItemCount(); i++)
		{
			if(m_list.GetCheck(i))
			{
				CString strDevID = m_list.GetItemText(i, 0);
				strDevID.TrimLeft();
				strDevID.TrimRight();
				BOOL flag = m_TeacherDevdb.LoadByDevid(strDevID);
				if(flag)
				{
					if(!m_TeacherDevdb.Delete(m_TeacherDevdb.m_ydid))
					{
						CString strTemp = "��ʦ���Ϊ��"+strDevID+"ɾ��ʧ�ܣ�";
						AfxMessageBox(strTemp);
					}
				}
			}
		}
		AfxMessageBox("ɾ���ɹ���");
		int iSize = m_list.GetItemCount();
		for(i = 0; i < iSize; i++)
		{
			BOOL flag = m_TeacherDevdb.LoadByDevid(m_list.GetItemText(i, 0));
			if(flag)
			{
				m_list.AddItem(0,m_TeacherDevdb.m_teacherid,m_TeacherDevdb.m_teachername,m_TeacherDevdb.m_devmac);
			}
		}
		for(i = 0; i < iSize; i++)
		{
			m_list.DeleteItem(0);
		}
		m_TeacherDevdb.Close();
	}
	CoUninitialize();
}

void CFormTeacherDev::OnUpdate() 
{
	CDlgTeacherdev	dlg;
	BOOL bMark = TRUE;
	for(int t = 0; t < m_list.GetItemCount(); t++)
	{
		if(m_list.GetCheck(t))
		{
			bMark = FALSE;
			break;
		}
	}
	if(bMark)
	{
		AfxMessageBox("û��ѡ�н�ʦ����������Ҫ�޸ĵĽ�ʦ��ǰ�򹴣�");
		return;
	}	

	CoInitialize(NULL);	
	CDBTeacherDev   m_TeacherDevdb;
	if ( !m_TeacherDevdb.Connect())
	{
		AfxMessageBox("TeacherDevdb:�޷����ӵ����ݿ��������ɾ��ʧ�ܣ�");
	}
	else
	{
		for(int i = 0; i < m_list.GetItemCount(); i++)
		{
			if(m_list.GetCheck(i))
			{
				dlg.m_devid = m_list.GetItemText(i,0);
				dlg.m_teachername = m_list.GetItemText(i,1);
				dlg.m_devmac = m_list.GetItemText(i,2);
				if(dlg.DoModal() == IDOK)
				{
					BOOL flag = m_TeacherDevdb.LoadByDevid(m_list.GetItemText(i,0));
					if(flag)
					{
						m_TeacherDevdb.m_teacherid = dlg.m_devid;
						m_TeacherDevdb.m_teachername = dlg.m_teachername;
						m_TeacherDevdb.m_devmac = dlg.m_devmac;
						if(m_TeacherDevdb.Update(m_TeacherDevdb.m_ydid))
							AfxMessageBox("�޸ĳɹ���");
						else
							AfxMessageBox("�޸�ʧ�ܣ�");
					}
				}
				m_list.SetItemText(i, 0, m_TeacherDevdb.m_teacherid);
				m_list.SetItemText(i, 1, m_TeacherDevdb.m_teachername);
				m_list.SetItemText(i, 2, m_TeacherDevdb.m_devmac);
			}
		}
		m_TeacherDevdb.Close();
	}
	CoUninitialize();
}

void CFormTeacherDev::OnShowall() 
{
	showdata();	
}
CString CFormTeacherDev::GetExePath()
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
void CFormTeacherDev::OnPaint() 
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
