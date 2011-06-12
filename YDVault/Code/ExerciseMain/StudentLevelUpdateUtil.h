#pragma once

#include <map>

class CYDObjectRef;

class CStudentLevelUpdateUtil
{
public:
	~CStudentLevelUpdateUtil(void);
	static CStudentLevelUpdateUtil* Instance(){return &s_instance;}

	HRESULT GetStudentLevel(OBJID vaultid, int condition, CString* plevel);
private:
	CStudentLevelUpdateUtil(void);
	HRESULT Init();
	HRESULT AddCfg(CYDObjectRef* cfg);

private:
	static CStudentLevelUpdateUtil	s_instance;
	typedef std::map<int, std::list<CYDObjectRef*>> MapLevel;
	MapLevel	m_vault2level;
};

