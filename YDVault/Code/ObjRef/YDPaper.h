/*
* Copyright (c) 2010, �Ͼ�����
* All rights reserved.
*
* �ļ����ƣ�YDPaper.h
* ժ Ҫ���Ծ���
*
* ��ǰ�汾��1.0
* �� �ߣ�lkf
* ������ڣ�2010��5��1��
*/

#pragma once
#include "ydobjectref.h"
#include "YDQuestionRef.h"
#include "../WordBase/WordAppHelper.h"
#include "PaperTemplate.h"
#include "../FtpBase/FtpRef.h"
#include "../UIBase/PdemWait.h"
#include <list>

//��������
class BASE_DLL_API CYDPaperBlockConfigureItem
{
public:
	CYDPaperBlockConfigureItem(UINT iLevel, UINT iRelNum, UINT iCount, 
		const std::list<UINT>& lst);

public:
	UINT		m_iLevel;//�Ѷ�ϵ��
	UINT		m_iCount;//����Ŀ
	UINT		m_iRelNum;//���������Ŀ��
	double		m_fMark;//����-wym�޸ģ�ֻ���ڳ���ϵͳ��

	std::list<UINT>		m_lstKP;//����֪ʶ��
};

//�����ͽ��Ծ�ֿ�
class BASE_DLL_API CYDPaperBlockConfigure
{
public:
	CYDPaperBlockConfigure();
	CYDPaperBlockConfigure(const CYDPaperBlockConfigure& cfg);
	~CYDPaperBlockConfigure();

public:
	bool SetQType(OBJID qtype);//��������
	bool AddConfigureItem(UINT iLevel, UINT iRelCount, UINT iCount, const std::list<UINT>& lstKP);//��������
	bool UpdateConfigureItem(UINT iLevel, UINT iRelCount, UINT iCount,double fMark = 0.0);
	
public:
	OBJID									m_QType;//����
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
	
	//�õ���ǰ�Ծ�ķ�ֵ��һ���ܷ�Ϊ100��
	HRESULT GetPaperMarkVal(long& _lVal);
	//_lstAns ��װ�������
	HRESULT GetStdAnswer(std::list<CYDObjectRef*>* _lstAns, std::list<CYDLinkRef*>* _links = NULL);
	HRESULT GetStdAnswer(CString &_strAns);
	//�õ���ǰ�Ծ��_lNo��ı�׼��
	HRESULT GetStdAsnwerByItemNo(long _lNo,CString& _strAnswer);
	
	bool SetQVault(OBJID id);//�����Ծ��ȡ��Ŀ�����

	HRESULT CreateVaultName(CString& _strPaper, CString& _strPaperAnswer);

	HRESULT SetTemplate(const CString& strName);
	HRESULT SetFtpRef(CFtpRef* ftp);

	bool AddPaperBlockConfigure(CYDPaperBlockConfigure* pCfg);
	HRESULT MakePaper(const CString& str);//�����Ծ�

	HRESULT PersistToMSWord(CWordAppHelper* pWord, CWordAppHelper* pWordAnswer, const CString& strFolder,
		CPdemWait* pWait);
	//�滻�Ծ��е�����
	HRESULT ReplaceQuestion(const CString& strQType, CYDQuestionRef* pQTarget, CYDQuestionRef* pQRef);

	HRESULT ReplaceByLeastUsedCountQuestion(const CString& strQType, CYDQuestionRef* pTargetQuestion, 
		CYDQuestionRef** pQuestion);

private:
	HRESULT GetQuestionsFromCfg(CYDPaperBlockConfigure* pCfg, 
		std::list<CYDQuestionRef*>* pLstQuestion);

	HRESULT GetQuestionByCfgItemFromOracle(OBJID typeID,CYDQuestionRef* pQTmp,CYDPaperBlockConfigureItem* pItem);
	HRESULT GetQuestionByCfgItemFromMSSQL(OBJID typeID,CYDQuestionRef* pQTmp, CYDPaperBlockConfigureItem* pItem);
	HRESULT GetQuestionByCfgItemFromAccess(OBJID typeID,CYDQuestionRef* pQTmp, CYDPaperBlockConfigureItem* pItem);


	//��������ʹ�ô���
	HRESULT UpdateUsedCount(std::list<CYDQuestionRef*>* pLst);
	
	//�����Ƿ��Ѿ������Ծ���
	BOOL IsExist(const CString& strQType, OBJID id);
	//��ȡ��һ�����ظ������ID
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
