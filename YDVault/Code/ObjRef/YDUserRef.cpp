#include "StdAfx.h"
#include "YDUserRef.h"

CYDUserRef::CYDUserRef(CDatabaseEx* pDB): CYDObjectRef(pDB)
{
	m_strDBName = DB_YDUSER;
	AddPropDef(FIELD_YDUSER_NAME,VT_BSTR,_T(""));
	AddPropDef(FIELD_YDUSER_PASSWORD,VT_BSTR,_T(""));
	AddPropDef(FIELD_YDUSER_AUTHORITY,VT_I4, _T("0"));
	AddPropDef(FIELD_YDUSER_AGE,VT_I4, _T("0"));
	AddPropDef(FIELD_YDUSER_GENDER,VT_BSTR,_T(""));
	AddPropDef(FIELD_YDUSER_ID,VT_BSTR,_T(""));
	AddPropDef(FIELD_YDUSER_LEVEL,VT_BSTR,_T(""));
	AddPropDef(FIELD_YDUSER_DEPARTMENT,VT_BSTR,_T(""));
	AddPropDef(FIELD_YDUSER_MPHONE,VT_BSTR,_T(""));
	AddPropDef(FIELD_YDUSER_QQ,VT_BSTR,_T(""));
	AddPropDef(FIELD_YDUSER_EMAIL,VT_BSTR,_T(""));
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
