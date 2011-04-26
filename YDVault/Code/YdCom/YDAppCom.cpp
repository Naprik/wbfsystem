#include "StdAfx.h"
#include "YDAppCom.h"
#include "../UIBase\ListBoxEx.h"
#include "../Include\YDVaultError.h"
#include "../\Base\DataHandler.h"
#include "StaticYdComApp.h"
#include "ExamQuestionSegment.h"

CYDAppCom::CYDAppCom(CString _strComPort)
{
	m_strComPort = _strComPort;
	m_strMac = _T("");
}

CYDAppCom::~CYDAppCom(void)
{
	Close();
}

HRESULT CYDAppCom::contrl(CString cmd, int iTime,CString &_str)
{	
	//	CMainFrame *fram = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	//	CJahFile    m_file;
	HRESULT hr = E_FAIL;
	COleDateTime tm;
	tm = COleDateTime::GetCurrentTime();
	CString temp = tm.Format()+_T(" SEND ") + _T(" ") + cmd ;
	CListBoxEx* pMsgListBox = (CListBoxEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_EXAM_MSG_LISTBOX);
	ASSERT(pMsgListBox);
	pMsgListBox->AddString(temp);
	_str = _T("");
	if(!m_com.Send(cmd))
		return S_OK;
	CString recv;
	COleDateTime t1, t2;
	t1 = COleDateTime::GetCurrentTime();
	COleDateTimeSpan timeSpan; 
	int  span=0;
	do
	{
		recv = m_com.Read();
		t2 = COleDateTime::GetCurrentTime();
		timeSpan = t2 - t1;
		span = (int)timeSpan.GetTotalSeconds();
	}while((recv == _T(""))&&(span <= iTime));
	tm = COleDateTime::GetCurrentTime();
	temp = tm.Format()+_T(" RECV ") + _T(" ") + recv ;
	//	m_file.WriteString(temp);
	pMsgListBox->AddString(temp);
	//	m_file.Close();
	if(!recv.IsEmpty() && 
		recv.CompareNoCase(cmd) != 0)
	{
		_str = recv;
	}
	return S_OK;
}

HRESULT CYDAppCom::OpenCom(DWORD _dwBaudRate /*= cBlueRedRate*/)
{
	HRESULT hr = E_FAIL;
	if (m_strComPort.GetLength() > 4) //如果端口号大于"com10"，需要在端口名称前面增加“\\\\.\\”
	{
		if(m_strComPort.Find(_T("\\\\.\\")) == -1)
		{
			m_strComPort.Format(_T("\\\\.\\%s"), m_strComPort);
		}
	}
	if(!m_com.OpenComm(m_strComPort,_dwBaudRate))
	{
		return E_HRESULT_EXAM_ERROR_OPENCOM;
	}
	return S_OK;
}


HRESULT CYDAppCom::Close()
{
	HRESULT hr = E_FAIL;
	hr = m_com.Close();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}


HRESULT CYDAppCom::Decode(CString input,BYTE *out,int min,int max)
{
	HRESULT hr = E_FAIL;
	typedef void (_cdecl *DecodeAnswer)(BYTE *in,int t1,int t2,BYTE *out);
	HINSTANCE hinstDLL=NULL; 
	hinstDLL=LoadLibrary(_T("0707.dll"));
	if (hinstDLL)
	{
		DecodeAnswer PDecodeAnswer;
		PDecodeAnswer = (DecodeAnswer)GetProcAddress (hinstDLL,"DecodeAnswer");
		BYTE in[1000];
		memset(in,0,sizeof(in));
		CDataHandler::String2Hex(input,(char *)in);
		memset(out,0,sizeof(out));
		PDecodeAnswer(in,min,max,out);
		FreeLibrary(hinstDLL);
	}
	return S_OK;
}

HRESULT  CYDAppCom::TransferAnswer(BYTE* out,CString &_strAnswer)
{
	HRESULT hr = E_FAIL;
	_strAnswer = _T("");
	for ( int j = 0 ; j <= 198 ;j++)
	{
		CString tmp;
		if (( out[j] == '0' ) || (out[j]==0) || ( out[j]==255 ))
			tmp = _T("0");
		else
			tmp.Format(_T("%x"),out[j]);

		if (tmp == _T("1"))
			tmp = _T("A");
		else if (tmp == _T("2"))
			tmp = _T("B");
		else if (tmp == _T("3"))
			tmp = _T("AB");
		else if (tmp == _T("4"))
			tmp = _T("C");
		else if (tmp == _T("5"))
			tmp = _T("AC");
		else if (tmp == _T("6"))
			tmp = _T("BC");
		else if (tmp == _T("7"))
			tmp = _T("ABC");
		else if (tmp == _T("8"))
			tmp = _T("D");
		else if (tmp == _T("9"))
			tmp = _T("AD");
		else if (tmp == _T("a"))
			tmp = _T("BD");
		else if (tmp == _T("b"))
			tmp = _T("ABD");
		else if (tmp == _T("c"))
			tmp = _T("CD");
		else if (tmp == _T("d"))
			tmp = _T("ACD");
		else if (tmp == _T("f"))
			tmp = _T("ABCD");
		else if (tmp == _T("0"))
			tmp = _T("");
		else
			tmp = _T("");

		if (tmp != _T(""))
		{
			CString temp = _T("");
			temp.Format(_T("%d%s"), j+1, tmp);
			_strAnswer = _strAnswer+temp;
		}
	}	
	return S_OK;
}

HRESULT CYDAppCom::GetState(BOOL &_bOpen)
{
	return m_com.GetState(_bOpen);
}

HRESULT CYDAppCom::GetDevMac(CString& _strMac,int& _iType)
{
	HRESULT hr = E_FAIL;
	_iType = -1;
	//m_com.Read();
	CString strRecv;
	hr = contrl(_T("a8 00 00 00 0d 0d 0a"), 5,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	if (!strRecv.IsEmpty())
	{
		if(strRecv.GetLength() == 42)
		{
			_strMac = strRecv.Mid(24,8);
			CString strType = strRecv.Mid(12,2);
			if(strType.CompareNoCase(_T("00")) == 0)
			{
				_iType = 0;//是教师机
			}
			else if(strType.CompareNoCase(_T("01")) == 0)
			{
				_iType = 1;//是学生机
			}
			else
			{
				ASSERT(FALSE);
			}
			return S_OK;
		}
	}
	return E_HRESULT_EXAM_ERROR_GETTEACHERDEV;

}

HRESULT CYDAppCom::Sendexaminfo()
{
	HRESULT hr = E_FAIL;
	m_com.Read();
	CString strCmd = _T("85 00 00 21 ");
	CString strExamInfo;
	hr = CreateExamInfo(strExamInfo);
	if(FAILED(hr))
	{
		return hr;
	}
	strCmd += strExamInfo;
	strCmd += _T("0d 0d 0a");
	CString  strRecv ;
	hr = contrl(strCmd, 8,strRecv);
	if(FAILED(hr))
	{
		return hr;
	}
	//	CString  strRecv = "95 00 00 00 0d 0d 0a ";
	if(strRecv != _T("95 00 00 00 0d 0d 0a "))
	{
		return E_HRESULT_EXAM_ERROR_SENDEXAMINFO_NEW;
	}
	
	return S_OK;
}

HRESULT CYDAppCom::CreateExamInfo(CString& _strExamInfo)
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	int iGrade = 0;
	if(pGExamStruct->m_pExamDetail->m_uExamKind == YDEXAM_KIND_NORMAL)
	{
		iGrade = 0;
	}
	else
	{
		iGrade = 1;
	}
	int iRolling = 0;
	if(pGExamStruct->m_pExamDetail->m_uRollingType == ROLLING_TYPE_BLUE)
		iRolling = 0;
	else
		iRolling = 1;

	int iFm = 0;
	if(pGExamStruct->m_pExamDetail->m_bListen)
		iFm = 1;
	else
		iFm = 0;

	std::list<std::pair<COleDateTime,COleDateTime> > lstFmTime;
	lstFmTime.push_back(std::make_pair(pGExamStruct->m_pExamDetail->m_timeStart1, pGExamStruct->m_pExamDetail->m_timeEnd1));
	lstFmTime.push_back(std::make_pair(pGExamStruct->m_pExamDetail->m_timeStart2, pGExamStruct->m_pExamDetail->m_timeEnd2));
	lstFmTime.push_back(std::make_pair(pGExamStruct->m_pExamDetail->m_timeStart3, pGExamStruct->m_pExamDetail->m_timeEnd3));


	COleDateTime currentimt = COleDateTime::GetCurrentTime();
	COleDateTimeSpan timeStartspan, timeEndspan, timeStartfm1span, timeEndfm1span, timeStartfm2span, timeEndfm2span, timeStartfm3span, timeEndfm3span;

	ASSERT(lstFmTime.size() == 3);
	timeStartspan = pGExamStruct->m_pExamDetail->m_timeStart - currentimt;
	timeEndspan = pGExamStruct->m_pExamDetail->m_timeEnd - currentimt;
#ifdef DEBUG
	CString strEnd = CDataHandler::DateTimeToStr(pGExamStruct->m_pExamDetail->m_timeEnd,0);
#endif

	int index = 0;
	for(std::list<std::pair<COleDateTime,COleDateTime> >::const_iterator itr = lstFmTime.begin();
		itr != lstFmTime.end();++itr)
	{
		if(index == 0)
		{
			timeStartfm1span = (*itr).first - currentimt;
			timeEndfm1span = (*itr).second - currentimt;
		}
		else if(index == 1)
		{
			timeStartfm2span = (*itr).first- currentimt;
			timeEndfm2span = (*itr).second - currentimt;
		}
		else if(index == 2)
		{
			timeStartfm3span = (*itr).first - currentimt;
			timeEndfm3span = (*itr).second - currentimt;
		}
		else
		{
			ASSERT(FALSE);
		}
		index ++ ;
	}

	int  iStartspan = (int)timeStartspan.GetTotalSeconds();
	if(iStartspan <= 0)
		iStartspan = 1;
	int  iEndspan = (int)timeEndspan.GetTotalSeconds();
	if(iEndspan <0)
		iEndspan = 2;
	int  iAlertspan = iEndspan - 300;                                    //考试提醒时间
	if(iAlertspan<1)
		iAlertspan = 1;

	int  iStartfm1span = (int)timeStartfm1span.GetTotalSeconds();
	if(iStartfm1span <= 0 || iFm == 0)
		iStartfm1span = 1;
	int  iEndfm1span = (int)timeEndfm1span.GetTotalSeconds();
	if(iEndfm1span < 0 || iFm == 0)
		iEndfm1span = 2;

	int  iStartfm2span = (int)timeStartfm2span.GetTotalSeconds();
	if(iStartfm2span <= 0 || iFm == 0)
		iStartfm2span = 1;
	int  iEndfm2span = (int)timeEndfm2span.GetTotalSeconds();
	if(iEndfm2span < 0 || iFm == 0)
		iEndfm2span = 2;

	int  iStartfm3span = (int)timeStartfm3span.GetTotalSeconds();
	if(iStartfm3span <= 0 || iFm == 0)
		iStartfm3span = 1;
	int  iEndfm3span = (int)timeEndfm3span.GetTotalSeconds();
	if(iEndfm3span < 0 || iFm == 0)
		iEndfm3span = 2;

	//const char *p;
	//p=strFM.GetBuffer(strFM.GetLength());
	//strFM.ReleaseBuffer();
	int iFMchanal = _ttoi(pGExamStruct->m_pExamDetail->m_strChanal);

	int iMaxNO = 0;
	for(std::list<CExamQuestionSegment*>::const_iterator itr = pGExamStruct->m_pExamDetail->m_lstQSeg.begin();
		itr != pGExamStruct->m_pExamDetail->m_lstQSeg.end();++itr)
	{
		if(iMaxNO < (*itr)->m_iEnd)
		{
			iMaxNO = (*itr)->m_iEnd;
		}
	}
	iMaxNO = 100;

	CString strHexStartspan = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iStartspan));
	CString strHexEndspan = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iEndspan));
	CString strHexAlertspan = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iAlertspan));
	CString strHexMaxno = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iMaxNO));
	CString strHexGrade = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iGrade));
	CString strHexRolling = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iRolling));
	CString strHexFm = CDataHandler::FormatNO(CDataHandler::DectoHexstr(iFm));
	CString strHexchanal = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iFMchanal));
	CString strHexStartfm1span = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iStartfm1span));
	CString strHexEndfm1span = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iEndfm1span));
	CString strHexStartfm2span = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iStartfm2span));
	CString strHexEndfm2span = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iEndfm2span));
	CString strHexStartfm3span = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iStartfm3span));
	CString strHexEndfm3span = CDataHandler::FormatLength(CDataHandler::DectoHexstr(iEndfm3span));

	_strExamInfo = strHexStartspan + strHexAlertspan + strHexEndspan + strHexMaxno +strHexGrade+strHexRolling+strHexFm+strHexchanal+strHexStartfm1span+strHexEndfm1span+strHexStartfm2span+strHexEndfm2span+strHexStartfm3span+strHexEndfm3span;
	{
		//题目段数设置
		index = 0;
		for(std::list<CExamQuestionSegment*>::const_iterator itr = pGExamStruct->m_pExamDetail->m_lstQSeg.begin();
			itr != pGExamStruct->m_pExamDetail->m_lstQSeg.end();++itr,++index)
		{
			CString strQuestionSegCmd ;
			hr = (*itr)->CreateRollcallCmd(strQuestionSegCmd);
			if(FAILED(hr))
			{
				return hr;
			}
			_strExamInfo += strQuestionSegCmd;
		}
		while(index < 3)
		{
			//题目段数不够三段，后面的补充为0
			_strExamInfo += _T("00 00 00 ");
			index++;
		}
	}
	return S_OK;
}

HRESULT CYDAppCom::ReadData(CString &_str)
{
	HRESULT hr = E_FAIL;
	_str = m_com.Read();
	return S_OK;
}


