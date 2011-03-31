// FtpUpload.cpp: implementation of the CFtpUpload class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FtpUpload.h"
#include "FTPUpLoadFiles.h"
#include "FtpInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
UINT UpLoadFunction(LPVOID lParam);//Global Prototype for thread function

CFtpUpload::CFtpUpload(CFtpInfo *_pFtpInfo, DWORD ctout /*= 0*/)
{
	m_pFtpInfo = _pFtpInfo;
	m_connection_timeout = ctout;
	m_result_str ="";
	m_abort_flag = FALSE;
	m_thread_finished_flag = FALSE;//When this == TRUE the other thread is finished with its job
	return;
}


HRESULT CFtpUpload::Upload()
{
	HRESULT hr= E_FAIL;
	m_cpd.Create();//Create the progress dialog box.
	
	CWinThread * dl_thread = AfxBeginThread(::UpLoadFunction, this);//Start downloading thread
	
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
		m_result_str = "User Cancelled Up Operation";//Now set up result string
		return E_FAIL;
	}
	else
	{
		m_result_str = "Internet Uploacd Completed Successfully";
		return S_OK;
	}
	return S_OK;
}

CFtpUpload::~CFtpUpload()
{

}

//NOT A MEMEBER FUNCTION !!!!!
//This is the seperate thread function
//****************************DOWNLOADFUNCTION***********************************
UINT UpLoadFunction(LPVOID lParam)
{
	CFtpUpload * pFtpI = (CFtpUpload *)lParam;//I want a try block to catch any exceptions
	try
	{	
		CFTPUpLoadFiles dl(pFtpI);	
	}
	//This performs the downloading
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
