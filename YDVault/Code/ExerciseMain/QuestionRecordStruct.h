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
	UINT	m_QNo;//���
	UINT	m_QTypeID;//���͵�OBJID
	UINT	m_uObjID;//OBJID
	int		m_answercount;
	int		m_yescount;
	float   m_fMark;//����
	std::list<CString>	m_listUserAnswers;//by lkf
	CString m_strStdAnswer;//��׼��
	CString m_strDesc;//����
	
	CYDQuestionRef*	m_pQuestion;//by lkf

	BOOL	m_bSaved;//�������˲���
	HRESULT Load(CXmlNode& _node);
	HRESULT Save(CXmlWriter& _writer);
	HRESULT ComputeMark(double* _dbMark);//���������Ŀ�ĳɼ�

private:
	HRESULT LoadUserAnswer(CXmlNode& _node);
	HRESULT SaveUserAnswer(CXmlWriter& _writer);
};
