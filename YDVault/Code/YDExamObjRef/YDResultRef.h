#pragma once
#include "../ObjRef\YDObjectRef.h"

class BASE_DLL_API CYDResultRef : public CYDObjectRef
{
public:
	CYDResultRef(CDatabaseEx* pDB);
	~CYDResultRef(void);
};
