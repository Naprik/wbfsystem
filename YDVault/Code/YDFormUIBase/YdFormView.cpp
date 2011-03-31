// YdFormView.cpp : implementation file
//

#include "stdafx.h"
#include "YdFormView.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../ObjRef/YDObjectRef.h"

// CYdFormView

IMPLEMENT_DYNCREATE(CYdFormView, CFormView)

CYdFormView::CYdFormView(UINT nIDTemplate)
:CFormView(nIDTemplate)
{
	
}
CYdFormView::CYdFormView()
	: CFormView(CYdFormView::IDD)
{

}

CYdFormView::~CYdFormView()
{
}

void CYdFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CYdFormView, CFormView)
END_MESSAGE_MAP()


// CYdFormView diagnostics

#ifdef _DEBUG
void CYdFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CYdFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CYdFormView message handlers
UINT CYdFormView::GetPropertyCount()
{
	ASSERT(m_pSheet != NULL);
	CYdObjWrapper* pObjWrapper = m_pSheet->GetCurObjWrapper();
	ASSERT(pObjWrapper);
	HRESULT hr = E_FAIL;
	UINT uCount = 0;
	hr = pObjWrapper->m_pObjRef->GetPropCount(&uCount);
	if(FAILED(hr))
	{
		return 0;
	}
	return uCount;
}

HRESULT CYdFormView::GetProperty(UINT _uIndex,BSTR* _bstrName,VARIANT* _val)
{
	HRESULT hr = E_FAIL;
	if(_uIndex >= GetPropertyCount())
	{
		return E_FAIL;
	}
	ASSERT(m_pSheet != NULL);
	CYdObjWrapper* pObjWrapper = m_pSheet->GetCurObjWrapper();
	ASSERT(pObjWrapper);
	hr = pObjWrapper->m_pObjRef->GetProp(_uIndex,_bstrName,_val);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

BOOL CYdFormView::PreTranslateMessage(MSG* pMsg)
{
	//为了支持Ctrl+c ctrl+v等操作
	if(pMsg->message   ==   WM_KEYDOWN)   
	{
		if(GetKeyState(VK_CONTROL)   &   0x80)   
			if(pMsg->wParam   ==   'c'   ||   pMsg->wParam   ==   'C'     
				||pMsg->wParam   ==   'v'   ||   pMsg->wParam   ==   'V'   
				||pMsg->wParam   ==   'z'   ||   pMsg->wParam   ==   'Z'   
				||pMsg->wParam   ==   'x'   ||   pMsg->wParam   ==   'X')   
			{   
				return   PreTranslateInput(pMsg);   
			}   
	}
	return   CFormView::PreTranslateMessage(pMsg);   
}
