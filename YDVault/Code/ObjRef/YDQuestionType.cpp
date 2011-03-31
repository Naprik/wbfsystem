#include "StdAfx.h"
#include "YDQuestionType.h"
#include "../DBBase/DatabaseEx.h"
#include "../Base/DataHandler.h"
#include "../Base/AutoClean.h"

CYDQuestionType::CYDQuestionType(CDatabaseEx* pDB)
:CYDObjectRef(pDB)
{
	m_strDBName = DB_QUESTIONTYPE;
	CYDPropDef* pPropDef = new CYDPropDef(_T("NAME"),VT_BSTR,_T(""));
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(_T("TYPE"),VT_I1);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(_T("CREATOR"),VT_BSTR);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(_T("CREATEDATE"),VT_DATE);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(_T("DESCRIPTION"),VT_BSTR);
	m_lstPropDef.push_back(pPropDef);
}

CYDQuestionType::~CYDQuestionType(void)
{
}

HRESULT CYDQuestionType::GetArticleRelateQuestionsCount(OBJID vaultID, std::list<UINT>* pLstCount)
{
	HRESULT hr = E_FAIL;
	
	CString strSQL = L"SELECT DISTINCT QNUM FROM ";
	strSQL += DB_ARTICLEQUESTION;
	strSQL += L" WHERE objid in (select id_b from ";
	strSQL += DB_VAULTQUESTION;
	strSQL += L" where id_a=";
	CString strVaultID;
	strVaultID.Format(L"%d", vaultID);
	strSQL += strVaultID;
	strSQL += L" and typeid = ?)";
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if (FAILED(hr))
	{
		return hr;
	}
	CComVariant var;
	var.vt = VT_I4;
	var.lVal = m_uObjID;
	hr = m_pDb->AddParameter(L"typeid", adUnsignedInt, adParamInput, sizeof(m_uObjID), &var);
	if (FAILED(hr))
	{
		return hr;
	}
	hr = m_pDb->ExecuteSQL();
	if (FAILED(hr))
	{
		return hr;
	}
	while (!m_pDb->IsEOF())
	{
		_variant_t varQNUM;
		hr = m_pDb->GetField(_bstr_t(L"QNUM"), varQNUM);
		if (FAILED(hr))
		{
			return hr;
		}
		long lQNum = CDataHandler::VariantToLong(varQNUM);
		pLstCount->push_back(lQNum);
		hr = m_pDb->MoveNext();
		if (FAILED(hr))
		{
			return hr;
		}
	}
	if (pLstCount->size() <= 0)
	{
		pLstCount->push_back(1);
	}
	return S_OK;
}

HRESULT CYDQuestionType::GetAllQType(CDatabaseEx* _pDB,std::list<CYDQuestionType*>* _lstAllQType)
{
	HRESULT hr = E_FAIL;
	CString strSQL = _T("SELECT OBJID FROM ");
	strSQL += DB_QUESTIONTYPE;
	hr = _pDB->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}
	hr = _pDB->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}
	while(!_pDB->IsEOF())
	{
		_variant_t val;
		hr = _pDB->GetField(_variant_t("OBJID"), val);
		if(FAILED(hr))
		{
			return hr;
		}
		UINT id = CDataHandler::VariantToLong(val);
		CYDQuestionType* pObjRef = new CYDQuestionType(_pDB);
		_lstAllQType->push_back(pObjRef);
		hr = pObjRef->SetID(id);
		if(FAILED(hr))
		{
			//Ê§°ÜÇå³ý
			CListAutoClean<CYDQuestionType> clean(*_lstAllQType);
			return hr;
		}
		hr = _pDB->MoveNext();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CYDQuestionType::GetLabel(CString* pStr)
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