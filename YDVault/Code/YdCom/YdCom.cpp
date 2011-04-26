#include "stdafx.h"
#include "YDCom.h"
#include "direct.h"


#define MAXBLOCK 8192
#define XON 0x11
#define XOFF 0x13

CYDCom::CYDCom()
{
	m_bConnected = FALSE;
}

CYDCom::~CYDCom()
{
	CloseComm();
}
/***********************************************
函数名：SetCommParameter
参数：CString Seriou(com口)
说明：设置com口的参数
***********************************************/

BOOL CYDCom::SetCommParameter(CString Seriou,DWORD _dwBaudRate )
{	
	m_SeriouStr = Seriou;
	DCB dcb;
	if(!GetCommState(m_hCom,&dcb))
		return FALSE;
	dcb.BaudRate = _dwBaudRate;					//设置波特率；
	dcb.ByteSize = 8;						//设置数据位
	dcb.Parity = NOPARITY;					//校验位
	dcb.StopBits = ONESTOPBIT;				//停止位
	dcb.fInX = FALSE;
	dcb.fOutX = FALSE;
	dcb.XonChar = XON;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fRtsControl = FALSE;
	dcb.fDtrControl = FALSE;
	dcb.XoffChar = XON;
	dcb.XonLim = 500;
	dcb.XoffLim = 500;
	dcb.fNull = FALSE;
	
	BOOL flag = SetCommState(m_hCom,&dcb);
	return(SetCommState(m_hCom,&dcb));
}

/***********************************************
函数名：OpenComm
参数：CString Seriou(com口)
说明：打开com口
***********************************************/
BOOL CYDCom::OpenComm(CString Seriou,DWORD _dwBaudRate /*= cBlueRedRate*/)
{
	if(m_bConnected)
	{
		return TRUE;
	}
	SetCommParameter(Seriou,_dwBaudRate);
	memset(&m_osRead,0,sizeof(OVERLAPPED));
	memset(&m_osWrite,0,sizeof(OVERLAPPED));
	m_osRead.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	if(m_osRead.hEvent==NULL)
	{
		AfxMessageBox(L"wrong2");
		return FALSE;
	}
	m_osWrite.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	if(m_osWrite.hEvent==NULL)
	{
		AfxMessageBox(L"wrong3");
		return FALSE;
	}
	
	COMMTIMEOUTS TimeOuts;

	m_hCom=CreateFile(m_SeriouStr,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,
	FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL);

	if(m_hCom==INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox("打开串口失败!");
		m_bConnected=0;
		return FALSE;
	}
	//设置工作者线程响应的事件
	SetCommMask(m_hCom,EV_RXCHAR);

	//设置读写缓冲区
	SetupComm(m_hCom,MAXBLOCK,MAXBLOCK);

	//设置超时
	TimeOuts.ReadIntervalTimeout=MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier=MAXDWORD;
	TimeOuts.ReadTotalTimeoutConstant=MAXDWORD;
	TimeOuts.WriteTotalTimeoutMultiplier=MAXDWORD;
	SetCommTimeouts(m_hCom,&TimeOuts);

	if(!SetCommParameter(Seriou,_dwBaudRate))
	{
		CloseHandle(m_hCom);
	//	AfxMessageBox(L"参数设置失败!");
		m_bConnected=0;
		return FALSE;
	}
	m_bConnected=1;
	return TRUE;
}
/***********************************************
函数名：CloseComm
参数：
说明：关闭com口
***********************************************/

void CYDCom::CloseComm()
{
	if(m_bConnected)
	{
		CloseHandle(m_hCom);
		m_bConnected = FALSE;
	}
}

BOOL CYDCom::Send(CString str)
{
	CString filepath;
	TCHAR abOut[MAXBLOCK];
	int length;
	memset(abOut,0,MAXBLOCK);

	char SendOut[MAXBLOCK];
	int len=str.GetLength();
	for(int i=0;i<len;i++)
		abOut[i]=str.GetAt(i);
	CString StrHexData;
	abOut[len]=NULL;
	StrHexData=CString(abOut);
	len=String2Hex(StrHexData,SendOut);
	length=WriteBlock(SendOut,len);
	if ( length <= 0 )
		return false;
	return true;
}
/***********************************************
函数名：ReadData
参数：
说明：从com口读取到数据，用字符串返回
***********************************************/
CString CYDCom::ReadData()
{
	COMSTAT ComStat;
	DWORD dwErrorFlags;

	OVERLAPPED os;
	memset(&os,0,sizeof(OVERLAPPED));
	os.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
	
	if(os.hEvent==NULL)
	{
		return L"";
	}
	CString data = L"";
	ClearCommError(m_hCom,&dwErrorFlags,&ComStat);
	if(ComStat.cbInQue)
	{
       	BYTE abIn[MAXBLOCK];
        int len = ReadBlock(abIn,MAXBLOCK);
	
		if ( len > 0 )
		{
			for(int i=0;i<len;i++)
			{
				BYTE bt=abIn[i];
				CString StrTemp ;
				StrTemp.Format(L"%02x ",bt);
				data+=StrTemp;
			}
		}
	}
	/*	DWORD dwMask=0,dwTrans=0;
	if(!WaitCommEvent(m_hCom,&dwMask,&os))
	{
		if(GetLastError()==ERROR_IO_PENDING)
		{
			GetOverlappedResult(m_hCom,&os,&dwTrans,TRUE);
		}
		else
		{
			CloseHandle(os.hEvent);
		}
	}*/
	CloseHandle(os.hEvent);
	return data;
}

/*******************************
函数名：String2Hex
参数：CString str, char *SendOut
说明：将字符串转为十六进制的整数
*******************************/
int CYDCom::String2Hex(CString str, char *SendOut)
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
/*******************************
函数名：ConvertHexData
参数：char ch
说明：转换十六进制数
*******************************/
TCHAR CYDCom::ConvertHexData(TCHAR ch)
{
	if((ch>='0')&&(ch<='9'))
		return ch-0x30;
	if((ch>='A')&&(ch<='F'))
		return ch-'A'+10;
	if((ch>='a')&&(ch<='f'))
		return ch-'a'+10;
    else return(-1);
}

int CYDCom::ReadBlock(BYTE *abIn,int MaxLength)
{
	BOOL JudgeRead;
	COMSTAT ComStat;
	DWORD dwErrorFlags,dwLength;
	ClearCommError(m_hCom,&dwErrorFlags,&ComStat);
	if(dwErrorFlags>0)
	{
		//AfxMessageBox(L"读串口错，请检查参数设置。");
		PurgeComm(m_hCom,PURGE_RXABORT|PURGE_RXCLEAR);
		return 0;
	}
	dwLength = ((DWORD)MaxLength<ComStat.cbInQue?MaxLength:ComStat.cbInQue);
	memset(abIn,0,MaxLength);
	//如果有字符即读入
	if(dwLength)
	{
		JudgeRead=ReadFile(m_hCom,abIn,dwLength,&dwLength,&m_osRead);//读出字符至abIn处
		if(!JudgeRead)
		{
			//如果重叠操作未完成,等待直到操作完成
			if(GetLastError()==ERROR_IO_PENDING)
			{
//				WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hCom,&m_osRead,&dwLength,TRUE);
				m_osRead.Offset=0;
//				m_osRead.Offset=(m_osRead.Offset+dwLength)%MAXBLOCK;
			}
			else
				dwLength=0;
		}
	}
	return dwLength;
}

int CYDCom::WriteBlock(char *abOut,int MaxLength)
{
	BOOL JudgeWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags,dwLength,lentest;
	m_osWrite.Offset=0;
	ClearCommError(m_hCom,&dwErrorFlags,&ComStat);
	if(dwErrorFlags>0)
	{
		//AfxMessageBox(L"写串口错！请检查参数设置。");
		PurgeComm(m_hCom,PURGE_TXABORT|PURGE_TXCLEAR);
		return 0;
	}
	dwLength=MaxLength;

	lentest=0;
	JudgeWrite=WriteFile(m_hCom,abOut,dwLength,&lentest,&m_osWrite);

	if(!JudgeWrite)
	{

		if(GetLastError()==ERROR_IO_PENDING)
		{
			GetOverlappedResult(m_hCom,&m_osWrite,&lentest,TRUE);
		}
		else
			lentest=0;
	}
	return lentest;
}

CString CYDCom::Read()
{
	CString ret = ReadData();
	if ( ret.IsEmpty() )
	{
		ret = ReadData();
	}
	if ( !ret.IsEmpty() )
	{
		while ( ret.GetLength() < 9 )
		{
			CString temp = ReadData();
			ret = ret + temp;
		}
		CString end = ret.Right(9);
		end.TrimLeft();
		end.TrimRight();
		while (end != L"0d 0d 0a")
		{
			CString temp = ReadData();
			ret = ret + temp;
			end = ret.Right(9);	
			end.TrimLeft();
		    end.TrimRight();
		}
	}
	//wht 增加，用来在8D指令时，会有2个od od oa
	Sleep(500);
	CString temp = ReadData();
	if (temp.IsEmpty())
	{
		temp = ReadData();
	}
	if (!temp.IsEmpty())
	{
		while (temp.GetLength() < 9)
		{
			CString temp1 = ReadData();
			temp = temp + temp1;
		}
		CString end = temp.Right(9);
		end.TrimLeft();
		end.TrimRight();
		while (end != "0d 0d 0a")
		{
			CString temp1 = ReadData();
			temp = temp + temp1;
			end = temp.Right(9);	
			end.TrimLeft();
			end.TrimRight();
		}
	}

	ret = ret + temp;

	return ret;
}

CString	CYDCom::ReadInfraRed()
{
	CString ret = _T("");
	CString str2 = _T("b6 00");
	str2.TrimRight();
	str2.TrimLeft();
	CString str1 = _T("");
	DWORD dwStart = GetTickCount();
	do 
	{
		ret = ret + ReadData();
		str1 = ret.Right(6);
		str1.TrimLeft();
		str1.TrimRight();
		if((GetTickCount() - dwStart)/1000 > 3)
		{
			//3秒钟还没有读到，则认为没有读到数据
			break;
		}
	} while (ret.GetLength() < 110*3 && (str1.CompareNoCase(str2) != 0));
	return ret;
}
BOOL CYDCom::Close()
{
	 CloseComm();
	 return true;
}

HRESULT	CYDCom::GetState(BOOL &_bOpen)
{
	_bOpen = m_bConnected;
	return S_OK;
}