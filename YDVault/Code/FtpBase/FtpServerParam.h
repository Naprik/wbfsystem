#pragma once

class CXmlNode;
class CXmlWriter;
class BASE_DLL_API CFtpServerParam
{
public:
	CFtpServerParam(void);
	~CFtpServerParam(void);
	HRESULT Read(CXmlNode &_node);
	HRESULT Write(CXmlWriter &_writer);
public:
	CString      m_strFtpAddr;
	UINT		 m_uPort;
	CString      m_strUser;
	CString      m_strPwd;
};
