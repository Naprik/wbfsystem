#include "StdAfx.h"
#include "YdObjWrapper.h"
#include "../Base/AutoClean.h"
#include "YDObjectRef.h"
#include "YDLinkRef.h"

CYdObjWrapper::CYdObjWrapper(void)
{
	m_pObjRef = NULL;
	m_pLinkRef = NULL;
	m_pRelatedWnd = NULL;
	m_hRelatedItem = NULL;
	m_state = S_UNKNOW;
}

CYdObjWrapper::~CYdObjWrapper(void)
{
	CPtrAutoClean<CYDObjectRef> clr1(m_pObjRef);
	CPtrAutoClean<CYDLinkRef> clr2(m_pLinkRef);
}
HRESULT CYdObjWrapper::Save()
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pObjRef != NULL && m_pLinkRef != NULL);
	hr = m_pObjRef->Save();
	if(FAILED(hr))
	{
		return hr;
	}
	hr = m_pLinkRef->Save();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
HRESULT CYdObjWrapper::Update()
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pObjRef != NULL && m_pLinkRef != NULL);
	hr = m_pObjRef->Update();
	if(FAILED(hr))
	{
		return hr;
	}
	hr = m_pLinkRef->Update();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
HRESULT CYdObjWrapper::Remove()
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pObjRef != NULL && m_pLinkRef != NULL);
	hr = m_pObjRef->Remove();
	if(FAILED(hr))
	{
		return hr;
	}
	hr = m_pLinkRef->Remove();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYdObjWrapper::Persist()
{
	HRESULT hr = E_FAIL;
	if (m_state == S_UNKNOW)
	{
		return S_FALSE;
	}
	if (m_state & S_NEED_UPDATE_OBJECT)
	{
		hr = m_pObjRef->Update();
		if (FAILED(hr))
		{
			return hr;
		}
	}
	if (m_state & S_NEED_UPDATE_LINK)
	{
		hr = m_pLinkRef->Update();
		if (FAILED(hr))
		{
			return hr;
		}
	}
	if (m_state & S_NEED_SAVE_OBJECT)
	{
		hr = m_pObjRef->Save();
		if (FAILED(hr))
		{
			return hr;
		}
	}
	if (m_state & S_NEED_SAVE_LINK)
	{
		hr = m_pLinkRef->Save();
		if (FAILED(hr))
		{
			return hr;
		}
	}

	if (m_state & S_NEED_DELETE_OBJECT)
	{
		hr = m_pObjRef->Remove();
		if (FAILED(hr))
		{
			return hr;
		}
	}

	if (m_state & S_NEED_DELETE_LINK)
	{
		hr = m_pLinkRef->Remove();
		if (FAILED(hr))
		{
			return hr;
		}
	}
	m_state = S_UNKNOW;
	return S_OK;
}