#pragma once
class CQuestionCfgStruct;
enum SEL_QUESTION_MODE //��ʲôģʽ��ѡ����Ŀ
{
	SEL_QUESTION_EXERCISE = 0, //�ڲ�����ѡ����Ŀ
	SEL_QUESTION_EXAM = 1     //�ڿ���ģʽ��ѡ����Ŀ
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
	UINT				m_uTimeSpan;//����ʱ������λ�Ƿ���,�ڿ���ģʽ����Ч
	SEL_QUESTION_MODE   m_uSelQuesitonMode;
	OBJID               m_uQuestionVaultID;//��������ID
public:
	std::list<CQuestionCfgStruct*> m_lstQuestionStruct;
	HRESULT Clear();
};
