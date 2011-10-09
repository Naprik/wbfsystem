#pragma once
#include "questionrecord.h"

class CExerciseQuestionRecord :
	public CQuestionRecord
{
public:
	CExerciseQuestionRecord(CString _strFileName);
	CExerciseQuestionRecord();
	~CExerciseQuestionRecord(void);
public:
	virtual HRESULT Load();
	virtual HRESULT Save();
	virtual HRESULT CreateLog();
	virtual HRESULT UpdateLogName(CString& _strFileName);

protected:
	virtual HRESULT GetFilePath(CString &_strPath);
};
