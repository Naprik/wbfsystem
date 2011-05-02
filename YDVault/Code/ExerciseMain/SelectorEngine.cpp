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

	std::list<CQuestionCfgStruct*>::const_iterator itrCfg = pCfgMgr->m_lstQuestionStruct.begin();
	for (; itrCfg != pCfgMgr->m_lstQuestionStruct.end(); ++itrCfg)
	{
		CYDQuestionFactory factory;
		CYDQuestionType qtype(theApp.m_pDatabase);
		qtype.SetID((*itrCfg)->m_QTypeID);
		CYDQuestionRef* pQ = factory.CreateQuestion(&qtype, theApp.m_pDatabase);
		CPtrAutoClean<CYDQuestionRef> cleanptr(pQ);
		CQuestionCfgStruct* pcfg = *itrCfg;
		
		if ((pcfg->m_cNum) > 0)
		{
			DATABASE_TYPE dbType = theApp.m_pDatabase->GetDBType();
			if(ACCESS == dbType)
			{
				hr = GetQuestionByCfgItemFromAccess(pcfg, pQ);
				if (FAILED(hr))
				{
					return hr;
				}
			}
			else
			{
				ASSERT(FALSE);
			}
			CYDChoiceQuestionRef* pChoiceQuestion =	dynamic_cast<CYDChoiceQuestionRef*>(pQ);
			CYDArticleQuestionRef* pArticleQuestion = dynamic_cast<CYDArticleQuestionRef*>(pQ);
			while (!theApp.m_pDatabase->IsEOF())
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
	strSQL += L" ) ";

	strSQL += L" ORDER BY USEDCOUNT ASC,OBJID desc ";


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