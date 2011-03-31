#include "RQDb.h"
/******************************************************
类名：CDBStdanswer 
说明：负责处理信息的增加、删除和修改，刷新
******************************************************/
#pragma once
#ifndef   _____CDBStdanswer_h____  
#define   _____CDBStdanswer_h____  
class CDBStdanswer : public CRQDb 
{
public:
	CDBStdanswer();
	 ~CDBStdanswer();
public:
	BOOL              Connect();
	void              Close();
	BOOL              Delete(int num);
	BOOL			  LoadByAll();
	BOOL              Move(BOOL isNext = TRUE);
	int               GetAccount();
	BOOL			  LoadBySubject(CString subject);
	BOOL			  LoadByPaperID(int paperID);
	BOOL			  Deletepaper(int paperid);
	BOOL              Add();
	BOOL			  LoadByQuery(int paperid, int no);
public:
	DWORD			  m_ydid;
	DWORD		      m_paperid;
	DWORD		      m_frage;
	CString			  m_res;
	double			  m_score;
private:
	CAdoConnection    m_connect;
	CString           GetExePath();
};
#endif