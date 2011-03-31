#include "stdafx.h"
#include "hardctrl.h"
#include "DBPaper.h"
#include "RQWhere.h"
#include "Base.h"


CDBPaper::CDBPaper()
{
	//在这里设置数据成员和表的影射关系
	m_colMap->SetAt("YDID",&m_ydid);
	m_colMap->SetAt("PAPERNAME",&m_papername);
	m_colMap->SetAt("SUBJECTID",&m_subjectid);
	m_colMap->SetAt("CLASSID",&m_classid);
	m_colMap->SetAt("STARTTIME",&m_starttime);
	m_colMap->SetAt("ENDTIME",&m_endtime);
	m_colMap->SetAt("STARTNO",&m_startNO);
	m_colMap->SetAt("ENDNO",&m_endNO);
	m_colMap->SetAt("MAXNO",&m_maxNO);
}

CDBPaper::~CDBPaper()
{
	Close();
}
CString CDBPaper::GetExePath()
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

BOOL CDBPaper::Connect()
{
	CJahCoverCapture capture;
	CBase base;
	capture.m_bmpFile =base.GetExePath()+"stusys.udl" ;
	BOOL ret= m_connect.OpenUDLFile(capture.m_bmpFile);
	if ( ret )
		ret = CRQDb::Open("ydpaper", &m_connect);	
	return ret; 
}
void CDBPaper::Close()
{
	CRQDb::Close();
}
BOOL CDBPaper::Move(BOOL isNext)
{
	return CRQDb::Move(isNext);
}
int CDBPaper::GetAccount()
{
	return GetRecordCount();
}
BOOL CDBPaper::Add()
{
	return CRQDb::Add();
}
BOOL CDBPaper::Delete(int num)
{
	return CRQDb::Delete(num);
}
BOOL CDBPaper::Update(int id)
{
	return CRQDb::Update(id);
}
BOOL CDBPaper::LoadByAll()
{
	CRQWhere w;
	w.Add("YDID","0", ">=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBPaper::LoadByDepart(CString strDepart)
{
	CRQWhere w;
	w.Add("Depart",CRQWhere::GetString(strDepart),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBPaper::LoadBySubjectid(CString strSubjectid)
{
	CRQWhere w;
	w.Add("SUBJECTID",CRQWhere::GetString(strSubjectid),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}

BOOL CDBPaper::LoadByPaperName(CString strPapername)
{
	CRQWhere w;
	w.Add("PAPERNAME",CRQWhere::GetString(strPapername),"=");
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBPaper::LoadByTime(COleDateTime time)
{
	CRQWhere w, w1, w2;
	w1.Add("StartTime",CRQWhere::GetString(time.Format()), ">=");
	COleDateTimeSpan   timeSpan(1,0,0,0); 
	time += timeSpan;
	w2.Add("StartTime",CRQWhere::GetString(time.Format()), "<=");
	w = w1 & w2;
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBPaper::LoadByPaper(CString strSubject, CString strClass, CString time)
{
	CRQWhere w1, w2, w3, w12, w123;

	w1.Add("Subject",CRQWhere::GetString(strSubject),"=" );
	w2.Add("Class",CRQWhere::GetString(strClass),"=" );
	w3.Add("StartTime",CRQWhere::GetString(time),"=" );
	w12 = w1 & w2;
	w123 = w12 & w3;
	CString tmp = w123.GetClausePlain();
	return Load(tmp);
}
// BOOL CDBPaper::LoadByQuery(CString strPaper, CString strSubject,CString strDepart,CString strClass,COleDateTime time, BOOL istime)
// {
// 	CRQWhere w,w1, w2, w3, w4, w5, w51,w52;
// 	if(strPaper == "")
// 		w1.Add("YDID","0", ">=");
// 	else
// 		w1.Add("Papername",CRQWhere::GetString(strPaper),"=" );
// 
// 	if(strSubject == "")
// 		w2.Add("YDID","0", ">=");
// 	else
// 		w2.Add("Subject",CRQWhere::GetString(strSubject),"=");
// 
// 	if(strDepart == "")
// 		w3.Add("YDID","0", ">=");
// 	else
// 		w3.Add("Depart",CRQWhere::GetString(strDepart),"=");
// 
// 	if(strClass == "")
// 		w4.Add("YDID","0", ">=");
// 	else
// 		w4.Add("Class",CRQWhere::GetString(strClass),"=");
// 
// 	if(!istime)
// 		w5.Add("YDID","0", ">=");
// 	else
// 	{
// 		w51.Add("StartTime",CRQWhere::GetString(time.Format()), ">=");
// 		COleDateTimeSpan   timeSpan(1,0,0,0); 
// 		time += timeSpan;
// 		w52.Add("StartTime",CRQWhere::GetString(time.Format()), "<=");
// 		w5 = w51&w52;
// 	}
// 	w = (((w1 & w2) & w3) & w4) & w5;
// 	CString tmp = w.GetClausePlain();
// 	return Load(tmp);	
// }

BOOL CDBPaper::LoadByQuery(CString strpaper, CString strSubjectid, CString strclassid, COleDateTime time, BOOL istime)
{
	CRQWhere w,w1, w2, w3, w4, w41, w42;
	if(strpaper == "")
		w1.Add("YDID","0", ">=");
	else
		w1.Add("PAPERNAME",CRQWhere::GetString(strpaper),"=" );
	
 	if(strSubjectid == "")
 		w2.Add("YDID","0", ">=");
 	else
 		w2.Add("SUBJECTID",CRQWhere::GetString(strSubjectid),"=");
 
 	if(strclassid == "")
 		w3.Add("YDID","0", ">=");
 	else
 		w3.Add("CLASSID",CRQWhere::GetString(strclassid),"="); 

 	if(!istime)
 		w4.Add("YDID","0", ">=");
	else
 	{
 		w41.Add("STARTTIME",CRQWhere::GetString(time.Format()), ">=");
 		COleDateTimeSpan   timeSpan(1,0,0,0); 
 		time += timeSpan;
 		w42.Add("StartTime",CRQWhere::GetString(time.Format()), "<=");
 		w4 = w41&w42;
 	}
 	w = ((w1 & w2) & w3) & w4;
 	CString tmp = w.GetClausePlain();
 	return Load(tmp);
}
BOOL CDBPaper::LoadByPaperID(int id)
{
	CRQWhere w;
	w.Add("YDID",CRQWhere::GetLong(id));
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}
BOOL CDBPaper::LoadByPaperSub(CString strSubject, CString strPaper)
{
	CRQWhere w, w1, w2;
	if(strSubject == "")
		w1.Add("YDID","0", ">=");
	else
		w1.Add("Subject",CRQWhere::GetString(strSubject),"=" );
	if(strPaper == "")
		w2.Add("YDID","0", ">=");
	else
		w2.Add("Papername",CRQWhere::GetString(strPaper),"=");
	w = w1 & w2;
	CString tmp = w.GetClausePlain();
	return Load(tmp);
}