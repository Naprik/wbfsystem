#pragma once

class BASE_DLL_API CAnswerRecord
{
public:
	CAnswerRecord(const CString& strNo, const CString& strAnswer, const CString& strMark);
	CString		m_strNO;//���
	CString		m_strAnswer;//��׼��
	CString		m_strMark;//��ֵ
	~CAnswerRecord(void);
};
