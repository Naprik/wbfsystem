#pragma once
#include "ydobjectref.h"

class BASE_DLL_API CYDUserRef:
	public CYDObjectRef
{
public:
	CYDUserRef(CDatabaseEx* pDB);
	~CYDUserRef(void);
public:
	HRESULT IsSysUser(VARIANT_BOOL* _bAdmin);
	HRESULT GetGender(CString* pStrGender);
	HRESULT SetGender(const CString& strGender);
};
