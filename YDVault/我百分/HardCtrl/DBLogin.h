#include "RQDb.h"
/******************************************************
������CDBLogin
˵������������Ϣ�����ӡ�ɾ�����޸ģ�ˢ��
******************************************************/

class CDBLogin : protected CRQDb
{
public:
	CDBLogin();
	 ~CDBLogin();

public:
	BOOL              Connect();
	void              Close();
	void              Delete(int num);
	BOOL			  LoadByAll();
	BOOL              Move(BOOL isNext = TRUE);
	int               GetAccount();

public:
	DWORD			  m_ydid;
	CString		      m_user;
	CString		      m_pass;
	DWORD			  m_authority;
private:
	CAdoConnection    m_connect;
	CString           GetExePath();
};

