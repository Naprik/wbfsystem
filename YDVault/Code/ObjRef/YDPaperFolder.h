/*
* Copyright (c) 2010, 南京永东
* All rights reserved.
*
* 文件名称：YDPaperFolder.h
* 摘 要：试卷夹类，试卷夹之间具有层次关系，试卷夹用于存放试卷
*
* 当前版本：1.0
* 作 者：lkf
* 完成日期：2010年5月1日
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
