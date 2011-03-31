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
	nRoll_pattern = -1;				//点名样式：1、蓝牙，2、红外，3、USB
	nRolling_pattern = -1;			//收卷样式：1、蓝牙，2、红外，3、USB
	arryClass.RemoveAll();			//班级数组
	sAddress = "";					//考试地点
	sDepart = "";					//考试的院系
	nPaperid = -1;					//试卷编号；
	sPapername = "";				//试卷名称
	sSubject = "";					//考试科目
	sStart = "";					//开考时间
	sEnd = "";						//收卷时间
	nMaxNO = -1;					//最大题号
	nStartNO = -1;					//开始题号
	nEndNO = -1;					//结束题号
	b_listen = FALSE;				//是否有听力
	b_part1 = FALSE;
	b_part2 = FALSE;
	b_part3 = FALSE;
	nSetp = 0;						//考试进行到的步骤
	bIsRollDone = FALSE;			//点名是否结束
	bIsRollingDone = FALSE;			//收卷是否结束
	m_arryStuIfo.RemoveAll();
}

BOOL CExamDetail::IsEmpty()
{
	if (nRoll_pattern == -1)
		return TRUE;
	else
		return FALSE;
}
