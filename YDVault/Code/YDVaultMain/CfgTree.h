#pragma once

//�����ж�Ҫ��ʾ����
class CCfgTree
{
public:
	CCfgTree(void);
	~CCfgTree(void);
public:
	void Init();
	BOOL m_bShowQuestionTree;//������Ƿ���ʾ
	BOOL m_bShowPaperTree;
	BOOL m_bShowExamTree;
	BOOL m_bShowDataMaintainTree;
};
