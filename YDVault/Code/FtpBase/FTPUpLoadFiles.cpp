// FTPUpLoadFiles.cpp: implementation of the CFTPUpLoadFiles class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FTPUpLoadFiles.h"
#include "FtpInfo.h"
#include "../Base/AutoClean.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define DL_BUFFER_SIZE 4096

//**********************CONSTRUCTOR*********************
CFTPUpLoadFiles::CFTPUpLoadFiles(CFtpUpload * t_pFtpI)
{//CinternetSession is contained and is created with defaults
	m_pFtpI = t_pFtpI;
	m_Ftp_Conn = 0;
	m_bUploadFileSuccess = TRUE;
	//I have found that by reducing the timeout connection, the internet connection speed is faster.
	if (m_pFtpI->m_connection_timeout)
	{
		//Only if not 0 do we bother changing the timeout value
		m_Cis.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, m_pFtpI->m_connection_timeout);//I like 3000
	}

	ReadFile(m_pFtpI->m_pFtpInfo->m_strLocalFile, m_pFtpI->m_pFtpInfo->m_strFtpFile);//Upload file name, disk file name


	return;
}

CFTPUpLoadFiles::~CFTPUpLoadFiles()
{
	if(!m_bUploadFileSuccess)
	{
		//没有成功上传文件，要将ftp上文件删除
		m_Ftp_Conn = m_Cis.GetFtpConnection(m_pFtpI->m_pFtpInfo->m_strFtpConnect,
			m_pFtpI->m_pFtpInfo->m_strUser,
			m_pFtpI->m_pFtpInfo->m_strPwd,
									m_pFtpI->m_pFtpInfo->m_uPort);//Connect to FTP Server
		CPtrAutoClean<CFtpConnection> clr(m_Ftp_Conn);
		CFtpFileFind finder(m_Ftp_Conn);
		
		//Lets check for the files existince first using a standard FindFile call
		if (finder.FindFile(m_pFtpI->m_pFtpInfo->m_strFtpFile) == FALSE)
		{
			return ;
		}	
		finder.Close();
		BOOL bDelete = m_Ftp_Conn->Remove(m_pFtpI->m_pFtpInfo->m_strFtpFile);
		if(!bDelete)
		{
			DWORD dwLastError = GetLastError(  );
			CString strMsg;
			strMsg.Format(_T("删除Ftp文件%s出错，错误值为：%d"),m_pFtpI->m_pFtpInfo->m_strFtpFile,dwLastError);
			AfxMessageBox(strMsg);
		}
		m_Ftp_Conn->Close();
	}
	m_Cis.Close();//Close this session
}
//m_pConn must be set B4 calling this function
//*********************READFILE*****************************
void CFTPUpLoadFiles::ReadFile(CString &source, CString &dest)
{
	if (m_pFtpI->m_abort_flag) 
	{
		return;//Has cancel been pressed ? If yes then split!!!
	}
	if (local_file_exists(source)==FALSE) 
	{
		return;
	}

	m_pFtpI->m_cpd.SetHeader(source,FALSE);
	//Unfortunately we have to reopen the ftp connection for each file.
	m_strStatus.Format( _T("连接到：%s"), m_pFtpI->m_pFtpInfo->m_strFtpConnect);
	UpdateStatus();
	if (m_pFtpI->m_abort_flag) 
	{
		return;//Make sure operation hasn't been cancelled yet b4 calling inet function
	}
	m_Ftp_Conn = m_Cis.GetFtpConnection(m_pFtpI->m_pFtpInfo->m_strFtpConnect,
									m_pFtpI->m_pFtpInfo->m_strUser,
									m_pFtpI->m_pFtpInfo->m_strPwd,
									m_pFtpI->m_pFtpInfo->m_uPort);//Connect to FTP Server
	CPtrAutoClean<CFtpConnection> clr(m_Ftp_Conn);
	m_strStatus = source;
	UpdateStatus();//Show source file name


	//If file ain't there we can't download it so split!!
	m_pFtpI->m_cpd.SetUpper(m_file_size);

	for (int i = 0; i < 6; i++)//做六次操作，防止异常
	{
		CInternetFile*  ifp = m_Ftp_Conn->OpenFile(dest,GENERIC_WRITE);
		CPtrAutoClean<CInternetFile> clr1(ifp);
		m_bUploadFileSuccess = FALSE;
		if (m_cfo.Open(source, CFile::modeRead , NULL)==FALSE)//Now open our disk file
		{
			m_strStatus.Format( _T("不能创建文件：%s"), dest);
			AfxMessageBox(m_strStatus);
			throw m_strStatus; 
		}
		
		m_pFtpI->m_cpd.SetHeader(source,FALSE);
		m_strStatus.Format(_T("打开文件：%s"), dest);
		UpdateStatus();
		if (m_pFtpI->m_abort_flag)
		{
			return;//Make sure operation hasn't been cancelled yet b4 calling inet function
		}
		
		char buffer[DL_BUFFER_SIZE];
		unsigned int amount_read = DL_BUFFER_SIZE;
		unsigned int total_read = 0;
		BOOL bError = FALSE;
		int iCount = 0;
		DWORD dwStart ;
		float fSpeend =10;
		while (amount_read == DL_BUFFER_SIZE && 
			m_pFtpI->m_abort_flag == FALSE)
		{
			if(iCount == 0)
			{
				dwStart = GetTickCount();
			}
		
			try
			{
				//	ifp->Seek(total_read,CFile::begin);
				amount_read = m_cfo.Read(buffer, DL_BUFFER_SIZE);
				ifp->Write(buffer,amount_read);
			}
			catch (CInternetException* e)
			{
				e->Delete();
				bError = TRUE;
				break;;
			}
			//cfo.Write(buffer, amount_read);//Write this to our data file
			total_read += amount_read;
			if(++iCount == 10)
			{
				//重新计算速度
				DWORD dwSpan = GetTickCount() - dwStart;
				fSpeend = (float) (DL_BUFFER_SIZE*10 )/dwSpan ;  ;
				iCount = 0;
			}
			m_strStatus.Format(_T("共%d,%d字节已写入,当前速度是：%.2f KB/s"), m_file_size,total_read, fSpeend);
			UpdateStatus();
			m_pFtpI->m_cpd.SetPos(total_read);
		
		}
		ifp->Close();
		m_cfo.Close();//Close the file
		if(m_pFtpI->m_abort_flag)
		{
			break;
		}
		if(!bError )
		{
			//没有有错误发生，退出for循环
			m_bUploadFileSuccess = TRUE;
			break;
		}
	}

	//Unforunately we have to close the FTP session in order to be able to change to root folder.
	//There is no way around it. We have reopen the ftp connection for each file. Oh well.
	m_strStatus.Format(_T("关闭连接：%s"), m_pFtpI->m_pFtpInfo->m_strFtpConnect);
	UpdateStatus();

	return;
}

//*********************************UPDATESTATUS***********************************
void CFTPUpLoadFiles::UpdateStatus(void)
{
	m_pFtpI->m_cpd.SetStatus(m_strStatus);
	return;
}

//This function performs three important functions.
//I have learned that calling OpenFile() on an FTP server when the files isn't there makes my
//program go BOOM so we have to check to see if the file is there first. I have also learned
//that in checking for the files existence we can also get the file size.

//1. It checks to see wether file exists on ftp server. If it doesn't it returns FALSE.
//2. Using CFtpFileFind it gets the FIRST file name matching input string. This means that if
	//a string like amex*.txt is passed the FIRST and only the first file matching that string will
	//be downloaded. This suits my purposes (for now) because I have to download daily data from a folder
	//that is updated daily with the latest data file. This file name changes according to the date with
	//the first four leters being the same. Using this logic you can still pass a specific file name
	//or use wilcards, but just remember that if you use wildcards only the first file matching will
	//be downloaded.

//3. Gets file size. we cannot however depend on this being correct. I read something about headers
	//on the FTP site having to be updated, and also that CERN proxys can't get the file info or something
	//Since this is the case and if it is less than DL_BUFFER_SIZE I just make it 0. If it is 0
	//the progess dialog knows to ignore the percent stuff.

//This function sets temp_ftp_name. Use this name for the actual download.
//This function also sets file_size.
//*****************************Local_FILE_EXISTS******************************
BOOL CFTPUpLoadFiles::local_file_exists(CString &source)
{
	m_strStatus.Format(_T("Getting File Information %s"), source);
	UpdateStatus();

	if (m_pFtpI->m_abort_flag) 
	{
		return FALSE;
	}
	CFileFind finder;

	//Lets check for the files existince first using a standard FindFile call
	if (m_pFtpI->m_abort_flag) 
		return FALSE;
	if (finder.FindFile(source) == FALSE)
	{
		file_not_found(source);
		return FALSE;	
	}	

	//We have to use a FindNextFile to get info on first file, why ?. Becuase FindNextFile doesn't get the
	//next file on its first invocation. It gets the first file! Makes sense doesn't it? Pure Genius...
	if (m_pFtpI->m_abort_flag)
	{
		return FALSE;
	}
	finder.FindNextFile();
	m_file_size = 0;
	CFile file;
	if (file.Open(source, CFile::modeRead , NULL)==FALSE)//Now open our disk file
	{
		m_strStatus.Format(_T("Unable to Open file %s"), source);
		AfxMessageBox(m_strStatus);
		throw m_strStatus; 
	}
	m_file_size = (unsigned int)file.GetLength();
	file.Close();
	//m_file_size = finder.GetLength();
	if (m_file_size < DL_BUFFER_SIZE) 
	{
		m_file_size = 0;
	}//This tells the progress dialog to ignore the progress.

	return TRUE;//If here file definitely exists
}

//***************************FILE_NOT_FOUND*************************
void CFTPUpLoadFiles::file_not_found(CString & source)
{
	m_strStatus.Format(_T("在本地找不到文件: %s"), source);
	UpdateStatus();
	AfxMessageBox(m_strStatus);//Put up message box to let user Know!!
	return;
}

