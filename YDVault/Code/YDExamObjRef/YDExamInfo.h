#pragma once
#include "..\objref\ydobjectref.h"

class BASE_DLL_API CYDExamInfo : public CYDObjectRef
{
public:
	CYDExamInfo(CDatabaseEx* pDB);
	~CYDExamInfo(void);
};
