#include "RQDb.h"
/******************************************************
类名：CDBDepartment
说明：负责处理信息的增加、删除和修改，刷新
******************************************************/
#pragma once
#ifndef   _____CDBDepartment_h____  
#define   _____CDBDepartment_h____  
class CDBDepartment : protected CRQDb
{
public:
	CDBDepartment();
	 ~CDBDepartment();
public:	
	BOOL              Connect();
	void              Close();
	int               GetAccount();
	BOOL			  LoadByAll();
	BOOL              Add();
	void              Delete(int num);
	BOOL              Move(BOOL isNext = TRUE);
	BOOL			  LoadByDepart(CString depart);
	BOOL			  LoadByDepartID(CString departID);
	BOOL		      LoadByClass(CString strClass);
	BOOL			  LoadByClassID(CString strClassID);
	BOOL			  Update(CString strDepartID, CString strnewDepartID, CString strDepar, CString strDeparthead, CString strDeparttel);
	BOOL              Update(int index);
	BOOL			  DeleteDepart(CString strDepartID);
	BOOL			  DeleteClass(CString strClassID);
	BOOL			  LoadByDepartid(CString sDepartid);

public:
	DWORD			  m_ydid;
	CString			  m_departid;
	CString			  m_departname;
	CString			  m_persident;
	CString			  m_persidenttel;
	
private:
	CAdoConnection    m_connect;
	CString           GetExePath();
};
#endif