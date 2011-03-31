// DBDepartment.cpp: implementation of the CDBDepartment class.
//
//////////////////////////////////////////////////////////////////////
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "stdafx.h"
#include "hardctrl.h"
#include "DBDepartment.h"
#include "RQWhere.h"
#include "Base.h"

/************************************************************************
以下四个方法需要根据具体的应用进行修改
************************************************************************/
//CDBDepartment 控制类

CDBDepartment::CDBDepartment()
{
	//在这里设置数据成员和表的影射关系
	m_colMap->SetAt("YDID",&m_ydid);
	m_colMap->SetAt("DEPARTID",&m_departid);
	m_colMap->SetAt("DEPARTNAME",&m_departname);
	m_colMap->SetAt("PRESIDENT",&m_persident);
	m_colMap->SetAt("PRESIDENTTEL",&m_persidenttel);
}

CDBDepartment::~CDBDepartment()
{
	Close();
}
CString CDBDepartment::GetExePath()
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
BOOL CDBDepartment::Connect()
{
	CJahCoverCapture capture;
	CBase base;
	capture.m_bmpFile =base.GetExePath()+"stusys.udl" ;
	BOOL ret= m_connect.OpenUDLFile(capture.m_bmpFile);
	if ( ret )
		ret = CRQDb::Open("yddepartment", &m_connect);	
	return ret;
}

void CDBDepartment::Close()
{
	CRQDb::Close();
}

int CDBDepartment::GetAccount()
{
	return GetRecordCount();
}

BOOL CDBDepartment::LoadByAll()
{
	CRQWhere w;
	w.Add("YDID","0", ">=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}

BOOL CDBDepartment::Add()
{
	return CRQDb::Add( );
}

BOOL CDBDepartment::Move(BOOL isNext)
{
	 return CRQDb::Move(isNext);
}
void CDBDepartment::Delete(int num)
{
	CRQDb::Delete(num);
}
BOOL CDBDepartment::LoadByDepart(CString depart)
{
	CRQWhere w;
	w.Add("DEPARTNAME",CRQWhere::GetString(depart),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBDepartment::LoadByDepartID(CString departID)
{
	CRQWhere w;
	w.Add("DEPARTID",CRQWhere::GetString(departID),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBDepartment::LoadByClass(CString strClass)
{
	CRQWhere w;
	w.Add("Class",CRQWhere::GetString(strClass),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBDepartment::LoadByClassID(CString strClassID)
{
	CRQWhere w;
	w.Add("ClassID",CRQWhere::GetString(strClassID),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBDepartment::Update(CString strDepartID, CString strnewDepartID, CString strDepar, CString strDeparthead, CString strDeparttel)
{
	CString strSQL;
	strSQL.Format("update yddepartment set DEPARTID = '%s',DEPARTNAME = '%s',PRESIDENT = '%s',PRESIDENTTEL = '%s' where DEPARTID = '%s'", strnewDepartID, strDepar, strDeparthead, strDeparttel, strDepartID);
	BOOL ret = RunSql(strSQL);
	return ret;
//	return CRQDb::Update(id);
}
BOOL CDBDepartment::Update(int index)
{
	return CRQDb::Update(index);
}
BOOL CDBDepartment::DeleteDepart(CString strDepartID)
{
	CString strSQL;
	strSQL.Format("delete from yddepartment where DEPARTID = '%s'",strDepartID);
	BOOL ret = RunSql(strSQL);
	return ret;
}
BOOL CDBDepartment::DeleteClass(CString strClassID)
{
	CString strSQL;
	strSQL.Format("delete from yddepartment where ClassID = '%s'",strClassID);
	BOOL ret = RunSql(strSQL);
	return ret;
}

BOOL CDBDepartment::LoadByDepartid(CString sDepartid)
{
	CRQWhere w;
	w.Add("DEPARTID",CRQWhere::GetString(sDepartid),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}