// DatabaseEx.cpp: implementation of the CDatabaseEx class.
//
// Copyright 2003 Nathan Davies
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DatabaseEx.h"
#include <assert.h>
#include "../Base/Xml.h"
#include "../Base/DataHandler.h"
#include "../Base\LanguageTranslator.h"
#include "DBConfig.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//	Class Constructor
CDatabaseEx::CDatabaseEx()
{
	m_bCloseConn = TRUE;
	m_pConnectionPtr = NULL;
	if(m_pConnectionPtr != NULL)
	{
		m_bCloseConn = FALSE;
	}
	m_pCommandPtr = NULL;
	m_pRecordsetPtr = NULL;
	m_lComTimeout = 30;
}

CDatabaseEx::CDatabaseEx(const CDatabaseEx& db)
{
	m_bCloseConn = TRUE;
	m_pConnectionPtr = db.m_pConnectionPtr;
	if(m_pConnectionPtr != NULL)
	{
		m_bCloseConn = FALSE;
	}
	m_pCommandPtr = NULL;
	m_pRecordsetPtr = NULL;
	m_lComTimeout = 30;
}

//	Class Destructor
CDatabaseEx::~CDatabaseEx()
{
	Close();
}

//	Open a new ADO Connection
HRESULT CDatabaseEx::Open()
{
	HRESULT hr = E_FAIL;
	try
	{
		CDBConfig DBConfig;
		hr = DBConfig.Read();
		if(FAILED(hr))
		{
			return hr;
		}
		hr = Open(&DBConfig);
		if(FAILED(hr))
		{
			return hr;
		}
		return S_OK;
	}
	catch (CXmlException* e) 
	{
		e->ReportError();
		e->Delete();
		return S_FALSE;
	}
	return S_OK;
}

HRESULT CDatabaseEx::Open(BSTR _btConnStr,BSTR _usr,BSTR _pwd,long timeout)
{
	HRESULT hr = E_FAIL;
	try
	{
		CComBSTR bstrConnStr(_btConnStr);
		CComBSTR bstrUser(_usr);

		if(bstrConnStr.Length() <= 0 )
		{
			return Open();
		}
		else 
		{
			//
			if( m_pConnectionPtr == NULL )
			{
				assert(m_bCloseConn);
				hr = m_pConnectionPtr.CreateInstance(__uuidof(Connection));
				if(FAILED(hr))
				{
					return hr;
				}
				hr = m_pConnectionPtr->Open( _btConnStr,_usr,_pwd,0 );
				if(FAILED(hr))
 				{
 					return hr;
 				}
				if( hr == S_OK )
				{
					m_pConnectionPtr->CursorLocation = adUseClient;
					m_pConnectionPtr->CommandTimeout = timeout;
					m_lComTimeout = timeout;
				}
			}
		}
		return S_OK;
	}
	catch (_com_error &e)
	{
		return e.Error();
		
	}
	catch (...) {
		return E_FAIL;
	}
}

HRESULT CDatabaseEx::Open(CDBConfig* _pDBConfig)
{
	HRESULT hr = E_FAIL;
	try
	{
		ASSERT(_pDBConfig);
		if(_pDBConfig->m_uDBConnetMode == DB_CONNET_MODE_ODBC)
		{

		}
		else if(_pDBConfig->m_uDBConnetMode == DB_CONNET_MODE_OLEDB) 
		{
			CString btConnectionStr;
			// 判断数据库类型，进行不同的处理

			if(_pDBConfig->m_uDBType == DB_TYPE_SQLSERVER	)	// SQL Server
			{
				btConnectionStr = L"Provider=";
				btConnectionStr += _pDBConfig->m_DBConnetParam.m_strDBProvider;
				btConnectionStr += L";";
				btConnectionStr += L"Data Source=";
				btConnectionStr += _pDBConfig->m_DBConnetParam.m_strSeverName;
				btConnectionStr += L";";
				btConnectionStr += L"Initial Catalog=";
				btConnectionStr += _pDBConfig->m_DBConnetParam.m_strDBName;
				btConnectionStr += L";";
				m_dbtype = SQLSERVER;
			}
			else if(_pDBConfig->m_uDBType == DB_TYPE_ORACLE)	// ORACLE
			{
				btConnectionStr = L"Provider=";
				btConnectionStr += _pDBConfig->m_DBConnetParam.m_strDBProvider;
				btConnectionStr += L";";
				btConnectionStr += L"Data Source=";
				btConnectionStr += _pDBConfig->m_DBConnetParam.m_strSeverName;
				btConnectionStr += L";";
				m_dbtype = ORACLE;
			}
			else if(_pDBConfig->m_uDBType == DB_TYPE_ACCESS) //Access
			{
				CString strSql;
				CString strName;
				ASSERT(!_pDBConfig->m_DBConnetParam.m_strDBFileName.IsEmpty());
				CString strDriver = _T("MICROSOFT ACCESS DRIVER (*.mdb)");
				CString strDsn;
				CString strFile = _pDBConfig->m_DBConnetParam.m_strDBFileName;
				strDsn.Format(_T("DRIVER={%s};UID=sa;PWD=%s;DBQ=%s;"),
								strDriver,
								_pDBConfig->m_DBConnetParam.m_strPwd,
								strFile);
				btConnectionStr = strDsn;
				m_dbtype = ACCESS;
				hr = Open(CComBSTR(btConnectionStr),
					L"",
					L"",
					_pDBConfig->m_DBConnetParam.m_uConectTime);
				if(FAILED(hr))
				{
					return hr;
				}
				return S_OK;
			}
			else
			{
				ASSERT(FALSE);
			}
			hr = Open(CComBSTR(btConnectionStr),
				CComBSTR(_pDBConfig->m_DBConnetParam.m_strUser),
				CComBSTR(_pDBConfig->m_DBConnetParam.m_strPwd),
				_pDBConfig->m_DBConnetParam.m_uConectTime);
			if(FAILED(hr))
			{
				return hr;
			}
		}
		return S_OK;
	}
	catch (CXmlException* e) 
	{
		e->ReportError();
		e->Delete();
		return S_FALSE;
	}
	return S_OK;
}

HRESULT CDatabaseEx::OpenSql(BSTR _bstrServer,BSTR _bstrDB,BSTR _bstrUser,BSTR _bstrPwd,long timeout)
{
	HRESULT hr = E_FAIL;
	try
	{
		CComBSTR bstrServer = _bstrServer;
		CComBSTR bstrDB = _bstrDB;
		CComBSTR bstrUser = _bstrUser;
		CComBSTR bstrPwd = _bstrPwd;
		if(bstrServer.Length() <= 0 || 
			bstrDB.Length() <= 0 ||
			bstrUser.Length() <= 0)
		{
			return Open();
		}
		else
		{
			if( m_pConnectionPtr == NULL )
			{
				assert(m_bCloseConn);
				hr = m_pConnectionPtr.CreateInstance(__uuidof(Connection));
				if(FAILED(hr))
				{
					return hr;
				}
				 //"driver={SQLServer};Server=127.0.0.1;DATABASE=vckbase;UID=sa;PWD=139"
				CString strOpen = _T("Provider=SQLOLEDB;driver={SQLServer};Server=");
				strOpen += (const TCHAR*)bstrServer;
				strOpen += _T(";DATABASE=");
				strOpen += (const TCHAR*)bstrDB;
				strOpen += _T(";UID=");
				strOpen += (const TCHAR*)bstrUser;
				strOpen += _T(";PWD=");
				strOpen += (const TCHAR*)bstrPwd;
				hr = m_pConnectionPtr->
					Open(
					_bstr_t(strOpen),"","",adModeUnknown
					);

				if(FAILED(hr))
				{
					return hr;
				}
				if( hr == S_OK )
				{
					m_pConnectionPtr->CursorLocation = adUseClient;
					m_pConnectionPtr->CommandTimeout = timeout;
					m_lComTimeout = timeout;
				}
			}
		}
		return S_OK;
	}
	catch (_com_error &e) {
		e.ErrorMessage();
		return E_FAIL;

	}
	catch (...) {
		return E_FAIL;
	}
}


//	Close the Current ADO Connection
HRESULT CDatabaseEx::Close()
{
	HRESULT hr = E_FAIL;

	if( m_pRecordsetPtr )
	{
		if( m_pRecordsetPtr->State == adStateOpen )
			m_pRecordsetPtr->Close();

		m_pRecordsetPtr = NULL;
	}

	m_pCommandPtr = NULL;

	if( m_pConnectionPtr!=NULL && m_bCloseConn )
	{
		if( m_pConnectionPtr->State == adStateOpen )
		{
			hr = m_pConnectionPtr->Close();
			m_pConnectionPtr = NULL;
		}
		else
			hr = S_OK;
	}
	else
		hr = S_OK;
	return hr;
}

//	Close the Recordset and Initialize the CommandPtr Object
HRESULT CDatabaseEx::Initialize( const _bstr_t& btStoredProcedureName )
{
	HRESULT hr = E_FAIL;
	if( IsConnected())
	{
		m_pCommandPtr = NULL;

		if( m_pRecordsetPtr )
		{
			if( m_pRecordsetPtr->State == adStateOpen )
				m_pRecordsetPtr->Close();

			m_pRecordsetPtr = NULL;
		}

		hr = m_pCommandPtr.CreateInstance( __uuidof( Command ));
		if(FAILED(hr))
		{
			return hr;
		}
		m_pCommandPtr->ActiveConnection = m_pConnectionPtr;
		m_pCommandPtr->CommandText = btStoredProcedureName;
		m_pCommandPtr->CommandType = adCmdStoredProc;

		hr = S_OK;
	}
	return hr;
}

//	Close the Recordset and Initialize the CommandPtr Object
HRESULT CDatabaseEx::InitializeSQL( const _bstr_t& strSQL )
{
	HRESULT hr = E_FAIL;
	if( IsConnected())
	{
		m_pCommandPtr = NULL;

		if( m_pRecordsetPtr )
		{
			if( m_pRecordsetPtr->State == adStateOpen )
				m_pRecordsetPtr->Close();

			m_pRecordsetPtr = NULL;
		}

		hr = m_pCommandPtr.CreateInstance( __uuidof( Command ));
		if(FAILED(hr))
		{
			return hr;
		}
		try
		{
			
			m_pCommandPtr->ActiveConnection = m_pConnectionPtr;
			m_pCommandPtr->CommandText = strSQL;
			m_pCommandPtr->CommandType = adCmdText;
		}
		catch(_com_error& e)
		{
			hr = e.Error();
			if((hr == 0x80004005) || (hr == 0x800A0E7D))
			{
				return hr;
			}
			throw e;
		}
		hr = S_OK;
	}
	return hr;
}

//	Check for Connection Status
BOOL CDatabaseEx::IsConnected()
{
	
	return ( m_pConnectionPtr!=NULL );
}

//	Execute the Stored Procedure using the CommandPtr Object
HRESULT CDatabaseEx::Execute()
{
	HRESULT hr = E_FAIL;
	try
	{
		if( IsConnected() && IsInitialized())
		{
			m_pCommandPtr->ActiveConnection = m_pConnectionPtr;
			m_pRecordsetPtr = m_pCommandPtr->Execute( NULL, NULL, adCmdStoredProc );
			
			hr=S_OK;
		}
	}
	catch(_com_error& e)
	{
		throw e;
	}
	return hr;
}

HRESULT CDatabaseEx::ExecuteSQL()
{
	HRESULT hr = E_FAIL;
	try
	{
		if( IsConnected() && IsInitialized())
		{
			m_pCommandPtr->CommandTimeout = m_lComTimeout;
			m_pRecordsetPtr = m_pCommandPtr->Execute( NULL, NULL, adCmdText);
			hr=S_OK;
		}
	}
	catch(_com_error& e)
	{
		hr = e.Error();
		if(hr == 0x80004005)
		{
			return hr;
		}
		CString strErr = e.Description();
		CLanguageTranslator LanguageTranslator;
		LanguageTranslator.InsertError(hr,strErr);
			//://*:;*/
		;
		throw e;
	}
	return hr;
}

//	Create a Parameter and Add it to the CommandPtr Object (Which will be used to Execute the Stored Procedure)
HRESULT CDatabaseEx::AddParameter( const _bstr_t& btParameterName, ADODB::DataTypeEnum enDataType, 
				ADODB::ParameterDirectionEnum enParameterDirection, long lSize, const _variant_t& vtValue )
{
	HRESULT hr = E_FAIL;
	if( IsConnected() && IsInitialized())
	{
		_ParameterPtr pParameterPtr = m_pCommandPtr->CreateParameter( btParameterName, enDataType, enParameterDirection, lSize, vtValue );
		if(NULL==pParameterPtr)
			return hr;
		hr = m_pCommandPtr->Parameters->Append( pParameterPtr );
	}
	return hr;
}

//	Add Parameter Heler Function for Long Type and Input Direction
HRESULT CDatabaseEx::AddParameterInputLong( const _bstr_t& btParameterName, long lValue )
{
	return AddParameter( btParameterName, adInteger, adParamInput, sizeof( long ), _variant_t( lValue ));
}

//	Add Parameter Helper Function for Text Type and Input Direction
HRESULT CDatabaseEx::AddParameterInputText( const _bstr_t& btParameterName, const _bstr_t& btValue )
{
	return AddParameter( btParameterName, adVarChar, adParamInput, btValue.length(), _variant_t( btValue ));
}

//	Add Parameter Helper Function for Long Type and Input/Output Direction
HRESULT CDatabaseEx::AddParameterInputOutputLong( const _bstr_t& btParameterName, long lValue )
{
	return AddParameter( btParameterName, adInteger, adParamInputOutput, sizeof( long ), _variant_t( lValue ));
}

//	Add Parameter Helper Function for Text Type and Input/Output Direction
HRESULT CDatabaseEx::AddParameterInputOutputText( const _bstr_t& btParameterName, const _bstr_t& btValue, DWORD dwMaxTextSize )
{
	return AddParameter( btParameterName, adVarChar, adParamInputOutput, dwMaxTextSize, _variant_t( btValue ));
}

//	Add Parameter Helper Function for Long Type and Output Direction
HRESULT CDatabaseEx::AddParameterOutputLong( const _bstr_t& btParameterName )
{
	_variant_t vtNull;

	return AddParameter( btParameterName, adInteger, adParamOutput, 0, vtNull );
}

//	Add Parameter Helper Function for Text Type and Output Direction
HRESULT CDatabaseEx::AddParameterOutputText( const _bstr_t& btParameterName, DWORD dwMaxTextSize )
{
	_variant_t vtNull;

	return AddParameter( btParameterName, adVarChar, adParamOutput, dwMaxTextSize, vtNull );
}

//	Add Parameter Helper Function for Return Value
HRESULT CDatabaseEx::AddParameterReturnValue()
{
	_variant_t vtNull;

	return AddParameter( "RETURN_VALUE", adInteger, adParamReturnValue, 0, vtNull );
}

HRESULT CDatabaseEx::AddNew(const _bstr_t& tablename)
{
	HRESULT hr =  E_FAIL;
	if(IsConnected())
	{
		if( m_pRecordsetPtr )
		{
			if( m_pRecordsetPtr->State == adStateOpen )
				m_pRecordsetPtr->Close();
			m_pRecordsetPtr = NULL;
		}	
		hr = m_pRecordsetPtr.CreateInstance(__uuidof(Recordset));
		if(FAILED(hr))
			return hr;
		m_pRecordsetPtr->CursorType = adOpenKeyset;
		m_pRecordsetPtr->LockType = adLockOptimistic;
		hr = m_pRecordsetPtr->Open(tablename,
				_variant_t((IDispatch*)m_pConnectionPtr,true),
				adOpenKeyset,adLockOptimistic,adCmdTable);
		if(FAILED(hr))
			return hr;
		if(m_pRecordsetPtr->Supports(adAddNew))
			return m_pRecordsetPtr->AddNew();
		else
			return E_FAIL;
	}
	return hr;
}

HRESULT CDatabaseEx::SetField(const _bstr_t& btFieldName,const _variant_t& value)
{
	if( IsConnected() )
	{
		if(m_pRecordsetPtr)
		{
			FieldPtr pField = m_pRecordsetPtr->Fields->GetItem(btFieldName);
			if(pField!=NULL)
			{
				pField->PutValue(value);
				return S_OK;
			}
		}
	}
	return E_FAIL;
}

HRESULT CDatabaseEx::SetFieldBlob(const _bstr_t& btFieldName,char *blob,unsigned long ulSize)
{
	HRESULT hr = E_FAIL;
	if(NULL == blob || ulSize <=0 ) return hr;
	if(IsConnected() && m_pRecordsetPtr!=NULL)
	{
		SAFEARRAY *psa; 
		SAFEARRAYBOUND rgsabound[1];
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = ulSize;
		psa = SafeArrayCreate(VT_UI1, 1, rgsabound);
		if(psa == NULL)
		{
			return ResultFromScode(E_OUTOFMEMORY);
		}
		for (long i = 0; i < (long)ulSize; i++)
			SafeArrayPutElement (psa, &i, blob++);
		_variant_t varBLOB;
		varBLOB.vt = VT_ARRAY | VT_UI1;
		varBLOB.parray = psa;
		FieldsPtr fields = m_pRecordsetPtr->GetFields();
		assert(fields!=NULL);
		FieldPtr field = fields->GetItem(btFieldName);
		assert(field!=NULL);
		hr = field->AppendChunk(varBLOB);
	}
	return hr;
}

HRESULT CDatabaseEx::Update()
{
	if(IsConnected())
	{
		if(m_pRecordsetPtr!=NULL)
		{
			HRESULT hr = m_pRecordsetPtr->Update();
			if(FAILED(hr))
				return hr;
			return m_pRecordsetPtr->Close();
		}
	}
	return E_FAIL;
}

HRESULT CDatabaseEx::GetFieldBlob(const _bstr_t& btFieldName,char *&blob,long ulSize,long& ulActualSize)
{
	HRESULT hr = E_FAIL;
	ulActualSize = 0;
	if( IsConnected() && m_pRecordsetPtr!=NULL)
	{

		ulActualSize = m_pRecordsetPtr->GetFields()->GetItem(btFieldName)->ActualSize;
		if (ulActualSize == 0)
		{
			return S_OK;
		}
		_variant_t varBLOB;
		FieldPtr pField = m_pRecordsetPtr->Fields->GetItem( btFieldName );
		if(NULL == pField) return hr;
		if (ulSize != 0)
		{
			varBLOB = pField->GetChunk(ulSize);
		}
		else
		{
			varBLOB = pField->GetChunk(ulActualSize);
			blob = new char[ulActualSize + 1];
		}
		if(varBLOB.vt == (VT_ARRAY | VT_UI1))
		{
			long lBound=-1,UBound=-1;
			hr = SafeArrayGetLBound(varBLOB.parray,1,&lBound);
			if(FAILED(hr))
				return hr;
			hr = SafeArrayGetUBound(varBLOB.parray,1,&UBound);
			if(FAILED(hr))
				return hr;
			ulActualSize = UBound-lBound+1;
			BYTE *pBuf = NULL;
			hr = SafeArrayAccessData(varBLOB.parray,(void **)&pBuf);
			if(FAILED(hr))
				return hr;
			memcpy(blob,pBuf,ulActualSize);
			hr = SafeArrayUnaccessData(varBLOB.parray);
		}
		else if(varBLOB.vt == VT_BSTR)
		{
			_bstr_t t(varBLOB.bstrVal);
			char* pTemp = (char*)t;
			ulActualSize = strlen(pTemp);
			memcpy(blob,pTemp,ulActualSize);
		}
		else assert(FALSE);
	}
	return hr;
}

//	Retrieve a Value from the Recordset (which was created during Stored Procedure Execution)
HRESULT CDatabaseEx::GetField( const _variant_t& vtFieldName, _variant_t& vtValue )
{
	HRESULT hr = E_FAIL;
	if( IsConnected() && IsInitialized())
	{
		FieldPtr pField = m_pRecordsetPtr->Fields->GetItem( vtFieldName );
		if(pField!=NULL)
		{
			vtValue = pField->Value;
			hr = S_OK;
		}
	}
	return hr;
}

//	Get Field Helper Function for Long Type
HRESULT CDatabaseEx::GetFieldLong( const _bstr_t& btFieldName, long* plValue )
{
	_variant_t vtValue;

	HRESULT hr = GetField( btFieldName, vtValue );

	if( hr == S_OK )
		*plValue = ( long )vtValue;

	return hr;
}

//	Get Field Helper Function for Text Type
HRESULT CDatabaseEx::GetFieldText( const _bstr_t& btFieldName, _bstr_t& szText, DWORD dwMaxTextSize )
{
	_variant_t vtValue;

	HRESULT hr = GetField( btFieldName, vtValue );

	if( hr == S_OK )
	{
		_bstr_t btValue = ( _bstr_t )vtValue;

		if( dwMaxTextSize < btValue.length())
			hr = S_FALSE;
		else
			szText = btValue;
	}

	return hr;
}

//	Retrieve a Parameter (which was previously set up as either an Output or InputOutput Direction and is set during Stored Procedure Execution)
HRESULT CDatabaseEx::GetParameter( const _variant_t& vtParameterName, _variant_t& vtValue )
{
	HRESULT hr = E_FAIL;
	if( IsConnected() && IsInitialized())
	{
		FieldPtr pField = m_pCommandPtr->Parameters->GetItem( vtParameterName );
		if(pField!=NULL)
		{
			vtValue = pField->Value;
			hr = S_OK;
		}
	}
	return hr;
}

//	Retrieve Parameter Helper Function for Long Type
HRESULT CDatabaseEx::GetParameterLong( const _bstr_t& btParameterName, long* plValue )
{
	_variant_t vtValue;
	HRESULT hr = GetParameter( btParameterName, vtValue );

	if( hr == S_OK )
		*plValue = ( long )vtValue;

	return hr;
}

//	Retrieve Parameter Helper Function for Return Value
HRESULT CDatabaseEx::GetParameterReturnValue( long* plReturnValue )
{
	return GetParameterLong( "RETURN_VALUE", plReturnValue );
}

//	Retrieve Parameter Helper Function for Text Type
HRESULT CDatabaseEx::GetParameterText( const _bstr_t& btParameterName, _bstr_t& szText, DWORD dwMaxTextSize )
{
	_variant_t vtValue;
	HRESULT hr = GetParameter( btParameterName, vtValue );
	if( hr == S_OK )
	{
		_bstr_t btValue = ( _bstr_t )vtValue;

		if( dwMaxTextSize < btValue.length())
			hr = S_FALSE;
		else
			szText = btValue;
	}

	return hr;
}

//	Retrieve the Record Count for the Recordset (which was created during Stored Procedure Execution)
HRESULT CDatabaseEx::GetRecordCount( long* lRecordCount )
{
	if(IsConnected() && m_pRecordsetPtr!=NULL )
	{
		*lRecordCount = m_pRecordsetPtr->RecordCount;
		return S_OK;
	}
	return E_FAIL;
}

//	Check for EOF on the Recordset (which was created during Stored Procedure Execution)
BOOL CDatabaseEx::IsEOF()
{
	BOOL hr = TRUE;
	if( IsConnected() && m_pRecordsetPtr!=NULL )
		if( m_pRecordsetPtr->State == adStateOpen && !m_pRecordsetPtr->adoEOF )
			hr = FALSE;
	return hr;
}

//	Check for Initialization Status (CommandPtr Object is valid)
BOOL CDatabaseEx::IsInitialized()
{
	return ( m_pCommandPtr!=NULL );
}

//	Move to the Next Record in the Recordset (which was created during Stored Procedure Execution)
HRESULT CDatabaseEx::MoveNext()
{
	HRESULT hr = E_FAIL;
	if( !IsEOF())
	{
		hr = m_pRecordsetPtr->MoveNext();
	}
	return hr;
}

HRESULT CDatabaseEx::BeginTrans()
{
	if( IsConnected() )
	{
		return m_pConnectionPtr->BeginTrans();
	}
	return E_FAIL;
}

HRESULT CDatabaseEx::CommitTrans()
{
	HRESULT hr = S_OK;
	if( IsConnected() )
	{
		hr = m_pConnectionPtr->CommitTrans();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return hr;
}

HRESULT CDatabaseEx::RollbackTrans()
{
	if(IsConnected())
	{
		return m_pConnectionPtr->RollbackTrans();
	}
	return E_FAIL;
}