#pragma once

//用来判断要显示的树
class CCfgTree
{
public:
	CCfgTree(void);
	~CCfgTree(void);
public:
	void Init();
	BOOL m_bShowQuestionTree;//试题库是否显示
	BOOL m_bShowPaperTree;
	BOOL m_bShowExamTree;
	BOOL m_bShowDataMaintainTree;
};
