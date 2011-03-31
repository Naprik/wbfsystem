#include "StdAfx.h"
#include "FtpRef.h"
#include "afxinet.h"
#include "../Base/AutoClean.h"
#include "../DBBase/DatabaseEx.h"
#include "../Base/DataHandler.h"
#include "FtpInfo.h"
#include "FtpUpload.h"
#include "FTPDownLoad.h"
#include <Shlwapi.h>
#include "../Include/YDVaultError.h"
#include "../Base\FilePathHelper.h"

CFtpRef::CFtpRef(void)
{
}

CFtpRef::~CFtpRef(void)
{
}



HRESULT CFtpRef::Init()
{
	HRESULT hr = E_FAIL;
	hr = m_FtpConfig.Read();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CFtpRef::Upload(CString _strLocalFile,CString _strFtpFile)
{
	HRESULT hr = E_FAIL;
	//上传
	if(m_FtpConfig.m_serverMode == SERVER_FTP)
	{
		CFtpInfo FtpInfo;
		FtpInfo.m_strFtpConnect = m_FtpConfig.m_FtpServerParam.m_strFtpAddr;
		FtpInfo.m_strUser = m_FtpConfig.m_FtpServerParam.m_strUser;
		FtpInfo.m_strPwd = m_FtpConfig.m_FtpServerParam.m_strPwd;
		FtpInfo.m_uPort = m_FtpConfig.m_FtpServerParam.m_uPort;
		FtpInfo.m_strLocalFile = _strLocalFile;
		FtpInfo.m_strFtpFile = _strFtpFile;

		CFtpUpload up(&FtpInfo);
		hr = up.Upload();
		if(FAILED(hr))
		{
			return hr;
		}
#ifdef DEBUG
		AfxMessageBox(up.m_result_str);
#endif
	}
	else if(m_FtpConfig.m_serverMode == SERVER_LOCAL)
	{
		CString ftpFile = m_FtpConfig.m_FtpServerParam.m_strFtpAddr + _strFtpFile;
		CopyFile(_strLocalFile,ftpFile,FALSE);
	}
	else
	{
		ASSERT(FALSE);
	}
	return S_OK;
}

HRESULT CFtpRef::DownLoad(CString _strLocalFile,CString _strFtpFile, BOOL bNoMsg)
{
	HRESULT hr = E_FAIL;
	if(m_FtpConfig.m_serverMode == SERVER_FTP)
	{
		CFtpInfo FtpInfo;
		FtpInfo.m_strFtpConnect = m_FtpConfig.m_FtpServerParam.m_strFtpAddr;
		FtpInfo.m_strUser = m_FtpConfig.m_FtpServerParam.m_strUser;
		FtpInfo.m_strPwd = m_FtpConfig.m_FtpServerParam.m_strPwd;
		FtpInfo.m_uPort = m_FtpConfig.m_FtpServerParam.m_uPort;
		FtpInfo.m_strLocalFile = _strLocalFile;
		FtpInfo.m_strFtpFile = _strFtpFile;

		CFTPDownload dl(&FtpInfo);
		hr = dl.Download();
		if(FAILED(hr))
		{
			return hr;
		}
	#ifdef DEBUG
		AfxMessageBox(dl.m_result_str);
	#endif
	}
	else if(m_FtpConfig.m_serverMode == SERVER_LOCAL)
	{
		CString ftpFile = m_FtpConfig.m_FtpServerParam.m_strFtpAddr + _strFtpFile;
		if(CFilePathHelper::FilePathExists(_strLocalFile))
		{
			DWORD dwAttrs=GetFileAttributesW(_strLocalFile);
			if (dwAttrs & FILE_ATTRIBUTE_READONLY)
			{
				SetFileAttributes(_strLocalFile,dwAttrs & ~FILE_ATTRIBUTE_READONLY);
			}

		}
		if (!CopyFile(ftpFile,_strLocalFile,FALSE))
		{
			if (!bNoMsg)
			{
				AfxMessageBox(L"拷贝文件失败！");
			}
			return E_FAIL;
		}
		if(CFilePathHelper::FilePathExists(_strLocalFile))
		{
			DWORD dwAttrs=GetFileAttributesW(_strLocalFile);
			if (dwAttrs & FILE_ATTRIBUTE_READONLY)
			{
				SetFileAttributes(_strLocalFile,dwAttrs & ~FILE_ATTRIBUTE_READONLY);
			}

		}
	}
	else
	{
		ASSERT(FALSE);
	}
	return S_OK;
}

HRESULT CFtpRef::Remove(CString _strFtpFile)
{
	HRESULT hr = E_FAIL;
	if(m_FtpConfig.m_serverMode == SERVER_FTP)
	{
		CInternetSession Cis;//All argument defaults are OK.
		CFtpConnection* pFtp_Conn;//The ftp_connection
		pFtp_Conn = Cis.GetFtpConnection(m_FtpConfig.m_FtpServerParam.m_strFtpAddr,
			m_FtpConfig.m_FtpServerParam.m_strUser,
			m_FtpConfig.m_FtpServerParam.m_strPwd,
			m_FtpConfig.m_FtpServerParam.m_uPort);//Connect to FTP Server
		CPtrAutoClean<CFtpConnection> clr(pFtp_Conn);
		CFtpFileFind finder(pFtp_Conn);

		//Lets check for the files existince first using a standard FindFile call
		if (finder.FindFile(_strFtpFile) == FALSE)
		{
			return S_OK	;
		}	
		finder.Close();
		BOOL bDelete = pFtp_Conn->Remove(_strFtpFile);
		if(!bDelete)
		{
			DWORD dwLastError = GetLastError(  );
			CString strMsg;
			strMsg.Format(_T("删除Ftp文件%s出错，错误值为：%d"),_strFtpFile,dwLastError);
			AfxMessageBox(strMsg);
		}
		pFtp_Conn->Close();
		Cis.Close();
#ifdef DEBUG
		CString strMsg;
		strMsg.Format(_T("删除文件%s成功！"),_strFtpFile);
		AfxMessageBox(strMsg);
#endif
	}
	else if(m_FtpConfig.m_serverMode == SERVER_LOCAL)
	{
		CString strFile = m_FtpConfig.m_FtpServerParam.m_strFtpAddr;
		strFile += _strFtpFile;
		BOOL bDelete = DeleteFile(strFile);
		if(!bDelete)
		{
			DWORD dwLastError = GetLastError(  );
			CString strMsg;
			strMsg.Format(_T("删除文件%s出错，错误值为：%d"),_strFtpFile,dwLastError);
			AfxMessageBox(strMsg);
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	return S_OK;
}

HRESULT CFtpRef::CreateFolder(CString _strFtpFolder)
{
	HRESULT hr = E_FAIL;
	if(m_FtpConfig.m_serverMode == SERVER_FTP)
	{
		CInternetSession Cis;//All argument defaults are OK.
		CFtpConnection* pFtp_Conn;//The ftp_connection
		pFtp_Conn = Cis.GetFtpConnection(m_FtpConfig.m_FtpServerParam.m_strFtpAddr,
			m_FtpConfig.m_FtpServerParam.m_strUser,
			m_FtpConfig.m_FtpServerParam.m_strPwd,
			m_FtpConfig.m_FtpServerParam.m_uPort);//Connect to FTP Server
		CPtrAutoClean<CFtpConnection> clr(pFtp_Conn);
		CFtpFileFind finder(pFtp_Conn);

		//Lets check for the files existince first using a standard FindFile call
		if (finder.FindFile(_strFtpFolder) == TRUE)
		{
			return S_FALSE	;
		}	
		finder.Close();
		BOOL bCreate = pFtp_Conn->CreateDirectory(_strFtpFolder);
		if(!bCreate)
		{
			DWORD dwLastError = GetLastError(  );
			CString strMsg;
			strMsg.Format(_T("创建Ftp文件夹%s出错，错误值为：%d"),_strFtpFolder,dwLastError);
			AfxMessageBox(strMsg);
		}
		pFtp_Conn->Close();
		Cis.Close();
#ifdef DEBUG
		CString strMsg;
		strMsg.Format(_T("创建文件夹%s成功！"),_strFtpFolder);
		AfxMessageBox(strMsg);
#endif
	}
	else if(m_FtpConfig.m_serverMode == SERVER_LOCAL)
	{
		CString strFoler = m_FtpConfig.m_FtpServerParam.m_strFtpAddr;
		strFoler += _strFtpFolder;
		BOOL bCreate =CreateDirectory(strFoler,NULL);
		if(!bCreate)
		{
			DWORD dwLastError = GetLastError(  );
			CString strMsg;
			strMsg.Format(_T("创建文件夹%s出错，错误值为：%d"),_strFtpFolder,dwLastError);
			AfxMessageBox(strMsg);
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	return S_OK;
}

HRESULT CFtpRef::RemoveFolder(CString _strFtpFolder)
{
	HRESULT hr = E_FAIL;
	if(m_FtpConfig.m_serverMode == SERVER_FTP)
	{
		CInternetSession Cis;//All argument defaults are OK.
		CFtpConnection* pFtp_Conn;//The ftp_connection
		pFtp_Conn = Cis.GetFtpConnection(m_FtpConfig.m_FtpServerParam.m_strFtpAddr,
			m_FtpConfig.m_FtpServerParam.m_strUser,
			m_FtpConfig.m_FtpServerParam.m_strPwd,
			m_FtpConfig.m_FtpServerParam.m_uPort);//Connect to FTP Server
		CPtrAutoClean<CFtpConnection> clr(pFtp_Conn);
		CFtpFileFind finder(pFtp_Conn);

		//Lets check for the files existince first using a standard FindFile call
		if (finder.FindFile(_strFtpFolder) == FALSE)
		{
			return S_FALSE	;
		}	
		finder.Close();
		BOOL bDelete = pFtp_Conn->RemoveDirectory(_strFtpFolder);
		if(!bDelete)
		{
			DWORD dwLastError = GetLastError(  );
			CString strMsg;
			strMsg.Format(_T("删除Ftp文件夹%s出错，错误值为：%d"),_strFtpFolder,dwLastError);
			AfxMessageBox(strMsg);
		}
		pFtp_Conn->Close();
		Cis.Close();
#ifdef DEBUG
		CString strMsg;
		strMsg.Format(_T("删除文件夹%s成功！"),_strFtpFolder);
		AfxMessageBox(strMsg);
#endif
	}
	else if(m_FtpConfig.m_serverMode == SERVER_LOCAL)
	{
		CString strFoler = m_FtpConfig.m_FtpServerParam.m_strFtpAddr;
		strFoler += _strFtpFolder;
		BOOL bDelete = RemoveDirectory(strFoler);
		if(!bDelete)
		{
			DWORD dwLastError = GetLastError(  );
			CString strMsg;
			strMsg.Format(_T("删除文件夹%s出错，错误值为：%d"),_strFtpFolder,dwLastError);
			AfxMessageBox(strMsg);
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	return S_OK;
}

HRESULT CFtpRef::TestConnect(CFtpConfig* _pFtpConfig)
{
	HRESULT hr = E_FAIL;
	if(_pFtpConfig->m_serverMode == SERVER_FTP)
	{
		try
		{
			CInternetSession Cis;//All argument defaults are OK.
			CFtpConnection* pFtp_Conn;//The ftp_connection
			pFtp_Conn = Cis.GetFtpConnection(_pFtpConfig->m_FtpServerParam.m_strFtpAddr,
				_pFtpConfig->m_FtpServerParam.m_strUser,
				_pFtpConfig->m_FtpServerParam.m_strPwd,
				_pFtpConfig->m_FtpServerParam.m_uPort);//Connect to FTP Server
			CPtrAutoClean<CFtpConnection> clr(pFtp_Conn);
		}
		catch (CInternetException * e)
		{
			e->Delete();
			return E_HRESULT_FTP_NOTCONNECT;
		}
		
	}
	else if(_pFtpConfig->m_serverMode == SERVER_LOCAL)
	{
		//只要保证文件夹存在就可以了
		if(!PathFileExists(_pFtpConfig->m_FtpServerParam.m_strFtpAddr))
		{
			return E_HRESULT_FOLDER_NOTEXIST;
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	return S_OK;
}
