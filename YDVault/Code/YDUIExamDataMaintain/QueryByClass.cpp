// QueryByClass.cpp : implementation file
//

#include "stdafx.h"
#include "QueryByClass.h"
#include "../Base/AutoClean.h"
#include "../YDExamObjRef/YDClassRef.h"
#include "../YDExamObjRef/YDDepartmentRef.h"
#include "../ObjHelper/StaticObjHelper.h"

// CQueryByClass dialog

IMPLEMENT_DYNAMIC(CQueryByClass, CDialog)

CQueryByClass::CQueryByClass(CWnd* pParent /*=NULL*/)
	: CDialog(CQueryByClass::IDD, pParent)
{

}

CQueryByClass::~CQueryByClass()
{
	CListAutoClean<CYDObjectRef> clr1(m_lstDeptCache);
	CListAutoClean<CYDObjectRef> clr2(m_lstClsCache);
	CListAutoClean<CYDObjectRef> clr3(m_lstSelClsCache);
}

void CQueryByClass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_DM_QMARK_SEL_CLASS, m_SelClassList);
	DDX_Control(pDX, IDC_LIST_DM_QMARK_ALL_CLASS, m_AllClassList);
	DDX_Control(pDX, IDC_CMB_DM_QMARK_DEPT, m_cmbDept);
}


BEGIN_MESSAGE_MAP(CQueryByClass, CDialog)
	ON_BN_CLICKED(IDC_BTN_DM_QMARK_ADD, &CQueryByClass::OnBnClickedBtnDmQmarkAdd)
	ON_BN_CLICKED(IDC_BTN_DM_QMARK_DEL, &CQueryByClass::OnBnClickedBtnDmQmarkDel)
	ON_CBN_SELCHANGE(IDC_CMB_DM_QMARK_DEPT, &CQueryByClass::OnCbnSelchangeCmbDmQmarkDept)
END_MESSAGE_MAP()


// CQueryByClass message handlers

INT_PTR CQueryByClass::DoModal()
{
	CExtDllState extdllstate;

	return CDialog::DoModal();
}

void CQueryByClass::OnBnClickedBtnDmQmarkAdd()
{
	POSITION pos = m_AllClassList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(L"请选择要查询的班级！");
	}
	else
	{
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		while (pos)
		{
			int nItem = m_AllClassList.GetNextSelectedItem(pos);
			CYDClassRef* pCls = (CYDClassRef*)m_AllClassList.GetItemData(nItem);
			if (IsClassExist(pCls))
			{
				continue;
			}
			CYDClassRef* pClsCopy = new CYDClassRef(pDB);
			OBJID objID;
			pCls->GetID(&objID);
			pClsCopy->SetID(objID);
			m_setExistCls.insert(objID);
			AddSelClassItem(pClsCopy);
		}
	}
}

BOOL CQueryByClass::IsClassExist(CYDObjectRef* pStu)
{
	OBJID objID;
	pStu->GetID(&objID);
	std::set<OBJID>::iterator itr = m_setExistCls.find(objID);
	if (itr != m_setExistCls.end())
	{
		return TRUE;
	}

	return FALSE;
}

HRESULT CQueryByClass::RemoveSelClass(CYDObjectRef* pStu)
{
	OBJID objID;
	pStu->GetID(&objID);
	std::set<OBJID>::iterator itr = m_setExistCls.find(objID);
	if (itr != m_setExistCls.end())
	{
		m_setExistCls.erase(itr);
	}
	std::list<CYDObjectRef*>::iterator itrLst = m_lstSelClsCache.begin();
	for (; itrLst != m_lstSelClsCache.end(); ++itrLst)
	{
		UINT selObjID;
		(*itrLst)->GetID(&selObjID);
		if (objID == selObjID)
		{
			delete pStu;
			pStu = NULL;
			m_lstSelClsCache.erase(itrLst);
			return S_OK;
		}
	}

	return S_OK;
}

void CQueryByClass::OnBnClickedBtnDmQmarkDel()
{
	POSITION pos = m_SelClassList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(L"请选择删除的查询班级！");
	}
	else
	{
		while (pos)
		{
			int nItem = m_SelClassList.GetNextSelectedItem(pos);
			CYDClassRef* pClassRef = (CYDClassRef*)m_SelClassList.GetItemData(nItem);
			RemoveSelClass(pClassRef);
			m_SelClassList.DeleteItem(nItem);
		}
	}
}

void CQueryByClass::OnCbnSelchangeCmbDmQmarkDept()
{
	CString strSelDept;
	m_cmbDept.GetWindowText(strSelDept);

	if (strSelDept.GetLength() > 0)
	{
		{
			CListAutoClean<CYDObjectRef> clr(m_lstClsCache);
		}
		CYDDepartmentRef* pDept = (CYDDepartmentRef*)m_cmbDept.GetItemData(m_cmbDept.GetCurSel());
		ASSERT(pDept);
		HRESULT hr = pDept->GetAllClasses(&m_lstClsCache, NULL);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		if (m_lstClsCache.size() > 0)
		{
			for (std::list<CYDObjectRef*>::const_iterator itr = m_lstClsCache.begin();
				itr != m_lstClsCache.end(); ++itr)
			{
				AddClassItem(*itr);
			}
		}
	}
}

HRESULT CQueryByClass::AddClassItem(CYDObjectRef* pObjRef)
{
	HRESULT hr = E_FAIL;
	CString strClassID;
	hr = pObjRef->GetPropVal(FIELD_YDCLASS_CLASSID, strClassID);
	if(FAILED(hr))
	{
		return hr;
	}

	CString strclassname;
	hr = pObjRef->GetPropVal(FIELD_YDCLASS_NAME, strclassname);
	if(FAILED(hr))
	{
		return hr;
	}

	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_AllClassList.GetItemCount()+1);
	int index = m_AllClassList.InsertItem(m_AllClassList.GetItemCount(), strIndex);
	m_AllClassList.SetItemText(index,1, strClassID);
	m_AllClassList.SetItemText(index,2, strclassname);

	m_AllClassList.SetItemData(index,(DWORD_PTR)pObjRef);

	return S_OK;
}

HRESULT CQueryByClass::AddSelClassItem(CYDObjectRef* pObjRef)
{
	HRESULT hr = E_FAIL;
	CString strClassID;
	hr = pObjRef->GetPropVal(FIELD_YDCLASS_CLASSID, strClassID);
	if(FAILED(hr))
	{
		return hr;
	}

	CString strclassname;
	hr = pObjRef->GetPropVal(FIELD_YDCLASS_NAME, strclassname);
	if(FAILED(hr))
	{
		return hr;
	}

	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_SelClassList.GetItemCount()+1);
	int index = m_SelClassList.InsertItem(m_SelClassList.GetItemCount(), strIndex);
	m_SelClassList.SetItemText(index,1, strClassID);
	m_SelClassList.SetItemText(index,2, strclassname);

	m_SelClassList.SetItemData(index,(DWORD_PTR)pObjRef);
	m_lstSelClsCache.push_back(pObjRef);
	return S_OK;
}


BOOL CQueryByClass::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化部门下拉框
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	HRESULT hr = CStaticObjHelper::GetObjRef(DB_YDDEPARTMENT, pDB, &m_lstDeptCache);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	if (m_lstDeptCache.size() > 0)
	{
		int nIndex = 0;
		m_cmbDept.InsertString(nIndex++, L"");
		for (std::list<CYDObjectRef*>::const_iterator itr=m_lstDeptCache.begin();
			itr != m_lstDeptCache.end(); ++itr, ++nIndex)
		{
			CString strDeptName;
			(*itr)->GetPropVal(FIELD_YDDEPARTMENT_NAME, strDeptName);
			m_cmbDept.InsertString(nIndex, strDeptName);
			m_cmbDept.SetItemData(nIndex, (DWORD_PTR)(*itr));
		}
	}

	CRect rect1;
	m_AllClassList.GetWindowRect(rect1);
	int iWidth = rect1.Width();
	m_AllClassList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_AllClassList.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/6);
	m_AllClassList.InsertColumn(1, L"编号", LVCFMT_LEFT,iWidth*2/6);
	m_AllClassList.InsertColumn(2, L"名称", LVCFMT_LEFT,iWidth*3/6 - 2);

	m_SelClassList.GetWindowRect(rect1);
	iWidth = rect1.Width();
	m_SelClassList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_SelClassList.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/6);
	m_SelClassList.InsertColumn(1, L"编号", LVCFMT_LEFT,iWidth*2/6);
	m_SelClassList.InsertColumn(2, L"名称", LVCFMT_LEFT,iWidth*3/6 - 2);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CQueryByClass::GetSearchClass(std::list<OBJID>* pListClass)
{
	pListClass->clear();
	UpdateData();
	HRESULT hr = E_FAIL;
	int uSize = m_SelClassList.GetItemCount();
	for (int i=0; i<uSize; ++i)
	{
		CYDClassRef* pClassRef = (CYDClassRef*)(m_SelClassList.GetItemData(i));
		ASSERT(pClassRef != NULL);
		UINT objID;
		hr = pClassRef->GetID(&objID);
		if (FAILED(hr))
		{
			return hr;
		}
		pListClass->push_back(objID);
	}

	return S_OK;
}