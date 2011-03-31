#include "stdafx.h"
#include "FTPDownload.h"
#include "FTPDownLoadFiles.h"

UINT DownLoadFunction(LPVOID lParam);//Global Prototype for thread function

//This class handles the progress dialog, and then launches a seperate thread to handle
//downloading files.
//**********************Default CONSTRUCTOR*********************
CFTPDownload::CFTPDownload(CFtpInfo *_pFtpInfo, DWORD ctout)
{
	m_pFtpInfo = _pFtpInfo;
	m_connection_timeout = ctout;
	m_result_str ="";
	m_abort_flag = FALSE;
	m_thread_finished_flag = FALSE;//When this == TRUE the other thread is finished with its job

	return;
}

HRESULT CFTPDownload::Download()
{
	HRESULT hr = E_FAIL;
	
	m_cpd.Create();//Create the progress dialog box.
	
	CWinThread * dl_thread = AfxBeginThread(::DownLoadFunction, this);//Start downloading thread
	
	while (m_thread_finished_flag == FALSE)
	{
		if (m_cpd.CheckCancelButton())
		{
			m_abort_flag = TRUE;//If cancel pushed let other thread know it's time to split
		}
	}
	if (m_result_str.IsEmpty()==FALSE) 
	{
		return E_FAIL;//Then an exception was thrown and caught in the other thread!!
	}
	if (m_abort_flag) 
	{
		m_result_str = _T("User Cancelled Download Operation");//Now set up result string
		return E_FAIL;
	}
	else 
	{
		m_result_str = _T("Internet Download Completed Successfully");
		return S_OK;
	}
	return S_OK;
}

//NOT A MEMEBER FUNCTION !!!!!
//This is the seperate thread function
//****************************DOWNLOADFUNCTION***********************************
UINT DownLoadFunction(LPVOID lParam)
{
	CFTPDownload * pFtpI = (CFTPDownload *)lParam;//I want a try block to catch any exceptions
	try	
	{
		CFTPDownLoadFiles dl(pFtpI);	
	}//This performs the downloading
	catch(char * str)//If a file can't be opened this is thrown
	{ 	
		pFtpI->m_result_str = str; 		
	}
	catch (CFileException * pEx)
	{ 	
		pFtpI->m_result_str.Format(_T("File Error %d %s"), pEx->m_cause, pEx->m_strFileName); 
	}
	catch (CInternetException* pEx)
	{
		pFtpI->m_result_str.Format(_T("Internet Exception Error %d"), pEx->m_dwError);
		if (pEx->m_dwError == ERROR_INTERNET_EXTENDED_ERROR)
		{
			TCHAR temp_str[1024];
			DWORD length = 1024;
			DWORD temp_int = 0;
		::InternetGetLastResponseInfo(&temp_int, temp_str, &length);
		pFtpI->m_result_str += temp_str;
		}
	}

	pFtpI->m_thread_finished_flag = TRUE;//Let the main thread know we finished
	return 0;//Don't care about return value
}
