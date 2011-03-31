// QuestionVaultClassificationFormView.cpp : implementation file
//

#include "stdafx.h"
#include "QuestionVaultClassificationFormView.h"
#include "../ObjRef/YDObjectRef.h"
#include "../Objref/YdObjWrapper.h"
#include "../Base/DataHandler.h"


// CQuestionVaultClassificationFormView

IMPLEMENT_DYNCREATE(CQuestionVaultClassificationFormView, CYdFormView)

CQuestionVaultClassificationFormView::CQuestionVaultClassificationFormView()
	: CYdFormView(CQuestionVaultClassificationFormView::IDD)
	, m_strName(_T(""))
	, m_strCreater(_T(""))
	, m_strDesc(_T(""))
	, m_strCreateDate(_T(""))
{

}

CQuestionVaultClassificationFormView::~CQuestionVaultClassificationFormView()
{
}

void CQuestionVaultClassificationFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_CREATER, m_strCreater);
	DDX_Text(pDX, IDC_EDIT_DESC, m_strDesc);
	DDX_Text(pDX, IDC_EDIT_QVC_CREATE_DATE, m_strCreateDate);
}

BEGIN_MESSAGE_MAP(CQuestionVaultClassificationFormView, CFormView)
END_MESSAGE_MAP()


// CQuestionVaultClassificationFormView diagnostics

#ifdef _DEBUG
void CQuestionVaultClassificationFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CQuestionVaultClassificationFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CQuestionVaultClassificationFormView message handlers

void CQuestionVaultClassificationFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	UpdateData(FALSE);
}

void CQuestionVaultClassificationFormView::ReadonlyPage(BOOL bReadonly)
{
	((CEdit*)(GetDlgItem(IDC_EDIT_NAME)))->SetReadOnly(bReadonly);
	((CEdit*)(GetDlgItem(IDC_EDIT_DESC)))->SetReadOnly(bReadonly);
}

HRESULT CQuestionVaultClassificationFormView::UpdateProp(BOOL bUpdate) 
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
		m_strCreater = CDataHandler::VariantToString(valCreator);
		if (m_strCreater.GetLength() <= 0)
		{
			m_strCreater = DEFAULT_CREATOR;
		}
		_variant_t valCreateDate;
		hr = pObjRef->GetPropVal(L"CREATEDATE",&valCreateDate);
		if(FAILED(hr))
		{
			return hr;
		}
		if(valCreateDate.vt == VT_EMPTY)
		{
			COleDateTime dt = COleDateTime::GetCurrentTime();
			valCreateDate.vt = VT_DATE;
			valCreateDate.date = dt;
		}
		m_strCreateDate = CDataHandler::VariantToString(valCreateDate);
		_variant_t valDescription;
		hr = pObjRef->GetPropVal(L"Description",&valDescription);
		if(FAILED(hr))
		{
			return hr;
		}
		m_strDesc = CDataHandler::VariantToString(valDescription);

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
		_variant_t valCreater(m_strCreater);
		hr = pObjRef->SetPropVal(L"CREATER",&valCreater);
		if(FAILED(hr))
		{
			return hr;
		}
		_variant_t valCreateDate;
		CDataHandler::StringToVariant(m_strCreateDate, VT_DATE, &valCreateDate);
		hr = pObjRef->SetPropVal(L"CREATEDATE",&valCreateDate);
		if(FAILED(hr))
		{
			return hr;
		}
		_variant_t valDesc(m_strDesc);
		hr = pObjRef->SetPropVal(L"DESCRIPTION",&valDesc);
		if(FAILED(hr))
		{
			return hr;
		}
	}

	return S_OK;
}

BOOL	CQuestionVaultClassificationFormView::ValidateData(OPERATION op)
{
	UpdateData();
	if(m_strName.IsEmpty())
	{
		AfxMessageBox(_T("名称不能为空！"));
		return FALSE;
	}
	return TRUE;
}
BOOL CQuestionVaultClassificationFormView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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