// ExamDetail.cpp: implementation of the CExamDetail class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hardctrl.h"
#include "ExamDetail.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExamDetail::CExamDetail()
{
	Initdata();
}

CExamDetail::~CExamDetail()
{

}

void CExamDetail::Initdata()
{
	nRoll_pattern = -1;				//������ʽ��1��������2�����⣬3��USB
	nRolling_pattern = -1;			//�վ���ʽ��1��������2�����⣬3��USB
	arryClass.RemoveAll();			//�༶����
	sAddress = "";					//���Եص�
	sDepart = "";					//���Ե�Ժϵ
	nPaperid = -1;					//�Ծ��ţ�
	sPapername = "";				//�Ծ�����
	sSubject = "";					//���Կ�Ŀ
	sStart = "";					//����ʱ��
	sEnd = "";						//�վ�ʱ��
	nMaxNO = -1;					//������
	nStartNO = -1;					//��ʼ���
	nEndNO = -1;					//�������
	b_listen = FALSE;				//�Ƿ�������
	b_part1 = FALSE;
	b_part2 = FALSE;
	b_part3 = FALSE;
	nSetp = 0;						//���Խ��е��Ĳ���
	bIsRollDone = FALSE;			//�����Ƿ����
	bIsRollingDone = FALSE;			//�վ��Ƿ����
	m_arryStuIfo.RemoveAll();
}

BOOL CExamDetail::IsEmpty()
{
	if (nRoll_pattern == -1)
		return TRUE;
	else
		return FALSE;
}
