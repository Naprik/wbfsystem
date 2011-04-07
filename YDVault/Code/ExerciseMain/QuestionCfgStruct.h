#pragma once
#include <list>

class CXmlNode;
class CXmlWriter;

class CQuestionCfgStruct
{
public:
	CQuestionCfgStruct(void);
	~CQuestionCfgStruct(void);

	HRESULT GetQuestionType(CString* questiontype);
	HRESULT GetDescription(CString* description);
public:
	OBJID	m_vaultID;
	OBJID	m_QTypeID; //题型ID
	int		m_cNum;//题数
	double  m_dMark;//分数
	std::list<std::pair<CString,CString>>	m_lstFactors;

	HRESULT Load(CXmlNode& _node);
	HRESULT Save(CXmlWriter& _writer);
};
