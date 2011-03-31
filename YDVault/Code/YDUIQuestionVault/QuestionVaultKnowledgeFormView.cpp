// QuestionVaultKnowledgeFormView.cpp : implementation file
//

#include "stdafx.h"
#include "QuestionVaultKnowledgeFormView.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../ObjRef/YDObjectRef.h"
#include "../Include/ShowErr.h"
#include "../Base/DataHandler.h"
#include "DlgKnowledge.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YDQuestionVault.h"
#include "../DBBase\DBTransactionRef.h"


// CQuestionVaultKnowledgeFormView

IMPLEMENT_DYNCREATE(CQuestionVaultKnowledgeFormView, CYdFormView)

CQuestionVaultKnowledgeFormView::CQuestionVaultKnowledgeFormView()
	: CYdFormView(CQuestionVaultKnowledgeFormView::IDD)
{

}

CQuestionVaultKnowledgeFormView::~CQuestionVaultKnowledgeFormView()
{
	CListAutoClean<CYdObjWrapper> clr(m_lstObjWrapper);
}

void CQuestionVaultKnowledgeFormView::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_KNOWLEDGE, m_lstKnowledge);
}

BEGIN_MESSAGE_MAP(CQuestionVaultKnowledgeFormView, CYdFormView)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_KNOWLEDGE, &CQuestionVaultKnowledgeFormView::OnLvnItemchangedListKnowledge)
	ON_BN_CLICKED(ID_BUTTON_NEW, &CQuestionVaultKnowledgeFormView::OnBnClickedButtonNew)
	ON_BN_CLICKED(ID_BUTTON_MODIFY, &CQuestionVaultKnowledgeFormView::OnBnClickedButtonModify)
	ON_BN_CLICKED(ID_BUTTON_DEL, &CQuestionVaultKnowledgeFormView::OnBnClickedButtonDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_KNOWLEDGE, &CQuestionVaultKnowledgeFormView::OnNMDblclkListKnowledge)
END_MESSAGE_MAP()


// CQuestionVaultKnowledgeFormView diagnostics

#ifdef _DEBUG
void CQuestionVaultKnowledgeFormView::AssertValid() const
{
	CYdFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CQuestionVaultKnowledgeFormView::Dump(CDumpContext& dc) const
{
	CYdFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CQuestionVaultKnowledgeFormView message handlers

void CQuestionVaultKnowledgeFormView::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	CRect rect1;
	GetDlgItem(IDC_LIST_KNOWLEDGE)->GetWindowRect(rect1);
	m_lstKnowledge.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lstKnowledge.InsertColumn(0,_T("序号"),LVCFMT_LEFT,rect1.Width()/12 -1);
	m_lstKnowledge.InsertColumn(1,_T("名称"),LVCFMT_LEFT,rect1.Width()*3/12 -1);
	m_lstKnowledge.InsertColumn(2,_T("编号"),LVCFMT_LEFT,rect1.Width()/12 -1);
	m_lstKnowledge.InsertColumn(3,_T("创建者"),LVCFMT_LEFT,rect1.Width()*2/12 -1);
	m_lstKnowledge.InsertColumn(4,_T("创建日期"),LVCFMT_LEFT,rect1.Width()*2/12 -1);
	m_lstKnowledge.InsertColumn(5,_T("描述"),LVCFMT_LEFT,rect1.Width()*3/12 -1);

	CYdObjWrapper* pParentWrapper = m_pSheet->GetCurObjWrapper();
	ASSERT(pParentWrapper);
	CYDQuestionVault* pQV = (CYDQuestionVault*)pParentWrapper->m_pObjRef;
	ASSERT(pQV);
	std::list<CYdKnowledge*> lstKnowledge;
	std::list<CYDLinkRef*>   lstLinkref;
	HRESULT hr = E_FAIL;
	hr = pQV->GetAllKnowledgePoint(&lstKnowledge,&lstLinkref);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(lstKnowledge.size() == lstLinkref.size());
	std::list<CYdKnowledge*>::iterator itr = lstKnowledge.begin();
	std::list<CYDLinkRef*>::iterator itrLink = lstLinkref.begin();
	for(;itr != lstKnowledge.end() && itrLink != lstLinkref.end();++itr,++itrLink)
	{
		CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
		pObjWrapper->m_pRelatedWnd = &m_lstKnowledge;
		pObjWrapper->m_pObjRef = (CYDObjectRef*)(*itr);
		pObjWrapper->m_pLinkRef = (*itrLink);
		hr = pObjWrapper->m_pObjRef->Restore();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		hr = InsertObj(pObjWrapper);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
	}
}

void CQuestionVaultKnowledgeFormView::ReadonlyPage(BOOL bReadonly)
{
	GetDlgItem(ID_BUTTON_NEW)->EnableWindow(!bReadonly);	
	GetDlgItem(ID_BUTTON_MODIFY)->EnableWindow(!bReadonly);
	GetDlgItem(ID_BUTTON_DEL)->EnableWindow(!bReadonly);
}

void CQuestionVaultKnowledgeFormView::OnLvnItemchangedListKnowledge(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	HRESULT hr = E_FAIL;
	if(pNMLV->uNewState & LVIS_SELECTED )
	{

	
		UpdateData(FALSE);

	}
}

void CQuestionVaultKnowledgeFormView::OnBnClickedButtonNew()
{
	// TODO: Add your control notification handler code here
	CYdObjWrapper* pParentWrapper = m_pSheet->GetCurObjWrapper();
	ASSERT(pParentWrapper);
	CDlgKnowledge dlg;
	dlg.m_pParentObjRef = pParentWrapper->m_pObjRef;
	dlg.m_uType = OP_NEW;
	if(dlg.DoModal() != IDOK)
	{
		return ;
	}
	HRESULT hr = E_FAIL;
	ASSERT(dlg.m_pObjWrapper);
	hr = InsertObj(dlg.m_pObjWrapper);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
}

void CQuestionVaultKnowledgeFormView::OnBnClickedButtonModify()
{
	// TODO: Add your control notification handler code here
	int nCount = m_lstKnowledge.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在列表中选择一条知识点修改！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_lstKnowledge.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_lstKnowledge.GetNextSelectedItem(pos);
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_lstKnowledge.GetItemData(nItem);
	ASSERT(pObjWrapper != NULL);
	CYdObjWrapper* pParentWrapper = m_pSheet->GetCurObjWrapper();
	ASSERT(pParentWrapper);
	CDlgKnowledge dlg;
	dlg.m_pParentObjRef = pParentWrapper->m_pObjRef;
	dlg.m_pObjWrapper = pObjWrapper;
	dlg.m_uType = OP_EDIT;
	if(dlg.DoModal() != IDOK)
	{
		return ;
	}
	HRESULT hr = E_FAIL;
	hr = UpdateObj(nItem,dlg.m_pObjWrapper);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
}

void CQuestionVaultKnowledgeFormView::OnBnClickedButtonDel()
{
	// TODO: Add your control notification handler code here
	int nCount = m_lstKnowledge.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在列表中选择一条知识点删除！"),MB_OK|MB_ICONWARNING);
		return;
	}
	if(AfxMessageBox(_T("你确定要删除选中的知识点吗？"),MB_YESNO|MB_ICONINFORMATION) != IDYES)
	{
		return;
	}
	POSITION pos = m_lstKnowledge.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_lstKnowledge.GetNextSelectedItem(pos);
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_lstKnowledge.GetItemData(nItem);
	ASSERT(pObjWrapper != NULL);
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef TransRef(pDB,TRUE);
	hr = pObjWrapper->Remove();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	hr = TransRef.Commit();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	m_lstKnowledge.DeleteItem(nItem);
}

HRESULT CQuestionVaultKnowledgeFormView::InsertObj(CYdObjWrapper* _pWrapper)
{
	HRESULT hr = E_FAIL;
	int iCount = m_lstKnowledge.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"),iCount+1);
	int iIndex = m_lstKnowledge.InsertItem(iCount,strIndex);
	CComVariant valName;
	hr = _pWrapper->m_pObjRef->GetPropVal(L"name",&valName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strName = CDataHandler::VariantToString(valName);
	m_lstKnowledge.SetItemText(iIndex,1,strName);
	
	CComVariant valCode;
	hr = _pWrapper->m_pObjRef->GetPropVal(L"CODE",&valCode);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strCode = CDataHandler::VariantToString(valCode);
	m_lstKnowledge.SetItemText(iIndex,2,strCode);

	CComVariant valCreator;
	hr = _pWrapper->m_pObjRef->GetPropVal(L"CREATOR",&valCreator);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strCreator = CDataHandler::VariantToString(valCreator);
	if (strCreator.GetLength() <= 0)
	{
		strCreator = DEFAULT_CREATOR;
	}
	m_lstKnowledge.SetItemText(iIndex,3,strCreator);

	CComVariant valCreatDate;
	hr = _pWrapper->m_pObjRef->GetPropVal(L"CREATEDATE",&valCreatDate);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strCreatDate = CDataHandler::VariantToString(valCreatDate);
	m_lstKnowledge.SetItemText(iIndex,4,strCreatDate);

	CComVariant valDescription;
	hr = _pWrapper->m_pObjRef->GetPropVal(L"DESCRIPTION",&valDescription);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strDescription = CDataHandler::VariantToString(valDescription);
	m_lstKnowledge.SetItemText(iIndex,4,strDescription);
	m_lstKnowledge.SetItemData(iIndex,(DWORD_PTR)_pWrapper);
	m_lstObjWrapper.push_back(_pWrapper);
	return S_OK;
}


HRESULT CQuestionVaultKnowledgeFormView::UpdateObj(int _index,CYdObjWrapper* _pWrapper)
{
	HRESULT hr = E_FAIL;
	CString strIndex;
	strIndex.Format(_T("%d"),_index+1);
	m_lstKnowledge.SetItemText(_index,1,strIndex);
	CComVariant valName;
	hr = _pWrapper->m_pObjRef->GetPropVal(L"name",&valName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strName = CDataHandler::VariantToString(valName);
	m_lstKnowledge.SetItemText(_index,1,strName);

	CComVariant valCode;
	hr = _pWrapper->m_pObjRef->GetPropVal(L"CODE",&valCode);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strCode = CDataHandler::VariantToString(valCode);
	m_lstKnowledge.SetItemText(_index,2,strCode);

	CComVariant valCreator;
	hr = _pWrapper->m_pObjRef->GetPropVal(L"CREATOR",&valCreator);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strCreator = CDataHandler::VariantToString(valCreator);
	if (strCreator.GetLength() <= 0)
	{
		strCreator = DEFAULT_CREATOR;
	}
	m_lstKnowledge.SetItemText(_index,3,strCreator);

	CComVariant valCreatDate;
	hr = _pWrapper->m_pObjRef->GetPropVal(L"CREATEDATE",&valCreatDate);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strCreatDate = CDataHandler::VariantToString(valCreatDate);
	m_lstKnowledge.SetItemText(_index,4,strCreatDate);

	CComVariant valDescription;
	hr = _pWrapper->m_pObjRef->GetPropVal(L"DESCRIPTION",&valDescription);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strDescription = CDataHandler::VariantToString(valDescription);
	m_lstKnowledge.SetItemText(_index,4,strDescription);
	m_lstKnowledge.SetItemData(_index,(DWORD_PTR)_pWrapper);
	return S_OK;
}
void CQuestionVaultKnowledgeFormView::OnNMDblclkListKnowledge(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem <0)
		return;
	*pResult = 0;
	CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_lstKnowledge.GetItemData(pNMListView->iItem);
	ASSERT(pObjWrapper != NULL);
	CYdObjWrapper* pParentWrapper = m_pSheet->GetCurObjWrapper();
	ASSERT(pParentWrapper);
	CDlgKnowledge dlg;
	dlg.m_pParentObjRef = pParentWrapper->m_pObjRef;
	dlg.m_pObjWrapper = pObjWrapper;
	dlg.m_uType = OP_VIEW;
	if(dlg.DoModal() != IDOK)
	{
		return ;
	}
}

BOOL CQuestionVaultKnowledgeFormView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
												  DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
												  CCreateContext* pContext)
{
	CExtDllState state;
	return CFormView::Create(lpszClassName,
		lpszWindowName,
		dwRequestedStyle,
		rect,
		pParentWnd,
		nID,
		pContext);
}