#include "StdAfx.h"
#include "YDDevice.h"
#include "../Base/DataHandler.h"
#include "YDEAddrUnit.h"
#include "../Base\AutoClean.h"

CYDDevice::CYDDevice(CDatabaseEx* pDb)
:CYDObjectRef(pDb)
{
	m_strDBName = DB_YDDEVICE;
	AddPropDef(FIELD_YDDEVICE_ID, VT_BSTR);	//Éè±¸±àºÅ
	AddPropDef(FIELD_YDDEVICE_MAC,VT_BSTR);	//À¶ÑÀµØÖ·
}

CYDDevice::~CYDDevice(void)
{
}

HRESULT CYDDevice::GetDeviceMAC(const CString& strID, CString* pStrMac)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("select ");
	strSQL += FIELD_YDDEVICE_MAC;
	strSQL += _T(" from ");
	strSQL += DB_YDDEVICE;
	strSQL += _T(" where ");
	strSQL += FIELD_YDDEVICE_ID;
	strSQL += _T(" = ");
	strSQL += strID;
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if (FAILED(hr))
	{
		return hr;
	}
	hr = m_pDb->ExecuteSQL();
	if (FAILED(hr))
	{
		return hr;
	}
	if (!m_pDb->IsEOF())
	{
		_variant_t val;
		hr = m_pDb->GetField(_variant_t(FIELD_YDDEVICE_MAC), val);
		if(FAILED(hr))
		{
			return hr;
		}
		*pStrMac = CDataHandler::VariantToString(val);
	}

	return S_OK;
}
HRESULT CYDDevice::GetDeviceID(const CString& strMac, CString* pStrID)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("select ");
	strSQL += FIELD_YDDEVICE_ID;
	strSQL += _T(" from ");
	strSQL += DB_YDDEVICE;
	strSQL += _T(" where ");
	strSQL += FIELD_YDDEVICE_MAC;
	strSQL += _T(" = ");
	strSQL += strMac;
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if (FAILED(hr))
	{
		return hr;
	}
	hr = m_pDb->ExecuteSQL();
	if (FAILED(hr))
	{
		return hr;
	}
	if (!m_pDb->IsEOF())
	{
		_variant_t val;
		hr = m_pDb->GetField(_variant_t(FIELD_YDDEVICE_ID), val);
		if(FAILED(hr))
		{
			return hr;
		}
		*pStrID = CDataHandler::VariantToString(val);
	}

	return S_OK;
}

CYDObjectRef* CYDDevice::CreateObject()
{
	return new CYDDevice(m_pDb);
}

HRESULT CYDDevice::GetInAddrUnit(std::list<CYDObjectRef*>& _lstUnit)
{
	HRESULT hr = E_FAIL;
	CString strSQL = _T("SELECT OBJID FROM ");
	strSQL += DB_YDEXAMADDRUNIT ;
	strSQL += _T(" WHERE ");
	strSQL += FIELD_YDEXAMADDRUNIT_DEVICEID;
	strSQL += _T(" = ? ");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valProp((long)m_uObjID);
	ADODB::DataTypeEnum DataType ;
	int size = 0;
	CDataHandler::VarTypeToDBType(&valProp,DataType,size);
	hr = m_pDb->AddParameter(_bstr_t(FIELD_YDEXAMADDRUNIT_STUID), DataType, adParamInput, 
		size, &valProp);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = m_pDb->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}
	while(!m_pDb->IsEOF())
	{
		_variant_t valOBJID;
		hr = m_pDb->GetField(_variant_t(_T("OBJID")),valOBJID);
		if(FAILED(hr))
		{
			return hr;
		}
		OBJID uID = CDataHandler::VariantToLong(valOBJID);
		CYDObjectRef* pRef = new CYDEAddrUnit(m_pDb);
		ASSERT(pRef);
		hr = pRef->SetID(uID);
		if(FAILED(hr))
		{
			return hr;
		}
		_lstUnit.push_back(pRef);
		hr = m_pDb->MoveNext();
		if(FAILED(hr))
		{
			return hr;
		}

	}
	return S_OK;
}

HRESULT CYDDevice::Remove()
{
	HRESULT hr = E_FAIL;
	std::list<CYDObjectRef*> lstAddrUnit;
	hr = GetInAddrUnit(lstAddrUnit);
	if(FAILED(hr))
	{
		return hr;
	}
	CListAutoClean<CYDObjectRef> clr(lstAddrUnit);
	for(std::list<CYDObjectRef*>::const_iterator itr = lstAddrUnit.begin();
		itr != lstAddrUnit.end();++itr)
	{
		hr = (*itr)->Remove();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	hr = CYDObjectRef::Remove();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}