#pragma once
#include "inputquestionhelper.h"

//导入选择题
class CVocabularyInputQuestionHelper :
	public CInputQuestionHelper
{
public:
	CVocabularyInputQuestionHelper(void);
	~CVocabularyInputQuestionHelper(void);
public:
	virtual HRESULT ExeInputFile(CString _strFile);
};

