#include "StdAfx.h"
#include "ListCtrlOperate.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../ObjRef/YDObjectRef.h"
#include "../base/AutoClean.h"

CListCtrlOperate::CListCtrlOperate(CListCtrl* _pListCtrl)
{
	m_pListCtrl = _pListCtrl;
}

CListCtrlOperate::~CListCtrlOperate(void)
{
	ClearDataCache();
}

HRESULT CListCtrlOperate::ClearDataCache()
{
	CListAutoClean<CYdObjWrapper > clr(m_lstClear);

	return S_OK;
}

HRESULT CListCtrlOperate::AddItem(CYdObjWrapper* _pWrapper)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pListCtrl);
	int iCount = m_pListCtrl->GetItemCount();
	ASSERT(_pWrapper);
	ASSERT(_pWrapper->m_pObjRef);
	CString strLabel;
	hr = _pWrapper->m_pObjRef->GetLabel(&strLabel);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strIndex;
	strIndex.Format(_T("%d"),iCount+1);
	int index = m_pListCtrl->InsertItem(iCount,strIndex);
	m_pListCtrl->SetItemText(index,1,strLabel);
	m_pListCtrl->SetItemData(index,(DWORD_PTR)_pWrapper);
	m_lstClear.push_back(_pWrapper);
	return S_OK;
}

HRESULT CListCtrlOperate::ModifyItem(int _index,CYdObjWrapper* _pWrapper)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pListCtrl);
	ASSERT(_pWrapper);
	ASSERT(_pWrapper->m_pObjRef);
	CString strLabel;
	hr = _pWrapper->m_pObjRef->GetLabel(&strLabel);
	if(FAILED(hr))
	{
		return hr;
	}
	m_pListCtrl->SetItemText(_index,1,strLabel);
	return S_OK;
}

HRESULT CListCtrlOperate::DeleteItem(int _index)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pListCtrl);
	m_pListCtrl->DeleteItem(_index);
	return S_OK;
}

HRESULT CListCtrlOperate::RemoveAllItem()
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pListCtrl);
	hr = ClearDataCache();
	if(FAILED(hr))
	{
		return hr;
	}
	m_pListCtrl->DeleteAllItems();
	return S_OK;
}