// YDMCIControl.cpp: implementation of the CYDMCIControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YDMCIControl.h"
#include <Vfw.h>
#include <atlbase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYDMCIControl::CYDMCIControl()
{
	m_MCIWnd = NULL;
}

CYDMCIControl::~CYDMCIControl()
{
	Close();
}

HWND CYDMCIControl::GetSafeHwnd() const
{
	return m_MCIWnd;
}

HRESULT CYDMCIControl::Close()
{
	if(m_MCIWnd != NULL)
	{
		MCIWndClose(m_MCIWnd);
		m_MCIWnd = NULL;
	}
	return S_OK;
}

HRESULT CYDMCIControl::OpenMCI(CString _strFile)
{
	HRESULT hr = E_FAIL;
	hr = Close();
	if(FAILED(hr))
	{
		return hr;
	}
	m_MCIWnd = MCIWndCreate(AfxGetMainWnd()->GetSafeHwnd(), 
		AfxGetInstanceHandle(), 
		WS_CHILD|MCIWNDF_NOMENU,_strFile); 
	return S_OK;
}

HRESULT CYDMCIControl::Play()
{
	HRESULT hr = E_FAIL;
	if(m_MCIWnd)
	{
		MCIWndPlay(m_MCIWnd);
	}
	return S_OK;
}

HRESULT CYDMCIControl::Stop()
{
	HRESULT hr = E_FAIL;
	if(m_MCIWnd)
	{
		MCIWndStop(m_MCIWnd);
		MCIWndSeek(m_MCIWnd, 0); 
	}
	return S_OK;
}

HRESULT CYDMCIControl::Pause()
{
	HRESULT hr = E_FAIL;
	if(m_MCIWnd)
	{
		MCIWndPause(m_MCIWnd);
	}
	return S_OK;
}

HRESULT CYDMCIControl::GetLength(long &_len)
{
	HRESULT hr = E_FAIL;
	if(m_MCIWnd)
	{
		_len = MCIWndGetLength(m_MCIWnd);
	}
	return S_OK;
}

HRESULT CYDMCIControl::GetPosition(long &_pos)
{
	HRESULT hr = E_FAIL;
	if(m_MCIWnd)
	{
		_pos = MCIWndGetPosition(m_MCIWnd);
	}
	return S_OK;
}

HRESULT CYDMCIControl::BeginRecord(HWND _hWnd)
{
	HRESULT hr = E_FAIL;
	hr = Close();
	if(FAILED(hr))
	{
		return hr;
	}
	m_MCIWnd = MCIWndCreate(_hWnd, 
		AfxGetInstanceHandle(), 
		WS_CAPTION,NULL);
	if(MCIWndNew(m_MCIWnd,_T("waveaudio")))
	{
		return S_FALSE;
	}
	if(!MCIWndCanRecord(m_MCIWnd))
	{
		return S_FALSE;
	}
	MCIWndRecord(m_MCIWnd);
	

	return S_OK;
}
HRESULT CYDMCIControl::EndRecord()
{
	HRESULT hr = E_FAIL;
	if(m_MCIWnd)
	{
		MCIWndStop(m_MCIWnd);
	}
	return S_OK;
}
	
HRESULT CYDMCIControl::SaveRecord(CString _strFile)
{
	HRESULT hr = E_FAIL;
	if(MCIWndCanSave(m_MCIWnd))
	{
		CComBSTR bstrFile = _strFile;
		if(MCIWndSave(m_MCIWnd, bstrFile) != 0)
		{
			AfxMessageBox(_T("err"));
		}
		//这里有点问题要说说了,MSDN里面写,这个MCIWndSave可以直接保存文件,第二个参数传入一个-1就会自动打开保存框给我们选择,但是我试了N久都没搞定,(也请高手看看,谁搞定了mail给我一份)它不打开保存框给我,但是返回一个成功值!靠~!还有直接保存为文件,文件名只要大于一位就是乱码,所以我在这里绕了一个圈,先保存成一个a,拷贝改名为要保存的文件名,最后在结束里面删除掉这个a.
		//	CopyFile("a",m_Path,FALSE);
	}
	return S_OK;
}