#pragma once
#include "afxwin.h"
#include <list>
#include "../ObjRef/YdKnowledge.h"

class CListCtrlOperate;
class CYdObjWrapper;
class CYDQuestionVault;
class CYDQuestionType;

class BASE_DLL_API CYDQuestionDlg :
	public CDialog
{
public:
	CYDQuestionDlg(UINT nIDTemplate,CWnd* _pParent);
	~CYDQuestionDlg(void);

	HRESULT SetListOperate(CListCtrlOperate* _pListOperate,int _index = 0);
	HRESULT SetObjWrapper(CYdObjWrapper* _pObjWrapper);//����ʱ�����
	HRESULT SetYdQValut(CYDQuestionVault* _pVault);//�½�ʱ�����
	HRESULT SetYdQType(CYDQuestionType* _pQType);//�½�ʱ�����
	HRESULT SetOpType(OPERATION _uType = OP_NEW);//0:���� 1:�޸� 2:�鿴

protected:
	HRESULT CreateQTypeID(VARIANT* _valTypeID);//�õ�����ID
	HRESULT GetQuestionType(QTYPE* pType);
	std::list<CYdKnowledge*>& GetAllKnowledgPoint();
	std::list<CYdObjWrapper*>& GetQuestionRelatedKnowledgePoint();

protected:
	CListCtrlOperate* m_pListOperate;
	int				  m_index;//�ڸ���ʱ��Ч��ָ���Ǹ��µ�Item
	CYdObjWrapper*    m_pObjWrapper;
	CYDQuestionVault* m_pQVault;//�½�ʱ�õ�������ָ�������ĸ������
	CYDQuestionType*  m_pQType;//�½�ʱ�õ�������ָ����ʲô���͵�����
	OPERATION		  m_uType ;
	std::list<CYdKnowledge*>	m_allKPs;
	std::list<CYdObjWrapper*>	m_relatedKPs;
public:
	BOOL			  m_bIsInPaperGenerate;//�Ƿ����������Ծ�ʱ��ͨ���½��滻����
	CYdObjWrapper*    m_pInPaperGenrateObjWrapper;//�������������Ծ�ʱ��ͨ���½��滻����ʱ
												  //�����Ϊ�����ɵķ��ظ�����
};
