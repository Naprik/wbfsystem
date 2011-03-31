#include "RQDb.h"
/******************************************************
������CDBSubject
˵������������Ϣ�����ӡ�ɾ�����޸ģ�ˢ��
******************************************************/
#pragma once
#ifndef   _____CDBSubject_h____  
#define   _____CDBSubject_h____   
class CDBSubject : protected CRQDb
{
public:
	CDBSubject();
	 ~CDBSubject();

public:
	BOOL              Connect();
	void			  Close();
	BOOL              Move(BOOL isNext = TRUE);
	int               GetAccount();
	BOOL			  LoadByAll();
	BOOL			  Add();
	BOOL              Delete(int num);
//	BOOL			  Update(int id, CString subject);
	BOOL			  Update(int id);
	BOOL			  LoadBySubject(CString subject);
	BOOL			  LoadBySubjectid(CString subjectid);


public:
	DWORD			  m_ydid;
	CString			  m_subjectid;
	CString			  m_subject;

private:
	CAdoConnection    m_connect;
	CString           GetExePath();

};
#endif

