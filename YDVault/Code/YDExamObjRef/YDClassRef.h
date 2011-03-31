#pragma once
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef/YDLinkRef.h"

class BASE_DLL_API CYDClassRef : public CYDObjectRef
{
public:
	CYDClassRef(CDatabaseEx* pDB);
	~CYDClassRef(void);
public:
	HRESULT GetDepartment(CYDObjectRef** _pDepartRef, CYDLinkRef** _link);
	HRESULT GetAllStudents(std::list<CYDObjectRef*> *_lstStudents,
						std::list<CYDLinkRef*> *_links = NULL);

	HRESULT Remove();
};
