#include "StdAfx.h"
#include "YdFactorInfoItemObjRef.h"



CYdFactorInfoItemObjRef::CYdFactorInfoItemObjRef(CDatabaseEx* pDb)
	: CYDObjectRef(pDb)
{
	m_strDBName = DB_YDFACTORINFOITEM;
	AddPropDef(FIELD_YDFACTORINFOITEM_VAULTID,VT_I4,_T("0"));
	AddPropDef(FIELD_YDFACTORINFOITEM_QTYPEID,VT_I4,_T("0"));
	AddPropDef(FIELD_YDFACTORINFOITEM_FACTORNAME,VT_BSTR,_T(""));
	AddPropDef(FIELD_YDFACTORINFOITEM_FIELDNAME,VT_BSTR,_T(""));
	AddPropDef(FIELD_YDFACTORINFOITEM_MIN,VT_I4,_T("0"));
	AddPropDef(FIELD_YDFACTORINFOITEM_MAX,VT_I4,_T("0"));
	AddPropDef(FIELD_YDFACTORINFOITEM_DESCRIPTION,VT_BSTR,_T(""));
}


CYdFactorInfoItemObjRef::~CYdFactorInfoItemObjRef(void)
{
}

