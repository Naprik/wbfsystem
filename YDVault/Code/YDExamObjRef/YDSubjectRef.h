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
	HRESULT RemovePaper(CYDObjectRef* _pPaper);//�Ƴ���_pPaper�Ĺ�ϵ
	virtual HRESULT Remove();
};
