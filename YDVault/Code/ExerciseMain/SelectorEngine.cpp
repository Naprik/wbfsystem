#include "StdAfx.h"
#include "SelectorEngine.h"
#include "../Base/AutoClean.h"
#include "../DBBase/DatabaseEx.h"
#include "../ObjRef/YDArticleQuestionRef.h"
#include "../ObjRef/YDChoiceQuestionRef.h"
#include "../ObjRef/YDQuestionType.h"
#include "../ObjRef/YDQuestionFactory.h"
#include "ExerciseMain.h"
#include "QuestionCfgStruct.h"

CSelectorEngine::CSelectorEngine(void)
{
}

CSelectorEngine::~CSelectorEngine(void)
{
}

HRESULT CSelectorEngine::GetQuestionsFromCfg(CSelectQuestionCfgMgr* pCfgMgr,
											 std::list<CYDQuestionRef*>* pLstQuestion)
{
	HRESULT hr = E_FAIL;
	m_mapRecord.clear();
	std::list<CQuestionCfgStruct*>::const_iterator itrCfg = pCfgMgr->m_lstQuestionStruct.begin();
	for (; itrCfg != pCfgMgr->m_lstQuestionStruct.end(); ++itrCfg)
	{
		CYDQuestionFactory factory;
		CYDQuestionType qtype(theApp.m_pDatabase);
		qtype.SetID((*itrCfg)->m_QTypeID);
		CYDQuestionRef* pQ = factory.CreateQuestion(&qtype, theApp.m_pDatabase);
		CPtrAutoClean<CYDQuestionRef> cleanptr(pQ);
		CQuestionCfgStruct* pcfg = *itrCfg;
		
		int iMaxNum = pcfg->m_cNum;
	
		for (int i=0; i < iMaxNum; ++i)
		{
			pcfg->m_cNum = 1;
			CString strKey;
			strKey.Format(L"%d:%d", pcfg->m_vaultID, pcfg->m_QTypeID);
			DATABASE_TYPE dbType = theApp.m_pDatabase->GetDBType();
			if(ACCESS == dbType)
			{
				hr = GetQuestionByCfgItemFromAccess(pcfg, pQ);
				if (FAILED(hr))
				{
					pcfg->m_cNum = iMaxNum;
					return hr;
				}
			}
			else
			{
				ASSERT(FALSE);
			}
			pcfg->m_cNum = iMaxNum;
			CYDChoiceQuestionRef* pChoiceQuestion =	dynamic_cast<CYDChoiceQuestionRef*>(pQ);
			CYDArticleQuestionRef* pArticleQuestion = dynamic_cast<CYDArticleQuestionRef*>(pQ);
			BOOL bFirst = TRUE;
			while (!theApp.m_pDatabase->IsEOF())
			{
				if (bFirst)
				{
					if (pChoiceQuestion != NULL)
					{
						CYDQuestionRef* p = new CYDChoiceQuestionRef(theApp.m_pDatabase);
						pLstQuestion->push_back(p);
						//野割ID
						_variant_t varVal;
						hr = theApp.m_pDatabase->GetField(L"OBJID", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						AddRecord(strKey, OBJID(varVal));
						p->SetID(OBJID(varVal));
						varVal.Clear();
						//野割code
						hr = theApp.m_pDatabase->GetField(L"CODE", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"CODE", &varVal);
						varVal.Clear();
						//野割title
						hr = theApp.m_pDatabase->GetField(L"TITLE", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"TITLE", &varVal);
						varVal.Clear();
						//野割choice
						hr = theApp.m_pDatabase->GetField(L"CHOICE", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"CHOICE", &varVal);
						varVal.Clear();
						//野割typeid;
						hr = theApp.m_pDatabase->GetField(L"TYPEID", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"TYPEID", &varVal);
						varVal.Clear();
						//野割answer
						hr = theApp.m_pDatabase->GetField(L"ANSWER", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"ANSWER", &varVal);
						varVal.Clear();
						//野割answerdesc
						hr = theApp.m_pDatabase->GetField(L"ANSWERDESC", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"ANSWERDESC", &varVal);
						varVal.Clear();
						//野割usedcount
						hr = theApp.m_pDatabase->GetField(L"USEDCOUNT", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"USEDCOUNT", &varVal);
						varVal.Clear();
						//野割hardlevel
						hr = theApp.m_pDatabase->GetField(L"HARDLEVEL", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"HARDLEVEL", &varVal);
						varVal.Clear();

						//野割CREATOR
						hr = theApp.m_pDatabase->GetField(L"CREATOR", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"CREATOR", &varVal);
						varVal.Clear();

						//野割CREATEDATE
						hr = theApp.m_pDatabase->GetField(L"CREATEDATE", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"CREATEDATE", &varVal);
						//野割Image
						hr = theApp.m_pDatabase->GetField(FIELD_ARTICLEQUESTION_IMAGETITLE, varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(FIELD_ARTICLEQUESTION_IMAGETITLE, &varVal);

						//野割mode
						hr = theApp.m_pDatabase->GetField(FIELD_ARTICLEQUESTION_TITLEMODE, varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(FIELD_ARTICLEQUESTION_TITLEMODE, &varVal);


						//野割Mark
						varVal.Clear();
						varVal = pcfg->m_dMark;
						p->SetPropVal(L"Mark",&varVal);

						varVal.Clear();
					}
					else if (pArticleQuestion != NULL)
					{
						CYDQuestionRef* p = new CYDArticleQuestionRef(theApp.m_pDatabase);
						pLstQuestion->push_back(p);
						//野割ID
						_variant_t varVal;
						hr = theApp.m_pDatabase->GetField(L"OBJID", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						AddRecord(strKey, OBJID(varVal));
						p->SetID(OBJID(varVal));
						varVal.Clear();

						//野割ARTICLE
						hr = theApp.m_pDatabase->GetField(L"ARTICLE", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"ARTICLE", &varVal);
						varVal.Clear();
						//野割typeid;
						hr = theApp.m_pDatabase->GetField(L"TYPEID", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"TYPEID", &varVal);
						varVal.Clear();

						//野割usedcount
						hr = theApp.m_pDatabase->GetField(L"USEDCOUNT", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"USEDCOUNT", &varVal);
						varVal.Clear();
						//野割hardlevel
						hr = theApp.m_pDatabase->GetField(L"HARDLEVEL", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"HARDLEVEL", &varVal);
						varVal.Clear();

						//野割CREATOR
						hr = theApp.m_pDatabase->GetField(L"CREATOR", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"CREATOR", &varVal);
						varVal.Clear();

						//野割CREATEDATE
						hr = theApp.m_pDatabase->GetField(L"CREATEDATE", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"CREATEDATE", &varVal);
						varVal.Clear();
						//野割QNUM
						hr = theApp.m_pDatabase->GetField(L"QNUM", varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(L"QNUM", &varVal);
						//野割Image
						hr = theApp.m_pDatabase->GetField(FIELD_CHOICEQUESTION_IMAGETITLE, varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(FIELD_CHOICEQUESTION_IMAGETITLE, &varVal);

						//野割mode
						hr = theApp.m_pDatabase->GetField(FIELD_CHOICEQUESTION_TITLEMODE, varVal);
						if (FAILED(hr))
						{
							return hr;
						}
						p->SetPropVal(FIELD_CHOICEQUESTION_TITLEMODE, &varVal);
						//野割Mark
						varVal.Clear();
						varVal = pcfg->m_dMark;
						p->SetPropVal(L"Mark",&varVal);

						varVal.Clear();
					}
					else
					{
						ASSERT(FALSE);
					}
					bFirst = FALSE;
				}
				theApp.m_pDatabase->MoveNext();
			}
		}
	}

	return S_OK;
}

HRESULT CSelectorEngine::GetQuestionByCfgItemFromAccess(CQuestionCfgStruct* pcfg, CYDQuestionRef* pQTmp)
{
	HRESULT hr = E_FAIL;
	CString strSQL = _T(" select Top  ");
	CString strNum;
	int iCount = pcfg->m_cNum;
	strNum.Format(L"%d", iCount);
	strSQL += strNum;
	strSQL += _T(" * from ");
	CString strTableName;
	hr = pQTmp->GetDBName(strTableName);
	if(FAILED(hr))
	{
		return hr;
	}
	strSQL += strTableName;
	strSQL += L" WHERE ";
	CString condition;
	GetQueryConditionFromFactor(pcfg, &condition);
	strSQL += condition;
	if (condition.GetLength() > 0)
	{
		strSQL += L" and ";
	}
	strSQL += L" objid in (select id_b from ";
	strSQL += DB_VAULTQUESTION;
	strSQL += L" where id_a = ";
	CString strIDVault;
	strIDVault.Format(L"%d", pcfg->m_vaultID);
	strSQL += strIDVault;
	strSQL += L" and id_type = ";
	CString strTypeID;
	strTypeID.Format(L"%d", pcfg->m_QTypeID);
	strSQL += strTypeID;
	
	CString strKey;
	strKey.Format(L"%d:%d", pcfg->m_vaultID, pcfg->m_QTypeID);
	std::list<OBJID> lstRecords;
	GetRecords(strKey, lstRecords);
	if (lstRecords.size() > 0)
	{
		strSQL += L" and id_b not in(";
		auto itrID = lstRecords.begin();
		CString strID;
		strID.Format(L"%d", *itrID);
		strSQL += strID;
		++itrID;
		for(; itrID!=lstRecords.end(); ++itrID)
		{
			strSQL += L",";
			strID.Format(L"%d", *itrID);
			strSQL += strID;
		}
		strSQL += L")";
	}
	
	strSQL += L") ";

	if (theApp.m_pDatabase->GetDBType() == ACCESS)
	{
		strSQL += L" Order BY Rnd(time() - USEDCOUNT-objid) ";
	}
	else if (theApp.m_pDatabase->GetDBType() == SQLSERVER)
	{
		strSQL += L" Order BY  NewID()";
	}
	theApp.m_pDatabase->InitializeSQL(_bstr_t(strSQL));
	hr = theApp.m_pDatabase->ExecuteSQL();
	if (FAILED(hr))
	{
		return hr;
	}

 	return S_OK;
}

HRESULT CSelectorEngine::GetQueryConditionFromFactor(CQuestionCfgStruct* pcfg, CString* condition)
{
	if (pcfg->m_lstFactors.size() > 0)
	{
		auto itr = pcfg->m_lstFactors.begin();
		*condition += L" ";
		*condition += (*itr).m_field;
		
		if ((*itr).m_field.Find(L"C") >= 0)
		{
			*condition += L"=";
			*condition += L"'";
			*condition += (*itr).m_value;
			*condition += L"'";
		}
		else
		{
			long lfactorvalue = _ttoi((*itr).m_value);
			long lfactorrange = _ttoi((*itr).m_range);
			if (lfactorrange == 0)
			{
				*condition += L"=";
				*condition += (*itr).m_value;
			}
			else
			{
				CString strFormat = L" between %d and %d ";
				CString str;
				str.Format(strFormat, lfactorvalue-lfactorrange, lfactorvalue+lfactorrange);
				*condition += str;
			}
			
		}
		++itr;
		for (; itr != pcfg->m_lstFactors.end(); ++itr)
		{
			*condition += L" and ";
			*condition += (*itr).m_field;
			if ((*itr).m_field.Find(L"C") >= 0)
			{
				*condition += L"=";
				*condition += L"'";
				*condition += (*itr).m_value;
				*condition += L"'";
			}
			else
			{
				long lfactorvalue = _ttoi((*itr).m_value);
				long lfactorrange = _ttoi((*itr).m_range);
				if (lfactorrange == 0)
				{
					*condition += L"=";
					*condition += (*itr).m_value;
				}
				else
				{
					CString strFormat = L" between %d and %d ";
					CString str;
					str.Format(strFormat, lfactorvalue-lfactorrange, lfactorvalue+lfactorrange);
					*condition += str;
				}
			
			}
		}
	}
	

	return S_OK;
}

BOOL CSelectorEngine::AddRecord(CString key, OBJID id)
{
	auto itr = m_mapRecord.find(key);
	if (itr != m_mapRecord.end())
	{
		itr->second.push_back(id);
	}
	else
	{
		std::list<OBJID> lst;
		lst.push_back(id);
		m_mapRecord.insert(std::map<CString, std::list<OBJID>>::value_type(key, lst));
	}

	return TRUE;
}
BOOL CSelectorEngine::GetRecords(CString key, std::list<OBJID>& ids)
{
	ids.clear();
	auto itr = m_mapRecord.find(key);
	if (itr != m_mapRecord.end())
	{
		ids = itr->second;
	}

	return TRUE;
}