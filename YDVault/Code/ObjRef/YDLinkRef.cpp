#include "StdAfx.h"
#include "YDLinkRef.h"
#include "../DBBase/DatabaseEx.h"
#include "../Base/DataHandler.h"

CYDLinkRef::CYDLinkRef(CDatabaseEx* pdb,CString _strDBNaem)
			: CYDObjectRef(pdb)
{
	m_pObjRefA = NULL;
	m_pObjRefB = NULL;
	m_strDBName = _strDBNaem;
}

CYDLinkRef::~CYDLinkRef(void)
{
}

HRESULT CYDLinkRef::Update()
{
	HRESULT hr = E_FAIL;

	CString strSQL = _T("Update ");
	strSQL += m_strDBName;
	strSQL += _T(" Set ID_A = ? ,ID_B = ? ");
	for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
	{
		strSQL += _T(", ");
		strSQL += (*itrProp)->m_strPropName;
		strSQL += _T(" = ? ");
	}
	strSQL += _T(" WHERE OBJID = ?");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}
	hr = FillObjIDParamDB();
	if(FAILED(hr))
	{
		return hr;
	}
	hr = FillParamDB();
	if(FAILED(hr))
	{
		return hr;
	}

	hr = FillIDParamDB();
	if(FAILED(hr))
	{
		return hr;
	}
	hr = m_pDb->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}	
	return S_OK;
}

HRESULT CYDLinkRef::LoadByObj()
{
	HRESULT hr = E_FAIL;
	CString strSQL = _T("SELECT OBJID From ");
	strSQL += m_strDBName;
	strSQL += _T(" Where ID_A = ? AND ID_B = ? ");

	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}
	hr = FillObjIDParamDB();
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
		_variant_t valID;
		hr = m_pDb->GetField(CComVariant(L"OBJID"),valID);
		if(FAILED(hr))
		{
			return hr;
		}
		m_uObjID = CDataHandler::VariantToLong(valID);
	}
	return S_OK;
}

HRESULT CYDLinkRef::Save()
{
	HRESULT hr = E_FAIL;
	
	DATABASE_TYPE dbtype = m_pDb->GetDBType();
	if (ORACLE == dbtype)
	{
		return OracleSave();
	}
	else if (SQLSERVER == dbtype)
	{
		return SQLServerSave();
	}
	else if (ACCESS == dbtype)
	{
		return AccessSave();
	}
	else
	{
		ASSERT(FALSE);
	}

	return S_OK;
}


HRESULT CYDLinkRef::OracleSave()
{
	HRESULT hr = E_FAIL;

	CString strSQL = _T("INSERT INTO ");
	strSQL += m_strDBName;
	strSQL += _T(" (ID_A,ID_B ");
	for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
	{
		strSQL += _T(", ");
		strSQL += (*itrProp)->m_strPropName;
	}
	strSQL += _T(" ) VALUES(?,?  ");
	for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
	{
		strSQL += _T(",? ");
	}
	strSQL += _T(")");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}
	hr = FillObjIDParamDB();
	if (FAILED(hr))
	{
		return hr;
	}
	hr = FillParamDB();
	if(FAILED(hr))
	{
		return hr;
	}


	hr = m_pDb->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}	

	CString strGetID = L"select S_";
	strGetID += m_strDBName;
	strGetID += L".currval as ID from dual";
	hr = m_pDb->InitializeSQL(_bstr_t(strGetID));
	if (FAILED(hr))
	{
		return hr;
	}
	hr = m_pDb->ExecuteSQL();
	if (FAILED(hr))
	{
		return hr;
	}
	_variant_t var;
	m_pDb->GetField(L"ID", var);
	m_uObjID = CDataHandler::VariantToLong(var);
	return S_OK;	
}

HRESULT CYDLinkRef::SQLServerSave()
{
	HRESULT hr = E_FAIL;

	m_uObjID++;
	CString strSQL = _T("INSERT INTO ");
	strSQL += m_strDBName;
	strSQL += _T(" (ID_A,ID_B ");
	for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
	{
		strSQL += _T(", ");
		strSQL += (*itrProp)->m_strPropName;
	}
	strSQL += _T(" ) VALUES(?,?  ");
	for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
	{
		strSQL += _T(",? ");
	}
	strSQL += _T(")");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}

	hr = FillObjIDParamDB();
	if(FAILED(hr))
	{
		return hr;
	}
	hr = FillParamDB();
	if(FAILED(hr))
	{
		return hr;
	}


	hr = m_pDb->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}	
	return S_OK;	
}

HRESULT CYDLinkRef::AccessSave()
{
	HRESULT hr = E_FAIL;

	CString strSQL = _T(" SELECT MAX(OBJID) As MAXID1 From ");
	strSQL += m_strDBName;
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
	_variant_t valID;
	hr = m_pDb->GetField(_variant_t("MAXID1"),valID);
	if(FAILED(hr))
	{
		return hr;
	}
	m_uObjID = (UINT)CDataHandler::VariantToLong(valID) + 1;
	valID.vt = VT_I4;
	valID.lVal = m_uObjID;
	strSQL = _T("Select * From ");
	strSQL += m_strDBName;
	_RecordsetPtr pRecordset = NULL;
	pRecordset.CreateInstance("ADODB.Recordset");
	CComVariant valConnection;
	valConnection.vt = VT_DISPATCH;
	_ConnectionPtr pConnection = m_pDb->GetConnection();
	hr = pConnection->QueryInterface(&valConnection.pdispVal);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pRecordset->Open(CComVariant(strSQL),valConnection,
		adOpenStatic,adLockOptimistic,adCmdText);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pRecordset->AddNew();
	if(FAILED(hr))
	{
		return hr;
	}
	for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
	{
		CString strPropName = (*itrProp)->m_strPropName;
		_variant_t valProp;
		hr = GetPropVal(_bstr_t(strPropName),&valProp);
		if(FAILED(hr))
		{
			return hr;
		}
		pRecordset->PutCollect(CComVariant(strPropName),valProp);
	}
	UINT uIDA = 0;
	hr = m_pObjRefA->GetID(&uIDA);
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t vtIDAParam((long)uIDA);
	pRecordset->PutCollect((L"ID_A"),vtIDAParam);

	UINT uIDB = 0;
	hr = m_pObjRefB->GetID(&uIDB);
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t vtIDBParam((long)uIDB);
	pRecordset->PutCollect((L"ID_B"),vtIDBParam);
	pRecordset->PutCollect(L"ObjID",&valID);
	hr = pRecordset->Update();
	if(FAILED(hr))
	{
		return hr;
	}


	return S_OK;	
}

HRESULT CYDLinkRef::FillObjIDParamDB()
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pObjRefA != NULL);
	ASSERT(m_pObjRefB != NULL);
	UINT uIDA = 0;
	hr = m_pObjRefA->GetID(&uIDA);
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t vtIDAParam((long)uIDA);
	hr = m_pDb->AddParameter(L"ID_A", adUnsignedInt, adParamInput, 
		sizeof(UINT), &vtIDAParam);
	if(FAILED(hr))
	{
		return hr;
	}

	UINT uIDB = 0;
	hr = m_pObjRefB->GetID(&uIDB);
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t vtIDBParam((long)uIDB);
	hr = m_pDb->AddParameter(L"ID_B", adUnsignedInt, adParamInput, 
		sizeof(UINT), &vtIDBParam);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYDLinkRef::PutObjRef(CYDObjectRef* _pObjA,CYDObjectRef* _pObjB)
{
	HRESULT hr = E_FAIL;
	m_pObjRefA = _pObjA;
	m_pObjRefB = _pObjB;
	return S_OK;
}

HRESULT CYDLinkRef::GetObjRef(CYDObjectRef* &_pObjA,CYDObjectRef* &_pObjB)
{
	_pObjA = m_pObjRefA;
	_pObjB = m_pObjRefB;
	return S_OK;
}