#pragma once
//��xml����ѧ���ɼ��ļ�¼
class CYDLinkRef;
class CInputStudentAnswerRecord
{
public:
	CInputStudentAnswerRecord(void);
	~CInputStudentAnswerRecord(void);
public:
	CString				m_strStuID;//ѧ��ѧ��
	COleDateTime		m_ExamTime;//����ʱ��
	CString				m_strSubjectID;//���Կ�ĿID
	CString				m_strExamPaperID;//�����Ծ�ID
	CString				m_strStuAnswer;//ѧ����
	CYDObjectRef*		m_pStudent;
	CYDObjectRef*		m_pSubject;
	CYDObjectRef*		m_pPaper;
	CYDLinkRef*			m_pLinkSubjectPaper;//���Կ�Ŀ���Ծ�Ĺ�ϵ
public:
	HRESULT CreateObj(CDatabaseEx* pDB);
	HRESULT ClearObj();
	HRESULT InsertToDB(CDatabaseEx* pDB);//����һ��ѧ���𰸵����ݿ���
};

