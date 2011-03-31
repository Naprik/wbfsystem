#include "StdAfx.h"
#include "YDEAddrUnit.h"
#include "../Base/DataHandler.h"
#include "../\Base\AutoClean.h"
#include "YDExamAddress.h"

CYDEAddrUnit::CYDEAddrUnit(CDatabaseEx* pDB)
:CYDObjectRef(pDB)
{
	m_strDBName = DB_YDEXAMADDRUNIT;
	AddPropDef(FIELD_YDEXAMADDRUNIT_STUID, VT_I4);	
	AddPropDef(FIELD_YDEXAMADDRUNIT_DEVICEID,VT_I4);	
	AddPropDef(FIELD_YDEXAMADDRUNIT_EXAMID,VT_BSTR);	
}

CYDEAddrUnit::~CYDEAddrUnit(void)
{
}

HRESULT CYDEAddrUnit::GetStudent(CYdStudentRef*& pStu)
{
	pStu = new CYdStudentRef(m_pDb);
	long stuID;
	GetPropVal(FIELD_YDEXAMADDRUNIT_STUID, &stuID);
	pStu->SetID(stuID);
	return S_OK;
}

HRESULT CYDEAddrUnit::GetDevice(CYDDevice*& pDevice)
{
	pDevice = new CYDDevice(m_pDb);
	long DeviceID;
	GetPropVal(FIELD_YDEXAMADDRUNIT_DEVICEID, &DeviceID);
	pDevice->SetID(DeviceID);
	return S_OK;
}

HRESULT CYDEAddrUnit::GetDeviceMac(CString& _strMac)
{
	HRESULT hr = E_FAIL;
	CYDDevice* pDevice = NULL;
	hr = GetDevice(pDevice);
	if(FAILED(hr))
	{
		return hr;
	}
	CPtrAutoClean<CYDDevice> clr(pDevice);
	hr = pDevice->GetPropVal(FIELD_YDDEVICE_MAC,_strMac);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYDEAddrUnit::GetInAddress(std::list<CYDObjectRef*>* _lstAddress,
					 std::list<CYDLinkRef*>* _links /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_A FROM ");
	strSQL += DB_YDLINK_EXAMADDR_UNIT;
	strSQL += _T(" WHERE ID_B = ?");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t vtParam((long)m_uObjID);
	hr = m_pDb->AddParameter(L"ID_B", 
		adUnsignedInt, 
		adParamInput, 
		sizeof(m_uObjID),&vtParam);
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
		_variant_t val;
		hr = m_pDb->GetField(_variant_t("ID_A"), val);
		if(FAILED(hr))
		{
			return hr;
		}
		UINT idStu= CDataHandler::VariantToLong(val);
		CYDObjectRef* pAddress = new CYDExamAddress(m_pDb);
		_lstAddress->push_back(pAddress);
		hr = pAddress->SetID(idStu);
		if(FAILED(hr))
		{
			return hr;
		}
		if(_links != NULL)
		{
			_variant_t valLinkID;
			hr = m_pDb->GetField(_variant_t(_T("OBJID")),valLinkID);
			UINT idLink = CDataHandler::VariantToLong(valLinkID);
			CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb, DB_YDLINK_EXAMADDR_UNIT);
			hr = pLinkRef->PutObjRef(pAddress, this);
			if(FAILED(hr))
			{
				return hr;
			}
			hr = pLinkRef->SetID(idLink);
			if(FAILED(hr))
			{
				return hr;
			}
			_links->push_back(pLinkRef);
		}
		hr = m_pDb->MoveNext();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CYDEAddrUnit::Remove()
{
	HRESULT hr = E_FAIL;
	std::list<CYDObjectRef*> lstAddress;
	std::list<CYDLinkRef*> link;
	hr = GetInAddress(&lstAddress,&link);
	if(FAILED(hr))
	{
		return hr;
	}
	CListAutoClean<CYDObjectRef> clr(lstAddress);
	CListAutoClean<CYDLinkRef> clr2(link);
	ASSERT(lstAddress.size() == link.size());
	for(std::list<CYDLinkRef*>::const_iterator itr = link.begin();
		itr != link.end();++itr)
	{
		hr = (*itr)->Remove();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return CYDObjectRef::Remove();
	return S_OK;
}