// DlgQuerystudentresult.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgQuerystudentresult.h"
#include "DBSubject.h"
#include "DBPaper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgQuerystudentresult dialog


CDlgQuerystudentresult::CDlgQuerystudentresult(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgQuerystudentresult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgQuerystudentresult)
	m_datetime = COleDateTime::GetCurrentTime();
	m_stuid = _T("");
	m_stuname = _T("");
	//}}AFX_DATA_INIT
}


void CDlgQuerystudentresult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgQuerystudentresult)
	DDX_Control(pDX, IDC_STUNAME, m_editname);
	DDX_Control(pDX, IDC_STUID, m_editid);
	DDX_Control(pDX, IDC_DATETIMEPICKER, m_datectl);
	DDX_Control(pDX, IDC_PAPER, m_paper);
	DDX_Control(pDX, IDC_SUB, m_sub);
	DDX_Control(pDX, IDC_NAME, m_name);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER, m_datetime);
	DDX_Text(pDX, IDC_STUID, m_stuid);
	DDX_Text(pDX, IDC_STUNAME, m_stuname);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgQuerystudentresult, CDialog)
	//{{AFX_MSG_MAP(CDlgQuerystudentresult)
	ON_CBN_SELCHANGE(IDC_SUB, OnSelchangeSub)
	ON_BN_CLICKED(IDC_ALL, OnAll)
	ON_BN_CLICKED(IDC_SUBJECTRADIO, OnSubjectradio)
	ON_BN_CLICKED(IDC_TIME, OnTime)
	ON_BN_CLICKED(IDC_STUDNUM, OnStudnum)
	ON_BN_CLICKED(IDC_NAME, OnName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgQuerystudentresult message handlers
BOOL CDlgQuerystudentresult::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_sub.EnableWindow(FALSE);
	m_paper.EnableWindow(FALSE);
	m_datectl.EnableWindow(FALSE);

	((CButton *)GetDlgItem(IDC_STUDNUM))->SetCheck(TRUE);
	GetDlgItem(IDC_STATIC1)->SetWindowText("学生学号");
	m_editname.ShowWindow(FALSE);

	((CButton *)GetDlgItem(IDC_ALL))->SetCheck(TRUE);

	CoInitialize(NULL);
	CDBSubject m_subjectdb;
	if (!m_subjectdb.Connect())
 		AfxMessageBox("Subjectdb:无法连接到数据库服务器");
	else
	{
		BOOL flag = m_subjectdb.LoadByAll();
		while (flag)
		{
			CString sSubject = m_subjectdb.m_subject;
			sSubject.TrimLeft();
			sSubject.TrimRight();
			if(sSubject != "")
			{
				int temp = m_sub.FindStringExact(-1, sSubject);
				if(temp == -1)
					m_sub.AddString(sSubject);
 			}
			flag = m_subjectdb.Move();

		}
		m_subjectdb.Close();
	}
	CoUninitialize();
		

	return true;
}


void CDlgQuerystudentresult::OnOK() 
{
	UpdateData(TRUE);
	if(((CButton *)GetDlgItem(IDC_STUDNUM))->GetCheck())
	{
		if(m_stuid == "")
		{
			AfxMessageBox("学生学号不能为空，请返回重新输入！");
			return;
		}
		else
		{
			strID = m_stuid;
			strName = "";
		}
	}
	if(((CButton *)GetDlgItem(IDC_NAME))->GetCheck())
	{
		if(m_stuname == "")
		{
			AfxMessageBox("学生姓名不能为空，请返回重新输入！");
			return;
		}
		else
		{
			strName = m_stuname;
			strID = "";
		}
	}
	if(((CButton *)GetDlgItem(IDC_ALL))->GetCheck())
	{
		strSubject = "";
		strPaper = "";
		bIstime = FALSE;
	}
	if(((CButton *)GetDlgItem(IDC_SUBJECTRADIO))->GetCheck())
	{
		int index = m_sub.GetCurSel();
		if(index != -1)
			m_sub.GetLBText(index,strSubject);	
		else
			strSubject = "";

		index = m_paper.GetCurSel();
		if(index != -1)
			m_paper.GetLBText(index,strPaper);	
		else
			strPaper = "";
		bIstime = FALSE;
	}
	if(((CButton *)GetDlgItem(IDC_TIME))->GetCheck())
	{
		strSubject = "";
		strPaper = "";
		bIstime = TRUE;
	}
	CDialog::OnOK();
}

void CDlgQuerystudentresult::OnCancel() 
{
	CDialog::OnCancel();
}

void CDlgQuerystudentresult::OnSelchangeSub() 
{
	m_paper.ResetContent();
	int index = m_sub.GetCurSel();
	if(index != -1)
		m_sub.GetLBText(index,strSubject);
	
	strSubject.TrimLeft();
	strSubject.TrimRight();
	
	CString strSubjectid = "";
	CoInitialize(NULL);
	CDBSubject m_subjectdb;
	if (!m_subjectdb.Connect())
		AfxMessageBox("m_subjectdb:无法连接到数据库服务器");
	else
	{
		BOOL subjectflag = m_subjectdb.LoadBySubject(strSubject);
		if (subjectflag)
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

void CDlgQuerystudentresult::OnAll() 
{
	m_sub.EnableWindow(FALSE);
	m_paper.EnableWindow(FALSE);
	m_datectl.EnableWindow(FALSE);
}

void CDlgQuerystudentresult::OnSubjectradio() 
{
	m_sub.EnableWindow(TRUE);
	m_paper.EnableWindow(TRUE);
	m_datectl.EnableWindow(FALSE);		
}

void CDlgQuerystudentresult::OnTime() 
{
	m_sub.EnableWindow(FALSE);
	m_paper.EnableWindow(FALSE);
	m_datectl.EnableWindow(TRUE);
}

void CDlgQuerystudentresult::OnStudnum() 
{
//	((CButton *)GetDlgItem(IDC_STUDNUM))->SetCheck(TRUE);
	GetDlgItem(IDC_STATIC1)->SetWindowText("学生学号");
	m_editname.ShowWindow(FALSE);
	m_editid.ShowWindow(TRUE);
}

void CDlgQuerystudentresult::OnName() 
{
	GetDlgItem(IDC_STATIC1)->SetWindowText("学生姓名");
	m_editname.ShowWindow(TRUE);
	m_editid.ShowWindow(FALSE);
}
