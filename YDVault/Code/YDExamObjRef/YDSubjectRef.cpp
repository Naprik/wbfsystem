#include "StdAfx.h"
#include "YDSubjectRef.h"
#include "../DBBase/DatabaseEx.h"
#include "../Base\DataHandler.h"
#include "../ObjRef\YDPaper.h"
#include "../Base\AutoClean.h"
//#include "YDPaperRef.h"

CYDSubjectRef::CYDSubjectRef(CDatabaseEx* pDB)
:CYDObjectRef(pDB)
{
	m_strDBName = DB_YDSUBJECT;//考试科目信息表
	AddPropDef(FIELD_YDSUBJECT_ID,VT_BSTR);        //科目编号
	AddPropDef(FIELD_YDSUBJECT_NAME,VT_BSTR); //科目的名称
}

CYDSubjectRef::~CYDSubjectRef(void)
{
}


HRESULT CYDSubjectRef::GetPapers(std::list<CYDObjectRef*> *_lstPaper,
				  std::list<CYDLinkRef*> *_links /*= NULL*/)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pDb != NULL);
	CString strSQL = _T("SELECT OBJID,ID_B FROM ");
	strSQL += DB_YDLINK_SUBJECT_PAPER;
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
		UINT idPaper= CDataHandler::VariantToLong(val);
		CYDPaper* pPaper = new CYDPaper(m_pDb);
		_lstPaper->push_back(pPaper);
		hr = pPaper->SetID(idPaper);
		if(FAILED(hr))
		{
			return hr;
		}
		if(_links != NULL)
		{
			_variant_t valLinkID;
			hr = m_pDb->GetField(_variant_t(_T("OBJID")),valLinkID);
			UINT idLink = CDataHandler::VariantToLong(valLinkID);
			CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb, DB_YDLINK_SUBJECT_PAPER);
			hr = pLinkRef->PutObjRef(this, pPaper);
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

HRESULT CYDSubjectRef::AddPaper(CYDObjectRef* _pPaper)
{
	HRESULT hr = E_FAIL;
	CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb,DB_YDLINK_SUBJECT_PAPER);
	CPtrAutoClean<CYDLinkRef> clr(pLinkRef);
	hr = pLinkRef->PutObjRef(this,_pPaper);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pLinkRef->Save();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYDSubjectRef::RemovePaper(CYDObjectRef* _pPaper)
{
	HRESULT hr = E_FAIL;
	CYDLinkRef* pLinkRef = new CYDLinkRef(m_pDb, DB_YDLINK_SUBJECT_PAPER);
	hr = pLinkRef->PutObjRef(this, _pPaper);
	if(FAILED(hr))
	{
		return hr;
	}
	CPtrAutoClean<CYDLinkRef> clr(pLinkRef);
	hr = pLinkRef->LoadByObj();
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pLinkRef->Remove();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
HRESULT CYDSubjectRef::Remove()
{
	HRESULT hr = E_FAIL;
	//将其与Paper的关系移除，但Paper本身不删除
	std::list<CYDObjectRef*> lstPaper;
	std::list<CYDLinkRef*> lstLink;
	CListAutoClean<CYDObjectRef> clr1(lstPaper);
	CListAutoClean<CYDLinkRef> clr2(lstLink);
	hr = GetPapers(&lstPaper,&lstLink);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CYDLinkRef*>::const_iterator itr = lstLink.begin();
		itr != lstLink.end();++itr)
	{
		hr = (*itr)->Remove();
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

