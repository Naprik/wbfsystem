#include "RQDb.h"
/******************************************************
类名：CDBDepartment
说明：负责处理信息的增加、删除和修改，刷新
******************************************************/
#pragma once
#ifndef   _____CDBClass_h____  
#define   _____CDBClass_h____  

class CDBClass : protected CRQDb  
{
public:
	CDBClass();
	virtual ~CDBClass();

public:	
	BOOL              Delete(int num);
	void              Close();
	BOOL              Connect();
	BOOL			  Add();
	BOOL              Move(BOOL isNext = TRUE);
	int               GetAccount();
	BOOL			  LoadByAll();
	BOOL			  LoadByDepartid(CString sDepartid);
	BOOL			  LoadByClassname(CString sClassname);
	BOOL			  LoadByClassid(CString sClassid);
	BOOL			  DeleteClassByDepartID(CString sDepartid);
	BOOL			  DeleteClassByclassid(CString sClassid);
	
public:
	DWORD			  m_ydid;
	CString			  m_classid;
	CString			  m_departid;
	CString           m_classname;
	CString			  m_head;
	CString		      m_headtel;
	
private:
	CAdoConnection    m_connect;
};

#endif // !defined(AFX_DBCLASS_H__FCFE1D85_38BB_49DF_905F_260B6ADC3BF3__INCLUDED_)
