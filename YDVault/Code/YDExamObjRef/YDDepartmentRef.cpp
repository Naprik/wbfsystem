#include "StdAfx.h"
#include "YDDepartmentRef.h"
#include "../Base\DataHandler.h"
#include "../Base\AutoClean.h"
#include "../DBBase/DatabaseEx.h"
#include "YDClassRef.h"


CYDDepartmentRef::CYDDepartmentRef(CDatabaseEx* pDB)
:CYDObjectRef(pDB)
{
	m_strDBName = DB_YDDEPARTMENT;
	AddPropDef(FIELD_YDDEPARTMENT_ID,VT_BSTR);	//部门ID
	AddPropDef(FIELD_YDDEPARTMENT_NAME,VT_BSTR);	//部门名称
	AddPropDef(FIELD_YDDEPARTMENT_PRESIDENT,VT_BSTR);	//院长
	AddPropDef(FIELD_YDDEPARTMENT_PRESIDENT_TEL,VT_BSTR);	//联系方式
}

CYDDepartmentRef::~CYDDepartmentRef(void)
{
}

HRESULT CYDDepartmentRef::GetAllClasses(std::list<CYDObjectRef*> *_lstClasses, 
										std::list<CYDLinkRef*> *_links /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_B FROM ");
	strSQL += DB_YDLINK_DEPARTMENT_CLASS;
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
		UINT idClass = CDataHandler::VariantToLong(val);
		CYDClassRef* pClass = new CYDClassRef(m_pDb);
		_lstClasses->push_back(pClass);
		hr = pClass->SetID(idClass);
		if(FAILED(hr))
		{
			return hr;
		}
		if(_links != NULL)
		{
			_variant_t valLinkID;
			hr = m_pDb->GetField(_variant_t(_T("OBJID")),valLinkID);
			UINT idLink = CDataHandler::VariantToLong(valLinkID);
			CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb, DB_YDLINK_DEPARTMENT_CLASS);
			hr = pLinkRef->PutObjRef(this, pClass);
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

HRESULT CYDDepartmentRef::Remove()
{
	HRESULT hr = E_FAIL;
	//删除该部门下所有的班级
	std::list<CYDObjectRef*> lstDelClass;
	std::list<CYDLinkRef*> lstDelLinks;
	CListAutoClean<CYDObjectRef> clr(lstDelClass);
	CListAutoClean<CYDLinkRef> clr2(lstDelLinks);
	hr = GetAllClasses(&lstDelClass, &lstDelLinks);
	if(FAILED(hr))
	{
		return hr;
	}
	std::list<CYDObjectRef*>::const_iterator itrClass = lstDelClass.begin();
	std::list<CYDLinkRef*>::iterator itrLink = lstDelLinks.begin();
	for(;itrClass != lstDelClass.end(); ++itrClass, ++itrLink)
	{
		hr = (*itrClass)->Remove();
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
