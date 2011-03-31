#include "stdafx.h"
#include "JahFile.h"

//##ModelId=4141A18702BC
CJahFile::CJahFile()
{
	m_handle = NULL;
}
//##ModelId=4141A18702B8
CJahFile::~CJahFile()
{ 
}
//##ModelId=4141A18702BD
BOOL CJahFile::Open(CString file,BOOL isRead)
{
	if ( IsLocked())
		return FALSE;
	BOOL ret = FALSE;
	CFileFind  find;
	if ( find.FindFile(file))
	{
		if ( isRead )
			ret = m_file.Open(file,CFile::modeRead|CFile::shareDenyNone);
        else
			ret = m_file.Open(file,CFile::modeWrite|CFile::shareDenyNone);
	}
    else
	{
		if ( isRead )
			ret = m_file.Open(file,CFile::modeCreate|CFile::modeRead|CFile::shareDenyNone);
        else
			ret = m_file.Open(file,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone);
	}
	find.Close();
    if ( !isRead )
		SeekTo( TRUE );
	Lock();
	return ret;
}
//##ModelId=4141A18702C0
BOOL CJahFile::Lock()
{
	return TRUE;
	BOOL ret = FALSE;
	m_handle=::CreateMutex(NULL,FALSE,JAH_FILE_SAVE_LOCK);
	if(GetLastError()==ERROR_ALREADY_EXISTS)	
		ret = FALSE;
	else
		ret = TRUE;
	return ret;
}
//##ModelId=4141A18702C1
BOOL CJahFile::UnLock()
{
	return TRUE;
	return CloseHandle(m_handle);
}
//##ModelId=4141A18702C6
BOOL CJahFile::IsLocked()
{
	return FALSE;
	BOOL ret = FALSE;
	HANDLE   handle;
	handle=::CreateMutex(NULL,FALSE,JAH_FILE_SAVE_LOCK);
	if(GetLastError()==ERROR_ALREADY_EXISTS)	
		ret = TRUE;
	else
		CloseHandle(handle);
	return ret;
}
void CJahFile::Close()
{
	UnLock();
	try
	{
		m_file.Close();
	}
	catch(...)
	{
	}
}
BOOL CJahFile::Write(void *buf, int len)
{
	BOOL ret = FALSE;
	try
	{
		m_file.Write(buf,len);
		ret = TRUE;
	}
	catch(...)
	{
	}
	return ret;
}

int CJahFile::Read(void *buf, int len)
{
	int ret = -1;
	try
	{
		ret = m_file.Read(buf , len);
	}
	catch(...)
	{
	}
	return ret;
}
BOOL CJahFile::ReadStruct(void *buf, int len)
{
	BOOL ret = FALSE;
	int  size = len;
	if ( Read(buf,size) == size)
		ret = TRUE;
	return ret;
}
BOOL CJahFile::WriteStruct(void *buf, int len)
{
	return Write(buf ,len);
}
BOOL CJahFile::WriteObj(CJahObject &obj)
{
	return Write(&obj , obj.GetObjSize());
}
BOOL CJahFile::ReadObj(CJahObject *obj)
{
	BOOL ret = FALSE;
	int  size = obj->GetObjSize();
	if ( Read(obj,size) == size)
		ret = TRUE;
	return ret;
}
void CJahFile::SeekTo(BOOL isEnd)
{
	if ( isEnd )
		m_file.SeekToEnd();
	else
		m_file.SeekToBegin();
}

BOOL CJahFile::Remove(CString fileName)
{
	CFileFind find;
	if (!find.FindFile(fileName))
		return TRUE;
	BOOL ret = FALSE;
	try
	{
		CFile::Remove(fileName);
		ret = TRUE;
	}
	catch(...)
	{
	}
	return ret;
}
CString CJahFile::ReadString()
{
	CString ret = "";
    int len = 0 ;
	if ( Read(&len,sizeof(len)) == sizeof(len))
	{
		if (( len > 0 ) && (len < 65535))
		{
			char buf[65535] = { 0 };
			if ( Read(buf,len) == len )
			{
				ret = buf;
			}
		}
	}
	return ret;
}
BOOL CJahFile::WriteString(CString str)
{
	int len = str.GetLength();
	BOOL ret = Write(&len,sizeof(len));
	if ( ret )
         ret = Write(str.GetBuffer(0),len);
	return ret;
}

BOOL CJahFile::Seek(long pos)
{
	BOOL ret = FALSE;
	SeekTo(FALSE);
    if ( m_file.Seek(pos,CFile::begin) >=0 )
		ret = TRUE;
	else
		ret = FALSE;
	return ret;
}

CString CJahFile::ReadFile(CString file)
{
	char buf[1024]={0};
	CString ret = "";
	CJahFile io;
	if ( io.Open(file))
	{	
		io.SeekTo(FALSE);
		int bytes = io.Read(buf,sizeof(buf));
		if ( (bytes >0 ) &&( bytes < sizeof(buf)))
		{
			buf[bytes] = '\0';
			ret = buf;
		}
		io.Close();
	}
	return ret;
}

BOOL CJahFile::WriteFile(CString file, CString text)
{
	char buf[1024]={0};
	if ( text.GetLength() > (sizeof(buf) -1 ))
		text = text.Left(sizeof(buf) - 1);
	strcpy(buf,text.GetBuffer(0));
	BOOL ret = FALSE;
	CJahFile io;
	CJahFile::Remove(file);
	if ( io.Open(file,FALSE))
	{
		io.SeekTo(FALSE);
		ret = io.WriteStruct(buf,strlen(buf));
		io.Close();
	}
	return ret;
}

DWORD CJahFile::GetLength()
{ 
	return m_file.GetLength();
}
