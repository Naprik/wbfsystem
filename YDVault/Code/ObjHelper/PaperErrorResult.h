#pragma once
#include <list>
class CYDObjectRef;
class CYDPaper;
class CYDStuMark;
class BASE_DLL_API CPaperErrorResult
{
public:
	CPaperErrorResult(void);
	~CPaperErrorResult(void);
public:
	CYDPaper*					m_pPaper;//��ǰ����
	std::list<UINT>				m_lstErrorItemNo;//�������Ŀ��
	std::list<CYDStuMark*>		m_lstStuMark;//ѧ���ɼ��б�
};
