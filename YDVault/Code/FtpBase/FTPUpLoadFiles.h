// FTPUpLoadFiles.h: interface for the CFTPUpLoadFiles class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPUPLOADFILES_H__17544538_22DE_4B7C_B630_5B0D2CDDF614__INCLUDED_)
#define AFX_FTPUPLOADFILES_H__17544538_22DE_4B7C_B630_5B0D2CDDF614__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FTPUpload.h"
class CFTPUpLoadFiles  
{
public:
	CFTPUpLoadFiles(CFtpUpload * t_pFtpI);
	virtual ~CFTPUpLoadFiles();
protected:
	CInternetSession m_Cis;//All argument defaults are OK.
	CFtpConnection* m_Ftp_Conn;//The ftp_connection
	
	CFtpUpload * m_pFtpI;//Pointer to FTPupload object in other thread that also holds dialog
	CFile m_cfo;//CFileObject used to read file
	
	void ReadFile(CString & source, CString & dest);
	BOOL local_file_exists(CString & source);
	void file_not_found(CString & source);
	
	void UpdateStatus(void);
	CString m_strStatus;
	
	unsigned int m_file_size;
	
private:
	BOOL m_bUploadFileSuccess;//是否成功上传文件
};

#endif // !defined(AFX_FTPUPLOADFILES_H__17544538_22DE_4B7C_B630_5B0D2CDDF614__INCLUDED_)
