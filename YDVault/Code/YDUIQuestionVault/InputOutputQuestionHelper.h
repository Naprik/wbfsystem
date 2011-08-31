#pragma once
#include "../wordbase/msword.h"
#include <list>

class CYDObjectRef;

class CInputOutputQuestionHelper
{
public:
	CInputOutputQuestionHelper(CYDObjectRef* _pVault,CYDObjectRef* _pType);
	~CInputOutputQuestionHelper(void);
public:
	virtual HRESULT ExeInputFile(CString _strFile) = 0;
	virtual HRESULT ExeOutputFile(CString _strFile,std::list<CYDObjectRef*> *_plstObj) = 0;
protected:
	CYDObjectRef*   m_pVault;
	CYDObjectRef*	 m_pType;
	_Application m_oWordApp ; 
	Selection m_oSel ; 
	Documents m_oDocs ; 
	_Document m_oDoc ; 
	Paragraphs m_paragraphs ;
	BOOL	m_bCreateWordSuc;
	HRESULT CreateWord(CString _strFile);
	HRESULT CreateBlankWord();
	HRESULT SaveWord(CString _strFile);
	//判断_strName是否为当前题型的指标名
	HRESULT IsFactorName(CString _strName,VARIANT_BOOL* _bIs);
private:
	std::list<CYDObjectRef*>	m_lstFactorInfo;//当前题型具有的指标名“预留字段”与指标的映射关系对象列表
};

