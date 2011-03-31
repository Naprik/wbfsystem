#include "StdAfx.h"
#include "AnswerRecord.h"

CAnswerRecord::CAnswerRecord(const CString& strNo, const CString& strAnswer, const CString& strMark)
: m_strNO(strNo), m_strAnswer(strAnswer), m_strMark(strMark)
{

}

CAnswerRecord::~CAnswerRecord(void)
{
}
