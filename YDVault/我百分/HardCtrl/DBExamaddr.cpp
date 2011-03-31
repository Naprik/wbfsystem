#include "stdafx.h"
#include "hardctrl.h"
#include "DBExamaddr.h"
#include "RQWhere.h"
#include "Base.h"

CDBExamaddr::CDBExamaddr()
{
	//在这里设置数据成员和表的影射关系
	m_colMap->SetAt("YDID",&m_ydid);
	m_colMap->SetAt("ADDRID",&m_addrid);
	m_colMap->SetAt("EXAMADDR",&m_examaddr);
}

CDBExamaddr::~CDBExamaddr()
{
	Close();
}
CString CDBExamaddr::GetExePath()
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
BOOL CDBExamaddr::Connect()
{
	CJahCoverCapture capture;
	CBase base;
	capture.m_bmpFile =base.GetExePath()+"stusys.udl" ;
	BOOL ret= m_connect.OpenUDLFile(capture.m_bmpFile);
	if ( ret )
		ret = CRQDb::Open("ydexamaddr", &m_connect);	
	return ret; 
}
void CDBExamaddr::Close()
{
	CRQDb::Close();
}
BOOL CDBExamaddr::Move(BOOL isNext)
{
   return CRQDb::Move(isNext);
}
int CDBExamaddr::GetAccount()
{
   return GetRecordCount();
}
BOOL CDBExamaddr::LoadByAll()
{
	CRQWhere w;
	w.Add("YDID","0", ">=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBExamaddr::Add()
{
	return CRQDb::Add();
}
BOOL CDBExamaddr::Delete(int num)
{
	return CRQDb::Delete(num);
}
BOOL CDBExamaddr::Update(int id)
{
	return CRQDb::Update(id);
}
BOOL CDBExamaddr::LoadByAddr(CString addr)
{
	CRQWhere w;
	w.Add("EXAMADDR",CRQWhere::GetString(addr),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBExamaddr::LoadByAddrID(CString addrID)
{
	CRQWhere w;
	w.Add("ADDRID",CRQWhere::GetString(addrID),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}