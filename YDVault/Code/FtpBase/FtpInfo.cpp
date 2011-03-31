// FtpInfo.cpp: implementation of the CFtpInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FtpInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFtpInfo::CFtpInfo()
{
	m_strFtpConnect = _T("");
	m_strUser = _T("");
	m_strPwd = _T("");
	m_strLocalFile = _T("");
	m_strFtpFile = _T("");
	m_uPort = 21;
}

CFtpInfo::~CFtpInfo()
{

}
