#pragma once

class BASE_DLL_API CAnswerRecord
{
public:
	CAnswerRecord(const CString& strNo, const CString& strAnswer, const CString& strMark);
	CString		m_strNO;//题号
	CString		m_strAnswer;//标准答案
	CString		m_strMark;//分值
	~CAnswerRecord(void);
};
