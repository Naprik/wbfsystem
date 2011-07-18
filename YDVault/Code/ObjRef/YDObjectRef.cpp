#include "StdAfx.h"
#include "YDObjectRef.h"
#include "../DBBase/DatabaseEx.h"
#include "../Base/AutoClean.h"
#include "../Base/DataHandler.h"
#include "../Base/FilePathHelper.h"
#include "../Base\Xml.h"
#include "../Include\YDVaultError.h"

CYDObjectRef::CYDObjectRef(CDatabaseEx* pDB)
{
	m_pDb = pDB;
	m_uObjID = ID_EMPTY;
}

CYDObjectRef::~CYDObjectRef(void)
{
	CListAutoClean<CYDPropDef> clr(m_lstPropDef);
}

//如果属性值var为VT_NULL,根据PropDef给定的缺省值来
//强制赋值,如果没有缺省值,则本函数自动赋值.
void CYDObjectRef::UseFieldDefault(CYDPropDef* pPropDef,VARIANT& var,ADODB::DataTypeEnum& type,int& size)
{
	if(NULL == pPropDef) return;
	if(var.vt == VT_NULL||var.vt == VT_EMPTY)
	{
		switch(pPropDef->m_uDataType) {
		case VT_BOOL:    //bool
			type = adBoolean;
			size = sizeof(VARIANT_BOOL);
			var.vt = VT_BOOL;
			var.boolVal = FALSE;
			if(!pPropDef->m_defaultVal.IsEmpty())
			{
				if( pPropDef->m_defaultVal.CompareNoCase(_T("是")) ==0 ||
					pPropDef->m_defaultVal.CompareNoCase(_T("对")) == 0|| 
					pPropDef->m_defaultVal.CompareNoCase(_T("真")) == 0)
				{
					var.boolVal = VARIANT_TRUE;
				}
			}
			break;
		case VT_I2:    //short
			type = adSmallInt;
			size = sizeof(short);
			var.vt = VT_I2;
			var.iVal = 0;
			if(!pPropDef->m_defaultVal.IsEmpty())
			{
				TCHAR* stop = NULL;
				var.iVal = (short)_tcstol(pPropDef->m_defaultVal,&stop,10);
			}
			break;
		case VT_I4:   //int,long,DWORD,UINT
			type = adInteger;
			size = sizeof(long);
			var.vt = VT_I4;
			var.lVal = 0;
			if(!pPropDef->m_defaultVal.IsEmpty())
			{
				TCHAR* stop = NULL;
				var.lVal = _tcstol(pPropDef->m_defaultVal,&stop,10);
			}
			break;  
		case VT_R4:   //float
			type = adSingle;
			size = sizeof(float);
			var.vt = VT_R4;
			var.fltVal = 0.0;
			if(!pPropDef->m_defaultVal.IsEmpty())
			{
				TCHAR* stop = NULL;
				var.fltVal = (float)_tcstod(pPropDef->m_defaultVal,&stop);
			}
			break;
		case VT_R8:    //double
			type = adDouble;
			size = sizeof(double);
			var.vt = VT_R8;
			var.dblVal = 0.0;
			if(!pPropDef->m_defaultVal.IsEmpty())
			{
				TCHAR* stop = NULL;
				var.dblVal = _tcstod(pPropDef->m_defaultVal,&stop);
			}
			break;
		case VT_DATE:    //short & long DATE
			type = adDBTimeStamp;
			size = sizeof(DATE);
			var.vt = VT_DATE;
			var.date = (DATE)0.0;
			if(!pPropDef->m_defaultVal.IsEmpty())
			{
				TCHAR* stop = NULL;
				var.date = (DATE)_tcstod(pPropDef->m_defaultVal,&stop);
			}
			break;  
		case VT_BSTR:    //TCHAR*
			type = adVarChar;
			var.vt = VT_BSTR;
			if(!pPropDef->m_defaultVal.IsEmpty())
			{
				var.bstrVal = pPropDef->m_defaultVal.AllocSysString();
				size = (pPropDef->m_defaultVal.GetLength()+1)*sizeof(TCHAR);
			}
			else
			{
				var.bstrVal = SysAllocString(L"");
				size = 1*sizeof(TCHAR);
			}
			break;
		case VT_DECIMAL:
			type = adDecimal;
			size = sizeof(DECIMAL);
			var.vt = VT_DECIMAL;
			if(!pPropDef->m_defaultVal.IsEmpty())
			{
				var.decVal = (DECIMAL)_variant_t(pPropDef->m_defaultVal);
			}
			else
			{
				var.decVal = (DECIMAL)_variant_t(L"");
			}
			break;
		default:
			_ASSERT(FALSE);
			break;
		}
	}
}

HRESULT CYDObjectRef::SetID(UINT ID)
{
	m_uObjID = ID;
	return S_OK;
}

HRESULT CYDObjectRef::GetID(UINT* _id)
{
	*_id = m_uObjID;
	return S_OK;
}
HRESULT CYDObjectRef::Restore(BOOL bReload)
{
	HRESULT hr = E_FAIL;
	if(bReload)
	{
		m_mapProp.clear();
	}
	if(m_mapProp.size() > 0)
	{
		return S_OK;
	}
	CString strSQL = _T("SELECT ");
	std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin();
	strSQL += (*itrProp)->m_strPropName;
	++itrProp;
	for (; itrProp != m_lstPropDef.end(); ++itrProp)
	{
		/*if ((*itrProp)->m_uDataType == VT_BLOB)
		{
			continue;
		}*/
		strSQL += _T(", ");
		strSQL += (*itrProp)->m_strPropName;
	}
	strSQL += _T(" FROM ");
	strSQL += m_strDBName;
	strSQL += _T(" WHERE OBJID = ?");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}

 	_variant_t vtParam((long)m_uObjID);
 	hr = m_pDb->AddParameter(L"OBJID", adUnsignedInt, adParamInput, 
 							sizeof(m_uObjID), &vtParam);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = m_pDb->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}	
	if(m_pDb->IsEOF())
	{
		return S_FALSE;
	}
	for(std::list<CYDPropDef*>::const_iterator itr = m_lstPropDef.begin();
		itr != m_lstPropDef.end(); ++itr)
	{
		_variant_t val;
		CString strPropName = (*itr)->m_strPropName;
		hr = m_pDb->GetField(_variant_t(strPropName),val);
		if(FAILED(hr))
		{
			return hr; 
		}
		m_mapProp.insert(MapProp::value_type(strPropName,val));
	}

	return S_OK;
}

HRESULT CYDObjectRef::GetPropVal(BSTR _bstrPropName, VARIANT* _val)
{
	HRESULT hr = E_FAIL;
	if (m_mapProp.size() <= 0)
	{
		hr = Restore();
		if (FAILED(hr))
		{
			return hr;
		}
	}
	VariantClear(_val);
	CString strPropName = _bstrPropName;
	MapProp::iterator itr = m_mapProp.find(strPropName);
	if (itr != m_mapProp.end())
	{
		VariantCopy(_val,&(*itr).second);
	}
	
	return S_OK;
}

HRESULT CYDObjectRef::GetPropVal(BSTR _bstrPropName,CString& _strVal)
{
	HRESULT hr = E_FAIL;
	CComVariant valProp;
	hr = GetPropVal(_bstrPropName,&valProp);
	if(FAILED(hr))
	{
		return hr;
	}
	_strVal = CDataHandler::VariantToString(valProp);
	return S_OK;
}

HRESULT CYDObjectRef::GetPropVal(BSTR _bstrPropName,long* _pVal)
{
	HRESULT hr = E_FAIL;
	CComVariant valProp;
	hr = GetPropVal(_bstrPropName,&valProp);
	if(FAILED(hr))
	{
		return hr;
	}
	*_pVal = CDataHandler::VariantToLong(valProp);
	return S_OK;
}

HRESULT CYDObjectRef::SetPropVal(BSTR _bstrPropName,VARIANT* _val)
{
	HRESULT hr = E_FAIL;
	CString strPropName = _bstrPropName;
	MapProp::iterator itr = m_mapProp.find(strPropName);
	if (itr != m_mapProp.end())
	{
		VariantCopy(&(*itr).second,_val);
	}
	else
	{
		m_mapProp.insert(MapProp::value_type(strPropName,_val));
	}
	return S_OK;
}

HRESULT CYDObjectRef::GetBlobProp(BSTR _bstrPropName, BSTR _bstrPath)
{
	HRESULT hr = E_FAIL;
	CString strSQL = _T("SELECT *");

	
	strSQL += _T(" FROM ");
	strSQL += m_strDBName;
	strSQL += _T(" WHERE OBJID = ?");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}

 	_variant_t vtParam((long)m_uObjID);
 	hr = m_pDb->AddParameter(L"OBJID", adUnsignedInt, adParamInput, 
 							sizeof(m_uObjID), &vtParam);
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
		char* pBuf = NULL;
		long ulActualSize = 0;
		m_pDb->GetFieldBlob(_bstrPropName, pBuf, 0, ulActualSize);
		if (ulActualSize != 0)
		{
			CFile outFile;
			outFile.Open(CString(_bstrPath), CFile::modeWrite);
			outFile.Write(pBuf, ulActualSize);
		}

	}
	
	
	return S_OK;
}

HRESULT CYDObjectRef::GetPropDef(BSTR _bstrPropName,CYDPropDef*& _pPropDef)
{
	HRESULT hr = E_FAIL;
	_pPropDef = NULL;
	for(std::list<CYDPropDef*>::const_iterator itr = m_lstPropDef.begin();
		itr != m_lstPropDef.end();++itr)
	{
		if(CDataHandler::BSTRCompare(_bstrPropName,_bstr_t((*itr)->m_strPropName)) == 0)
		{
			_pPropDef = (*itr);
			return S_OK;
		}
		
	}
	return E_FAIL;
}

HRESULT CYDObjectRef::GetDBName(CString& _strDBName)
{
	HRESULT hr = E_FAIL;
	_strDBName = m_strDBName;
	return S_OK;
}

HRESULT CYDObjectRef::GetPropCount(UINT* _count)
{
	*_count  = m_mapProp.size();
	return S_OK;
}

HRESULT CYDObjectRef::GetProp(UINT _iIndex,BSTR* _bstrName,VARIANT* _val)
{
	ASSERT( _iIndex >= 0 && _iIndex <m_mapProp.size());
	HRESULT hr = E_FAIL;
	int i =0;
	for(MapProp::const_iterator itr = m_mapProp.begin();
		itr != m_mapProp.end();++itr,i++)
	{
		if(i == _iIndex)
		{
			*_bstrName = (*itr).first.AllocSysString();
			VariantCopy(_val,&(*itr).second);
			break;
		}
	}
	return S_OK;
}

HRESULT CYDObjectRef::Update()
{
	HRESULT hr = E_FAIL;
	VARIANT_BOOL bHasBlobObject = VARIANT_FALSE;
	hr = IsHasBlobObject(&bHasBlobObject);
	if(FAILED(hr))
	{
		return hr;
	}

	if(bHasBlobObject)
	{
		_RecordsetPtr pRecordset = NULL;
		hr = GetCurRecord(pRecordset);
		if(FAILED(hr))
		{
			return hr;
		}
		ASSERT(pRecordset);
		for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
		{
			CString strPropName = (*itrProp)->m_strPropName;
			_variant_t valProp;
			hr = GetPropVal(_bstr_t(strPropName),&valProp);
			if(FAILED(hr))
			{
				return hr;
			}
			if((*itrProp)->m_uDataType == VT_BLOB_OBJECT)
			{
				//对象Image图像，要特殊处理
				if(valProp.vt != VT_EMPTY)
				{
					pRecordset->GetFields()->GetItem(_variant_t(strPropName))->AppendChunk(valProp);
				}
			}
			else
			{
				pRecordset->PutCollect(CComVariant(strPropName),valProp);
			}

		}
		hr = pRecordset->Update();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	else
	{
		DATABASE_TYPE uDBtype = m_pDb->GetDBType();
		if (uDBtype == ORACLE)
		{
			return OracleUpdate();
		}
		else if (uDBtype == SQLSERVER)
		{
			return SQLServerUpdate();
		}
		else if(uDBtype == ACCESS)
		{
			return AccessUpdate();
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	return S_OK;
}


HRESULT CYDObjectRef::OracleUpdate()
{
	HRESULT hr = E_FAIL;
	CString strSQL = _T("Update ");
	strSQL += m_strDBName;
	strSQL += _T(" Set ");
	std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin();
	strSQL += (*itrProp)->m_strPropName;
	strSQL += _T(" = ? ");
	++itrProp;
	for (; itrProp != m_lstPropDef.end(); ++itrProp)
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
HRESULT CYDObjectRef::SQLServerUpdate()
{
	HRESULT hr = E_FAIL;
	CString strSQL = _T("Update ");
	strSQL += m_strDBName;
	strSQL += _T(" Set ");
	std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin();
	strSQL += (*itrProp)->m_strPropName;
	strSQL += _T(" = ? ");
	++itrProp;
	for (; itrProp != m_lstPropDef.end(); ++itrProp)
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
HRESULT CYDObjectRef::AccessUpdate()
{
	HRESULT hr = E_FAIL;
	CString strSQL = _T("Update ");
	strSQL += m_strDBName;
	strSQL += _T(" Set ");
	BOOL bFirst = TRUE;
	CString strLogSQL = strSQL;
	for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin();
		itrProp != m_lstPropDef.end(); ++itrProp)
	{
		if(bFirst)
		{
			bFirst = FALSE;
		}
		else
		{
#ifdef _DEBUG
			strLogSQL += _T(", ");
#endif
			strSQL += _T(", ");
		}
		strSQL += (*itrProp)->m_strPropName;
		strSQL += _T(" = ? ");
#ifdef _DEBUG
		strLogSQL += (*itrProp)->m_strPropName;
		strLogSQL += _T(" = ");
		CString strPropVal;
		GetPropVal(CComBSTR((*itrProp)->m_strPropName),strPropVal);
		//CString strProp = CDataHandler::VariantToString(valProp,)
		strLogSQL += strPropVal;
#endif
		

	}
	strSQL += _T(" WHERE OBJID = ?");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}
	//
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
	_RecordsetPtr pRecordSet = m_pDb->GetRecordset();
	ASSERT(pRecordSet);
	
	return S_OK;
}

HRESULT CYDObjectRef::OracleSaveID()
{
	HRESULT hr = E_FAIL;
	CString strGetID = L"select S_";
	strGetID += m_strDBName;
	strGetID += L".nextval as ID from dual";
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
HRESULT CYDObjectRef::SQLServerSaveID()
{
	HRESULT hr = E_FAIL;
	CString strSQL = _T(" SELECT IDENT_CURRENT('");
	strSQL += m_strDBName;
	strSQL += _T("') AS MAXID1");
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
	m_uObjID = (UINT)CDataHandler::VariantToLong(valID);
	return S_OK;
}
HRESULT CYDObjectRef::AccessSaveID()
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
	return S_OK;
}

HRESULT CYDObjectRef::OracleSave()
{
	HRESULT hr = E_FAIL;
	hr = OracleSaveID();
	if(FAILED(hr))
	{
		return hr;
	}
	VARIANT_BOOL bHasBlobObject = VARIANT_FALSE;
	hr = IsHasBlobObject(&bHasBlobObject);
	if(FAILED(hr))
	{
		return hr;
	}

	if(bHasBlobObject)
	{
		hr = AddNewRecordByBlobOject();
		if(FAILED(hr))
		{
			return hr;
		}
		return S_OK;
	}
 	CString strSQL = _T("INSERT INTO ");
 	strSQL += m_strDBName;
 	strSQL += _T(" (OBJID,  ");
 	BOOL bFirst = TRUE;
 	for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
 	{
		if ((*itrProp)->m_uDataType == VT_BLOB)
		{
			continue;
		}
 		if(bFirst)
 		{
 			bFirst = FALSE;
 		}
 		else
 		{
 			strSQL += _T(", ");
 		}
 		strSQL += (*itrProp)->m_strPropName;
 	}
 	strSQL += _T(" ) VALUES(?,  ");
 	bFirst = TRUE;
 	for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
 	{
		if ((*itrProp)->m_uDataType == VT_BLOB)
		{
			continue;
		}
 		if(bFirst)
 		{
 			bFirst = FALSE;
 		}
 		else
 		{
 			strSQL += _T(", ");
 		}
 		strSQL += _T("? ");
 	}
 
 	strSQL += _T(")");
 	
 	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
 	if(FAILED(hr))
 	{
 		return hr;
 	}
 	CComVariant var(m_uObjID);
 	hr = m_pDb->AddParameter(L"OBJID", adDecimal, adParamInput, 
 		sizeof(m_uObjID), &var);
 
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

HRESULT CYDObjectRef::SQLServerSave()
{
	HRESULT hr = E_FAIL;
	VARIANT_BOOL bHasBlobObject = VARIANT_FALSE;
	hr = IsHasBlobObject(&bHasBlobObject);
	if(FAILED(hr))
	{
		return hr;
	}

	if(bHasBlobObject)
	{
		hr = AddNewRecordByBlobOject();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	else
	{
		CString strSQL = _T("INSERT INTO ");
		strSQL += m_strDBName;
		strSQL += _T(" (  ");
		BOOL bFirst = TRUE;
		for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
		{
			if(bFirst)
			{
				bFirst = FALSE;
			}
			else
			{
				strSQL += _T(", ");
			}
			strSQL += (*itrProp)->m_strPropName;
		}
		strSQL += _T(" ) VALUES(  ");
		bFirst = TRUE;
		for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
		{
			if(bFirst)
			{
				bFirst = FALSE;
			}
			else
			{
				strSQL += _T(", ");
			}
			strSQL += _T("? ");
		}
		strSQL += _T(")");

		hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
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
	}
	
	hr = SQLServerSaveID();
	if(FAILED(hr))
	{
		return hr;
	}	

	return S_OK;
}

HRESULT CYDObjectRef::AccessSave()
{
	HRESULT hr = E_FAIL;
	hr = AccessSaveID();
	if (FAILED(hr))
	{
		return hr;
	}
	VARIANT_BOOL bHasBlobObject = VARIANT_FALSE;
	hr = IsHasBlobObject(&bHasBlobObject);
	if(FAILED(hr))
	{
		return hr;
	}

	if(bHasBlobObject)
	{
		hr = AddNewRecordByBlobOject();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	else
	{
		CString strSQL = _T("INSERT INTO ");
		strSQL += m_strDBName;
		strSQL += _T(" ( OBJID ");
		for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
		{
			strSQL += _T(", ");
			strSQL += (*itrProp)->m_strPropName;
		}
		strSQL += _T(" ) VALUES(?  ");
		for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
		{
			strSQL += _T(", ");
			strSQL += _T("? ");
		}
		strSQL += _T(")");

		hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
		if(FAILED(hr))
		{
			return hr;
		}
		hr = FillIDParamDB();
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
	}

	

	return S_OK;
}

HRESULT CYDObjectRef::GetLabel(CString* pStr)
{
	return S_OK;
}

BOOL CYDObjectRef::IsEqual(CYDObjectRef* pobj)
{
	if (pobj->m_uObjID == m_uObjID &&
		pobj->m_strDBName == m_strDBName)
	{
		return TRUE;
	}
	return FALSE;
}

HRESULT CYDObjectRef::IsFieldExist(const CString& strField, const CString& strVaule, BOOL* pbExist)
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
	strSQL += L"' ";

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

HRESULT CYDObjectRef::Save()
{
	HRESULT hr = E_FAIL;
	DATABASE_TYPE uDBtype = m_pDb->GetDBType();
	if (uDBtype == ORACLE)
	{
		return OracleSave();
	}
	else if (uDBtype == SQLSERVER)
	{
		return SQLServerSave();
	}
	else if(uDBtype == ACCESS)
	{
		return AccessSave();
	}
	else
	{
		ASSERT(FALSE);
	}
	return S_OK;
}

HRESULT CYDObjectRef::Remove()
{
	HRESULT hr = E_FAIL;
	CString strSQL = _T("DELETE From ");
	strSQL += m_strDBName;
	strSQL += _T(" where OBJID = ? ");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
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

HRESULT CYDObjectRef::FillParamDB()
{
	HRESULT hr = E_FAIL;
	for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
	{
		CString strPropName = (*itrProp)->m_strPropName;
		_variant_t valProp;
		hr = GetPropVal(_bstr_t(strPropName),&valProp);
		if(FAILED(hr))
		{
			return hr;
		}
		ADODB::DataTypeEnum DataType = adEmpty;
		int size = 0;
		CDataHandler::VarTypeToDBType(&valProp,DataType,size);
		if(DataType == adEmpty)
		{
			UseFieldDefault(*itrProp,valProp,DataType,size);
		}
		else if((*itrProp)->m_uDataType == VT_I4)
		{
			DataType = adInteger;
		}
//  		if((*itrProp)->m_uDataType == VT_BLOB)
//  		{
//  			char     *pBuf   =   valProp.pbstrVal;   
//  			VARIANT     varBLOB;   
//  			SAFEARRAY   *psa;   
//  			SAFEARRAYBOUND   rgsabound[1];   
//  			rgsabound[0].lLbound   =   0;   
//  			rgsabound[0].cElements   =   size;   
//  			psa   =   SafeArrayCreate(VT_UI1,   1,   rgsabound);                                             ///创建SAFEARRAY对象   
//  			for   (long   i   =   0;   i   <   (long)size;   i++)   
//  				SafeArrayPutElement   (psa,   &i,   pBuf++);                                                   ///将pBuf指向的二进制数据保存到SAFEARRAY对象psa中   
//  			varBLOB.vt   =   VT_ARRAY   |   VT_UI1;                                                                       ///将varBLOB的类型设置为BYTE类型的数组   
//  			varBLOB.parray   =   psa;    
//  		}
		hr = m_pDb->AddParameter(_bstr_t(strPropName), DataType, adParamInput, 
			size, &valProp);
		if(FAILED(hr))
		{
			return hr;
		}

	}
	return S_OK;
}

HRESULT CYDObjectRef::UpdateBlob()
{
	HRESULT hr = E_FAIL;
	CString strSQL = _T("SELECT *");


	strSQL += _T(" FROM ");
	strSQL += m_strDBName;
	strSQL += _T(" WHERE OBJID = ?");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if(FAILED(hr))
	{
		return hr;
	}

	_variant_t vtParam((long)m_uObjID);
	hr = m_pDb->AddParameter(L"OBJID", adUnsignedInt, adParamInput, 
		sizeof(m_uObjID), &vtParam);
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
		BOOL bNeedUpdate = FALSE;
		for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
		{
			CString strPropName = (*itrProp)->m_strPropName;
			_variant_t valProp;
			hr = GetPropVal(_bstr_t(strPropName),&valProp);
			if(FAILED(hr))
			{
				return hr;
			}
			
			if((*itrProp)->m_uDataType == VT_BLOB)
			{
				if (valProp.vt != VT_EMPTY)
				{
					CFile inputFile;
					CString strFilePath = CDataHandler::VariantToString(valProp);
					if (!CFilePathHelper::FilePathExists(strFilePath))
					{
						CString strInfo = L"文件：" + strFilePath + L"不存在！";
						AfxMessageBox(strInfo);
						return E_FAIL;
					}
					inputFile.Open(strFilePath, CFile::modeRead);

					ULONGLONG nFileLen = 0;
					nFileLen = inputFile.GetLength();
					UINT nSize = (UINT)nFileLen + 1;
					char* pBMPBuffer = new char[nSize];
					inputFile.Read(pBMPBuffer, sizeof(pBMPBuffer));
					m_pDb->SetFieldBlob(_bstr_t(strPropName), pBMPBuffer, (unsigned long)nFileLen);
					bNeedUpdate = TRUE;
				}
			}
		}
		if (bNeedUpdate)
		{
			hr = m_pDb->Update();
			if (FAILED(hr))
			{
				return hr;
			}
		}
	}
	return S_OK;
}

HRESULT CYDObjectRef::FillIDParamDB()
{
	HRESULT hr = E_FAIL;
	_variant_t vtParam((long)m_uObjID);
	hr = m_pDb->AddParameter(L"OBJID", adUnsignedInt, adParamInput, 
		sizeof(m_uObjID), &vtParam);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYDObjectRef::AddPropDef(CString strPropName,UINT _type,CString _strDefault/* = _T("")*/)
{
	CYDPropDef* pDef = new CYDPropDef(strPropName,_type,_strDefault);
	m_lstPropDef.push_back(pDef);
	return S_OK;
}
HRESULT CYDObjectRef::DelPropDef(CString strPropName)
{
	for(std::list<CYDPropDef*>::iterator itr = m_lstPropDef.begin();
		itr != m_lstPropDef.end();++itr)
	{
		if((*itr)->m_strPropName.CompareNoCase(strPropName) == 0)
		{
			m_lstPropDef.erase(itr);
			return S_OK;
		}
	}
	return S_FALSE;
}

HRESULT CYDObjectRef::GetObjectByCondition(int _iPage,
										   std::list<CPropQueryContidition*> *_lstCondition,
										   std::list<CYDObjectRef*>* _lstObjects)
{
	HRESULT hr = E_FAIL;
	DATABASE_TYPE dbType = m_pDb->GetDBType();
	if(dbType == SQLSERVER || 
		dbType == ACCESS)
	{
		hr = AccessQueryCondition(_lstCondition);
		if (FAILED(hr))
		{
			return hr;
		}
		UINT uStart = -1;
		UINT uEnd = -1;
		if (_iPage != -1)
		{
			uStart = _iPage * QUESTION_PAGE_COUNT + 1;
			uEnd = uStart + QUESTION_PAGE_COUNT -1;
		}
		hr = AccessExeCreateObject(_lstObjects,uStart,uEnd);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	else if(dbType == ORACLE)
	{
		hr = OracleQueryCondition(_iPage, _lstCondition);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = OracleExeCreateObject(_lstObjects);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CYDObjectRef::OracleExeCreateObject(std::list<CYDObjectRef*>* _lstObjects)
{
	HRESULT hr = E_FAIL;

	while(!m_pDb->IsEOF())
	{
		_variant_t valOBJID;
		hr = m_pDb->GetField(_variant_t(_T("OBJID")), valOBJID);
		if (FAILED(hr))
		{
			return hr;
		}
		OBJID uID = CDataHandler::VariantToLong(valOBJID);
		if(uID != 0)
		{
			CYDObjectRef* pObj = CreateObject();
			ASSERT(pObj);
			hr = pObj->SetID(uID);
			if (FAILED(hr))
			{
				return hr;
			}
			_lstObjects->push_back(pObj);
		}
		hr = m_pDb->MoveNext();
		if (FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}
HRESULT CYDObjectRef::OracleQueryCondition(int _iPage,
									 std::list<CPropQueryContidition*> *_lstCondition)
{
	HRESULT hr = E_FAIL;
	CString strSQL;
	hr = CreateSQLFromQueryCondition(_lstCondition, strSQL);
	if (FAILED(hr))
	{
		return hr;
	}
	ASSERT(m_pDb != NULL);
	if(_iPage != -1)
	{
		int iRowStart = _iPage * QUESTION_PAGE_COUNT;
		CString strRowStart;
		strRowStart.Format(_T("%d"),iRowStart+1);
		int iRowEnd = iRowStart + QUESTION_PAGE_COUNT;
		CString strRowEnd;
		strRowEnd.Format(_T("%d"),iRowEnd);
		CString strBeginPageSQL = _T(" SELECT * FROM ( SELECT Z.*, ROWNUM RN FROM ( SELECT * FROM ( ");
		CString strEndPageSQL = _T(" ) WHERE ROWNUM <= ");
		strEndPageSQL += strRowEnd;
		strEndPageSQL += _T(" ) Z ");
		strEndPageSQL += _T(" ) WHERE RN >= ");
		strEndPageSQL += strRowStart;
		strSQL = strBeginPageSQL + strSQL;
		strSQL = strSQL + strEndPageSQL;
	}
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if (FAILED(hr))
	{
		return hr;
	}
	for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
		itr != _lstCondition->end();++itr)
	{
		hr = (*itr)->CreateParamter(m_pDb);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	hr = m_pDb->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}


HRESULT CYDObjectRef::AccessQueryCondition(std::list<CPropQueryContidition*> *_lstCondition)
{
	HRESULT hr = E_FAIL;
	CString strSQL;
	hr = CreateSQLFromQueryCondition(_lstCondition, strSQL);
	if (FAILED(hr))
	{
		return hr;
	}
	ASSERT(m_pDb != NULL);
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if (FAILED(hr))
	{
		return hr;
	}
	CreateParamter(_lstCondition);
	hr = m_pDb->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
HRESULT CYDObjectRef::CreateParamter(std::list<CPropQueryContidition*> *_lstCondition)
{
	HRESULT hr = E_FAIL;
	for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
		itr != _lstCondition->end();++itr)
	{
		hr = (*itr)->CreateParamter(m_pDb);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CYDObjectRef::CreateSQLFromQueryCondition(std::list<CPropQueryContidition*> *_lstCondition,
										 CString &_strSQL)
{
	HRESULT hr = E_FAIL;
	_strSQL = _T("SELECT objid FROM ");
	_strSQL += m_strDBName;
	BOOL bFirst = TRUE;
	for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
		itr != _lstCondition->end(); ++itr)
	{
		CString strItrSQL;
		hr = (*itr)->CreateContidition(strItrSQL);
		if (FAILED(hr))
		{
			return hr;
		}
		if(strItrSQL.IsEmpty())
		{
			continue;
		}
		if(bFirst)
		{
			_strSQL += _T(" WHERE ");
			bFirst = FALSE;
		}
		else
		{
			_strSQL += _T(" AND ");
		}

		_strSQL += strItrSQL;
	}


	return S_OK;
}

HRESULT CYDObjectRef::AccessExeCreateObject(std::list<CYDObjectRef*>* _lstObjects,
									  int _iStart,
									  int _iEnd)
{
	HRESULT hr = E_FAIL;
	int iCount = 1;
	while(!m_pDb->IsEOF())
	{
		BOOL bMatch = FALSE;
		if(iCount >= _iStart && iCount <= _iEnd)
		{
			bMatch = TRUE;
		}
		if(_iStart == -1 && _iEnd == -1)
		{
			//是所有的查询到的对象
			bMatch = TRUE;
		}
		if(bMatch)
		{
			_variant_t valOBJID;
			hr = m_pDb->GetField(_variant_t(_T("OBJID")), valOBJID);
			if (FAILED(hr))
			{
				return hr;
			}
			OBJID uID = CDataHandler::VariantToLong(valOBJID);
			if(uID != 0)
			{
				CYDObjectRef* pObj = CreateObject();
				ASSERT(pObj);
				hr = pObj->SetID(uID);
				if (FAILED(hr))
				{
					return hr;
				}
				_lstObjects->push_back(pObj);
			}
		}
		hr = m_pDb->MoveNext();
		if (FAILED(hr))
		{
			return hr;
		}
		iCount++;
		if(iCount > _iEnd && _iEnd != -1)
		{
			break;
		}
	}
	return S_OK;
}

CYDObjectRef* CYDObjectRef::CreateObject()
{
	return new CYDObjectRef(m_pDb);
}

HRESULT CYDObjectRef::GetObjectTotalNumber(std::list<CPropQueryContidition*> *_lstCondition,
										   long* _lCount)
{
	HRESULT hr = E_FAIL;
	CString strSQL;
	hr = CreateSQLFromQueryCondition(_lstCondition,strSQL);
	if (FAILED(hr))
	{
		return hr;
	}
	strSQL = _T("Select Count(*) As IDCOUNT From ( ") + strSQL;
	strSQL += _T(" ) ");
	hr = m_pDb->InitializeSQL(_bstr_t(strSQL));
	if (FAILED(hr))
	{
		return hr;
	}
	for(std::list<CPropQueryContidition*>::const_iterator itr = _lstCondition->begin();
		itr != _lstCondition->end();++itr)
	{
		hr = (*itr)->CreateParamter(m_pDb);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	hr = m_pDb->ExecuteSQL();
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t valCount;
	hr = m_pDb->GetField(_variant_t(_T("IDCOUNT")),valCount);
	if(FAILED(hr))
	{
		return hr;
	}
	*_lCount = CDataHandler::VariantToLong(valCount);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYDObjectRef::LoadXml(CXmlNode& _node)
{
	HRESULT hr = E_FAIL;
	CComBSTR bstrNodeName = _node.GetNodeName();
	ASSERT(CDataHandler::BSTRCompare(bstrNodeName,L"Object") == 0);
	CXmlNodeList nodeList;
	_node.GetChilds(nodeList);
	for(long i = 0; i < nodeList.GetLength();i++)
	{
		CXmlNode nodeChild;
		nodeList.GetNode(i,nodeChild);
		CComBSTR bstrName = nodeChild.GetNodeName();
		CComBSTR bstrVal = nodeChild.GetNodeValue();
		if(CDataHandler::BSTRCompare(bstrName,L"ObjID") == 0)
		{
			CComVariant val = bstrVal;
			m_uObjID = CDataHandler::VariantToLong(val);
		}
		else if(CDataHandler::BSTRCompare(bstrName,L"ClassName") == 0)
		{
			m_strDBName = bstrVal;
		}
		else if(CDataHandler::BSTRCompare(bstrName,L"Properties") == 0)
		{
			CXmlNodeList nodePropList;
			nodeChild.GetChilds(nodePropList);
			for(int j = 0; j < nodePropList.GetLength();j++)
			{
				CXmlNode nodeProp;
				nodePropList.GetNode(j,nodeProp);
				hr = LoadProperty(nodeProp);
				if(FAILED(hr))
				{
					return hr;
				}
			}
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	return S_OK;
}

HRESULT CYDObjectRef::LoadProperty(CXmlNode& _node)
{
	HRESULT hr = E_FAIL;
	CComBSTR bstrName = _node.GetNodeName();
	CString strName = (const TCHAR*)bstrName;
	CComBSTR bstrVal = _node.GetNodeValue();
	CComVariant val = bstrVal;
	m_mapProp.insert(MapProp::value_type(strName,val));
	
	return S_OK;
}

HRESULT CYDObjectRef::SaveXml(CXmlWriter& _writer)
{
	HRESULT hr = E_FAIL;
	_writer.WriteStartElement(L"Object");
	_writer.WriteStartElement(L"ObjID");
	CString strObjID;
	strObjID.Format(_T("%d"),m_uObjID);
	_writer.WriteString(CComBSTR(strObjID));
	_writer.WriteEndElement();

	_writer.WriteStartElement(L"ClassName");
	_writer.WriteString(CComBSTR(m_strDBName));
	_writer.WriteEndElement();

	_writer.WriteStartElement(L"Properties");
	for(MapProp::const_iterator itr = m_mapProp.begin();
		itr != m_mapProp.end();++itr)
	{
		CString strPropName = (*itr).first;
		CString strPropVal = CDataHandler::VariantToString((*itr).second);
		_writer.WriteStartElement((CComBSTR)strPropName);
		_writer.WriteString((CComBSTR)strPropVal);
		_writer.WriteEndElement();
	}
	_writer.WriteEndElement();

	_writer.WriteEndElement();
	return S_OK;
}

HRESULT CYDObjectRef::GetCurRecord(_RecordsetPtr& _pRecord)
{
	HRESULT hr = E_FAIL;
	_RecordsetPtr  pRecord = NULL;
	hr = pRecord.CreateInstance(__uuidof(Recordset));
	if(FAILED(hr))
	{
		return hr;
	}
	CString strFilter;
	strFilter.Format(_T("OBJID = %d"),m_uObjID);
	hr = pRecord->put_Filter(CComVariant(strFilter));
	if(FAILED(hr))
	{
		return hr;
	}
	CString strSQL;
	strSQL = _T("SELECT * FROM ");
	strSQL += m_strDBName;

	hr = pRecord->Open(CComVariant(strSQL),
		_variant_t((IDispatch *)m_pDb->GetConnection(),true),
		adOpenDynamic,adLockPessimistic,adCmdText);
	if(FAILED(hr))
	{
		return hr;
	}
	if(pRecord->GetRecordCount() <= 0)
	{
		return E_HRESULT_OBJ_NOT_EXIST;
	}
	ASSERT(pRecord->GetRecordCount() == 1);
	_pRecord = pRecord;
		return S_OK;
}


HRESULT CYDObjectRef::AddNewRecordByBlobOject()
{
	HRESULT hr = E_FAIL;
	CComVariant valID;
	valID.vt = VT_I4;
	valID.lVal = m_uObjID;
	CString strSQL = _T("Select * From ");
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
		if((*itrProp)->m_uDataType == VT_BLOB_OBJECT)
		{
			//对象Image图像，要特殊处理
			if(valProp.vt != VT_EMPTY && valProp.vt != VT_NULL)
			{
				pRecordset->GetFields()->GetItem(_variant_t(strPropName))->AppendChunk(valProp);
			}
		}
		else
		{
			pRecordset->PutCollect(CComVariant(strPropName),valProp);
		}

	}
	pRecordset->PutCollect(L"ObjID",&valID);
	hr = pRecordset->Update();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CYDObjectRef::IsHasBlobObject(VARIANT_BOOL* _bHas)
{
	HRESULT hr = E_FAIL;
	*_bHas = VARIANT_FALSE;
	for (std::list<CYDPropDef*>::const_iterator itrProp =m_lstPropDef.begin(); itrProp != m_lstPropDef.end(); ++itrProp)
	{
		if((*itrProp)->m_uDataType == VT_BLOB_OBJECT)
		{
			*_bHas = VARIANT_TRUE;
			break;
		}
	}
	return S_OK;
}