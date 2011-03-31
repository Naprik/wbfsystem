#include "StdAfx.h"
#include "YDUserRef.h"

CYDUserRef::CYDUserRef(CDatabaseEx* pDB): CYDObjectRef(pDB)
{
	m_strDBName = DB_YDUSER;
	AddPropDef(FIELD_YDUSER_NAME,VT_BSTR,_T(""));
	AddPropDef(FIELD_YDUSER_PASSWORD,VT_BSTR,_T(""));
	AddPropDef(FIELD_YDUSER_AUTHORITY,VT_I4, _T("0"));
}

CYDUserRef::~CYDUserRef(void)
{
}

HRESULT CYDUserRef::IsSysUser(VARIANT_BOOL* _bAdmin)
{
	HRESULT hr = E_FAIL;
	if(m_uObjID == 1)
	{
		*_bAdmin = VARIANT_TRUE;
	}
	else
	{
		*_bAdmin = VARIANT_FALSE;
	}
	
	return S_OK;
}
