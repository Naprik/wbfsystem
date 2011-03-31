// DBStuInfo.cpp: implementation of the CDBStuInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hardctrl.h"
#include "DBStuInfo.h"
#include "RQWhere.h"
#include "Base.h"


/************************************************************************
以下四个方法需要根据具体的应用进行修改
************************************************************************/
//CDBStuInfo 控制类

CDBStuInfo::CDBStuInfo()
{	
	//在这里设置数据成员和表的影射关系
	m_colMap->SetAt("YDID",&m_ydid);
	m_colMap->SetAt("STUID",&m_stuid);
	m_colMap->SetAt("STUNAME", &m_stuname);
	m_colMap->SetAt("STUGENDER", &m_stugender);
	m_colMap->SetAt("STUAGE", &m_stuage);
	m_colMap->SetAt("STUDEPARTID", &m_studeparid);
	m_colMap->SetAt("STUCLASSID", &m_stuclassid);
	m_colMap->SetAt("STUTEL", &m_stutel);
	m_colMap->SetAt("STUEMAIL", &m_stuemail);
	m_colMap->SetAt("STUPHOTO", &m_stuphoto);
	m_colMap->SetAt("STUDEVID", &m_devid);
}

CDBStuInfo::~CDBStuInfo()
{
	Close();
}
CString CDBStuInfo::GetExePath()
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
BOOL CDBStuInfo::Connect()
{
	CJahCoverCapture capture;
	CBase base;
	capture.m_bmpFile =base.GetExePath()+"stusys.udl" ;
	BOOL ret= m_connect.OpenUDLFile(capture.m_bmpFile);
	if ( ret )
		ret = CRQDb::Open("ydstuinfo", &m_connect);	
	return ret;
}

void CDBStuInfo::Close()
{
	CRQDb::Close();
}

BOOL CDBStuInfo::Delete(int num)
{
	return CRQDb::Delete(num);
}

int CDBStuInfo::GetAccount()
{
    return GetRecordCount();
}

BOOL CDBStuInfo::Move(BOOL isNext)
{
    return CRQDb::Move(isNext);
}

/*BOOL CDBStuInfo::Add(CString stuid, CString stuname, CString stusex, CString studepart, CString stuclass, CString stutel ,CString stuemail, CString stuphoto)
{
	//return CRQDb::Add( );
	CString strSQL;
	strSQL.Format("insert into ydstudinfo(Studid, Studname, Studgender, Studdepartment, Studclass, Studtel, Studemail, Studphoto) values ('%s','%s','%s','%s','%s','%s','%s','%s')",stuid, stuname, stusex, studepart, stuclass, stutel, stuemail, stuphoto);
	BOOL ret = RunSql(strSQL);
	return ret;
}*/
BOOL CDBStuInfo::Add()
{
	return CRQDb::Add();
}

BOOL CDBStuInfo::LoadByAll()
{
	CRQWhere w;
	w.Add("YDID","0", ">=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}

BOOL CDBStuInfo::LoadByStuid(CString id)
{
	CRQWhere w;
	w.Add("STUID",CRQWhere::GetString(id),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBStuInfo::LoadByStuname(CString name)
{
	CRQWhere w;
	w.Add("STUNAME",CRQWhere::GetString(name),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
/*BOOL CDBStuInfo::LoadByClass(CString strClass)
{
	CRQWhere w;
	w.Add("Studclass",CRQWhere::GetString(strClass),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}*/
BOOL CDBStuInfo::LoadByQuery(CString num, CString name, CString depart)
{
	CRQWhere w, w1, w2, w3, w12, w123;
	if(num == "")
		w1.Add("YDID","0", ">=");
	else
		w1.Add("Studid",CRQWhere::GetString(num),"=" );
	if(name == "")
		w2.Add("YDID","0", ">=");
	else
		w2.Add("Studname",CRQWhere::GetString(name),"=");
	if(depart == "")
		w3.Add("YDID","0", ">=");
	else
		w3.Add("Studdepartment", CRQWhere::GetString(depart), "=");

	w12 = w1 & w2;
	w123 = w12 & w3;
	CString tmp = w123.GetClausePlain();
	return Load(tmp);
}

BOOL  CDBStuInfo::Update(int id, CString studid, CString stuname, CString studgender, CString studpart, 
						 CString studclass, CString studtel, CString studemail, CString studphoto)
{
	CString strSQL;
	strSQL.Format("update ydstudinfo set Studid = '%s',Studname = '%s',Studgender = '%s',Studdepartment = '%s', Studclass = '%s', Studtel = '%s', Studemail = '%s',Studphoto = '%s' where YDID = %d", studid, stuname, studgender, studpart, studclass, studtel, studemail, studphoto, id);
	BOOL ret = RunSql(strSQL);
	return ret;
}
BOOL CDBStuInfo::Update(int index)
{
	return CRQDb::Update(index);
}

BOOL CDBStuInfo::LoadByDepart(CString depart)
{
	CRQWhere w;
	w.Add("Studdepartment",CRQWhere::GetString(depart),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBStuInfo::LoadByClass(CString strClass)
{
	CRQWhere w;
	w.Add("Studclass",CRQWhere::GetString(strClass),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBStuInfo::LoadByDev(CString strDev)
{
	CRQWhere w;
	w.Add("StudevID",CRQWhere::GetString(strDev),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBStuInfo::LoadByStu(CString stuid, CString stuname)
{
	CRQWhere w, w1, w2, w3;
	if(stuid == "")
		w1.Add("YDID","0", ">=");
	else
		w1.Add("STUID",CRQWhere::GetString(stuid),"=" );
	if(stuname == "")
		w2.Add("YDID","0", ">=");
	else
		w2.Add("STUNAME",CRQWhere::GetString(stuname),"=");
	w = w1 & w2;
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBStuInfo::LoadByDepClass(CString strDepart, CString strClass)
{
	CRQWhere w, w1, w2;
	if(strDepart == "")
		w1.Add("YDID","0", ">=");
	else
		w1.Add("Studdepartment",CRQWhere::GetString(strDepart),"=" );
	if(strClass == "")
		w2.Add("YDID","0", ">=");
	else
		w2.Add("Studclass",CRQWhere::GetString(strClass),"=");
	w = w1 & w2;
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}

BOOL CDBStuInfo::LoadByClassid(CString strClassid)
{
	CRQWhere w;
	w.Add("STUCLASSID",CRQWhere::GetString(strClassid),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}