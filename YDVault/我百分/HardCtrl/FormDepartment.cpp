#include "stdafx.h"
#include "hardctrl.h"
#include "FormDepartment.h"
#include "MainFrm.h"
#include "DlgDpartment.h"
#include "DlgClassSet.h"

extern CHardCtrlApp theApp;

IMPLEMENT_DYNCREATE(CFormDepartment, CDialog)


CFormDepartment::CFormDepartment(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormDepartment::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormDepartment)
	//}}AFX_DATA_INIT
}


void CFormDepartment::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormDepartment)
	DDX_Control(pDX, IDC_UPDATEDEPART, m_updatedepart);
	DDX_Control(pDX, IDC_UPDATECLASS, m_updateclass);
	DDX_Control(pDX, IDC_DISPLAY, m_display);
	DDX_Control(pDX, IDC_DELETEDEPART, m_deletedepart);
	DDX_Control(pDX, IDC_DELETECLASS, m_deleteclass);
	DDX_Control(pDX, IDC_ADDDEPART, m_adddepart);
	DDX_Control(pDX, IDC_ADDCLASS, m_addclass);
	DDX_Control(pDX, IDC_CLASS, m_classlist);
	DDX_Control(pDX, IDC_DEPARTMENTLIST, m_departlist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormDepartment, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormDepartment)
	ON_BN_CLICKED(IDC_DISPLAY, OnDisplay)
	ON_BN_CLICKED(IDC_ADDDEPART, OnAdddepart)
	ON_BN_CLICKED(IDC_ADDCLASS, OnAddclass)
	ON_NOTIFY(NM_DBLCLK, IDC_DEPARTMENTLIST, OnDblclkDepartmentlist)
	ON_BN_CLICKED(IDC_UPDATEDEPART, OnUpdatedepart)
	ON_BN_CLICKED(IDC_UPDATECLASS, OnUpdateclass)
	ON_BN_CLICKED(IDC_DELETEDEPART, OnDeletedepart)
	ON_BN_CLICKED(IDC_DELETECLASS, OnDeleteclass)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormDepartment message handlers

BOOL CFormDepartment::OnInitDialog()
{
	cdxCSizingDialog::OnInitDialog();

// 	CRect btnRect; 
// 	m_deleteclass.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_deleteclass.SetBitmapId(IDB_DELETECLASS,IDB_DELETECLASS,IDB_DELETECLASS_DOWN,IDB_DELETECLASS); 
// 
// 	m_deletedepart.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_deletedepart.SetBitmapId(IDB_DELETEDEPART_DOWN,IDB_DELETEDEPART_DOWN,IDB_DELETEDEPART,IDB_DELETEDEPART_DOWN);
// 	
// 	m_updatedepart.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_updatedepart.SetBitmapId(IDB_UPDATEDEPART,IDB_UPDATEDEPART,IDB_UPDATEDEPART_DOWN,IDB_UPDATEDEPART);
// 	
// 	m_updateclass.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_updateclass.SetBitmapId(IDB_UPDATECLASS,IDB_UPDATECLASS,IDB_UPDATECLASS_DOWN,IDB_UPDATECLASS);
// 
// 	m_adddepart.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_adddepart.SetBitmapId(IDB_ADDDEPART,IDB_ADDDEPART,IDB_ADDDEPART_DOWN,IDB_ADDDEPART);
// 
// 	m_addclass.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_addclass.SetBitmapId(IDB_ADDCLASS,IDB_ADDCLASS,IDB_ADDCLASS_DOWN,IDB_ADDCLASS);
// 
// 	m_display.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_display.SetBitmapId(IDB_SHOWALL,IDB_SHOWALL,IDB_SHOWALL_DOWN,IDB_SHOWALL);
	

	AddSzControl(*GetDlgItem(IDC_ADDDEPART),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_ADDCLASS),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_UPDATEDEPART),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_UPDATECLASS),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_DISPLAY),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdResize,mdResize);
	AddSzControl(*GetDlgItem(IDC_STATIC2),mdResize,mdResize);
	AddSzControl(*GetDlgItem(IDC_DELETEDEPART),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_DELETECLASS),mdNone,mdRepos);	
	AddSzControl(m_departlist,mdResize,mdResize);
	AddSzControl(m_classlist,mdResize,mdResize);

	
    DWORD dwStyle=GetWindowLong(m_departlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_departlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_departlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_departlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_departlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_departlist.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );

	m_departlist.ClearHeadings();
	m_departlist.SetHeadings("院系编号,100;系名,150;系主任,100;联系电话,100");

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
	m_classlist.SetHeadings("班级编号,100;班级,150;所属院系,150;班主任,100;联系电话,100");

	return TRUE;
}
void CFormDepartment::showdata()
{
	m_departlist.DeleteAllItems();
	CoInitialize(NULL);
	CDBDepartment m_departmentdb;
	if (!m_departmentdb.Connect())
	{
		AfxMessageBox("departmentdb:无法连接到数据库服务器");
	}
	else
	{
		BOOL flag = m_departmentdb.LoadByAll();
		while (flag)
		{
			m_departlist.AddItem(0, m_departmentdb.m_departid, m_departmentdb.m_departname, m_departmentdb.m_persident, m_departmentdb.m_persidenttel);
			flag = m_departmentdb.Move();
		}
		m_departmentdb.Close();
	}

	CoUninitialize();

// 	CoInitialize(NULL);	
// 	CDBDepartment  m_Departmentdb;
// 	if ( !m_Departmentdb.Connect())
// 	{
// 		AfxMessageBox("Departmentdb:无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		BOOL flag = m_Departmentdb.LoadByAll();
// 		while(flag)
// 		{
// 			CString departid, depart, head, tel;
// 			departid = m_Departmentdb.m_departid;
// 			departid.TrimRight();
// 			departid.TrimLeft();
// 			depart = m_Departmentdb.m_depart;
// 			head = m_Departmentdb.m_departhead;
// 			tel = m_Departmentdb.m_departheadtel;
// 			BOOL bMark = TRUE;
// 			for(int i = 0; i < m_departlist.GetItemCount(); i++)
// 			{
// 				CString sTemp = m_departlist.GetItemText(i,0);
// 				sTemp.TrimLeft();
// 				sTemp.TrimRight();
// 				if(departid == sTemp)
// 				{
// 					bMark = FALSE;
// 					break;
// 				}
// 			}
// 			if(bMark)
// 				m_departlist.AddItem(0, departid, depart, head, tel);
// 			flag = m_Departmentdb.Move();
// 		}
// 		m_Departmentdb.Close();
// 	}
// 	CoUninitialize();
}


void CFormDepartment::OnDisplay() 
{
	showdata();
}

void CFormDepartment::OnAdddepart() 
{
	CDlgDpartment	dlg;
	if(dlg.DoModal() == IDOK)
	{
		CoInitialize(NULL);
		CDBDepartment m_departmentdb;
		if (!m_departmentdb.Connect())
		{
			AfxMessageBox("Departmentdb:无法连接到数据库服务器");
		}
		else
		{
			CString departid = dlg.m_departid;
			BOOL flag = m_departmentdb.LoadByDepartID(departid);
			if (!flag)
			{
				m_departmentdb.m_ydid = 0;
				m_departmentdb.m_departid = dlg.m_departid;
				m_departmentdb.m_departname = dlg.m_depart;
				m_departmentdb.m_persident = dlg.m_head;
				m_departmentdb.m_persidenttel = dlg.m_tel;
				if(m_departmentdb.Add())
					AfxMessageBox("添加成功！");
				else
					AfxMessageBox("添加失败！");
			}
			else
 			{
 				CString temp = "该院系编号已存在，无法添加进数据库！";
 				AfxMessageBox(temp);
 			}
		}

		CoUninitialize();
	}
	showdata();
}

void CFormDepartment::OnAddclass() 
{
	CDlgClassSet  dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString strDepart, strClass, strClassID;
		CString strDepartid = "";
		strDepart = dlg.strDepart;
		strDepart.TrimLeft();
		strDepart.TrimRight();
		strClass = dlg.m_class;
		strClass.TrimLeft();
		strClass.TrimRight();
		strClassID = dlg.m_classid;
		strClassID.TrimLeft();
		strClassID.TrimRight();
		CoInitialize(NULL);
		CDBDepartment m_departdb;
		if (!m_departdb.Connect())
		{
			AfxMessageBox("Departmentdb:无法连接到数据库服务器");
		}
		else
		{
			BOOL flag = m_departdb.LoadByDepart(strDepart);
			if (flag)
			{
				strDepartid = m_departdb.m_departid;
			}
			m_departdb.Close();
		}

		if (strDepartid != "")
		{
			CDBClass m_classdb;
			if (!m_classdb.Connect())
			{
				AfxMessageBox("m_classdb:无法连接到数据库服务器");
			}
			else
			{
				m_classdb.m_ydid = 0;
				m_classdb.m_departid = strDepartid;
				m_classdb.m_classid = strClassID;
				m_classdb.m_classname = strClass;
				m_classdb.m_head = dlg.m_classhead;
				m_classdb.m_headtel = dlg.m_classheadtel;
				if (m_classdb.Add())
				{
					AfxMessageBox("添加成功！");
				}
				else
					AfxMessageBox("添加失败！");
				m_classdb.Close();
			}
		}

		CoUninitialize();
// 		CoInitialize(NULL);
// 		CDBDepartment  m_Departmentdb;
// 		if ( !m_Departmentdb.Connect())
// 		{
// 			AfxMessageBox("Departmentdb:无法连接到数据库服务器");
// 			AfxMessageBox("添加失败！");
// 			CoUninitialize();
// 			return;
// 		}
// 		else
// 		{
// 			if((m_Departmentdb.LoadByClass(strClass))||(m_Departmentdb.LoadByClassID(strClassID)))
// 			{
// 				AfxMessageBox("班级编号或班级名称已存在，无法添加进数据库！");
// 			}
// 			else
// 			{
// 				BOOL flag = m_Departmentdb.LoadByDepart(strDepart);
// 				if(flag)
// 				{
// 					CString strDepartID = m_Departmentdb.m_departid;
// 					CString strDepart = m_Departmentdb.m_depart;
// 					CString strDepartHead = m_Departmentdb.m_departhead;
// 					CString strDepartHeadtel = m_Departmentdb.m_departheadtel;
// 
// 					m_Departmentdb.m_ydid = 0;
// 					m_Departmentdb.m_departid = strDepartID;
// 					m_Departmentdb.m_depart = strDepart;
// 					m_Departmentdb.m_departhead = strDepartHead;
// 					m_Departmentdb.m_departheadtel = strDepartHeadtel;
// 					m_Departmentdb.m_classid = strClassID;
// 					m_Departmentdb.m_class = strClass;
// 					m_Departmentdb.m_classhead = dlg.m_classhead;
// 					m_Departmentdb.m_classheadtel = dlg.m_classheadtel;
// 					if(m_Departmentdb.Add())
// 					{
// 						AfxMessageBox("添加成功！");
// 					}
// 					else
// 					{
// 						AfxMessageBox("添加失败！");
// 					}
// 				}
// 			}
// 			m_Departmentdb.Close();
// 		}
// 		CoUninitialize();
	}
}

void CFormDepartment::OnDblclkDepartmentlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int index =pNMListView->iItem;
	CString strDepartID = m_departlist.GetItemText(index,0);
	CString strDepart = m_departlist.GetItemText(index,1);
	strDepartID.TrimRight();
	strDepartID.TrimLeft();
	if(strDepartID != "")
	{
		m_classlist.DeleteAllItems();
		Sleep(200);
		CoInitialize(NULL);
		CDBClass m_classdb;
		if (!m_classdb.Connect())
		{
			AfxMessageBox("classdb:无法连接到数据库服务器");
		}
		else
		{
			BOOL flag = m_classdb.LoadByDepartid(strDepartID);
			while (flag)
			{
				m_classlist.AddItem(0, m_classdb.m_classid, m_classdb.m_classname, strDepart, m_classdb.m_head, m_classdb.m_headtel);
				flag = m_classdb.Move();
			}
			m_classdb.Close();
		}
		CoUninitialize();
// 		CoInitialize(NULL);	
// 		CDBDepartment  m_Departmentdb;
// 		if ( !m_Departmentdb.Connect())
// 		{
// 			AfxMessageBox("Departmentdb:无法连接到数据库服务器");
// 			CoUninitialize();
// 			return;
// 		}
// 		else
// 		{
// 			BOOL flag = m_Departmentdb.LoadByDepartID(strDepartID);
// 			while(flag)
// 			{
// 				CString strClass, strClassID,strClasshead, strClassheadtel, strDepart;
// 				strClass = m_Departmentdb.m_class;
// 				strClassID = m_Departmentdb.m_classid;
// 				strClasshead = m_Departmentdb.m_classhead;
// 				strClassheadtel = m_Departmentdb.m_classheadtel;
// 				strDepart = m_Departmentdb.m_depart;
// 				if(strClassID != "" || strClass != "")
// 					m_classlist.AddItem(0, strClassID, strClass, strDepart, strClasshead, strClassheadtel);
// 				flag = m_Departmentdb.Move();
// 			}
// 			m_Departmentdb.Close();
// 		}
// 		CoUninitialize();
	}
	
	*pResult = 0;
}

void CFormDepartment::OnUpdatedepart() 
{
	for(int i = 0; i < m_departlist.GetItemCount(); i++)
	{
		if(m_departlist.GetCheck(i))
		{
			CString strDepartID = m_departlist.GetItemText(i,0);
			CString strDepart = m_departlist.GetItemText(i,1);
			CString strDepartHead = m_departlist.GetItemText(i,2);
			CString strDepartHeadtel = m_departlist.GetItemText(i,3);
			CDlgDpartment	dlg;
			dlg.m_departid = strDepartID;
			dlg.m_depart = strDepart;
			dlg.m_head = strDepartHead;
			dlg.m_tel = strDepartHeadtel;
			if(dlg.DoModal() == IDOK)
			{
				CoInitialize(NULL);	
				CDBDepartment  m_Departmentdb;
				if (!m_Departmentdb.Connect())
				{
					AfxMessageBox("Departmentdb:无法连接到数据库服务器");
				}
				else
				{
					BOOL flag = m_Departmentdb.LoadByDepartID(strDepartID);
					if (flag)
					{

 						if(m_Departmentdb.Update(strDepartID, dlg.m_departid, dlg.m_depart, dlg.m_head, dlg.m_tel))
							AfxMessageBox("修改成功！");
 						else
							AfxMessageBox("修改失败！");		
					}
					m_Departmentdb.Close();
				}
				CoUninitialize();
// 				CoInitialize(NULL);	
// 				CDBDepartment  m_Departmentdb;
// 				if ( !m_Departmentdb.Connect())
// 				{
// 					AfxMessageBox("Departmentdb:无法连接到数据库服务器");
// 					CoUninitialize();
// 					return;
// 				}
// 				else
// 				{
// 					BOOL flag = m_Departmentdb.LoadByDepartID(strDepartID);
// 					if(flag)
// 					{
// 					//  m_Departmentdb.m_departid = dlg.m_departid;
// 					//	m_Departmentdb.m_depart = dlg.m_depart;
// 					//	m_Departmentdb.m_departhead = dlg.m_head;
// 					//	m_Departmentdb.m_departheadtel = dlg.m_tel;
// 						if(m_Departmentdb.Update(strDepartID, dlg.m_departid, dlg.m_depart, dlg.m_head, dlg.m_tel))
// 							AfxMessageBox("修改成功！");
// 						else
// 							AfxMessageBox("修改失败！");
// 					//	flag = m_Departmentdb.Move();
// 					}
// 					m_Departmentdb.Close();
// 				}
// 				CoUninitialize();
			}
			break;
		}  
	}
}

void CFormDepartment::OnUpdateclass() 
{
	for(int i = 0; i < m_classlist.GetItemCount(); i++)
	{
		if(m_classlist.GetCheck(i))
		{
			CString strClassID = m_classlist.GetItemText(i, 0);
			CString strClass = m_classlist.GetItemText(i, 1);
			CString strDepart = m_classlist.GetItemText(i, 2);
			CString strClasshead = m_classlist.GetItemText(i, 3);
			CString strClassHeadtel = m_classlist.GetItemText(i, 4);
			CDlgClassSet dlg;
			dlg.m_class = strClass;
			dlg.m_classid = strClassID;
			dlg.m_classhead = strClasshead;
			dlg.m_classheadtel = strClassHeadtel;
			if(dlg.DoModal() == IDOK)
			{
// 				CoInitialize(NULL);	
// 				CDBDepartment  m_Departmentdb;
// 				if ( !m_Departmentdb.Connect())
// 				{
// 					AfxMessageBox("Departmentdb:无法连接到数据库服务器");
// 					CoUninitialize();
// 					return;
// 				}
// 				else
// 				{
// 					CString strDepartID;
// 					BOOL flag1 = m_Departmentdb.LoadByDepart(dlg.strDepart);
// 					if(flag1)
// 					{
// 						strDepartID = m_Departmentdb.m_departid;
// 					}
// 					BOOL flag = m_Departmentdb.LoadByClassID(strClassID);
// 					if(flag)
// 					{
// 						m_Departmentdb.m_class = dlg.m_class;
// 						m_Departmentdb.m_classid = dlg.m_classid;
// 						m_Departmentdb.m_depart = dlg.strDepart;
// 						m_Departmentdb.m_departid = strDepartID;
// 						m_Departmentdb.m_classhead = dlg.m_classhead;
// 						m_Departmentdb.m_classheadtel= dlg.m_classheadtel;
// 						if(m_Departmentdb.Update(m_Departmentdb.m_ydid))
// 							AfxMessageBox("修改成功！");
// 						else
// 							AfxMessageBox("修改失败！");
// 					}
// 					m_Departmentdb.Close();
// 				}
// 				CoUninitialize();
			}
			break;

		}
	}
}

void CFormDepartment::OnDeletedepart() 
{
	CString msg;
	msg.Format("删除选中的院系，将会同时删除该院系中的班级\n确定继续吗？");
	if ( MessageBox(msg,"确认",MB_OKCANCEL) != IDOK)
		return;	
	
	CoInitialize(NULL);
	CDBDepartment m_departmentdb;
	if (!m_departmentdb.Connect())
	{
		AfxMessageBox("Departmentdb:无法连接到数据库服务器");
	}
	else
	{
		CDBClass m_classdb;
		if (!m_classdb.Connect())
		{
			AfxMessageBox("Departmentdb:无法连接到数据库服务器");
		}
		else
		{
			for(int i = 0; i < m_departlist.GetItemCount(); i++)
			{
				if(m_departlist.GetCheck(i))
				{
					CString strDepartID = m_departlist.GetItemText(i, 0);
	 				strDepartID.TrimRight();
 					strDepartID.TrimLeft();
					if (m_departmentdb.DeleteDepart(strDepartID))
					{
						CString sTemp = "院系："+m_departlist.GetItemText(i, 1)+"删除成功！";
	 					AfxMessageBox(sTemp);
					}
					else
					{
	 					CString sTemp = "院系："+m_departlist.GetItemText(i, 1)+"删除失败！";
						AfxMessageBox(sTemp);
					}

					if (m_classdb.DeleteClassByDepartID(strDepartID))
					{
						AfxMessageBox("班级同时删除成功！");
					}
				}
			}

			m_classdb.Close();
		}
		m_departmentdb.Close();
	}
	CoUninitialize();
	showdata();
// 	CoInitialize(NULL);	
// 	CDBDepartment  m_Departmentdb;
// 	if ( !m_Departmentdb.Connect())
// 	{
// 		AfxMessageBox("Departmentdb:无法连接到数据库服务器");
// 		CoUninitialize();
// 		return;
// 	}
// 	else
// 	{
// 		for(int i = 0; i < m_departlist.GetItemCount(); i++)
// 		{
// 			if(m_departlist.GetCheck(i))
// 			{
// 				CString strDepartID = m_departlist.GetItemText(i, 0);
// 				strDepartID.TrimRight();
// 				strDepartID.TrimLeft();
// 				if(m_Departmentdb.DeleteDepart(strDepartID))
// 				{
// 					CString sTemp = "院系："+m_departlist.GetItemText(i, 1)+"删除成功！";
// 					AfxMessageBox(sTemp);
// 				}
// 				else
// 				{
// 					CString sTemp = "院系："+m_departlist.GetItemText(i, 1)+"删除失败！";
// 					AfxMessageBox(sTemp);
// 				}
// 			}
// 		}
// 		m_Departmentdb.Close();
// 	}
// 	CoUninitialize();
}

void CFormDepartment::OnDeleteclass() 
{
	CString msg;
	msg.Format("确定要删除选择的班级吗？");
	if ( MessageBox(msg,"确认",MB_OKCANCEL) != IDOK)
		return;	
	CoInitialize(NULL);	
	CDBClass m_classdb;
	if (!m_classdb.Connect())
	{
		AfxMessageBox("m_classdb:无法连接到数据库服务器");
	}
	else
	{
		for(int i = 0; i < m_classlist.GetItemCount(); i++)
		{
			if (m_classlist.GetCheck(i))
			{
				CString strClassID = m_classlist.GetItemText(i, 0);
				if (m_classdb.DeleteClassByclassid(strClassID))
				{
					CString sTemp = "班级："+m_classlist.GetItemText(i, 1)+"删除成功！";
 					AfxMessageBox(sTemp);
				}
				else
				{
					CString sTemp = "班级："+m_classlist.GetItemText(i, 1)+"删除失败！";
 					AfxMessageBox(sTemp);
				}
			}
		}
		m_classdb.Close();
	}

	CoUninitialize();
// 	CoInitialize(NULL);	
// 	CDBDepartment  m_Departmentdb;
// 	if ( !m_Departmentdb.Connect())
// 	{
// 		AfxMessageBox("Departmentdb:无法连接到数据库服务器");
// 		CoUninitialize();
// 		return;
// 	}
// 	else
// 	{
// 		for(int i = 0; i < m_classlist.GetItemCount(); i++)
// 		{
// 			if(m_classlist.GetCheck(i))
// 			{
// 				CString strClassID = m_classlist.GetItemText(i, 0);
// 				strClassID.TrimLeft();
// 				strClassID.TrimRight();
// 				if(m_Departmentdb.DeleteClass(strClassID))
// 				{
// 					CString sTemp = "班级："+m_classlist.GetItemText(i, 1)+"删除成功！";
// 					AfxMessageBox(sTemp);
// 				}
// 				else
// 				{
// 					CString sTemp = "班级："+m_classlist.GetItemText(i, 1)+"删除失败！";
// 					AfxMessageBox(sTemp);
// 				}
// 			}
// 		}
// 		m_Departmentdb.Close();
// 	}
// 	CoUninitialize();
}
CString CFormDepartment::GetExePath()
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
void CFormDepartment::OnPaint() 
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
