#include "StdAfx.h"
#include "YDQuestionVaultClassification.h"
#include "../DBBase/DatabaseEx.h"
#include "../Base/AutoClean.h"
#include "../Base/DataHandler.h"
#include "YDLinkRef.h"

CYDQuestionVaultClassification::CYDQuestionVaultClassification(CDatabaseEx* pDB) 
	: CYDObjectRef(pDB)
{
	m_strDBName = DB_QVAULTCLASSIFICATION;
	CYDPropDef* pPropDef = new CYDPropDef(_T("NAME"),VT_BSTR,_T(""));
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(_T("CREATOR"),VT_BSTR);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(_T("CREATEDATE"),VT_DATE);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(_T("DESCRIPTION"),VT_BSTR);
	m_lstPropDef.push_back(pPropDef);
}	

CYDQuestionVaultClassification::~CYDQuestionVaultClassification(void)
{
}

HRESULT CYDQuestionVaultClassification::GetChildQVClassification(
										std::list<CYDQuestionVaultClassification*>* sub_qvc,
										std::list<CYDLinkRef*>* _sub_link /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_B FROM ");
	strSQL += DB_QVAULTCLASSIFICATIONHIERARCHY;
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
		UINT idChild = CDataHandler::VariantToLong(val);
		CYDQuestionVaultClassification* pChild = new CYDQuestionVaultClassification(m_pDb);
		sub_qvc->push_back(pChild);
		hr = pChild->SetID(idChild);
		if(FAILED(hr))
		{
			//失败清除
			CListAutoClean<CYDQuestionVaultClassification> clean(*sub_qvc);
			return hr;
		}
		if(_sub_link != NULL)
		{
			_variant_t valLinkID;
			hr = m_pDb->GetField(_variant_t(_T("OBJID")),valLinkID);
			UINT idLink = CDataHandler::VariantToLong(valLinkID);
			CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb,DB_QVAULTCLASSIFICATIONHIERARCHY);
			hr = pLinkRef->PutObjRef(this,pChild);
			if(FAILED(hr))
			{
				return hr;
			}
			hr = pLinkRef->SetID(idLink);
			if(FAILED(hr))
			{
				return hr;
			}
			_sub_link->push_back(pLinkRef);
		}
		hr = m_pDb->MoveNext();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CYDQuestionVaultClassification::GetAllQVault(std::list<CYDQuestionVault*>* lstVault,
													 std::list<CYDLinkRef*>* _sub_link/* = NULL*/)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_B FROM ");
	strSQL += DB_QCLASSIFICATIONQVAULT;
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
		UINT idChild = CDataHandler::VariantToLong(val);
		CYDQuestionVault* pChild = new CYDQuestionVault(m_pDb);
		lstVault->push_back(pChild);
		hr = pChild->SetID(idChild);
		if(FAILED(hr))
		{
			//失败清除
			CListAutoClean<CYDQuestionVault> clean(*lstVault);
			return hr;
		}
		if(_sub_link != NULL)
		{
			_variant_t valLinkID;
			hr = m_pDb->GetField(_variant_t(_T("OBJID")),valLinkID);
			UINT idLink = CDataHandler::VariantToLong(valLinkID);
			CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb,DB_QCLASSIFICATIONQVAULT);
			hr = pLinkRef->PutObjRef(this,pChild);
			if(FAILED(hr))
			{
				return hr;
			}
			hr = pLinkRef->SetID(idLink);
			if(FAILED(hr))
			{
				return hr;
			}
			_sub_link->push_back(pLinkRef);
		}
		hr = m_pDb->MoveNext();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CYDQuestionVaultClassification::Remove()
{
	HRESULT hr = E_FAIL;
	//要将大类的字删除
	{
		//大类的字大类删除
		std::list<CYDQuestionVaultClassification*> sub_qvc;
		std::list<CYDLinkRef*> sub_qvc_link ;
		CListAutoClean<CYDQuestionVaultClassification> clr1(sub_qvc);
		CListAutoClean<CYDLinkRef> clr2(sub_qvc_link);
		hr = GetChildQVClassification(&sub_qvc,&sub_qvc_link);
		if(FAILED(hr))
		{
			return hr;
		}
		ASSERT(sub_qvc.size() == sub_qvc_link.size());
		std::list<CYDQuestionVaultClassification*>::const_iterator itr = sub_qvc.begin();
		std::list<CYDLinkRef*>::const_iterator itrLink = sub_qvc_link.begin();
		for (;itr != sub_qvc.end() && itrLink != sub_qvc_link.end();++itr,++itrLink)
		{
			hr = (*itr)->Remove();
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
	}

	{
		//大类的字题库删除
		std::list<CYDQuestionVault*> sub_qv;
		std::list<CYDLinkRef*> sub_qv_link ;
		CListAutoClean<CYDQuestionVault> clr1(sub_qv);
		CListAutoClean<CYDLinkRef> clr2(sub_qv_link);
		hr = GetAllQVault(&sub_qv,&sub_qv_link);
		if(FAILED(hr))
		{
			return hr;
		}
		ASSERT(sub_qv.size() == sub_qv_link.size());
		std::list<CYDQuestionVault*>::const_iterator itr = sub_qv.begin();
		std::list<CYDLinkRef*>::const_iterator itrLink = sub_qv_link.begin();
		for (;itr != sub_qv.end() && itrLink != sub_qv_link.end();++itr,++itrLink)
		{
			hr = (*itr)->Remove();
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
	}
	hr = CYDObjectRef::Remove();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}