/*
* Copyright (c) 2010, 南京永东
* All rights reserved.
*
* 文件名称：YDObjectRef.h
* 摘 要：试题库类
*
* 当前版本：1.0
* 作 者：lkf
* 完成日期：2010年4月29日
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
	//sub_knowledge用户要自己释放
	//得到题库的知识点
	HRESULT GetAllKnowledgePoint(std::list<CYdKnowledge*>* sub_knowledge,
		std::list<CYDLinkRef*>* _sub_link = NULL);
	//得到题库的试题类型
	HRESULT GetAllQuestionType(std::list<CYDQuestionType*>* _sub_qtype,
							std::list<CYDLinkRef*>* _sub_link = NULL);
	//得到题库下面的所有试题
	HRESULT GetAllQuestion(std::list<CYDQuestionRef*>* _sub_question,
							std::list<CYDLinkRef*>* _sub_link = NULL);
	//_idQuestionType = 试题类型的ID
	HRESULT GetQuestionByTypeID(UINT _idQuestionType,
								std::list<CYDQuestionRef*>* _sub_question,
								std::list<CYDLinkRef*>* _sub_link = NULL);
	//_idQuestionType = 试题类型的ID
	//_lstCondition查询的条件
	//分页查询，_iPage = -1时：不分页查询，显示所有
	//_iPage != -1时，进行分页查询，当前的页数，每页最多30条记录
	HRESULT GetQuestionByTypeIDCondition(UINT _idQuestionType,
										 int _iPage,
										std::list<CPropQueryContidition*> *_lstCondition,
										std::list<CYDQuestionRef*>* _sub_question,
										std::list<CYDLinkRef*>* _sub_link = NULL);
	////_idQuestionType = 试题类型的ID
	//_lstCondition查询的条件
	//_lCount返回的记录数
	HRESULT GetQuestionByTypeIDConditionCount(UINT _idQuestionType,
											std::list<CPropQueryContidition*> *_lstCondition,
											long* _lCount);
	//当前题库下查询到的题目是否为最后一个
	HRESULT CurQuestionIsEof(BOOL &_bIsEof);
private:
	//_idQuestionType是试题类型的OBJID,_idType是返回值，0：代表是选题等 
	//1代表是阅读题
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
	//根据m_pDB创建试题，_qType = QUESTION_CHOICE：choiceQuestion QUESTION_ARTICLE:ArticleQuestion
	HRESULT ExeDBCreateQuestion(QUESTION_TYPE _qType,
								std::list<CYDQuestionRef*>* _sub_question,
								std::list<CYDLinkRef*>* _sub_link = NULL	);
	//根据条件生成SQL语句
	HRESULT CreateConditionSQL(CString _strDBName,
		std::list<CPropQueryContidition*> *_lstCondition,
		CString &_strSQL);
private:
	CDatabaseEx*						m_pQueryQuestionDb; //当前题库中查询到的题目的记录集,因为要做分页查询，必须用一个新的数据库连接
};
