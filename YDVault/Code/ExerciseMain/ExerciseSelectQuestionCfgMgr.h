#pragma once

#include "SelectQuestionCfgMgr.h"

class CExerciseSelectQuestionCfgMgr
{
public:
	CExerciseSelectQuestionCfgMgr(void);
	~CExerciseSelectQuestionCfgMgr(void);
	static CSelectQuestionCfgMgr* Instance(){return &s_Instance;}
private:
	static CSelectQuestionCfgMgr	s_Instance;
};
