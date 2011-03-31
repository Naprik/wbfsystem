#include "StdAfx.h"
#include "YDPaperFolder.h"
#include "../DBBase/DatabaseEx.h"
#include "../Base/DataHandler.h"
#include "../Base/AutoClean.h"

CYDPaperFolder::CYDPaperFolder(CDatabaseEx* pDB) : CYDObjectRef(pDB)
{
	m_strDBName = DB_PAPERFOLDER;
	CYDPropDef* pPropDef = new CYDPropDef(_T("NAME"),VT_BSTR,_T(""));
	m_lstPropDef.push_back(pPropDef);
 	pPropDef = new CYDPropDef(_T("CREATOR"),VT_BSTR);
 	m_lstPropDef.push_back(pPropDef);
 	pPropDef = new CYDPropDef(_T("CREATEDATE"),VT_DATE);
 	m_lstPropDef.push_back(pPropDef);
 	pPropDef = new CYDPropDef(_T("DESCRIPTION"),VT_BSTR);
 	m_lstPropDef.push_back(pPropDef);
}

CYDPaperFolder::~CYDPaperFolder(void)
{
}

HRESULT CYDPaperFolder::GetChildPaperFolder(std::list<CYDPaperFolder*>* sub_paperfolder,
											std::list<CYDLinkRef*>* _sub_link)
{
	HRESULT hr = E_FAIL;

	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_B FROM ");
	strSQL += DB_PFOLDERHIERARCHY;
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
		CYDPaperFolder* pChild = new CYDPaperFolder(m_pDb);
		sub_paperfolder->push_back(pChild);
		hr = pChild->SetID(idChild);
		if(FAILED(hr))
		{
			return hr;
		}
		if(_sub_link != NULL)
		{
			_variant_t valLinkID;
			hr = m_pDb->GetField(_variant_t(_T("OBJID")),valLinkID);
			UINT idLink = CDataHandler::VariantToLong(valLinkID);
			CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb, DB_PFOLDERHIERARCHY);
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


HRESULT CYDPaperFolder::GetAllPapers(std::list<CYDPaper*>* _papers, 
									 std::list<CYDLinkRef*>* _links)
{
	ASSERT(m_pDb != NULL);
	
	HRESULT hr = E_FAIL;

	CString strSql = L"SELECT OBJID, ID_B FROM ";
	strSql += DB_PFOLDERPAPER;
	strSql += L" WHERE ID_A = ?";
	
	hr = m_pDb->InitializeSQL(_bstr_t(strSql));
	if (FAILED(hr))
	{
		return hr;
	}
	_variant_t vtParam((long)m_uObjID);
	hr = m_pDb->AddParameter(L"ID_A", adUnsignedInt, adParamInput, 
								sizeof(long), vtParam);
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
		_variant_t varVal;
		hr = m_pDb->GetField(_variant_t(L"ID_B"), varVal);
		if (FAILED(hr))
		{
			return hr;
		}
		long idB = CDataHandler::VariantToLong(varVal);
		CYDPaper* pPaper = new CYDPaper(m_pDb);
		pPaper->SetID(idB);
		_papers->push_back(pPaper);
		if (_links != NULL)
		{
			varVal.Clear();
			hr = m_pDb->GetField(_variant_t(L"OBJID"), varVal);
			if (FAILED(hr))
			{
				return hr;
			}
			long idLink = CDataHandler::VariantToLong(varVal);
			CYDLinkRef* pLink = new CYDLinkRef(m_pDb, DB_PFOLDERPAPER);
			pLink->PutObjRef(this, pPaper);
			pLink->SetID(idLink);
			_links->push_back(pLink);
		}
		hr = m_pDb->MoveNext();
		if (FAILED(hr))
		{
			return hr;
		}
	}

	return S_OK;
}

HRESULT CYDPaperFolder::Remove()
{
	HRESULT hr = E_FAIL;

	//É¾³ý×ÓÎÄ¼þ¼Ð
	std::list<CYDPaperFolder*> lstChildPFs;
	std::list<CYDLinkRef*> lstLinks;
	CListAutoClean<CYDPaperFolder> autoClean1(lstChildPFs);
	CListAutoClean<CYDLinkRef> autoClean2(lstLinks);
	hr = GetChildPaperFolder(&lstChildPFs, &lstLinks);
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<CYDPaperFolder*>::iterator itrObj = lstChildPFs.begin();
	std::list<CYDLinkRef*>::iterator itrLink = lstLinks.begin();
	for (; itrObj != lstChildPFs.end(); ++itrObj, ++itrLink)
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
	//É¾³ýÊÔ¾í
	std::list<CYDPaper*> lstPapers;
	std::list<CYDLinkRef*> lstLinks2;
	hr = GetAllPapers(&lstPapers, &lstLinks);
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<CYDPaper*>::iterator itrPaper = lstPapers.begin();
	itrLink = lstLinks2.begin();
	for (; itrPaper != lstPapers.end(); ++itrPaper, ++itrLink)
	{
		hr = (*itrPaper)->Remove();
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

	//É¾³ý×ÔÉí
	hr = CYDObjectRef::Remove();
	if (FAILED(hr))
	{
		return hr;
	}
	
	return S_OK;
}