// DlgClassSet.cpp : implementation file
//

#include "stdafx.h"
#include "DlgClassSet.h"
#include "../base/AutoClean.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../Include/ShowErr.h"
#include "../Base/DataHandler.h"
#include "../YDExamObjRef\YDDepartmentRef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgClassSet dialog


CDlgClassSet::CDlgClassSet(OPERATION op, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgClassSet::IDD, pParent)
	, m_strDepart(_T(""))
{
	//{{AFX_DATA_INIT(CDlgClassSet)
	m_classid = _T("");
	m_class = _T("");
	m_classheadtel = _T("");
	m_classhead = _T("");
	m_opMode = op;
	m_pDepart = NULL;
	m_pClass = NULL;
	//}}AFX_DATA_INIT
}


void CDlgClassSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgClassSet)
	DDX_Text(pDX, IDC_CLASSID, m_classid);
	DDX_Text(pDX, IDC_CLASS, m_class);
	DDX_Text(pDX, IDC_CLASSHEADTEL, m_classheadtel);
	DDX_Text(pDX, IDC_CLASSHEAD, m_classhead);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_DEPARTMENT, m_strDepart);
}


BEGIN_MESSAGE_MAP(CDlgClassSet, CDialog)
	//{{AFX_MSG_MAP(CDlgClassSet)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDlgClassSet::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgClassSet message handlers

BOOL CDlgClassSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
 	if (m_opMode == OP_VIEW)
 	{
 		GetDlgItem(IDC_CLASSID)->EnableWindow(FALSE);
 		GetDlgItem(IDC_CLASS)->EnableWindow(FALSE);
 		GetDlgItem(IDC_CLASSHEAD)->EnableWindow(FALSE);
 		GetDlgItem(IDC_CLASSHEADTEL)->EnableWindow(FALSE);
 	}
	return TRUE; 
}

void CDlgClassSet::OnBnClickedOk()
{
	HRESULT hr = E_FAIL;
	UpdateData(TRUE);
	m_classid.Trim();
	if(m_classid.IsEmpty())
	{
		AfxMessageBox(_T("没有输入班级编号，请输入班级编号！"));
		return;
	}
	m_class.Trim();
	if(m_class.IsEmpty())
	{
		AfxMessageBox(_T("没有输入班级名称，请输入班级名称！"));
		return;
	}
	m_classhead.Trim();
	m_classheadtel.Trim();
	if(m_opMode == OP_NEW || m_opMode == OP_EDIT)
	{
		//要验证班级编号和名称是否重复
		ASSERT(m_pDepart);
		std::list<CYDObjectRef*> lstObjClass;
		CYDDepartmentRef* pDepart = (CYDDepartmentRef*)m_pDepart;
		ASSERT(pDepart);
		hr = pDepart->GetAllClasses(&lstObjClass);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CListAutoClean<CYDObjectRef> clr(lstObjClass);
		BOOL bFind = FALSE;
		for(std::list<CYDObjectRef*>::const_iterator itr = lstObjClass.begin();
			itr != lstObjClass.end();++itr)
		{
			CString strClassID;
			hr = (*itr)->GetPropVal(FIELD_YDCLASS_CLASSID,strClassID);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return ;
			}
			if(strClassID.CompareNoCase(m_classid) == 0)
			{
				if(m_opMode == OP_NEW)
				{
					bFind = TRUE;
					break;
				}
				else if(m_opMode == OP_EDIT)
				{
					//要验证两个对象是否是一个
					OBJID idItr = ID_EMPTY;
					hr = (*itr)->GetID(&idItr);
					if(FAILED(hr))
					{
						DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
						return ;
					}
					OBJID idClass = ID_EMPTY;
					hr = m_pClass->GetID(&idClass);
					if(FAILED(hr))
					{
						DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
						return ;
					}
					if(idClass != idItr)
					{
						bFind = TRUE;
						break;
					}
				}
				
			}
		}
		if(bFind)
		{
			AfxMessageBox(_T("班级编号不能重复！"));
			return;
		}
		//要验证名称是否相同
		bFind = FALSE;
		for(std::list<CYDObjectRef*>::const_iterator itr = lstObjClass.begin();
			itr != lstObjClass.end();++itr)
		{
			CString strClassName;
			hr = (*itr)->GetPropVal(FIELD_YDCLASS_NAME,strClassName);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return ;
			}
			if(strClassName.CompareNoCase(m_class) == 0)
			{
				if(m_opMode == OP_NEW)
				{
					bFind = TRUE;
					break;
				}
				else if(m_opMode == OP_EDIT)
				{
					//要验证两个对象是否是一个
					OBJID idItr = ID_EMPTY;
					hr = (*itr)->GetID(&idItr);
					if(FAILED(hr))
					{
						DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
						return ;
					}
					OBJID idClass = ID_EMPTY;
					hr = m_pClass->GetID(&idClass);
					if(FAILED(hr))
					{
						DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
						return ;
					}
					if(idClass != idItr)
					{
						bFind = TRUE;
						break;
					}
				}

			}
		}
		if(bFind)
		{
			AfxMessageBox(_T("班级名称不能重复！"));
			return;
		}
	}
	

	UpdateData(FALSE);
	OnOK();
}

INT_PTR CDlgClassSet::DoModal()
{
	CExtDllState extdllstate;

	return CDialog::DoModal();
}
