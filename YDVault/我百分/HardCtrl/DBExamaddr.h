#include "RQDb.h"

/******************************************************
类名：CDBExamaddr
说明：负责处理信息的增加、删除和修改，刷新
******************************************************/
#pragma once
#ifndef   _____CDBExamaddr_h____  
#define   _____CDBExamaddr_h____  
class CDBExamaddr : protected CRQDb
{
public:
	CDBExamaddr();
	 ~CDBExamaddr();
public:
	BOOL              Connect();
	void			  Close();
	BOOL              Move(BOOL isNext = TRUE);
	int               GetAccount();
	BOOL			  LoadByAll();
	BOOL			  Add();
	BOOL              Delete(int num);
	BOOL			  Update(int id);
	BOOL			  LoadByAddr(CString addr);
	BOOL			  LoadByAddrID(CString addrID);

public:
	DWORD			  m_ydid;
	CString			  m_addrid;
	CString			  m_examaddr;

private:
	CAdoConnection    m_connect;
	CString           GetExePath();
};
#endif
