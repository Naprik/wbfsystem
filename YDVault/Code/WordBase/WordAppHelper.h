// WordAppHelper.h: interface for the CWordAppHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORDAPPHELPER_H__19AD807B_64D5_4E21_9D0E_49B78BD56249__INCLUDED_)
#define AFX_WORDAPPHELPER_H__19AD807B_64D5_4E21_9D0E_49B78BD56249__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "msword.h"

class BASE_DLL_API CWordAppHelper  
{
public:
	CWordAppHelper();
	virtual ~CWordAppHelper();
	HRESULT Create();
	HRESULT Write(CString _str);
	HRESULT StartUnline();//开始下划线
	HRESULT EndUnline();//结束下划线
	HRESULT Save(CString _strFile);
	HRESULT AddOleObject(CString _strFile);
private:
	_Application m_wdApp;
    Documents m_wdDocs;
    _Document m_wdDoc;
    Selection m_wdSel;
	BOOL m_bCreate;//是否创建过
};

#endif // !defined(AFX_WORDAPPHELPER_H__19AD807B_64D5_4E21_9D0E_49B78BD56249__INCLUDED_)
