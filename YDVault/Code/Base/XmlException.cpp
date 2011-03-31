// XmlException.cpp: implementation of the CXmlException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XmlException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXmlException::CXmlException()
{
	m_lErrorCode=0;
}

CXmlException::~CXmlException()
{

}

void CXmlException::SetAdditionalMsg(const CString& strMsg1,const CString& strMsg2,
									 long lErrorCode) 
{
	m_strMsg1 = strMsg1 ; 
	m_strMsg2 = strMsg2 ;
	m_lErrorCode = lErrorCode;
}

void CXmlException::ReportError(UINT nType )
{
	AfxMessageBox(m_strMsg1 + _T("\r\n") + m_strMsg2 , nType );
	return ;
}
