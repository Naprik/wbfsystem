#pragma once
#include "YDObjectRef.h"
class CDatabaseEx;



class BASE_DLL_API CYDQuestionType
	:public CYDObjectRef
{
public:
	CYDQuestionType(CDatabaseEx* pDB);
	~CYDQuestionType(void);
public:
	virtual HRESULT GetLabel(CString* pStr);
	HRESULT GetArticleRelateQuestionsCount(OBJID vaultID, std::list<UINT>* pLstCount);
	static HRESULT GetAllQType(CDatabaseEx* _pDB,std::list<CYDQuestionType*>* _lstAllQType);
};
