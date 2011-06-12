#include "StdAfx.h"
#include "YDVaultLevel.h"


CYDVaultLevel::CYDVaultLevel(CDatabaseEx* pDb) : CYDObjectRef(pDb)
{
	m_strDBName = DB_VAULTLEVEL;
	AddPropDef(FIELD_VAULTLEVEL_VAULTID,VT_I4);
	AddPropDef(FIELD_VAULTLEVEL_LEVEL,VT_BSTR);	
	AddPropDef(FIELD_VAULTLEVEL_CONDITION,VT_I4);	
}


CYDVaultLevel::~CYDVaultLevel(void)
{
}
