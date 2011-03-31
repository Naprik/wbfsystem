#pragma once
#include "../ObjRef\YDObjectRef.h"

class CYDLinkRef;
class BASE_DLL_API CYDSubjectRef : public CYDObjectRef
{
public:
	CYDSubjectRef(CDatabaseEx* pDB);
	~CYDSubjectRef(void);
public:
	HRESULT GetPapers(std::list<CYDObjectRef*> *_lstPaper,
		std::list<CYDLinkRef*> *_links = NULL);
	HRESULT AddPaper(CYDObjectRef* _pPaper);
	HRESULT RemovePaper(CYDObjectRef* _pPaper);//移除与_pPaper的关系
	virtual HRESULT Remove();
};
