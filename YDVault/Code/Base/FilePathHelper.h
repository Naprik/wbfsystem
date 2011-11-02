#pragma once


class BASE_DLL_API CFilePathHelper
{
public:
	CFilePathHelper(void);
	virtual ~CFilePathHelper(void);

	static BOOL CreateFolder(const CString& dir,BOOL bIncludeFile=TRUE);
	static BOOL DeleteFolder(const CString& dir,BOOL bIncludeSelf=TRUE);

	//判断strFileName是文件夹还是文件
	//返回值： -1 :不存在 0：文件 1：文件夹
	static int IsDirectoryOrFile(CString& strFileName);
	static BOOL DeleteDirectory(const CString& lpszPath);
	static BOOL FilePathExists(const CString& lpszPath);
	static void SplitFileName(const CString& sPath,CString& filename,CString& fileext);
	static void RemoveInvalidCharInFilename(CString& filepath);
	static BOOL FilePathIsSame(const CString& path1,const CString& path2);

	static BOOL IsFileNameValid(const CString& strName);
	static void TrimFileNameToValid(CString& strName);

	static BOOL IsFilePathValid(const CString& strPath);
	static void TrimFilePathToValid(CString& strPath);
	//将文件夹下的所有文件夹和文档拷贝到文件夹_strDestFolder下
	static void CopyFolder(CString& _sSourceFolder,CString _strDestFolder,BOOL _bFailIfExists = TRUE);
	static void GetMainCfgPath(CString& strPath);
};
