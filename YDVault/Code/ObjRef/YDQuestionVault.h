/*
* Copyright (c) 2010, �Ͼ�����
* All rights reserved.
*
* �ļ����ƣ�YDObjectRef.h
* ժ Ҫ���������
*
* ��ǰ�汾��1.0
* �� �ߣ�lkf
* ������ڣ�2010��4��29��
*/

#pragma once
#include "ydobjectref.h"

class CYdKnowledge;
class CYDLinkRef;
class CYDQuestionType;
class CYDQuestionRef;
class CPropQueryContidition;

class CYDID
{
public:
	UINT m_objID,m_IDB,m_IDType;
	CYDID()
	{
		m_objID =0;
		m_IDB = 0;
		m_IDType =0;
	}
};

class  BASE_DLL_API CYDQuestionVault :
	public CYDObjectRef
{
public:
	CYDQuestionVault(CDatabaseEx* pDB);
	~CYDQuestionVault(void);
public:
	//sub_knowledge�û�Ҫ�Լ��ͷ�
	//�õ�����֪ʶ��
	HRESULT GetAllKnowledgePoint(std::list<CYdKnowledge*>* sub_knowledge,
		std::list<CYDLinkRef*>* _sub_link = NULL);
	//�õ�������������
	HRESULT GetAllQuestionType(std::list<CYDQuestionType*>* _sub_qtype,
							std::list<CYDLinkRef*>* _sub_link = NULL);
	//�õ�����������������
	HRESULT GetAllQuestion(std::list<CYDQuestionRef*>* _sub_question,
							std::list<CYDLinkRef*>* _sub_link = NULL);
	//_idQuestionType = �������͵�ID
	HRESULT GetQuestionByTypeID(UINT _idQuestionType,
								std::list<CYDQuestionRef*>* _sub_question,
								std::list<CYDLinkRef*>* _sub_link = NULL);
	//_idQuestionType = �������͵�ID
	//_lstCondition��ѯ������
	//��ҳ��ѯ��_iPage = -1ʱ������ҳ��ѯ����ʾ����
	//_iPage != -1ʱ�����з�ҳ��ѯ����ǰ��ҳ����ÿҳ���30����¼
	HRESULT GetQuestionByTypeIDCondition(UINT _idQuestionType,
										 int _iPage,
										std::list<CPropQueryContidition*> *_lstCondition,
										std::list<CYDQuestionRef*>* _sub_question,
										std::list<CYDLinkRef*>* _sub_link = NULL);
	////_idQuestionType = �������͵�ID
	//_lstCondition��ѯ������
	//_lCount���صļ�¼��
	HRESULT GetQuestionByTypeIDConditionCount(UINT _idQuestionType,
											std::list<CPropQueryContidition*> *_lstCondition,
											long* _lCount);
	//��ǰ����²�ѯ������Ŀ�Ƿ�Ϊ���һ��
	HRESULT CurQuestionIsEof(BOOL &_bIsEof);
private:
	//_idQuestionType���������͵�OBJID,_idType�Ƿ���ֵ��0��������ѡ��� 
	//1�������Ķ���
	HRESULT GetQuestionTypeIDDB(UINT _idQuestionType,
								QUESTION_TYPE& _qType);
	HRESULT GetChoiceQuestionByTypeIDCondition(UINT _idQuestionType,
		int _iPage,
		std::list<CPropQueryContidition*> *_lstCondition,
		std::list<CYDQuestionRef*>* _sub_question,
		std::list<CYDLinkRef*>* _sub_link = NULL);
	HRESULT GetArticleQuestionByTypeIDCondition(UINT _idQuestionType,
		int _iPage,
		std::list<CPropQueryContidition*> *_lstCondition,
		std::list<CYDQuestionRef*>* _sub_question,
		std::list<CYDLinkRef*>* _sub_link = NULL);
	HRESULT GetQuestionCount(
		UINT _idQuestionType,
		CString  _strDBName,
		std::list<CPropQueryContidition*> *_lstCondition,
		long* _lCount);
	HRESULT CreateParamter(CDatabaseEx* _pDB,
							UINT _idQuestionType,
							std::list<CPropQueryContidition*> *_lstCondition);
	HRESULT ExeConditionDB(UINT _idQuestionType,
							CString _strDBName,
						   std::list<CPropQueryContidition*> *_lstCondition);
	//����m_pDB�������⣬_qType = QUESTION_CHOICE��choiceQuestion QUESTION_ARTICLE:ArticleQuestion
	HRESULT ExeDBCreateQuestion(QUESTION_TYPE _qType,
								std::list<CYDQuestionRef*>* _sub_question,
								std::list<CYDLinkRef*>* _sub_link = NULL	);
	//������������SQL���
	HRESULT CreateConditionSQL(CString _strDBName,
		std::list<CPropQueryContidition*> *_lstCondition,
		CString &_strSQL);
private:
	CDatabaseEx*						m_pQueryQuestionDb; //��ǰ����в�ѯ������Ŀ�ļ�¼��,��ΪҪ����ҳ��ѯ��������һ���µ����ݿ�����
};
