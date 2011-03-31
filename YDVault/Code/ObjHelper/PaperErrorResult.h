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
	CYDPaper*					m_pPaper;//当前考卷
	std::list<UINT>				m_lstErrorItemNo;//错误的题目号
	std::list<CYDStuMark*>		m_lstStuMark;//学生成绩列表
};
