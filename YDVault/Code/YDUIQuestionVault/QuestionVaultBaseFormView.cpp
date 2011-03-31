// QuestionVaultBaseFormView.cpp : implementation file
//

#include "stdafx.h"
#include "../ObjRef/YDObjectRef.h"
#include "QuestionVaultBaseFormView.h"
#include "../ObjRef/YDLinkRef.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../Base/DataHandler.h"


// CQuestionVaultBaseFormView

IMPLEMENT_DYNCREATE(CQuestionVaultBaseFormView, CYdFormView)

CQuestionVaultBaseFormView::CQuestionVaultBaseFormView()
	: CYdFormView(CQuestionVaultBaseFormView::IDD)
	, m_strName(_T(""))
	, m_strCreator(_T(""))
	, m_strDescription(_T(""))
	, m_strCreateDate(_T(""))
{

}

CQuestionVaultBaseFormView::~CQuestionVaultBaseFormView()
{
}

void CQuestionVaultBaseFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_CREATER, m_strCreator);
	DDX_Text(pDX, IDC_EDIT_DESC, m_strDescription);
	DDX_Text(pDX, IDC_EDIT_QVB_CREATE_DATE, m_strCreateDate);
}

BEGIN_MESSAGE_MAP(CQuestionVaultBaseFormView, CFormView)
END_MESSAGE_MAP()


// CQuestionVaultBaseFormView diagnostics

#ifdef _DEBUG
void CQuestionVaultBaseFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CQuestionVaultBaseFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CQuestionVaultBaseFormView message handlers
void CQuestionVaultBaseFormView::ReadonlyPage(BOOL bReadonly)
{
	((CEdit*)(GetDlgItem(IDC_EDIT_NAME)))->SetReadOnly(bReadonly);
	((CEdit*)(GetDlgItem(IDC_EDIT_DESC)))->SetReadOnly(bReadonly);
}

HRESULT CQuestionVaultBaseFormView::UpdateProp(BOOL bUpdate) 
{

	HRESULT hr = E_FAIL;
	ASSERT(m_pSheet);
	CYdObjWrapper* pObjWrapper = m_pSheet->GetCurObjWrapper();
	ASSERT(pObjWrapper);
	CYDObjectRef* pObjRef = pObjWrapper->m_pObjRef;
	ASSERT(pObjRef);
	if(bUpdate)
	{
		//将属性放到控件中
		_variant_t valName;
		hr = pObjRef->GetPropVal(L"Name",&valName);
		if(FAILED(hr))
		{
			return hr;
		}
		m_strName = CDataHandler::VariantToString(valName);
		_variant_t valCreator;
		hr = pObjRef->GetPropVal(L"Creator",&valCreator);
		if(FAILED(hr))
		{
			return hr;
		}
		m_strCreator = CDataHandler::VariantToString(valCreator);
		if (m_strCreator.GetLength() <= 0)
		{
			m_strCreator = DEFAULT_CREATOR;
		}
		_variant_t valCreateDate;
		hr = pObjRef->GetPropVal(L"CREATEDATE",&valCreateDate);
		if(FAILED(hr))
		{
			return hr;
		}
		if (valCreateDate.vt == VT_EMPTY)
		{
			COleDateTime date = COleDateTime::GetCurrentTime();
			valCreateDate.vt = VT_DATE;
			valCreateDate.date = date;
		}
		m_strCreateDate = CDataHandler::VariantToString(valCreateDate);

		_variant_t valDescription;
		hr = pObjRef->GetPropVal(L"Description",&valDescription);
		if(FAILED(hr))
		{
			return hr;
		}
		m_strDescription = CDataHandler::VariantToString(valDescription);

		UpdateData(FALSE);
	}
	else
	{
		//将控件的值填到属性中
		UpdateData(TRUE);
		
		_variant_t valName(m_strName);
		hr = pObjRef->SetPropVal(L"Name",&valName);
		if(FAILED(hr))
		{
			return hr;
		}
		
		_variant_t valCreater(m_strCreator);
		hr = pObjRef->SetPropVal(L"CREATER",&valCreater);
		if(FAILED(hr))
		{
			return hr;
		}
		_variant_t valCreateDate;
		hr = CDataHandler::StringToVariant(m_strCreateDate, VT_DATE, &valCreateDate);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = pObjRef->SetPropVal(L"CREATEDATE",&valCreateDate);
		if(FAILED(hr))
		{
			return hr;
		}
		_variant_t valDesc(m_strDescription);
		hr = pObjRef->SetPropVal(L"DESCRIPTION",&valDesc);
		if(FAILED(hr))
		{
			return hr;
		}
	}

	return S_OK;
}

BOOL CQuestionVaultBaseFormView::ValidateData(OPERATION op)
{
	CString strNameOrign = m_strName;
	UpdateData();
	if(m_strName.IsEmpty())
	{
		AfxMessageBox(_T("名称不能为空！"));
		return FALSE;
	}
	if (op == OP_NEW || (op == OP_EDIT && 0 != m_strName.CompareNoCase(strNameOrign)))
	{
		BOOL bExist = FALSE;
		CYdObjWrapper* pObjWrapper = m_pSheet->GetCurObjWrapper();
		ASSERT(pObjWrapper);
		CYDObjectRef* pObjRef = pObjWrapper->m_pObjRef;
		ASSERT(pObjRef);
		pObjRef->IsFieldExist(L"name", m_strName, &bExist);
		if (bExist)
		{
			AfxMessageBox(L"当前名称已经存在，请重新输入！");
			m_strName = L"";
			UpdateData(FALSE);
			GetDlgItem(IDC_EDIT_NAME)->SetFocus();
			return FALSE;
		}

	}
	return TRUE;
}
BOOL CQuestionVaultBaseFormView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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