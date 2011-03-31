// YdConfigSheet.cpp : implementation file
//

#include "stdafx.h"
#include "YdConfig.h"
#include "YdConfigSheet.h"


// CYdConfigSheet

IMPLEMENT_DYNAMIC(CYdConfigSheet, CPropertySheet)

CYdConfigSheet::CYdConfigSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_pageConfigDB);
	AddPage(&m_pageConfigFtp);
}

CYdConfigSheet::CYdConfigSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_pageConfigDB);
	AddPage(&m_pageConfigFtp);
}

CYdConfigSheet::~CYdConfigSheet()
{
}


BEGIN_MESSAGE_MAP(CYdConfigSheet, CPropertySheet)
END_MESSAGE_MAP()


// CYdConfigSheet message handlers
HRESULT CYdConfigSheet::Save()
{
	HRESULT hr = E_FAIL;
	hr = m_DBConfig.Write();
	if(FAILED(hr))
	{
		return hr;
	}
	hr = m_FtpConfig.Write();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}