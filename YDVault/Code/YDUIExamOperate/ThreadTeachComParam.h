#pragma once
class CYDTeacherAppCom;
class CFormExamBlue;
class CYDProgressControl;

class CThreadTeachComParam//�����������߳�ʱ������ʹ�õ�
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
