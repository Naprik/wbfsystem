#pragma once
/*************************************************
类名:CYDCom
说明:com口通信类
*************************************************/
class CYDCom
{
public:  
	BOOL Close();
	CYDCom();
	~CYDCom();
	BOOL		  OpenComm(CString Seriou);                
	void		  CloseComm();
	BOOL		  Send(CString str);
	CString		  ReadData();
	CString		  Read();    
private:
	HANDLE        m_hCom;
	BOOL          SetCommParameter(CString Seriou);
	CString	      m_SeriouStr;
	volatile int  m_bConnected;
	CWinThread    *m_pThread;
	int           String2Hex(CString str, char *SendOut);
	char		  ConvertHexData(char ch);
	OVERLAPPED	  m_osRead,m_osWrite; 
	int			  ReadBlock(BYTE *abIn,int MaxLength);
	int			  WriteBlock(char *abOut,int MaxLength);

};


