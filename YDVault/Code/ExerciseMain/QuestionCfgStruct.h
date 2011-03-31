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
	HARDLEVEL m_level;//��Ŀ���Ѷ�
	UINT	  m_uEachNum;//ÿ��������
	UINT	  m_uNumber;//�������Ŀ
	double    m_fMark; //ÿ�����
};
class CQuestionCfgStruct
{
public:
	CQuestionCfgStruct(void);
	~CQuestionCfgStruct(void);
public:
	OBJID	m_QTypeID; //����ID
	std::list<CQuestionLevelNum* >	m_lstLevelNum;//���������Ѷȵ���Ŀ
	HRESULT Load(CXmlNode& _node);
	HRESULT Save(CXmlWriter& _writer);
};
