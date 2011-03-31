#include "stdafx.h"
#include "FormDepartment.h"
#include "DlgDepartment.h"
#include "DlgClassSet.h"
#include "../YDExamObjRef/YDDepartmentRef.h"
#include "../base/AutoClean.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../Base/DataHandler.h"
#include "../DBBase/DBTransactionRef.h"
#include "../ObjRef/PropQueryContidition.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../YDExamObjRef/YDClassRef.h"

//*extern CHardCtrlApp theApp;*/

IMPLEMENT_DYNCREATE(CFormDepartment, CYdFormView)

CFormDepartment::CFormDepartment()
: CYdFormView(CFormDepartment::IDD)
{
	m_bLoad = FALSE;
}

CFormDepartment::~CFormDepartment()
{
	ClearDepart();
	ClearClass();
}

void CFormDepartment::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormDepartment)

	DDX_Control(pDX, IDC_CLASS_LIST, m_ClassListCtrl);
	DDX_Control(pDX, IDC_DEPARTMENT_LIST, m_DepartListCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFormDepartment, CFormView)
	ON_BN_CLICKED(IDC_ADDDEPART, OnAdddepart)
	ON_BN_CLICKED(IDC_ADDCLASS, OnAddclass)
	ON_BN_CLICKED(IDC_UPDATEDEPART, OnUpdatedepart)
	ON_BN_CLICKED(IDC_UPDATECLASS, OnUpdateclass)
	ON_BN_CLICKED(IDC_DELETEDEPART, OnDeletedepart)
	ON_BN_CLICKED(IDC_DELETECLASS, OnDeleteclass)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CLASS_LIST, &CFormDepartment::OnLvnItemchangedClass)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DEPARTMENT_LIST, &CFormDepartment::OnLvnItemchangedDepartmentlist)
	ON_NOTIFY(NM_CLICK, IDC_DEPARTMENT_LIST, &CFormDepartment::OnNMClickDepartmentlist)
	ON_NOTIFY(NM_DBLCLK, IDC_CLASS_LIST, &CFormDepartment::OnHdnItemdblclickClassList)
	ON_NOTIFY(NM_DBLCLK, IDC_DEPARTMENT_LIST, &CFormDepartment::OnHdnItemdblclickDeptList)
END_MESSAGE_MAP()


// CYdFormView 诊断

#ifdef _DEBUG
void CFormDepartment::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormDepartment::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CFormDepartment message handlers


void CFormDepartment::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	m_DepartListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_DepartListCtrl.GetWindowRect(rect);
	int iWidth = rect.Width();
	m_DepartListCtrl.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/10);
	m_DepartListCtrl.InsertColumn(1, L"编号", LVCFMT_LEFT,iWidth*1/5);
	m_DepartListCtrl.InsertColumn(2, L"名称", LVCFMT_LEFT,iWidth*3/10);
	m_DepartListCtrl.InsertColumn(3, L"系主任", LVCFMT_LEFT,iWidth*1/5);
	m_DepartListCtrl.InsertColumn(4, L"联系电话", LVCFMT_LEFT,iWidth*1/5 -1);

	CRect rect2;
	m_ClassListCtrl.GetWindowRect(rect2);
	iWidth = rect.Width();
	m_ClassListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ClassListCtrl.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/10);
	m_ClassListCtrl.InsertColumn(1, L"编号", LVCFMT_LEFT,iWidth*1/5);
	m_ClassListCtrl.InsertColumn(2, L"名称", LVCFMT_LEFT,iWidth*3/10);
	m_ClassListCtrl.InsertColumn(3, L"班主任", LVCFMT_LEFT,iWidth*1/5);
	m_ClassListCtrl.InsertColumn(4, L"联系电话", LVCFMT_LEFT,iWidth*1/5 -1);
	
	showdata();
	m_bLoad = TRUE;
}

void CFormDepartment::showdata()
{
	m_DepartListCtrl.DeleteAllItems();
	HRESULT hr =E_FAIL;
	hr = ClearDepart();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
	}
	hr = ClearClass();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
	}
	//得到所有的部门
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	std::list<CYDObjectRef*> lstDepts;
	hr = CStaticObjHelper::GetObjRef(DB_YDDEPARTMENT, pDB, &lstDepts);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}

	for(std::list<CYDObjectRef*>::const_iterator itr = lstDepts.begin();
		itr != lstDepts.end();++itr)
	{
		hr = AddDepartItem(*itr);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
}

HRESULT CFormDepartment::AddDepartItem(CYDObjectRef* _pref)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pref);

	hr = _pref->Restore();
	if(FAILED(hr))
	{
		return hr;
	}

	CComVariant valDepartID;
	hr = _pref->GetPropVal(FIELD_YDDEPARTMENT_ID,&valDepartID);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strDepartID = CDataHandler::VariantToString(valDepartID);

	CComVariant valDepartName;
	hr = _pref->GetPropVal(FIELD_YDDEPARTMENT_NAME,&valDepartName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strDepartName = CDataHandler::VariantToString(valDepartName);

	CComVariant valPresident;
	hr = _pref->GetPropVal(FIELD_YDDEPARTMENT_PRESIDENT,&valPresident);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strPresident = CDataHandler::VariantToString(valPresident);

	CComVariant valPresidenttel;
	hr = _pref->GetPropVal(FIELD_YDDEPARTMENT_PRESIDENT_TEL,&valPresidenttel);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strPresidenttel = CDataHandler::VariantToString(valPresidenttel);
	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_DepartListCtrl.GetItemCount()+1);
	int index = m_DepartListCtrl.InsertItem(m_DepartListCtrl.GetItemCount(), strIndex);
	m_DepartListCtrl.SetItemText(index,1, strDepartID);
	m_DepartListCtrl.SetItemText(index,2, strDepartName);
	m_DepartListCtrl.SetItemText(index,3, strPresident);
	m_DepartListCtrl.SetItemText(index,4, strPresidenttel);
	m_DepartListCtrl.SetItemData(index, (DWORD_PTR)_pref);

	m_lstDepart.push_back(_pref);
	return S_OK;
}


void CFormDepartment::OnAdddepart() 
{
	HRESULT hr = E_FAIL;
	CDlgDepartment	dlg(OP_NEW, this);
	if(dlg.DoModal() == IDOK)
	{
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CYDDepartmentRef* pDepartRef = new CYDDepartmentRef(pDB);

		CComVariant valDepartID(dlg.m_departid);
		hr = pDepartRef->SetPropVal(FIELD_YDDEPARTMENT_ID,&valDepartID);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CComVariant valDepartName(dlg.m_depart);
		hr = pDepartRef->SetPropVal(FIELD_YDDEPARTMENT_NAME,&valDepartName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CComVariant valPresident(dlg.m_head);
		hr = pDepartRef->SetPropVal(FIELD_YDDEPARTMENT_PRESIDENT,&valPresident);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CComVariant valPresidenttel(dlg.m_tel);
		hr = pDepartRef->SetPropVal(FIELD_YDDEPARTMENT_PRESIDENT_TEL,&valPresidenttel);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CDBTransactionRef TransRef(pDB,TRUE);
		hr = pDepartRef->Save();
		if(FAILED(hr))
		{
			TransRef.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		hr = TransRef.Commit();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		hr = AddDepartItem(pDepartRef);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
	}

}


void CFormDepartment::OnAddclass() 
{
	HRESULT hr = E_FAIL;
	int nCount = m_DepartListCtrl.GetSelectedCount();
	if(nCount != 1)
	{
		AfxMessageBox(_T("请在列表中选择一个院系！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_DepartListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_DepartListCtrl.GetNextSelectedItem(pos);
	CYDObjectRef* pObjDepart = (CYDObjectRef*)m_DepartListCtrl.GetItemData(nItem);
	ASSERT(pObjDepart != NULL);
	CComVariant valDepartName;
	hr = pObjDepart->GetPropVal(FIELD_YDDEPARTMENT_NAME,&valDepartName);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CDlgClassSet  dlg(OP_NEW);
	dlg.m_pDepart = pObjDepart;
	dlg.m_strDepart = CDataHandler::VariantToString(valDepartName); 
	if(dlg.DoModal() == IDOK)
	{
		CString  strClass, strClassID, strHead, strHeadtel;
		strClass = dlg.m_class;
		strClassID = dlg.m_classid;
		strHead = dlg.m_classhead;
		strHeadtel = dlg.m_classheadtel;
		
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CYDClassRef* pClassRef = new CYDClassRef(pDB);
		CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
		pObjWrapper->m_pObjRef = pClassRef;
		CYDLinkRef* pLink = new CYDLinkRef(pDB, DB_YDLINK_DEPARTMENT_CLASS);
		pLink->PutObjRef(pObjDepart, pClassRef);
		pObjWrapper->m_pLinkRef = pLink;
	
		CComVariant valClassID(strClassID);
		hr = pClassRef->SetPropVal(FIELD_YDCLASS_CLASSID,&valClassID);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CComVariant valClassName(strClass);
		hr = pClassRef->SetPropVal(FIELD_YDCLASS_NAME,&valClassName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CComVariant valhead(strHead);
		hr = pClassRef->SetPropVal(FIELD_YDCLASS_HEAD,&valhead);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CComVariant valHeadtel(strHeadtel);
		hr = pClassRef->SetPropVal(FIELD_YDCLASS_HEADTEL,&valHeadtel);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		
		CDBTransactionRef Trans(pDB,TRUE);
		hr = pObjWrapper->Save();
		if(FAILED(hr))
		{
			Trans.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		hr = Trans.Commit();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		hr = AddClassItem(pObjWrapper);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}	
	}
}


HRESULT CFormDepartment::AddClassItem(CYdObjWrapper* _pWrapper)
{
	HRESULT hr = E_FAIL;
	CComVariant valClassID;
	hr = _pWrapper->m_pObjRef->GetPropVal(FIELD_YDCLASS_CLASSID,&valClassID);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strClassID = CDataHandler::VariantToString(valClassID);

	CComVariant valclassname;
	hr = _pWrapper->m_pObjRef->GetPropVal(FIELD_YDCLASS_NAME,&valclassname);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strclassname = CDataHandler::VariantToString(valclassname);
	CComVariant valhead;
	hr = _pWrapper->m_pObjRef->GetPropVal(FIELD_YDCLASS_HEAD,&valhead);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strhead = CDataHandler::VariantToString(valhead);

	CComVariant valheadtel;
	hr = _pWrapper->m_pObjRef->GetPropVal(FIELD_YDCLASS_HEADTEL,&valheadtel);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strheadtel = CDataHandler::VariantToString(valheadtel);
	
	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_ClassListCtrl.GetItemCount()+1);
	int index = m_ClassListCtrl.InsertItem(m_ClassListCtrl.GetItemCount(), strIndex);
	m_ClassListCtrl.SetItemText(index,1, strClassID);
	m_ClassListCtrl.SetItemText(index,2, strclassname);
	m_ClassListCtrl.SetItemText(index,3, strhead);
	m_ClassListCtrl.SetItemText(index,4, strheadtel);

	m_ClassListCtrl.SetItemData(index,(DWORD_PTR)_pWrapper);
	m_lstClass.push_back(_pWrapper);
	return S_OK;
}

void CFormDepartment::OnUpdatedepart() 
{
	HRESULT hr = E_FAIL;
 	int nCount = m_DepartListCtrl.GetSelectedCount();
 	if(nCount != 1)
 	{
 		AfxMessageBox(_T("请在列表中选择一个院系！"),MB_OK|MB_ICONWARNING);
 		return;
 	}
 	POSITION pos = m_DepartListCtrl.GetFirstSelectedItemPosition();
 	ASSERT(pos);
 	int nItem = m_DepartListCtrl.GetNextSelectedItem(pos);
 	CYDObjectRef* pObjDepart = (CYDObjectRef*)m_DepartListCtrl.GetItemData(nItem);
 	ASSERT(pObjDepart != NULL);
 	CComVariant valDepartID;
 	hr = pObjDepart->GetPropVal(FIELD_YDDEPARTMENT_ID, &valDepartID);
 	if(FAILED(hr))
 	{
 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
 		return;
 	}
 	CString strDepartID = CDataHandler::VariantToString(valDepartID);
 
 	CComVariant valDepartName;
 	hr = pObjDepart->GetPropVal(FIELD_YDDEPARTMENT_NAME,&valDepartName);
 	if(FAILED(hr))
 	{
 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
 		return;
 	}
 	CString strDepartName = CDataHandler::VariantToString(valDepartName);
 
 	CComVariant valPresident;
 	hr = pObjDepart->GetPropVal(FIELD_YDDEPARTMENT_PRESIDENT,&valPresident);
 	if(FAILED(hr))
 	{
 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
 		return;
 	}
 	CString strPresident = CDataHandler::VariantToString(valPresident);
 
 	CComVariant valPresidenttel;
 	hr = pObjDepart->GetPropVal(FIELD_YDDEPARTMENT_PRESIDENT_TEL,&valPresidenttel);
 	if(FAILED(hr))
 	{
 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
 		return;
 	}
 	CString strPresidenttel = CDataHandler::VariantToString(valPresidenttel);
 	CDlgDepartment	dlg(OP_EDIT, this);
	dlg.m_pDepart = pObjDepart;
 	dlg.m_departid = strDepartID;
 	dlg.m_depart = strDepartName;
 	dlg.m_head = strPresident;
 	dlg.m_tel = strPresidenttel;
 	if(dlg.DoModal() == IDOK)
 	{
 		CYDDepartmentRef* pDepartRef = (CYDDepartmentRef*)pObjDepart;
 	
 		CComVariant valDepartID(dlg.m_departid);
 		hr = pObjDepart->SetPropVal(FIELD_YDDEPARTMENT_ID,&valDepartID);
 		if(FAILED(hr))
 		{
 			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 			return ;
 		}
 
 		CComVariant valdepartname(dlg.m_depart);
 		hr = pObjDepart->SetPropVal(FIELD_YDDEPARTMENT_NAME,&valdepartname);
 		if(FAILED(hr))
 		{
 			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 			return ;
 		}
 		CComVariant valpresident(dlg.m_head);
 		hr = pObjDepart->SetPropVal(FIELD_YDDEPARTMENT_PRESIDENT,&valpresident);
 		if(FAILED(hr))
 		{
 			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 			return ;
 		}
 
 		CComVariant valpresidenttel(dlg.m_tel);
 		hr = pObjDepart->SetPropVal(FIELD_YDDEPARTMENT_PRESIDENT_TEL,&valpresidenttel);
 		if(FAILED(hr))
 		{
 			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 			return ;
 		}
 		
 		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
 		ASSERT(pDB);
 		CDBTransactionRef transRef(pDB,TRUE);
 		hr = pObjDepart->Update();
 		if(FAILED(hr))
 		{
 			transRef.Rollback();
 			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 			return;
 		}
 		
 		hr = transRef.Commit();
 		if(FAILED(hr))
 		{
 			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 			return;
 		}
 		m_DepartListCtrl.SetItemText(nItem,1,dlg.m_departid);
		m_DepartListCtrl.SetItemText(nItem,2,dlg.m_depart);
		m_DepartListCtrl.SetItemText(nItem,3,dlg.m_head);
		m_DepartListCtrl.SetItemText(nItem,4,dlg.m_tel);
 	}
}

void CFormDepartment::OnUpdateclass() 
{
	HRESULT hr = E_FAIL;
 	int nCount = m_DepartListCtrl.GetSelectedCount();
 	if(nCount != 1)
 	{
 		AfxMessageBox(_T("请在列表中选择一个院系！"),MB_OK|MB_ICONWARNING);
 		return;
 	}
 	POSITION pos = m_DepartListCtrl.GetFirstSelectedItemPosition();
 	ASSERT(pos);
 	int nItem = m_DepartListCtrl.GetNextSelectedItem(pos);
 	CYDObjectRef* pObjDepart = (CYDObjectRef*)m_DepartListCtrl.GetItemData(nItem);
 	ASSERT(pObjDepart != NULL);
 	CComVariant valDepartName;
 	hr = pObjDepart->GetPropVal(FIELD_YDDEPARTMENT_NAME,&valDepartName);
 	if(FAILED(hr))
 	{
 		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
 		return;
 	}
 	CString strDepartName = CDataHandler::VariantToString(valDepartName);
 
 	nCount = m_ClassListCtrl.GetSelectedCount();
 	if(nCount != 1)
 	{
 		AfxMessageBox(_T("请在列表中选择一个班级!"),MB_OK|MB_ICONINFORMATION);
 		return;
 	}
 	pos = m_ClassListCtrl.GetFirstSelectedItemPosition();
 	ASSERT(pos);
 	nItem = m_ClassListCtrl.GetNextSelectedItem(pos);
 	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_ClassListCtrl.GetItemData(nItem);
	CYDObjectRef* pObjClass = pObjWrapper->m_pObjRef;
 	ASSERT(pObjClass );
 	CComVariant valClassID;
 	hr = pObjClass->GetPropVal(FIELD_YDCLASS_CLASSID,&valClassID);
 	if(FAILED(hr))
 	{
 		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 		return;
 	}
 	CString strClassID = CDataHandler::VariantToString(valClassID);
 
 	CComVariant valClassName;
 	hr = pObjClass->GetPropVal(FIELD_YDCLASS_NAME,&valClassName);
 	if(FAILED(hr))
 	{
 		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 		return;
 	}
 	CString strClassName = CDataHandler::VariantToString(valClassName);
 
 
 	CComVariant valClassHead;
 	hr = pObjClass->GetPropVal(FIELD_YDCLASS_HEAD,&valClassHead);
 	if(FAILED(hr))
 	{
 		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 		return;
 	}
 	CString strClassHead = CDataHandler::VariantToString(valClassHead);
 
 	CComVariant valClassHeadTel;
 	hr = pObjClass->GetPropVal(FIELD_YDCLASS_HEADTEL,&valClassHeadTel);
 	if(FAILED(hr))
 	{
 		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 		return;
 	}
 	CString strClassHeadTel = CDataHandler::VariantToString(valClassHeadTel);
 
 	CDlgClassSet dlg(OP_EDIT);
	dlg.m_pDepart = pObjDepart;
	dlg.m_pClass = pObjClass;
 	dlg.m_strDepart = strDepartName;
 	dlg.m_class = strClassName;
 	dlg.m_classid = strClassID;
 	dlg.m_classhead = strClassHead;
 	dlg.m_classheadtel = strClassHeadTel;
 	if(dlg.DoModal() == IDOK)
 	{
 		CComVariant valclassid(dlg.m_classid);
 		hr = pObjClass->SetPropVal(FIELD_YDCLASS_CLASSID,&valclassid);
 		if(FAILED(hr))
 		{
 			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 			return ;
 		}
 
 		CComVariant valclassname(dlg.m_class);
 		hr = pObjClass->SetPropVal(FIELD_YDCLASS_NAME,&valclassname);
 		if(FAILED(hr))
 		{
 			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 			return ;
 		}
 
 		CComVariant valhead(dlg.m_classhead);
 		hr = pObjClass->SetPropVal(FIELD_YDCLASS_HEAD,&valhead);
 		if(FAILED(hr))
 		{
 			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 			return ;
 		}
 
 		CComVariant valheadtel(dlg.m_classheadtel);
 		hr = pObjClass->SetPropVal(FIELD_YDCLASS_HEADTEL,&valheadtel);
 		if(FAILED(hr))
 		{
 			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 			return ;
 		}
 		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
 		ASSERT(pDB);
 		CDBTransactionRef TransRef(pDB,TRUE);
 		hr = pObjClass->Update();
 		if(FAILED(hr))
 		{
 			TransRef.Rollback();
 			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 			return;
 		}
 		hr = TransRef.Commit();
 		if(FAILED(hr))
 		{
 			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 			return;
 		}
		m_ClassListCtrl.SetItemText(nItem,1,dlg.m_classid);
		m_ClassListCtrl.SetItemText(nItem,2,dlg.m_class);
		m_ClassListCtrl.SetItemText(nItem,3,dlg.m_classhead);
		m_ClassListCtrl.SetItemText(nItem,4,dlg.m_classheadtel);
 	
 	}
}

void CFormDepartment::OnDeletedepart() 
{
 	int nCount = m_DepartListCtrl.GetSelectedCount();
 	if(nCount <= 0)
 	{
 		AfxMessageBox(_T("请在列表中选择一个院系！"),MB_OK|MB_ICONWARNING);
 		return;
 	}
 	POSITION pos = m_DepartListCtrl.GetFirstSelectedItemPosition();
 	ASSERT(pos);
 	int nItem = m_DepartListCtrl.GetNextSelectedItem(pos);
 	CYDObjectRef* pObjDepart = (CYDObjectRef*)m_DepartListCtrl.GetItemData(nItem);
 	ASSERT(pObjDepart );
 	CString msg;
 	msg.Format(_T("删除选中的院系，将会同时删除该院系中的班级\n确定继续吗？"));
 	if ( MessageBox(msg,_T("确认"),MB_OKCANCEL) != IDOK)
 		return;
 	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
 	ASSERT(pDB);
 	HRESULT hr = E_FAIL;
 	CDBTransactionRef TransRef(pDB,TRUE);
 	hr = pObjDepart->Remove();
 	if(FAILED(hr))
 	{
 		TransRef.Rollback();
 		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 		return;
 	}
 	hr = TransRef.Commit();
 	if(FAILED(hr))
 	{
 		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 		return;
 	}
 	m_DepartListCtrl.DeleteItem(nItem);
}

void CFormDepartment::OnDeleteclass() 
{
 	int nCount = m_ClassListCtrl.GetSelectedCount();
 	if(nCount <= 0)
 	{
 		AfxMessageBox(_T("请在列表中选择一个班级！"),MB_OK|MB_ICONWARNING);
 		return;
 	}
 	POSITION pos = m_ClassListCtrl.GetFirstSelectedItemPosition();
 	ASSERT(pos);
 	int nItem = m_ClassListCtrl.GetNextSelectedItem(pos);
 	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_ClassListCtrl.GetItemData(nItem);
 	ASSERT(pObjWrapper );
 	CString msg;
 	msg.Format(_T("确定要删除选择的班级吗？"));
 	if ( MessageBox(msg,_T("确认"),MB_OKCANCEL) != IDOK)
 		return;	
 	HRESULT hr = E_FAIL;
 	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
 	ASSERT(pDB);
 	CDBTransactionRef TransRef(pDB,TRUE);
 	hr = pObjWrapper->Remove();
 	if(FAILED(hr))
 	{
 		TransRef.Rollback();
 		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 		return;
 	}
 	hr = TransRef.Commit();
 	if(FAILED(hr))
 	{
 		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
 		return;
 	}
 	m_ClassListCtrl.DeleteItem(nItem);
}


HRESULT CFormDepartment::ClearDepart()
{
	CListAutoClean<CYDObjectRef> clr(m_lstDepart);
	return S_OK;
}

HRESULT CFormDepartment::ClearClass()
{
	CListAutoClean<CYdObjWrapper> clr(m_lstClass);
	return S_OK;
}
void CFormDepartment::OnLvnItemchangedClass(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

}


void CFormDepartment::OnLvnItemchangedDepartmentlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (m_bLoad)
	{
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		int index =pNMListView->iItem;
		if(index < 0 || index >= m_DepartListCtrl.GetItemCount())
		{
			return ;
		}
		CYDObjectRef* pObjDept = (CYDObjectRef*)m_DepartListCtrl.GetItemData(index);
		ASSERT(pObjDept);

		m_ClassListCtrl.DeleteAllItems();
		HRESULT hr = E_FAIL;
		hr = ClearClass();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CYDDepartmentRef* pDepartRef = (CYDDepartmentRef*)pObjDept;
		std::list<CYDObjectRef*> lstClass;
		std::list<CYDLinkRef*> lstLinks;
		hr = pDepartRef->GetAllClasses(&lstClass, &lstLinks);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		std::list<CYDLinkRef*>::const_iterator itrLink = lstLinks.begin();
		std::list<CYDObjectRef*>::const_iterator itr = lstClass.begin();
		for(; itr != lstClass.end();++itr, ++itrLink)
		{
			CYdObjWrapper* pObjWrapper = new CYdObjWrapper;
			pObjWrapper->m_pObjRef = (*itr);
			pObjWrapper->m_pLinkRef = (*itrLink);
			hr = AddClassItem(pObjWrapper);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return ;
			}
		}
	}
	
	*pResult = 0;
}

void CFormDepartment::OnNMClickDepartmentlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	*pResult = 0;
	
}

void CFormDepartment::OnHdnItemdblclickClassList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	HRESULT hr = E_FAIL;
	POSITION pos = m_DepartListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_DepartListCtrl.GetNextSelectedItem(pos);
	CYDObjectRef* pObjDepart = (CYDObjectRef*)m_DepartListCtrl.GetItemData(nItem);
	ASSERT(pObjDepart != NULL);
	CComVariant valDepartName;
	hr = pObjDepart->GetPropVal(FIELD_YDDEPARTMENT_NAME,&valDepartName);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strDepartName = CDataHandler::VariantToString(valDepartName);
	
	pos = m_ClassListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	nItem = m_ClassListCtrl.GetNextSelectedItem(pos);
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_ClassListCtrl.GetItemData(nItem);
	CYDObjectRef* pObjClass = pObjWrapper->m_pObjRef;
	ASSERT(pObjClass );
	CComVariant valClassID;
	hr = pObjClass->GetPropVal(FIELD_YDCLASS_CLASSID,&valClassID);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	CString strClassID = CDataHandler::VariantToString(valClassID);

	CComVariant valClassName;
	hr = pObjClass->GetPropVal(FIELD_YDCLASS_NAME,&valClassName);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	CString strClassName = CDataHandler::VariantToString(valClassName);


	CComVariant valClassHead;
	hr = pObjClass->GetPropVal(FIELD_YDCLASS_HEAD,&valClassHead);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	CString strClassHead = CDataHandler::VariantToString(valClassHead);

	CComVariant valClassHeadTel;
	hr = pObjClass->GetPropVal(FIELD_YDCLASS_HEADTEL,&valClassHeadTel);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	CString strClassHeadTel = CDataHandler::VariantToString(valClassHeadTel);

	CDlgClassSet dlg(OP_VIEW);
	dlg.m_pDepart = pObjDepart;
	dlg.m_pClass = pObjClass;
	dlg.m_strDepart = strDepartName;
	dlg.m_class = strClassName;
	dlg.m_classid = strClassID;
	dlg.m_classhead = strClassHead;
	dlg.m_classheadtel = strClassHeadTel;
	dlg.DoModal();
	*pResult = 0;
}

void CFormDepartment::OnHdnItemdblclickDeptList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	HRESULT hr = E_FAIL;
	POSITION pos = m_DepartListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_DepartListCtrl.GetNextSelectedItem(pos);
	CYDObjectRef* pObjDepart = (CYDObjectRef*)m_DepartListCtrl.GetItemData(nItem);
	ASSERT(pObjDepart != NULL);
	CComVariant valDepartID;
	hr = pObjDepart->GetPropVal(FIELD_YDDEPARTMENT_ID, &valDepartID);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strDepartID = CDataHandler::VariantToString(valDepartID);

	CComVariant valDepartName;
	hr = pObjDepart->GetPropVal(FIELD_YDDEPARTMENT_NAME,&valDepartName);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strDepartName = CDataHandler::VariantToString(valDepartName);

	CComVariant valPresident;
	hr = pObjDepart->GetPropVal(FIELD_YDDEPARTMENT_PRESIDENT,&valPresident);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strPresident = CDataHandler::VariantToString(valPresident);

	CComVariant valPresidenttel;
	hr = pObjDepart->GetPropVal(FIELD_YDDEPARTMENT_PRESIDENT_TEL,&valPresidenttel);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strPresidenttel = CDataHandler::VariantToString(valPresidenttel);
	CDlgDepartment	dlg(OP_VIEW, this);
	dlg.m_departid = strDepartID;
	dlg.m_depart = strDepartName;
	dlg.m_head = strPresident;
	dlg.m_tel = strPresidenttel;
	dlg.DoModal();
	*pResult = 0;
}

BOOL CFormDepartment::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	CExtDllState extdllstate;

	return CYdFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
