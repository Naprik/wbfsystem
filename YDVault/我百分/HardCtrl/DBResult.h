#include "RQDb.h"
/******************************************************
类名：CDBResult
说明：负责处理信息的增加、删除和修改，刷新
******************************************************/
#pragma once
#ifndef   _____CDBResult_h____  
#define   _____CDBResult_h____  
class CDBResult : public CRQDb 
{
public:
	CDBResult();
	 ~CDBResult();
public:	
	BOOL              Connect();
	void              Delete(int num);
	void              Close();
	BOOL              Move(BOOL isNext = TRUE);
	int               GetAccount();
	BOOL			  LoadByAll();
	BOOL              Add();
	BOOL			  Update(int id);
	BOOL			  LoadByStuid(CString id);
	BOOL			  LoadByQuery(CString stuid,
								  int paperid);
	BOOL			  LoadByPaperID(int id);
	BOOL			  Deletepaper(int paperid);
	BOOL              LoadByRes(CString stuid, CString stime);
public:
	DWORD			  m_ydid;
	CString			  m_stuid;
	DWORD			  m_paperid;	
	COleDateTime      m_starttime;
	COleDateTime      m_endtime;
	CString			  m_addrid;	
	CString			  m_stuanswer;

private:
	CAdoConnection    m_connect;
	CString           GetExePath();
};
#endif