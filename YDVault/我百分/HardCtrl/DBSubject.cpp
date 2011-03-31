#include "stdafx.h"
#include "hardctrl.h"
#include "DBSubject.h"
#include "RQWhere.h"
#include "Base.h"

CDBSubject::CDBSubject()
{
	//在这里设置数据成员和表的影射关系
	m_colMap->SetAt("YDID",&m_ydid);
	m_colMap->SetAt("SUBJECTID",&m_subjectid);
	m_colMap->SetAt("SUBJECT",&m_subject);
}

CDBSubject::~CDBSubject()
{
	Close();
}
CString CDBSubject::GetExePath()
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
BOOL CDBSubject::Connect()
{
	CJahCoverCapture capture;
	capture.m_bmpFile =GetExePath()+"stusys.udl" ;
	BOOL ret= m_connect.OpenUDLFile(capture.m_bmpFile);
	if ( ret )
		ret = CRQDb::Open("ydsubject", &m_connect);	
	return ret;  
}

void CDBSubject::Close()
{
	CRQDb::Close();
}

BOOL CDBSubject::Move(BOOL isNext)
{
   return CRQDb::Move(isNext);
}
int CDBSubject::GetAccount()
{
   return GetRecordCount();
}
BOOL CDBSubject::LoadByAll()
{
	CRQWhere w;
	w.Add("YDID","0", ">=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBSubject::Add()
{
	return CRQDb::Add();
}
BOOL CDBSubject::Delete(int num)
{
	return CRQDb::Delete(num);
}
/*BOOL CDBSubject::Update(int id, CString subject)
{
	CString strSQL;
	strSQL.Format("update ydsubject set Studsubject = '%s' where YDID = %d", subject, id);
	BOOL ret = RunSql(strSQL);
	return ret;
}*/
BOOL CDBSubject::Update(int id)
{
	return CRQDb::Update(id);
}
BOOL CDBSubject::LoadBySubject(CString subject)
{
	CRQWhere w;
	w.Add("SUBJECT",CRQWhere::GetString(subject),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBSubject::LoadBySubjectid(CString subjectid)
{
	CRQWhere w;
	w.Add("SUBJECTID",CRQWhere::GetString(subjectid),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}