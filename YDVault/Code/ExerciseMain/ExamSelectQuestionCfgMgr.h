#pragma once

#include "SelectQuestionCfgMgr.h"

class CExamSelectQuestionCfgMgr
{
public:
	CExamSelectQuestionCfgMgr(void);
	~CExamSelectQuestionCfgMgr(void);

	static CSelectQuestionCfgMgr*	Instance(){return &s_Instance;}

private:
	static CSelectQuestionCfgMgr	s_Instance;
};
