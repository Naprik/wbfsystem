#include "StdAfx.h"
#include "YDAppCom.h"

CYDAppCom::CYDAppCom(void)
{
}

CYDAppCom::~CYDAppCom(void)
{
}

BOOL CYDAppCom::Close()
{
	return m_com.Close();
}

int CYDAppCom::TestCom(CString &strCom, CString &strMac)
{
	for(int i = 1; i < 20; i++)
	{
		CString strTemp;
		if (i >= 10)
			strTemp.Format(_T("\\\\.\\com%d"),i);
		else
			strTemp.Format(_T("com%d"),i);

		if(OpenCome(strTemp))
		{
			//AfxMessageBox(_T("找到串口！"));
			CString strStuMac = _T("");
			BOOL flag = GetStuMac(strStuMac);
			if (flag)
			{
				strMac = strStuMac;
				strCom = strTemp;
			}
			Close();
		}

	}
	return 0;
}



BOOL CYDAppCom::OpenCome(CString strCom)
{
	int len = strCom.GetLength();
	if (len > 4)
	{
		strCom.Format(_T("\\\\.\\%s"), strCom);
	}
	return  m_com.OpenComm(strCom);	
}


BOOL CYDAppCom::GetStuMac(CString &strMac)
{
	BOOL ret = FALSE;

	CString strRecv = contrl(_T("a8 00 00 00 0d 0d 0a"), 5);
	if (strRecv != _T(""))
	{
		CString strFlag = strRecv.Mid(12, 2);
		if (strFlag == _T("01"))
			strRecv = strRecv.Mid(24,8);
		else
			strRecv = "";

		if(strRecv != _T(""))
			ret = TRUE;
		else
			ret = FALSE;

	}
	else
		ret = FALSE;

	strMac = strRecv;
	return ret;
}


BOOL CYDAppCom::SendStuNo(int iNo)
{
	BOOL ret = FALSE;

	CString strLen = FormatLength(DectoHexstr(iNo));
	if (strLen == _T(""))
		return ret;
	CString strCmd = _T("");
	strCmd.Format(_T("87 00 00 02 %s0d 0d 0a"),strLen);
	CString strRecv = contrl(strCmd, 5);
	if (strRecv == _T("97 00 00 00 0d 0d 0a "))
		ret = TRUE;
	else
		ret = FALSE;
	return ret;
}

CString	CYDAppCom::contrl(CString cmd, int iTime)
{
	if(!m_com.Send(cmd))
		return _T("");

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
	}while((recv == "")&&(span <= iTime));
	return recv;
}

CString CYDAppCom::ReadData()
{
	CString strData = _T("");
	strData = m_com.Read();
	return strData;
}

CString CYDAppCom::DectoHexstr(int iDec)
{
	if(iDec >= 65535)
		iDec = 0;
	CString str;
	str.Format(_T("%x"),iDec);
	return str;
}

CString CYDAppCom::FormatLength(CString strHexMaclength)
{
	CString ret = _T("");
	if(strHexMaclength == _T("0"))
		return _T("00 00 ");
	int i = strHexMaclength.GetLength();
	switch(i)
	{
	case 0:
		ret = _T("00 00 ");
		return ret;
		break;
	case 1:
		ret = _T("00 0") + strHexMaclength + _T(" ");
		return ret;
		break;
	case 2:
		ret = _T("00 ") + strHexMaclength + _T(" ");
		return ret;
	case 3:
		strHexMaclength.Insert(0, _T("0"));
		strHexMaclength.Insert(2, _T(" "));
		strHexMaclength.Insert(5, _T(" "));
		ret = strHexMaclength;
		return ret;
		break;
	case 4:
		strHexMaclength.Insert(2, _T(" "));
		strHexMaclength.Insert(5, _T(" "));
		ret = strHexMaclength;
		return ret;
		break;
	default:
		return ret;
		break;
	}
}

BOOL CYDAppCom::DecodeAnswer(CString strRecv, CString &strAnswer, CString &strMac)
{
	if (proofdata(strRecv))
	{	
		CString strEncond = _T("");
		//		CString strMac = _T("");
		strMac = strRecv.Mid(6, 8);
		strEncond = strRecv.Mid(15, 100*3);
		BYTE out[1000];
		Decode(strEncond,out,0,199);
		CString strTempAnswer = _T("");
		HRESULT hr = E_FAIL;
		hr = TransferAnswer(out,strTempAnswer);
		if(FAILED(hr))
		{
			return FALSE;
		}
		strAnswer = strTempAnswer;

		return TRUE;
	}
	else
		return FALSE;
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
/**********************************
说明：数据校验 
对收上来的字符串进行异或校验and 和校验
**********************************/
BOOL CYDAppCom::proofdata(CString strData)
{
	strData = strData.Mid(0, 110*3);
	if (strData.Right(6) == _T("b6 00 "))
	{
		CString strParityBit = strData.Mid(105*3,2);
		CString strSumBit = strData.Mid(106*3, 5);

		//int iSumBit = Str2Dec(strSumBit);
		int iSum = 0;
		CString strCheckdata = strData.Mid(0, 105*3);
		CString strOP = _T("00");

		for(int i = 0; i < (strCheckdata.GetLength()/3); i++)
		{
			CString str1 = strCheckdata.Mid(i*3, 2);
			int i1 = Str2Dec(str1);
			iSum += i1;
			int i2 = Str2Dec(strOP);

			int i3 = i1 ^ i2;
			strOP.Format(_T("%x"), i3);
			strOP = FormatNO(strOP);
		}
		strParityBit.TrimLeft();
		strParityBit.TrimRight();
		strOP.TrimRight();
		strOP.TrimLeft();
		if (strParityBit.CompareNoCase(strOP) == 0)
		{
			CString strSum = FormatLength(DectoHexstr(iSum));
			if (strSumBit.CompareNoCase(strSum))
				return TRUE;
			else
				return FALSE;
		}
		else
			return FALSE;



	}
	else
		return FALSE;
}
/************************************************
说明：字符串转10进制数
************************************************/
int CYDAppCom::Str2Dec(CString str)
{
	BYTE in[1];
	memset(in,0,sizeof(in));
	String2Hex(str,(char *)in);	
	CString strOut = _T("");
	strOut.Format(_T("%d"),in[0]);

	int i = _ttoi(strOut);
	return i;
}

CString CYDAppCom::FormatNO(CString strNO)
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
		String2Hex(input,(char *)in);
		memset(out,0,sizeof(out));
		PDecodeAnswer(in,min,max,out);
		FreeLibrary(hinstDLL);
	}
	return S_OK;
}

int CYDAppCom::String2Hex(CString str, char *SendOut)
{
	int hexdata,lowhexdata; 
	int hexdatalen=0;
	int len=str.GetLength();
	for(int i=0;i<len;)
	{
		TCHAR lstr,hstr=str[i];
		if(hstr==' '||hstr=='\r'||hstr=='\n')
		{
			i++;
			continue;
		}
		i++;
		if (i>=len)
			break;
		lstr=str[i];
		hexdata=ConvertHexData(hstr);
		lowhexdata=ConvertHexData(lstr);
		if((hexdata==16)||(lowhexdata==16))
			break;
		else
			hexdata=hexdata*16+lowhexdata;
		i++;
		SendOut[hexdatalen]=(char)hexdata;
		hexdatalen++;
	}
	return hexdatalen;
}

TCHAR CYDAppCom::ConvertHexData(TCHAR ch)
{
	if((ch>='0')&&(ch<='9'))
		return ch-0x30;
	if((ch>='A')&&(ch<='F'))
		return ch-'A'+10;
	if((ch>='a')&&(ch<='f'))
		return ch-'a'+10;
	else return(-1);
}

