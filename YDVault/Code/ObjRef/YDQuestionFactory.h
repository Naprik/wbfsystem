#pragma once

class CYDQuestionRef;
class CDatabaseEx;
class CYDQuestionType;

class BASE_DLL_API CYDQuestionFactory
{
public:
	CYDQuestionFactory();
	~CYDQuestionFactory(void);

public:
	CYDQuestionRef* CreateQuestion(CYDQuestionType* pType, CDatabaseEx* pDB);
};
