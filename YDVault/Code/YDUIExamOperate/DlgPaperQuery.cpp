// DlgPaperQuery.cpp : implementation file
//

#include "stdafx.h"
#include "DlgPaperQuery.h"
#include "../ObjRef\YDObjectRef.h"
#include "../ObjHelper\StaticObjHelper.h"
#include "../Base\AutoClean.h"
#include "../YDExamObjRef\YDDepartmentRef.h"
#include "../YDExamObjRef\YDSubjectRef.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPaperQuery dialog


CDlgPaperQuery::CDlgPaperQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPaperQuery::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPaperQuery)
	m_time = COleDateTime::GetCurrentTime();
	//}}AFX_DATA_INIT
}

CDlgPaperQuery::~CDlgPaperQuery()
{
	Clear();
}

HRESULT CDlgPaperQuery::Clear()
{
	CListAutoClean<CYDObjectRef> clr1(m_lstDepart);
	CListAutoClean<CYDObjectRef> clr2(m_lstSubject);
	return S_OK;
}

void CDlgPaperQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPaperQuery)
	DDX_Control(pDX, IDC_PAPERNAME, m_paper);
	DDX_Control(pDX, IDC_CHECKTIME, m_checktime);
	DDX_Control(pDX, IDC_CHECKDEPART, m_checkdepart);
	DDX_Control(pDX, IDC_DATETIMEPICKER, m_timectl);
	DDX_Control(pDX, IDC_CHECKSUBJECT, m_checksub);
	DDX_Control(pDX, IDC_SUBJECT, m_subject);
	DDX_Control(pDX, IDC_DEPART, m_depart);
	DDX_Control(pDX, IDC_CLASS, m_class);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER, m_time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPaperQuery, CDialog)
	//{{AFX_MSG_MAP(CDlgPaperQuery)
	ON_BN_CLICKED(IDC_CHECKSUBJECT, OnChecksubject)
	ON_BN_CLICKED(IDC_CHECKDEPART, OnCheckdepart)
	ON_BN_CLICKED(IDC_CHECKTIME, OnChecktime)
	ON_CBN_SELCHANGE(IDC_DEPART, OnSelchangeDepart)
	ON_CBN_SELCHANGE(IDC_SUBJECT, OnSelchangeSubject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPaperQuery message handlers

void CDlgPaperQuery::OnChecksubject() 
{
	if(m_checksub.GetCheck())
	{
		m_subject.EnableWindow(TRUE);
		m_paper.EnableWindow(TRUE);
	}
	else
	{
		m_subject.EnableWindow(FALSE);
		m_paper.EnableWindow(FALSE);
	}
}

BOOL CDlgPaperQuery::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_subject.EnableWindow(FALSE);
	m_paper.EnableWindow(FALSE);
	m_class.EnableWindow(FALSE);
	m_depart.EnableWindow(FALSE);
	m_timectl.EnableWindow(FALSE);	
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

void CDlgPaperQuery::OnCheckdepart() 
{
	if(m_checkdepart.GetCheck())
	{
		m_depart.EnableWindow(TRUE);
		m_class.EnableWindow(TRUE);
	}
	else
	{
		m_depart.EnableWindow(FALSE);
		m_class.EnableWindow(FALSE);
	}
}

void CDlgPaperQuery::OnChecktime() 
{
	if(m_checktime.GetCheck())
		m_timectl.EnableWindow(TRUE);
	else
		m_timectl.EnableWindow(FALSE);
}

void CDlgPaperQuery::OnSelchangeDepart() 
{
	m_class.ResetContent();
	int index = m_depart.GetCurSel();
	if(index != -1)
		m_depart.GetLBText(index,strDepart);
	CYDObjectRef* pObjRef = (CYDObjectRef*)m_depart.GetItemData(index);
	ASSERT(pObjRef);
	ASSERT(pObjRef->IsKindOf(RUNTIME_CLASS(CYDDepartmentRef)));
	CYDDepartmentRef* pDepartRef = (CYDDepartmentRef*)pObjRef;
	ASSERT(pDepartRef);
	CString strSql;
	HRESULT hr = E_FAIL;
	std::list<CYDObjectRef*> lstClassRef;
	//hr = pDepartRef->GetAllClassRef(&lstClassRef);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CListAutoClean<CYDObjectRef> clr(lstClassRef);

// 	strSql.Format("select classname from ydclass where departid = (select departid from yddepartment where departname = '%s');",
// 		strDepart);	
// 	CMyODBC db;
	for(std::list<CYDObjectRef*>::const_iterator itr = lstClassRef.begin();
		itr != lstClassRef.end();++itr)
	{
		CString sClass;
		hr = (*itr)->GetPropVal(FIELD_YDCLASS_NAME,strClass);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return;
		}
		sClass.TrimLeft();
		sClass.TrimRight();
		if(!sClass.IsEmpty())
		{
			int temp = m_class.FindStringExact(-1, sClass);
			if(temp == -1)
			{
				m_class.AddString(sClass);
			}
		}
	}		
}

void CDlgPaperQuery::OnOK() 
{
	UpdateData(TRUE);
	strSubject = "";
	strDepart = "";
	strClass = "";
	strPaper = "";

	if(!m_checktime.GetCheck())
		bIstime = FALSE;
	else
		bIstime = TRUE;
	int index = m_subject.GetCurSel();
	if(index != -1)
		m_subject.GetLBText(index,strSubject);

	index = m_paper.GetCurSel();
	if(index != -1)
	{
		m_paper.GetLBText(index,strPaper);
		if(strPaper == "全部")
			strPaper = "";
	}
	
	index = m_depart.GetCurSel();
	if(index != -1)
		m_depart.GetLBText(index,strDepart);

	index = m_class.GetCurSel();
	if(index != -1)
	{
		m_class.GetLBText(index,strClass);
//		if(strClass == "全部")
//			strClass = "";
	}

//	AfxMessageBox(m_time.Format());
	
	CDialog::OnOK();
}

void CDlgPaperQuery::OnSelchangeSubject() 
{
	m_paper.ResetContent();
	int index = m_subject.GetCurSel();
	if(index != -1)
		m_subject.GetLBText(index,strSubject);
	HRESULT hr = E_FAIL;
	CYDSubjectRef* pSubjectRef = (CYDSubjectRef*)m_subject.GetItemData(index);
	ASSERT(pSubjectRef);

	strSubject.TrimLeft();
	strSubject.TrimRight();
	
	
	//strSql.Format("select papername from ydpaper where subjectid = (select subjectid from ydsubject where subject = '%s' );", strSubject);
	std::list<CYDObjectRef*> lstPaperRef;
	CListAutoClean<CYDObjectRef> clr(lstPaperRef);
	//hr = pSubjectRef->GetAllPaperRef(&lstPaperRef);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}

	for(std::list<CYDObjectRef*>::const_iterator itr = lstPaperRef.begin();
		itr != lstPaperRef.end();++itr)
	{
		CString sPaper ;
		hr = (*itr)->GetPropVal(FIELD_YDPAPER_NAME,sPaper);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return;
		}

		sPaper.TrimLeft();
		sPaper.TrimRight();
		if(sPaper != "")
		{
			int temp = m_paper.FindStringExact(-1, sPaper);
			if(temp == -1)
				m_paper.AddString(sPaper);
		}
	}
}

INT_PTR CDlgPaperQuery::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	CExtDllState state;
	return CDialog::DoModal();
}
