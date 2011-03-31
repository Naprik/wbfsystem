#include "StdAfx.h"
#include "YdKnowledge.h"
#include "../Base/DataHandler.h"
#include "../DBBase/DatabaseEx.h"

CYdKnowledge::CYdKnowledge(CDatabaseEx* pDB)
: CYDObjectRef(pDB)
{
	m_strDBName = DB_KNOWLEDGEPOINT;
	AddPropDef(_T("NAME"),VT_BSTR,_T(""));
	AddPropDef(_T("CODE"),VT_BSTR);
	AddPropDef(_T("CREATOR"),VT_BSTR);
	AddPropDef(_T("CREATEDATE"),VT_DATE);
	AddPropDef(_T("DESCRIPTION"),VT_BSTR);
}

CYdKnowledge::~CYdKnowledge(void)
{
}

HRESULT CYdKnowledge::GetLabel(CString* pStr)
{
	HRESULT hr = E_FAIL;
	CComVariant valName;
	hr = GetPropVal(L"Name",&valName);
	if(FAILED(hr))
	{
		return hr;
	}
	*pStr = CDataHandler::VariantToString(valName);
	return S_OK;
}

HRESULT CYdKnowledge::IsFieldExistOfVault(const CString& strField, 
										  const CString& strVaule, 
										  OBJID vaultID, 
										  BOOL* pbExist)
{
	HRESULT hr = E_FAIL;
	*pbExist = FALSE;

	CString strSQL;
	strSQL = _T("Select OBJID From ");
	strSQL += m_strDBName;
	strSQL += _T(" Where ");
	strSQL += strField;
	strSQL += L" = '";
	strSQL += strVaule;
	strSQL += L"' and OBJID in (select id_b from ";
	strSQL += DB_QVAULTKPOINT;
	strSQL += L" where id_a = ";
	CString strVaultID;
	strVaultID.Format(L"%d", vaultID);
	strSQL += strVaultID;
	strSQL += L")";

	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}

	hr = m_pDb->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}
	if(!m_pDb->IsEOF())
	{
		*pbExist = TRUE;	
	}
	return S_OK;
}