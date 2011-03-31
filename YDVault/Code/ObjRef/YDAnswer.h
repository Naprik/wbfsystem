#pragma once
#include "ydobjectref.h"

class BASE_DLL_API CYDAnswer : public CYDObjectRef
{
public:
	CYDAnswer(CDatabaseEx* pDb);
	~CYDAnswer(void);
};
