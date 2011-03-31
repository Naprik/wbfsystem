#include "StdAfx.h"
#include "ExamQuestionSegment.h"
#include "../Base\DataHandler.h"

CExamQuestionSegment::CExamQuestionSegment(void)
{
	m_iStart = 0;//开始题号
	m_iEnd = 0;  //结束题号
	m_iMultiType = 1; // 0 :代表多选 1:代表单选
	m_iOptionNum = 0;//选项的个数
}

CExamQuestionSegment::~CExamQuestionSegment(void)
{
}

HRESULT CExamQuestionSegment::CreateRollcallCmd(CString &_strCmd)//生成点名的信息
{
	_strCmd = _T("");
	// XX XX XX 第一段题目段数设置 
	// 第一个XX代表第一段题号的开始题号，
	// 第二个XX代表第一段题号的结束题号。
	// 第三个XX：高4位代表第一段的题目是否多选例如：0000 代表多选 0001 代表单选。
	// 低四位代码选项的个数例如：0011 代表三个选项ABC  1111 代表15个选项A-O的题目。
	CString strStart  = CDataHandler::FormatNO(CDataHandler::DectoHexstr(m_iStart));
	_strCmd += strStart;
	CString strEnd = CDataHandler::FormatNO(CDataHandler::DectoHexstr(m_iEnd));
	_strCmd += strEnd;
	if(m_iMultiType == 0)
	{
		//单选
		_strCmd += _T("1");
	}
	else if(m_iMultiType == 1)
	{
		//多选
		_strCmd += _T("0");
	}
	else
	{
		ASSERT(FALSE);
	}
	_strCmd += CDataHandler::DectoHexstr(m_iOptionNum);
	_strCmd += _T(" ");
	return S_OK;
}
