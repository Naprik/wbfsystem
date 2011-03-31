#pragma once
#include "../Include/YdConfigCommon.h"
#include "FtpServerParam.h"
#include "../Include/YdConfigCommon.h"

class BASE_DLL_API CFtpConfig
{
public:
	CFtpConfig(void);
	~CFtpConfig(void);
public:
	HRESULT Read();
	HRESULT Write();
	CONFIG_SEVER_MODE   m_serverMode;
	CFtpServerParam		m_FtpServerParam;
private:
	HRESULT GetXml(CString &_strXml);
};
