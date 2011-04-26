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
��������SetCommParameter
������CString Seriou(com��)
˵��������com�ڵĲ���
***********************************************/

BOOL CYDCom::SetCommParameter(CString Seriou,DWORD _dwBaudRate )
{	
	m_SeriouStr = Seriou;
	DCB dcb;
	if(!GetCommState(m_hCom,&dcb))
		return FALSE;
	dcb.BaudRate = _dwBaudRate;					//���ò����ʣ�
	dcb.ByteSize = 8;						//��������λ
	dcb.Parity = NOPARITY;					//У��λ
	dcb.StopBits = ONESTOPBIT;				//ֹͣλ
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
��������OpenComm
������CString Seriou(com��)
˵������com��
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
		//AfxMessageBox("�򿪴���ʧ��!");
		m_bConnected=0;
		return FALSE;
	}
	//���ù������߳���Ӧ���¼�
	SetCommMask(m_hCom,EV_RXCHAR);

	//���ö�д������
	SetupComm(m_hCom,MAXBLOCK,MAXBLOCK);

	//���ó�ʱ
	TimeOuts.ReadIntervalTimeout=MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier=MAXDWORD;
	TimeOuts.ReadTotalTimeoutConstant=MAXDWORD;
	TimeOuts.WriteTotalTimeoutMultiplier=MAXDWORD;
	SetCommTimeouts(m_hCom,&TimeOuts);

	if(!SetCommParameter(Seriou,_dwBaudRate))
	{
		CloseHandle(m_hCom);
	//	AfxMessageBox(L"��������ʧ��!");
		m_bConnected=0;
		return FALSE;
	}
	m_bConnected=1;
	return TRUE;
}
/***********************************************
��������CloseComm
������
˵�����ر�com��
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
��������ReadData
������
˵������com�ڶ�ȡ�����ݣ����ַ�������
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
��������String2Hex
������CString str, char *SendOut
˵�������ַ���תΪʮ�����Ƶ�����
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
��������ConvertHexData
������char ch
˵����ת��ʮ��������
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
		//AfxMessageBox(L"�����ڴ�����������á�");
		PurgeComm(m_hCom,PURGE_RXABORT|PURGE_RXCLEAR);
		return 0;
	}
	dwLength = ((DWORD)MaxLength<ComStat.cbInQue?MaxLength:ComStat.cbInQue);
	memset(abIn,0,MaxLength);
	//������ַ�������
	if(dwLength)
	{
		JudgeRead=ReadFile(m_hCom,abIn,dwLength,&dwLength,&m_osRead);//�����ַ���abIn��
		if(!JudgeRead)
		{
			//����ص�����δ���,�ȴ�ֱ���������
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
		//AfxMessageBox(L"д���ڴ�����������á�");
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
	//wht ���ӣ�������8Dָ��ʱ������2��od od oa
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
			//3���ӻ�û�ж���������Ϊû�ж�������
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