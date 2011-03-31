// XmlException.h: interface for the CXmlException class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class BASE_DLL_API CXmlException : public CException  
{
public:
	CXmlException();
	virtual ~CXmlException();

private:
	CString m_strMsg1 , m_strMsg2 ;
public:
	long m_lErrorCode;
public:
	void	SetAdditionalMsg(const CString& strMsg1,const CString& strMsg2 = L"",
		long lErrorCode=0) ;
	void	ReportError(UINT nType = MB_OK|MB_ICONINFORMATION ) ;
};

