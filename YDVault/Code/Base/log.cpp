#include "stdafx.h"
#include <io.h>
#include <stdlib.h>
#include "Log.h"
#include "FilePathHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int CLog::ToDebug   =  1;
const int CLog::ToFile    =  2;
const int CLog::ToConsole =  4;

const static int LINE_BUFFER_SIZE = 4096*10;

CLog::CLog()
{
	m_lastLogTime = 0;
	m_filename = NULL;
	m_append = false;
    m_hlogfile = NULL;
    m_todebug = false;
    m_toconsole = false;
    m_tofile = false;
}

CLog::~CLog()
{
	if (m_filename != NULL)
		free(m_filename);
    CloseFile();
}

void CLog::SetMode(int mode) 
{
    
    if (mode & ToDebug)
        m_todebug = true;
    else
        m_todebug = false;
	
    if (mode & ToFile)  
	{
		if (!m_tofile)
			OpenFile();
	} 
	else 
	{
		CloseFile();
        m_tofile = false;
    }
    
    if (mode & ToConsole) 
	{
        if (!m_toconsole) 
		{
            AllocConsole();
            fclose(stdout);
            fclose(stderr);
            int fh = _open_osfhandle((LONG_PTR)GetStdHandle(STD_OUTPUT_HANDLE), 0);
            _dup2(fh, 1);
            _dup2(fh, 2);
            _tfdopen(1, _T("wt"));
            _tfdopen(2, _T("wt"));
            printf("fh is %d\n",fh);
            fflush(stdout);
        }
        m_toconsole = true;
    }
	else 
	{
        m_toconsole = false;
    }
}


void CLog::SetLevel(int level)
{
    m_level = level;
}

void CLog::SetFile(LPCTSTR filename, bool append) 
{
	if (m_filename != NULL)
		free(m_filename);
	m_filename = _tcsdup(filename);
	m_append = append;
	if (m_tofile)
		OpenFile();
}

void CLog::OpenFile()
{
	// Is there a file-name?
	if (m_filename == NULL)
	{
        m_todebug = true;
        m_tofile = false;
        Print(0, _T("Error opening log file\n"));
		return;
	}
	
    m_tofile  = true;
    
	// If there's an existing log and we're not appending then move it
	if (!m_append)
	{
		// Build the backup filename
		TCHAR *backupfilename = new TCHAR[_tcslen(m_filename)+5];
		if (backupfilename)
		{
			_tcscpy_s(backupfilename, _tcslen(m_filename),m_filename);
			_tcscat_s(backupfilename, 3,_T(".bak"));
			// Attempt the move and replace any existing backup
			// Note that failure is silent - where would we log a message to? ;)
			MoveFileEx(m_filename, backupfilename, MOVEFILE_REPLACE_EXISTING);
			delete [] backupfilename;
		}
	}
	
	CloseFile();
	
    // If filename is NULL or invalid we should throw an exception here
    m_hlogfile = CreateFile(
        m_filename,  GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL  );
    
    if (m_hlogfile == INVALID_HANDLE_VALUE) 
	{
        // We should throw an exception here
        m_todebug = true;
        m_tofile = false;
        Print(0, _T("Error opening log file %s\n"), m_filename);
    }
    if (m_append) 
	{
        SetFilePointer( m_hlogfile, 0, NULL, FILE_END );
    } 
	else 
	{
        SetEndOfFile( m_hlogfile );
    }
#ifdef _UNICODE
	DWORD filesize = 0;
	GetFileSize(m_hlogfile,&filesize);
	if(filesize <= 0)
	{
		char sUnicode[2];
		sUnicode[0] = (char)0xFF;
		sUnicode[1] = (char)0xFE;
		DWORD byteswritten;
		WriteFile(m_hlogfile,sUnicode,2*sizeof(char), &byteswritten, NULL); 
	}
#endif
}

// if a log file is open, close it now.
void CLog::CloseFile() 
{
    if (m_hlogfile != NULL) 
	{
        CloseHandle(m_hlogfile);
        m_hlogfile = NULL;
    }
}

void CLog::ReallyPrintLine(LPCTSTR line) 
{
    if (m_todebug) OutputDebugString(line);
    if (m_toconsole) 
	{
        DWORD byteswritten;
        WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), line, _tcslen(line), &byteswritten, NULL); 
    };
    if (m_tofile && (m_hlogfile != NULL))
	{
        DWORD byteswritten;
        WriteFile(m_hlogfile, line, _tcslen(line)*sizeof(TCHAR), &byteswritten, NULL); 
    }
}

void CLog::ReallyPrint(LPCTSTR format, va_list ap) 
{
	time_t current = time(0);
	if (current != m_lastLogTime) 
	{
		m_lastLogTime = current;
		ReallyPrintLine(_tctime(&m_lastLogTime));
	}
	
	// - Write the log message, safely, limiting the output buffer size
	TCHAR line[LINE_BUFFER_SIZE];
    _vsntprintf_s(line, LINE_BUFFER_SIZE, format, ap);
	ReallyPrintLine(line);
}

//////////////////////////////////////////////////////////////////////
// CLogHelper Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CLogHelper::GetDllModulePath(HINSTANCE hInstance,TCHAR *sPath,int reverse_level)
{
	DWORD nret = GetModuleFileName(hInstance,sPath,MAX_PATH);
	sPath[nret] = 0;
	TCHAR* p = _tcsrchr(sPath,_T('\\')); //bin/
	if(p!=NULL) *p = 0;
	for(int i=0;i<reverse_level;i++)
	{
		p = _tcsrchr(sPath,_T('\\')); //bin/
		if(p!=NULL) *p = 0;
	}
}

void CLogHelper::GetExeModulePath(TCHAR *sPath,int reverse_level)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	_ASSERT(hInstance != NULL);
	GetDllModulePath(hInstance,sPath,reverse_level);
}

void CLogHelper::SetLogPath(CLog* pLog,LPCTSTR sPlatPath,LPCTSTR sLogPath,int level)
{
	CString filename = sPlatPath;
	filename += sLogPath;
	CFilePathHelper::CreateFolder(filename,TRUE);
	
	pLog->SetFile(filename,true);
	pLog->SetLevel(level);
	pLog->SetMode(CLog::ToFile);
}
