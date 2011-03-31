#include "stdafx.h"
#include "jahobject.h"

IMPLEMENT_DYNCREATE(CJahObject, CObject)

CJahObject::CJahObject()
{
}
CJahObject::~CJahObject()
{
}
CRuntimeClass * CJahObject::GetObjClass()
{
	CRuntimeClass* pClassRef = this->GetRuntimeClass();
    return pClassRef;
}

CString CJahObject::GetObjName()
{
	CString ret = "";	
	CRuntimeClass* pClassRef = GetObjClass();
	if ( pClassRef )
       ret.Format("%s",pClassRef->m_lpszClassName);
	return ret;
}

int CJahObject::GetObjSize()
{
	int ret = 0;	
	CRuntimeClass* pClassRef = GetObjClass();
	if ( pClassRef )
       ret = pClassRef->m_nObjectSize;
	return ret;
}
