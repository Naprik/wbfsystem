#pragma once
#include <list>
class CQuestionRecordStruct;
class CYDQuestionRef;
//新建练习时，先CreateLog()生成 m_strfilename，再AddRecord(),最后Save();
//加载练习时，Load(),开放m_lstQuestionStruct
class CQuestionRecord
{
public:
	CQuestionRecord(CString _strFileName);
	~CQuestionRecord(void);
public:
	virtual HRESULT Load(){return S_OK;}
	virtual HRESULT Save(){return S_OK;}
	//创建文件名workspace里的目录ExamRecord201012092034.xml或者ExerciseRecord201012092034.xml
	virtual HRESULT CreateLog(){return S_OK;}
	//增加一条记录
	virtual HRESULT AddRecord(UINT _uQNo, CYDQuestionRef* pQestion, CQuestionRecordStruct** pNewRecordStruct);
	//计算成绩
	virtual HRESULT ComputeMark(double* _dbMark, int* _accuracy = NULL);
	//bRenameFile:是否要修改文件名
	virtual HRESULT SetFileName(CString& _strFileName,BOOL bRenameFile = FALSE);
	virtual HRESULT GetFileName(CString& _strFileName);
protected:
	CString m_strFileName;
	int		m_iCurNo;
	virtual HRESULT GetFilePath(CString &_strPath);
	HRESULT Clear();
public:
	std::list<CQuestionRecordStruct*> m_lstQuestionStruct;
};
