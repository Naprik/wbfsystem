#pragma once

class CXmlNode;
class CXmlWriter;
class CDBConfigConnetParam
{
public:
	CDBConfigConnetParam(void);
	~CDBConfigConnetParam(void);
	HRESULT Read(CXmlNode &_node);
	HRESULT Write(CXmlWriter &_writer);
public:
	CString m_strSeverName;
	CString m_strDBName;
	CString m_strUser;
	CString m_strPwd;
	CString m_strDBProvider;
	UINT    m_uConectTime;
	CString m_strDBLongConnection;
	CString m_strDBFileName;
};
