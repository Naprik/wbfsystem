#include "RQDb.h"
/******************************************************
类名：CDbhnn 
说明：负责处理信息的增加、删除和修改，刷新
******************************************************/
#pragma once
#ifndef   _____CDBStuInfo_h____  
#define   _____CDBStuInfo_h____   
class CDBStuInfo : protected CRQDb
{
public:
	CDBStuInfo();
    ~CDBStuInfo();
public:	
	BOOL              Delete(int num);
	void              Close();
	BOOL              Connect();
	BOOL			  Add();
	BOOL              Move(BOOL isNext = TRUE);
	int               GetAccount();
	BOOL			  LoadByAll();
	BOOL			  LoadByQuery(CString num, CString name, CString depart);
	BOOL			  LoadByStuid(CString id);
	BOOL			  LoadByStuname(CString name);
	BOOL			  LoadByClass(CString strClass);
	BOOL			  Update(int index);
	BOOL			  Update(int id, CString studid, CString stuname, CString studgender, CString studpart, 
					  CString studclass, CString studtel, CString studemail, CString studphoto);
	BOOL			  LoadByDepart(CString depart);
	BOOL			  LoadByDev(CString strDev);
	BOOL			  LoadByStu(CString stuid, CString stuname);
	BOOL			  LoadByDepClass(CString strDepart, CString strClass);

	BOOL              LoadByClassid(CString strClassid);
public:
	DWORD			  m_ydid;
	CString			  m_stuid;
	CString			  m_stuname;
	CString			  m_stugender;
	DWORD			  m_stuage;
	CString			  m_studeparid;
	CString			  m_stuclassid;
	CString			  m_stutel;
	CString			  m_stuemail;
	CString			  m_stuphoto;
	CString			  m_devid;
	
private:
	CAdoConnection    m_connect;
	CString           GetExePath();

};
#endif