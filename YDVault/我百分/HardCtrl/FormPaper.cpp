#include "stdafx.h"
#include "hardctrl.h"
#include "FormPaper.h"
#include "MainFrm.h"
#include "DlgPaperset.h"
#include "DlgPaperQuery.h"

extern CHardCtrlApp theApp;

IMPLEMENT_DYNCREATE(CFormPaper, CDialog)


CFormPaper::CFormPaper(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormPaper::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormPaper)
	//}}AFX_DATA_INIT
}


void CFormPaper::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormPaper)
	DDX_Control(pDX, IDC_DELETEPAPER, m_delete);
	DDX_Control(pDX, IDC_SHOWALL, m_showall);
	DDX_Control(pDX, IDC_QUERYPAPER, m_query);
	DDX_Control(pDX, IDC_ADD, m_add);
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormPaper, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormPaper)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_SHOWALL, OnShowall)
	ON_BN_CLICKED(IDC_QUERYPAPER, OnQuerypaper)
	ON_BN_CLICKED(IDC_DELETEPAPER, OnDeletepaper)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormPaper message handlers

BOOL CFormPaper::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();

// 	CRect btnRect;  
// 	m_show.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_show.SetBitmapId(IDB_SHOWALL,IDB_SHOWALL,IDB_SHOWALL_DOWN,IDB_SHOWALL); 	
// 
// 	m_query.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_query.SetBitmapId(IDB_QUERY1,IDB_QUERY1,IDB_UPDATE_DOWN,IDB_QUERY1); 
// 	
// 	m_delete.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_delete.SetBitmapId(IDB_DELETE,IDB_DELETE,IDB_QUERY_DOWN1,IDB_DELETE); 
// 
// 	m_add.GetWindowRect(btnRect); 
// 	ScreenToClient(btnRect); 
// 	m_add.SetBitmapId(IDB_ADD,IDB_ADD,IDB_ADD_DOWN,IDB_ADD); 

	AddSzControl(*GetDlgItem(IDC_ADD),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_SHOWALL),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_QUERYPAPER),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_DELETEPAPER),mdNone,mdRepos);	
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
	m_list.SetHeadings("考卷名称,100;考试科目,100;考试班级,100;开考时间,150;收卷试卷,150;开始题号,100;结束题号,100;最大题号,100");  	
	
	return TRUE;
}

void CFormPaper::OnAdd() 
{
	CDlgPaperset dlg;
	if(dlg.DoModal() == IDOK)
	{
		CoInitialize(NULL);
		CDBPaper       m_Paperdb;
		if ( !m_Paperdb.Connect())
		{
			AfxMessageBox("Paperdb: 无法连接到数据库服务器");
		}
		else
		{
			CString strSubjectid, strClassid;
			CString strSubject = dlg.strSubject;
			CString strClass = dlg.strClass;
			CDBSubject m_subjectdb;
			if (!m_subjectdb.Connect())
				AfxMessageBox("subjectdb: 无法连接到数据库服务器");
			else
			{
				CDBClass m_classdb;
				if (!m_classdb.Connect())
					AfxMessageBox("classdb: 无法连接到数据库服务器");
				else
				{
					BOOL subjectflag = m_subjectdb.LoadBySubject(strSubject);
					if (subjectflag)
					{
						strSubjectid = m_subjectdb.m_subjectid;
					}

					BOOL classflag = m_classdb.LoadByClassname(strClass);
					if (classflag)
					{
						strClassid = m_classdb.m_classid;
					}
				
					m_Paperdb.m_ydid = 0;
					m_Paperdb.m_papername = dlg.strPaper;
					m_Paperdb.m_subjectid = strSubjectid;
					m_Paperdb.m_classid = strClassid;
					m_Paperdb.m_starttime = dlg.tStart;
					m_Paperdb.m_endtime = dlg.tEnd;
					m_Paperdb.m_startNO = dlg.m_startNO;
					m_Paperdb.m_endNO = dlg.m_endNO;
					m_Paperdb.m_maxNO = dlg.m_maxNO;

					if(m_Paperdb.Add())
					{
						AfxMessageBox("添加成功！");
						CString tep1,tep2,tep3;
						tep1.Format("%d",dlg.m_startNO);
						tep2.Format("%d",dlg.m_endNO);
						tep3.Format("%d",dlg.m_maxNO);
						m_list.AddItem(0,dlg.strPaper,
							dlg.strSubject,
							dlg.strClass,
							dlg.tStart.Format(),
							dlg.tEnd.Format(),
							tep1,tep2,tep3);
					}
					else
						AfxMessageBox("添加失败！");
					
					m_classdb.Close();
				}
				m_subjectdb.Close();
			}
			m_Paperdb.Close();
		}
		CoUninitialize();
	}
}

void CFormPaper::OnShowall() 
{
	m_list.DeleteAllItems();
	CoInitialize(NULL);
	CDBPaper       m_Paperdb;
	CDBSubject     m_subjectdb;
	CDBClass       m_classdb;
	if ( !m_Paperdb.Connect())
	{
		AfxMessageBox("Paperdb: 无法连接到数据库服务器");
	}	
	else
	{
		if ((!m_subjectdb.Connect()) || (!m_classdb.Connect()))
		{
			AfxMessageBox("m_subjectdb: 无法连接到数据库服务器");
		}
		else
		{
			BOOL paperflag = m_Paperdb.LoadByAll();
			while (paperflag)
			{
				CString tep1,tep2,tep3,strSubject,strclass;
				tep1.Format("%d",m_Paperdb.m_startNO);
				tep2.Format("%d",m_Paperdb.m_endNO);
				tep3.Format("%d",m_Paperdb.m_maxNO);
				CString strSubjectid = m_Paperdb.m_subjectid;
				CString strClassid = m_Paperdb.m_classid;
				BOOL subjectflag = m_subjectdb.LoadBySubjectid(strSubjectid);
				if (subjectflag)
				{
					strSubject = m_subjectdb.m_subject;
				}
				BOOL classflag = m_classdb.LoadByClassid(strClassid);
				if (classflag)
				{
					strclass = m_classdb.m_classname;
				}
				m_list.AddItem(0,m_Paperdb.m_papername,
								strSubject,
								strclass,
								m_Paperdb.m_starttime.Format(),
								m_Paperdb.m_endtime.Format(),
								tep1,tep2,tep3);
				paperflag = m_Paperdb.Move();
			}
			m_classdb.Close();
			m_subjectdb.Close();
		}
		m_Paperdb.Close();
	}
	CoUninitialize();
}

void CFormPaper::OnQuerypaper() 
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
// 			AfxMessageBox("Paperdb: 无法连接到数据库服务器");
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
// 				//AfxMessageBox(m_Paperdb.m_starttime.Format());
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

void CFormPaper::OnDeletepaper() 
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
		AfxMessageBox("没有选中试卷，请在需要删除的试卷前打勾！");
		return;
	}	
	CString msg;
	msg.Format("确定要删除选中的内容吗？\n删除此试卷将同时删除该份试卷的标准答案和学生答案，确定继续？");
	if ( MessageBox(msg,"确认",MB_OKCANCEL) != IDOK)
		return;	

	CoInitialize(NULL);
	CDBPaper m_paperdb;
	if (!m_paperdb.Connect())
		AfxMessageBox("Paperdb: 无法连接到数据库服务器");
	else
	{
		for(int i = 0; i < m_list.GetItemCount(); i++)
		{
			if(m_list.GetCheck(i))
			{
				CString strPapername = m_list.GetItemText(i,0);
				strPapername.TrimRight();
				strPapername.TrimLeft();
				BOOL flag = m_paperdb.LoadByPaperName(strPapername);
				if (flag)
				{
					int m_paperid = m_paperdb.m_ydid;
					if(!m_paperdb.Delete(m_paperid))
					{
						CString temp = "试卷："+strPapername+"删除失败！";
						AfxMessageBox(temp);
 					}
					else
						AfxMessageBox("删除成功！");
				}
			}
		}
		OnShowall();
		m_paperdb.Close();
	}
	CoUninitialize();


// 	CoInitialize(NULL);
// 	CDBPaper       m_Paperdb;
// 	CDBStdanswer   m_Stdanswerdb;
// 	CDBResult      m_Resultdb;
// 	if ( !m_Paperdb.Connect())
// 	{
// 		AfxMessageBox("Paperdb: 无法连接到数据库服务器");
// 	}
// 	if ( !m_Stdanswerdb.Connect())
// 	{
// 		AfxMessageBox("Stdanswerdb: 无法连接到数据库服务器");
// 	}
// 	if ( !m_Resultdb.Connect())
// 	{
// 		AfxMessageBox("Resultdb: 无法连接到数据库服务器");
// 	}
// 	else
// 	{
// 		for(int i = 0; i < m_list.GetItemCount(); i++)
// 		{
// 			if(m_list.GetCheck(i))
// 			{
// 				CString sSubject = m_list.GetItemText(i, 1);
// 				CString sClass = m_list.GetItemText(i, 3);
// 				CString sTime = m_list.GetItemText(i, 4);
// 				sTime.TrimLeft();
// 				sTime.TrimRight();
// 				sSubject.TrimLeft();
// 				sSubject.TrimRight();
// 				sClass.TrimLeft();
// 				sClass.TrimRight();
// 				BOOL flag = m_Paperdb.LoadByPaper(sSubject, sClass, sTime);
// 				if(flag)
// 				{
// 					int m_paperid = m_Paperdb.m_ydid;
// 					if(!m_Paperdb.Delete(m_paperid))
// 					{
// 						CString temp = "试卷："+m_Paperdb.m_papername+"删除失败！";
// 						AfxMessageBox(temp);
// 					}
// 					BOOL flag1 = m_Stdanswerdb.LoadByPaperID(m_paperid);
// 					if(flag1)
// 					{
// 						if(!m_Stdanswerdb.Deletepaper(m_paperid))
// 							AfxMessageBox("删除标准答案失败！");
// 					}
// 					BOOL flag2 = m_Resultdb.LoadByPaperID(m_paperid);
// 					if(flag2)
// 					{
// 						if(!m_Resultdb.Deletepaper(m_paperid))
// 							AfxMessageBox("删除学生答案失败！");
// 					}
// 				}
// 			}
// 		}
// 		AfxMessageBox("删除成功！");
// 		int iSize = m_list.GetItemCount();
// 		for(i = 0; i < iSize; i++)
// 		{
// 			BOOL flag = m_Paperdb.LoadByPaper(m_list.GetItemText(i, 1), m_list.GetItemText(i, 3), m_list.GetItemText(i, 4));
// 			if(flag)
// 			{
// 				CString tep1,tep2,tep3;
// 				tep1.Format("%d",m_Paperdb.m_startNO);
// 				tep2.Format("%d",m_Paperdb.m_endNO);
// 				tep3.Format("%d",m_Paperdb.m_maxNO);
// 				m_list.AddItem(0,m_Paperdb.m_papername,m_Paperdb.m_subject,m_Paperdb.m_depart,
// 								m_Paperdb.m_class,m_Paperdb.m_starttime.Format(),
// 								m_Paperdb.m_endtime.Format(),tep1,tep2,tep3);
// 			}
// 		}
// 		for(i = 0; i < iSize; i++)
// 		{
// 			m_list.DeleteItem(0);
// 		}
// 		m_Paperdb.Close();
// 	}
// 	CoUninitialize();
}
CString CFormPaper::GetExePath()
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
void CFormPaper::OnPaint() 
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
