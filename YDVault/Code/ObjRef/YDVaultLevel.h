#pragma once

#include "ydobjectref.h"

class BASE_DLL_API CYDVaultLevel : public CYDObjectRef
{
public:
	CYDVaultLevel(CDatabaseEx* pDb);
	~CYDVaultLevel(void);
};

