#pragma once
#include <list>
class CDBUpdateFromXml
{
public:
	CDBUpdateFromXml(void);
	~CDBUpdateFromXml(void);
public:
	//ǰ����ַ���������������ľ����������SQL
	std::list<std::pair<CString,CString> > m_lstUpdateSQL;
	virtual HRESULT Load();
private:
	HRESULT GetXml(CString &_strXml);
};

