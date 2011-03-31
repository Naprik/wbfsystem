// ExamDetail.cpp: implementation of the CExamDetail class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExamDetail.h"
#include "../Base\AutoClean.h"
#include "../ObjRef\YDObjectRef.h"
#include "ExamQuestionSegment.h"

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
	m_pObjAddress = NULL;
	m_pDepart = NULL;
	m_pPaper = NULL;
	m_pSubject = NULL;
	m_strXmlFile = _T("");
	InitClearData();
}

CExamDetail::~CExamDetail()
{
	InitClearData();
}

HRESULT CExamDetail::InitClearData()
{
	CListAutoClean<CYDObjectRef> clr1(m_lstClass);
	CPtrAutoClean<CYDObjectRef> clr2(m_pObjAddress);
	CPtrAutoClean<CYDObjectRef> clr3(m_pDepart);
	CPtrAutoClean<CYDObjectRef> clr4(m_pPaper);
	CPtrAutoClean<CYDObjectRef> clr5(m_pSubject);
	CListAutoClean<CYDObjectRef> clr6(m_lstUnit);
	CListAutoClean<CExamQuestionSegment> clr7(m_lstQSeg);
	m_uRollCallType = ROLLCALL_TYPE_UNKNOW;				//������ʽ��1��������2�����⣬3��USB
	m_uRollingType = ROLLING_TYPE_UNKNOW;			//�վ���ʽ��1��������2�����⣬3��USB
	m_pObjAddress = NULL;					//���Եص�
	m_pDepart = NULL;					//���Ե�Ժϵ
	m_pPaper = NULL;					//�Ծ�
	m_pSubject = NULL;					//���Կ�Ŀ
	m_timeStart = COleDateTime::GetCurrentTime();					//����ʱ��
	m_timeEnd = COleDateTime::GetCurrentTime();						//�վ�ʱ��
	m_bListen = FALSE;				//�Ƿ�������
	m_bListenPart1 = FALSE;
	m_bListenPart2 = FALSE;
	m_bListenPart3 = FALSE;
	m_nSetp = YDEXAM_STEP_NOT;		//���Խ��е��Ĳ���
	m_bIsRollDone = FALSE;			//�����Ƿ����
	m_bIsRollingDone = FALSE;			//�վ��Ƿ����
	
	return S_OK;
}

HRESULT	CExamDetail::GetMinMaxNo(int &_iMin,int &_iMax)
{
	HRESULT hr = E_FAIL;
	_iMin = 200;
	_iMax = 0;
	for(std::list<CExamQuestionSegment*>::const_iterator itr = m_lstQSeg.begin();
		itr != m_lstQSeg.end();++itr)
	{
		if((*itr)->m_iStart < _iMin)
		{
			_iMin = (*itr)->m_iStart;
		}
		if((*itr)->m_iEnd > _iMax)
		{
			_iMax = (*itr)->m_iEnd;
		}
	}
	return S_OK;
}
