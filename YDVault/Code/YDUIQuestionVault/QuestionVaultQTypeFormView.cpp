// QuestionVaultQTypeFormView.cpp : implementation file
//

#include "stdafx.h"
#include "QuestionVaultQTypeFormView.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef/YDLinkRef.h"
#include "../ObjRef/YDQuestionVault.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../Include/ShowErr.h"
#include "../ObjRef/YDQuestionType.h"
#include "../DBBase/DBTransactionRef.h"
#include "../Base/DataHandler.h"
#include "DlgYDVaultFactorInfoConfig.h"


// CQuestionVaultQTypeFormView

IMPLEMENT_DYNCREATE(CQuestionVaultQTypeFormView, CYdFormView)

CQuestionVaultQTypeFormView::CQuestionVaultQTypeFormView()
	: CYdFormView(CQuestionVaultQTypeFormView::IDD)
{

}

CQuestionVaultQTypeFormView::~CQuestionVaultQTypeFormView()
{
	CListAutoClean<CYDObjectRef> clr1(m_lstClearObj);
	CListAutoClean<CYDLinkRef>	clr2(m_lstClearLink);
}

void CQuestionVaultQTypeFormView::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_QTYPE_EXIST, m_lstQTypeExist);
	DDX_Control(pDX, IDC_LIST_QTYPE_ALL, m_lstQTypeAll);
}

BEGIN_MESSAGE_MAP(CQuestionVaultQTypeFormView, CYdFormView)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CQuestionVaultQTypeFormView::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CQuestionVaultQTypeFormView::OnBnClickedButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_FACTORINFO_CONIFG, &CQuestionVaultQTypeFormView::OnBnClickedButtonFactorinfoConifg)
END_MESSAGE_MAP()


// CQuestionVaultQTypeFormView diagnostics

#ifdef _DEBUG
void CQuestionVaultQTypeFormView::AssertValid() const
{
	CYdFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CQuestionVaultQTypeFormView::Dump(CDumpContext& dc) const
{
	CYdFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CQuestionVaultQTypeFormView message handlers

void CQuestionVaultQTypeFormView::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	m_lstQTypeExist.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect1;
	GetDlgItem(IDC_LIST_QTYPE_ALL)->GetWindowRect(rect1);
	m_lstQTypeExist.InsertColumn(0,_T("序号"),LVCFMT_LEFT,rect1.Width()/10 -1);
	m_lstQTypeExist.InsertColumn(1,_T("名称"),LVCFMT_LEFT,rect1.Width()*3/10 -1);
	m_lstQTypeExist.InsertColumn(2,_T("创建者"),LVCFMT_LEFT,rect1.Width()/10 -1);
	m_lstQTypeExist.InsertColumn(3,_T("创建日期"),LVCFMT_LEFT,rect1.Width()*2/10 -1);
	m_lstQTypeExist.InsertColumn(4,_T("描述"),LVCFMT_LEFT,rect1.Width()*3/10 -1);

	m_lstQTypeAll.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lstQTypeAll.InsertColumn(0,_T("序号"),LVCFMT_LEFT,rect1.Width()/10 -1);
	m_lstQTypeAll.InsertColumn(1,_T("名称"),LVCFMT_LEFT,rect1.Width()*3/10 -1);
	m_lstQTypeAll.InsertColumn(2,_T("创建者"),LVCFMT_LEFT,rect1.Width()/10 -1);
	m_lstQTypeAll.InsertColumn(3,_T("创建日期"),LVCFMT_LEFT,rect1.Width()*2/10 -1);
	m_lstQTypeAll.InsertColumn(4,_T("描述"),LVCFMT_LEFT,rect1.Width()*3/10 -1);

	HRESULT hr = E_FAIL;
	std::list<CYDQuestionType*> lstAllQType;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	hr = CYDQuestionType::GetAllQType(pDB,&lstAllQType);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	for(std::list<CYDQuestionType*>::const_iterator itr = lstAllQType.begin();
		itr != lstAllQType.end();++itr)
	{
		hr = (*itr)->Restore();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		hr = InsertAllQType(*itr);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
	}

	CYdObjWrapper* pParentWrapper = m_pSheet->GetCurObjWrapper();
	ASSERT(pParentWrapper);
	CYDQuestionVault* pQV = (CYDQuestionVault*)pParentWrapper->m_pObjRef;
	ASSERT(pQV);
	std::list<CYDQuestionType*> lstExistQType;
	std::list<CYDLinkRef*>      lstExistLink;
	hr = pQV->GetAllQuestionType(&lstExistQType,&lstExistLink);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(lstExistQType.size() == lstExistLink.size());
	CListAutoClean<CYDQuestionType> clr(lstExistQType);
	std::list<CYDQuestionType*>::iterator itr = lstExistQType.begin();
	std::list<CYDLinkRef*>::iterator itrLink = lstExistLink.begin();
	for(;itr != lstExistQType.end() && itrLink != lstExistLink.end();
		 ++itr,++itrLink)
	{
		hr = RepalceQType(*itrLink,lstAllQType);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		hr = InsertExistQType(*itrLink);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
	}

}

void CQuestionVaultQTypeFormView::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	int nCount = m_lstQTypeAll.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在列表中选择一条要增加的题型！"),MB_OK|MB_ICONWARNING);
		return;
	}
	HRESULT hr = E_FAIL;
	POSITION pos = m_lstQTypeAll.GetFirstSelectedItemPosition();
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef TransRef(pDB,TRUE);
	while(pos)
	{
		int nItem = m_lstQTypeAll.GetNextSelectedItem(pos);
		CYDObjectRef* pObjRef = (CYDObjectRef*)m_lstQTypeAll.GetItemData(nItem);
		ASSERT(pObjRef != NULL);
		UINT uObjID = 0;
		hr = pObjRef->GetID(&uObjID);
		if(FAILED(hr))
		{
			TransRef.Rollback();
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}
		//要判断pObjRef是否已经在当前题型中了
		CYdObjWrapper* pParentWrapper = m_pSheet->GetCurObjWrapper();
		CYDQuestionVault* pQV = (CYDQuestionVault*)pParentWrapper->m_pObjRef;
		ASSERT(pQV);
		BOOL bFind = FALSE;
		std::list<CYDQuestionType*> lstType;
		hr = pQV->GetAllQuestionType(&lstType);
		if(FAILED(hr))
		{
			TransRef.Rollback();
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}
		CListAutoClean<CYDQuestionType> clr(lstType);
		for(std::list<CYDQuestionType*>::const_iterator itr = lstType.begin();
			itr != lstType.end();++itr)
		{
			UINT uItrID = 0;
			hr = (*itr)->GetID(&uItrID);
			if(FAILED(hr))
			{
				TransRef.Rollback();
				DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
				return ;
			}
			if(uItrID == uObjID)
			{
				AfxMessageBox(_T("选中的题型已经在当前题型中了，不能添加！"));
				return;
			}
		}
		//插入当前选择的题型到当前题型中
		CYDLinkRef* pLinkRef = new CYDLinkRef(pDB,DB_QVAULTQTYPE);
		hr = pLinkRef->PutObjRef(pParentWrapper->m_pObjRef,pObjRef);
		if(FAILED(hr))
		{
			TransRef.Rollback();
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}
		hr = pLinkRef->Save();
		if(FAILED(hr))
		{
			TransRef.Rollback();
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}
		hr = InsertExistQType(pLinkRef);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}
	}
	hr = TransRef.Commit();
	if(FAILED(hr))
	{
		TransRef.Rollback();
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}

}

void CQuestionVaultQTypeFormView::OnBnClickedButtonRemove()
{
	// TODO: Add your control notification handler code here
	int nCount = m_lstQTypeExist.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在列表中选择一条要移除的题型！"),MB_OK|MB_ICONWARNING);
		return;
	}
	if(AfxMessageBox(_T("确定要移除当前选择的题型吗？"),MB_YESNO) != IDYES)
	{
		return;
	}
	HRESULT hr = E_FAIL;
	POSITION pos = m_lstQTypeExist.GetFirstSelectedItemPosition();
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef TransRef(pDB,TRUE);
	while(pos)
	{
		int nItem = m_lstQTypeExist.GetNextSelectedItem(pos);
		CYDLinkRef* pLinkRef = (CYDLinkRef*)m_lstQTypeExist.GetItemData(nItem);
		ASSERT(pLinkRef != NULL);
		hr = pLinkRef->Remove();
		if(FAILED(hr))
		{
			TransRef.Rollback();
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}
		m_lstQTypeExist.DeleteItem(nItem);
		pos = m_lstQTypeExist.GetFirstSelectedItemPosition();
	}
	hr = TransRef.Commit();
	if(FAILED(hr))
	{
		TransRef.Rollback();
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
}

HRESULT CQuestionVaultQTypeFormView::InsertExistQType(CYDLinkRef* _pLinkRef)
{
	HRESULT hr = E_FAIL;
	int iCount = m_lstQTypeExist.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"),iCount+1);
	int iIndex = m_lstQTypeExist.InsertItem(iCount,strIndex);
	CYDObjectRef* pQV = NULL;
	CYDObjectRef* pQType = NULL;
	hr = _pLinkRef->GetObjRef(pQV,pQType);
	ASSERT(pQType != NULL);
	CComVariant valName;
	hr = pQType->GetPropVal(L"name",&valName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strName = CDataHandler::VariantToString(valName);
	m_lstQTypeExist.SetItemText(iIndex,1,strName);

	CComVariant valCreator;
	hr = pQType->GetPropVal(L"CREATOR",&valCreator);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strCreator = CDataHandler::VariantToString(valCreator);
	if (strCreator.GetLength() <= 0)
	{
		strCreator = DEFAULT_CREATOR;
	}
	m_lstQTypeExist.SetItemText(iIndex,2,strCreator);

	CComVariant valCreatDate;
	hr = pQType->GetPropVal(L"CREATEDATE",&valCreatDate);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strCreatDate = CDataHandler::VariantToString(valCreatDate);
	m_lstQTypeExist.SetItemText(iIndex,3,strCreatDate);

	CComVariant valDescription;
	hr = pQType->GetPropVal(L"DESCRIPTION",&valDescription);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strDescription = CDataHandler::VariantToString(valDescription);
	m_lstQTypeExist.SetItemText(iIndex,4,strDescription);
	m_lstQTypeExist.SetItemData(iIndex,(DWORD_PTR)_pLinkRef);
	m_lstClearLink.push_back(_pLinkRef);
	return S_OK;
}
HRESULT CQuestionVaultQTypeFormView::InsertAllQType(CYDObjectRef* _pObjRef)
{
	HRESULT hr = E_FAIL;
	int iCount = m_lstQTypeAll.GetItemCount();
	CString strIndex;
	strIndex.Format(_T("%d"),iCount+1);
	int iIndex = m_lstQTypeAll.InsertItem(iCount,strIndex);
	CComVariant valName;
	hr = _pObjRef->GetPropVal(L"name",&valName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strName = CDataHandler::VariantToString(valName);
	m_lstQTypeAll.SetItemText(iIndex,1,strName);


	CComVariant valCreator;
	hr = _pObjRef->GetPropVal(L"CREATOR",&valCreator);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strCreator = CDataHandler::VariantToString(valCreator);
	if (strCreator.GetLength() <= 0)
	{
		strCreator = DEFAULT_CREATOR;
	}
	m_lstQTypeAll.SetItemText(iIndex,2,strCreator);

	CComVariant valCreatDate;
	hr = _pObjRef->GetPropVal(L"CREATEDATE",&valCreatDate);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strCreatDate = CDataHandler::VariantToString(valCreatDate);
	m_lstQTypeAll.SetItemText(iIndex,3,strCreatDate);

	CComVariant valDescription;
	hr = _pObjRef->GetPropVal(L"DESCRIPTION",&valDescription);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strDescription = CDataHandler::VariantToString(valDescription);
	m_lstQTypeAll.SetItemText(iIndex,4,strDescription);
	m_lstQTypeAll.SetItemData(iIndex,(DWORD_PTR)_pObjRef);
	m_lstClearObj.push_back(_pObjRef);
	return S_OK;
}

HRESULT CQuestionVaultQTypeFormView::RepalceQType(CYDLinkRef* _pLinkRef,std::list<CYDQuestionType*> _lstAllQType)
{
	HRESULT hr = E_FAIL;
	CYDObjectRef* pQV = NULL;
	CYDObjectRef* pQType = NULL;
	hr = _pLinkRef->GetObjRef(pQV,pQType);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pQV != NULL && pQType != NULL);
	UINT uID = 0;
	hr = pQType->GetID(&uID);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CYDQuestionType*>::const_iterator itr = _lstAllQType.begin();
		itr != _lstAllQType.end();++itr)
	{
		UINT uItrID = 0;
		hr = (*itr)->GetID(&uItrID);
		if(FAILED(hr))
		{
			return hr;
		}
		if(uItrID == uID)
		{
			hr = _pLinkRef->PutObjRef(pQV,*itr);
			if(FAILED(hr))
			{
				return hr;
			}
			return S_OK;
		}
	}
	ASSERT(FALSE);//_lstAllQType中一定有一个
	return S_FALSE;
}

void CQuestionVaultQTypeFormView::ReadonlyPage(BOOL bReadonly)
{
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(!bReadonly);
	GetDlgItem(IDC_BUTTON_REMOVE)->EnableWindow(!bReadonly);
	GetDlgItem(IDC_BUTTON_FACTORINFO_CONIFG)->EnableWindow(!bReadonly);
}

BOOL CQuestionVaultQTypeFormView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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

void CQuestionVaultQTypeFormView::OnBnClickedButtonFactorinfoConifg()
{
	// TODO: Add your control notification handler code here
	//题型指标配置
	CYdObjWrapper* pParentWrapper = m_pSheet->GetCurObjWrapper();
	ASSERT(pParentWrapper);
	CYDQuestionVault* pQV = (CYDQuestionVault*)pParentWrapper->m_pObjRef;
	ASSERT(pQV);

	CDlgYDVaultFactorInfoConfig dlg;
	dlg.m_pVault = pQV;
	dlg.DoModal();
}
