// BackUp.cpp: implementation of the CBackUp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hardctrl.h"
#include "BackUp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBackUp::CBackUp()
{
	m_filepath = "";
}

CBackUp::~CBackUp()
{

}
CString GetExePath()
{
	CString file;
	char process[MAX_PATH];
	GetModuleFileName(NULL, process, MAX_PATH);
    for ( int i = strlen(process);i>0 ;i --)
	if ( process[i] =='\\')
	{
		process[i+1]='\0';
		break;
	}
	file.Format(process);
	return file;
}
void CBackUp::Backup(CString strTitle, CString strDetails)
{

	CString  FilePath = GetExePath()+ "back.bak";
/*	if(m_filepath == "")
	{
		CFileDialog dlg(FALSE,"*.bak",);
		if(dlg.DoModal()==IDOK)
		{
			m_filepath=dlg.GetPathName();
		} 
	}*/
	CString strSection       = strTitle;
    CString strSectionKey    = strTitle;
    CString strValue       = strDetails;
	WritePrivateProfileString (strSection,strSectionKey, strValue, FilePath);		
}

void  CBackUp::Createfile(CString strTitle, CString strDetails)
{
	CString  FilePath = GetExePath()+ "temp.ini";

	CString strSection       = strTitle;
    CString strSectionKey    = strTitle;
    CString strValue       = strDetails;
	WritePrivateProfileString (strSection,strSectionKey, strValue, FilePath);
}

void  CBackUp::Createfile(CString strTitle, CString strKey, CString strDetails)
{
	CString  FilePath = GetExePath()+ "temp.ini";
	
	CString strSection       = strTitle;
    CString strSectionKey    = strKey;
    CString strValue       = strDetails;
	WritePrivateProfileString (strSection,strSectionKey, strValue, FilePath);	
}

CString  CBackUp::Load(CString strTitle, CString strFilename)
{
	//CString  FilePath = GetExePath()+ "back.bak";

	CString strSection       = strTitle;
	CString strSectionKey    = strTitle;
	CString strValue       = _T("");
	char inBuf[65535];
	memset(inBuf,0,sizeof(inBuf));
	GetPrivateProfileString (strSection,strSectionKey, NULL, inBuf, 65535, strFilename); 
	strValue=inBuf;
	return strValue;
}

CString CBackUp::Load(CString strTitle, CString strKey, CString strFilename)
{
	CString strSection       = strTitle;
	CString strSectionKey    = strKey;
	CString strValue       = _T("");
	char inBuf[65535];
	memset(inBuf,0,sizeof(inBuf));
	GetPrivateProfileString (strSection,strSectionKey, NULL, inBuf, 65535, strFilename); 
	strValue=inBuf;
	return strValue;
}

void CBackUp::Remove()
{
	CString  FilePath = GetExePath()+ "back.bak";
	CJahFile    m_file;
	m_file.Remove(FilePath);
}
void CBackUp::Remove(BOOL flag)
{
	CString  FilePath = GetExePath()+ "temp.ini";
	CJahFile    m_file;
	m_file.Remove(FilePath);	
}