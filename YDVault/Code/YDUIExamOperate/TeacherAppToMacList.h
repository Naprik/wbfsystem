#pragma once

class CTeacherAppToMacList
{
public:
	CTeacherAppToMacList(void);
	~CTeacherAppToMacList(void);
public:
public:
	CYDTeacherAppCom*	m_pTeacherAppCom;
	std::list<CString>	m_lstMacStu;
};
