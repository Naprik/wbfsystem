
#pragma once

enum CONFIG_DB_TYPE
{
	DB_TYPE_ORACLE     = 0 ,//Oracle���ݿ�
	DB_TYPE_SQLSERVER  = 1 , //SQLServer���ݿ�
	DB_TYPE_ACCESS     = 2
};

enum CONFIG_DB_CONNET_MODE
{
	DB_CONNET_MODE_OLEDB  = 0 ,//OLEDB����ģʽ
	DB_CONNET_MODE_ODBC   = 1  //ODBC����ģʽ
};

enum CONFIG_SEVER_MODE
{
	SERVER_FTP     = 0, //ftp
	SERVER_LOCAL   = 1  //����
};