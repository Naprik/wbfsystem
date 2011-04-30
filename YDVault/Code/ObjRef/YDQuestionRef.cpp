#include "StdAfx.h"
#include "YDQuestionRef.h"
#include "../Base/DataHandler.h"
#include "../Base/AutoClean.h"
#include "../DBBase/DatabaseEx.h"
#include "../DBBase/DBTransactionRef.h"
#include "YDQuestionType.h"
#include "YdObjWrapper.h"

CYDQuestionRef::CYDQuestionRef(CDatabaseEx* pDb)
: CYDObjectRef(pDb)
{
}

CYDQuestionRef::~CYDQuestionRef(void)
{
}

bool CYDQuestionRef::ToString(CString* pStr)
{
	return true;
}

bool CYDQuestionRef::GetAllHardLevel(std::list<CString>* pList)
{
	ASSERT(pList != NULL);
	pList->push_back(L"初级");
	pList->push_back(L"中高级");
	return true;
}

HARDLEVEL CYDQuestionRef::TranslateHardLevel(const CString& str)
{
	if (str == L"初级")
	{
		return EASY;
	}
	if (str == L"中高级")
	{
		return HARD;
	}
	else
	{
		ASSERT(FALSE);
	}
	return EASY;
}

HRESULT CYDQuestionRef::GetAllRelatedKnowledgePoints(std::list<CYdKnowledge*>* lstKPs,
													 std::list<CYDLinkRef*>* lstLinks)
{
	HRESULT hr = E_FAIL;
	
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_B FROM ");
	strSQL += DB_QUESTIONKPOINT;
	strSQL += _T(" WHERE ID_A = ? and ID_TYPE = ?");
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
	_variant_t varIDType;
	hr = GetPropVal(L"TYPEID", &varIDType);
	if (FAILED(hr))
	{
		return hr;
	}
	long idtype = CDataHandler::VariantToLong(varIDType);
	_variant_t vtParamType((long)idtype);
	hr = m_pDb->AddParameter(L"ID_TYPE", 
		adUnsignedInt, 
		adParamInput, 
		sizeof(long),&vtParamType);
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
		CYdKnowledge* pKP = new CYdKnowledge(m_pDb);
		lstKPs->push_back(pKP);
		hr = pKP->SetID(idChild);
		if(FAILED(hr))
		{
			//失败清除
			CListAutoClean<CYdKnowledge> clean(*lstKPs);
			return hr;
		}
		if(lstLinks != NULL)
		{
			_variant_t valLinkID;
			hr = m_pDb->GetField(_variant_t(_T("OBJID")),valLinkID);
			UINT idLink = CDataHandler::VariantToLong(valLinkID);
			CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb, DB_QUESTIONKPOINT);
			hr = pLinkRef->PutObjRef(this, pKP);
			if(FAILED(hr))
			{
				return hr;
			}
			hr = pLinkRef->SetID(idLink);
			if(FAILED(hr))
			{
				return hr;
			}
			lstLinks->push_back(pLinkRef);
		}
		hr = m_pDb->MoveNext();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}


HRESULT CYDQuestionRef::GetAllMedia(std::list<CYDMediaRef*>* lstMedia, 
									std::list<CYDLinkRef*>* lstLinks /*= NULL*/)
{

	HRESULT hr = E_FAIL;
	ASSERT(m_pDb != NULL);
	_variant_t varIDType;
	hr = GetPropVal(L"TYPEID", &varIDType);
	if (FAILED(hr))
	{
		return hr;
	}
	CString strSQL = _T("SELECT OBJID,ID_B FROM ");
	strSQL += DB_QUESTIONMEDIA;
	strSQL += _T(" WHERE ID_A = ? and ID_TYPE = ?");
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

	long idtype = CDataHandler::VariantToLong(varIDType);
	_variant_t vtParamType((long)idtype);
	hr = m_pDb->AddParameter(L"ID_TYPE", 
		adUnsignedInt, 
		adParamInput, 
		sizeof(long),&vtParamType);
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
		CYDMediaRef* pMedia = new CYDMediaRef(m_pDb);
		lstMedia->push_back(pMedia);
		hr = pMedia->SetID(idChild);
		if(FAILED(hr))
		{
			return hr;
		}
		if(lstLinks != NULL)
		{
			_variant_t valLinkID;
			hr = m_pDb->GetField(_variant_t(_T("OBJID")),valLinkID);
			UINT idLink = CDataHandler::VariantToLong(valLinkID);
			CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb, DB_QUESTIONMEDIA);
			hr = pLinkRef->PutObjRef(this, pMedia);
			if(FAILED(hr))
			{
				return hr;
			}
			hr = pLinkRef->SetID(idLink);
			if(FAILED(hr))
			{
				return hr;
			}
			lstLinks->push_back(pLinkRef);
		}
		hr = m_pDb->MoveNext();
		if(FAILED(hr))
		{
			return hr;
		}
	}

	return S_OK;
}

HRESULT CYDQuestionRef::GetQuestionType(QTYPE* type)
{
	ASSERT(type != NULL);

	HRESULT hr = E_FAIL;
	CComVariant varType;
	hr = GetPropVal(L"TYPEID", &varType);
	if (FAILED(hr))
	{
		return hr;
	}
	long idtype = CDataHandler::VariantToLong(varType);
	CYDQuestionType qtype(m_pDb);
	qtype.SetID(idtype);
	varType.Clear();
	qtype.GetPropVal(FIELD_QUESTIONTYPE_TYPE, &varType);
	*type = QTYPE(CDataHandler::VariantToLong(varType));

	return S_OK;
}

HRESULT CYDQuestionRef::Remove()
{
	HRESULT hr = E_FAIL;
	{
		std::list<CYdObjWrapper*> lstDelObjWrapper;
		CListAutoClean<CYdObjWrapper> clr(lstDelObjWrapper);
		//删除关联的Media
		std::list<CYDMediaRef*> lstMedia;
		std::list<CYDLinkRef*> sub_link;
		hr = GetAllMedia(&lstMedia,&sub_link);
		if(FAILED(hr))
		{
			return hr;
		}
		ASSERT(lstMedia.size() == sub_link.size());
		std::list<CYDMediaRef*>::iterator itr = lstMedia.begin();
		std::list<CYDLinkRef*>::iterator itrLink = sub_link.begin();
		for (;itr != lstMedia.end() && itrLink != sub_link.end();
			++itr,++itrLink)
		{
			CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
			pObjWrapper->m_state = S_NEED_DELETE_BOTH;
			pObjWrapper->m_pObjRef = (*itr);
			pObjWrapper->m_pLinkRef = (*itrLink);
			hr = pObjWrapper->Remove();
			if(FAILED(hr))
			{
				return hr;
			}
			lstDelObjWrapper.push_back(pObjWrapper);
		}

	}
	{
		//删除与知识点的关系
		std::list<CYdKnowledge*> lstKP;
		std::list<CYDLinkRef*> lstLinks;
		CListAutoClean<CYdKnowledge> clr1(lstKP);
		CListAutoClean<CYDLinkRef> clr2(lstLinks);
		hr = GetAllRelatedKnowledgePoints(&lstKP,&lstLinks);
		if(FAILED(hr))
		{
			return hr;
		}
		ASSERT(lstKP.size() == lstLinks.size());
		std::list<CYdKnowledge*>::iterator itr = lstKP.begin();
		std::list<CYDLinkRef*>::iterator itrLink = lstLinks.begin();
		for (;itr != lstKP.end() && itrLink != lstLinks.end();
			++itr,++itrLink)
		{
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

HRESULT CYDQuestionRef::PersistToMSWord(int* pIndex, CWordAppHelper* pWord, CWordAppHelper* pWordAnswer, const CString& str)
{
	return S_OK;
}

HRESULT CYDQuestionRef::GetAnswerList(std::list<CString>* lst)
{
	return S_OK;
}

HRESULT CYDQuestionRef::SetFtp(CFtpRef* ftp)
{
	m_pFtp = ftp;

	return S_OK;
}

HRESULT CYDQuestionRef::UpdateUsedCount(int usedcount)
{
	HRESULT hr = E_FAIL;
	
	CString strsqlformat = L"update " + m_strDBName + L" set USEDCOUNT=%d";
	CString strsql;
	strsql.Format(strsqlformat, usedcount);
	m_pDb->InitializeSQL(_bstr_t(strsql));
	hr = m_pDb->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}	
	return S_OK;
}

HRESULT CYDQuestionRef::CreateFactorInfoItem()
{
	HRESULT hr = E_FAIL;
	for(int i = 1; i <= 25;i++)
	{
		CString strPropName;
		strPropName.Format(_T("C%d"),i);
		AddPropDef(strPropName,VT_BSTR,_T(""));
	}
	for(int i = 1; i <= 25;i++)
	{
		CString strPropName;
		strPropName.Format(_T("D%d"),i);
		AddPropDef(strPropName,VT_I4);
	}
	return S_OK;
}

HRESULT CYDQuestionRef::GetStandardAnswer(CString &_str)
{
	HRESULT hr = E_FAIL;
	std::list<CString> lstStdAnswer;
	hr = GetStandardAnswer(lstStdAnswer);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CString>::const_iterator itr = lstStdAnswer.begin();
		itr != lstStdAnswer.end();++itr)
	{
		_str += *itr;
	}
	return S_OK;
}

HRESULT CYDQuestionRef::GetStandardAnswer(std::list<CString> &_lstStdAnswer)
{
	HRESULT hr = E_FAIL;
	CString str;
	hr = GetPropVal(L"ANSWER",str);
	if(FAILED(hr))
	{
		return hr;
	}
	_lstStdAnswer.push_back(str);
	return S_OK;
}