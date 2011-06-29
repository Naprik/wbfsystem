#pragma once
#include <list>
class CDBUpdateFromXml
{
public:
	CDBUpdateFromXml(void);
	~CDBUpdateFromXml(void);
public:
	//前面的字符串是描述，后面的具体的升级的SQL
	std::list<std::pair<CString,CString> > m_lstUpdateSQL;
	virtual HRESULT Load();
private:
	HRESULT GetXml(CString &_strXml);
};

