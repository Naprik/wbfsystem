#pragma once
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef/YDLinkRef.h"
//部门
class BASE_DLL_API CYDDepartmentRef : public CYDObjectRef
{
public:
	CYDDepartmentRef(CDatabaseEx* pDB);
	~CYDDepartmentRef(void);
public:
	virtual HRESULT Remove();
	//得到该部门的班级
	virtual HRESULT GetAllClasses(std::list<CYDObjectRef*> *_lstClasses, std::list<CYDLinkRef*> *_links = NULL);
};
