#pragma once
/*************************************************
����:CYDCom
˵��:com��ͨ����,������д�˿����ݣ��򿪣��رն˿�
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
	HRESULT		GetState(BOOL &_bOpen);
private:
	HANDLE        m_hCom;
	BOOL          SetCommParameter(CString Seriou);
	CString	      m_SeriouStr;
	volatile BOOL  m_bConnected;
	CWinThread    *m_pThread;
	int           String2Hex(CString str, char *SendOut);
	TCHAR		  ConvertHexData(TCHAR ch);
	OVERLAPPED	  m_osRead,m_osWrite; 
	int			  ReadBlock(BYTE *abIn,int MaxLength);
	int			  WriteBlock(char *abOut,int MaxLength);

};


