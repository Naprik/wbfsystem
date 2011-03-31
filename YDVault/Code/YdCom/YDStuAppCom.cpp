#include "StdAfx.h"
#include "YDStuAppCom.h"
#include "../Include\YDVaultError.h"
#include "Studev.h"
#include "../Base/DataHandler.h"
#include "../Base\AutoClean.h"
#include "StaticYdComApp.h"
#include "ExamQuestionSegment.h"

CYDStuAppCom::CYDStuAppCom(CString _strCom)
:CYDAppCom(_strCom)
{
	m_bBeginExam = FALSE;
	m_bRolling = FALSE;
}

CYDStuAppCom::~CYDStuAppCom(void)
{
}

HRESULT  CYDStuAppCom::FindStucom(std::list<CYDStuAppCom*>& _lstStuAppCom)
{
	HRESULT hr = E_FAIL;
	{
		CListAutoClean<CYDStuAppCom> clr(_lstStuAppCom);
	}
	for(int i = 1; i < 15; i++)
	{
		CString strTemp;
		strTemp.Format(_T("com%d"),i);
		CYDStuAppCom* pAppCom = new CYDStuAppCom(strTemp);
		hr = pAppCom->OpenCom();
		if(FAILED(hr))
		{
			CPtrAutoClean<CYDStuAppCom> clr(pAppCom);
			if(hr == E_HRESULT_EXAM_ERROR_OPENCOM)
			{
				continue;
			}
			else
			{
				return hr;
			}
		}
		hr = pAppCom->Close();
		if(FAILED(hr))
		{
			return hr;
		}
		_lstStuAppCom.push_back(pAppCom);
	}
	return S_OK;
}

HRESULT CYDStuAppCom::OpenCom()
{
	HRESULT hr = E_FAIL;
	hr = CYDAppCom::OpenCom();
	if(FAILED(hr))
	{
		return hr;
	}

	CString strRecv;
	int iType = -1;
	hr = GetDevMac(strRecv,iType);
	if(FAILED(hr))
	{
		Close();
		return E_HRESULT_EXAM_ERROR_OPENCOM;
	}
	if(iType == 0)
	{
		Close();
		return E_HRESULT_EXAM_ERROR_OPENCOM;
	}
	m_strMac = strRecv;

	return S_OK;
}
HRESULT CYDStuAppCom::Linksuccess()
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	CString strCMD = _T("a3 00 00 00 0d 0d 0a");
	CString strRecv;
	hr = contrl(strCMD, 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv != _T("b3 00 00 00 0d 0d 0a "))
	{
		return E_HRESULT_EXAM_ERROR_LINKSUCCESS;
	}
	m_bRolling = TRUE;
	m_timeRolling = COleDateTime::GetCurrentTime();
	return S_OK;

}

HRESULT CYDStuAppCom::Sendexaminfo()
{
	HRESULT hr = E_FAIL;
	hr = CYDAppCom::Sendexaminfo();
	if(FAILED(hr))
	{
		return hr;
	}
	m_bBeginExam = TRUE;
	m_timeBeginExam = COleDateTime::GetCurrentTime(); 
	return S_OK;
}

/********************************************************
函数名：Getanswer
参数：CArray<CStudev, CStudev>  &arry 学生动态数组
iStartNO          开始的题号
iEndNO            结束的题号
返回值：TRUE    获取答案失败
FALSE   成功
说明：对得到的答案进行解码，存放到学生动态数组中
********************************************************/

HRESULT CYDStuAppCom::LinkRolling(CString &_str)
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	int iStartNO = 100;
	int iEndNO = 0;
	for(std::list<CExamQuestionSegment*>::const_iterator itr = pGExamStruct->m_pExamDetail->m_lstQSeg.begin();
		itr != pGExamStruct->m_pExamDetail->m_lstQSeg.end();++itr)
	{
		if(iStartNO > (*itr)->m_iStart)
		{
			iStartNO = (*itr)->m_iStart;
		}
		if(iEndNO < (*itr)->m_iEnd)
		{
			iEndNO  = (*itr)->m_iEnd;
		}
	}
	
	CString strCMD = _T("8c 00 00 02 ");
	CString strStartNO = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iStartNO));
	CString strEndNO = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iEndNO));
	strCMD = strCMD + strStartNO + strEndNO + _T("0d 0d 0a");
	
	CString strRecv;
	hr = contrl(strCMD, 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T(""))
	{
		_str = _T("没有接收到答案");
		return S_FALSE;
	}
	int iCmdlength = strRecv.GetLength();
	CString strTotalanswer = strRecv.Mid(12, iCmdlength-12-15);

	BYTE out[1000];
	hr = Decode(strTotalanswer,out,0,199);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = TransferAnswer(out,_str);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

