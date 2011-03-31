#ifndef G_FTPDOWNLOAD_H
#define G_FTPDOWNLOAD_H

#include <afxinet.h>
#include "ProgDlg.h"
//This class handles the the progress dialog, and then launches a seperate thread to handle downloading files.

//To use pass a CStringArray * to the constructor
//This CStringArray has certain important CStrings in each position.
//Csa[0] == Server Name
//Csa[1] == User Name
//Csa[2] == Password
//Csa[3] = Ftp file name
//Csa[4] = Local file name (name on local drive after downloaded)
//As many Files as desired can be downloaded. Just begin filenames at index 3.
//File names are to be stored in alternating order, first the ftp file name then the local file name
//An optional second argument specifies the connection timeout value. In my opinion 3000 seemed the best.
//The default timeout of 0 will not change any timeout value in CInternetSession.
class CFtpInfo;
class CFTPDownload
{
public:
	unsigned int m_thread_finished_flag;
	unsigned int m_abort_flag;
	CString m_result_str;
	CProgressDlg m_cpd;

	DWORD m_connection_timeout;
public:
	CFTPDownload(CFtpInfo *_pFtpInfo, DWORD ctout = 0);//Constructor
	CFtpInfo* m_pFtpInfo;
	HRESULT Download();
};
#endif
