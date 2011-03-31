#include "StdAfx.h"
#include "ExamQuestionSegment.h"
#include "../Base\DataHandler.h"

CExamQuestionSegment::CExamQuestionSegment(void)
{
	m_iStart = 0;//��ʼ���
	m_iEnd = 0;  //�������
	m_iMultiType = 1; // 0 :�����ѡ 1:����ѡ
	m_iOptionNum = 0;//ѡ��ĸ���
}

CExamQuestionSegment::~CExamQuestionSegment(void)
{
}

HRESULT CExamQuestionSegment::CreateRollcallCmd(CString &_strCmd)//���ɵ�������Ϣ
{
	_strCmd = _T("");
	// XX XX XX ��һ����Ŀ�������� 
	// ��һ��XX�����һ����ŵĿ�ʼ��ţ�
	// �ڶ���XX�����һ����ŵĽ�����š�
	// ������XX����4λ�����һ�ε���Ŀ�Ƿ��ѡ���磺0000 �����ѡ 0001 ����ѡ��
	// ����λ����ѡ��ĸ������磺0011 ��������ѡ��ABC  1111 ����15��ѡ��A-O����Ŀ��
	CString strStart  = CDataHandler::FormatNO(CDataHandler::DectoHexstr(m_iStart));
	_strCmd += strStart;
	CString strEnd = CDataHandler::FormatNO(CDataHandler::DectoHexstr(m_iEnd));
	_strCmd += strEnd;
	if(m_iMultiType == 0)
	{
		//��ѡ
		_strCmd += _T("1");
	}
	else if(m_iMultiType == 1)
	{
		//��ѡ
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
