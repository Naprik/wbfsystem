#pragma once
//从xml导入学生成绩的记录
class CYDLinkRef;
class CInputStudentAnswerRecord
{
public:
	CInputStudentAnswerRecord(void);
	~CInputStudentAnswerRecord(void);
public:
	CString				m_strStuID;//学生学号
	COleDateTime		m_ExamTime;//考试时间
	CString				m_strSubjectID;//考试科目ID
	CString				m_strExamPaperID;//考试试卷ID
	CString				m_strStuAnswer;//学生答案
	CYDObjectRef*		m_pStudent;
	CYDObjectRef*		m_pSubject;
	CYDObjectRef*		m_pPaper;
	CYDLinkRef*			m_pLinkSubjectPaper;//考试科目与试卷的关系
public:
	HRESULT CreateObj(CDatabaseEx* pDB);
	HRESULT ClearObj();
	HRESULT InsertToDB(CDatabaseEx* pDB);//插入一个学生答案到数据库中
};

