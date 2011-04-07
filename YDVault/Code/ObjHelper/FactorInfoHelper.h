#pragma once
#include <list>
class CDatabaseEx;
class CYDObjectRef;
class BASE_DLL_API CFactorInfoHelper
{
public:
	CFactorInfoHelper(void);
	~CFactorInfoHelper(void);
public:
	HRESULT GetFactorInfoByVaultQType(CDatabaseEx* pDb,CYDObjectRef* _pVault,CYDObjectRef* _pQType,
										std::list<CYDObjectRef*> *_pListFactorInfo);
};

