#include "RQDb.h"

#pragma once
#ifndef   _____CDBTeacherDev_h____  
#define   _____CDBTeacherDev_h____  
class CDBTeacherDev  : protected CRQDb
{
public:
	CDBTeacherDev();
	 ~CDBTeacherDev();
public:
	BOOL              Connect();
	void			  Close();
	BOOL              Move(BOOL isNext = TRUE);
	int               GetAccount();
	BOOL			  LoadByAll();
	BOOL              Add();
	BOOL              Delete(int num);
	BOOL			  LoadByDevid(CString devid);
//	BOOL			  Update(int id, CString devid, CString teachername, CString devmac);
	BOOL			  Update(int id);
	BOOL			  LoadByDevmac(CString devmac);

public:
	DWORD			  m_ydid;
	CString			  m_teacherid;
	CString			  m_teachername;
	CString			  m_devmac;
private:
	CAdoConnection    m_connect;
	CString           GetExePath();

};
#endif

