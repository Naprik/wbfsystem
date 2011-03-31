#include "StdAfx.h"
#include "YDQuestionFactory.h"
#include "YDQuestionRef.h"
#include "YDQuestionType.h"
#include "../DBBase/DatabaseEx.h"
#include "../Base/DataHandler.h"
#include "YDChoiceQuestionRef.h"
#include "YDArticleQuestionRef.h"

CYDQuestionFactory::CYDQuestionFactory()
{
}

CYDQuestionFactory::~CYDQuestionFactory(void)
{

}

CYDQuestionRef* CYDQuestionFactory::CreateQuestion(CYDQuestionType* pType, CDatabaseEx* pDB)
{
	CComVariant varType;
	HRESULT hr = pType->GetPropVal(L"TYPE", &varType);
	if (FAILED(hr))
	{
		return NULL;
	}
	long lType = CDataHandler::VariantToLong(varType);
	if (lType == QTYPE_WRITING ||
		lType == QTYPE_VOCABULARY ||
		lType == QTYPE_TRANSLATION)
	{
		//—°‘ÒÃ‚
		return new CYDChoiceQuestionRef(pDB);
	}
	else if(
		lType == QTYPE_READINGCOMPREHENSION ||
		lType == QTYPE_LISTENINGCOMPREHENSION ||
		lType == QTYPE_CLOZE ||
		lType == QTYPE_LISTENINGVOCABULARY ||
		lType == QTYPE_SKIMMING ||
		lType == QTYPE_READINGVOCABULARY ||
		lType == QTYPE_LISTENINGLONG ||
		lType == QTYPE_LISTENINGSHORT ||
		lType == QTYPE_ERRORCORRECTION)
	{
		//‘ƒ∂¡Ã‚
		return new CYDArticleQuestionRef(pDB);
	}
	else 
	{
		ASSERT(FALSE);
	}

	return NULL;
}

