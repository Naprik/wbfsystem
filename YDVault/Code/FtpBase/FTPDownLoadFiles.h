#ifndef G_FTPDOWNLOAD_FILES_H
#define G_FTPDOWNLOAD_FILES_H

#include "FTPDownload.h"

//CInternetSession can throw an exception catch it like so: catch (CInternetException* pEx)

class CFTPDownLoadFiles
{
protected:
	CInternetSession m_Cis;//All argument defaults are OK.
	CFtpConnection* m_Ftp_Conn;//The ftp_connection

	CFTPDownload * m_pFtpI;//Pointer to FTPDownload object in other thread that also holds dialog
	CFile m_cfo;//CFileObject used to write file

	void ReadFile(CString & source, CString & dest);
	BOOL ftp_file_exists(CString & source);
	void file_not_found(CString & source);

	void UpdateStatus(void);
	CString m_strStatus;

	unsigned int m_file_size;
	CString m_temp_ftp_name;

public:
	CFTPDownLoadFiles(CFTPDownload * t_pFtpI);//Constructor
	virtual ~CFTPDownLoadFiles();
private:
	BOOL	m_bReadFileSuccess;//是否成功下载文件
};

#endif
