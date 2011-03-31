/*
* Copyright (c) 2010, 南京永东
* All rights reserved.
*
* 文件名称：YDQuestionVault.h
* 摘 要：试题库
*
* 当前版本：1.0
* 作 者：wym
* 完成日期：2010年4月20日
*/
#pragma once

#include "ydobjectref.h"
#include "YDQuestionVault.h"
#include <list>
class CYDLinkRef;

class BASE_DLL_API CYDQuestionVaultClassification : public CYDObjectRef
{
public:
	CYDQuestionVaultClassification(CDatabaseEx* pDB);
	~CYDQuestionVaultClassification(void);
	virtual HRESULT Remove();
public:
	//sub_qvc用户要自己释放
	HRESULT GetChildQVClassification(std::list<CYDQuestionVaultClassification*>* sub_qvc,
									 std::list<CYDLinkRef*>* _sub_link = NULL);

	//获取大类下的试题库
	HRESULT GetAllQVault(std::list<CYDQuestionVault*>* lstVault,
		std::list<CYDLinkRef*>* _sub_link = NULL);
};
