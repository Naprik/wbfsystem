#include "stdafx.h"
#include "hardctrl.h"
#include "FormExamSet.h"
#include "MainFrm.h"
#include "DlgPaperQuery.h"
#include "DlgPaperset.h"
#include "ExamDetail.h"
#include "DlgTeacherSet.h"

extern CHardCtrlApp theApp;
extern CExamDetail  m_examdeteil;

IMPLEMENT_DYNCREATE(CFormExamSet, CDialog)

CFormExamSet::CFormExamSet(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormExamSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormExamSet)
	//}}AFX_DATA_INIT
}


void CFormExamSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamSet)
	DDX_Control(pDX, IDC_UP, m_up);
	DDX_Control(pDX, IDC_SHOWALL, m_showall);
	DDX_Control(pDX, IDC_SET, m_set);
	DDX_Control(pDX, IDC_PAPERQUERY, m_paperquery);
	DDX_Control(pDX, IDC_NEXT, m_next);
	DDX_Control(pDX, IDC_ADDPAPER, m_addpaper);
	DDX_Control(pDX, IDC_PAPERLIST, m_paperlist);
	DDX_Control(pDX, IDC_CLASSLIST, m_classlist);
	DDX_Control(pDX, IDC_ADDR, m_address);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamSet, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormExamSet)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_SHOWALL, OnShowall)
	ON_BN_CLICKED(IDC_PAPERQUERY, OnPaperquery)
	ON_BN_CLICKED(IDC_ADDPAPER, OnAddpaper)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_NOTIFY(NM_DBLCLK, IDC_PAPERLIST, OnDblclkPaperlist)
	ON_NOTIFY(NM_DBLCLK, IDC_CLASSLIST, OnDblclkClasslist)
	ON_BN_CLICKED(IDC_SET, OnSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamSet message handlers

BOOL CFormExamSet::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();
	
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_STATIC2),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_STATIC3),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_STATIC4),mdRelative,mdRelative);	
	AddSzControl(*GetDlgItem(IDC_STATIC5),mdRelative,mdRelative);	
	
	AddSzControl(*GetDlgItem(IDC_UP),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_SHOWALL),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_PAPERQUERY),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_ADDPAPER),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_NEXT),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_ADDR),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_SET),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_RADIO_STANDARD),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_RADIO_NORMAL),mdRelative,mdRelative);
	AddSzControl(*GetDlgItem(IDC_TEACHER),mdRelative,mdRelative);

	AddSzControl(m_paperlist,mdRelative,mdRelative);
	AddSzControl(m_classlist,mdRelative,mdRelative);
	

	DWORD dwStyle=GetWindowLong(m_paperlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_paperlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  

	m_paperlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_paperlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_paperlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_paperlist.SetExtendedStyle(dwStyle);


    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );

 	m_paperlist.ClearHeadings();
	m_paperlist.SetHeadings("考卷名称,150; 考试科目,100;开考时间,150;收卷试卷,150;开始题号,100;结束题号,100;最大题号,100"); 	
	

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
	m_classlist.SetHeadings("班级编号,100;班级名称,100; 所属院系,100; 班主任,100"); 

// 	CoInitialize(NULL);
// 	CDBDepartment  m_Departmentdb;
// 	if ( !m_Departmentdb.Connect())
// 	{
// 		AfxMessageBox("Departmentdb: 无法连接到数据库服务器");
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
// 		AfxMessageBox("Examaddrdb: 无法连接到数据库服务器");
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

void CFormExamSet::SetInfo()
{
	m_classlist.DeleteAllItems();
	CoInitialize(NULL);
	CDBExamaddr m_examaddrdb;
	if (!m_examaddrdb.Connect())
	{
		AfxMessageBox("m_examaddrdb:无法连接到数据库服务器");
	}
	else
	{
		BOOL flag = m_examaddrdb.LoadByAll();
		while(flag)
		{
			CString sAddr = m_examaddrdb.m_examaddr;
			sAddr.TrimLeft();
			sAddr.TrimRight();
			if(sAddr != "")
			{
				int temp = m_address.FindStringExact(-1, sAddr);
				if(temp == -1)
					m_address.AddString(sAddr);
			}
			flag = m_examaddrdb.Move();
		}
		m_examaddrdb.Close();
	}
	
	CDBClass m_classdb;
	CDBDepartment m_departmentdb;
	if (m_classdb.Connect())
	{
		if (m_departmentdb.Connect())
		{
			BOOL flag1 = m_classdb.LoadByAll();
			while(flag1)
			{
				CString sDepartid = m_classdb.m_departid;
				sDepartid.TrimRight();
				sDepartid.TrimLeft();
				BOOL flag2 = m_departmentdb.LoadByDepartid(sDepartid);
				if (flag2)
				{
					CString sDepartname = m_departmentdb.m_departname;
					CString sClassname = m_classdb.m_classname;
					CString sHead = m_classdb.m_head;
					CString sClassid = m_classdb.m_classid;
					m_classlist.AddItem(0, sClassid, sClassname, sDepartname, sHead);
				}
				
				flag1 = m_classdb.Move();
			}
			
			m_classdb.Close();
			m_departmentdb.Close();
		}
		else
		{
			m_classdb.Close();
			AfxMessageBox("m_departmentdb:无法连接到数据库服务器");
		}
	}
	else
		AfxMessageBox("m_classdb:无法连接到数据库服务器");
	
	CoUninitialize();
}

void CFormExamSet::OnUp() 
{
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	//	fram->m_dlgMain.m_examtime.SetInfo();
	fram->m_dlgMain.ShowDlg(IDD_FORM_EXMA_INFO_BACK);
}

void CFormExamSet::OnShowall() 
{
	m_paperlist.DeleteAllItems();
	m_paperlist.DeleteAllItems();
	CoInitialize(NULL);
	CDBPaper m_paperdb;
	if (!m_paperdb.Connect())
	{
		AfxMessageBox("m_paperdb:无法连接到数据库服务器");
	}
	else
	{
		CDBSubject m_subjectdb;
		if (!m_subjectdb.Connect())
		{
			AfxMessageBox("m_subjectdb:无法连接到数据库服务器");
		}
		else
		{
			BOOL paperflag = m_paperdb.LoadByAll();
			while (paperflag)
			{
				CString strSubjectid = m_paperdb.m_subjectid;
				CString strSubject = "";
				BOOL subjectflag = m_subjectdb.LoadBySubjectid(strSubjectid);
				if (subjectflag)
				{
					strSubject = m_subjectdb.m_subject;
				}

				CString sStartNo = "";
				sStartNo.Format("%d", m_paperdb.m_startNO);
				CString sEndNo = "";
				sEndNo.Format("%d", m_paperdb.m_endNO);
				m_paperlist.AddItem(0, m_paperdb.m_papername, strSubject,m_paperdb.m_starttime.Format(), m_paperdb.m_endtime.Format(), sStartNo, sEndNo, sEndNo);
	 			paperflag = m_paperdb.Move();
			}
			m_subjectdb.Close();
		}
// 		BOOL flag = m_paperdb.LoadByAll();
// 		while (flag)
// 		{
// 			CString sStartNo = "";
// 			sStartNo.Format("%d", m_paperdb.m_startNO);
// 			CString sEndNo = "";
// 			sEndNo.Format("%d", m_paperdb.m_endNO);
// 			m_paperlist.AddItem(0, m_paperdb.m_papername, m_paperdb.m_starttime.Format(), m_paperdb.m_endtime.Format(), sStartNo, sEndNo, sEndNo);
// 			flag = m_paperdb.Move();
// 		}
 		m_paperdb.Close();
	}
	CoUninitialize();
}

void CFormExamSet::OnPaperquery() 
{
	CDlgPaperQuery  dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_paperlist.DeleteAllItems();
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
// 				CString tep1,tep2,tep3;
// 				tep1.Format("%d",m_Paperdb.m_startNO);
// 				tep2.Format("%d",m_Paperdb.m_endNO);
// 				tep3.Format("%d",m_Paperdb.m_maxNO);
// 				m_paperlist.AddItem(0,m_Paperdb.m_papername,
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

void CFormExamSet::OnAddpaper() 
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
// 						CString tep1,tep2,tep3;
// 						tep1.Format("%d",dlg.m_startNO);
// 						tep2.Format("%d",dlg.m_endNO);
// 						tep3.Format("%d",dlg.m_maxNO);
// 						m_list.AddItem(0,dlg.strPaper,
// 							dlg.strSubject,
// 							dlg.strClass,
// 							dlg.tStart.Format(),
// 							dlg.tEnd.Format(),
// 							tep1,tep2,tep3);
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

void CFormExamSet::OnNext() 
{
	int iCount = 0;
	for(int i = 0; i < m_paperlist.GetItemCount(); i++)
	{
		if(m_paperlist.GetCheck(i))
			iCount++;
	}
	if(iCount == 0)
	{
		AfxMessageBox("没有选择试卷，请返回选择需要考试的试卷！");
		return;
	}
	if(iCount > 1)
	{
		AfxMessageBox("选择了多张试卷，请返回重新选择！");
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
		AfxMessageBox("没有选择本场考试的班级，请返回选择需要考试的班级！");
		return;
	}
	
	CString sPaper,sSubject,sStarttime,sEndtime,sStartNO,sEndNO,sMaxNO;
	for(i = 0; i < m_paperlist.GetItemCount(); i++)
	{
		if(m_paperlist.GetCheck(i))
		{
			sPaper = m_paperlist.GetItemText(i,0);
			sSubject = m_paperlist.GetItemText(i,1);
			sStarttime = m_paperlist.GetItemText(i,2);
			sEndtime = m_paperlist.GetItemText(i,3);
			sStartNO = m_paperlist.GetItemText(i,4);
			sEndNO = m_paperlist.GetItemText(i,5);
			sMaxNO = m_paperlist.GetItemText(i,6);
			
			
			m_examdeteil.sPapername = sPaper;
			m_examdeteil.sSubject = sSubject;
			m_examdeteil.sStart = sStarttime;
			m_examdeteil.sEnd = sEndtime;
			m_examdeteil.nMaxNO = atoi(sMaxNO);
			m_examdeteil.nStartNO = atoi(sStartNO);
			m_examdeteil.nEndNO = atoi(sEndNO);
		}
	}
	
	int index = m_address.GetCurSel();
	if(index != -1)
		m_address.GetLBText(index,m_examdeteil.sAddress);
	else
		m_examdeteil.sAddress = "";
	
	m_examdeteil.arryClass.RemoveAll();
	for(i = 0; i < m_classlist.GetItemCount(); i++)
	{
		if(m_classlist.GetCheck(i))
			m_examdeteil.arryClass.Add(m_classlist.GetItemText(i,1));
	}
	
	if(!(((CButton *)GetDlgItem(IDC_RADIO_NORMAL))->GetCheck()) && !(((CButton *)GetDlgItem(IDC_RADIO_STANDARD))->GetCheck()))
	{
		AfxMessageBox("没有选择本场考试性质，无法进入下一步！");
		return;
	}
	
	if(((CButton *)GetDlgItem(IDC_RADIO_NORMAL))->GetCheck())
	{
		m_examdeteil.nExam_quality = 2;
	}
	if(((CButton *)GetDlgItem(IDC_RADIO_STANDARD))->GetCheck())
	{
		m_examdeteil.nExam_quality = 1;
	}
	
	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	fram->m_dlgMain.m_examtime.SetInfo();
	fram->m_dlgMain.ShowDlg(IDD_FORM_EXAM_TIME);
}

void CFormExamSet::SetKind(int kind)
{
	m_kind = kind;
}

void CFormExamSet::OnDblclkPaperlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item =pNMListView->iItem;
	m_paperlist.SelectAll();
	m_paperlist.DeSelectAll();
	m_paperlist.SetCheck(item);
	
	*pResult = 0;
}

void CFormExamSet::OnDblclkClasslist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item =pNMListView->iItem;
	m_classlist.SetCheck(item);
	
	*pResult = 0;
}

void CFormExamSet::OnSet() 
{
	CDlgTeacherSet  dlg;
	dlg.DoModal();
	 
}
