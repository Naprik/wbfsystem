#pragma once
#include "inputquestionhelper.h"

//����ѡ����
class CVocabularyInputQuestionHelper :
	public CInputQuestionHelper
{
public:
	CVocabularyInputQuestionHelper(void);
	~CVocabularyInputQuestionHelper(void);
public:
	virtual HRESULT ExeInputFile(CString _strFile);
};

