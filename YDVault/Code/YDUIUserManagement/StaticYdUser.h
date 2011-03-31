#pragma once
class CYDUserRef;

class BASE_DLL_API  CStaticYdUser
{
public:
	CStaticYdUser(void);
	~CStaticYdUser(void);
public:
	static CStaticYdUser* Instance(){return &m_staticYdUser;}
private:
	static CStaticYdUser m_staticYdUser;
	CYDUserRef*			m_pCurUser;
public:
	HRESULT GetCurUser(CYDUserRef*& _pUser);
	HRESULT SetCurUser(CYDUserRef*& _pUser);
};
