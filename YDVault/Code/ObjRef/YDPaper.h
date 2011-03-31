/*
* Copyright (c) 2010, 南京永东
* All rights reserved.
*
* 文件名称：YDPaper.h
* 摘 要：试卷类
*
* 当前版本：1.0
* 作 者：lkf
* 完成日期：2010年5月1日
*/

#pragma once
#include "ydobjectref.h"
#include "YDQuestionRef.h"
#include "../WordBase/WordAppHelper.h"
#include "PaperTemplate.h"
#include "../FtpBase/FtpRef.h"
#include "../UIBase/PdemWait.h"
#include <list>

//出题配置
class BASE_DLL_API CYDPaperBlockConfigureItem
{
public:
	CYDPaperBlockConfigureItem(UINT iLevel, UINT iRelNum, UINT iCount, 
		const std::list<UINT>& lst);

public:
	UINT		m_iLevel;//难度系数
	UINT		m_iCount;//题数目
	UINT		m_iRelNum;//问题管理题目数
	double		m_fMark;//分数-wym修改，只用在出题系统中

	std::list<UINT>		m_lstKP;//包含知识点
};

//按题型将试卷分块
class BASE_DLL_API CYDPaperBlockConfigure
{
public:
	CYDPaperBlockConfigure();
	CYDPaperBlockConfigure(const CYDPaperBlockConfigure& cfg);
	~CYDPaperBlockConfigure();

public:
	bool SetQType(OBJID qtype);//设置题型
	bool AddConfigureItem(UINT iLevel, UINT iRelCount, UINT iCount, const std::list<UINT>& lstKP);//新增配置
	bool UpdateConfigureItem(UINT iLevel, UINT iRelCount, UINT iCount,double fMark = 0.0);
	
public:
	OBJID									m_QType;//题型
	std::list<CYDPaperBlockConfigureItem*>	m_lstConfigureItem;
};

class BASE_DLL_API CYDPaper :
	public CYDObjectRef
{
public:
	CYDPaper(CDatabaseEx* pDB);
	~CYDPaper(void);

public:
	HRESULT Remove();
	
	//得到当前试卷的分值，一般总分为100分
	HRESULT GetPaperMarkVal(long& _lVal);
	//_lstAns 安装题号排序
	HRESULT GetStdAnswer(std::list<CYDObjectRef*>* _lstAns, std::list<CYDLinkRef*>* _links = NULL);
	HRESULT GetStdAnswer(CString &_strAns);
	//得到当前试卷第_lNo题的标准答案
	HRESULT GetStdAsnwerByItemNo(long _lNo,CString& _strAnswer);
	
	bool SetQVault(OBJID id);//设置试卷抽取题目的题库

	HRESULT CreateVaultName(CString& _strPaper, CString& _strPaperAnswer);

	HRESULT SetTemplate(const CString& strName);
	HRESULT SetFtpRef(CFtpRef* ftp);

	bool AddPaperBlockConfigure(CYDPaperBlockConfigure* pCfg);
	HRESULT MakePaper(const CString& str);//生成试卷

	HRESULT PersistToMSWord(CWordAppHelper* pWord, CWordAppHelper* pWordAnswer, const CString& strFolder,
		CPdemWait* pWait);
	//替换试卷中的试题
	HRESULT ReplaceQuestion(const CString& strQType, CYDQuestionRef* pQTarget, CYDQuestionRef* pQRef);

	HRESULT ReplaceByLeastUsedCountQuestion(const CString& strQType, CYDQuestionRef* pTargetQuestion, 
		CYDQuestionRef** pQuestion);

private:
	HRESULT GetQuestionsFromCfg(CYDPaperBlockConfigure* pCfg, 
		std::list<CYDQuestionRef*>* pLstQuestion);

	HRESULT GetQuestionByCfgItemFromOracle(OBJID typeID,CYDQuestionRef* pQTmp,CYDPaperBlockConfigureItem* pItem);
	HRESULT GetQuestionByCfgItemFromMSSQL(OBJID typeID,CYDQuestionRef* pQTmp, CYDPaperBlockConfigureItem* pItem);
	HRESULT GetQuestionByCfgItemFromAccess(OBJID typeID,CYDQuestionRef* pQTmp, CYDPaperBlockConfigureItem* pItem);


	//更新试题使用次数
	HRESULT UpdateUsedCount(std::list<CYDQuestionRef*>* pLst);
	
	//试题是否已经存在试卷中
	BOOL IsExist(const CString& strQType, OBJID id);
	//获取下一个不重复试题的ID
	HRESULT GetNextId(const CString& strQType, CString strTable, CString strHardLevel, CString strTypeId, OBJID* pID);

	HRESULT ClearContent();


protected:
	CYDObjectRef* CreateObject();

public:
	std::list<CYDPaperBlockConfigure*>		m_lstBlock;
	typedef std::map<CString, std::list<CYDQuestionRef*>> MapContent;
	MapContent								m_mapPaperContent;
	OBJID									m_IDVault;

	CPaperTemplate*							m_pTemplate;
	CFtpRef*								m_pFtp;

	BOOL									m_bSaved;
};
