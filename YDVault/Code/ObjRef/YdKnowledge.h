#pragma once
#include "YDObjectRef.h"

class BASE_DLL_API CYdKnowledge : public CYDObjectRef
{
public:
	CYdKnowledge(CDatabaseEx* pDB);
	~CYdKnowledge(void);
public:
	virtual HRESULT GetLabel(CString* pStr);

	HRESULT IsFieldExistOfVault(const CString& strField, const CString& strVaule, OBJID vaultID, BOOL* pbExist);
};
