#include "stdafx.h"
#include "hardctrl.h"
#include "DBStdanswer.h"
#include "RQWhere.h"
#include "Base.h"


CDBStdanswer::CDBStdanswer()
{
	//在这里设置数据成员和表的影射关系
	m_colMap->SetAt("YDID",&m_ydid);
	m_colMap->SetAt("PAPERID",&m_paperid);
	m_colMap->SetAt("FRAGE",&m_frage);
	m_colMap->SetAt("RES",&m_res);
	m_colMap->SetAt("SCORE",&m_score);
}

CDBStdanswer::~CDBStdanswer()
{
	Close();
}
CString CDBStdanswer::GetExePath()
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
BOOL CDBStdanswer::Connect()
{
	CJahCoverCapture capture;
	capture.m_bmpFile =GetExePath()+"stusys.udl" ;
	BOOL ret= m_connect.OpenUDLFile(capture.m_bmpFile);
	if ( ret )
		ret = CRQDb::Open("ydstdanswer", &m_connect);	
	return ret;  
}
void CDBStdanswer::Close()
{
	CRQDb::Close();
}
BOOL CDBStdanswer::Delete(int num)
{
	return CRQDb::Delete(num);
}
BOOL CDBStdanswer::Deletepaper(int paperid)
{
	CString strSQL;
	strSQL.Format("delete  from ydstdanswer where PaperID=%d",paperid);
	BOOL ret = RunSql(strSQL);
	return ret;	
}
int CDBStdanswer::GetAccount()
{
    return GetRecordCount();
}

BOOL CDBStdanswer::Move(BOOL isNext)
{
    return CRQDb::Move(isNext);
}
BOOL CDBStdanswer::LoadBySubject(CString subject)
{
	CRQWhere w;
	w.Add("Subject",CRQWhere::GetString(subject),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBStdanswer::Add()
{
	return CRQDb::Add( );
}
BOOL CDBStdanswer::LoadByPaperID(int pepaerID)
{
	CRQWhere w;
	w.Add("PAPERID",CRQWhere::GetLong(pepaerID));
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBStdanswer::LoadByQuery(int paperid, int no)
{
	CRQWhere w,w1, w2;
	if(paperid == 0)
		w1.Add("YDID","0", ">=");
	else
		w1.Add("PAPERID",CRQWhere::GetLong(paperid) );

	if(no == 0)
		w2.Add("YDID","0", ">=");
	else
		w2.Add("FRAGE",CRQWhere::GetLong(no) );
	w = w1 & w2;
	CString tmp = w.GetClausePlain();
	return Load(tmp);		
}