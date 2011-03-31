#pragma once
#include "../Include/YdConfigCommon.h"
#include "DBConfigConnetParam.h"
#include "../Include/YdVaultConst.h"

class BASE_DLL_API CDBConfig
{
public:
	CDBConfig(void);
	~CDBConfig(void);
	HRESULT Read();
	HRESULT Write();
public:
	CONFIG_DB_TYPE				m_uDBType;//数据库类型，oracle或sql
	CONFIG_DB_CONNET_MODE		m_uDBConnetMode;//连接模式 ODBC或OLEDB
	CDBConfigConnetParam		m_DBConnetParam;
private:
	HRESULT GetXml(CString &_strXml);
};
