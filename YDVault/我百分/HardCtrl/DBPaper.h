#include "RQDb.h"

#pragma once
#ifndef   _____CDBPaper_h____  
#define   _____CDBPaper_h____   
class CDBPaper : public CRQDb  
{
public:
	CDBPaper();
	virtual ~CDBPaper();
public:
	BOOL              Connect();
	void              Close();
	BOOL              Delete(int num);
	BOOL			  LoadByAll();
	BOOL              Move(BOOL isNext = TRUE);
	int               GetAccount();
	BOOL			  Add();
	BOOL			  Update(int id);
	BOOL			  LoadByDepart(CString strDepart);
	BOOL			  LoadBySubjectid(CString strSubjectid);
	BOOL			  LoadByTime(COleDateTime time);
	BOOL			  LoadByPaper(CString strSubject, CString strClass, CString time);
// 	BOOL			  LoadByQuery(CString strPaper,
// 								  CString strSubject,
// 								  CString strDepart,
// 								  CString strClass,
// 								  COleDateTime time,
// 								  BOOL istime);
	BOOL			  LoadByQuery(CString strpaper,
								  CString strSubjectid,
								  CString strclassid,
								  COleDateTime time,
 								  BOOL istime);
	BOOL			  LoadByPaperID(int id);
	BOOL			  LoadByPaperSub(CString strSubject, CString strPaper);
	BOOL			  LoadByPaperName(CString strPapername);

public:
	DWORD			  m_ydid;
	CString			  m_papername;
	CString			  m_subjectid;
	CString		      m_classid;
	COleDateTime      m_starttime;
	COleDateTime      m_endtime;
	DWORD			  m_startNO;
	DWORD			  m_endNO;
	DWORD		      m_maxNO;
private:
	CAdoConnection    m_connect;
	CString           GetExePath();
};
#endif