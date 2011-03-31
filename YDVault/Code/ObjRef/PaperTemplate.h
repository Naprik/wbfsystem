#pragma once

#include <list>

struct CPaperTemplateItem 
{
	OBJID		id;
	CString		strDirection;
};
class BASE_DLL_API CPaperTemplate
{
public:
	CPaperTemplate();
	~CPaperTemplate(void);

public:
	HRESULT Load(const CString& strTempName);

public:
	std::list<CPaperTemplateItem*>	m_lstItem;

private:
	CString		m_strTempName;
};
