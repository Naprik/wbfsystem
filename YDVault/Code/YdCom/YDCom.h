#pragma once
/*************************************************
类名:CYDCom
说明:com口通信类,用来读写端口数据，打开，关闭端口
*************************************************/
class AFX_EXT_CLASS CYDCom
{
public:  
	BOOL Close();
	CYDCom();
	~CYDCom();
	virtual BOOL		  OpenComm(CString Seriou,DWORD _dwBaudRate = 9600);                
	void		  CloseComm();
	BOOL		  Send(CString str);
	CString		  ReadData();
	CString		  Read(); 
	CString		  ReadInfraRed();//红外教师机读取数据调用
	HRESULT		GetState(BOOL &_bOpen);
protected:
	virtual BOOL          SetCommParameter(CString Seriou,DWORD _dwBaudRate );
private:
	HANDLE        m_hCom;
	CString	      m_SeriouStr;
	volatile BOOL  m_bConnected;
	CWinThread    *m_pThread;
	int           String2Hex(CString str, char *SendOut);
	TCHAR		  ConvertHexData(TCHAR ch);
	OVERLAPPED	  m_osRead,m_osWrite; 
	int			  ReadBlock(BYTE *abIn,int MaxLength);
	int			  WriteBlock(char *abOut,int MaxLength);

};


