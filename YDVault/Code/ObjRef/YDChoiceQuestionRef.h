#pragma once
#include "YDQuestionRef.h"
#include <list>

class BASE_DLL_API CYDChoiceQuestionRef
	:	public CYDQuestionRef
{
public:
	CYDChoiceQuestionRef(CDatabaseEx* pDb);
	~CYDChoiceQuestionRef(void);

public:
	bool ToString(CString* pStr);

	HRESULT PersistToMSWord(int* pIndex, CWordAppHelper* pWord, CWordAppHelper* pWordAnswer, const CString& strFolder);

	virtual HRESULT GetLabel(CString* pStr);
	virtual HRESULT GetTitleMode(TITLEMODE* titlemode);
	HRESULT GetOptionList(std::list<CString>* lst);
	HRESULT SetOptionList(std::list<CString>* lst);

	HRESULT GetAnswerList(std::list<CString>* lst);
public:
	std::list<CString>	m_LstSequence;
};
