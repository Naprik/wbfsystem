#include "stdafx.h"
#include "YDThreadFun.h"
#include "SoftStat.h"
#include "DlgExamstat.h"
#include "../Base\AutoClean.h"
#include "../ObjRef\YDObjectRef.h"
#include "../UIBase\ListBoxEx.h"
#include "../\Base\DataHandler.h"
#include "InvigilateInfo.h"
#include "../DBBase\DBTransactionRef.h"
#include "../YDExamObjRef\YDEAddrUnit.h"
#include "YDProgressControl.h"
#include "../UIBase\ControlWnd.h"
#include "ThreadTeachComParam.h"
#include "ThreadFunHelper.h"
#include "../Base\CriticalSectionControl.h"
#include "ThreadFunOperate.h"
#include "FormExamBlue.h"

CCriticalSection gThreadCriticalSection;

ULONG SearchThread(LPVOID pParam)
{
	CFormExamBlue *pForm = (CFormExamBlue *)pParam;
	ASSERT(pForm);
	CBOOLControl BOOLControl(&(pForm->m_bCanClose));
	HRESULT hr = E_FAIL;
	CThreadFunOperate ThreadFunOperate(pForm);
	hr = ThreadFunOperate.SearchThread();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return -1;
	}
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return -1;
	}
	
	std::list<CYDObjectRef*>  lstStu;
	for(int i = 0; i < pForm->m_lstCtrlUnit.GetItemCount(); i++)
	{
		if(pForm->m_lstCtrlUnit.GetCheck(i))
		{
			CYDObjectRef* pObjStu = (CYDObjectRef*)pForm->m_lstCtrlUnit.GetItemData(i);
			ASSERT(pObjStu);
			lstStu.push_back(pObjStu);
		}
	}
	if(lstStu.size() <= 0)
	{
		return 0;
	}
	//	GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(FALSE);
	if(pGExamStruct->m_pExamDetail->m_uExamKind == YDEXAM_KIND_NORMAL)
	{
		//��ʽ���Բſ��Կ���
		pForm->GetDlgItem(IDC_BUTTON_BEGINEXAM)->EnableWindow(TRUE);
		pForm->GetDlgItem(IDC_BUTTON_ROLLING)->EnableWindow(FALSE);
	}
	else
	{
		pForm->GetDlgItem(IDC_BUTTON_BEGINEXAM)->EnableWindow(FALSE);
		pForm->GetDlgItem(IDC_BUTTON_ROLLING)->EnableWindow(TRUE);
	}
	pGExamStruct->m_pExamDetail->m_nSetp = YDEXAM_STEP_ROOLCALL;
	return 0;
}

ULONG SpecSearchThreadByUsb(LPVOID pParam,CProgressThreadDlg* pProgressDlg)//���ⵥ������,ͨ��USB
{
	pProgressDlg->m_flag = FALSE;
	return 0;
}

ULONG BeginExamThread(LPVOID pParam)
//��ѧ�������������Ϳ�����Ϣ��
{
	HRESULT hr = E_FAIL;
	CFormExamBlue *pForm = (CFormExamBlue *)pParam;
	ASSERT(pForm);
	CBOOLControl BOOLControl(&(pForm->m_bCanClose));
	CThreadFunOperate ThreadFunOperate(pForm);
	hr = ThreadFunOperate.BeginExamThread();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return -1;
	}
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return -1;
	}
	pGExamStruct->m_pExamDetail->m_nSetp = YDEXAM_STEP_BGGINEXAM;
	pForm->GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(FALSE);
	pForm->GetDlgItem(IDC_BUTTON_BEGINEXAM)->EnableWindow(FALSE);
	pForm->GetDlgItem(IDC_BUTTON_ROLLING)->EnableWindow(TRUE);
	return 0;
}


ULONG SpecRollThreadByUse(LPVOID pParam,CProgressThreadDlg* pProgressDlg)//���ⵥ��ͨ��USB����
{

	return 0;
}

ULONG RollingThread(LPVOID pParam)      // �վ�
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return -1;
	}
	CFormExamBlue *pForm = (CFormExamBlue *)pParam;
	ASSERT(pForm);
	CBOOLControl BOOLControl(&(pForm->m_bCanClose));

	CThreadFunOperate ThreadFunOperate(pForm);
	hr = ThreadFunOperate.RollingThread();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return -1;
	}
	if(pGExamStruct->m_pExamDetail->m_nSetp = YDEXAM_STEP_ROLLING)
	{
		pForm->GetDlgItem(IDC_BUTTON_QUERYMARK)->EnableWindow(TRUE);
		pForm->GetDlgItem(IDC_BUTTON_PAPER_ANSYS)->EnableWindow(TRUE);
	}
	return 0;
}



ULONG RollingThreadByTeachcom(LPVOID pParam)
{
	CThreadFunHelper ThreadFunHelper;
	CThreadTeachComParam* pTeachComParam = (CThreadTeachComParam*)pParam;
	ASSERT(pTeachComParam);
	ASSERT(pTeachComParam->m_pForm);
	ASSERT(pTeachComParam->m_pAppCom);
	HRESULT hr = E_FAIL;
	DWORD dwSpanLimit = (pTeachComParam->m_pAppCom->m_lstStuDev.size())*20;//һ��ѧ��20�룬(*itr)->m_lstStuDevѧ��������
	BOOL bRoll = FALSE;
	BOOL bRolling = FALSE;
	DWORD dwStart = GetTickCount();
	while(TRUE)
	{
		CCriticalSectionControl CriticalSectionControl(&gThreadCriticalSection);
		std::list<CStudev*> lstRollingStudev;//֮ǰ�Ѿ��վ��ѧ����
		hr = ThreadFunHelper.GetRollingStudev(pTeachComParam->m_pForm,lstRollingStudev);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return -1;
		}
		hr = pTeachComParam->m_pAppCom->Updatestat(bRoll,bRolling);
		if(FAILED(hr))
		{
			TRACE0("Error ");
			DWORD dwSpan = (GetTickCount() - dwStart)/1000;
			if(dwSpan > dwSpanLimit)
			{
				break;
			}
			Sleep(1000);
			continue;
		}
		if(bRolling)
		{
			break;
		}
		Sleep(1000);
		for(std::list<CStudev*>::const_iterator itrDev = pTeachComParam->m_pAppCom->m_lstStuDev.begin();
			itrDev != pTeachComParam->m_pAppCom->m_lstStuDev.end();++itrDev)
		{
			int iRolling = -1;
			hr = (*itrDev)->GetRolling(iRolling);
			if(FAILED(hr))
			{
				return hr;
			}
			if(iRolling != 1)
			{
				//��ǰѧ������û���յ�����
				continue;
			}
			BOOL bFind = FALSE;
			for(std::list<CStudev*>::const_iterator itrDev1 = lstRollingStudev.begin();
				itrDev1 != lstRollingStudev.end();++itrDev1)
			{
				if(*itrDev1 == *itrDev)
				{
					bFind = TRUE;
					break;
				}
			}
			if(bFind)
			{
				continue;
			}
			Sleep(500);
			CString strAnswer;
			hr = pTeachComParam->m_pAppCom->GetAnswerByStudev(
													*itrDev,
													pTeachComParam->m_iStart,
													pTeachComParam->m_iEnd,
													strAnswer);
			if(FAILED(hr))
			{
				DWORD dwSpan = (GetTickCount() - dwStart)/1000;
				if(dwSpan > dwSpanLimit)
				{
					break;
				}
				continue;
			}
			//AfxMessageBox(strAnswer);
			hr = ThreadFunHelper.UpdateListRollingByStudev(*itrDev,pTeachComParam->m_pForm,FALSE);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return -1;
			}
			if(!bFind)
			{
				pTeachComParam->m_pYDProgressControl->StepIt();
			}
		}

		DWORD dwSpan = (GetTickCount() - dwStart)/1000;
		if(dwSpan > dwSpanLimit)
		{
			if(AfxMessageBox(_T("��ʦ���վ�ʧ�ܣ��Ƿ�����ȴ�?"),MB_YESNO) == IDYES)
			{
				dwStart = GetTickCount();
			}
			else
			{
				//�˳�ѭ���������߳�
				return -1;
			}
		}
	}
	return 0;
}

ULONG BeginExamThreadByTeachcom(LPVOID pParam)
{
	CThreadFunHelper ThreadFunHelper;
	CThreadTeachComParam* pTeachComParam = (CThreadTeachComParam*)pParam;
	ASSERT(pTeachComParam);
	ASSERT(pTeachComParam->m_pForm);
	ASSERT(pTeachComParam->m_pAppCom);
	ASSERT(pTeachComParam->m_pYDProgressControl);
	HRESULT hr = E_FAIL;
	DWORD dwSpanLimit = (pTeachComParam->m_pAppCom->m_lstStuDev.size())*20;//һ��ѧ��20�룬(*itr)->m_lstStuDevѧ��������
	BOOL bRool = FALSE,bRolling = FALSE;
	DWORD dwStart = GetTickCount();
	while(TRUE)
	{
		CCriticalSectionControl CriticalSectionControl(&gThreadCriticalSection);
		std::list<CStudev*> lstBeginExamStudev;//ÿ��ѭ��ǰ�ѽ������״̬��ѧ��������
		hr = ThreadFunHelper.GetBeginExamStudev(pTeachComParam->m_pForm,lstBeginExamStudev);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return -1;
		}
		hr = pTeachComParam->m_pAppCom->Updatestat(bRool,bRolling);
		if(FAILED(hr))
		{
			TRACE0("Error ");
		}
		if(bRool)
		{
			break;
		}
		Sleep(1000);
		for(std::list<CStudev*>::const_iterator itrDev = pTeachComParam->m_pAppCom->m_lstStuDev.begin();
			itrDev != pTeachComParam->m_pAppCom->m_lstStuDev.end();++itrDev)
		{
			int iRollCall = -1;
			hr = (*itrDev)->GetRollCall(iRollCall);
			if(FAILED(hr))
			{
				return hr;
			}
			if(iRollCall != 1)
			{
				//��ǰѧ������û�е���
				continue;
			}
			BOOL bFind = FALSE;
			for(std::list<CStudev*>::const_iterator itrDev1 = lstBeginExamStudev.begin();
				itrDev1 != lstBeginExamStudev.end();++itrDev1)
			{
				if(*itrDev1 == *itrDev)
				{
					bFind = TRUE;
					break;
				}
			}
			if(bFind)
			{
				continue;
			}
			hr = ThreadFunHelper.UpdateListBeginExamByStudev(*itrDev,pTeachComParam->m_pForm,FALSE);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return -1;
			}
			pTeachComParam->m_pYDProgressControl->StepIt();
		}

		DWORD dwSpan = (GetTickCount() - dwStart)/1000;
		if(dwSpan > dwSpanLimit)
		{
			if(AfxMessageBox(_T("��ʦ������ʧ�ܣ��Ƿ�����ȴ�?"),MB_YESNO) == IDYES)
			{
				dwStart = GetTickCount();
			}
			else
			{
				//�˳�ѭ���������߳�
				return -1;
			}
		}
	}
	return 0;
}
