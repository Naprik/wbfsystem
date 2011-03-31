#pragma once
class CQuestionCfgStruct;
enum SEL_QUESTION_MODE //在什么模式下选择题目
{
	SEL_QUESTION_EXERCISE = 0, //在测试下选择题目
	SEL_QUESTION_EXAM = 1     //在考试模式下选择题目
};

class CSelectQuestionCfgMgr
{
public:
	CSelectQuestionCfgMgr();
	~CSelectQuestionCfgMgr(void);

	HRESULT Load(const CString& strPath);
	HRESULT Save();

private:
	CString		m_strPath;
public:
	UINT				m_uTimeSpan;//考试时长，单位是分钟,在考试模式下有效
	SEL_QUESTION_MODE   m_uSelQuesitonMode;
	OBJID               m_uQuestionVaultID;//考试题库的ID
public:
	std::list<CQuestionCfgStruct*> m_lstQuestionStruct;
	HRESULT Clear();
};
