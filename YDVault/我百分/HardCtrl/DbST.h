#pragma once
//#include "RQDb.h"
/******************************************************
������CCDST 
˵������������Ϣ�����ӡ�ɾ�����޸ģ�ˢ��
******************************************************/
class CCDST : protected CRQDb
{
public:	
	CCDST();
	~CCDST();
public:	
	BOOL              LoadByUser(CString domain);
public:	
	BOOL              LoadToCheck();
	BOOL              Delete(CString domain);
	BOOL              ChangeKind(CString domain,int kind);
	BOOL              LoadByAll(int kind);
	void              Close();
	BOOL              Connect();
	BOOL              Add();
	BOOL              Update(DWORD id);
	BOOL              Move(BOOL isNext = TRUE);
	int               GetAccount();
public:
	DWORD             m_id;                            		
	CString           m_name;      
	CString           m_mark;   
	int 		  m_kind; 
	
private:
	CAdoConnection    m_connect;
	CString kind;
};
