/*
* Copyright (c) 2010, �Ͼ�����
* All rights reserved.
*
* �ļ����ƣ�YDQuestionVault.h
* ժ Ҫ�������
*
* ��ǰ�汾��1.0
* �� �ߣ�wym
* ������ڣ�2010��4��20��
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
	//sub_qvc�û�Ҫ�Լ��ͷ�
	HRESULT GetChildQVClassification(std::list<CYDQuestionVaultClassification*>* sub_qvc,
									 std::list<CYDLinkRef*>* _sub_link = NULL);

	//��ȡ�����µ������
	HRESULT GetAllQVault(std::list<CYDQuestionVault*>* lstVault,
		std::list<CYDLinkRef*>* _sub_link = NULL);
};
