#include "stdafx.h"
#include "hardctrl.h"
#include "FormStuinfoSet.h"
#include "MainFrm.h"
#include "DlgStunfo.h"
#include "DlgStuQuery.h"
#include "DlgPhoto.h"
#include "DBClass.h"
#include "DBDepartment.h"

extern CHardCtrlApp theApp;

IMPLEMENT_DYNCREATE(CFormStuinfoSet, CDialog)

CFormStuinfoSet::CFormStuinfoSet(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormStuinfoSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormStuinfoSet)
	//}}AFX_DATA_INIT
}


void CFormStuinfoSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormStuinfoSet)
	DDX_Control(pDX, IDC_UPDATESTUD, m_update);
	DDX_Control(pDX, IDC_QUERYSTUD, m_querystud);
	DDX_Control(pDX, IDC_DELETESTUD, m_delete);
	DDX_Control(pDX, IDC_ADDSTUD, m_add);
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormStuinfoSet, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormStuinfoSet)
	ON_BN_CLICKED(IDC_QUERYSTUD, OnQuerystud)
	ON_BN_CLICKED(IDC_ADDSTUD, OnAddstud)
	ON_BN_CLICKED(IDC_DELETESTUD, OnDeletestud)
	ON_BN_CLICKED(IDC_UPDATESTUD, OnUpdatestud)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormStuinfoSet message handlers

BOOL CFormStuinfoSet::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();   

// 	CRect btnRect;  
// 	m_querystud.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_querystud.SetBitmapId(IDB_QUERY1,IDB_QUERY1,IDB_QUERY_DOWN1,IDB_QUERY1); 	
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

	AddSzControl(*GetDlgItem(IDC_UPDATESTUD),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_DELETESTUD),mdNone,mdRepos);
    AddSzControl(*GetDlgItem(IDC_ADDSTUD),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_QUERYSTUD),mdNone,mdRepos);
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
	m_list.SetHeadings("学号,100;姓名,150;性别,50;院系,100;班级,100;电话,100;电子邮箱,150;学生机蓝牙地址,150");	
	
	return TRUE;  
}

void CFormStuinfoSet::OnQuerystud() 
{
	CDlgStuQuery dlg;
	if(dlg.DoModal() == IDOK)
	{
		CoInitialize(NULL);
		CDBStuInfo  m_StuInfodb;	
		if ( !m_StuInfodb.Connect())
		{
			AfxMessageBox("StuInfodb:无法连接到数据库服务器");
		}
		else
		{
			m_list.DeleteAllItems();
			if(dlg.m_stuid != "")
			{
				BOOL flag = m_StuInfodb.LoadByStuid(dlg.m_stuid);
				if(flag)
				{
					CString strDepart = "";
					CString strClass = "";
					CString strDepartid = m_StuInfodb.m_studeparid;
					CString strClassid = m_StuInfodb.m_stuclassid;
					CDBDepartment m_departdb;
					if (!m_departdb.Connect())
					{
						AfxMessageBox("m_departdb:无法连接到数据库服务器");
					}
					else
					{
						BOOL flag = m_departdb.LoadByDepartID(strDepartid);
						if (flag)
						{
							strDepart = m_departdb.m_departname;
						}
						m_departdb.Close();
					}

					CDBClass m_classdb;
					if (!m_classdb.Connect())
					{
						AfxMessageBox("m_classdb:无法连接到数据库服务器");
					}
					else
					{
						BOOL flag = m_classdb.LoadByClassid(strClassid);
						if (flag)
						{
							strClass = m_classdb.m_classname;
						}
						m_classdb.Close();
					}

					m_list.AddItem(0,
									m_StuInfodb.m_stuid,
									m_StuInfodb.m_stuname,
									m_StuInfodb.m_stugender,
									strDepart,
									strClass,
									m_StuInfodb.m_stutel,
									m_StuInfodb.m_stuemail,
									m_StuInfodb.m_devid);
				}
			}
			else if(dlg.m_stuname != "")
			{
				BOOL flag = m_StuInfodb.LoadByStuname(dlg.m_stuname);
				if(flag)
				{
					CString strDepart = "";
					CString strClass = "";
					CString strDepartid = m_StuInfodb.m_studeparid;
					CString strClassid = m_StuInfodb.m_stuclassid;
					CDBDepartment m_departdb;
					if (!m_departdb.Connect())
					{
						AfxMessageBox("m_departdb:无法连接到数据库服务器");
					}
					else
					{
						BOOL flag = m_departdb.LoadByDepartID(strDepartid);
						if (flag)
						{
							strDepart = m_departdb.m_departname;
						}
						m_departdb.Close();
					}
					
					CDBClass m_classdb;
					if (!m_classdb.Connect())
					{
						AfxMessageBox("m_classdb:无法连接到数据库服务器");
					}
					else
					{
						BOOL flag = m_classdb.LoadByClassid(strClassid);
						if (flag)
						{
							strClass = m_classdb.m_classname;
						}
						m_classdb.Close();
					}
					
					m_list.AddItem(0,
						m_StuInfodb.m_stuid,
						m_StuInfodb.m_stuname,
						m_StuInfodb.m_stugender,
						strDepart,
						strClass,
						m_StuInfodb.m_stutel,
						m_StuInfodb.m_stuemail,
									m_StuInfodb.m_devid);
				}
			}
			else
			{
				CString strClass = dlg.strClass;
				CString strClassid = "";
				CString strDepartid = "";
				CString strDepart = "";
				CDBClass m_classdb;
				if (!m_classdb.Connect())
				{
					AfxMessageBox("m_classdb:无法连接到数据库服务器");
				}
				else
				{
					BOOL flag = m_classdb.LoadByClassname(strClass);
					if (flag)
					{
						strClassid = m_classdb.m_classid;
						strDepartid = m_classdb.m_departid;
					}
					m_classdb.Close();
				}

				if (strDepartid != "")
				{
					CDBDepartment m_departdb;
					if (!m_departdb.Connect())
					{
						AfxMessageBox("m_departdb:无法连接到数据库服务器");
					}
					else
					{
						BOOL flag = m_departdb.LoadByDepartID(strDepartid);
						if (flag)
						{
							strDepart = m_departdb.m_departname;
						}
						m_departdb.Close();
					}
				}

				if (strClassid != "")
				{
					BOOL flag = m_StuInfodb.LoadByClassid(strClassid);
					while (flag)
					{
						m_list.AddItem(0,
										m_StuInfodb.m_stuid,
										m_StuInfodb.m_stuname,
										m_StuInfodb.m_stugender,
										strDepart,
										strClass,
										m_StuInfodb.m_stutel,
										m_StuInfodb.m_stuemail,
	 									m_StuInfodb.m_devid);
						flag = m_StuInfodb.Move();
					}
					
				}

//				BOOL flag = m_StuInfodb.LoadByClass(dlg.strClass);
//				while(flag)
//				{
// 					m_list.AddItem(0,
// 									m_StuInfodb.m_stuid,
// 									m_StuInfodb.m_stuname,
// 									m_StuInfodb.m_Stugender,
// 									m_StuInfodb.m_Studepartment,
// 									m_StuInfodb.m_Stuclass,
// 									m_StuInfodb.m_stutel,
// 									m_StuInfodb.m_stuemail,
// 									m_StuInfodb.m_devid);					
//					flag = m_StuInfodb.Move();
//				}
			}
			m_StuInfodb.Close();
		}
		CoUninitialize();
	}
//	if(m_list.GetItemCount() == 0)
//		AfxMessageBox("没有查询到相应内容！");
	
}

void CFormStuinfoSet::OnAddstud() 
{
	CDlgStunfo  dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString strStuID = dlg.m_num;
		strStuID.TrimLeft();
		strStuID.TrimRight();
		CoInitialize(NULL);
		CDBStuInfo  m_StuInfodb;	
		if ( !m_StuInfodb.Connect())
		{
			AfxMessageBox("StuInfodb:无法连接到数据库服务器");
		}
		else
		{
			if(m_StuInfodb.LoadByStuid(strStuID))
				AfxMessageBox("学号已存在，添加失败！");
			else
			{
				CString strDepart = dlg.strDepart;
				CString strClass = dlg.strClass;
				CString strDepartid, strClassid;
				CDBClass m_classdb;
				if (!m_classdb.Connect())
				{
					AfxMessageBox("m_classdb:无法连接到数据库服务器");
				}
				else
				{
					BOOL flag = m_classdb.LoadByClassname(strClass);
					if (flag)
					{
						strDepartid = m_classdb.m_departid;
						strClassid = m_classdb.m_classid;
					}
					m_classdb.Close();
				}

				m_StuInfodb.m_ydid = 0;
				m_StuInfodb.m_stuid = dlg.m_num;
				m_StuInfodb.m_stuname = dlg.m_name;
				m_StuInfodb.m_stugender = dlg.strSex;
				m_StuInfodb.m_studeparid = strDepartid;
				m_StuInfodb.m_stuclassid = strClassid;
				m_StuInfodb.m_stutel = dlg.m_tel;
				m_StuInfodb.m_stuemail = dlg.m_email;
				m_StuInfodb.m_stuphoto = dlg.strPath;
				m_StuInfodb.m_devid = dlg.m_blueteeth;

				if(m_StuInfodb.Add())
					AfxMessageBox("添加成功！");
				else
					AfxMessageBox("添加失败！");
			}


// 			BOOL flag = m_StuInfodb.LoadByStuid(strStuID);
// 			if(flag)
// 			{
// 				m_list.AddItem(0,
// 								m_StuInfodb.m_stuid,
// 								m_StuInfodb.m_stuname,
// 								m_StuInfodb.m_Stugender,
// 								m_StuInfodb.m_Studepartment,
// 								m_StuInfodb.m_Stuclass,
// 								m_StuInfodb.m_stutel,
// 								m_StuInfodb.m_stuemail,
// 								m_StuInfodb.m_devid);
// 			}
			m_StuInfodb.Close();
		}
		CoUninitialize();
	}
}

void CFormStuinfoSet::OnDeletestud() 
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
		AfxMessageBox("没有选中学生，请在需要删除的学生前打勾！");
		return;
	}	
	CString msg;
	msg.Format("确认删除选中的学生？");
	if ( MessageBox(msg,"确认",MB_OKCANCEL) != IDOK)
		return;	

// 	CoInitialize(NULL);	
// 	CDBStuInfo  m_StuInfodb;
// 	if ( !m_StuInfodb.Connect())
// 	{
// 		AfxMessageBox("StuInfodb:无法连接到数据库服务器，删除失败！");
// 	}
// 	else
// 	{
// 		for(int i = 0; i < m_list.GetItemCount(); i++)
// 		{
// 			if(m_list.GetCheck(i))
// 			{
// 				CString strStuID = m_list.GetItemText(i, 0);
// 				strStuID.TrimLeft();
// 				strStuID.TrimRight();
// 				BOOL flag = m_StuInfodb.LoadByStuid(strStuID);
// 				if(flag)
// 				{
// 					if(!m_StuInfodb.Delete(m_StuInfodb.m_ydid))
// 					{
// 						CString strTemp = "学号为："+strStuID+"删除失败！";
// 						AfxMessageBox(strTemp);
// 					}
// 				}
// 			}
// 		}
// 		AfxMessageBox("删除成功！");
// 		int iSize = m_list.GetItemCount();
// 		for(i = 0; i < iSize; i++)
// 		{
// 			BOOL flag = m_StuInfodb.LoadByStuid(m_list.GetItemText(i, 0));
// 			if(flag)
// 			{
// 				m_list.AddItem(0,
// 								m_StuInfodb.m_stuid,
// 								m_StuInfodb.m_stuname,
// 								m_StuInfodb.m_Stugender,
// 								m_StuInfodb.m_Studepartment,
// 								m_StuInfodb.m_Stuclass,
// 								m_StuInfodb.m_stutel,
// 								m_StuInfodb.m_stuemail,
// 								m_StuInfodb.m_devid);
// 			}
// 		}
// 		for(i = 0; i < iSize; i++)
// 		{
// 			m_list.DeleteItem(0);
// 		}
// 		m_StuInfodb.Close();
// 	}
// //	CoUninitialize();
	refurbish();
}

void CFormStuinfoSet::OnUpdatestud() 
{
	CDlgStunfo  dlg;
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
		AfxMessageBox("没有选中学生，请在需要修改的学生前打勾！");
		return;
	}


	CoInitialize(NULL);
	CDBStuInfo  m_StuInfodb;	
	if ( !m_StuInfodb.Connect())
	{
		AfxMessageBox("StuInfodb:无法连接到数据库服务器");
	}
	else
	{
		for(int i = 0; i < m_list.GetItemCount(); i++)
		{
			if(m_list.GetCheck(i))
			{
				dlg.m_num = m_list.GetItemText(i,0);
				dlg.m_name = m_list.GetItemText(i,1);
				dlg.strSex = m_list.GetItemText(i,2);
				dlg.strDepart = m_list.GetItemText(i,3);
				dlg.strClass = m_list.GetItemText(i,4);
				dlg.m_tel = m_list.GetItemText(i,5);
				dlg.m_email = m_list.GetItemText(i,6);
				dlg.m_blueteeth = m_list.GetItemText(i,7);
				BOOL flag = m_StuInfodb.LoadByStuid(m_list.GetItemText(i,0));
				if(flag)
					dlg.strPath = m_StuInfodb.m_stuphoto;
				if(dlg.DoModal() == IDOK)
				{
					CString strDepart = dlg.strDepart;
					CString strclass = dlg.strClass;
					CString strDepartid = "";
					CString strClassid = "";
					CDBClass m_classdb;
					if (!m_classdb.Connect())
					{
						AfxMessageBox("m_classdb:无法连接到数据库服务器");
					}
					else
					{
						BOOL classflag = m_classdb.LoadByClassname(strclass);
						if (classflag)
						{
							strDepartid = m_classdb.m_departid;
							strClassid = m_classdb.m_classid;
						}
						m_classdb.Close();
					}
					m_StuInfodb.m_stuid = dlg.m_num;
					m_StuInfodb.m_stuname = dlg.m_name;
					m_StuInfodb.m_stugender = dlg.strSex;
					m_StuInfodb.m_studeparid = strDepartid;
					m_StuInfodb.m_stuclassid = strClassid;
					m_StuInfodb.m_stutel = dlg.m_tel;
					m_StuInfodb.m_stuemail = dlg.m_email;
					m_StuInfodb.m_devid = dlg.m_blueteeth;
					m_StuInfodb.m_stuphoto = dlg.strPath;
					if(m_StuInfodb.Update(m_StuInfodb.m_ydid))
						AfxMessageBox("修改成功！");
					else
						AfxMessageBox("修改失败！");

				}
// 				m_list.SetItemText(i, 0, m_StuInfodb.m_stuid);
// 				m_list.SetItemText(i, 1, m_StuInfodb.m_stuname);
// 				m_list.SetItemText(i, 2, m_StuInfodb.m_stugender);
// 				m_list.SetItemText(i, 3, m_StuInfodb.m_Studepartment);
// 				m_list.SetItemText(i, 4, m_StuInfodb.m_Stuclass);
// 				m_list.SetItemText(i, 5, m_StuInfodb.m_stutel);
// 				m_list.SetItemText(i, 6, m_StuInfodb.m_stuemail);
// 				m_list.SetItemText(i, 7, m_StuInfodb.m_devid);
			}
		}
		m_StuInfodb.Close();
	}
	CoUninitialize();
}
void CFormStuinfoSet::shownew(CString strStuID)
{
// 	CoInitialize(NULL);	
// 	CDBStuInfo  m_StuInfodb;
// 	if ( !m_StuInfodb.Connect())
// 	{
// 		AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		BOOL flag = m_StuInfodb.LoadByStuid(strStuID);
// 		if(flag)
// 		{
// 			m_list.AddItem(0,
// 							m_StuInfodb.m_stuid,
// 							m_StuInfodb.m_stuname,
// 							m_StuInfodb.m_Stugender,
// 							m_StuInfodb.m_Studepartment,
// 							m_StuInfodb.m_Stuclass,
// 							m_StuInfodb.m_stutel,
// 							m_StuInfodb.m_stuemail,
// 							m_StuInfodb.m_devid);
// 		}
// 		m_StuInfodb.Close();
// 	}
// 	CoUninitialize();
}

void CFormStuinfoSet::refurbish()
{
// 	CoInitialize(NULL);	
// 	CDBStuInfo  m_StuInfodb;
// 	if ( !m_StuInfodb.Connect())
// 	{
// 		AfxMessageBox("StuInfodb:无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		int iSize = m_list.GetItemCount();
// 		for(int i = 0; i < iSize; i++)
// 		{
// 			BOOL flag = m_StuInfodb.LoadByStuid(m_list.GetItemText(i, 0));
// 			if(flag)
// 			{
// 				m_list.AddItem(0,
// 								m_StuInfodb.m_stuid,
// 								m_StuInfodb.m_stuname,
// 								m_StuInfodb.m_Stugender,
// 								m_StuInfodb.m_Studepartment,
// 								m_StuInfodb.m_Stuclass,
// 								m_StuInfodb.m_stutel,
// 								m_StuInfodb.m_stuemail,
// 								m_StuInfodb.m_devid);
// 			}
// 		}
// 		for(i = 0; i < iSize; i++)
// 		{
// 			m_list.DeleteItem(0);
// 		}
// 		m_StuInfodb.Close();
// 	}
// 	CoUninitialize();
}

void CFormStuinfoSet::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item =pNMListView->iItem;
	CString strStuID = m_list.GetItemText(item,0);
	if(strStuID != "")
	{
// 		CoInitialize(NULL);
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
CString CFormStuinfoSet::GetExePath()
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
void CFormStuinfoSet::OnPaint() 
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
