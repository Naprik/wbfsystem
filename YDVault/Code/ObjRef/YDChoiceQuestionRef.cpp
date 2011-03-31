#include "StdAfx.h"
#include "YDChoiceQuestionRef.h"
#include "../Base/DataHandler.h"



CYDChoiceQuestionRef::CYDChoiceQuestionRef(CDatabaseEx* pDb)
:CYDQuestionRef(pDb)
{
	m_strDBName = DB_CHOICEQUESTION;
	AddPropDef(_T("CODE"),VT_BSTR,_T(""));
	AddPropDef(_T("TITLE"),VT_BLOB);
	AddPropDef(_T("CHOICE"),VT_BSTR);
	AddPropDef(_T("ANSWER"),VT_BSTR);
	AddPropDef(_T("TYPEID"),VT_I4);
	AddPropDef(_T("HARDLEVEL"),VT_I4);
	AddPropDef(_T("USEDCOUNT"), VT_I4, L"0");
	AddPropDef(_T("CREATOR"),VT_BSTR);
	AddPropDef(_T("CREATEDATE"),VT_DATE);
	AddPropDef(_T("ANSWERDESC"),VT_BSTR);
	

	for (int i = int(L'A'); i <= int(L'Z'); ++i)
	{
		TCHAR ch = i;
		CString str(ch);
		str += L") ";
		m_LstSequence.push_back(str);
	}
}

CYDChoiceQuestionRef::~CYDChoiceQuestionRef(void)
{
}

bool CYDChoiceQuestionRef::ToString(CString* pStr)
{
	_variant_t var;
	GetPropVal(L"TITLE", &var);
	CString strTitle = CDataHandler::VariantToString(var);
	*pStr = strTitle;
	
	*pStr += L"\r\n";//ÌâÄ¿
	
	std::list<CString> lstOptions;
	GetOptionList(&lstOptions);
	bool bChoice = lstOptions.size() > 1 ? true : false;
	std::list<CString>::const_iterator itrSeq = m_LstSequence.begin();
	for (std::list<CString>::const_iterator itr = lstOptions.begin();
		itr != lstOptions.end(); ++itr, ++itrSeq)
	{
		if (bChoice)
		{
			*pStr += *itrSeq;
		}
		*pStr += *itr;
		*pStr += L"\r\n";
	}

	return true;
}

HRESULT CYDChoiceQuestionRef::GetLabel(CString* pStr)
{
	HRESULT hr = E_FAIL;
	CComVariant valTitle;
	
	QTYPE type;
	hr = GetQuestionType(&type);
	if (FAILED(hr))
	{
		return hr;
	}
	if (type == QTYPE_LISTENINGCOMPREHENSION ||
		type == QTYPE_READINGVOCABULARY||
		type == QTYPE_LISTENINGLONG ||
		type == QTYPE_CLOZE ||
		type == QTYPE_LISTENINGSHORT)
	{
		hr = GetPropVal(L"CHOICE", &valTitle);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	else if (type == QTYPE_LISTENINGVOCABULARY ||
		type == QTYPE_ERRORCORRECTION)
	{
		hr = GetPropVal(L"ANSWER", &valTitle);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	else
	{
		hr = GetPropVal(L"TITLE",&valTitle);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	
	*pStr = CDataHandler::VariantToString(valTitle);
	return S_OK;
}


HRESULT CYDChoiceQuestionRef::GetOptionList(std::list<CString>* lst)
{
	HRESULT hr = E_FAIL;
	
	CComVariant varChoice;
	hr = GetPropVal(L"CHOICE", &varChoice);
	if (FAILED(hr))
	{
		return hr;
	}
	CString str = CDataHandler::VariantToString(varChoice);
	str.Trim();
	std::list<CString>::const_iterator itr = m_LstSequence.begin();
	++itr;
	int iBeginIndex = 0 + (*itr).GetLength();
	int iEndIndex = str.GetLength();
	for (; itr != m_LstSequence.end(); ++itr)
	{
		int iPos = str.Find(*itr);
		if (iPos < 0)
		{
			break;
		}
		int len = iPos - iBeginIndex;
		if (len <= 0)
		{
			break;
		}
		CString strOption = str.Mid(iBeginIndex, len);
		iBeginIndex = iPos + (*itr).GetLength();
		lst->push_back(strOption);
	}
	CString strLast = str.Mid(iBeginIndex, iEndIndex - iBeginIndex);
	if (strLast.GetLength() > 0)
	{
		lst->push_back(strLast);
	}
	
	return S_OK;
}

HRESULT CYDChoiceQuestionRef::GetAnswerList(std::list<CString>* lst)
{
	HRESULT hr = E_FAIL;

	CComVariant var;
	hr = GetPropVal(L"ANSWER", &var);
	if (FAILED(hr))
	{
		return hr;
	}
	CString strAnswer = CDataHandler::VariantToString(var);
	QTYPE type;
	hr = GetQuestionType(&type);
	if (FAILED(hr))
	{
		return hr;
	}
	if (type == QTYPE_READINGVOCABULARY)
	{
		for (int i=0; i<strAnswer.GetLength(); ++i)
		{
			CString str(strAnswer.GetAt(i));
			lst->push_back(str);
		}
	}
	else
	{
		lst->push_back(strAnswer);
	}
	
	return S_OK;
}

HRESULT CYDChoiceQuestionRef::SetOptionList(std::list<CString>* lst)
{
	HRESULT hr = E_FAIL;
	CString strOption = L"";
	std::list<CString>::const_iterator itrSequence = m_LstSequence.begin();
	std::list<CString>::const_iterator itrOption = lst->begin();
	for (; itrOption != lst->end(); ++itrOption)
	{
		if (itrOption->GetLength() <= 0)
		{
			break;
		}
		strOption += *itrSequence;
		strOption += *itrOption;
		++itrSequence;
	}
	CComVariant var(strOption);
	SetPropVal(L"CHOICE", &var);

	return S_OK;
}

HRESULT CYDChoiceQuestionRef::PersistToMSWord(int* pIndex, 
											  CWordAppHelper* pWord, 
											  CWordAppHelper* pWordAnswer,
											  const CString& strFolder)
{
	HRESULT hr = E_FAIL;
	QTYPE type;
	hr = GetQuestionType(&type);
	if (FAILED(hr))
	{
		return hr;
	}
	CString strQuestion;
	ToString(&strQuestion);
	
	if (type != QTYPE_WRITING)
	{
		CString strIndex;
		strIndex.Format(L"%d. ", *pIndex);
		strQuestion = strIndex + strQuestion;

		(*pIndex)++;
	}
	
	hr = pWord->Write(strQuestion);
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<CString> lstAnswer;
	GetAnswerList(&lstAnswer);
	ASSERT(lstAnswer.size() == 1);
	CString strAnswer = *(lstAnswer.begin());
	strAnswer += L"\r\n";
	hr = pWordAnswer->Write(strAnswer);
	if (FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}