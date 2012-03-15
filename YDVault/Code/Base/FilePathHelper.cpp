#include "stdafx.h"
#include "FilePathHelper.h"
#include <shlwapi.h>
extern TCHAR _ModulePath[MAX_PATH];
CFilePathHelper::CFilePathHelper(void)
{
}

CFilePathHelper::~CFilePathHelper(void)
{
}

BOOL CFilePathHelper::FilePathExists(const CString& lpszPath)
{
	return ::PathFileExists(lpszPath);
}

BOOL CFilePathHelper::CreateFolder(const CString& Path,BOOL FilenameIncluded)
{
	if (!IsFilePathValid(Path))
	{
		return FALSE;
	}

	CString strTmp = Path;
	strTmp.Replace(_T('/'), _T('\\'));
	int Pos = 0;
	while((Pos=strTmp.Find(_T('\\'),Pos))!=-1)
	{
		CString strDir = strTmp.Left(Pos + 1);
		while (strTmp[Pos] == _T('\\')) Pos++;	// 跳过多个斜杠组成的目录符号，如C:\\

		if(!PathIsRoot(strDir))
		{
			if(!CreateDirectory(strDir,NULL))
			{
				DWORD nErr = GetLastError();
				if(nErr != ERROR_ALREADY_EXISTS) 
				{
					LPVOID lpMsgBuf;
					FormatMessage( 
						FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM | 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						nErr,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						(LPTSTR) &lpMsgBuf,
						0,
						NULL 
						);
					MessageBox( NULL, (LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION );
					LocalFree( lpMsgBuf );
					return FALSE;
				}
			}
		}
	}
	if(!FilenameIncluded)
	{
		if(!PathIsRoot(strTmp) && !CreateDirectory(strTmp,NULL))
		{
			DWORD nErr = GetLastError();
			if(nErr != ERROR_ALREADY_EXISTS) 
			{
				LPVOID lpMsgBuf;
				FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					nErr,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL 
					);
				MessageBox( NULL, (LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION );
				LocalFree( lpMsgBuf );
				return FALSE;
			}
		}
	}
	return ((!FilenameIncluded)?!_taccess(strTmp,0):
		!_taccess(strTmp.Left(strTmp.ReverseFind(_T('\\'))),0));
}

BOOL CFilePathHelper::DeleteFolder(const CString& DirName,BOOL bIncludeSelf)
{
	CFileFind tempFind;
	TCHAR tempFileFind[MAX_PATH] = {0};
	swprintf_s(tempFileFind,_T("%s\\*.*"),(LPCTSTR)DirName);
	BOOL IsFinded=(BOOL)tempFind.FindFile(tempFileFind);
	while(IsFinded)
	{
		IsFinded=(BOOL)tempFind.FindNextFile();
		if(!tempFind.IsDots())
		{
			TCHAR foundFileName[MAX_PATH] = {0};
			wcscpy_s(foundFileName,tempFind.GetFileName().GetBuffer(200));
			if(tempFind.IsDirectory())
			{
				TCHAR tempDir[200] = {0};
				swprintf_s(tempDir,_T("%s\\%s"),(LPCTSTR)DirName,foundFileName);
				if(!DeleteFolder(tempDir,TRUE))
				{
				}
			}
			else
			{
				TCHAR tempFileName[MAX_PATH] = {0};
				swprintf_s(tempFileName,_T("%s\\%s"),(LPCTSTR)DirName,foundFileName);
				DeleteFile(tempFileName);
			}
		}
	}
	tempFind.Close();
	if(bIncludeSelf && !RemoveDirectory(DirName))
	{
		return FALSE;
	}	
	return TRUE;
}

//判断strFileName是文件夹还是文件
//返回值： -1 :不存在 0：文件 1：文件夹
int CFilePathHelper::IsDirectoryOrFile(CString& strFileName)
{
	strFileName.TrimLeft();
	strFileName.TrimRight();

	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(strFileName,&fd);
	::FindClose(hFind);

	//不存在同名的文件或文件夹
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return -1 ;
	}
	//判断是否为目录
	else if (fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
	{
		return 1;
	}
	else//判断为文件
	{        
		return 0;
	}    
}


//////////////////////////////////////////////////////////////////////////
//删除非空目录
//////////////////////////////////////////////////////////////////////////
BOOL CFilePathHelper::DeleteDirectory(const CString& lpszPath)
{
	SHFILEOPSTRUCT FileOp;
	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.hNameMappings = NULL;
	FileOp.hwnd = NULL;
	FileOp.lpszProgressTitle = NULL;
	FileOp.pFrom = lpszPath;
	FileOp.pTo = NULL;
	FileOp.wFunc = FO_DELETE;
	return SHFileOperation(&FileOp) == 0;
}

void CFilePathHelper::SplitFileName(const CString& sPath,CString& filename,CString& fileext)
{
	filename.Empty();
	fileext.Empty();

	CString str(_T(""));
	int nPos = -1;
	if((nPos = sPath.ReverseFind(_T('\\'))) != -1 || (nPos = sPath.ReverseFind(_T('/'))) != -1)
	{
		str = sPath.Right(sPath.GetLength()-nPos-1);
	}
	else
	{
		str = sPath;
	}
	if(!str.IsEmpty())
	{
		if((nPos = str.ReverseFind(_T('.'))) != -1 )
		{
			filename = str.Left(nPos);
			fileext = str.Right(str.GetLength()-nPos-1);
		}
		else
		{
			filename = str;
		}
	}
}

void CFilePathHelper::RemoveInvalidCharInFilename(CString& filepath)
{
	if(filepath.IsEmpty()) return;
	filepath.Replace(_T('/'),_T('\\'));
	while(true)
	{
		int nPos = filepath.Find(_T("\\.."));
		if(nPos == -1) break;
		for(int i=nPos-1;i>=0;i--)
		{
			if(filepath[i] == _T('\\'))
			{
				CString str = filepath.Mid(i,nPos-i);
				str += _T("\\..");
				filepath.Replace(str,_T(""));
				break;
			}
		}
	}
}

BOOL CFilePathHelper::FilePathIsSame(const CString& path1,const CString& path2)
{
	CString spath1(path1),spath2(path2);
	RemoveInvalidCharInFilename(spath1);
	RemoveInvalidCharInFilename(spath2);
	return spath1.CompareNoCase(spath2) == 0;
}

BOOL CFilePathHelper::IsFileNameValid(const CString& strName)
{
	if(!strName.IsEmpty())
	{
		if( strName.Find(_T('/')) == -1 &&
			strName.Find(_T('\\')) == -1 &&
			strName.Find(_T(':')) == -1 &&
			strName.Find(_T('?')) == -1 &&
			strName.Find(_T('*')) == -1 &&
			strName.Find(_T('<')) == -1 &&
			strName.Find(_T('>')) == -1 &&
			strName.Find(_T('|')) == -1 &&
			strName.Find(_T('"')) == -1 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CFilePathHelper::TrimFileNameToValid(CString& strName)
{
	if (strName.GetLength() <= 0) return;

	int nPos = -1;
	while (TRUE)
	{
		nPos = strName.Find(_T('/'));
		if (nPos != -1)
		{
			strName.Delete(nPos);
		}
		else
		{
			break;
		}
	}

	while (TRUE)
	{
		nPos = strName.Find(_T('\\'));
		if (nPos != -1)
		{
			strName.Delete(nPos);
		}
		else
		{
			break;
		}
	}

	while (TRUE)
	{
		nPos = strName.Find(_T(':'));
		if (nPos != -1)
		{
			strName.Delete(nPos);
		}
		else
		{
			break;
		}
	}

	while (TRUE)
	{
		nPos = strName.Find(_T('?'));
		if (nPos != -1)
		{
			strName.Delete(nPos);
		}
		else
		{
			break;
		}
	}

	while (TRUE)
	{
		nPos = strName.Find(_T('*'));
		if (nPos != -1)
		{
			strName.Delete(nPos);
		}
		else
		{
			break;
		}
	}

	while (TRUE)
	{
		nPos = strName.Find(_T('<'));
		if (nPos != -1)
		{
			strName.Delete(nPos);
		}
		else
		{
			break;
		}
	}

	while (TRUE)
	{
		nPos = strName.Find(_T('>'));
		if (nPos != -1)
		{
			strName.Delete(nPos);
		}
		else
		{
			break;
		}
	}

	while (TRUE)
	{
		nPos = strName.Find(_T('|'));
		if (nPos != -1)
		{
			strName.Delete(nPos);
		}
		else
		{
			break;
		}
	}

	while (TRUE)
	{
		nPos = strName.Find(_T('"'));
		if (nPos != -1)
		{
			strName.Delete(nPos);
		}
		else
		{
			break;
		}
	}

	if (strName.GetLength() <= 0)	// 都是不合法字符
	{
		COleDateTime time;
		DATE date = time.GetCurrentTime();
		strName.Format(_T("%f"), date);
	}
}

BOOL CFilePathHelper::IsFilePathValid(const CString& strPath)
{
	if(!strPath.IsEmpty())
	{
		int Pos=0;
		Pos = strPath.Find(_T(':'));
		if(Pos == -1) return FALSE; 
		Pos = strPath.Find(_T('\\'));
		if(Pos == -1)
		{
			Pos = strPath.Find(_T('/'));
			if (Pos == -1 ) return FALSE;  //不包含驱动器符
		}

		if( strPath.Find(_T(':'), Pos + 1) == -1 &&
			strPath.Find(_T('?'), Pos + 1) == -1 &&
			strPath.Find(_T('*'), Pos + 1) == -1 &&
			strPath.Find(_T('<'), Pos + 1) == -1 &&
			strPath.Find(_T('>'), Pos + 1) == -1 &&
			strPath.Find(_T('|'), Pos + 1) == -1 && 
			strPath.Find(_T('"'), Pos + 1) == -1)
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CFilePathHelper::TrimFilePathToValid(CString& strPath)
{
	if (strPath.GetLength() <= 0) return;
	
	int nPos = -1;

	while (TRUE)
	{
		nPos = strPath.Find(_T(':'));
		nPos = strPath.Find(_T(':'), nPos + 1);	// 跳过驱动器盘符中的冒号
		if (nPos != -1)
		{
			strPath.Delete(nPos);
		}
		else
		{
			break;
		}
	}

	while (TRUE)
	{
		nPos = strPath.Find(_T('?'));
		if (nPos != -1)
		{
			strPath.Delete(nPos);
		}
		else
		{
			break;
		}
	}

	while (TRUE)
	{
		nPos = strPath.Find(_T('*'));
		if (nPos != -1)
		{
			strPath.Delete(nPos);
		}
		else
		{
			break;
		}
	}

	while (TRUE)
	{
		nPos = strPath.Find(_T('<'));
		if (nPos != -1)
		{
			strPath.Delete(nPos);
		}
		else
		{
			break;
		}
	}

	while (TRUE)
	{
		nPos = strPath.Find(_T('>'));
		if (nPos != -1)
		{
			strPath.Delete(nPos);
		}
		else
		{
			break;
		}
	}

	while (TRUE)
	{
		nPos = strPath.Find(_T('|'));
		if (nPos != -1)
		{
			strPath.Delete(nPos);
		}
		else
		{
			break;
		}
	}

	while (TRUE)
	{
		nPos = strPath.Find(_T('"'));
		if (nPos != -1)
		{
			strPath.Delete(nPos);
		}
		else
		{
			break;
		}
	}
}

void CFilePathHelper::CopyFolder(CString& _sSourceFolder,CString _strDestFolder,BOOL _bFailIfExists /*= TRUE*/)
{
	CreateFolder(_strDestFolder,FALSE);
	CFileFind finder;

	// build a string with wildcards
	CString strWildcard(_sSourceFolder);
	strWildcard += _T("\\*.*");

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		// skip . and .. files; otherwise, we'd
		// recur infinitely!
		if (finder.IsDots())
			continue;
		// if it's a directory, recursively search it
		if (finder.IsDirectory())
		{
			CString strPath = finder.GetFilePath();
			CString strLastPath = strPath.Right(strPath.GetLength() - strPath.ReverseFind(_T('\\')) );
			CString strDestPath = _strDestFolder + strLastPath;
			CopyFolder(strPath,strDestPath,_bFailIfExists);
		}
		else
		{
			CString strFileName = finder.GetFileName();
			CString strDestFileName = _strDestFolder + _T("\\");
			strDestFileName += strFileName;
			CString strSoucePath = finder.GetFilePath();
			CopyFile(strSoucePath,strDestFileName,_bFailIfExists);
		}
	}
	finder.Close();
}

void CFilePathHelper::GetMainCfgPath(CString& strPath)
{
	
	CString strCfgPath = _ModulePath;
	strCfgPath += L"\\..\\Good Future\\cfg\\DBCfg.xml";

	if (FilePathExists(strCfgPath))
	{
		strPath = _ModulePath;
	}
	else
	{
		TCHAR userPath[MAX_PATH];
		::SHGetSpecialFolderPath(NULL, userPath, CSIDL_COMMON_DOCUMENTS, FALSE);
		strPath = userPath;
	}

	strPath += L"\\..\\Good Future";
}