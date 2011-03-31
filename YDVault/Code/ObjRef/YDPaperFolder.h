/*
* Copyright (c) 2010, �Ͼ�����
* All rights reserved.
*
* �ļ����ƣ�YDPaperFolder.h
* ժ Ҫ���Ծ���࣬�Ծ��֮����в�ι�ϵ���Ծ�����ڴ���Ծ�
*
* ��ǰ�汾��1.0
* �� �ߣ�lkf
* ������ڣ�2010��5��1��
*/

#pragma once
#include "ydobjectref.h"
#include <list>
#include "YDLinkRef.h"
#include "YDPaper.h"

class BASE_DLL_API CYDPaperFolder :	public CYDObjectRef
{
public:
	CYDPaperFolder(CDatabaseEx* pDb);
	~CYDPaperFolder(void);

public:
	HRESULT GetChildPaperFolder(std::list<CYDPaperFolder*>* sub_paperfolder,
		std::list<CYDLinkRef*>* _sub_link = NULL);

	HRESULT GetAllPapers(std::list<CYDPaper*>* _papers, 
		std::list<CYDLinkRef*>* _links);

	HRESULT Remove();
};
