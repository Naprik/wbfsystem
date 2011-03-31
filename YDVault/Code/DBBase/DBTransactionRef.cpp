#include "StdAfx.h"
#include "DBTransactionRef.h"
#include "DatabaseEx.h"

CDBTransactionContext CDBTransactionRef::m_context;

CDBTransactionRef::CDBTransactionRef(CDatabaseEx* _pDB,BOOL bAutoTransaction)
{
	m_pDB = _pDB;
	m_bBegin = FALSE;
	HRESULT hr = E_FAIL;
	if(bAutoTransaction)
	{
		hr = Begin();
		if(FAILED(hr))
		{
			CDBTransactionRef::GetContext()->SetCurrentTransaction(NULL);
			m_pDB = NULL;
			m_bBegin = FALSE;
			return;
		}
		m_bBegin = TRUE;
	}
}
CDBTransactionContext* CDBTransactionRef::GetContext()
{
	return &m_context;
}

CDBTransactionRef::~CDBTransactionRef(void)
{
	if(m_bBegin)
	{
		_ASSERT(m_pDB != NULL);
		Rollback();
	}
	m_pDB = NULL;
	m_bBegin = FALSE;

}

HRESULT CDBTransactionRef::Begin()
{
	ASSERT(m_pDB != NULL);
	if(m_bBegin)
	{
		return S_OK;
	}
	HRESULT hr = m_pDB->BeginTrans();
	if(FAILED(hr))
	{
		return hr;
	}
	m_bBegin = TRUE;
	CDBTransactionRef::GetContext()->SetCurrentTransaction(this);
	return S_OK;
}

HRESULT CDBTransactionRef::Commit()
{
	_ASSERT(m_pDB != NULL);
	CDBTransactionRef::GetContext()->SetCurrentTransaction(NULL);
	if(!m_bBegin) 
	{
		return E_FAIL;
	}
	HRESULT hr = m_pDB->CommitTrans();
	if(FAILED(hr))
	{
		return hr;
	}
	m_bBegin = FALSE;
	return S_OK;
}

HRESULT CDBTransactionRef::Rollback()
{
	_ASSERT(m_pDB != NULL);
	CDBTransactionRef::GetContext()->SetCurrentTransaction(NULL);
	if(!m_bBegin) 
	{
		return E_FAIL;
	}
	HRESULT hr = m_pDB->RollbackTrans();
	if(FAILED(hr))
	{
		return hr;
	}
	m_bBegin = FALSE;
	return S_OK;
}