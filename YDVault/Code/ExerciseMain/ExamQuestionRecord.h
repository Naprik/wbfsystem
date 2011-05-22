#pragma once
#include "questionrecord.h"

class CExamQuestionRecord :
	public CQuestionRecord
{
public:
	CExamQuestionRecord(CString _strFileName);
	~CExamQuestionRecord(void);
public:
	virtual HRESULT Load();
	virtual HRESULT Save();
	virtual HRESULT CreateLog();
protected:
	virtual HRESULT GetFilePath(CString &_strPath);
};
