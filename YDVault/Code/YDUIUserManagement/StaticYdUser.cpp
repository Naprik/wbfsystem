#include "StdAfx.h"
#include "StaticYdUser.h"
#include "../Base\AutoClean.h"
#include "../ObjRef\YDUserRef.h"
#include "../ObjRef\PropQueryContidition.h"

CStaticYdUser CStaticYdUser::m_staticYdUser;

CStaticYdUser::CStaticYdUser(void)
{
	m_pCurUser = NULL;
}

CStaticYdUser::~CStaticYdUser(void)
{
	CPtrAutoClean<CYDUserRef> clr(m_pCurUser);
}

HRESULT CStaticYdUser::GetCurUser(CYDUserRef*& _pUser)
{
	HRESULT hr = E_FAIL;
	_pUser = m_pCurUser;
	return S_OK;
}
HRESULT CStaticYdUser::SetCurUser(CYDUserRef*& _pUser)
{
	HRESULT hr = E_FAIL;
	{
		CPtrAutoClean<CYDUserRef> clr(m_pCurUser);
	}
	m_pCurUser = _pUser;
	return S_OK;

}