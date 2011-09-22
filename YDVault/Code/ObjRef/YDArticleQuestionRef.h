#pragma once
#include "YDQuestionRef.h"
class CYDLinkRef;
class CYDMediaRef;

class BASE_DLL_API CYDArticleQuestionRef
		:	public CYDQuestionRef
{
public:
	CYDArticleQuestionRef(CDatabaseEx* pDb);
	~CYDArticleQuestionRef(void);
public:
	//当前是阅读题，得到它的下面的题目，如一道阅读有有多个选择题
	HRESULT GetChildQueston(std::list<CYDQuestionRef*>* _lstQuestion,
		std::list<CYDLinkRef*>* _sub_link = NULL);

	virtual HRESULT GetLabel(CString* pStr);
	virtual HRESULT GetPropVal(BSTR _bstrPropName,VARIANT* _val);
	virtual HRESULT Remove();

	bool ToString(CString* pStr);
	virtual HRESULT GetStandardAnswer(CString &_str);
	virtual HRESULT GetStandardAnswer(std::list<CString> &_lstStdAnswer);
	virtual HRESULT GetStandardAnswer(std::list<std::pair<CString,CString>> &_lstStdAnswer);
	virtual HRESULT GetTitleMode(TITLEMODE* titlemode);

	HRESULT PersistToMSWord(int* pIndex, CWordAppHelper* pWord, CWordAppHelper* pWordAnswer, const CString& strFolder);

	HRESULT GetUnderLineNum(int* pNum);
private:
	HRESULT ToString(int* pIndex, CString* pStr);
	HRESULT WriteArticle(int* pIndex, CString& strArticle, CWordAppHelper* pWord);

	HRESULT PersistListioningToMSWord(int* pIndex, CWordAppHelper* pWord, CWordAppHelper* pWordAnswer, const CString& strFolder, QTYPE qType);
	
	HRESULT PersistUnlineTypeToMSWord(int* pIndex, CWordAppHelper* pWord, CWordAppHelper* pWordAnswer, const CString& strFolder, QTYPE qType);
	
	HRESULT PersistNormalTypeToMSWord(int* pIndex, CWordAppHelper* pWord, CWordAppHelper* pWordAnswer, const CString& strFolder, QTYPE qType);
	
	std::list<CString>	m_LstSequence;
};
