#pragma once
#include "YDObjectRef.h"
#include "YdKnowledge.h"
#include "YDLinkRef.h"
#include "YDMediaRef.h"
#include "../WordBase/WordAppHelper.h"
#include "../FtpBase/FtpRef.h"

class BASE_DLL_API CYDQuestionRef 
	: public CYDObjectRef
{
public:
	CYDQuestionRef(CDatabaseEx* pDb);
	~CYDQuestionRef(void);

public:
	virtual bool ToString(CString* pStr);
	virtual HRESULT GetStandardAnswer(CString &_str);
	virtual HRESULT GetStandardAnswer(std::list<CString> &_lstStdAnswer);

	virtual HRESULT PersistToMSWord(int* pIndex, CWordAppHelper* pWord, CWordAppHelper* pWordAnswer, const CString& strFolder);

	virtual HRESULT GetAllRelatedKnowledgePoints(std::list<CYdKnowledge*>* lstKP,
													std::list<CYDLinkRef*>* lstLinks);

	virtual HRESULT GetAllMedia(std::list<CYDMediaRef*>* lstMedia, 
								std::list<CYDLinkRef*>* lstLinks = NULL);

	virtual HRESULT GetQuestionType(QTYPE* type);
	virtual HRESULT Remove();
	virtual HRESULT GetAnswerList(std::list<CString>* lst);

	virtual HRESULT SetFtp(CFtpRef* ftp);

protected:
	virtual HRESULT CreateFactorInfoItem();
public:
	//题型的难度系数，以后从XML获取
	static bool GetAllHardLevel(std::list<CString>* pList);
	static HARDLEVEL TranslateHardLevel(const CString& str);

public:
	CFtpRef*		m_pFtp;
};
