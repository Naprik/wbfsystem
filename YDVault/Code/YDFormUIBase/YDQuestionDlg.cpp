#include "StdAfx.h"
#include "YDQuestionDlg.h"
#include "ListCtrlOperate.h"
#include "../ObjRef/YDQuestionType.h"
#include "../Base/DataHandler.h"
#include "../ObjRef/YdObjWrapper.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YDQuestionVault.h"
#include "../ObjRef/YDQuestionRef.h"

CYDQuestionDlg::CYDQuestionDlg(UINT nIDTemplate,CWnd* _pParent)
:CDialog(nIDTemplate,_pParent)
{
	m_pListOperate = NULL;
	m_index = 0;
	m_pObjWrapper= NULL;
	m_pQVault = NULL;
	m_pQType = NULL;
	m_uType = OP_NEW;
	m_bIsInPaperGenerate = FALSE;
	m_pInPaperGenrateObjWrapper = NULL;
}

CYDQuestionDlg::~CYDQuestionDlg(void)
{
	CListAutoClean<CYdKnowledge> clean1(m_allKPs);
	CListAutoClean<CYdObjWrapper> clean2(m_relatedKPs);
}

HRESULT CYDQuestionDlg::SetListOperate(CListCtrlOperate* _pListOperate,int _index /*= 0*/)
{
	m_pListOperate = _pListOperate;
	m_index = _index;
	return S_OK;
}


HRESULT CYDQuestionDlg::SetObjWrapper(CYdObjWrapper* _pObjWrapper)
{
	m_pObjWrapper = _pObjWrapper;
	return S_OK;
}

HRESULT CYDQuestionDlg::SetYdQValut(CYDQuestionVault* _pVault)
{
	m_pQVault = _pVault;
	return S_OK;
}

HRESULT CYDQuestionDlg::SetYdQType(CYDQuestionType* _pQType)
{
	m_pQType = _pQType;
	return S_OK;
}

HRESULT CYDQuestionDlg::SetOpType(OPERATION _uType /*= 0*/)
{
	m_uType = _uType;
	return S_OK;
}


HRESULT CYDQuestionDlg::CreateQTypeID(VARIANT* _valTypeID)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pQType);
	OBJID idType = ID_EMPTY;
	hr = m_pQType->GetID(&idType);
	if(FAILED(hr))
	{
		return hr;
	}
	_valTypeID->vt = VT_I4;
	_valTypeID->lVal = idType;
	return S_OK;
}
HRESULT CYDQuestionDlg::GetQuestionType(QTYPE* pType)
{	
	HRESULT hr = E_FAIL;
	
	CComVariant var;
	if (m_uType == OP_NEW)
	{
		hr = m_pQType->GetPropVal(L"TYPE", &var);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	else
	{
		CComVariant varID;
		hr = m_pObjWrapper->m_pObjRef->GetPropVal(L"TYPEID", &varID);
		if (FAILED(hr))
		{
			return hr;
		}
		OBJID idType = CDataHandler::VariantToLong(varID);
		CYDQuestionType qtype(m_pObjWrapper->m_pObjRef->m_pDb);
		qtype.SetID(idType);
		hr = qtype.GetPropVal(L"TYPE", &var);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	*pType = (QTYPE)CDataHandler::VariantToLong(var);
	return S_OK;
}

std::list<CYdKnowledge*>& CYDQuestionDlg::GetAllKnowledgPoint()
{
	if (m_allKPs.size() <= 0)
	{
		m_pQVault->GetAllKnowledgePoint(&m_allKPs);
	}
	
	return m_allKPs;
}

std::list<CYdObjWrapper*>& CYDQuestionDlg::GetQuestionRelatedKnowledgePoint()
{
	if (m_relatedKPs.size() <= 0 &&
		m_uType == OP_EDIT)
	{
		CYDQuestionRef* question = dynamic_cast<CYDQuestionRef*>(m_pObjWrapper->m_pObjRef);
		ASSERT(question != NULL);	
		std::list<CYdKnowledge*> lstKPs;
		std::list<CYDLinkRef*> lstLinks;
		question->GetAllRelatedKnowledgePoints(&lstKPs, &lstLinks);
		std::list<CYdKnowledge*>::const_iterator itrKP = lstKPs.begin();
		std::list<CYDLinkRef*>::const_iterator itrLink = lstLinks.begin();
		for (; itrKP != lstKPs.end(); ++itrKP, ++itrLink)
		{
			CYdObjWrapper*  pObjWrapper = new CYdObjWrapper();
			pObjWrapper->m_pObjRef = *itrKP;
			pObjWrapper->m_pLinkRef = *itrLink;
			m_relatedKPs.push_back(pObjWrapper);
		}
	}

	return m_relatedKPs;
}