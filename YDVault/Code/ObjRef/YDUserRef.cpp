#include "StdAfx.h"
#include "YDUserRef.h"
#include "../Base/DataHandler.h"

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
HRESULT CYDUserRef::GetGender(CString* pStrGender)
{
	CComVariant var;
	GetPropVal(FIELD_YDUSER_GENDER, &var);
	long lGender = CDataHandler::VariantToLong(var);
	if (lGender == 0)
	{
		*pStrGender = L"ÄÐ";
	}
	else if (lGender == 1)
	{
		*pStrGender = L"Å®";
	}
	else
	{
		*pStrGender = L"";
	}

	return S_OK;
}

HRESULT CYDUserRef::SetGender(const CString& strGender)
{
	long lGender = 0;
	if (strGender.CompareNoCase(L"ÄÐ") == 0)
	{
		lGender = 0;
	}
	else if (strGender.CompareNoCase(L"Å®") == 0)
	{
		lGender = 1;
	}
	CComVariant varGender((long)lGender);
	SetPropVal(FIELD_YDUSER_GENDER, &varGender);
	
	return S_OK;
}