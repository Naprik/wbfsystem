#include "StdAfx.h"
#include "YDExamAddress.h"
#include "../DBBase/DatabaseEx.h"
#include "../Base/DataHandler.h"
#include "YDEAddrUnit.h"
#include "../Base/AutoClean.h"

CYDExamAddress::CYDExamAddress(CDatabaseEx* pDB) : CYDObjectRef(pDB)
{
	m_strDBName = DB_YDEXAMADDR;
	AddPropDef(FIELD_YDEXAMADDR_ID,VT_BSTR);	//考场编号
	AddPropDef(FIELD_YDEXAMADDR_ADDR,VT_BSTR);	//考场名称
}

CYDExamAddress::~CYDExamAddress(void)
{
}


HRESULT CYDExamAddress::GetAllUnits(std::list<CYDObjectRef*>* _lstUnits, std::list<CYDLinkRef*>* _links)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_B FROM ");
	strSQL += DB_YDLINK_EXAMADDR_UNIT;
	strSQL += _T(" WHERE ID_A = ?");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t vtParam((long)m_uObjID);
	hr = m_pDb->AddParameter(L"ID_A", 
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
		hr = m_pDb->GetField(_variant_t("ID_B"), val);
		if(FAILED(hr))
		{
			return hr;
		}
		UINT idStu= CDataHandler::VariantToLong(val);
		CYDEAddrUnit* pStu = new CYDEAddrUnit(m_pDb);
		_lstUnits->push_back(pStu);
		hr = pStu->SetID(idStu);
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
			hr = pLinkRef->PutObjRef(this, pStu);
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

HRESULT CYDExamAddress::Remove()
{
	HRESULT hr = E_FAIL;
	//删除该班级下所有学生
	std::list<CYDObjectRef*> lstDelUnits;
	std::list<CYDLinkRef*> lstDelLinks;
	CListAutoClean<CYDObjectRef> clr(lstDelUnits);
	CListAutoClean<CYDLinkRef> clr2(lstDelLinks);
	hr = GetAllUnits(&lstDelUnits, &lstDelLinks);
	if(FAILED(hr))
	{
		return hr;
	}
	std::list<CYDObjectRef*>::const_iterator itrUnit = lstDelUnits.begin();
	std::list<CYDLinkRef*>::iterator itrLink = lstDelLinks.begin();
	for(;itrUnit != lstDelUnits.end(); ++itrUnit, ++itrLink)
	{
		hr = (*itrUnit)->Remove();
		if(FAILED(hr))
		{
			return hr;
		}
		hr = (*itrLink)->Remove();
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

CYDObjectRef* CYDExamAddress::CreateObject()
{
	return new CYDExamAddress(m_pDb);
}