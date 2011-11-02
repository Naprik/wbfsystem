#pragma once


class BASE_DLL_API CFilePathHelper
{
public:
	CFilePathHelper(void);
	virtual ~CFilePathHelper(void);

	static BOOL CreateFolder(const CString& dir,BOOL bIncludeFile=TRUE);
	static BOOL DeleteFolder(const CString& dir,BOOL bIncludeSelf=TRUE);

	//�ж�strFileName���ļ��л����ļ�
	//����ֵ�� -1 :������ 0���ļ� 1���ļ���
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
	//���ļ����µ������ļ��к��ĵ��������ļ���_strDestFolder��
	static void CopyFolder(CString& _sSourceFolder,CString _strDestFolder,BOOL _bFailIfExists = TRUE);
	static void GetMainCfgPath(CString& strPath);
};
