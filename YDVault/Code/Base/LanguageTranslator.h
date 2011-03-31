// LanguageTranslator.h: interface for the CLanguageTranslator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANGUAGETRANSLATOR_H__EF48BC13_3E24_4FBE_A3EE_7F741E3BDB22__INCLUDED_)
#define AFX_LANGUAGETRANSLATOR_H__EF48BC13_3E24_4FBE_A3EE_7F741E3BDB22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <list>
#include <map>

class BASE_DLL_API CLanguageTranslator  
{
public:
	CLanguageTranslator();
	virtual ~CLanguageTranslator();

	BOOL Translate(UINT msg_code,CString& strMsg);
//	BOOL LoadDictionary(const CString& dic_file);
	BOOL InsertError(UINT _msg_code,CString _strMsg);
private:
//	std::map<UINT,CString>     m_mapDictionary;
	static std::map<UINT,CString> m_mapError;//²¶»ñµÄÒì³£´íÎóÂð
};

#endif // !defined(AFX_LANGUAGETRANSLATOR_H__EF48BC13_3E24_4FBE_A3EE_7F741E3BDB22__INCLUDED_)
