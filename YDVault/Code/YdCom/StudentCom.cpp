#include "StdAfx.h"
#include "StudentCom.h"

CStudentCom::CStudentCom(void)
:m_com(_T("Com1"))
{
	
}

CStudentCom::~CStudentCom(void)
{
}

HRESULT  CStudentCom::LinkRolling(int iStart, int iEnd,CString &_str)
{
	HRESULT hr = E_FAIL;
	hr =  m_com.LinkRolling(_str);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
HRESULT CStudentCom::LinkSuccess()
{
	HRESULT hr = E_FAIL;
	hr = m_com.Linksuccess();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CStudentCom::Opencom(CString strCom, CString strMac)
{
	HRESULT hr = E_FAIL;
// 	strCom.TrimRight();
// 	strMac.TrimRight();
// 	m_port = strCom;
// 	m_mac = strMac;
// 	hr = m_com.Opencom(m_port);
// 	if(FAILED(hr))
// 	{
// 		return hr;
// 	}
// 	m_isuse = TRUE;
	return S_OK;
}

HRESULT CStudentCom::Closecom()
{
	HRESULT hr = E_FAIL;
// 	if(m_isuse == TRUE)
// 	{
// 		hr = m_com.Close();
// 		if(FAILED(hr))
// 		{
// 			return hr;
// 		}
// 		m_isuse = FALSE;
// 	}
	return S_OK;

	//return m_com.Close();
}
HRESULT CStudentCom::Get_Comstat_text(CString &_str)
{
	CString ret;
// 	if(m_isuse )
// 	{
// 		_str.Format(_T("打开串口 %s 成功！"), m_port);
// 	}
// 	else
// 	{
// 		_str.Format(_T("打开串口 %s 失败！"), m_port);
// 	}
	return S_OK;
}
