#pragma once

#include "../ObjRef/YDQuestionRef.h"
#include "SelectQuestionCfgMgr.h"
#include "QuestionCfgStruct.h"
#include <map>
#include <list>

typedef std::list<CYDQuestionRef*> CLISTQUESTION;
class CSelectorEngine
{
public:
	CSelectorEngine(void);
	~CSelectorEngine(void);

	HRESULT GetQuestionsFromCfg(CSelectQuestionCfgMgr* pCfgMgr,
							CLISTQUESTION* pLstQuestion);

private:
	HRESULT GetQuestionByCfgItemFromAccess(CQuestionCfgStruct* pcfg, CYDQuestionRef* pQTmp);
	HRESULT GetQueryConditionFromFactor(CQuestionCfgStruct* pcfg, CString* condition);

	BOOL AddRecord(CString key, OBJID id);
	BOOL GetRecords(CString key, std::list<OBJID>& ids);

	std::map<CString, std::list<OBJID>> m_mapRecord;
};
