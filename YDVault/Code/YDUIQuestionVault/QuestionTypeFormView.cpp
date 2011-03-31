// QuestionTypeFormView.cpp : implementation file
//

#include "stdafx.h"
#include "QuestionTypeFormView.h"
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../Base/DataHandler.h"


// CQuestionTypeFormView

IMPLEMENT_DYNCREATE(CQuestionTypeFormView, CYdFormView)

CQuestionTypeFormView::CQuestionTypeFormView()
	: CYdFormView(CQuestionTypeFormView::IDD)
	, m_strName(_T(""))
	, m_strCreator(_T(""))
	, m_strDescription(_T(""))
	, m_strCreateDate(_T(""))
{

}

CQuestionTypeFormView::~CQuestionTypeFormView()
{
}

void CQuestionTypeFormView::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_QT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_QT_CREATER, m_strCreator);
	DDX_Text(pDX, IDC_EDIT_QT_DESC, m_strDescription);
	DDX_Text(pDX, IDC_EDIT_QT_CREATE_DATE, m_strCreateDate);
}

BEGIN_MESSAGE_MAP(CQuestionTypeFormView, CYdFormView)
END_MESSAGE_MAP()


// CQuestionTypeFormView diagnostics

#ifdef _DEBUG
void CQuestionTypeFormView::AssertValid() const
{
	CYdFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CQuestionTypeFormView::Dump(CDumpContext& dc) const
{
	CYdFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CQuestionTypeFormView message handlers
void CQuestionTypeFormView::ReadonlyPage(BOOL bReadonly)
{
	((CEdit*)(GetDlgItem(IDC_EDIT_QT_NAME)))->SetReadOnly(bReadonly);
	((CEdit*)(GetDlgItem(IDC_EDIT_QT_DESC)))->SetReadOnly(bReadonly);
}

HRESULT CQuestionTypeFormView::UpdateProp(BOOL bUpdate) 
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

BOOL CQuestionTypeFormView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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