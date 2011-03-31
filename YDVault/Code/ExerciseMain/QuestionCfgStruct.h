#pragma once
#include <list>
/*<QUESTION type="1">
<HARDLEVEL level="1" number="10" /> 
<HARDLEVEL level="16" number="10" /> 
</QUESTION>
*/

class CXmlNode;
class CXmlWriter;

class CQuestionLevelNum
{
public:
	CQuestionLevelNum()
	{
		m_level = (HARDLEVEL)-1;
		m_uEachNum = 0;
		m_uNumber = 0;
		m_fMark = 0.0;
	}
	HARDLEVEL m_level;//题目的难度
	UINT	  m_uEachNum;//每题问题数
	UINT	  m_uNumber;//出题的数目
	double    m_fMark; //每题分数
};
class CQuestionCfgStruct
{
public:
	CQuestionCfgStruct(void);
	~CQuestionCfgStruct(void);
public:
	OBJID	m_QTypeID; //题型ID
	std::list<CQuestionLevelNum* >	m_lstLevelNum;//各个考试难度的题目
	HRESULT Load(CXmlNode& _node);
	HRESULT Save(CXmlWriter& _writer);
};
