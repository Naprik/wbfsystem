#include "StdAfx.h"
#include "YDAnswer.h"

CYDAnswer::CYDAnswer(CDatabaseEx* pDb) : CYDObjectRef(pDb)
{
	m_strDBName = DB_ANSWER;
	AddPropDef(FIELD_YDANSWER_NO,VT_I4);
	AddPropDef(FIELD_YDANSWER_ANSWER,VT_BSTR);	
	AddPropDef(FIELD_YDANSWER_MARK,VT_R4, L"1");	
}

CYDAnswer::~CYDAnswer(void)
{
}
