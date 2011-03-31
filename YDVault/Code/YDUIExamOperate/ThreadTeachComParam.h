#pragma once
class CYDTeacherAppCom;
class CFormExamBlue;
class CYDProgressControl;

class CThreadTeachComParam//用来在启动线程时做参数使用的
{
public:
	CThreadTeachComParam(void);
	~CThreadTeachComParam(void);
public:
	CYDTeacherAppCom*	m_pAppCom;
	CFormExamBlue *		m_pForm;
	int					m_iStart;
	int					m_iEnd;
	CYDProgressControl*  m_pYDProgressControl;
};
