// LanguageTranslator.cpp: implementation of the CLanguageTranslator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LanguageTranslator.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "AutoClean.h"
#include "../Base/FilePathHelper.h"

extern TCHAR _ModulePath[MAX_PATH];


std::map<UINT,CString> CLanguageTranslator::m_mapError;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLanguageTranslator::CLanguageTranslator()
{

}

CLanguageTranslator::~CLanguageTranslator()
{

}

BOOL CLanguageTranslator::Translate(UINT msg_code,CString& strMsg)
{
	//先判断是否为windows的系统错误
	HLOCAL hlocal = NULL;   
	BOOL fOk = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, 
		NULL, msg_code, 
		///*MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)*/0,
		MAKELANGID(LANG_CHINESE_SIMPLIFIED, SUBLANG_CHINESE_SIMPLIFIED),
		
		//MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(PTSTR)&hlocal, 0, NULL);
	if (hlocal != NULL)
	{
		CString str = (PCTSTR)LocalLock(hlocal);
		LocalFree(hlocal);
		strMsg = str;
		return TRUE;
	} 

	//再判断是否是windows的com标准错误
	HRESULT hr = E_FAIL;
	CComQIPtr < IErrorInfo > spErrInfo;
	hr = ::GetErrorInfo( 0, &spErrInfo);
	if( SUCCEEDED( hr ) && spErrInfo != NULL )
	{
		_com_error err(msg_code,spErrInfo,true);
		_bstr_t sMsg = err.Description();
		::SetErrorInfo( 0, NULL);
		if(sMsg.length() > 0)
		{
			strMsg = (const TCHAR*)sMsg;
			return S_OK;
		}
	}
	
	for(std::map<UINT,CString>::const_iterator itr = m_mapError.begin();
		itr != m_mapError.end();++itr)
	{
		if((*itr).first == msg_code)
		{
			strMsg = (*itr).second;
			m_mapError.clear();
			return S_OK;
		}
	}
	m_mapError.clear();

	CString path;
	CFilePathHelper::GetMainCfgPath(path);
	path += _T("\\cfg\\msg.xml");

	CFileStatus status;
	if( !CFile::GetStatus(path,status))
		return FALSE;
	else
	{
		if(status.m_size >= 64*1024 || status.m_size <= 0)
			return FALSE;
	}
	
	TCHAR s[16] = {0};
	_stprintf_s(s,_T("0x%08x"),msg_code);

	try 
	{
		CFile file(path,CFile::modeRead);
		BYTE *sBuf = (BYTE*)(new BYTE[(UINT)status.m_size]);
		memset(sBuf,0,(size_t)status.m_size);
		CArrayAutoClean<BYTE> clean(sBuf);
		CString str;
		int nRet = file.Read(sBuf,(UINT)status.m_size-1);
		sBuf[nRet] = 0;
		str = sBuf;

		int nPos = str.Find(s,0);
		if(-1 != nPos)
		{
			int nPos1 = str.Find(_T("tip="),nPos);
			if(nPos1 == -1) 
			{
				return FALSE;
			}
			//nPos1 += sizeof(_T("tip="));
			nPos1 += _tcslen(_T("tip="))+1;
			int nPos2 = str.Find(_T(">"),nPos);
			if(nPos2 == -1)
			{
				return FALSE;
			}

			strMsg = str.Mid(nPos1,nPos2-nPos1-1);
			return TRUE;
		}
		return FALSE;
	}
	catch(CFileException*)
	{
		return FALSE;
	}
}

BOOL CLanguageTranslator::InsertError(UINT _msg_code,CString _strMsg)
{
	m_mapError.insert(std::map<UINT,CString>::value_type(_msg_code,_strMsg));
	return TRUE;
}