// DBClass.cpp: implementation of the CDBClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hardctrl.h"
#include "DBClass.h"
#include "RQWhere.h"
#include "Base.h"

CDBClass::CDBClass()
{
	//在这里设置数据成员和表的影射关系
	m_colMap->SetAt("YDID",&m_ydid);
	m_colMap->SetAt("CLASSID",&m_classid);
	m_colMap->SetAt("DEPARTID",&m_departid);
	m_colMap->SetAt("CLASSNAME",&m_classname);
	m_colMap->SetAt("HEAD",&m_head);
	m_colMap->SetAt("HEADTEL",&m_headtel);
}

CDBClass::~CDBClass()
{
	Close();
}
void CDBClass::Close()
{
	CRQDb::Close();
}

BOOL CDBClass::Connect()
{
	CJahCoverCapture capture;
	CBase base;
	capture.m_bmpFile =base.GetExePath()+"stusys.udl" ;
	BOOL ret= m_connect.OpenUDLFile(capture.m_bmpFile);
	if ( ret )
		ret = CRQDb::Open("ydclass", &m_connect);	
	return ret; 
}

BOOL CDBClass::Delete(int num)
{
	return CRQDb::Delete(num);
}

int CDBClass::GetAccount()
{
    return GetRecordCount();
}

BOOL CDBClass::Move(BOOL isNext)
{
    return CRQDb::Move(isNext);
}

BOOL CDBClass::Add()
{
	return CRQDb::Add();
}

BOOL CDBClass::LoadByAll()
{
	CRQWhere w;
	w.Add("YDID","0", ">=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}

BOOL CDBClass::LoadByDepartid(CString sDepartid)
{
	CRQWhere w;
	w.Add("DEPARTID",CRQWhere::GetString(sDepartid),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBClass::LoadByClassname(CString sClassname)
{
	CRQWhere w;
	w.Add("CLASSNAME",CRQWhere::GetString(sClassname),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}

BOOL CDBClass::LoadByClassid(CString sClassid)
{
	CRQWhere w;
	w.Add("CLASSID",CRQWhere::GetString(sClassid),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);	
}

BOOL CDBClass::DeleteClassByDepartID(CString sDepartid)
{
	CString strSQL;
	strSQL.Format("delete from ydclass where DEPARTID = '%s'",sDepartid);
	BOOL ret = RunSql(strSQL);
	return ret;
}

BOOL CDBClass::DeleteClassByclassid(CString sClassid)
{
	CString strSQL;
	strSQL.Format("delete from ydclass where CLASSID = '%s'",sClassid);
	BOOL ret = RunSql(strSQL);
	return ret;
}
