// DlgPaperset.cpp : implementation file
//

#include "stdafx.h"
#include "DlgPaperset.h"
#include "../Base\AutoClean.h"
#include "../DBBase\DatabaseEx.h"
#include "../ObjHelper\StaticObjHelper.h"
#include "../objref\YDObjectRef.h"
#include "../YDExamObjRef\YDDepartmentRef.h"


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

HRESULT CDlgPaperset::Clear()
{
	HRESULT hr = E_FAIL;
	CListAutoClean<CYDObjectRef> clr1(m_lstDepart);
	CListAutoClean<CYDObjectRef> clr2(m_lstSubject);
	return S_OK;
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
		AfxMessageBox(_T("没有选择考试科目，请返回选择！"));
		return;
	}
	index = m_depart.GetCurSel();
	if(index != -1)
		m_depart.GetLBText(index,strDepart);	
	else
	{
		AfxMessageBox(_T("没有选择参考院系，请返回选择！"));
		return;
	}
	index = m_class.GetCurSel();
	if(index != -1)
		m_class.GetLBText(index, strClass);
	else
	{
		AfxMessageBox(_T("没有选择参考班级，请返回选择！"));
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
		AfxMessageBox(_T("收卷时间必须大于开考时间，请返回重新设置！"));
		return;
	}

	if(m_startNO <= 0)
	{
		AfxMessageBox(_T("开始题号设置错误，请返回重新设置！"));
		return;
	}

	if(m_endNO <= 0)
	{
		AfxMessageBox(_T("结束题号设置错误，请返回重新设置！"));
		return;
	}
	if(m_maxNO <= 0)
	{
		AfxMessageBox(_T("最大题号设置错误，请返回重新设置！"));
		return;
	}
	if(m_papername.IsEmpty())
	{
		COleDateTime time;
		time.SetDate(tStart.GetYear(),tStart.GetMonth(),tStart.GetDay());
		CString sTemp = _T("没有设置试卷名称！系统将默认设置为：")+strSubject+time.Format()+"\n"+"确定？";
		if ( MessageBox(sTemp,_T("确认"),MB_OKCANCEL) != IDOK)
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
	//strSql.Format("select departname from yddepartment;  ");	
	Clear();

	//strSql.Format("select departname from yddepartment;  ");	

	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	HRESULT hr = E_FAIL;

	hr = CStaticObjHelper::GetObjRef(DB_YDDEPARTMENT,pDB,&m_lstDepart);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	for(std::list<CYDObjectRef*>::const_iterator itr = m_lstDepart.begin();
		itr != m_lstDepart.end();++itr)
	{
		CString sDepart;
		hr = (*itr)->GetPropVal(FIELD_YDDEPARTMENT_NAME,strDepart);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		sDepart.TrimLeft();
		sDepart.TrimRight();
		int temp = m_depart.FindStringExact(-1, sDepart);
		if(temp == -1)
		{
			int index = m_depart.AddString(sDepart);
			m_depart.SetItemData(index,(DWORD_PTR)(*itr));
		}
	}

	//ydsubject
	//strSql.Format("select subject from ydsubject;");

	hr = CStaticObjHelper::GetObjRef(DB_YDSUBJECT,pDB,&m_lstSubject);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = m_lstSubject.begin();
		itr != m_lstSubject.end();++itr)
	{
		CString sClass;
		hr = (*itr)->GetPropVal(FIELD_YDSUBJECT_NAME,strClass);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		sClass.TrimLeft();
		sClass.TrimRight();
		int temp = m_subject.FindStringExact(-1, sClass);
		if(temp == -1)
		{
			int index = m_subject.AddString(sClass);
			m_subject.SetItemData(index,(DWORD_PTR)(*itr));
		}
	}

	
	return TRUE;  
}

void CDlgPaperset::OnSelchangeDepart() 
{
	m_class.ResetContent();
	int index = m_depart.GetCurSel();
	if(index != -1)
		m_depart.GetLBText(index,strDepart);

	strDepart.TrimLeft();
	strDepart.TrimRight();
	CYDDepartmentRef* pDepartRef = (CYDDepartmentRef*)m_depart.GetItemData(index);
	ASSERT(pDepartRef);
	HRESULT hr = E_FAIL;
	std::list<CYDObjectRef*> lstClass;
//	hr = pDepartRef->GetAllClassRef(&lstClass);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CListAutoClean<CYDObjectRef> clr(lstClass);
	
// 	strSql.Format("select classname from ydclass where departid = (select departid from yddepartment where departname = '%s');",
// 		strDepart);	
	for(std::list<CYDObjectRef*>::const_iterator itr = lstClass.begin();
		itr != lstClass.end();++itr)
	{
		CString sClass ;
		hr = (*itr)->GetPropVal(FIELD_YDCLASS_NAME,sClass);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return;
		}
		sClass.TrimLeft();
		sClass.TrimRight();
		if(sClass != "")
		{
			int temp = m_class.FindStringExact(-1, sClass);
			if(temp == -1)
				m_class.AddString(sClass);
		}
	}	
}

INT_PTR CDlgPaperset::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	CExtDllState state;
	return CDialog::DoModal();
}
