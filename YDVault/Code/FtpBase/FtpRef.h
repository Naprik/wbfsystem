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
	//��_strLocalFile�ϴ���_strFtpFile
	HRESULT Upload(CString _strLocalFile,CString _strFtpFile);
	//��_strFtpFile���ص�����_strLocalFile
	HRESULT DownLoad(CString _strLocalFile,CString _strFtpFile, BOOL bNoMsg=FALSE);
	HRESULT Remove(CString _strFtpFile);
	HRESULT CreateFolder(CString _strFtpFolder);
	HRESULT RemoveFolder(CString _strFtpFolder);
	HRESULT TestConnect(CFtpConfig* _pFtpConfig);

private:
	CFtpConfig	m_FtpConfig;
};
