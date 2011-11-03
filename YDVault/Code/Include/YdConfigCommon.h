
#pragma once

enum CONFIG_DB_TYPE
{
	DB_TYPE_ORACLE     = 0 ,//Oracle数据库
	DB_TYPE_SQLSERVER  = 1 , //SQLServer数据库
	DB_TYPE_ACCESS     = 2
};

enum CONFIG_DB_CONNET_MODE
{
	DB_CONNET_MODE_OLEDB  = 0 ,//OLEDB连接模式
	DB_CONNET_MODE_ODBC   = 1  //ODBC连接模式
};

enum CONFIG_SEVER_MODE
{
	SERVER_FTP     = 0, //ftp
	SERVER_LOCAL   = 1  //本地
};