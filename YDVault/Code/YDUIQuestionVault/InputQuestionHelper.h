#pragma once
#include "../wordbase/msword.h"
#include <list>

class CYDObjectRef;

class CInputQuestionHelper
{
public:
	CInputQuestionHelper(OBJID _IDVault,OBJID _IDType);
	~CInputQuestionHelper(void);
public:
	virtual HRESULT ExeInputFile(CString _strFile) = 0;
protected:
	OBJID   m_IDVault;
	OBJID	m_IDType;
	_Application m_oWordApp ; 
	Selection m_oSel ; 
	Documents m_oDocs ; 
	_Document m_oDoc ; 
	Paragraphs m_paragraphs ;
	BOOL	m_bCreateWordSuc;
	HRESULT CreateWord(CString _strFile);
	//�ж�_strName�Ƿ�Ϊ��ǰ���͵�ָ����
	HRESULT IsFactorName(CString _strName,VARIANT_BOOL* _bIs);
private:
	std::list<CYDObjectRef*>	m_lstFactorInfo;//��ǰ���;��е�ָ������Ԥ���ֶΡ���ָ���ӳ���ϵ�����б�
};

