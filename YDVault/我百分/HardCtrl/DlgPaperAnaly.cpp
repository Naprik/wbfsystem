// DlgPaperAnaly.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgPaperAnaly.h"
#include "DBClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPaperAnaly dialog


CDlgPaperAnaly::CDlgPaperAnaly(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPaperAnaly::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPaperAnaly)
	//}}AFX_DATA_INIT
}


void CDlgPaperAnaly::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPaperAnaly)
	DDX_Control(pDX, IDC_SUBJECT, m_subject);
	DDX_Control(pDX, IDC_PAPER, m_paper);
	DDX_Control(pDX, IDC_CLASSLIST, m_classlist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPaperAnaly, CDialog)
	//{{AFX_MSG_MAP(CDlgPaperAnaly)
	ON_CBN_SELCHANGE(IDC_SUBJECT, OnSelchangeSubject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPaperAnaly message handlers

BOOL CDlgPaperAnaly::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	DWORD dwStyle=GetWindowLong(m_classlist.GetSafeHwnd(),GWL_STYLE);
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

	CoInitialize(NULL);
	CDBClass  m_classdb;
	if ( !m_classdb.Connect())
	{
		AfxMessageBox("m_classdb: 无法连接到数据库服务器");
	}
	else
	{
		BOOL flag = m_classdb.LoadByAll();
		while(flag)
		{
			//if(m_Departmentdb.m_classid != "")
				//m_classlist.AddItem(0,m_Departmentdb.m_classid,m_Departmentdb.m_class,m_Departmentdb.m_depart,m_Departmentdb.m_classhead);
			m_classlist.AddItem(0, m_classdb.m_classid, m_classdb.m_classname,"",m_classdb.m_head);
			flag = m_classdb.Move();
		}
		m_classdb.Close();
	}
 	
 
	CDBSubject	   m_Subjectdb;
	if ( !m_Subjectdb.Connect())
	{
		AfxMessageBox("Subjectdb:无法连接到数据库服务器");
	}
	else
	{
		BOOL flag = m_Subjectdb.LoadByAll();	
		while(flag)
		{
			CString sSubject = m_Subjectdb.m_subject;
			sSubject.TrimLeft();
			sSubject.TrimRight();
			int temp = m_subject.FindStringExact(-1, sSubject);
			if(temp == -1)
				m_subject.AddString(sSubject);
			flag = m_Subjectdb.Move();
		}
		m_Subjectdb.Close();
	}
	CoUninitialize();	
	return TRUE;  
}

void CDlgPaperAnaly::OnSelchangeSubject() 
{
	m_paper.ResetContent();
	int index = m_subject.GetCurSel();
	if(index != -1)
		m_subject.GetLBText(index,strSubject);

	strSubject.TrimLeft();
	strSubject.TrimRight();
	CString strSubjectid = "";
	CoInitialize(NULL);
	CDBSubject m_subjectdb;
	if (!m_subjectdb.Connect())
		AfxMessageBox("Paperdb:无法连接到数据库服务器");
	else
	{
		BOOL flag = m_subjectdb.LoadBySubject(strSubject);
		if (flag)
		{
			strSubjectid = m_subjectdb.m_subjectid;	
		}
		m_subjectdb.Close();
	}
	
	CDBPaper m_paperdb;
	if (!m_paperdb.Connect())
		AfxMessageBox("Paperdb:无法连接到数据库服务器");
	else
	{
		BOOL flag = m_paperdb.LoadBySubjectid(strSubjectid);
		while (flag)
		{
			CString sPaper = m_paperdb.m_papername;
			sPaper.TrimLeft();
			sPaper.TrimRight();
			if(sPaper != "")
			{
				int temp = m_paper.FindStringExact(-1, sPaper);
				if(temp == -1)
					m_paper.AddString(sPaper);
			}
			flag = m_paperdb.Move();
		}
		m_paperdb.Close();
	}
	
	CoUninitialize();
}

void CDlgPaperAnaly::OnOK() 
{
	UpdateData(TRUE);
	strSubject = "";
	strPaper = "";

	arryClass.RemoveAll();
	for(int i = 0; i < m_classlist.GetItemCount(); i++)
	{
		if(m_classlist.GetCheck(i))
			arryClass.Add(m_classlist.GetItemText(i,1));
	}
	if(arryClass.GetSize() == 0)
	{
		AfxMessageBox("没有选择班级，请返回选择你需要分析的班级！");
		return;	
	}
	
	int index = m_paper.GetCurSel();
	if(index == -1)
	{
		AfxMessageBox("没有选择试卷，请返回选择你需要分析的试卷！");
		return;	
	}
	if(index != -1)
	{
		m_paper.GetLBText(index, strPaper);
	}

	CDialog::OnOK();
}
