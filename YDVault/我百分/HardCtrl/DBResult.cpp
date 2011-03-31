#include "stdafx.h"
#include "hardctrl.h"
#include "DBResult.h"
#include "RQWhere.h"
#include "Base.h"

CDBResult::CDBResult()
{
	//在这里设置数据成员和表的影射关系
	m_colMap->SetAt("YDID",&m_ydid);
	m_colMap->SetAt("STUID",&m_stuid);
	m_colMap->SetAt("PAPERID",&m_paperid);
	m_colMap->SetAt("STUSTART",&m_starttime);
	m_colMap->SetAt("STUEND",&m_endtime);
	m_colMap->SetAt("ADDRID",&m_addrid);
	m_colMap->SetAt("STUANSWER",&m_stuanswer);
}

CDBResult::~CDBResult()
{
	Close();
}
CString CDBResult::GetExePath()
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
BOOL CDBResult::Connect()
{
	CJahCoverCapture capture;
	capture.m_bmpFile =GetExePath()+"stusys.udl" ;
	BOOL ret= m_connect.OpenUDLFile(capture.m_bmpFile);
	if ( ret )
		ret = CRQDb::Open("ydresult", &m_connect);	
	return ret;  
}
void CDBResult::Close()
{
	CRQDb::Close();
}
void CDBResult::Delete(int num)
{
	CRQDb::Delete(num);
}

int CDBResult::GetAccount()
{
    return GetRecordCount();
}

BOOL CDBResult::Move(BOOL isNext)
{
    return CRQDb::Move(isNext);
}
BOOL CDBResult::Add()
{
	return CRQDb::Add();
}
BOOL CDBResult::Update(int id)
{
	return CRQDb::Update(id);
}
BOOL CDBResult::Deletepaper(int paperid)
{
	CString strSQL;
	strSQL.Format("delete from ydresult where PaperID=%d",paperid);
	BOOL ret = RunSql(strSQL);
	return ret;	
}
BOOL CDBResult::LoadByStuid(CString id)
{
	CRQWhere w;
	w.Add("StudID",CRQWhere::GetString(id),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBResult::LoadByQuery(CString stuid,int paperid)
{
	CRQWhere w,w1, w2;
	if(stuid == "")
		w1.Add("YDID","0", ">=");
	else
		w1.Add("STUID",CRQWhere::GetString(stuid),"=" );

	if(paperid == 0)
		w2.Add("YDID","0", ">=");
	else
		w2.Add("PAPERID",CRQWhere::GetLong(paperid) );
	w = w1 & w2;
	CString tmp = w.GetClausePlain();
	return Load(tmp);		
}
BOOL CDBResult::LoadByAll()
{
	CRQWhere w;
	w.Add("YDID","0", ">=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBResult::LoadByPaperID(int id)
{
	CRQWhere w;
	w.Add("YDID",CRQWhere::GetLong(id));
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBResult::LoadByRes(CString stuid, CString stime)
{
	CRQWhere w1, w2, w;

	w1.Add("StudID",CRQWhere::GetString(stuid),"=" );
	w2.Add("Studstart",CRQWhere::GetString(stime),"=" );
	w = w1 & w2;
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}