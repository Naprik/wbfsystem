
#include "stdafx.h"
#include "hardctrl.h"
#include "DBLogin.h"
#include "RQWhere.h"



CDBLogin::CDBLogin()
{
	m_colMap->SetAt("YDID",&m_ydid);
	m_colMap->SetAt("Uesr",&m_user);
	m_colMap->SetAt("Password",&m_pass);
	m_colMap->SetAt("Authority",&m_authority);
}

CDBLogin::~CDBLogin()
{
	Close();
}
CString CDBLogin::GetExePath()
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
BOOL CDBLogin::Connect()
{
	CJahCoverCapture capture;
	capture.m_bmpFile =GetExePath()+"stusys.udl" ;
	BOOL ret= m_connect.OpenUDLFile(capture.m_bmpFile);
	if ( ret )
		ret = CRQDb::Open("ydlogin", &m_connect);	
	return ret;  
}
void CDBLogin::Close()
{
	CRQDb::Close();
}
void CDBLogin::Delete(int num)
{
	CRQDb::Delete(num);
}
BOOL CDBLogin::LoadByAll()
{
	CRQWhere w;
	w.Add("YDID",CRQWhere::GetLong(0), ">=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBLogin::Move(BOOL isNext)
{
	return CRQDb::Move(isNext);
}
int CDBLogin::GetAccount()
{
	return GetRecordCount();
}