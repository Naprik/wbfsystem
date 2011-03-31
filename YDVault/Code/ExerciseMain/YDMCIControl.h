// YDMCIControl.h: interface for the CYDMCIControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YDMCICONTROL_H__F95C9E42_9761_4DA8_90F5_3915754170C6__INCLUDED_)
#define AFX_YDMCICONTROL_H__F95C9E42_9761_4DA8_90F5_3915754170C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CYDMCIControl  
{
public:
	CYDMCIControl();
	virtual ~CYDMCIControl();
	HWND GetSafeHwnd() const;
	HRESULT Close();
	HRESULT OpenMCI(CString _strFile);//打开音频文件
	HRESULT Play();
	HRESULT Stop();
	HRESULT Pause();
	HRESULT GetLength(long &_len);
	HRESULT GetPosition(long &_pos);
	HRESULT BeginRecord(HWND _hWnd);
	HRESULT EndRecord();
	HRESULT SaveRecord(CString _strFile);
private:
	HWND m_MCIWnd;
};

#endif // !defined(AFX_YDMCICONTROL_H__F95C9E42_9761_4DA8_90F5_3915754170C6__INCLUDED_)
