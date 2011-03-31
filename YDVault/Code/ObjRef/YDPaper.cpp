#include "StdAfx.h"
#include "YDPaper.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YDQuestionFactory.h"
#include "../ObjRef/YDQuestionType.h"
#include "../DBBase/DatabaseEx.h"
#include "../ObjRef/YDChoiceQuestionRef.h"
#include "../Base/StdioFileEx.h"
#include "../Base/DataHandler.h"
#include "../DBBase/DBTransactionRef.h"
#include "../ObjRef/YDArticleQuestionRef.h"
#include "YDAnswer.h"
#include <map>
#include "../DBBase/DatabaseEx.h"

///////////////////////////////////////////////////////////////////////////
CYDPaperBlockConfigureItem::CYDPaperBlockConfigureItem(UINT iLevel, UINT iRelNum,
													   UINT iCount,
													   const std::list<UINT>& lst)
:m_iLevel(iLevel)
,m_iRelNum(iRelNum)
,m_iCount(iCount)
,m_lstKP(lst)
{
	m_fMark = 0.0;
}

///////////////////////////////////////////////////////////////////////////
CYDPaperBlockConfigure::CYDPaperBlockConfigure()
{

}

CYDPaperBlockConfigure::CYDPaperBlockConfigure(const CYDPaperBlockConfigure &cfg)
{
	m_QType = cfg.m_QType;
	std::list<CYDPaperBlockConfigureItem*>::const_iterator itr = 
		m_lstConfigureItem.begin();
	for (; itr != m_lstConfigureItem.end(); ++itr)
	{
		CYDPaperBlockConfigureItem* pTmp = *itr;
		CYDPaperBlockConfigureItem* pItem = new CYDPaperBlockConfigureItem
			(pTmp->m_iLevel, pTmp->m_iRelNum, pTmp->m_iCount, pTmp->m_lstKP);
		m_lstConfigureItem.push_back(pItem);
	}
}

CYDPaperBlockConfigure::~CYDPaperBlockConfigure()
{
	CListAutoClean<CYDPaperBlockConfigureItem> clean(m_lstConfigureItem);
}

bool CYDPaperBlockConfigure::SetQType(OBJID qtype)
{
	m_QType = qtype;
	return true; 
}

bool CYDPaperBlockConfigure::AddConfigureItem(UINT iLevel, 
											  UINT iRelCount, 
											  UINT iCount, 
											  const std::list<UINT>& lstKP)
{
	CYDPaperBlockConfigureItem* pConfigItem = new 
		CYDPaperBlockConfigureItem(iLevel, iRelCount, iCount, lstKP);
	m_lstConfigureItem.push_back(pConfigItem);
	
	return true;
}

bool CYDPaperBlockConfigure::UpdateConfigureItem(UINT iLevel, UINT iRelCount, UINT iCount,double fMark /*= 0.0*/)
{
	std::list<CYDPaperBlockConfigureItem*>::const_iterator itr = m_lstConfigureItem.begin();
	for (; itr != m_lstConfigureItem.end(); ++itr)
	{
		if ((*itr)->m_iLevel == iLevel &&
			(*itr)->m_iRelNum == iRelCount)
		{
			(*itr)->m_iCount = iCount;
			(*itr)->m_fMark = fMark;
			return true;	
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////
CYDPaper::CYDPaper(CDatabaseEx* pDB) : CYDObjectRef(pDB)
{
	m_IDVault = ID_EMPTY;

	m_strDBName = DB_PAPER;
	CYDPropDef* pPropDef = new CYDPropDef(FIELD_YDPAPER_NAME,VT_BSTR,_T(""));
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_CREATOR,VT_BSTR);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_CREATEDATE,VT_DATE);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_DESCRIPTION,VT_BSTR);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_CODE, VT_BSTR);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_GUIDNAME, VT_BSTR);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_GUIDANSWERNAME, VT_BSTR);
	m_lstPropDef.push_back(pPropDef);

	pPropDef = new CYDPropDef(FIELD_YDPAPER_BEGIN1, VT_I4);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_END1, VT_I4);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_CHOICECOUNT1, VT_I4);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_MULTIFLAG1, VT_I4);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_ANSWERCHOICE1, VT_I4);
	m_lstPropDef.push_back(pPropDef);

	pPropDef = new CYDPropDef(FIELD_YDPAPER_BEGIN2, VT_I4);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_END2, VT_I4);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_CHOICECOUNT2, VT_I4);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_MULTIFLAG2, VT_I4);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_ANSWERCHOICE2, VT_I4);
	m_lstPropDef.push_back(pPropDef);

	pPropDef = new CYDPropDef(FIELD_YDPAPER_BEGIN3, VT_I4);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_END3, VT_I4);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_CHOICECOUNT3, VT_I4);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_MULTIFLAG3, VT_I4);
	m_lstPropDef.push_back(pPropDef);
	pPropDef = new CYDPropDef(FIELD_YDPAPER_ANSWERCHOICE3, VT_I4);
	m_lstPropDef.push_back(pPropDef);

	m_pTemplate = new CPaperTemplate();
	m_bSaved = FALSE;
}

CYDPaper::~CYDPaper(void)
{
	ClearContent();
	CListAutoClean<CYDPaperBlockConfigure> clean2(m_lstBlock);
	CPtrAutoClean<CPaperTemplate>	clean3(m_pTemplate);
}

HRESULT CYDPaper::ClearContent()
{
	MapContent::iterator itrMap = m_mapPaperContent.begin();
	for (; itrMap != m_mapPaperContent.end(); ++itrMap)
	{
		CListAutoClean<CYDQuestionRef> clean1(itrMap->second);
	}
	m_mapPaperContent.clear();
	return S_OK;
}

HRESULT CYDPaper::GetPaperMarkVal(long& _lVal)
{
	_lVal = 0;
	HRESULT hr = E_FAIL;
	std::list<CYDObjectRef*> lstAns;
	hr = GetStdAnswer(&lstAns);
	if(FAILED(hr))
	{
		return hr;
	}	
	CListAutoClean<CYDObjectRef> clr(lstAns);
	for(std::list<CYDObjectRef*>::const_iterator itr = lstAns.begin();
		itr != lstAns.end();++itr)
	{
		long lItrVal = 0;
		hr = (*itr)->GetPropVal(FIELD_YDANSWER_MARK,&lItrVal);
		if(FAILED(hr))
		{
			return hr;
		}	
		_lVal += lItrVal;
	}
	return S_OK;
}

HRESULT CYDPaper::GetStdAnswer(std::list<CYDObjectRef*>* _lstAns, std::list<CYDLinkRef*>* _links)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_B FROM ");
	strSQL += DB_PAPERANSWER;
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
	std::list<CYDAnswer*> lstAnsw;
	while(!m_pDb->IsEOF())
	{
		_variant_t val;
		hr = m_pDb->GetField(_variant_t("ID_B"), val);
		if(FAILED(hr))
		{
			return hr;
		}
		UINT idChild = CDataHandler::VariantToLong(val);
		CYDAnswer* pChild = new CYDAnswer(m_pDb);
		hr = pChild->SetID(idChild);
		if(FAILED(hr))
		{
			return hr;
		}
		lstAnsw.push_back(pChild);

		if(_links != NULL)
		{
			_variant_t valLinkID;
			hr = m_pDb->GetField(_variant_t(_T("OBJID")),valLinkID);
			UINT idLink = CDataHandler::VariantToLong(valLinkID);
			CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb,DB_PAPERANSWER);
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
			_links->push_back(pLinkRef);
		}
		hr = m_pDb->MoveNext();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	std::map<long, CYDAnswer*> mapCache;
	for (std::list<CYDAnswer*>::const_iterator itr = lstAnsw.begin();
		itr != lstAnsw.end(); ++itr)
	{
		CComVariant varNO;
		hr = (*itr)->GetPropVal(FIELD_YDANSWER_NO, &varNO);
		if (FAILED(hr))
		{
			return hr;
		}

		long lNo = CDataHandler::VariantToLong(varNO);
		mapCache.insert(std::map<long, CYDAnswer*>::value_type(lNo, *itr));
	}
	
	std::map<long, CYDAnswer*>::const_iterator itr = mapCache.begin();
	for (; itr != mapCache.end(); ++itr)
	{
		_lstAns->push_back(itr->second);
	}
	mapCache.clear();
	return S_OK;
}

HRESULT CYDPaper::GetStdAnswer(CString &_strAns)
{
	HREFTYPE hr = E_FAIL;
	std::list<CYDObjectRef*> lstAns;
	CListAutoClean<CYDObjectRef> clr(lstAns);
	hr = GetStdAnswer(&lstAns);
	if(FAILED(hr))
	{
		return hr;
	}
	_strAns = _T("");
	for(std::list<CYDObjectRef*>::const_iterator itr = lstAns.begin();
		itr != lstAns.end();++itr)
	{
		long lItrNo = 0;
		hr = (*itr)->GetPropVal(FIELD_YDANSWER_NO,&lItrNo);
		if(FAILED(hr))
		{
			return hr;
		}
		CString strItrAnswer;
		hr = (*itr)->GetPropVal(FIELD_YDANSWER_ANSWER,strItrAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
		CString strItrNoAnswer;
		strItrNoAnswer.Format(_T("%d%s"),lItrNo,strItrAnswer);
		_strAns += strItrNoAnswer;
	
	}
	return S_OK;
}

HRESULT CYDPaper::GetStdAsnwerByItemNo(long _lNo,CString& _strAnswer)
{
	HRESULT hr = E_FAIL;
	std::list<CYDObjectRef*> lstAns;
	CListAutoClean<CYDObjectRef> clr(lstAns);
	hr = GetStdAnswer(&lstAns);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = lstAns.begin();
		itr != lstAns.end();++itr)
	{
		long lItrNo = 0;
		hr = (*itr)->GetPropVal(FIELD_YDANSWER_NO,&lItrNo);
		if(FAILED(hr))
		{
			return hr;
		}
		if(lItrNo == _lNo)
		{
			hr = (*itr)->GetPropVal(FIELD_YDANSWER_ANSWER,_strAnswer);
			if(FAILED(hr))
			{
				return hr;
			}
			return S_OK;
		}
	}
	return S_OK;
}

HRESULT CYDPaper::Remove()
{
	HRESULT hr = E_FAIL;
	CString strPaperPath, strAnswerPath;
	hr = CreateVaultName(strPaperPath, strAnswerPath);
	if(FAILED(hr))
	{
		return hr;
	}
	if (!strPaperPath.IsEmpty())
	{
		CFtpRef* pFtpRef = NULL;
		CWnd* pWnd = AfxGetMainWnd();
		if(pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
		{
			pFtpRef = (CFtpRef*)pWnd->SendMessage(WM_YD_GET_FTPREF);
		}
		ASSERT(pFtpRef);
		hr = pFtpRef->Remove(strPaperPath);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pFtpRef->Remove(strAnswerPath);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	std::list<CYDObjectRef*> lstAnswer;
	std::list<CYDLinkRef*> lstLinks;
	CListAutoClean<CYDObjectRef> clr1(lstAnswer);
	CListAutoClean<CYDLinkRef> clr2(lstLinks);
	hr = GetStdAnswer(&lstAnswer, &lstLinks);
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<CYDObjectRef*>::iterator itrObj = lstAnswer.begin();
	std::list<CYDLinkRef*>::iterator itrLink = lstLinks.begin();
	for (; itrObj != lstAnswer.end(); ++itrObj, ++itrLink)
	{
		hr = (*itrObj)->Remove();
		if (FAILED(hr))
		{
			return hr;
		}
		hr = (*itrLink)->Remove();
		if (FAILED(hr))
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

HRESULT CYDPaper::ReplaceQuestion(const CString& strQType, CYDQuestionRef* pQTarget, CYDQuestionRef* pQRef)
{
	HRESULT hr = E_FAIL;
	MapContent::iterator itrMap = m_mapPaperContent.find(strQType);
	if (itrMap == m_mapPaperContent.end())
	{
		return E_FAIL;
	}
	std::list<CYDQuestionRef*>::iterator itrList = (*itrMap).second.begin();
	for (; itrList != (*itrMap).second.end(); ++itrList)
	{
		if (pQTarget == *itrList)
		{
			CPtrAutoClean<CYDQuestionRef> clr(pQTarget);
			*itrList = pQRef;
			break;
		}
	}

	return S_OK;
}

bool CYDPaper::SetQVault(OBJID id)
{
	m_IDVault = id;

	return true;
}

HRESULT CYDPaper::SetTemplate(const CString& strName)
{
	return m_pTemplate->Load(strName);
}

HRESULT CYDPaper::MakePaper(const CString& str)
{
	ASSERT(ID_EMPTY != m_IDVault);
	if (m_mapPaperContent.size() > 0)
	{
		ClearContent();
	}
	CComVariant varName(str);
	SetPropVal(L"NAME", &varName);
	std::list<CYDPaperBlockConfigure*>::const_iterator itrCfg = 
		m_lstBlock.begin();
	int i = 1;
	for (; itrCfg != m_lstBlock.end(); ++itrCfg)
	{
		std::list<CYDQuestionRef*> lstQuestions;
		HRESULT hr = GetQuestionsFromCfg(*itrCfg, &lstQuestions);
		if (FAILED(hr))
		{
			return hr;
		}
		if (lstQuestions.size() > 0)
		{
			CComVariant varName;
			CYDQuestionType qtype(m_pDb);
			qtype.SetID((*itrCfg)->m_QType);
			hr = qtype.GetPropVal(L"NAME", &varName);
			if (FAILED(hr))
			{
				return hr;
			}
			CString strQType = CDataHandler::VariantToString(varName);
			m_mapPaperContent.insert(MapContent::value_type(strQType, lstQuestions));
		}
		hr = UpdateUsedCount(&lstQuestions);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	
	return S_OK;
}

HRESULT CYDPaper::SetFtpRef(CFtpRef* ftp)
{
	m_pFtp = ftp;
	return S_OK;
}

HRESULT CYDPaper::CreateVaultName(CString& _strVaultName, CString& _strPaperAnswer)
{
	HRESULT hr = E_FAIL;
	//_strVaultName += _T("\\Vault\\");
	_variant_t valTYPE;
	hr = GetPropVal(L"TYPE",&valTYPE);
	if(FAILED(hr))
	{
		return hr;
	}
	MEDIATYPE MType = (MEDIATYPE)CDataHandler::VariantToLong(valTYPE);
	
	_strVaultName += _T("\\paper\\");
	_strPaperAnswer = _strVaultName;
	CComVariant valGuidName;
	hr = GetPropVal(L"GUIDNAME",&valGuidName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strGuidName = CDataHandler::VariantToString(valGuidName);
	if (strGuidName.IsEmpty())
	{
		_strVaultName = L"";
		_strPaperAnswer = L"";
		return S_OK;
	}
	_strVaultName += strGuidName;

	hr = GetPropVal(L"GUIDANSWERNAME", &valGuidName);
	if (FAILED(hr))
	{
		return hr;
	}

	strGuidName = CDataHandler::VariantToString(valGuidName);

	_strPaperAnswer += strGuidName;
	return S_OK;
}


HRESULT CYDPaper::PersistToMSWord(CWordAppHelper* pWord, 
								  CWordAppHelper* pWordAnswer, 
								  const CString& strFolder,
								  CPdemWait* pWait)
{
	HRESULT hr = E_FAIL;

	int index = 1;
	std::list<CPaperTemplateItem*>::const_iterator itr = m_pTemplate->m_lstItem.begin();

	for (; itr != m_pTemplate->m_lstItem.end(); ++itr)
	{
		CYDQuestionType qtype(m_pDb);
		qtype.SetID((*itr)->id);
		CComVariant varQTypeName;
		hr = qtype.GetPropVal(L"NAME", &varQTypeName);
		if (FAILED(hr))
		{
			return hr;
		}
		CString strQType = CDataHandler::VariantToString(varQTypeName);
		CYDPaper::MapContent::const_iterator itrMap = m_mapPaperContent.find(strQType);
		
		if (itrMap != m_mapPaperContent.end())
		{
			std::list<CYDQuestionRef*>::const_iterator itrQ = (itrMap->second).begin();
			CString strQType = itrMap->first;
			CString info;
			CString strFormat = L"屎壓伏撹%s��萩不昨...";
			info.Format(strFormat, strQType);
			pWait->ResetMessage(info);
			strQType += L"\r\n";
			pWord->Write(strQType);
			pWordAnswer->Write(strQType);
			for (; itrQ != (itrMap->second).end(); ++itrQ)
			{
				(*itrQ)->SetFtp(m_pFtp);
				hr = (*itrQ)->PersistToMSWord(&index, pWord, pWordAnswer, strFolder);
				if (FAILED(hr))
				{
					return hr;
				}
			}
			pWait->StepIt();
		}
	}

	return S_OK;
}

HRESULT CYDPaper::UpdateUsedCount(std::list<CYDQuestionRef*>* pLst)
{
	HRESULT hr = E_FAIL;
	if (pLst->size() > 0)
	{
		CDBTransactionRef trans(m_pDb, TRUE);
		std::list<CYDQuestionRef*>::iterator itr = pLst->begin();
		for (; itr != pLst->end(); ++itr)
		{
			_variant_t var;
			(*itr)->GetPropVal(L"USEDCOUNT", &var);
			long lCount = CDataHandler::VariantToLong(var);
			var.Clear();
			CString str;
			lCount++;
			str.Format(L"%d", lCount);
			CDataHandler::StringToVariant(str, VT_DECIMAL, &var);
			(*itr)->SetPropVal(L"USEDCOUNT", &var);
			hr = (*itr)->Update();
			if (FAILED(hr))
			{
				trans.Rollback();
				return hr;
			}
		}
		trans.Commit();

	}
	return S_OK;
}

HRESULT CYDPaper::GetQuestionsFromCfg(CYDPaperBlockConfigure* pCfg,
									  std::list<CYDQuestionRef*>* pLstQuestion)
{
	HRESULT hr = E_FAIL;

	CYDQuestionFactory factory;
	CYDQuestionType qtype(m_pDb);
	qtype.SetID(pCfg->m_QType);
	CYDQuestionRef* pQ = factory.CreateQuestion(&qtype, m_pDb);
	CPtrAutoClean<CYDQuestionRef> cleanptr(pQ);

	std::list<CYDPaperBlockConfigureItem*>::const_iterator itr = 
		pCfg->m_lstConfigureItem.begin();

	for (; itr != pCfg->m_lstConfigureItem.end(); ++itr)
	{
		if (((*itr)->m_iCount) > 0)
		{
			DATABASE_TYPE dbType = m_pDb->GetDBType();
			if (ORACLE == dbType)
			{
				hr = GetQuestionByCfgItemFromOracle(pCfg->m_QType, pQ, *itr);
				if (FAILED(hr))
				{
					return hr;
				}
			}
			else if(SQLSERVER == dbType)
			{
				hr = GetQuestionByCfgItemFromMSSQL(pCfg->m_QType, pQ, *itr);
				if (FAILED(hr))
				{
					return hr;
				}
			}
			else if(ACCESS == dbType)
			{
				hr = GetQuestionByCfgItemFromAccess(pCfg->m_QType,pQ,*itr);
				if (FAILED(hr))
				{
					return hr;
				}
			}
			else
			{
				ASSERT(FALSE);
			}
			CYDChoiceQuestionRef* pChoiceQuestion =
				dynamic_cast<CYDChoiceQuestionRef*>(pQ);
			CYDArticleQuestionRef* pArticleQuestion = 
				dynamic_cast<CYDArticleQuestionRef*>(pQ);
			while (!m_pDb->IsEOF())
			{
				if (pChoiceQuestion != NULL)
				{
					CYDQuestionRef* p = new CYDChoiceQuestionRef(m_pDb);
					pLstQuestion->push_back(p);
					//野割ID
					_variant_t varVal;
					hr = m_pDb->GetField(L"OBJID", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetID(OBJID(varVal));
					varVal.Clear();
					//野割code
					hr = m_pDb->GetField(L"CODE", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetPropVal(L"CODE", &varVal);
					varVal.Clear();
					//野割title
					hr = m_pDb->GetField(L"TITLE", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetPropVal(L"TITLE", &varVal);
					varVal.Clear();
					//野割choice
					hr = m_pDb->GetField(L"CHOICE", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetPropVal(L"CHOICE", &varVal);
					varVal.Clear();
					//野割typeid;
					hr = m_pDb->GetField(L"TYPEID", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetPropVal(L"TYPEID", &varVal);
					varVal.Clear();
					//野割answer
					hr = m_pDb->GetField(L"ANSWER", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetPropVal(L"ANSWER", &varVal);
					varVal.Clear();
					//野割usedcount
					hr = m_pDb->GetField(L"USEDCOUNT", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetPropVal(L"USEDCOUNT", &varVal);
					varVal.Clear();
					//野割hardlevel
					hr = m_pDb->GetField(L"HARDLEVEL", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetPropVal(L"HARDLEVEL", &varVal);
					varVal.Clear();

					//野割CREATOR
					hr = m_pDb->GetField(L"CREATOR", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetPropVal(L"CREATOR", &varVal);
					varVal.Clear();

					//野割CREATEDATE
					hr = m_pDb->GetField(L"CREATEDATE", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetPropVal(L"CREATEDATE", &varVal);
					varVal.Clear();
				}
				else if (pArticleQuestion != NULL)
				{
					CYDQuestionRef* p = new CYDArticleQuestionRef(m_pDb);
					pLstQuestion->push_back(p);
					//野割ID
					_variant_t varVal;
					hr = m_pDb->GetField(L"OBJID", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetID(OBJID(varVal));
					varVal.Clear();

					//野割ARTICLE
					hr = m_pDb->GetField(L"ARTICLE", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetPropVal(L"ARTICLE", &varVal);
					varVal.Clear();
					//野割typeid;
					hr = m_pDb->GetField(L"TYPEID", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetPropVal(L"TYPEID", &varVal);
					varVal.Clear();

					//野割usedcount
					hr = m_pDb->GetField(L"USEDCOUNT", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetPropVal(L"USEDCOUNT", &varVal);
					varVal.Clear();
					//野割hardlevel
					hr = m_pDb->GetField(L"HARDLEVEL", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetPropVal(L"HARDLEVEL", &varVal);
					varVal.Clear();

					//野割CREATOR
					hr = m_pDb->GetField(L"CREATOR", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetPropVal(L"CREATOR", &varVal);
					varVal.Clear();

					//野割CREATEDATE
					hr = m_pDb->GetField(L"CREATEDATE", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetPropVal(L"CREATEDATE", &varVal);
					varVal.Clear();
					//野割QNUM
					hr = m_pDb->GetField(L"QNUM", varVal);
					if (FAILED(hr))
					{
						return hr;
					}
					p->SetPropVal(L"QNUM", &varVal);
					varVal.Clear();
				}
				else
				{
					ASSERT(FALSE);
				}

				m_pDb->MoveNext();
			}
		}
		
	}

	return S_OK;
}

HRESULT CYDPaper::GetQuestionByCfgItemFromOracle(OBJID typeID, 
												 CYDQuestionRef* pQTmp, 
												 CYDPaperBlockConfigureItem* pItem)
{
	HRESULT hr = E_FAIL;
	CString strSQL = L"select * from (select * from ";
	CString strTableName;
	pQTmp->GetDBName(strTableName);
	strSQL += strTableName;
	strSQL += L" WHERE ";
	strSQL += L" HARDLEVEL = ";
	CString strLevel;
	strLevel.Format(L"%d", pItem->m_iLevel);
	strSQL += strLevel;
	if (pItem->m_iRelNum > 1)
	{
		strSQL += L" and QNUM = ";
		CString strQNum;
		strQNum.Format(L"%d", pItem->m_iRelNum);
		strSQL += strQNum;
	}
	strSQL += L" and objid in (select id_b from ";
	strSQL += DB_VAULTQUESTION;
	strSQL += L" where id_a = ";
	CString strIDVault;
	strIDVault.Format(L"%d", m_IDVault);
	strSQL += strIDVault;
	strSQL += L" and id_type = ";
	CString strTypeID;
	strTypeID.Format(L"%d", typeID);
	strSQL += strTypeID;
	strSQL += L" ) ";

	strSQL += L" ORDER BY USEDCOUNT ASC) where rownum <";
	CString strNum;
	int iCount = pItem->m_iCount + 1;
	strNum.Format(L"%d", iCount);
	strSQL += strNum;
	
	m_pDb->InitializeSQL(_bstr_t(strSQL));
	hr = m_pDb->ExecuteSQL();
	if (FAILED(hr))
	{
		return hr;
	}

	

	return S_OK;
}

HRESULT CYDPaper::GetQuestionByCfgItemFromMSSQL(OBJID typeID, 
												CYDQuestionRef* pQTmp,
												CYDPaperBlockConfigureItem* pItem)
{
	return S_OK;
}

HRESULT CYDPaper::GetQuestionByCfgItemFromAccess(OBJID typeID, CYDQuestionRef* pQTmp,
												 CYDPaperBlockConfigureItem* pItem)
{
	HRESULT hr = E_FAIL;
	CString strSQL = _T(" select Top  ");
	CString strNum;
	int iCount = pItem->m_iCount ;
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
	strSQL += L" HARDLEVEL = ";
	CString strLevel;
	strLevel.Format(L"%d", pItem->m_iLevel);
	strSQL += strLevel;
	if (pItem->m_iRelNum > 1)
	{
		strSQL += L" and QNUM = ";
		CString strQNum;
		strQNum.Format(L"%d", pItem->m_iRelNum);
		strSQL += strQNum;
	}
	strSQL += L" and objid in (select id_b from ";
	strSQL += DB_VAULTQUESTION;
	strSQL += L" where id_a = ";
	CString strIDVault;
	strIDVault.Format(L"%d", m_IDVault);
	strSQL += strIDVault;
	strSQL += L" and id_type = ";
	CString strTypeID;
	strTypeID.Format(L"%d", typeID);
	strSQL += strTypeID;
	strSQL += L" ) ";

	strSQL += L" ORDER BY USEDCOUNT ASC,OBJID desc ";
	

	m_pDb->InitializeSQL(_bstr_t(strSQL));
	hr = m_pDb->ExecuteSQL();
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}
bool CYDPaper::AddPaperBlockConfigure(CYDPaperBlockConfigure* pCfg)
{
	m_lstBlock.push_back(pCfg);
	return true;
}

HRESULT CYDPaper::ReplaceByLeastUsedCountQuestion(const CString& strQType,
														CYDQuestionRef* pTargetQuestion, 
														CYDQuestionRef** pQuestion)
{
	HRESULT hr = E_FAIL;

	CComVariant var;
	hr = pTargetQuestion->GetPropVal(L"HARDLEVEL", &var);
	if (FAILED(hr))
	{
		return hr;
	}
	CString strHardLevel = CDataHandler::VariantToString(var);
	var.Clear();
	hr = pTargetQuestion->GetPropVal(L"TYPEID", &var);
	if (FAILED(hr))
	{
		return hr;
	}
	long typeID = CDataHandler::VariantToLong(var);

	CString strTypeId = CDataHandler::VariantToString(var);
	CYDQuestionFactory factory;
	CYDQuestionType qType(m_pDb);
	qType.SetID(typeID);
	*pQuestion = factory.CreateQuestion(&qType, m_pDb);

	OBJID idQuestion = 0;
	
	hr = GetNextId(strQType, pTargetQuestion->m_strDBName, strHardLevel, strTypeId, &idQuestion);
	if (FAILED(hr))
	{
		return hr;
	}

	(*pQuestion)->SetID(idQuestion);

	hr = ReplaceQuestion(strQType, pTargetQuestion, *pQuestion);
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<CYDQuestionRef*> lst;
	lst.push_back(*pQuestion);
	UpdateUsedCount(&lst);

	return S_OK;
}

HRESULT CYDPaper::GetNextId(const CString& strQType, CString strTable, CString strHardLevel, CString strTypeId, OBJID* pID)
{
	HRESULT hr = E_FAIL;

	CString strSQL =  L" select objid from ";
	strSQL += strTable;
	strSQL += L"  where hardlevel=";
	strSQL += strHardLevel;
	strSQL += L" and objid in (select id_b from QVAULTQUESTION where id_a=";
	CString strVaultID;
	strVaultID.Format(L"%d", m_IDVault);
	strSQL += strVaultID;
	strSQL += L" and id_type = ";
	strSQL += strTypeId;
	strSQL += L" ) order by usedcount asc";

	m_pDb->InitializeSQL(_bstr_t(strSQL));
	m_pDb->ExecuteSQL();
	while (!m_pDb->IsEOF())
	{
		_variant_t var;
		m_pDb->GetField(L"OBJID", var);
		*pID = (long)(var);
		if (IsExist(strQType, *pID))
		{
			m_pDb->MoveNext();
		}
		else
		{
			break;
		}
	}

	return S_OK;
}

BOOL CYDPaper::IsExist(const CString& strQType, OBJID id)
{
	MapContent::iterator itrMap = m_mapPaperContent.find(strQType);
	if (itrMap == m_mapPaperContent.end())
	{
		return FALSE;
	}
	std::list<CYDQuestionRef*>::iterator itrList = (*itrMap).second.begin();
	for (; itrList != (*itrMap).second.end(); ++itrList)
	{
		OBJID idTarget;
		(*itrList)->GetID(&idTarget);
		if (idTarget == id)
		{
			return TRUE;
		}
	}

	return FALSE;
}

CYDObjectRef* CYDPaper::CreateObject()
{
	return new CYDPaper(m_pDb);
}