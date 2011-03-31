#include "StdAfx.h"
#include "ThreadFunOperate.h"
#include "YDProgressControl.h"
#include "ThreadFunHelper.h"
#include "ThreadTeachComParam.h"
#include "../Base\AutoClean.h"
#include "YDThreadFun.h"
#include "InvigilateInfo.h"
#include "../ObjRef\YDObjectRef.h"
#include "../Base\DataHandler.h"
#include "../DBBase\DBTransactionRef.h"
#include "../UIBase/ControlWnd.h"
#include "../UIBase\ListBoxEx.h"
#include "../YDExamObjRef\YDDevice.h"
#include "../YDExamObjRef\YDEAddrUnit.h"
#include "TeacherAppToMacList.h"
#include "FormExamBlue.h"


CThreadFunOperate::CThreadFunOperate(CFormExamBlue* _pForm)
{
	m_pForm = _pForm;
}

CThreadFunOperate::~CThreadFunOperate(void)
{
}

HRESULT CThreadFunOperate::SearchThread()
{
	HRESULT hr = E_FAIL;
	CListBoxEx* pMsgListBox = (CListBoxEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_EXAM_MSG_LISTBOX);
	ASSERT(pMsgListBox);
	CYDProgressControl YDProgressControl(m_pForm,TRUE);
	CWnd* pSearchButton = m_pForm->GetDlgItem(IDC_BUTTON_SEARCH);
	ASSERT(pSearchButton);
	CControlWnd ControlWnd1(pSearchButton);
	CWnd* pBeginExamButton = m_pForm->GetDlgItem(IDC_BUTTON_BEGINEXAM);
	ASSERT(pBeginExamButton);
	CControlWnd ControlWnd2(pBeginExamButton);
	CWnd* pRollingButton = m_pForm->GetDlgItem(IDC_BUTTON_ROLLING);
	ASSERT(pRollingButton);
	CControlWnd ControlWnd3(pRollingButton);

	std::list<CYDObjectRef*> lstStuUnit;
	for(int i = 0; i < m_pForm->m_lstCtrlUnit.GetItemCount(); i++)
	{
		CYDObjectRef* pObjRef = (CYDObjectRef*)m_pForm->m_lstCtrlUnit.GetItemData(i);
		ASSERT(pObjRef);
		lstStuUnit.push_back(pObjRef);
	}
	YDProgressControl.SetRange(10,TRUE);
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	//搜索时只需要用第一个已经打开的教师机就可以了
	ASSERT(pGExamStruct->m_lstTacherAppCom.size() > 0);
	CYDTeacherAppCom* pFirstOpenAppCom = NULL;
	for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
		itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
	{
		BOOL bOpen = FALSE;
		hr = (*itr)->GetState(bOpen);
		if(FAILED(hr))
		{
			return hr;
		}
		if(!bOpen)
		{
			continue;
		}
		pFirstOpenAppCom = (*itr);
		break;
	}
	ASSERT(pFirstOpenAppCom);

	for (int i = 0; i < m_pForm->m_lstCtrlUnit.GetItemCount(); i++)
	{
		CYDEAddrUnit* pUnit = (CYDEAddrUnit*)m_pForm->m_lstCtrlUnit.GetItemData(i);
		ASSERT(pUnit);
		m_pForm->m_lstCtrlUnit.SetCheck(i,FALSE);
		m_pForm->m_lstCtrlUnit.SetItemText(i,4,_T(""));
	}

	BOOL bFirst = TRUE;
	DWORD dwStart = GetTickCount();
	do
	{
		int t1 = lstStuUnit.size();
		std::list<CString> lstStuMac;//搜索到的学生机的Mac地址

		if(bFirst)
		{
			bFirst = FALSE;
			hr = pFirstOpenAppCom->Reset();
			if (FAILED(hr))
			{
				hr = pFirstOpenAppCom->Reset();
				if (FAILED(hr))
				{
					return hr;
				}
			}
			Sleep(1000);
			hr = pFirstOpenAppCom->InitSearchStu();
			if(FAILED(hr))
			{
				return hr;
			}
		}
		Sleep(2000);
		hr =pFirstOpenAppCom->GetStudev(lstStuMac);
		if(FAILED(hr))
		{
			hr = pFirstOpenAppCom->GetStudev(lstStuMac);
			if (FAILED(hr))
			{
				return hr;
			}
		}
		int t2 = lstStuMac.size();
		CString strMessage;
		strMessage.Format(_T("共搜索到有效学生机%d台！"), lstStuMac.size());
		pMsgListBox->AddString(strMessage);

		//for (int i = 0; i < m_pForm->m_lstUnit.GetItemCount(); i++)
		//{
		//	CYDEAddrUnit* pUnit = (CYDEAddrUnit*)m_pForm->m_lstUnit.GetItemData(i);
		//	ASSERT(pUnit);
		//	m_pForm->m_lstUnit.SetCheck(i,FALSE);
		//	m_pForm->m_lstUnit.SetItemText(i,4,_T(""));
		//}

		UINT uValidateStu = 0;//当前搜索到的学生机，是属性当前列表上的个数
		for(std::list<CString>::const_iterator itr = lstStuMac.begin();
			itr != lstStuMac.end();++itr)
		{
			CString strStuMac = (*itr);
			strStuMac.TrimRight();
			for(int j = 0; j < m_pForm->m_lstCtrlUnit.GetItemCount(); j++)
			{
				CYDEAddrUnit* pUnit = (CYDEAddrUnit*)m_pForm->m_lstCtrlUnit.GetItemData(j);
				ASSERT(pUnit);
				CYDDevice* pDev = NULL;
				hr = pUnit->GetDevice(pDev);
				if(FAILED(hr))
				{
					return hr;
				}
				CPtrAutoClean<CYDDevice> clr(pDev);
				CString strListid ;
				hr = pDev->GetPropVal(FIELD_YDDEVICE_MAC,strListid);
				if(FAILED(hr))
				{
					return hr;
				}
				strListid.TrimRight();
				if(strListid.CompareNoCase(strStuMac) == 0)
				{
					m_pForm->m_lstCtrlUnit.SetCheck(j);
					m_pForm->m_lstCtrlUnit.SetItemText(j,4,_T("检测到！"));
					uValidateStu++;
					break;
				}
			}
		}
		CString strInfo;
		strInfo.Format(_T("共检测到学生机%d台"),uValidateStu);
		m_pForm->GetDlgItem(IDC_EDIT_INFO)->SetWindowText(strInfo);
		BOOL bAllFind = TRUE;//是否所有的学生都已经收到
		for(int i = 0; i < m_pForm->m_lstCtrlUnit.GetItemCount(); i++)
		{
			if(!m_pForm->m_lstCtrlUnit.GetCheck(i))
			{
				bAllFind = FALSE;
			}
		}
		if(bAllFind)
		{
			break;//退出循环，因为所有的学生都已经找到
		}
		//要判断是否到一分钟,如果满一分钟退出循环
		DWORD dwSpan = (GetTickCount() - dwStart) / 1000;
		if(dwSpan > 60)
		{
			break;//退出循环，超过一分钟
		}
		if(m_pForm->m_bCancelThread)
		{
			//用户点了取消按钮，退出循环
			break;
		}
	}while(TRUE);
	return S_OK;
}
HRESULT CThreadFunOperate::BeginExamThread()
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pForm);
	CWnd* pSearchButton = m_pForm->GetDlgItem(IDC_BUTTON_SEARCH);
	ASSERT(pSearchButton);
	CControlWnd ControlWnd1(pSearchButton);
	CWnd* pBeginExamButton = m_pForm->GetDlgItem(IDC_BUTTON_BEGINEXAM);
	ASSERT(pBeginExamButton);
	CControlWnd ControlWnd2(pBeginExamButton);
	CWnd* pRollingButton = m_pForm->GetDlgItem(IDC_BUTTON_ROLLING);
	ASSERT(pRollingButton);
	CControlWnd ControlWnd3(pRollingButton);
	CYDProgressControl YDProgressControl(m_pForm);
	int iCount = 0;
	for(std::list<CTeacherAppToMacList*>::const_iterator itr = m_pForm->m_lstAppToMacList.begin();
		itr != m_pForm->m_lstAppToMacList.end();++itr)
	{
		CYDTeacherAppCom* pAppCom = (*itr)->m_pTeacherAppCom;
		ASSERT(pAppCom);
		iCount += pAppCom->m_lstStuDev.size();
	}
	YDProgressControl.SetRange(iCount);
	CThreadFunHelper ThreadFunHelper;
	std::list<CThreadTeachComParam*> lstComParam;
	CListAutoClean<CThreadTeachComParam> clr1(lstComParam);
	ASSERT(m_pForm->m_lstAppToMacList.size() > 0);
	int iThreadCount = m_pForm->m_lstAppToMacList.size();
	ASSERT(iThreadCount);
	HANDLE* pHandleThread = new HANDLE[iThreadCount];
	int iThread = 0;
	for(std::list<CTeacherAppToMacList*>::const_iterator itr = m_pForm->m_lstAppToMacList.begin();
		itr != m_pForm->m_lstAppToMacList.end();++itr,iThread++)
	{
		CYDTeacherAppCom* pAppCom = (*itr)->m_pTeacherAppCom;
		ASSERT(pAppCom);
		CThreadTeachComParam* pComParam = new CThreadTeachComParam;
		pComParam->m_pAppCom = pAppCom;
		pComParam->m_pForm = m_pForm;
		pComParam->m_pYDProgressControl = &YDProgressControl;
		lstComParam.push_back(pComParam);
		//启动线程
		CWinThread* pThread = ::AfxBeginThread((AFX_THREADPROC)BeginExamThreadByTeachcom, pComParam);
		if (!pThread)	
		{
			ASSERT(FALSE);
			return -1;
		}
		pHandleThread[iThread] = pThread->m_hThread;
	}
	WaitForMultipleObjects(iThreadCount, pHandleThread, TRUE, INFINITE);
	delete[] pHandleThread;
	Sleep(500);
	YDProgressControl.SetRange(iCount);
	for(std::list<CTeacherAppToMacList*>::const_iterator itr = m_pForm->m_lstAppToMacList.begin();
		itr != m_pForm->m_lstAppToMacList.end();++itr)
	{
		CYDTeacherAppCom* pAppCom = (*itr)->m_pTeacherAppCom;
		ASSERT(pAppCom);
		for(std::list<CStudev*>::const_iterator itrDev = pAppCom->m_lstStuDev.begin();
			itrDev != pAppCom->m_lstStuDev.end();++itrDev)
		{
			hr = ThreadFunHelper.UpdateListBeginExamByStudev(*itrDev,m_pForm,FALSE);
			if(FAILED(hr))
			{
				return hr;
			}
			YDProgressControl.StepIt();
		}
	}
	{
		//要显示开考成功和失败各是多少人
		UINT uSucRolling = 0;
		UINT uErrRolling = 0;
		for(std::list<CTeacherAppToMacList*>::const_iterator itr = m_pForm->m_lstAppToMacList.begin();
			itr != m_pForm->m_lstAppToMacList.end();++itr)
		{
			CYDTeacherAppCom* pAppCom = (*itr)->m_pTeacherAppCom;
			ASSERT(pAppCom);
			for(std::list<CStudev*>::const_iterator itrDev = pAppCom->m_lstStuDev.begin();
				itrDev != pAppCom->m_lstStuDev.end();++itrDev)
			{
				int iItrRollCall = -1;
				hr = (*itrDev)->GetRollCall(iItrRollCall);
				if(FAILED(hr))
				{
					return hr;
				}
				if(iItrRollCall == 1)
				{
					uSucRolling ++;
				}
				else
				{
					uErrRolling++;
				}
				hr = ThreadFunHelper.UpdateListBeginExamByStudev(*itrDev,m_pForm,TRUE);
				if(FAILED(hr))
				{
					return hr;
				}
			}
		}
		CString strInfo;
		strInfo.Format(_T("开考成功%d台，失败%d台"),uSucRolling,uErrRolling);
		m_pForm->GetDlgItem(IDC_EDIT_INFO)->SetWindowText(strInfo);
	}
	return S_OK;
}

HRESULT CThreadFunOperate::RollingThread()
{
	HRESULT hr = E_FAIL;
	CWnd* pSearchButton = m_pForm->GetDlgItem(IDC_BUTTON_SEARCH);
	ASSERT(pSearchButton);
	CControlWnd ControlWnd1(pSearchButton);
	CWnd* pBeginExamButton = m_pForm->GetDlgItem(IDC_BUTTON_BEGINEXAM);
	ASSERT(pBeginExamButton);
	CControlWnd ControlWnd2(pBeginExamButton);
	CWnd* pRollingButton = m_pForm->GetDlgItem(IDC_BUTTON_ROLLING);
	ASSERT(pRollingButton);
	CControlWnd ControlWnd3(pRollingButton);
	CYDProgressControl YDProgressControl(m_pForm);
	int iCount = 0;
	for(std::list<CTeacherAppToMacList*>::const_iterator itr = m_pForm->m_lstAppToMacList.begin();
		itr != m_pForm->m_lstAppToMacList.end();++itr)
	{
		CYDTeacherAppCom* pAppCom = (*itr)->m_pTeacherAppCom;
		ASSERT(pAppCom);
		iCount += pAppCom->m_lstStuDev.size();
	}
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	int iStart = 0,iEnd = 0;
	hr = pGExamStruct->m_pExamDetail->GetMinMaxNo(iStart,iEnd);
	if(FAILED(hr))
	{
		return hr;
	}
	CThreadFunHelper ThreadFunHelper;
	CInvigilateInfo InvigilateInfo;
	YDProgressControl.SetRange(iCount);
	if(!pGExamStruct->m_bExtractOnlyFromTeacher)
	{
		
		int times = 0;
		std::list<CThreadTeachComParam*> lstComParam;
		CListAutoClean<CThreadTeachComParam> clr1(lstComParam);
		ASSERT(m_pForm->m_lstAppToMacList.size() > 0);
		int iThreadCount = m_pForm->m_lstAppToMacList.size();
		ASSERT(iThreadCount);
		HANDLE* pHandleThread = new HANDLE[iThreadCount];
		int iThread = 0;
		for(std::list<CTeacherAppToMacList*>::const_iterator itr = m_pForm->m_lstAppToMacList.begin();
			itr != m_pForm->m_lstAppToMacList.end();++itr,iThread++)
		{
			CYDTeacherAppCom* pAppCom = (*itr)->m_pTeacherAppCom;
			ASSERT(pAppCom);
			CThreadTeachComParam* pComParam = new CThreadTeachComParam;
			pComParam->m_pAppCom = pAppCom;
			pComParam->m_pForm = m_pForm;
			pComParam->m_pYDProgressControl = &YDProgressControl;
			pComParam->m_iStart = iStart;
			pComParam->m_iEnd = iEnd;
			lstComParam.push_back(pComParam);
			//启动线程
			CWinThread* pThread = ::AfxBeginThread((AFX_THREADPROC)RollingThreadByTeachcom, pComParam);
			if (!pThread)	
			{
				ASSERT(FALSE);
				return -1;
			}
			pHandleThread[iThread] = pThread->m_hThread;
		}
		WaitForMultipleObjects(iThreadCount, pHandleThread, TRUE, INFINITE);
		delete[] pHandleThread;
		Sleep(1000);
	}

	YDProgressControl.SetRange(iCount);
	for(std::list<CTeacherAppToMacList*>::const_iterator itr = m_pForm->m_lstAppToMacList.begin();
		itr != m_pForm->m_lstAppToMacList.end();++itr)
	{
		CYDTeacherAppCom* pAppCom = (*itr)->m_pTeacherAppCom;
		ASSERT(pAppCom);
		CString strAnswer;
		hr = pAppCom->Getanswer(pGExamStruct->m_pExamDetail->m_lstQSeg,strAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
		//将列表上所有的学生机用新取到的成绩更新
		for(std::list<CStudev*>::const_iterator itrDev = pAppCom->m_lstStuDev.begin();
			itrDev != pAppCom->m_lstStuDev.end();++itrDev)
		{
			hr = ThreadFunHelper.UpdateListRollingByStudev(*itrDev,m_pForm,FALSE);
			if(FAILED(hr))
			{
				return hr;
			}
			YDProgressControl.StepIt();
		}

	}
	CYDObjectRef* pObjSubject = pGExamStruct->m_pExamDetail->m_pSubject;//考试科目
	ASSERT(pObjSubject);
	CString strSubjectName;
	hr = pObjSubject->GetPropVal(FIELD_YDSUBJECT_NAME,strSubjectName);
	if(FAILED(hr))
	{
		return hr;
	}
	CYDObjectRef* pObjPaper = pGExamStruct->m_pExamDetail->m_pPaper;//考试试卷
	ASSERT(pObjPaper);
	CComVariant valPaperId;
	hr = pObjPaper->GetPropVal(FIELD_YDRESULT_PAPERID,&valPaperId);
	if(FAILED(hr))
	{
		return hr;
	}
	CYDObjectRef* pObjAddress = pGExamStruct->m_pExamDetail->m_pObjAddress;//考场
	ASSERT(pObjAddress);
	CString strAddress;
	hr = pObjAddress->GetPropVal(FIELD_YDEXAMADDR_ADDR,strAddress);
	if(FAILED(hr))
	{
		return hr;
	}
	int nPaperID = CDataHandler::VariantToLong(valPaperId);
	pGExamStruct->m_pExamDetail->m_bIsRollingDone = TRUE;
	//将成绩保存到xml
	ASSERT(!pGExamStruct->m_pExamDetail->m_strXmlFile.IsEmpty());
	hr = InvigilateInfo.Save(CComBSTR(pGExamStruct->m_pExamDetail->m_strXmlFile));
	if(FAILED(hr))
	{
		return hr;
	}
	pGExamStruct->m_pExamDetail->m_nSetp = YDEXAM_STEP_ROLLING;
	CString strMsg;
	{
		//要显示收卷成功和失败各是多少人
		UINT uSucRolling = 0;
		UINT uErrRolling = 0;
		for(std::list<CTeacherAppToMacList*>::const_iterator itr = m_pForm->m_lstAppToMacList.begin();
			itr != m_pForm->m_lstAppToMacList.end();++itr)
		{
			CYDTeacherAppCom* pAppCom = (*itr)->m_pTeacherAppCom;
			ASSERT(pAppCom);
			for(std::list<CStudev*>::const_iterator itrDev = pAppCom->m_lstStuDev.begin();
				itrDev != pAppCom->m_lstStuDev.end();++itrDev)
			{
				int iItrRolling = -1;
				hr = (*itrDev)->GetRolling(iItrRolling);
				if(FAILED(hr))
				{
					return hr;
				}
				if(iItrRolling == 1)
				{
					uSucRolling ++;
				}
				else
				{
					uErrRolling++;
				}
				hr = ThreadFunHelper.UpdateListRollingByStudev(*itrDev,m_pForm,TRUE);
				if(FAILED(hr))
				{
					return hr;
				}
			}
		}
		CString strInfo;
		strInfo.Format(_T("收卷成功%d台，失败%d台"),uSucRolling,uErrRolling);
		m_pForm->GetDlgItem(IDC_EDIT_INFO)->SetWindowText(strInfo);
		strMsg.Format(_T("收卷完成，成功%d台,失败%d台!"),uSucRolling,uErrRolling);
	}

	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	{
		//状态更新成功
		//把监考信息数据插入到数据库中
		//先判断这个监考信息表在数据库中是否已经存在
		VARIANT_BOOL bExist = VARIANT_FALSE;
		hr = InvigilateInfo.IsExist(pDB,&bExist);
		if(FAILED(hr))
		{
			return hr;
		}
		if(bExist)
		{
			if (AfxMessageBox(_T("当前监考信息已经在数据库中存在，是否继续导入？"),MB_YESNO|MB_ICONINFORMATION) != IDYES)
			{
				return S_FALSE;
			}

		}

		CDBTransactionRef TransRef(pDB,TRUE);
		hr = InvigilateInfo.SaveInvigilateDB(pDB);
		if(FAILED(hr))
		{
			return hr;
		}
		for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
			itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
		{
			BOOL bOpen = FALSE;
			hr = (*itr)->GetState(bOpen);
			if(FAILED(hr))
			{
				return hr;
			}
			//将列表上所有的学生机用新取到的成绩写入到数据库
			for(std::list<CStudev*>::const_iterator itrDev = (*itr)->m_lstStuDev.begin();
				itrDev != (*itr)->m_lstStuDev.end();++itrDev)
			{
				int iRolling = -1;
				hr = (*itrDev)->GetRolling(iRolling);
				if(FAILED(hr))
				{
					return hr;
				}
				if(iRolling == 1)
				{
					hr = ThreadFunHelper.SaveStudevAnswer(*itrDev,m_pForm,&InvigilateInfo,pDB);
					if(FAILED(hr))
					{
						return hr;
					}
				}
			
			}

		}
		hr = TransRef.Commit();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	AfxMessageBox(strMsg);
	return S_OK;
}