#pragma once
#include "../Include/YDVaultComm.h"
#include "FtpConfig.h"



class BASE_DLL_API CFtpRef
{
public:
	CFtpRef(void);
	~CFtpRef(void);
public:
	HRESULT Init();
	//将_strLocalFile上传到_strFtpFile
	HRESULT Upload(CString _strLocalFile,CString _strFtpFile);
	//将_strFtpFile下载到本地_strLocalFile
	HRESULT DownLoad(CString _strLocalFile,CString _strFtpFile, BOOL bNoMsg=FALSE);
	HRESULT Remove(CString _strFtpFile);
	HRESULT CreateFolder(CString _strFtpFolder);
	HRESULT RemoveFolder(CString _strFtpFolder);
	HRESULT TestConnect(CFtpConfig* _pFtpConfig);

private:
	CFtpConfig	m_FtpConfig;
};
