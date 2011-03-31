#include "stdafx.h"
#include "ExamApp.h"
//#include "BackUp.h"
#include "ExamDetail.h"
#include "StaticYdComApp.h"
#include "../Include\ShowErr.h"
#include "../Base\AutoClean.h"
#include "../ObjRef\YDObjectRef.h"
#include "YDStuAppCom.h"
#include "StudentCom.h"
#include "ExamQuestionSegment.h"
#include "../YDExamObjRef\YDEAddrUnit.h"


CExamApp::CExamApp()
{

}

CExamApp::~CExamApp()
{
	ClearStuCom();
}

HRESULT CExamApp::FindTeacherDev()
{
	HRESULT hr = E_FAIL;
	hr = CYDTeacherAppCom::FindTeacherCom();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CExamApp::TestStu()
{
	HRESULT hr = E_FAIL;
/*	CYDStuAppCom  com;*/
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	/*	CTeacherDev td;
	memset(td.m_com,0,sizeof(td.m_com));
	strcpy(td.m_com,"com4");
	memset(td.m_mac,0,sizeof(td.m_mac));
	strcpy(td.m_mac,"00 00 00");
	arryTeacherdev.Add(td);	*/
	int iCount = 0;
//	int iCount = arryTeacherdev.GetSize();
	if(iCount == 0 )
		return  FALSE;
	else
		return TRUE;
}

HRESULT CExamApp::OpenStucom()
{
	HRESULT hr = E_FAIL;
	hr = ClearStuCom();
	if(FAILED(hr))
	{
		return hr;
	}
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CExamApp::Get_teach_stat(CString strCom,BOOL& _bState)
{
	strCom.TrimRight();
	_bState = FALSE;
	return S_FALSE;
}
/************************************************
��������Get_teach_count
˵������ȡ����ʹ�õĽ�ʦ������
************************************************/
HRESULT CExamApp::Get_teach_count(int& _iCount)
{
	_iCount = 0;
	HRESULT hr = E_FAIL;
	return S_OK;
}

HRESULT CExamApp::Search(std::list<CStusearch*> &_lstFindStu, 
						 std::list<CYDObjectRef*> &_lstUnit,
						 BOOL &_bSearch)
{
	_bSearch = FALSE;
	HRESULT hr = E_FAIL;

	return S_OK;
}


HRESULT CExamApp::Rollcall_all(std::list<CYDObjectRef*> &_lstStuRef, COleDateTime tStart, COleDateTime tEnd, int iMaxNO)
{
	return S_OK;
}
/****************************************************
��������Rollcall_all_new(���ʦ�����Ϳ�����Ϣ��)��
������CStringArray &arryvalidID��Ч��ѧ����ѧ�ţ�
	  COleDateTime tStart���Կ���ʱ�䣬
	  COleDateTime tEnd���Խ���ʱ��
*****************************************************/
HRESULT CExamApp::Rollcall_all_new(std::list<CYDObjectRef*> &_lstStuRef, 
								   COleDateTime tStart, COleDateTime tEnd)
{
	HRESULT hr = E_FAIL;

	return S_OK;
}
/**************************************************
��������Rollcall_infra
����
˵��������ģʽ�£����ʦ�����Ϳ�����Ϣ������Ϣ������CExamDetail��
**************************************************/
HRESULT CExamApp::Rollcall_infra()
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	int iGrade;
	if(pGExamStruct->m_pExamDetail->m_uExamKind == YDEXAM_KIND_NORMAL )
		iGrade = 0;
	else
		iGrade = 1;

	int iRolling;
	if(pGExamStruct->m_pExamDetail->m_uRollingType == ROLLING_TYPE_BLUE)
		iRolling = 0;
	else
		iRolling = 1;

	int iFm;
	if(pGExamStruct->m_pExamDetail->m_bListen)
		iFm = 1;
	else
		iFm = 0;

	
	return S_OK;
}
/********************************************
��������assignmac
������
˵������������ѧ������mac��ַ�����2̨��ʦ��ƽ��ѧ����ַ
********************************************/
HRESULT CExamApp::Assignmac()
{
	HRESULT hr = E_FAIL;

	return S_OK;
}
/*****************************************
��������Get_stat
������CStringArray &arryRoll �����Ѿ���ɵ�ѧ��ѧ��
	  CStringArray &arryRolling �վ��Ѿ���ɵ�ѧ��ѧ��
	  BOOL &bRoll�����Ƿ����
	  int &iRolldone��������������
	  BOOL &bRolling�վ��Ƿ����
	  int &iRollingdone�վ����������
˵������ȡ��ʦ����ѧ����״̬
*****************************************/
HRESULT CExamApp::Get_stat(CStringArray &_arryRoll,
						   CStringArray &_arryRolling, 
						   BOOL &_bRoll,
						   int &_iRolldone,
						   BOOL &_bRolling, 
						   int &_iRollingdone)
{
	HRESULT hr = E_FAIL;
	_bRoll = TRUE;
	_bRolling = TRUE;
	_iRolldone = 0;
	_iRollingdone = 0;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);


	_arryRoll.RemoveAll();
	_arryRolling.RemoveAll();
// 	for(std::list<CStudev*>::const_iterator itr = pGExamStruct->m_lstStudev.begin();
// 		itr != pGExamStruct->m_lstStudev.end();++itr)
// 	{
// 		if((*itr)->m_rollcall == 1)
// 		{
// 			_iRolldone++;
// 			_arryRoll.Add((*itr)->m_stuid);
// 		}
// 		if((*itr)->m_rolling == 1 || 
// 			(*itr)->m_rolling == 3)
// 		{
// 			_iRollingdone++;
// 			_arryRolling.Add((*itr)->m_stuid);
// 		}
// 	}
	return S_OK;
}
/**********************************************
��������Get_start_time
����:CString strIDѧ��ѧ��
˵������ȡ��ѧ���Ŀ���ʱ��
**********************************************/
HRESULT CExamApp::Get_start_time(CString strID,
									  COleDateTime& _strart_time)
{
	HRESULT hr = E_FAIL;
	CGExamStruct *pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	strID.TrimRight();
// 	for(std::list<CStudev*>::const_iterator itr = pGExamStruct->m_lstStudev.begin();
// 		itr != pGExamStruct->m_lstStudev.end();++itr)
// 	{
// 		CString strTemp = (*itr)->m_stuid;
// 		strTemp.TrimRight();
// 		if(strTemp == strID)
// 		{
// 			_strart_time = (*itr)->m_start;
// 			return S_OK;
// 		}
// 	}
	_strart_time = COleDateTime::GetCurrentTime();
	return S_OK;
}
/**********************************************
��������Get_start_time
����:CString strIDѧ��ѧ��
˵������ȡ��ѧ�����վ�ʱ��
**********************************************/
HRESULT CExamApp::Get_end_time(CString strID,COleDateTime& _end_time)
{
	HRESULT hr = E_FAIL;
	CGExamStruct *pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	strID.TrimRight();
// 	for(std::list<CStudev*>::const_iterator itr = pGExamStruct->m_lstStudev.begin();
// 		itr != pGExamStruct->m_lstStudev.end();++itr)
// 	{
// 		CString strTemp = (*itr)->m_stuid;
// 		strTemp.TrimRight();
// 		if(strTemp == strID)
// 		{
// 			_end_time = (*itr)->m_end;
// 			return S_OK;
// 		}
// 	}
	_end_time = COleDateTime::GetCurrentTime();
	return S_OK;
}
/********************************************
��������Get_stud_count
������
˵������ȡ�μӿ��Ե�ѧ����ʶ
********************************************/
HRESULT CExamApp::Get_stud_count(int &_iCount)
{
	HRESULT hr = E_FAIL;
	_iCount = 0;
	CGExamStruct *pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
//	_iCount = pGExamStruct->m_lstStudev.size();
	return S_OK;
}
/********************************************
��������Rollcall_single
������CString strIDѧ��ѧ��
	  COleDateTime tStart��ʼ����ʱ��
	  COleDateTime tEnd��������ʱ��
	  int iMaxNO ������
	  BOOL bLate�Ƿ��ǳٵ���ѧ��
˵������ѧ����������ָ��
********************************************/
HRESULT CExamApp::Rollcall_single(CString strID, COleDateTime tStart, COleDateTime tEnd, int iMaxNO, BOOL bLate)
{
	HRESULT hr = E_FAIL;
	strID.TrimRight();
	BOOL bIn = FALSE;
	hr = Ismac_in(strID,bIn);
	if(FAILED(hr))
	{
		return hr;
	}
	if(!bIn)
	{
		hr  = Add(strID, tStart, tEnd, bLate,FALSE,FALSE);
		if(FAILED(hr))
		{
			return hr;
		}
	}

	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	CString strmac;
	int index = 0;

// 	for(std::list<CStudev*>::const_iterator itr = pGExamStruct->m_lstStudev.begin();
// 		itr != pGExamStruct->m_lstStudev.end();++itr)
// 	{
// 		CString temp = (*itr)->m_stuid;
// 		temp.TrimRight();
// 		if(temp == strID)
// 		{
// 			strmac = (*itr)->m_mac;
// 			index = (*itr)->m_index;
// 			break;
// 		}
// 	}


	return S_OK;
}
/********************************************
��������Roll_link
������CString strIDѧ��ѧ��
	  COleDateTime tStart��ʼ����ʱ��
	  COleDateTime tEnd��������ʱ��
	  int iMaxNO ������
˵������������
********************************************/
HRESULT CExamApp::Roll_link(CString strID, COleDateTime tStart, COleDateTime tEnd, int iMaxNO)
{
	strID.TrimLeft();
	strID.TrimRight();
	HRESULT hr = E_FAIL;
	BOOL bMacIn = FALSE;
	hr = Ismac_in(strID,bMacIn);
	if(FAILED(hr))
	{
		return hr;
	}
	if(!bMacIn)
	{
		COleDateTime tCurrent;
		tCurrent = COleDateTime::GetCurrentTime();
		hr = Add(strID, tCurrent, tEnd, FALSE,TRUE,FALSE);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	else
	{
		CGExamStruct* pGExamStruct = NULL;
		hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
		if(FAILED(hr))
		{
			return hr;
		}
// 		for(std::list<CStudev*>::const_iterator itr = pGExamStruct->m_lstStudev.begin();
// 			itr != pGExamStruct->m_lstStudev.end();++itr)
// 		{
// 			if(strID.CompareNoCase((*itr)->m_stuid) == 0)	
// 			{
// 				(*itr)->m_islinkroll = TRUE;
// 				break;
// 			}
// 		}
	}
	
	for(std::list<CStudentCom*>::const_iterator itr = m_lstStuCom.begin();
		itr != m_lstStuCom.end();++itr)
	{
		//hr = (*itr)->Roll_link(tStart,tEnd,iMaxNO);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}
/********************************************
��������LinkRolling
������CString strIDѧ��ѧ��
	  COleDateTime tStart��ʼ����ʱ��
	  COleDateTime tEnd��������ʱ��
	  int iMaxNO ������
˵������������
********************************************/
// HRESULT CExamApp::LinkRolling(CString strID,int iStart, int iEnd, int paperid,
// 							  CString strAddr,CString& _strRolling)
// {
// 	strID.TrimLeft();
// 	strID.TrimRight();
// 	HRESULT hr = E_FAIL;
// 	for(std::list<CTeacherCom*>::const_iterator itr = m_lstStuCom.begin();
// 		itr != m_lstStuCom.end();++itr)
// 	{
// 		hr = (*itr)->LinkRolling(iStart,iEnd,_strRolling);
// 		if(FAILED(hr))
// 		{
// 			return hr;
// 		}
// 		if(_strRolling.IsEmpty())
// 		{
// 			return S_OK;
// 		}
// 	}
// 	
// 	COleDateTime tEnd;
// 	tEnd = COleDateTime::GetCurrentTime();
// 	COleDateTime tStart;
// 
// 	BOOL bMacIn = FALSE;
// 	hr = Ismac_in(strID,bMacIn);
// 	if(FAILED(hr))
// 	{
// 		return hr;
// 	}
// 	if(!bMacIn)
// 	{
// 		AfxMessageBox(_T("��ѧ���������ѿ�����ѧ���У����⣡"));
// 		return S_OK;
// 	}
// 	else
// 	{
// 		CGExamStruct* pGExampStruct = NULL;
// 		hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExampStruct);
// 		if(FAILED(hr))
// 		{
// 			return hr;
// 		}
// 		for(std::list<CStudev*>::const_iterator itr = pGExampStruct->m_lstStudev.begin();
// 			itr != pGExampStruct->m_lstStudev.end();++itr)
// 		{
// 			if(strID.CompareNoCase((*itr)->m_stuid) == 0)
// 			{
// 				(*itr)->m_islinkroling = TRUE;
// 				(*itr)->m_end = tEnd;
// 				tStart = (*itr)->m_start;
// 				(*itr)->m_answer = _strRolling;
// 				break;
// 			}
// 		}
// 	}
// 
// 
// 	return S_OK;
// }
/********************************************
��������Ismac_in
������CString strIDѧ��ѧ��
˵������ѧ���Ƿ��ڶ�̬����֮��
********************************************/

HRESULT CExamApp::Ismac_in(CString strID,BOOL &_bInMac)
{
	_bInMac = FALSE;
	strID.TrimRight();
	HRESULT hr= E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
// 	for(std::list<CStudev*>::const_iterator itr = pGExamStruct->m_lstStudev.begin();
// 		itr != pGExamStruct->m_lstStudev.end();++itr)
// 	{
// 		CString temp = (*itr)->m_stuid;
// 		temp.TrimRight();
// 		if(temp == strID)
// 		{
// 			_bInMac = TRUE;
// 			break;
// 		}
// 	}
	return S_OK;
}
/********************************************
��������add
������CString strIDѧ��ѧ��
	  COleDateTime tStart�ÿ�����ʼ���Ե�ʱ��
	  COleDateTime tEnd�ÿ����������Ե�ʱ��
	  BOOL bLate�ÿ����Ƿ�ٵ�
	  BOOL bIsLingroll�ÿ����Ƿ��������� 
	  BOOL bIsLinkrolling�ÿ����Ƿ������վ�
˵������ѧ���Ƿ��ڶ�̬����֮��
********************************************/
HRESULT CExamApp::Add(CString strID, COleDateTime tStart, COleDateTime tEnd,
					  BOOL bLate, BOOL bIsLingroll, BOOL bIsLinkrolling)
{
	strID.TrimRight();
	strID.TrimLeft();
	CString strmac;
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	CStudev* pStudev = new CStudev();
// 	int index = pGExamStruct->m_lstStudev.size();
// 	pStudev->m_index = index;
// 	pStudev->m_stuid = strID;
// 	pStudev->m_mac = strmac;
// 	pStudev->m_start = tStart;
// 	pStudev->m_end = tEnd;
// 	pStudev->m_islate = bLate;
// 	pStudev->m_rollcall = 0;
// 	pStudev->m_rolling = 0;
// 	pStudev->m_islinkroll = bIsLingroll;
// 	pStudev->m_islinkroling = bIsLinkrolling;
// 	pStudev->m_answer = _T("");
// 	pGExamStruct->m_lstStudev.push_back(pStudev);
	return S_OK;
}
/********************************************
��������Get_singlemac_stat
������CString strIDѧ��ѧ��
	  int &iRoll������ʽ
	  int &iRolling�վ�ʽ
˵������ȡ����ѧ���������վ�ģʽ
********************************************/
HRESULT CExamApp::Get_singlemac_stat(CString strID, int &iRoll, int &iRolling,BOOL &_bFind)
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	_bFind = FALSE;
	strID.TrimRight();
// 	for(std::list<CStudev*>::const_iterator itr = pGExamStruct->m_lstStudev.begin();
// 		itr != pGExamStruct->m_lstStudev.end();++itr)
// 	{
// 		CString temp = (*itr)->m_stuid;
// 		temp.TrimRight();
// 		if(temp == strID)
// 		{
// 			_bFind = TRUE;
// 			iRoll = (*itr)->m_rollcall;
// 			iRolling = (*itr)->m_rolling;
// 		}
// 	}
	return S_OK;
}
/****************************************
��������Rolling_all
������  int iStart��ʼ���
		int iEnd�������
		COleDateTime tEnd�վ�ʱ��
˵����  �����Ե�ѧ�������վ�
****************************************/
HRESULT CExamApp::Rolling_all(std::list<CExamQuestionSegment*> &_lstQSeg, COleDateTime tEnd)
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
// 	for(std::list<CStudev*>::const_iterator itr = pGExamStruct->m_lstStudev.begin();
// 		itr != pGExamStruct->m_lstStudev.end();++itr)
// 	{
// 		if((*itr)->m_rolling == 0 ||
// 			(*itr)->m_rolling == 2)
// 		{
// 			(*itr)->m_end = tEnd;
// 		}
// 	}
	
	

	return S_OK;
}
/********************************************
��������Get_answer
������int iStart��ʼ���
	  CString strSubject���Կ�Ŀ������
	  int paperid�Ծ�ı��
	  CString strAddrѧ�����Եĵص�
˵������ȡѧ���Ĵ𰸣����������ݿ�
********************************************/
HRESULT CExamApp::Get_answer(std::list<CExamQuestionSegment*> &_lstQSeg, 
							 CString strSubject, int paperid, CString strAddr)
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	return S_OK;

}

HRESULT CExamApp::Pre_Rolling(CString strID, COleDateTime tEnd)
{
	HRESULT hr = E_FAIL;

	return S_OK;
}

HRESULT CExamApp::GetSingleAnswer(CString strID, int iStart, int iEnd,int paperid, CString strAddr)
{
	HRESULT hr = E_FAIL;


	return S_OK;
}

HRESULT CExamApp::SingleRolling(CString strID, COleDateTime tEnd)
{
	HRESULT hr = E_FAIL;

	return S_OK;
}

// HRESULT CExamApp::LinkSuccess()
// {
// 	HRESULT hr = E_FAIL;
// 	for(std::list<CTeacherCom*>::const_iterator itr = m_lstStuCom.begin();
// 		itr != m_lstStuCom.end();++itr)
// 	{
// 		hr = (*itr)->LinkSuccess();
// 		if(FAILED(hr))
// 		{
// 			return hr;
// 		}
// 	}
// 	return S_OK;
// }

HRESULT CExamApp::Offexaminfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO,int iStartNO,int iEndNO)
{
	HRESULT hr = E_FAIL;
	for(std::list<CStudentCom*>::const_iterator itr = m_lstStuCom.begin();
		itr != m_lstStuCom.end();++itr)
	{
		//hr = (*itr)->Offexaminfo(timeStart,timeEnd,iMaxNO,iStartNO,iEndNO);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CExamApp::Offsendmac(std::list<CStudev*> &_lstStuDev)
{
	HRESULT hr = E_FAIL;
	for(std::list<CStudentCom*>::const_iterator itr = m_lstStuCom.begin();
		itr != m_lstStuCom.end();++itr)
	{
		//hr = (*itr)->Offsendmac(_lstStuDev);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}
HRESULT CExamApp::Rest()
{
	HRESULT hr = E_FAIL;

	return S_OK;
}

HRESULT CExamApp::OpenFM()
{
	HRESULT hr = E_FAIL;

	return S_OK;
}
HRESULT CExamApp::SearchFM()
{
	HRESULT hr = E_FAIL;

	return S_OK;
}
HRESULT CExamApp::CloseFM()
{
	HRESULT hr = E_FAIL;

	return S_OK;

}
HRESULT CExamApp::UpFM()
{
	HRESULT hr = E_FAIL;

	return S_OK;
}

HRESULT CExamApp::DownFM()
{
	HRESULT hr = E_FAIL;

	return S_OK;
}

HRESULT CExamApp::SetFM(CString strFM)
{
	HRESULT hr = E_FAIL;
	return S_OK;
}

HRESULT CExamApp::GetFm(CString& _str)
{
	HRESULT hr = E_FAIL;

	return S_OK;
}

HRESULT CExamApp::Getoffanswer(CStringArray &arryList,int iStartNO, int iEndNO)
{
	HRESULT hr = E_FAIL;
	for(std::list<CStudentCom*>::const_iterator itr = m_lstStuCom.begin();
		itr != m_lstStuCom.end();++itr)
	{
		//hr = (*itr)->Getoffanswer(arryList,iStartNO,iEndNO);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CExamApp::Getrelationmap(CStringArray &arryrelation)
{
	HRESULT hr = E_FAIL;
	for(std::list<CStudentCom*>::const_iterator itr = m_lstStuCom.begin();
		itr != m_lstStuCom.end();++itr)
	{
		//hr = (*itr)->Getrelationmap(arryrelation);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CExamApp::FetchAnswer()
{
	HRESULT hr = E_FAIL;

	return S_OK;
}

HRESULT CExamApp::ClearStuCom()
{
	CListAutoClean<CStudentCom> clr(m_lstStuCom);
	return S_OK;
}


