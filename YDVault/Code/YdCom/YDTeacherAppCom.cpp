// YDAppCom.cpp: implementation of the CYDTeacherAppCom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YDTeacherAppCom.h"
#include "afxtempl.h"
#include "../Include\YDVaultError.h"
#include "../UIBase\ListBoxEx.h"
#include "ExamQuestionSegment.h"
#include "../Base/DataHandler.h"
#include "../Base\AutoClean.h"
#include "Studev.h"
#include "StaticYdComApp.h"
#include "SearchDevCom.h"
#include <list>
#include "../Base\CriticalSectionControl.h"


CYDTeacherAppCom::CYDTeacherAppCom(CString _strCom)
:CYDAppCom(_strCom)
{
	isSearch = false;
//	CJahFile    m_file;
//	m_file.Remove(GetExePath());

	//getpath();
}

CYDTeacherAppCom::~CYDTeacherAppCom()
{
	CListAutoClean<CStudev> clr(m_lstStuDev);
}


HRESULT CYDTeacherAppCom::OpenCom()
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
	if(iType == 1)
	{
		Close();
		return E_HRESULT_EXAM_ERROR_OPENCOM;
	}
	m_strMac = strRecv;

	return S_OK;
}


HRESULT CYDTeacherAppCom::ResetSYS()
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	CString strRecv = _T("");
	hr = contrl(_T("a7 00 00 00 0d 0d 0a "), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T("b7 00 00 00 0d 0d 0a "))
	{
		return S_OK;
	}
	return E_HRESULT_EXAM_ERROR_RESETSYS;
}

HRESULT CYDTeacherAppCom::Resetmac()
{
	m_com.Read();
	HRESULT hr = E_FAIL;
	CString strRecv = _T("");
	hr = contrl(_T("a9 00 00 00 0d 0d 0a "), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T("b9 00 00 00 0d 0d 0a "))
		return S_OK;
	return E_HRESULT_EXAM_ERROR_RESETMAC;
}

/******************************************
函数名：Reset
参数：
返回值：TRUE    复位成功
		FASLE   复位失败		
*******************************************/
HRESULT CYDTeacherAppCom::Reset()
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	
	CString strRecv = _T("");
	hr = contrl(_T("81 00 00 00 0d 0d 0a"), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
//	CString strRecv = "91 00 00 00 0d 0d 0a ";
	if(strRecv == _T("91 00 00 00 0d 0d 0a "))
	{
		return S_OK;
	}
	return E_HRESULT_EXAM_ERROR_RESET;
}
/************************************************
函数名：Initsearch
参数：
返回值：TRUE
		FALSE
说明：向教师机发送搜索指令，使教师机开始搜索学生机
************************************************/
HRESULT CYDTeacherAppCom::InitSearchStu()
{
	m_com.Read();
	HRESULT hr = E_FAIL;
	CString strRecv = _T("");
	hr = contrl(_T("82 00 00 00 0d 0d 0a"), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
//	CString strRecv = "92 00 00 00 0d 0d 0a ";
	if(strRecv == _T("92 00 00 00 0d 0d 0a "))
	{
		return S_OK;
	}
	return E_HRESULT_EXAM_ERROR_INITSEARCH;
}
/*****************************************************
函数名：GetStudev
参数：存放搜索到学生机地址的字符串数组
返回值：FALSE 没有得到搜索结果
		TRUE  得到搜索结果，包含没有搜索到学生机的情况
*****************************************************/
HRESULT CYDTeacherAppCom::GetStudev(std::list<CString> &_lstStrMa)
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	CString strRecv = _T("");
	hr = contrl(_T("83 00 00 00 0d 0d 0a"), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
//	CString strRecv = m_bk.Load("搜索到学生机地址",m_path);
//	strRecv += " ";
	if(strRecv == _T(""))
	{
		return E_HRESULT_EXAM_ERROR_GETSTUDEV;
	}
	int iCmdlength = strRecv.GetLength();
	CString strTotalmac = strRecv.Mid(12, iCmdlength-12-9);
	int iMaccount = (strTotalmac.GetLength()+1)/9;
	for(int i=0; i<iMaccount; i++)
	{
		CString strPerMca = strTotalmac.Mid(9*i,9);
		strPerMca.MakeUpper();
		_lstStrMa.push_back(strPerMca);
	}
	return S_OK;
}
/*******************************************
类名：Sendvalidmac
参数：字符串数组 arry， mac地址字符串
返回值：true 发送合法地址到教师机成功
		false 失败
说明：发送合法学生机蓝牙地址指令
*******************************************/
HRESULT CYDTeacherAppCom::Sendvalidmac(std::list<CString> &_lstStrMac)
{
	m_com.Read();
	HRESULT hr = E_FAIL;
	{
		CListAutoClean<CStudev> clr(m_lstStuDev);
	}
	for(std::list<CString>::const_iterator itr = _lstStrMac.begin();
		itr != _lstStrMac.end();++itr)
	{
		CStudev *pStudev = new CStudev;
		hr = pStudev->SetMac(*itr);
		if(FAILED(hr))
		{
			return hr;
		}
		m_lstStuDev.push_back(pStudev);
	}
	CString strCmd = _T("84 00 ");

	int iCount = _lstStrMac.size();
	int iStrlength = iCount * 3;
	CString strHexStrlength = _T("");
	strHexStrlength = CDataHandler::DectoHexstr(iStrlength);
	CString  strIndexHexlength ;
	strIndexHexlength = CDataHandler::FormatLength(strHexStrlength);
	CString strMac = _T("");
	for(std::list<CString>::const_iterator itr = _lstStrMac.begin();
		itr != _lstStrMac.end();++itr)
	{
		strMac += (*itr);
		strMac += _T(" ");
	}
	strCmd = strCmd + strIndexHexlength + strMac + _T("0d 0d 0a");
	
	CString  strRecv;
	hr =  contrl(strCmd, 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
//	CString  strRecv = "94 00 00 00 0d 0d 0a ";
	if(strRecv == _T("94 00 00 00 0d 0d 0a "))
	{
		return S_OK;
	}
	return E_HRESULT_EXAM_ERROR_SENDVALIDMAC;
}


HRESULT CYDTeacherAppCom::TestKinkSendvalidMac(std::list<CString> &_lstStrMac)
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	{
		CListAutoClean<CStudev> clr(m_lstStuDev);
	}
	for(std::list<CString>::const_iterator itr = _lstStrMac.begin();
		itr != _lstStrMac.end();++itr)
	{
		CStudev *pStudev = new CStudev;
		hr = pStudev->SetMac(*itr);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pStudev->SetEnd(COleDateTime::GetCurrentTime());
		if(FAILED(hr))
		{
			return hr;
		}
		m_lstStuDev.push_back(pStudev);
	}
	CString strCmd = _T("87 00 ");

	int iCount = _lstStrMac.size();
	int iStrlength = iCount * 3;
	CString strHexStrlength = _T("");
	strHexStrlength = CDataHandler::DectoHexstr(iStrlength);
	CString  strIndexHexlength ;
	strIndexHexlength = CDataHandler::FormatLength(strHexStrlength);
	CString strMac = _T("");
	for(std::list<CString>::const_iterator itr = _lstStrMac.begin();
		itr != _lstStrMac.end();++itr)
	{
		strMac += (*itr);
		strMac += _T(" ");
	}
	strCmd = strCmd + strIndexHexlength + strMac + _T("0d 0d 0a");

	CString  strRecv;
	hr =  contrl(strCmd, 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	//	CString  strRecv = "94 00 00 00 0d 0d 0a ";
	if(strRecv == _T("97 00 00 00 0d 0d 0a "))
	{
		return S_OK;
	}
	return E_HRESULT_EXAM_ERROR_SENDVALIDMAC;
}
HRESULT CYDTeacherAppCom::SendFirstMac(CString strMac)
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	CString strCmd = _T("84 00 00 04 00 ");
	strCmd = strCmd + strMac + _T(" 0d 0d 0a");
	
	CString  strRecv = _T("");
	hr = contrl(strCmd, 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T("94 00 00 00 0d 0d 0a "))
		return S_OK;
	return E_HRESULT_EXAM_ERROR_SENDFIRSTMAC;

}
/*******************************************************************
函数名：Sendsinglemac
参数：mac 需要发送的学生机地址
返回值：TRUE
		FALSE
说明：此函数只能使用在单独点名中，且必须先发送所有合法的地址给教师机，
	  此地址必须存在与这些地址当中
********************************************************************/
HRESULT CYDTeacherAppCom::Sendsinglemac(CString mac, int index,int& _iRetrun)
{
	HRESULT hr = E_FAIL;
	CString strCmd = _T("89 00 00 04 ");
	CString strHexIndex;
	strHexIndex = CDataHandler::DectoHexstr(index);
	CString  strIndexHexlength;
	strIndexHexlength =  CDataHandler::FormatNO(strHexIndex);
	strIndexHexlength.TrimRight();
	strCmd += strIndexHexlength + _T(" ") + mac + _T(" ") + _T("0d 0d 0a");
	CString  strRecv ;
	hr =  contrl(strCmd, 8,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
//	CString  strRecv = "99 00 00 00 0d 0d 0a ";
	if(strRecv == _T("99 00 00 00 0d 0d 0a "))
	{
		_iRetrun = 1;
	}
	else if(strRecv == _T("9c 00 00 00 0d 0d 0a "))
	{
		_iRetrun = 2;
	}
	else
	{
		_iRetrun =  0;
	}
	return S_OK;
}


/**********************************************
函数名：UpdateStudev
参数：该地址在教师机中的编号index；
      要更新的地址 		mac		
返回值：TRUE
		FALSE
说明：更新学生设备
***********************************************/
// HRESULT CYDTeacherAppCom::UpdateStudev(int index, CString mac)
// {
// 	m_com.Read();
// 	CString strCmd = _T("87 00 00 04 ");
// 	HRESULT hr = E_FAIL;
// 	CString  strIndexHexlength;
// 	strIndexHexlength  = DectoHexstr(index);
// 	CString strFormatNo;
// 	strFormatNo = FormatNO(strIndexHexlength);
// 	strCmd += strFormatNo + mac + _T(" ") + _T("0d 0d 0a");
// 	CString  strRecv;
// 	hr = contrl(strCmd, 5,strRecv);
// 	if(FAILED(hr))
// 	{
// 		return hr;
// 	}
// 	if(strRecv == _T("97 00 00 00 0d 0d 0a "))
// 		return S_OK;
// 	else
// 		return E_HRESULT_EXAM_ERROR_UPDATESTUDEV;
// }
/*********************************************************
函数名：Addmac
参数：index 新增的地址的编号
	  mac  新增的地址
返回值：TRUE
		FALSE
说明：将新增的地址加入教师机合法的地址中
**********************************************************/
HRESULT CYDTeacherAppCom::Addmac(int index, CString strMac)
{
	HRESULT hr = E_FAIL;
	m_com.Read();
/*	CString strCmd = "88 00 ";
	int iCount = arry.GetSize();
	int iStrlength = iCount *4;
	CString  strIndexHexlength = formatlength(DectoHexstr(iStrlength));
	CString strMac = "";
	for(int i = 0; i < iCount; i++)
	{
		//int index = arry[i].m_index;
		CString  strIndexHexlength = formatNO(DectoHexstr(index));
		index++;
		strMac += strIndexHexlength + arry[i] + " ";
	}
	strCmd = strCmd + strIndexHexlength + strMac + "0d 0d 0a";
	CString  strRecv = contrl(strCmd);
	if(strRecv == recvAddmac)
		return TRUE;
	else
		return FALSE;*/
	CString strCmd = _T("88 00 00 04 ");
	CString  strIndexHexlength ;
	strIndexHexlength =  CDataHandler::DectoHexstr(index);
	strMac.TrimRight();
	CString strFormatNo;
	strFormatNo = CDataHandler::FormatNO(strIndexHexlength);
	strCmd += strFormatNo + strMac + _T(" ") + _T("0d 0d 0a");	
	CString  strRecv;
	hr = contrl(strCmd, 5,strFormatNo);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T("98 00 00 00 0d 0d 0a "))
		return S_OK;
	else
		return E_HRESULT_EXAM_ERROR_ADDMAC;
}


HRESULT CYDTeacherAppCom::Sendexaminfo()
{
	HRESULT hr = E_FAIL;
	hr = CYDAppCom::Sendexaminfo();
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
	ASSERT(pGExamStruct);
	//修改学生机信息的开考时间
	for(std::list<CStudev*>::const_iterator itr = m_lstStuDev.begin();
		itr != m_lstStuDev.end();++itr)
	{
		hr = (*itr)->SetStart(pGExamStruct->m_pExamDetail->m_timeStart);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = (*itr)->SetEnd(pGExamStruct->m_pExamDetail->m_timeEnd);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CYDTeacherAppCom::Sendoffexaminfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO,int iStartNO,int iEndNO)
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	
	CString strCmd = _T("ab 00 00 0c ");
	COleDateTime currentimt = COleDateTime::GetCurrentTime();
	COleDateTimeSpan timeStartspan, timeEndspan;
	timeStartspan = timeStart - currentimt;
	timeEndspan = timeEnd - currentimt;
	int  iStartspan = (int)timeStartspan.GetTotalSeconds();
	if(iStartspan <= 0)
		iStartspan = 1;
	int  iEndspan = (int)timeEndspan.GetTotalSeconds();
	if(iEndspan <= 1)
		iEndspan = 2;
	int  iAlertspan = iEndspan - 300;                                    //考试提醒时间
	if(iAlertspan<1)
		iAlertspan = 1;

	int iStartHour = timeStart.GetHour();
	int iStartMin = timeStart.GetMinute();
	int iEndHour = timeEnd.GetHour();
	int iEndMin = timeEnd.GetMinute();

	CString strHexStartspan = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iStartspan));
	CString strHexEndspan = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iEndspan));
	CString strHexAlertspan = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iAlertspan));
	CString  strHexMaxNO = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iMaxNO));
	CString strHexStartHour = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iStartHour));
	CString strHexStartMin = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iStartMin));
	CString strHexEndHour = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iEndHour));
	CString strHexEndMin = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iEndMin));
	CString strHexStartNO = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iStartNO));
	CString strHexEndNO = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iEndNO));
		
	strCmd = strCmd + strHexStartspan + strHexAlertspan + strHexEndspan + strHexMaxNO +strHexStartHour+strHexStartMin+strHexEndHour+strHexEndMin+strHexStartNO+strHexEndNO+ _T("0d 0d 0a");
	
	CString  strRecv;
	hr =  contrl(strCmd, 8,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
//	CString  strRecv = "95 00 00 00 0d 0d 0a ";
	if(strRecv == _T("bb 00 00 00 0d 0d 0a "))
		return S_OK;
	else
		return E_HRESULT_EXAM_ERROR_SENDOFFEXAMINFO;
}

HRESULT CYDTeacherAppCom::SendSingleinfo(COleDateTime timeStart, COleDateTime timeEnd, int iMaxNO)
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	
	CString strCmd = _T("8a 00 00 0b ");
	COleDateTime currentimt = COleDateTime::GetCurrentTime();
	COleDateTimeSpan timeStartspan, timeEndspan;
	timeStartspan = timeStart - currentimt;
	timeEndspan = timeEnd - currentimt;
	int  iStartspan = (int)timeStartspan.GetTotalSeconds();
	if(iStartspan <= 0)
		iStartspan = 1;
	int  iEndspan = (int)timeEndspan.GetTotalSeconds();
	if(iEndspan <= 1)
		iEndspan = 2;
	int  iAlertspan = iEndspan - 300;                                    //考试提醒时间
	if(iAlertspan<1)
		iAlertspan = 1;

	int iStartHour = timeStart.GetHour();
	int iStartMin = timeStart.GetMinute();
	int iEndHour = timeEnd.GetHour();
	int iEndMin = timeEnd.GetMinute();

	CString strHexStartspan = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iStartspan));
	CString strHexEndspan = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iEndspan));
	CString strHexAlertspan = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iAlertspan));
	CString  strHexMaxNO = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iMaxNO));
	CString strHexStartHour = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iStartHour));
	CString strHexStartMin = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iStartMin));
	CString strHexEndHour = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iEndHour));
	CString strHexEndMin = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iEndMin));
		
	strCmd = strCmd + strHexStartspan + strHexAlertspan + strHexEndspan + strHexMaxNO +strHexStartHour+strHexStartMin+strHexEndHour+strHexEndMin+ _T("0d 0d 0a");
	
	CString  strRecv ;
	hr = contrl(strCmd, 8,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
//	CString  strRecv = "9a 00 00 00 0d 0d 0a ";
	if(strRecv == _T("9a 00 00 00 0d 0d 0a "))
		return S_OK;
	else
		return E_HRESULT_EXAM_ERROR_SENDSINGLEINFO;
}
/**************************************************************************
函数名：Sendstepexaminfo
参数：CArray<CStudev, CStudev>  arry学生信息列表
返回值：TRUE  重新发送成功
		FALSE 失败
说明：对失败的考生重新发送考试信息包，只要发送未成功的学生的在列表中的序号
***************************************************************************/

HRESULT CYDTeacherAppCom::Sendstepexaminfo(CArray<CStudev, CStudev>  &arry)
{

	HRESULT hr = E_FAIL;
	m_com.Read();
	
	CString strCmd = _T("86 00 ");
	int iCount = arry.GetSize();
	int iLength = 0;
	CString strIndex = _T("");
	for(int i = 0; i < iCount; i++)
	{
		int iRollCall = -1;
		hr = arry[i].GetRollCall(iRollCall);
		if(FAILED(hr))
		{
			return hr;
		}
		if(iRollCall == 2)
		{
			int index = -1;
			hr = arry[i].GetIndex(index);
			if(FAILED(hr))
			{
				return hr;
			}
			CString strIndexHexlength = CDataHandler::FormatNO(CDataHandler::DectoHexstr(index));
			strIndex += strIndexHexlength;
			iLength++;
		}
	}

	CString strHexlength = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iLength));
	strCmd += strHexlength+ strIndex + _T("0d 0d 0a");

	CString  strRecv ;
	hr = contrl(strCmd, 8,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T("96 00 00 00 0d 0d 0a "))
		return S_OK;
	else
		return E_HRESULT_EXAM_ERROR_SENDSTEPEXAMINFO;

}

/**********************************************************
函数名：Updatestat
参数：CArray<CStudev, CStudev>  &arry学生地址列表
返回值：
说明：更新列表中学生状态标志位
	  标准位分为2部分
	  前部分为 考试信息发送标志位00 建链失败 01 考试信息发送成功 02 失败
	  后部分为 收卷信息标准位00 建链失败 01 收卷成功 02 失败 03 提前交卷
**********************************************************/
HRESULT  CYDTeacherAppCom::Updatestat(BOOL &bRoll, BOOL &bRolling)
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	
	CString strRecv;
	hr = contrl(_T("8b 00 00 03 00 00 00 0d 0d 0a"), 2,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}


//	CString strRecv = "9b 00 00 34 00 01 01 01 02 01 03 01 04 01 05 01 06 01 07 01 08 01 09 01 0a 01 0b 01 0c 01 00 01 01 01 02 01 03 01 04 01 05 01 06 01 07 01 08 01 09 01 0a 01 0b 01 0c 01 0d 0d 0a ";
//	CString strRecv = m_bk.Load("状态标志位",m_path);
//	strRecv += " ";
	if(strRecv == _T(""))
		return E_HRESULT_EXAM_ERROR_UPDATESTAT;
	int iCount = m_lstStuDev.size();
	CString strRollStat = strRecv.Mid(12, 2);
	if(strRollStat == _T("00"))
		bRoll = FALSE;
	else if(strRollStat == _T("01"))
		bRoll = TRUE;
	else
		bRoll = FALSE;

/*		CString strRollingStat = strRecv.Mid(15+iCount*6,2);
		if(strRollingStat == "00")
			bRolling = FALSE;
		else if(strRollingStat == "01")
			bRolling = TRUE;
		else
			bRolling = FALSE;*/

	CString strRollingStat = strRecv.Mid(15,2);
	if(strRollingStat == _T("00"))
		bRolling = FALSE;
	else if(strRollingStat == _T("01"))
		bRolling = TRUE;
	else
		bRolling = FALSE;

	int iLength = strRecv.GetLength();
	CString strStat = strRecv.Mid(18, iLength-18);
	int iFirst = strStat.Find(_T("0d 0d 0a "));
	if(iFirst == -1)
	{
		return E_HRESULT_EXAM_ERROR_UPDATESTAT;
	}
	strStat = strStat.Left(iFirst);
	std::list<CStuMacState*> lstStuMacState;
	hr = ParseStuStr(strStat,lstStuMacState);
	if(FAILED(hr))
	{
		return hr;
	}
	CListAutoClean<CStuMacState> clr(lstStuMacState);

	int index = 0;
	for(std::list<CStuMacState*>::const_iterator itrMacState = lstStuMacState.begin();
		itrMacState != lstStuMacState.end();++itrMacState)
	{
		for(std::list<CStudev*>::const_iterator itr = m_lstStuDev.begin();
			itr != m_lstStuDev.end();++itr,index++)
		{
			CString strItrMac;
			hr = (*itr)->GetMac(strItrMac);
			if(FAILED(hr))
			{
				return hr;
			}
			if(strItrMac.CompareNoCase((*itrMacState)->m_strMac) == 0)
			{
				hr = (*itr)->SetRollCall((*itrMacState)->m_iBeginExam);
				if(FAILED(hr))
				{
					return hr;
				}
				hr = (*itr)->SetRolling((*itrMacState)->m_iRolling);
				if(FAILED(hr))
				{
					return hr;
				}
				break;
			}
		}
	}

	return S_OK;
}

HRESULT CYDTeacherAppCom::UpdateOnlyFromTeacher()
{
	HRESULT hr = E_FAIL;
	m_com.Read();

	CString strRecv;
	hr = contrl(_T("8b 00 00 03 00 00 00 0d 0d 0a"), 2,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}


	if(strRecv == _T(""))
		return E_HRESULT_EXAM_ERROR_UPDATESTAT;
	{
		CListAutoClean<CStudev> clr(m_lstStuDev);
	}
	int iLength = strRecv.GetLength();
	CString strStat = strRecv.Mid(18, iLength-18);
	int iFirst = strStat.Find(_T("0d 0d 0a "));
	if(iFirst == -1)
	{
		return E_HRESULT_EXAM_ERROR_UPDATESTAT;
	}
	strStat = strStat.Left(iFirst);
	std::list<CStuMacState*> lstStuMacState;
	hr = ParseStuStr(strStat,lstStuMacState);
	if(FAILED(hr))
	{
		return hr;
	}
	CListAutoClean<CStuMacState> clr(lstStuMacState);

	int index = 0;
	for(std::list<CStuMacState*>::const_iterator itrMacState = lstStuMacState.begin();
		itrMacState != lstStuMacState.end();++itrMacState)
	{
		CStudev* pStudev = new CStudev();
		hr = pStudev->SetMac((*itrMacState)->m_strMac);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pStudev->SetRollCall((*itrMacState)->m_iBeginExam);
		if(FAILED(hr))
		{
			return hr;
		}
		hr  = pStudev->SetRolling((*itrMacState)->m_iRolling);
		if(FAILED(hr))
		{
			return hr;
		}
		m_lstStuDev.push_back(pStudev);
	}

	return S_OK;
}


HRESULT  CYDTeacherAppCom::ParseStuStr(CString _str,std::list<CStuMacState*> &_lstStuMac)
{
	HRESULT hr = E_FAIL;

	while(_str.GetLength() >= 15)
	{
		CString strTemp = _str.Left(15);
		CString strMac = strTemp.Left(8);
		CStuMacState* pStuMac = new CStuMacState();
		pStuMac->m_strMac = strMac;
		CString strBeginExam = strTemp.Mid(9,2);
		if(strBeginExam.CompareNoCase(_T("00")) == 0)
		{
			pStuMac->m_iBeginExam = 0;
		}
		else if(strBeginExam.CompareNoCase(_T("01")) == 0)
		{
			pStuMac->m_iBeginExam = 1;
		}
		else 
		{
			pStuMac->m_iBeginExam = 2;
		}
		CString strRolling = strTemp.Mid(12,2);
		if(strRolling.CompareNoCase(_T("00")) == 0)
		{
			pStuMac->m_iRolling = 0;
		}
		else if(strRolling.CompareNoCase(_T("01")) == 0)
		{
			pStuMac->m_iRolling = 1;
		}
		else 
		{
			pStuMac->m_iRolling = 2;
		}
		_lstStuMac.push_back(pStuMac);
		_str = _str.Right(_str.GetLength() - 15);
	}
	return S_OK;
}

HRESULT CYDTeacherAppCom::IsRolldone(std::list<CStudev*>  &_lstStu)
{
	m_com.Read();
	HRESULT hr = E_FAIL;
	CString strRecv ;
	hr = contrl(_T("8b 00 00 00 0d 0d 0a"), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T(""))
		return E_HRESULT_EXAM_ERROR_ISROLLDONE;
	CString strStat = strRecv.Mid(12, 2);
	if(strStat == _T("00"))
	{
		return E_HRESULT_EXAM_ERROR_ISROLLDONE;
	}
	else if(strStat == _T("01"))
	{
		return S_OK;
	}
	else
		return E_HRESULT_EXAM_ERROR_ISROLLDONE;
}

HRESULT CYDTeacherAppCom::IsRollingdone(std::list<CStudev*>  &_lstStu)
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	CString strRecv;
	hr = contrl(_T("8b 00 00 00 0d 0d 0a"), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T(""))
		return E_HRESULT_EXAM_ERROR_ISROLLINGDONE;
	int iCount = _lstStu.size();
	CString strStat = strRecv.Mid(15+iCount*6,2);
	if(strStat == _T("00"))
	{
		return E_HRESULT_EXAM_ERROR_ISROLLINGDONE;
	}
	else if(strStat == _T("01"))
	{
		return S_OK;
	}
	else
		return E_HRESULT_EXAM_ERROR_ISROLLINGDONE;


}
/*****************************************
函数名：Rolling
参数：iStartNO 开始题号  
	  iEndNO   结束题号
返回值：true 
        false
******************************************/
HRESULT CYDTeacherAppCom::Rolling(std::list<CExamQuestionSegment*> &_lstQSeg)
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	
	CString strCMD = _T("8c 00 00 02 ");
	int iStartNo = 100;
	int iEndNo = 0;
	for(std::list<CExamQuestionSegment*>::const_iterator itr = _lstQSeg.begin();
		itr != _lstQSeg.end();++itr)
	{
		if(iStartNo > (*itr)->m_iStart)
		{
			iStartNo = (*itr)->m_iStart;
		}
		if(iEndNo < (*itr)->m_iEnd)
		{
			iEndNo = (*itr)->m_iEnd;
		}
	}
	CString strStartNO = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iStartNo));
	CString strEndNO = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iEndNo));
	strCMD = strCMD + strStartNO + strEndNO + _T("0d 0d 0a");
	
	CString strRecv ;
	hr = contrl(strCMD, 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
//	CString strRecv = "9c 00 00 00 0d 0d 0a ";
	if(strRecv == _T("9c 00 00 00 0d 0d 0a "))
		return S_OK;
	else
		return E_HRESULT_EXAM_ERROR_ROLLING;	
}




/******************************************************
函数名：PreRolling
参数：index    学生机在教师机中的序号
返回值
说明：对学生机提前交卷的，进行单独收卷；
******************************************************/
HRESULT CYDTeacherAppCom::PreRolling(int index)
{
	HRESULT hr = E_FAIL;
	m_com.Read();

	CString strCMD = _T("a9 00 00 01 ");
	CString  strIndexHexlength = CDataHandler::FormatNO(CDataHandler::DectoHexstr(index));
	strCMD += strIndexHexlength + _T("0d 0d 0a");

	CString strRecv;
	hr = contrl(strCMD, 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T("b9 00 00 00 0d 0d 0a "))
		return S_OK;
	else
		return E_HRESULT_EXAM_ERROR_PREROLLING;
}
HRESULT CYDTeacherAppCom::SingleRolling(int index)
{
	HRESULT hr = E_FAIL;
	m_com.Read();

	CString strCMD = _T("a2 00 00 01 ");
	CString  strIndexHexlength = CDataHandler::FormatNO(CDataHandler::DectoHexstr(index));
	strCMD += strIndexHexlength + _T("0d 0d 0a");

	CString strRecv ;
	hr = contrl(strCMD, 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T("b2 00 00 00 0d 0d 0a "))
		return S_OK;
	else
		return E_HRESULT_EXAM_ERROR_SINGLEROLLING;
}



HRESULT CYDTeacherAppCom::OpenFM()
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	CString strRecv;
	hr = contrl(_T("aa 00 00 07 01 00 00 00 00 00 00 0d 0d 0a "), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T("ba 00 00 00 0d 0d 0a "))
		return S_OK;
	else
		return E_HRESULT_EXAM_ERROR_OPENFM;
}
HRESULT CYDTeacherAppCom::SearchFM()
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	CString strRecv;
	hr = contrl(_T("aa 00 00 07 00 00 01 00 00 00 00 0d 0d 0a "), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T("ba 00 00 00 0d 0d 0a "))
		return S_OK;
	else
		return E_HRESULT_EXAM_ERROR_SEARCHFM;
}
HRESULT CYDTeacherAppCom::CloseFM()
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	CString strRecv;
	hr = contrl(_T("aa 00 00 07 00 01 00 00 00 00 00 0d 0d 0a "), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T("ba 00 00 00 0d 0d 0a "))
		return S_OK;
	else
		return E_HRESULT_EXAM_ERROR_CLOSEFM;
}
HRESULT CYDTeacherAppCom::UpFM()
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	CString strRecv ;
	hr = contrl(_T("aa 00 00 07 00 00 00 01 00 00 00 0d 0d 0a "), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T("ba 00 00 00 0d 0d 0a "))
		return S_OK;
	else
		return E_HRESULT_EXAM_ERROR_UPFM;
}
HRESULT CYDTeacherAppCom::DownFM()
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	CString strRecv;
	hr = contrl(_T("aa 00 00 07 00 00 00 00 01 00 00 0d 0d 0a "), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T("ba 00 00 00 0d 0d 0a "))
		return S_OK;
	else
		return E_HRESULT_EXAM_ERROR_DOWNFM;
}
HRESULT CYDTeacherAppCom::SetFM(CString strFM)
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	int iFM = _ttoi(strFM);
	
	CString strCMD = _T("aa 00 00 07 00 00 00 00 00 ");
	strCMD = strCMD + CDataHandler::FormatLength(CDataHandler::DectoHexstr(iFM)) + _T("0d 0d 0a");
	CString strRecv;
	hr = contrl(strCMD, 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T("ba 00 00 00 0d 0d 0a "))
		return S_OK;
	else
		return E_HRESULT_EXAM_ERROR_SETFM;
}
HRESULT CYDTeacherAppCom::GetFM(CString &_strFM)
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	hr = contrl(_T("ac 00 00 00 0d 0d 0a "), 5,_strFM);
	if(FAILED(hr))
	{
		return hr;
	}
	if(_strFM == _T(""))
	{
		return S_OK;
	}
	_strFM =  _strFM.Mid(12,5);
	return S_OK;
}



HRESULT CYDTeacherAppCom::GetSingleanswer(int index, int iStartNO, int iEndNO, CString &strAnswerout)
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	CString strRecv;
	hr = contrl(_T("8d 00 00 00 0d 0d 0a"), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T(""))
	{
		strAnswerout = _T("没有接收到答案");
		return S_FALSE;
	}
	else
	{
		int iCmdlength = strRecv.GetLength();
		CString strTotalanswer = strRecv.Mid(18, iCmdlength-18-9);
		int iPeranswer = 303;
	
		CString strAnswer = strTotalanswer.Mid(index*iPeranswer+3,iPeranswer-3);
		BYTE out[1000];
		hr = Decode(strAnswer,out,0,199);
		if(FAILED(hr))
		{
			return hr;
		}
		CString answerstr = _T("");
		for ( int j = iStartNO-1 ; j <= iEndNO-1 ;j++)
		{
			CString tmp;
			if (( out[j] == '0' ) || (out[j]==0) || ( out[j]==255 ))
				tmp = _T("0");
			else
				tmp.Format(_T("%d"),out[j]);
			answerstr = answerstr+tmp;
		}	
		CString strsutanswer = _T(""); 
		for(int strlength = 0; strlength < answerstr.GetLength(); strlength++)
		{
			CString test = answerstr.Mid(strlength,1);
			if(answerstr.Mid(strlength,1) == _T("1"))
				strsutanswer += _T("A");
			else if(answerstr.Mid(strlength,1) == _T("2"))
				strsutanswer += _T("B");
			else if(answerstr.Mid(strlength,1) == _T("3"))
				strsutanswer += _T("C");
			else if(answerstr.Mid(strlength,1) == _T("4"))
				strsutanswer += _T("D");
			else
				strsutanswer += _T("0");
		}
		strAnswerout = strsutanswer;
	}
	return S_OK;
}

HRESULT CYDTeacherAppCom::Getanswer(std::list<CExamQuestionSegment*> &_lstQSeg,
									CString &strAnswerout)
{
	HRESULT hr = E_FAIL;
	m_com.Read();

	CString strRecv ;
	hr = contrl(_T("8d 00 00 00 0d 0d 0a"), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
//	CString strRecv = m_bk.Load("学生答案",m_path);
//	strRecv += " ";
//	CString strRecv ="9d 00 00 c3 01 0a 00 45 19 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 59 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 28 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 43 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 53 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 32 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 34 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 57 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 2c 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 4f 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 3a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 2a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 01 0a 00 45 55 0a 0a 0a 0a 0a 0a 0a 0a 0a 0a 0d 0d 0a ";
	strAnswerout = strRecv;
//	AfxMessageBox(strAnswerout);
	if(strRecv == _T(""))
	{
		//AfxMessageBox(_T("没有接收到答案"));
		return E_HRESULT_EXAM_ERROR_NOANSWER;
	}
	int iCmdlength = strRecv.GetLength();
	CString strTotalanswer = strRecv.Mid(18, iCmdlength-18-9);
	int iPeranswer = strTotalanswer.GetLength()/(m_lstStuDev.size());
	int iStartNO = 100;
	int iEndNO = 0;
	for(std::list<CExamQuestionSegment*>::const_iterator itr = _lstQSeg.begin();
		itr != _lstQSeg.end();++itr)
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
	for(UINT i = 0; i < m_lstStuDev.size(); i++)
	{
		CString strAnswer = strTotalanswer.Mid(i*iPeranswer+15,iPeranswer-15);
		BYTE out[1000];
		Decode(strAnswer,out,0,199);
		CString answerstr = _T("");
		hr = TransferAnswer(out,answerstr);
		if(FAILED(hr))
		{
			return hr;
		}
#ifdef DEBUG
//		AfxMessageBox(answerstr);
#endif

		CString strMac = strTotalanswer.Mid(i*iPeranswer+6, 8);
		strMac.MakeUpper();
		for(std::list<CStudev*>::const_iterator itr = m_lstStuDev.begin();
			itr != m_lstStuDev.end();++itr)
		{
			CString strItrMac;
			hr = (*itr)->GetMac(strItrMac);
			if(FAILED(hr))
			{
				return hr;
			}
			if(strMac.CompareNoCase(strItrMac) == 0)
			{
				hr = (*itr)->SetAnswer(answerstr);
				if(FAILED(hr))
				{
					return hr;
				}
			}
		}
	}
	return S_OK;
}

HRESULT CYDTeacherAppCom::Getoffanswer(CStringArray &arryList,int iStartNO, int iEndNO)
{
	HRESULT hr = E_FAIL;
	arryList.RemoveAll();
	m_com.Read();
	CString strRecv;
	hr =  contrl(_T("8d 00 00 00 0d 0d 0a"), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T(""))
	{
		return E_HRESULT_EXAM_ERROR_GETOFFANSWER;
	}
	int iCmdlength = strRecv.GetLength();
	CString strTotalanswer = strRecv.Mid(18, iCmdlength-18-9);
	int count = strTotalanswer.GetLength()/303;
	for(int i = 0; i < count; i++)
	{
		CString strAnswer = strTotalanswer.Mid(i*303+3,300);
		BYTE out[1000];
		Decode(strAnswer,out,0,199);
		CString answerstr = _T("");
		for ( int j = iStartNO-1 ; j <= iEndNO-1 ;j++)
		{
			CString tmp;
			if (( out[j] == '0' ) || (out[j]==0) || ( out[j]==255 ))
				tmp = "0";
			else
				tmp.Format(_T("%d"),out[j]);
			answerstr = answerstr+tmp;
		}
		CString strsutanswer = _T(""); 
		for(int strlength = 0; strlength < answerstr.GetLength(); strlength++)
		{
			CString test = answerstr.Mid(strlength,1);
			if(answerstr.Mid(strlength,1) == _T("1"))
				strsutanswer += _T("A");
			else if(answerstr.Mid(strlength,1) == _T("2"))
				strsutanswer += _T("B");
			else if(answerstr.Mid(strlength,1) == _T("3"))
				strsutanswer += _T("C");
			else if(answerstr.Mid(strlength,1) == _T("4"))
				strsutanswer += _T("D");
			else
				strsutanswer += _T("0");
		}
		arryList.Add(strsutanswer);
	}
	return S_OK;
}

HRESULT	CYDTeacherAppCom::DecodeAnswer(CString _strEncode,int iStartNO, int iEndNO,
									   CString &_strAnswer)
{
	HRESULT hr = E_FAIL;
	BYTE out[1000];
	Decode(_strEncode,out,0,199);
	CString strTempAnswer = _T("");
	hr = TransferAnswer(out,strTempAnswer);
	if(FAILED(hr))
	{
		return hr;
	}
	_strAnswer = strTempAnswer;
//#ifdef DEBUG
//	AfxMessageBox(_strAnswer);
//#endif
	
	return S_OK;
}

HRESULT CYDTeacherAppCom::GetAnswerByStudev(CStudev* pStudev,
											int iStartNO, int iEndNO,
											CString& _strAnswer)
{
	HRESULT hr = E_FAIL;
	CString strCmd;
	strCmd = _T("8b 00 00 03 ");
	CString strMac;
	hr = pStudev->GetMac(strMac);
	if(FAILED(hr))
	{
		return hr;
	}
	strCmd += strMac;
	strCmd += _T(" 0d 0d 0a");
	CString strRev;
	hr = contrl(strCmd,8,strRev);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRev == _T(""))
	{
		return E_HRESULT_EXAM_ERROR_NOANSWER;
	}
	CString strTemp = strRev.Mid(0,2);
	if (strTemp != _T("9b"))
	{
		return E_HRESULT_EXAM_ERROR_NOANSWER;
	}
	int iLength = strRev.GetLength();
	CString strStat = strRev.Mid(18, iLength-18);
	int iFirst = strStat.Find(_T("0d 0d 0a "));
	if(iFirst == -1)
	{
		return E_HRESULT_EXAM_ERROR_UPDATESTAT;
	}
	strStat = strStat.Left(iFirst);
	std::list<CStuMacState*> lstStuMacState;
	hr = ParseStuStr(strStat,lstStuMacState);
	if(FAILED(hr))
	{
		return hr;
	}
	CListAutoClean<CStuMacState> clr(lstStuMacState);

	int index = 0;
	for(std::list<CStuMacState*>::const_iterator itrMacState = lstStuMacState.begin();
		itrMacState != lstStuMacState.end();++itrMacState)
	{
		for(std::list<CStudev*>::const_iterator itr = m_lstStuDev.begin();
			itr != m_lstStuDev.end();++itr,index++)
		{
			CString strItrMac;
			hr = (*itr)->GetMac(strItrMac);
			if(FAILED(hr))
			{
				return hr;
			}
			if(strItrMac.CompareNoCase((*itrMacState)->m_strMac) == 0)
			{
				hr = (*itr)->SetRollCall((*itrMacState)->m_iBeginExam);
				if(FAILED(hr))
				{
					return hr;
				}
				hr = (*itr)->SetRolling((*itrMacState)->m_iRolling);
				if(FAILED(hr))
				{
					return hr;
				}
				break;
			}
		}
	}

	iFirst = strRev.Find(_T("0d 0d 0a "));
	CString strDecodeAnswer = strRev.Right(strRev.GetLength() - iFirst - 9);
	int iLast = strDecodeAnswer.Find(_T("0d 0d 0a"));
	if(iLast == -1 )
	{
		return E_HRESULT_EXAM_ERROR_NOANSWER;
	}
	strDecodeAnswer = strDecodeAnswer.Left(iLast);
	if (strDecodeAnswer.IsEmpty())
	{
		return S_FALSE;
	}
//#ifdef DEBUG
//	AfxMessageBox(strDecodeAnswer);
//#endif
	hr = DecodeAnswer(strDecodeAnswer,iStartNO,iEndNO,_strAnswer);
	if(FAILED(hr))
	{
		return hr;
	}
//#ifdef DEBUG
//	AfxMessageBox(_strAnswer);
//#endif
	hr = pStudev->SetAnswer(_strAnswer);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYDTeacherAppCom::FetchAnswer(CString &_str)
{
	HRESULT hr = E_FAIL;
	CString strRecv;
	hr = contrl(_T("8d 00 00 00 0d 0d 0a"), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T(""))
	{
		return E_HRESULT_EXAM_ERROR_FETCHANSWER;
	}
	{
		CListAutoClean<CStudev> clr(m_lstStuDev);
	}
	_str = strRecv;
	int iLen = strRecv.GetLength();
	CString sTotalAnswer = strRecv.Mid(18, iLen-18-9);
	int tt = sTotalAnswer.GetLength();
	int iStuCount = sTotalAnswer.GetLength()/(105*3);
	int itemp = sTotalAnswer.GetLength()%(105*3);
	if (itemp != 0)
	{
		return FALSE;
	}
	for(int i = 0; i < iStuCount; i++)
	{
		CString sStuIndex = sTotalAnswer.Mid(0+i*105*3, 5);
		sStuIndex.Replace(_T(" "), _T(""));
		//int  iStuIndex = (int)strtol(sStuIndex,NULL,16);
		CString sStuMac = sTotalAnswer.Mid(6+i*105*3, 8);
		sStuMac.TrimRight();
		sStuMac.TrimLeft();
		sStuMac.MakeUpper();
		CString sStuAnswer = sTotalAnswer.Mid(15+i*105*3,300);
		CString strAnswer;
		DecodeAnswer(sStuAnswer,1,100,strAnswer);
		CStudev* pStudev = new CStudev();
		hr = pStudev->SetMac(sStuMac);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pStudev->SetAnswer(strAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
		m_lstStuDev.push_back(pStudev);
	}
	return S_OK;
}


HRESULT CYDTeacherAppCom::Getrelationmap(CStringArray &arryrelation)
{
	HRESULT hr = E_FAIL;
	arryrelation.RemoveAll();
	m_com.Read();
	CString strRecv;
	hr = contrl(_T("a3 00 00 00 0d 0d 0a"), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRecv == _T(""))
	{
		return E_HRESULT_EXAM_ERROR_GETRELATIONMAP;
	}
	int iCmdlength = strRecv.GetLength();
	CString total = strRecv.Mid(12, iCmdlength-12-9);
	int count = total.GetLength()/12;
	for(int i = 0; i < count; i++)
	{
		CString mac = total.Mid(3+i*12, 8);
		arryrelation.Add(mac);
	}
	return S_OK;
}
/***********************************************
类名：contrl
参数：CString cmd发送给教师机的指令
返回值：CString 教师机返回的指令
说明：发送完指令，如果教师在3秒内无回复，则返回空；
***********************************************/
//CString CYDTeacherAppCom::GetExePath()
//{
//	CString file;
//	char process[MAX_PATH];
//	GetModuleFileName(NULL, process, MAX_PATH);
//    for ( int i = strlen(process);i>0 ;i --)
//	if ( process[i] =='\\')
//	{
//		process[i+1]='\0';
//		break;
//	}
//	file.Format(process);
//	CString temp = file + _T("syslog.txt");
//	return temp;
//}


HRESULT CYDTeacherAppCom::FindTeacherCom()
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExampStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExampStruct);
	ASSERT(pGExampStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	CCriticalSectionControl cc(&(pGExampStruct->m_cs));
	{
		CListAutoClean<CYDTeacherAppCom> clr(pGExampStruct->m_lstTacherAppCom);
	}
	CSearchDevCom SearchDevCom;
	std::list<CString> lstCom;
	hr = SearchDevCom.SearchDev(lstCom);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CString>::const_iterator itr = lstCom.begin();
		itr != lstCom.end();++itr)
	{
		CYDTeacherAppCom* pAppCom = new CYDTeacherAppCom(*itr);
		hr = pAppCom->OpenCom();
		if(FAILED(hr))
		{
			CPtrAutoClean<CYDTeacherAppCom> clr(pAppCom);
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
		pGExampStruct->m_lstTacherAppCom.push_back(pAppCom);
	}
// 	for(int i = 1; i < 20; i++)
// 	{
// 		CString strTemp;
// 		//strTemp.Format(_T("com%d"),i);
// 		if (i >= 10)
// 			strTemp.Format(_T("\\\\.\\com%d"),i);
// 		else
// 			strTemp.Format(_T("com%d"),i);
// 
// 		CYDTeacherAppCom* pAppCom = new CYDTeacherAppCom(strTemp);
// 		hr = pAppCom->OpenCom();
// 		if(FAILED(hr))
// 		{
// 			CPtrAutoClean<CYDTeacherAppCom> clr(pAppCom);
// 			if(hr == E_HRESULT_EXAM_ERROR_OPENCOM)
// 			{
// 				continue;
// 			}
// 			else
// 			{
// 				return hr;
// 			}
// 		}
// 		hr = pAppCom->Close();
// 		if(FAILED(hr))
// 		{
// 			return hr;
// 		}
// 		_lstTeacherAppCom.push_back(pAppCom);
// 	}
	return S_OK;
}

HRESULT CYDTeacherAppCom::InrarSendExamInfo()
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	//只有点名方式选择红外才能执行当前函数
	ASSERT(pGExamStruct->m_pExamDetail->m_uRollCallType == ROLLCALL_TYPE_INFRAR);
	std::list<CString> lstStrMac;
	//lstStrMac为空，相当于清空清除上场考试的对应关系和答案
	hr = Sendvalidmac(lstStrMac);
	if(FAILED(hr))
	{
		return hr;
	}
	Sleep(2000);
	hr = Sendexaminfo();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYDTeacherAppCom::BeginExamSingle(CString _strMac)
{
	HRESULT hr = E_FAIL;
	//要先判断_pStudev是否在当前的TeacherAppCom中
	BOOL bFind = FALSE;
	for(std::list<CStudev*>::const_iterator itr = m_lstStuDev.begin();
		itr != m_lstStuDev.end();++itr)
	{
		CString strItrMac;
		hr = (*itr)->GetMac(strItrMac);
		if(FAILED(hr))
		{
			return hr;
		}
		if(strItrMac.CompareNoCase(_strMac) == 0)
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		//当前的_pStudev不在当前已点名的学生机列表中，不能单独续考
		return E_HRESULT_EXAM_NOT_IN_STU;
	}
	hr = Reset();
	if(FAILED(hr))
	{
		return hr;
	}
	Sleep(500);
	//发送单独建立连接的指令
	CString strCmd = _T("89 00 00 04 ");
	strCmd += _strMac;
	strCmd += _T(" 0d 0d 0a");
	CString strRev;
	hr = contrl(strCmd,20,strRev);
	if(FAILED(hr))
	{
		return hr;
	}
	if(strRev.CompareNoCase(_T("99 00 00 00 0d 0d 0a ")) == 0)
	{
		//建立连接成功v 99 00 00 00 0d 0d 0a
		
	}
	else if(strRev.CompareNoCase(_T("C9 00 00 00 0d 0d 0a ")) == 0)
	{
		//建立连接失败
		return E_HRESULT_EXAM_ERROR_SINGLE_BEGINEXAM;
	}
	else
	{
		ASSERT(FALSE);
		return E_HRESULT_EXAM_ERROR_SINGLE_BEGINEXAM;
	}
	//单独发送时间信息包
	strCmd = _T("8a 00 00 21 ");
	CString strExamInfo;
	hr = CreateExamInfo(strExamInfo);
	if(FAILED(hr))
	{
		return hr;
	}
	strCmd += strExamInfo;
	strCmd += _T("0d 0d 0a");
	strRev = _T("");
	for(int i =0;i < 2;i++)
	{
		//发送两次考试信息，如果第一次出错的话，就再发一次
		hr = contrl(strCmd,8,strRev);
		if(strRev.CompareNoCase(_T("9a 00 00 00 0d 0d 0a ")) == 0)
		{
			break;//发送考试信息成功
		}
		if(i == 1)
		{
			return E_HRESULT_EXAM_ERROR_SENDEXAMINFO; 
		}
		Sleep(500);
	}
	BOOL bRoll = FALSE,bRolling = FALSE;
	hr = Updatestat(bRoll,bRolling);
	if(FAILED(hr))
	{
		return hr;
	}
	
	return S_OK;
}





