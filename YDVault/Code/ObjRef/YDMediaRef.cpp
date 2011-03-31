#include "StdAfx.h"
#include "YDMediaRef.h"
#include "YDObjectRef.h"
#include "../Base/DataHandler.h"
#include "../FtpBase/FtpRef.h"

CYDMediaRef::CYDMediaRef(CDatabaseEx* pDB) : CYDObjectRef(pDB)
{
	m_strDBName = DB_MEDIA;
	AddPropDef(_T("NAME"),VT_BSTR,_T(""));
	AddPropDef(_T("GUIDNAME"),VT_BSTR,_T(""));
	AddPropDef(_T("TYPE"),VT_I4);
	AddPropDef(_T("USEFOR"),VT_I4);
	AddPropDef(_T("CREATOR"),VT_BSTR);
	AddPropDef(_T("CREATEDATE"),VT_DATE);
}

CYDMediaRef::~CYDMediaRef(void)
{
}

HRESULT CYDMediaRef::GetLabel(CString* pStr)
{
	HRESULT hr = E_FAIL;
	CComVariant valName;
	hr = GetPropVal(L"Name",&valName);
	if(FAILED(hr))
	{
		return hr;
	}
	*pStr = CDataHandler::VariantToString(valName);
	return S_OK;
}

HRESULT CYDMediaRef::CreateVaultName(CString& _strVaultName)
{
	HRESULT hr = E_FAIL;
	_strVaultName += _T("\\Vault\\");
	_variant_t valTYPE;
	hr = GetPropVal(L"TYPE",&valTYPE);
	if(FAILED(hr))
	{
		return hr;
	}
	MEDIATYPE MType = (MEDIATYPE)CDataHandler::VariantToLong(valTYPE);
	if(MType == M_IMAGE)
	{
		_strVaultName += _T("image\\");
	}
	else if(MType == M_MP3)
	{
		_strVaultName += _T("mp3\\");
	}
	else if(MType == M_VEDIO)
	{
		_strVaultName += _T("vedio\\");
	}
	else
	{
		ASSERT(FALSE);
	}
	CComVariant valGuidName;
	hr = GetPropVal(L"GUIDNAME",&valGuidName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strGuidName = CDataHandler::VariantToString(valGuidName);
	_strVaultName += strGuidName;
	return S_OK;
}

HRESULT CYDMediaRef::Remove()
{
	HRESULT hr = E_FAIL;
	CString strVaultName;
	hr = CreateVaultName(strVaultName);
	if(FAILED(hr))
	{
		return hr;
	}
	CFtpRef* pFtpRef = NULL;
	CWnd* pWnd = AfxGetMainWnd();
	if(pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
	{
		pFtpRef = (CFtpRef*)pWnd->SendMessage(WM_YD_GET_FTPREF);
	}
	ASSERT(pFtpRef);
	hr = pFtpRef->Remove(strVaultName);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = CYDObjectRef::Remove();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}