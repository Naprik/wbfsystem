// QuestionKnowledgeListFormView.cpp : implementation file
//

#include "stdafx.h"
#include "QuestionKnowledgeListFormView.h"
#include "../objref/YdObjWrapper.h"
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef/YDLinkRef.h"
#include "../Include/ShowErr.h"
#include "../ObjRef/YDQuestionVault.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YdKnowledge.h"
#include "../ObjRef/YDQuestionRef.h"


// CQuestionKnowledgeListFormView

IMPLEMENT_DYNCREATE(CQuestionKnowledgeListFormView, CYdFormView)

CQuestionKnowledgeListFormView::CQuestionKnowledgeListFormView()
	: CYdFormView(CQuestionKnowledgeListFormView::IDD)
{

}

CQuestionKnowledgeListFormView::~CQuestionKnowledgeListFormView()
{
	CListAutoClean<CYdKnowledge> clr(m_lstKnowledgeRef);
}

void CQuestionKnowledgeListFormView::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST_ALL_KNOWLEDGE, m_lstAllKnowledge);
	DDX_Control(pDX, IDC_LIST_RElATE_KNOWLEDGE, m_lstRelateKnowledge);
}

BEGIN_MESSAGE_MAP(CQuestionKnowledgeListFormView, CYdFormView)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CQuestionKnowledgeListFormView::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CQuestionKnowledgeListFormView::OnBnClickedButtonDel)
END_MESSAGE_MAP()


// CQuestionKnowledgeListFormView diagnostics

#ifdef _DEBUG
void CQuestionKnowledgeListFormView::AssertValid() const
{
	CYdFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CQuestionKnowledgeListFormView::Dump(CDumpContext& dc) const
{
	CYdFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CQuestionKnowledgeListFormView message handlers
void CQuestionKnowledgeListFormView::ReadonlyPage(BOOL bReadonly)
{
	return;
}

HRESULT CQuestionKnowledgeListFormView::UpdateProp(BOOL bUpdate) 
{
	return S_OK;
}

void CQuestionKnowledgeListFormView::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();
	HRESULT hr = E_FAIL;
	m_lstAllKnowledge.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lstAllKnowledge.InsertColumn(0,_T("序号"),LVCFMT_LEFT,50);
	m_lstAllKnowledge.InsertColumn(1,_T("名称"),LVCFMT_LEFT,150);

	m_lstRelateKnowledge.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lstRelateKnowledge.InsertColumn(0,_T("序号"),LVCFMT_LEFT,50);
	m_lstRelateKnowledge.InsertColumn(1,_T("名称"),LVCFMT_LEFT,150);
	CYdObjWrapper* pObjWrapper = m_pSheet->GetCurObjWrapper();
	ASSERT(pObjWrapper);
	CYDObjectRef* pObjRef = pObjWrapper->m_pObjRef;
	ASSERT(pObjRef);
	CYDLinkRef* pLinkRef = pObjWrapper->m_pLinkRef;
	ASSERT(pLinkRef != NULL);
	CYDObjectRef* pObjRefQuestionVault ,*pObjRefQuestion;
	hr = pLinkRef->GetObjRef(pObjRefQuestionVault,pObjRefQuestion);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	ASSERT(pObjRefQuestionVault);
	ASSERT(pObjRefQuestionVault->IsKindOf(RUNTIME_CLASS(CYDQuestionVault)));
	CYDQuestionVault* pYDQuestionVault = (CYDQuestionVault*)pObjRefQuestionVault;
	hr = pYDQuestionVault->GetAllKnowledgePoint(&m_lstKnowledgeRef);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	
	int iCount = 0;
	for(std::list<CYdKnowledge*>::const_iterator itr = m_lstKnowledgeRef.begin();
		itr != m_lstKnowledgeRef.end();++itr,++iCount)
	{
		CString strIndex;
		strIndex.Format(_T("%d"),iCount+1);
		int index = m_lstAllKnowledge.InsertItem(iCount,strIndex);
		CString strLabel;
		hr = (*itr)->GetLabel(&strLabel);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}
		m_lstAllKnowledge.SetItemText(index,1,strLabel);
		m_lstAllKnowledge.SetItemData(index,(DWORD_PTR)(*itr));
	}
	
	return ;
}
void CQuestionKnowledgeListFormView::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	int nCount = m_lstAllKnowledge.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在左边列表中选择一条知识点增加！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_lstAllKnowledge.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_lstAllKnowledge.GetNextSelectedItem(pos);
	CYdKnowledge* pKnowledge = (CYdKnowledge*)m_lstAllKnowledge.GetItemData(nItem);
	ASSERT(pKnowledge);
	//判断是否在右边列表中
	for(int i =0 ;i < m_lstRelateKnowledge.GetItemCount();++i)
	{
		CYdKnowledge* pIKnowledge = (CYdKnowledge*)m_lstRelateKnowledge.GetItemData(i);
		ASSERT(pIKnowledge);
		if(pIKnowledge == pKnowledge)
		{
			AfxMessageBox(_T("该知识点已经被关联!"));
			return;
		}
	}
	int iCount = m_lstRelateKnowledge.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"),iCount+1);
	int index = m_lstRelateKnowledge.InsertItem(iCount,strIndex);
	CString strLabel;
	hr = pKnowledge->GetLabel(&strLabel);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
	}
	m_lstRelateKnowledge.SetItemText(index,1,strLabel);
	m_lstRelateKnowledge.SetItemData(index,DWORD_PTR(pKnowledge));
}

void CQuestionKnowledgeListFormView::OnBnClickedButtonDel()
{
	// TODO: Add your control notification handler code here
	int nCount = m_lstRelateKnowledge.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在右边列表中选择一条知识点删除！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_lstRelateKnowledge.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_lstRelateKnowledge.GetNextSelectedItem(pos);
	m_lstRelateKnowledge.DeleteItem(nItem);
}

HRESULT CQuestionKnowledgeListFormView::Save()
{
	HRESULT hr = E_FAIL;
	CYdObjWrapper* pObjWrapper = m_pSheet->GetCurObjWrapper();
	ASSERT(pObjWrapper);
	CYDObjectRef* pObjRef = pObjWrapper->m_pObjRef;
	ASSERT(pObjRef);
	ASSERT(pObjRef->IsKindOf(RUNTIME_CLASS(CYDQuestionRef)));
	CComVariant valTYPEID;//题型ID
	hr = pObjRef->GetPropVal(L"TYPEID",&valTYPEID);
	if(FAILED(hr))
	{
		return hr;
	}
	for(int i = 0; i < m_lstRelateKnowledge.GetItemCount();++i)
	{
		CYdKnowledge* pKnowledge = (CYdKnowledge*)m_lstRelateKnowledge.GetItemData(i);
		ASSERT(pKnowledge);
		//创建一个关系
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CYDLinkRef* pLinkref = new CYDLinkRef(pDB,DB_QUESTIONKPOINT);
		hr = pLinkref->AddPropDef(_T("ID_TYPE"),VT_UINT);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pLinkref->SetPropVal(L"ID_TYPE",&valTYPEID);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pLinkref->PutObjRef(pObjRef,pKnowledge);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pLinkref->Save();
		if(FAILED(hr))
		{
			return hr;
		}
		CPtrAutoClean<CYDLinkRef> clr(pLinkref);
	}
	return S_OK;
}

BOOL CQuestionKnowledgeListFormView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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