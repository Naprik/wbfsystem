#include "StdAfx.h"
#include "ThreadFunHelper.h"
#include "../YDExamObjRef\YDDevice.h"
#include "../YDExamObjRef\YDEAddrUnit.h"
#include "../Base\AutoClean.h"
#include "../Base\DataHandler.h"
#include "../YDUIExamOperate\InvigilateInfo.h"
#include "../Base\CriticalSectionControl.h"
#include "TeacherAppToMacList.h"
#include "FormExamBlue.h"

CCriticalSection gCriticalSection;

CThreadFunHelper::CThreadFunHelper(void)
{
}

CThreadFunHelper::~CThreadFunHelper(void)
{
}

HRESULT CThreadFunHelper::GetBeginExamStudev(CFormExamBlue* _pForm,std::list<CStudev*>& _lstStudev)
{
	HRESULT hr = E_FAIL;
	CCriticalSectionControl CriticalSectionControl(&gCriticalSection);
	for(std::list<CTeacherAppToMacList*>::const_iterator itr = _pForm->m_lstAppToMacList.begin();
		itr != _pForm->m_lstAppToMacList.end();++itr)
	{
		CYDTeacherAppCom* pAppCom = (*itr)->m_pTeacherAppCom;
		ASSERT(pAppCom);
		for(std::list<CStudev*>::const_iterator itrStu = pAppCom->m_lstStuDev.begin();
			itrStu != pAppCom->m_lstStuDev.end();++itrStu)
		{
			int iRollCall = -1;
			hr = (*itrStu)->GetRollCall(iRollCall);
			if(FAILED(hr))
			{
				return hr;
			}
			if(iRollCall == 1)
			{
				_lstStudev.push_back(*itrStu);
			}
		}
	}

	return S_OK;
}


HRESULT CThreadFunHelper::GetRollingStudev(CFormExamBlue* _pForm,std::list<CStudev*>& _lstStudev)
{
	HRESULT hr = E_FAIL;
	CCriticalSectionControl CriticalSectionControl(&gCriticalSection);
	for(std::list<CTeacherAppToMacList*>::const_iterator itr = _pForm->m_lstAppToMacList.begin();
		itr != _pForm->m_lstAppToMacList.end();++itr)
	{
		CYDTeacherAppCom* pAppCom = (*itr)->m_pTeacherAppCom;
		ASSERT(pAppCom);
		for(std::list<CStudev*>::const_iterator itrStu = pAppCom->m_lstStuDev.begin();
			itrStu != pAppCom->m_lstStuDev.end();++itrStu)
		{
			int iRolling = -1;
			hr = (*itrStu)->GetRolling(iRolling);
			if(FAILED(hr))
			{
				return hr;
			}
			if(iRolling == 1)
			{
				_lstStudev.push_back(*itrStu);
			}
		}
	}
	return S_OK;
}

HRESULT CThreadFunHelper::UpdateListBeginExamByStudev(CStudev* _pStuDev,CFormExamBlue* _pForm,BOOL bError)
{
	HRESULT hr = E_FAIL;
	CCriticalSectionControl CriticalSectionControl(&gCriticalSection);
	ASSERT(_pStuDev);
	int iRollCall = -1;
	hr = _pStuDev->GetRollCall(iRollCall);
	if(FAILED(hr))
	{
		return hr;
	}
	if(iRollCall != 1 && !bError)
	{
		return S_OK;
	}
	CString strItrDevMac;
	hr = _pStuDev->GetMac(strItrDevMac);
	if(FAILED(hr))
	{
		return hr;
	}
	strItrDevMac.TrimRight();
	for(int j = 0; j < _pForm->m_lstCtrlUnit.GetItemCount(); j++)
	{
		CYDEAddrUnit* pUnit = (CYDEAddrUnit*)_pForm->m_lstCtrlUnit.GetItemData(j);
		ASSERT(pUnit);
		CYDDevice* pDevice = NULL;
		hr = pUnit->GetDevice(pDevice);
		if(FAILED(hr))
		{
			return hr;
		}
		CPtrAutoClean<CYDDevice> clr(pDevice);
		CString strDevMacID;
		hr = pDevice->GetPropVal(FIELD_YDDEVICE_MAC,strDevMacID);
		if(FAILED(hr))
		{
			return hr;
		}
		strDevMacID.TrimRight();
		if(strDevMacID.CompareNoCase(strItrDevMac) == 0)
		{
			int iRollCall = -1;
			hr = _pStuDev->GetRollCall(iRollCall);
			if(FAILED(hr))
			{
				return hr;
			}
			if(iRollCall == 1)
			{
				_pForm->m_lstCtrlUnit.SetItemText(j,4,_T("进入考试状态！"));
				COleDateTime tStart;
				hr = _pStuDev->GetStart(tStart);
				if(FAILED(hr))
				{
					return hr;
				}
				CString strStart = CDataHandler::DateTimeToStr(tStart,2);
				_pForm->m_lstCtrlUnit.SetItemText(j,5,strStart);
			}
			else if(bError)
			{
				//显示开考不成功
				_pForm->m_lstCtrlUnit.SetItemText(j,4,_T("开考失败！"));
			}
			break;
		}
	}
	return S_OK;
}

HRESULT CThreadFunHelper::UpdateListRollingByStudev(CStudev* _pStuDev,CFormExamBlue* _pForm,BOOL _bError)
{
	HRESULT hr = E_FAIL;
	CCriticalSectionControl CriticalSectionControl(&gCriticalSection);
	ASSERT(_pStuDev);
	int iRolling = -1;
	hr = _pStuDev->GetRolling(iRolling);
	if(FAILED(hr))
	{
		return hr;
	}
	if(iRolling != 1 && !_bError)
	{
		return S_OK;
	}
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	CString strItrDevMac;
	hr =  _pStuDev->GetMac(strItrDevMac);
	strItrDevMac.TrimRight();
	for(int j = 0; j < _pForm->m_lstCtrlUnit.GetItemCount(); j++)
	{
		CYDEAddrUnit* pUnit = (CYDEAddrUnit*)_pForm->m_lstCtrlUnit.GetItemData(j);
		ASSERT(pUnit);
		CYDDevice* pDevice = NULL;
		hr = pUnit->GetDevice(pDevice);
		if(FAILED(hr))
		{
			return hr;
		}
		CPtrAutoClean<CYDDevice> clr(pDevice);
		CString strDevMacID;
		hr = pDevice->GetPropVal(FIELD_YDDEVICE_MAC,strDevMacID);
		if(FAILED(hr))
		{
			return hr;
		}
		strDevMacID.TrimRight();
		if(strDevMacID.CompareNoCase(strItrDevMac) == 0)
		{
			if(iRolling == 1)
			{
				_pForm->m_lstCtrlUnit.SetItemText(j,4,_T("已收卷！"));
				COleDateTime tEnd;
				hr = _pStuDev->GetEnd(tEnd);
				if(FAILED(hr))
				{
					return hr;
				}
				CString strEnd = CDataHandler::DateTimeToStr(tEnd,2);
				_pForm->m_lstCtrlUnit.SetItemText(j,6,strEnd);
				if(pGExamStruct->m_pExamDetail->m_uExamKind == YDEXAM_KIND_TEST)
				{
					//一般测试时，收卷成功要将checkbox选中取消
					_pForm->m_lstCtrlUnit.SetCheck(j,FALSE);
				}
			}
			else if(_bError)
			{
				//显示收卷失败
				_pForm->m_lstCtrlUnit.SetItemText(j,4,_T("收卷失败"));
			}
			break;
		}
	}
	return S_OK;
}

HRESULT CThreadFunHelper::SaveStudevAnswer(CStudev* _pStuDev,CFormExamBlue* _pForm,
						 CInvigilateInfo* _pInvigilateInfo,CDatabaseEx* _pDB)
{
	HRESULT hr = E_FAIL;
	CCriticalSectionControl CriticalSectionControl(&gCriticalSection);
	ASSERT(_pStuDev);
	ASSERT(_pForm);
	ASSERT(_pInvigilateInfo);
	ASSERT(_pDB);
	int iRolling = -1;
	hr = _pStuDev->GetRolling(iRolling);
	if(FAILED(hr))
	{
		return hr;
	}
	if(iRolling != 1)
	{
		return S_OK;
	}
	BOOL bWriteDB = FALSE;
	hr = _pStuDev->GetIsWriteDB(bWriteDB);
	if(FAILED(hr))
	{
		return hr;
	}
	if(bWriteDB)
	{
		//先前已经插入过数据库了，不要再插入
		return S_OK;
	}
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	CString strItrDevMac;
	hr = _pStuDev->GetMac(strItrDevMac);
	strItrDevMac.TrimRight();
	for(int j = 0; j < _pForm->m_lstCtrlUnit.GetItemCount(); j++)
	{
		CYDEAddrUnit* pUnit = (CYDEAddrUnit*)_pForm->m_lstCtrlUnit.GetItemData(j);
		ASSERT(pUnit);
		CYDDevice* pDevice = NULL;
		hr = pUnit->GetDevice(pDevice);
		if(FAILED(hr))
		{
			return hr;
		}
		CPtrAutoClean<CYDDevice> clr(pDevice);
		CString strDevMacID;
		hr = pDevice->GetPropVal(FIELD_YDDEVICE_MAC,strDevMacID);
		if(FAILED(hr))
		{
			return hr;
		}
		strDevMacID.TrimRight();
		if(strDevMacID.CompareNoCase(strItrDevMac) == 0)
		{
			CString strAnswer;
			hr = _pStuDev->GetAnswer(strAnswer);
			if(FAILED(hr))
			{
				return hr;
			}
			hr = _pInvigilateInfo->SaveAnswer(_pDB,pUnit,
				pGExamStruct->m_pExamDetail->m_pPaper,
				pGExamStruct->m_pExamDetail->m_pSubject,
				strAnswer);
			if(FAILED(hr))
			{
				return hr;
			}
			hr = _pStuDev->SetIsWriteDB(TRUE);
			if(FAILED(hr))
			{
				return hr;
			}
			break;
		}
	}
	return S_OK;
}


