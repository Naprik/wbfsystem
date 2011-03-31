#include "stdafx.h"
#include "hardctrl.h"
#include "DBTeacherDev.h"
#include "RQWhere.h"
#include "Base.h"

CDBTeacherDev::CDBTeacherDev()
{
	m_colMap->SetAt("YDID",&m_ydid);
	m_colMap->SetAt("TEACHID",&m_teacherid);
	m_colMap->SetAt("TEACHERNAME",&m_teachername);
	m_colMap->SetAt("TEACHDEVID",&m_devmac);
}

CDBTeacherDev::~CDBTeacherDev()
{
	Close();
}
CString CDBTeacherDev::GetExePath()
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
BOOL CDBTeacherDev::Connect()
{
	CJahCoverCapture capture;
	capture.m_bmpFile =GetExePath()+"stusys.udl" ;
	BOOL ret= m_connect.OpenUDLFile(capture.m_bmpFile);
	if ( ret )
		ret = CRQDb::Open("ydteacherdev", &m_connect);	
	return ret;  
}

void CDBTeacherDev::Close()
{
	CRQDb::Close();
}

BOOL CDBTeacherDev::Move(BOOL isNext)
{
   return CRQDb::Move(isNext);
}
int CDBTeacherDev::GetAccount()
{
   return GetRecordCount();
}
BOOL CDBTeacherDev::LoadByAll()
{
	CRQWhere w;
	w.Add("YDID","0", ">=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBTeacherDev::Add()
{
	return CRQDb::Add( );
}
BOOL CDBTeacherDev::Delete(int num)
{
	return CRQDb::Delete(num);
}
BOOL CDBTeacherDev::LoadByDevid(CString devid)
{
	CRQWhere w;
	w.Add("TeachID",CRQWhere::GetString(devid),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBTeacherDev::LoadByDevmac(CString devmac)
{
	CRQWhere w;
	w.Add("TeachdevID",CRQWhere::GetString(devmac),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
/*BOOL CDBTeacherDev::Update(int id, CString devid, CString teachername, CString devmac)
{
	CString strSQL;
	strSQL.Format("update ydteacherdev set Teachid = '%s',Teachname = '%s',Devid = '%s' where YDID = %d", devid, teachername, devmac, id);
	BOOL ret = RunSql(strSQL);
	return ret;
}*/
BOOL CDBTeacherDev::Update(int id)
{
	return CRQDb::Update(id);
}