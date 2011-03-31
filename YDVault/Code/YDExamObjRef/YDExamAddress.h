#pragma once
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef/YDLinkRef.h"


//考场信息
class BASE_DLL_API CYDExamAddress :
	public CYDObjectRef
{
public:
	CYDExamAddress(CDatabaseEx* pDB);
	~CYDExamAddress(void);

	
	//得到考场的所有考试单元
	HRESULT GetAllUnits(std::list<CYDObjectRef*>* _lstUnits, std::list<CYDLinkRef*>* _links = NULL);
	HRESULT Remove();

protected:
	CYDObjectRef* CreateObject();
};
