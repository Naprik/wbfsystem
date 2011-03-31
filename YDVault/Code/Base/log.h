#pragma once

#pragma warning (disable:4786)


#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>


#define PLMLOG(s)	_T(" ---- " s)

enum ERRORSERVERLEVEL{SERVER_DEBUG=1,SERVER_LOW,SERVER_MIDDLE,SERVER_HIGH};

class BASE_DLL_API CLog  
{
public:
    // Logging mode flags:
    static const int ToDebug;
    static const int ToFile;
    static const int ToConsole;

    // Create a new log object.
    // Parameters as follows:
    //    mode     - specifies where output should go, using combination
    //               of flags above.
    //    level    - the default level
    //    filename - if flag Log::ToFile is specified in the type,
    //               a filename must be specified here.
    //    append   - if logging to a file, whether or not to append to any
    //               existing log.
    CLog();
    virtual ~CLog();	

    void Print(int level, LPCTSTR format, ...) {
        if (level < m_level) return;
        va_list ap;
        va_start(ap, format);
        ReallyPrint(format, ap);
        va_end(ap);
    }
    
    // Change the log level
    void SetLevel(int level);

    // Change the logging mode
    void SetMode(int mode);

    // Change or set the logging filename.  This only has an effect if
	// the log mode includes ToFile
    void SetFile(LPCTSTR filename, bool append = false);

private:
	void ReallyPrintLine(LPCTSTR line);
    void ReallyPrint(LPCTSTR format, va_list ap);
	void OpenFile();
    void CloseFile();

    bool	m_tofile, m_todebug, m_toconsole;
    int		m_level;
    HANDLE	m_hlogfile;
	LPTSTR	m_filename;
	bool	m_append;
	time_t	m_lastLogTime;
};

class BASE_DLL_API CLogHelper
{
public:
	CLogHelper(){}
	virtual ~CLogHelper(){}

	void GetDllModulePath(HINSTANCE hInstance,TCHAR *sPath,int reverse_level=1);
	void GetExeModulePath(TCHAR *sPath,int reverse_level=1);

	void SetLogPath(CLog* pLog,LPCTSTR sPlatPath,LPCTSTR sLogPath,int level=SERVER_DEBUG);
};