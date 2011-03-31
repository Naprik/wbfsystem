#include "StdAfx.h"
#include "YDClassRef.h"
#include "../DBBase/DatabaseEx.h"
#include "../\Base\DataHandler.h"
#include "YDDepartmentRef.h"
#include "YdStudentRef.h"
#include "../Base/AutoClean.h"

CYDClassRef::CYDClassRef(CDatabaseEx* pDB)
:CYDObjectRef(pDB)
{
	m_strDBName = DB_YDCLASS;
	AddPropDef(FIELD_YDCLASS_CLASSID,VT_BSTR);	//班级ID
	AddPropDef(FIELD_YDCLASS_NAME,VT_BSTR);	//班级名称
	AddPropDef(FIELD_YDCLASS_HEAD,VT_BSTR);	//班主任
	AddPropDef(FIELD_YDCLASS_HEADTEL,VT_BSTR);	//联系方式
}

CYDClassRef::~CYDClassRef(void)
{
}

HRESULT CYDClassRef::GetDepartment(CYDObjectRef** _pDepartRef, CYDLinkRef** _link)
{
	HRESULT hr = E_FAIL;
	*_pDepartRef = NULL;
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_A FROM ");
	strSQL += DB_YDLINK_DEPARTMENT_CLASS;
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
	if (!m_pDb->IsEOF())
	{
		_variant_t val;
		hr = m_pDb->GetField(_variant_t("ID_A"), val);
		if(FAILED(hr))
		{
			return hr;
		}
		UINT idClass = CDataHandler::VariantToLong(val);
		*_pDepartRef = new CYDDepartmentRef(m_pDb);
		hr = (*_pDepartRef)->SetID(idClass);
		if(FAILED(hr))
		{
			return hr;
		}
		if(_link != NULL)
		{
			_variant_t valLinkID;
			hr = m_pDb->GetField(_variant_t(_T("OBJID")),valLinkID);
			UINT idLink = CDataHandler::VariantToLong(valLinkID);
			*_link = new CYDLinkRef(m_pDb, DB_YDLINK_DEPARTMENT_CLASS);
			hr = (*_link)->PutObjRef((*_pDepartRef), this);
			if(FAILED(hr))
			{
				return hr;
			}
			hr = (*_link)->SetID(idLink);
			if(FAILED(hr))
			{
				return hr;
			}
		}
		hr = m_pDb->MoveNext();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CYDClassRef::GetAllStudents(std::list<CYDObjectRef*> *_lstStudents, 
									std::list<CYDLinkRef*> *_links /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_B FROM ");
	strSQL += DB_YDLINK_CLASS_STUDENT;
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
		CYdStudentRef* pStu = new CYdStudentRef(m_pDb);
		_lstStudents->push_back(pStu);
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
			CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb, DB_YDLINK_CLASS_STUDENT);
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

HRESULT CYDClassRef::Remove()
{
	HRESULT hr = E_FAIL;
	//删除该班级下所有学生
	std::list<CYDObjectRef*> lstDelStus;
	std::list<CYDLinkRef*> lstDelLinks;
	CListAutoClean<CYDObjectRef> clr(lstDelStus);
	CListAutoClean<CYDLinkRef> clr2(lstDelLinks);
	hr = GetAllStudents(&lstDelStus, &lstDelLinks);
	if(FAILED(hr))
	{
		return hr;
	}
	std::list<CYDObjectRef*>::const_iterator itrStu = lstDelStus.begin();
	std::list<CYDLinkRef*>::iterator itrLink = lstDelLinks.begin();
	for(;itrStu != lstDelStus.end(); ++itrStu, ++itrLink)
	{
		hr = (*itrStu)->Remove();
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