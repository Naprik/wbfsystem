#pragma once

class CInitInfoXml
{
public:
	CInitInfoXml(void);
	~CInitInfoXml(void);
public:
	HRESULT LoadInitXmlInfo(CString &_strXmlFile);
	HRESULT SaveInitXmlInfo(CString &_strXmlFile);
private:
	HRESULT GetInitXmlFile(CString &_strFile);
};
