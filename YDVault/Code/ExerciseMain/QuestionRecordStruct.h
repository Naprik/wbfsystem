#pragma once

#include <list>
#include "../ObjRef/YDQuestionRef.h"
class CXmlNode;
class CXmlWriter;

class CQuestionRecordStruct
{
public:
	CQuestionRecordStruct(void);
	~CQuestionRecordStruct(void);
public:
	UINT	m_QNo;//题号
	UINT	m_QTypeID;//题型的OBJID
	UINT	m_uObjID;//OBJID
	int		m_answercount;
	int		m_yescount;
	float   m_fMark;//分数
	std::list<CString>	m_listUserAnswers;//by lkf
	CString m_strStdAnswer;//标准答案
	CString m_strDesc;//评论
	
	CYDQuestionRef*	m_pQuestion;//by lkf

	BOOL	m_bSaved;//用于载人测试
	HRESULT Load(CXmlNode& _node);
	HRESULT Save(CXmlWriter& _writer);
	HRESULT ComputeMark(double* _dbMark);//计算这个题目的成绩

private:
	HRESULT LoadUserAnswer(CXmlNode& _node);
	HRESULT SaveUserAnswer(CXmlWriter& _writer);
};
