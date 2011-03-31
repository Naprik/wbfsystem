// DlgPaperset.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "DlgPaperset.h"


CDlgPaperset::CDlgPaperset(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPaperset::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPaperset)
	m_endNO = 0;
	m_enddate = COleDateTime::GetCurrentTime();
	m_endtime = COleDateTime::GetCurrentTime();
	m_maxNO = 0;
	m_startNO = 0;
	m_startdate = COleDateTime::GetCurrentTime();
	m_starttime = COleDateTime::GetCurrentTime();
	m_papername = _T("");
	//}}AFX_DATA_INIT
}


void CDlgPaperset::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPaperset)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_SUBJECT, m_subject);
	DDX_Control(pDX, IDC_DEPART, m_depart);
	DDX_Control(pDX, IDC_CLASS, m_class);
	DDX_Text(pDX, IDC_END_NO, m_endNO);
	DDX_DateTimeCtrl(pDX, IDC_ENDDATE, m_enddate);
	DDX_DateTimeCtrl(pDX, IDC_ENDTIM, m_endtime);
	DDX_Text(pDX, IDC_MAX_NO, m_maxNO);
	DDX_Text(pDX, IDC_START_NO, m_startNO);
	DDX_DateTimeCtrl(pDX, IDC_STARTDATE, m_startdate);
	DDX_DateTimeCtrl(pDX, IDC_STARTTIME, m_starttime);
	DDX_Text(pDX, IDC_PAPERNAME, m_papername);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPaperset, CDialog)
	//{{AFX_MSG_MAP(CDlgPaperset)
	ON_CBN_SELCHANGE(IDC_DEPART, OnSelchangeDepart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPaperset message handlers

void CDlgPaperset::OnOK() 
{
	UpdateData(TRUE);

	int index = m_subject.GetCurSel();
	if(index != -1)
		m_subject.GetLBText(index,strSubject);	
	else
	{
		AfxMessageBox("û��ѡ���Կ�Ŀ���뷵��ѡ��");
		return;
	}
	index = m_depart.GetCurSel();
	if(index != -1)
		m_depart.GetLBText(index,strDepart);	
	else
	{
		AfxMessageBox("û��ѡ��ο�Ժϵ���뷵��ѡ��");
		return;
	}
	index = m_class.GetCurSel();
	if(index != -1)
		m_class.GetLBText(index, strClass);
	else
	{
		AfxMessageBox("û��ѡ��ο��༶���뷵��ѡ��");
		return;
	}
	
	tStart.SetDateTime(m_startdate.GetYear(),m_startdate.GetMonth(),
						m_startdate.GetDay(),m_starttime.GetHour(),
						m_starttime.GetMinute(),m_starttime.GetSecond());
	tEnd.SetDateTime(m_enddate.GetYear(), m_enddate.GetMonth(),
						m_enddate.GetDay(),m_endtime.GetHour(),
						m_endtime.GetMinute(),m_endtime.GetMinute());

	COleDateTimeSpan timeSpan; 
	timeSpan = tEnd - tStart;
	int span = (int)timeSpan.GetTotalSeconds();
	if(span <= 0)
	{
		AfxMessageBox("�վ�ʱ�������ڿ���ʱ�䣬�뷵���������ã�");
		return;
	}

	if(m_startNO <= 0)
	{
		AfxMessageBox("��ʼ������ô����뷵���������ã�");
		return;
	}

	if(m_endNO <= 0)
	{
		AfxMessageBox("����������ô����뷵���������ã�");
		return;
	}
	if(m_maxNO <= 0)
	{
		AfxMessageBox("���������ô����뷵���������ã�");
		return;
	}
	if(m_papername == "")
	{
		COleDateTime time;
		time.SetDate(tStart.GetYear(),tStart.GetMonth(),tStart.GetDay());
		CString sTemp = "û�������Ծ����ƣ�ϵͳ��Ĭ������Ϊ��"+strSubject+time.Format()+"\n"+"ȷ����";
		if ( MessageBox(sTemp,"ȷ��",MB_OKCANCEL) != IDOK)
			return;	
		else
			strPaper = strSubject+time.Format();
	}
	else
		strPaper = m_papername;
	
	CDialog::OnOK();
}

BOOL CDlgPaperset::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CoInitialize(NULL);
	CDBDepartment  m_Departmentdb;
	if ( !m_Departmentdb.Connect())
	{
		AfxMessageBox("DBDepartmentdb:�޷����ӵ����ݿ������");
	}
	else
	{
		BOOL flag = m_Departmentdb.LoadByAll();
		//m_depart.AddString("ȫ��");
		while(flag)
		{
			CString sDepart = m_Departmentdb.m_departname;
			sDepart.TrimLeft();
			sDepart.TrimRight();
			int temp = m_depart.FindStringExact(-1, sDepart);
			if(temp == -1)
				m_depart.AddString(sDepart);
			flag = m_Departmentdb.Move();
		}
		m_Departmentdb.Close();
	};

	
	CDBSubject     m_Subjectdb;
	if ( !m_Subjectdb.Connect())
	{
		AfxMessageBox("Subjectdb:�޷����ӵ����ݿ������");
	}
	else
	{
		BOOL flag = m_Subjectdb.LoadByAll();
		while(flag)
		{
			CString sClass = m_Subjectdb.m_subject;
			sClass.TrimLeft();
			sClass.TrimRight();
			int temp = m_subject.FindStringExact(-1, sClass);
			if(temp == -1)
				m_subject.AddString(sClass);
			flag = m_Subjectdb.Move();
		}
		m_Subjectdb.Close();
	}
	CoUninitialize();
	
	return TRUE;  
}

void CDlgPaperset::OnSelchangeDepart() 
{
	m_class.ResetContent();
//	m_class.AddString("ȫ��");
	int index = m_depart.GetCurSel();
	if(index != -1)
		m_depart.GetLBText(index,strDepart);

	strDepart.TrimLeft();
	strDepart.TrimRight();
	
	CoInitialize(NULL);
	CDBDepartment  m_Departmentdb;
	CDBClass       m_classdb;
	if ( !m_Departmentdb.Connect())
		AfxMessageBox("DBDepartmentdb:�޷����ӵ����ݿ������");
	else
	{
		if (!m_classdb.Connect())
			AfxMessageBox("m_classdb:�޷����ӵ����ݿ������");
		else
		{
			BOOL departflag = m_Departmentdb.LoadByDepart(strDepart);
			if (departflag)
			{
				CString strDepartid = m_Departmentdb.m_departid;
				BOOL classflag = m_classdb.LoadByDepartid(strDepartid);
				while (classflag)
				{
					CString strClass = m_classdb.m_classname;
					strClass.TrimLeft();
					strClass.TrimRight();
					if(strClass != "")
					{
						int temp = m_class.FindStringExact(-1, strClass);
						if(temp == -1)
							m_class.AddString(strClass);
					}		
					classflag = m_classdb.Move();
				}
			}
			m_classdb.Close();
		}
		m_Departmentdb.Close();
	}
	CoUninitialize();	
}
