#pragma once
#include <list>

class CXmlNode;
class CXmlWriter;
class CFactorValue
{
public:
	CFactorValue(const CString& field, const CString& name, const CString& value, const CString& range):
	  m_field(field), m_name(name),m_value(value), m_range(range)
	  {

	  }
	CString		m_field;
	CString		m_value;
	CString		m_name;
	CString		m_range;
};

class CQuestionCfgStruct
{
public:
	CQuestionCfgStruct(void);
	~CQuestionCfgStruct(void);

	HRESULT GetQuestionType(CString* questiontype);
	HRESULT GetDescription(CString* description);
public:
	OBJID	m_vaultID;
	OBJID	m_QTypeID; //����ID
	int		m_cNum;//����
	double  m_dMark;//����
	std::list<CFactorValue>	m_lstFactors;

	HRESULT Load(CXmlNode& _node);
	HRESULT Save(CXmlWriter& _writer);
};
