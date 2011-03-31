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
	HRESULT SetObjWrapper(CYdObjWrapper* _pObjWrapper);//更新时会调用
	HRESULT SetYdQValut(CYDQuestionVault* _pVault);//新建时会调用
	HRESULT SetYdQType(CYDQuestionType* _pQType);//新建时会调用
	HRESULT SetOpType(OPERATION _uType = OP_NEW);//0:新增 1:修改 2:查看

protected:
	HRESULT CreateQTypeID(VARIANT* _valTypeID);//得到题型ID
	HRESULT GetQuestionType(QTYPE* pType);
	std::list<CYdKnowledge*>& GetAllKnowledgPoint();
	std::list<CYdObjWrapper*>& GetQuestionRelatedKnowledgePoint();

protected:
	CListCtrlOperate* m_pListOperate;
	int				  m_index;//在更新时有效，指的是更新的Item
	CYdObjWrapper*    m_pObjWrapper;
	CYDQuestionVault* m_pQVault;//新建时用到，用来指定是在哪个题库下
	CYDQuestionType*  m_pQType;//新建时用到，用来指定是什么类型的试题
	OPERATION		  m_uType ;
	std::list<CYdKnowledge*>	m_allKPs;
	std::list<CYdObjWrapper*>	m_relatedKPs;
public:
	BOOL			  m_bIsInPaperGenerate;//是否是在生成试卷时，通过新建替换调用
	CYdObjWrapper*    m_pInPaperGenrateObjWrapper;//当是在在生成试卷时，通过新建替换调用时
												  //这个作为新生成的返回给外面
};
