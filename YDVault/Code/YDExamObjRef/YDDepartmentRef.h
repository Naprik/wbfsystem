#pragma once
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef/YDLinkRef.h"
//����
class BASE_DLL_API CYDDepartmentRef : public CYDObjectRef
{
public:
	CYDDepartmentRef(CDatabaseEx* pDB);
	~CYDDepartmentRef(void);
public:
	virtual HRESULT Remove();
	//�õ��ò��ŵİ༶
	virtual HRESULT GetAllClasses(std::list<CYDObjectRef*> *_lstClasses, std::list<CYDLinkRef*> *_links = NULL);
};
