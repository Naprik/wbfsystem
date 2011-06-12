#pragma once
#include <list>
class CQuestionRecordStruct;
class CYDQuestionRef;
//�½���ϰʱ����CreateLog()���� m_strfilename����AddRecord(),���Save();
//������ϰʱ��Load(),����m_lstQuestionStruct
class CQuestionRecord
{
public:
	CQuestionRecord(CString _strFileName);
	~CQuestionRecord(void);
public:
	virtual HRESULT Load(){return S_OK;}
	virtual HRESULT Save(){return S_OK;}
	//�����ļ���workspace���Ŀ¼ExamRecord201012092034.xml����ExerciseRecord201012092034.xml
	virtual HRESULT CreateLog(){return S_OK;}
	//����һ����¼
	virtual HRESULT AddRecord(UINT _uQNo, CYDQuestionRef* pQestion, CQuestionRecordStruct** pNewRecordStruct);
	//����ɼ�
	virtual HRESULT ComputeMark(double* _dbMark, int* _accuracy = NULL);
	//bRenameFile:�Ƿ�Ҫ�޸��ļ���
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
