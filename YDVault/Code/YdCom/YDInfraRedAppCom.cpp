#include "StdAfx.h"
#include "YDInfraRedAppCom.h"
#include "../Base\DataHandler.h"
#include "../UIBase\ListBoxEx.h"
#include "StaticYdComApp.h"
#include "../\Base\CriticalSectionControl.h"

const int CON_INFRARED_DATA_SIZE = 110; //红外每次接受的有效的一段数据块大小

CYDInfraRedAppCom::CYDInfraRedAppCom(CString _strCom)
:CYDAppCom(_strCom)
{
	m_strReadData = _T("");
}

CYDInfraRedAppCom::~CYDInfraRedAppCom(void)
{
}
HRESULT CYDInfraRedAppCom::ReadData(CString &_str)
{
	HRESULT hr= E_FAIL;
	_str = m_com.ReadInfraRed();
	return S_OK;
}

HRESULT CYDInfraRedAppCom::DecodData(std::list<std::pair<CString,CString> > &_lstMacAnswer)
{
	HRESULT hr = E_FAIL;
	if(m_strReadData.IsEmpty())
	{
		return S_FALSE;
	}
	int iData = m_strReadData.GetLength();
	while ( iData >= CON_INFRARED_DATA_SIZE * 3 )
	{
		CString strMid05 = m_strReadData.Mid(0,5);
		if(strMid05.CompareNoCase(_T("b5 00")) == 0)
		{
			//AfxMessageBox(m_recv);
			CString strAnswer = _T("");
			CString strMac = _T("");
			CString strMid01083;
			strMid01083 = m_strReadData.Mid(0, CON_INFRARED_DATA_SIZE*3);
			hr = DecodeAnswer(strMid01083,strAnswer,strMac);
			if(FAILED(hr))
			{
				return hr;
			}
			if(strMac.GetLength() > 0)
			{
				_lstMacAnswer.push_back(std::make_pair(strMac, strAnswer));
			}		
			{
				CCriticalSectionControl cs(&m_cs);
				m_strReadData = m_strReadData.Mid(CON_INFRARED_DATA_SIZE*3, m_strReadData.GetLength());
			}
			
		}
		else
		{
			CCriticalSectionControl cs(&m_cs);
			m_strReadData = m_strReadData.Mid(3, iData - 3);
		}
		iData = m_strReadData.GetLength();
	}
	return S_OK;
}

HRESULT CYDInfraRedAppCom::DecodeAnswer(CString _str,CString &_strAnswer,CString &_strMac)
{
	HRESULT hr = E_FAIL;
	VARIANT_BOOL bValidtate = VARIANT_FALSE;
	hr = ValidateData(_str,&bValidtate);
	if(FAILED(hr))
	{
		return hr;
	}
	if(!bValidtate)
	{
		return S_FALSE;
	}
	CString strEncond = _T("");
	_strMac = _str.Mid(6, 8);
	strEncond = _str.Mid(15, 100*3);
	BYTE out[1000];
	Decode(strEncond,out,0,199);
	CString strTempAnswer = _T("");
	hr = TransferAnswer(out,strTempAnswer);
	if(FAILED(hr))
	{
		return FALSE;
	}
	_strAnswer = strTempAnswer;
	return S_OK;
}

HRESULT CYDInfraRedAppCom::ValidateData(CString _strData,VARIANT_BOOL* _bValid)
{
	HRESULT hr = E_FAIL;
	*_bValid = VARIANT_FALSE;
	_strData = _strData.Mid(0, CON_INFRARED_DATA_SIZE*3);
	CString strRight6 = _strData.Right(6);
	if (strRight6.CompareNoCase(_T("b6 00 ")) != 0)
	{
		return S_OK;
	}
	CString strCheckbit = _strData.Mid(105*3,2);
	CString strCheckdata = _strData.Mid(0, 105*3);
	CString strOP = _T("00");

	for(int i = 0; i < (strCheckdata.GetLength()/3); i++)
	{
		CString str1 = strCheckdata.Mid(i*3, 2);
		int i1 = Str2Dec(str1);
		int i2 = Str2Dec(strOP);

		int i3 = i1 ^ i2;
		strOP.Format(_T("%x"), i3);
		strOP = FormatNO(strOP);
	}
	strCheckbit.TrimLeft();
	strCheckbit.TrimRight();
	strOP.TrimRight();
	strOP.TrimLeft();
	if (strCheckbit.CompareNoCase(strOP) != 0)
	{
		return S_OK;
	}
	*_bValid = VARIANT_TRUE;
	return S_OK;
}

/************************************************
说明：字符串转10进制数
************************************************/
int CYDInfraRedAppCom::Str2Dec(CString str)
{
	BYTE in[1];
	memset(in,0,sizeof(in));
	CDataHandler::String2Hex(str,(char *)in);	
	CString strOut = _T("");
	strOut.Format(_T("%d"),in[0]);

	int i = _ttoi(strOut);
	return i;
}
CString CYDInfraRedAppCom::FormatNO(CString strNO)
{
	int iLength = strNO.GetLength();
	if(iLength == 2)
	{
		strNO = strNO+_T(" ");
	}
	else if(iLength == 1)
	{
		//if(strNO == "0")
		//	strNO = "ff ";
		//	else
		strNO = _T("0") + strNO+ _T(" ");
	}
	else 
		strNO = _T("ff ");

	return strNO;
}
HRESULT CYDInfraRedAppCom::ReadData()
{
	HRESULT hr = E_FAIL;
	CString strReadData;
	hr = ReadData(strReadData);
	if(FAILED(hr))
	{
		return hr;
	}
	COleDateTime tm;
	tm = COleDateTime::GetCurrentTime();
	CString temp = tm.Format()+_T(" ReadData: ") + _T(" ") + strReadData ;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	ASSERT(pGExamStruct->m_pMainWnd);
	CListBoxEx* pMsgListBox = (CListBoxEx*)pGExamStruct->m_pMainWnd->SendMessage(WM_YD_GET_EXAM_MSG_LISTBOX);
	ASSERT(pMsgListBox);
	pMsgListBox->AddString(temp);
	{
		CCriticalSectionControl cs(&m_cs);
		m_strReadData += strReadData;
	}
	
	return S_OK;
}


