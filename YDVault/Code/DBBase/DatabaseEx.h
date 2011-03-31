// DatabaseEx.h: interface for the CMyADO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYADO_H__E144D98C_2388_4800_BC00_F7E963816A73__INCLUDED_)
#define AFX_MYADO_H__E144D98C_2388_4800_BC00_F7E963816A73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)


#define TRANSACTION_TIMEOUT 60
#pragma warning(disable:4786)
#include <list>





class CXmlNode;
class CDBConfig;
enum DATABASE_TYPE
{
	SQLSERVER,
	ORACLE,
	ACCESS
};
class BASE_DLL_API CDatabaseEx  
{
public:
	CDatabaseEx();
	CDatabaseEx(const CDatabaseEx& db);
	virtual ~CDatabaseEx();

public:
	//下面通过配置文件打开
	HRESULT Open();

	HRESULT Open(BSTR btConnStr,BSTR usr,BSTR pwd,long timeout=TRANSACTION_TIMEOUT);
	HRESULT Open(CDBConfig* _pDBConfig);
	//m_pConnection->	Open(
		//"driver={SQLServer};Server=127.0.0.1;DATABASE=vckbase;UID=sa;PWD=139","","",adModeUnknown
		//);
	//下面是通过指定打开SQL
	HRESULT OpenSql(BSTR _bstrServer,BSTR _bstrDB,BSTR _bstrUser,BSTR _bstrPwd,long timeout=TRANSACTION_TIMEOUT);
public:
	HRESULT Close();

	DATABASE_TYPE GetDBType(){return m_dbtype;}

	HRESULT AddParameter( const _bstr_t& btParameterName, ADODB::DataTypeEnum enDataType, 
		            ADODB::ParameterDirectionEnum enParameterDirection, long lSize, const _variant_t& vtValue );
	HRESULT AddParameterReturnValue();
	HRESULT AddParameterInputLong( const _bstr_t& btParameterName, long lValue );
	HRESULT AddParameterInputText( const _bstr_t& btParameterName, const _bstr_t& btValue );

	HRESULT AddParameterInputOutputLong( const _bstr_t& btParameterName, long lValue );
	HRESULT AddParameterInputOutputText( const _bstr_t& btParameterName, const _bstr_t& btValue, DWORD dwMaxTextSize );

	HRESULT AddParameterOutputLong( const _bstr_t& btParameterName );
	HRESULT AddParameterOutputText( const _bstr_t& btParameterName, DWORD dwMaxTextSize );

	//
	HRESULT AddNew(const _bstr_t& tablename);
	HRESULT SetField(const _bstr_t& btFieldName,const _variant_t& value);
	HRESULT SetFieldBlob(const _bstr_t& btFieldName,char *blob,unsigned long ulSize);
	HRESULT Update();
	HRESULT GetFieldBlob(const _bstr_t& btFieldName,char *&blob,long ulSize,long& ulActualSize);

	HRESULT Execute();
	HRESULT ExecuteSQL();

	HRESULT GetField( const _variant_t& vtFieldName, _variant_t& vtValue );
	HRESULT GetFieldLong( const _bstr_t& btFieldName, long* plValue );
	HRESULT GetFieldText( const _bstr_t& btFieldName, _bstr_t& szText, DWORD dwMaxTextSize );

	HRESULT GetParameterReturnValue( long* plReturnValue );
	HRESULT GetParameter( const _variant_t& vtParameterName, _variant_t& vtValue );
	HRESULT GetParameterLong( const _bstr_t& btParameterName, long* plValue );
	HRESULT GetParameterText( const _bstr_t& btParameterName, _bstr_t& szText, DWORD dwMaxTextSize );

	HRESULT Initialize( const _bstr_t& btStoredProcedureName );
	HRESULT InitializeSQL( const _bstr_t& strSQL );

	BOOL IsEOF();
	HRESULT MoveNext();

	HRESULT BeginTrans();
	HRESULT CommitTrans();
	HRESULT RollbackTrans();

	_RecordsetPtr GetRecordset()
	{
		if(IsConnected())
			return m_pRecordsetPtr;
		return NULL;
	}
	_Connection* GetConnection(){return m_pConnectionPtr;}
	void SetConnection(_Connection* p)
	{
		m_bCloseConn = TRUE;
		m_pConnectionPtr = p;
		if(m_pConnectionPtr != NULL)
		{
			m_bCloseConn = FALSE;
		}
		m_pCommandPtr = NULL;
		m_pRecordsetPtr = NULL;
	}
	HRESULT GetRecordCount( long* lRecordCount );
	
protected:
	BOOL IsConnected();
	BOOL IsInitialized();

	_ConnectionPtr  m_pConnectionPtr;
	_CommandPtr     m_pCommandPtr;
	_RecordsetPtr   m_pRecordsetPtr;
	BOOL            m_bCloseConn;
	long			m_lComTimeout;
	DATABASE_TYPE	m_dbtype;
private:
};

#endif // !defined(AFX_MYADO_H__E144D98C_2388_4800_BC00_F7E963816A73__INCLUDED_)
