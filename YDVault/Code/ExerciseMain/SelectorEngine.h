#pragma once

#include "../ObjRef/YDQuestionRef.h"
#include "SelectQuestionCfgMgr.h"
#include "QuestionCfgStruct.h"

typedef std::list<CYDQuestionRef*> CLISTQUESTION;
class CSelectorEngine
{
public:
	CSelectorEngine(void);
	~CSelectorEngine(void);

	HRESULT GetQuestionsFromCfg(CSelectQuestionCfgMgr* pCfgMgr,
							CLISTQUESTION* pLstQuestion);

private:
	HRESULT GetQuestionByCfgItemFromAccess(OBJID vaultID, OBJID typeID, 
		CYDQuestionRef* pQTmp//,
		//CQuestionLevelNum* pItem
		);
};
