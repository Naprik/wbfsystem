// AtricleQuestionFormView.cpp : implementation file
//

#include "stdafx.h"
#include "../ObjRef/YDObjectRef.h"
#include "AtricleQuestionFormView.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../Base/DataHandler.h"
#include "Resource.h"


// CAtricleQuestionFormView

IMPLEMENT_DYNCREATE(CAtricleQuestionFormView, CYdFormView)

CAtricleQuestionFormView::CAtricleQuestionFormView()
	: CYdFormView(CAtricleQuestionFormView::IDD)
	, m_strArticle(_T(""))
	, m_iHardLevel(0)
	, m_strCreator(_T(""))
	, m_strCreateDate(_T(""))
{

}

CAtricleQuestionFormView::~CAtricleQuestionFormView()
{
}

void CAtricleQuestionFormView::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RICHEDIT_AC_ARTICLE, m_strArticle);
	DDX_CBIndex(pDX, IDC_COMBO_AQ_HARDLEVEL, m_iHardLevel);
	DDX_Text(pDX, IDC_EDIT_AQ_CREATOR, m_strCreator);
	DDX_Text(pDX, IDC_EDIT_AQ_CREATE_DATE, m_strCreateDate);
	DDX_Control(pDX, IDC_COMBO_AQ_HARDLEVEL, m_cmbHardLevel);
}

BEGIN_MESSAGE_MAP(CAtricleQuestionFormView, CYdFormView)
END_MESSAGE_MAP()


// CAtricleQuestionFormView diagnostics

#ifdef _DEBUG
void CAtricleQuestionFormView::AssertValid() const
{
	CYdFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAtricleQuestionFormView::Dump(CDumpContext& dc) const
{
	CYdFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAtricleQuestionFormView message handlers
void CAtricleQuestionFormView::ReadonlyPage(BOOL bReadonly)
{
	((CEdit*)(GetDlgItem(IDC_RICHEDIT_AC_ARTICLE)))->SetReadOnly(bReadonly);
	GetDlgItem(IDC_COMBO_AQ_HARDLEVEL)->EnableWindow(!bReadonly);
	((CEdit*)(GetDlgItem(IDC_EDIT_AQ_CREATOR)))->SetReadOnly(bReadonly);
	GetDlgItem(IDC_EDIT_AQ_CREATE_DATE)->EnableWindow(!bReadonly);
}

HRESULT CAtricleQuestionFormView::UpdateProp(BOOL bUpdate) 
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
		_variant_t valArticle;
		hr = pObjRef->GetPropVal(L"ARTICLE",&valArticle);
		if(FAILED(hr))
		{
			return hr;
		}
		m_strArticle = CDataHandler::VariantToString(valArticle);

		_variant_t valHardLevel;
		hr = pObjRef->GetPropVal(L"HARDLEVEL",&valHardLevel);
		if(FAILED(hr))
		{
			return hr;
		}
		UINT iLevel = CDataHandler::VariantToLong(valHardLevel);
		if(iLevel == 1)
		{
			m_iHardLevel = 0;
		}
		else if(iLevel == 16)
		{
			m_iHardLevel = 1;
		}


		_variant_t valCreator;
		hr = pObjRef->GetPropVal(L"CREATOR",&valCreator);
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
		UpdateData(FALSE);
	}
	else
	{
		//将控件的值填到属性中
		UpdateData(TRUE);

		_variant_t valArticle(m_strArticle);
		hr = pObjRef->SetPropVal(L"ARTICLE",&valArticle);
		if(FAILED(hr))
		{
			return hr;
		}
		ASSERT(m_iHardLevel != -1);
		UINT iHardLevel = 1;//默认是简单
		if(m_iHardLevel == 0)
		{
			iHardLevel = 1;
		}
		else if(m_iHardLevel == 1)
		{
			iHardLevel = 16;
		}
		else
		{
			ASSERT(FALSE);
		}
	
		_variant_t valHardLevel((long)iHardLevel);
		hr = pObjRef->SetPropVal(L"HARDLEVEL",&valHardLevel);
		if(FAILED(hr))
		{
			return hr;
		}

		_variant_t valCreator(m_strCreator);
		hr = pObjRef->SetPropVal(L"CREATOR",&valCreator);
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
	}
	return S_OK;
}
void CAtricleQuestionFormView::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	m_iHardLevel = 0;
	UpdateData(FALSE);
}

BOOL CAtricleQuestionFormView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
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