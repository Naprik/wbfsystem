#include "StdAfx.h"
#include "DBConfigConnetParam.h"
#include "../Base/Xml.h"
#include "../Base/DataHandler.h"
#include "../Base/Crypt.h"
#include "../Include/YDVaultComm.h"

CDBConfigConnetParam::CDBConfigConnetParam(void)
{
	m_strSeverName = _T("");
	m_strDBName = _T("");
	m_strUser  = _T("");;
	m_strPwd  = _T("");;
	m_strDBProvider  = _T("");;
	m_uConectTime = 60;
	m_strDBLongConnection = _T("");
	m_strDBFileName = _T("");
}

CDBConfigConnetParam::~CDBConfigConnetParam(void)
{
}

HRESULT CDBConfigConnetParam::Read(CXmlNode &_node)
{
	HRESULT hr = E_FAIL;
	for(int i = 0; i< _node.GetChildCount();i++)
	{
		CXmlNode nodeChild;
		_node.GetChildeNode(i,nodeChild);
		CComBSTR bstrNamVal;
		nodeChild.ReadAttributeByNoCase(L"name",&bstrNamVal);
		if(CDataHandler::BSTRCompare(bstrNamVal,L"database_server") == 0)
		{
			CComBSTR bstrServer;
			nodeChild.ReadAttributeByNoCase(L"value",&bstrServer);
			m_strSeverName = bstrServer;
		}
		else if(CDataHandler::BSTRCompare(bstrNamVal,L"database_name") == 0)
		{
			CComBSTR bstrDBName;
			nodeChild.ReadAttributeByNoCase(L"value",&bstrDBName);
			m_strDBName = bstrDBName;
		}
		else if(CDataHandler::BSTRCompare(bstrNamVal,L"dbfile_name") == 0)
		{
			CComBSTR bstrDBFileName;
			nodeChild.ReadAttributeByNoCase(L"value",&bstrDBFileName);
			m_strDBFileName = bstrDBFileName;
		}
		else if(CDataHandler::BSTRCompare(bstrNamVal,L"database_user") == 0)
		{
			CComBSTR bstrUser;
			nodeChild.ReadAttributeByNoCase(L"value",&bstrUser);
			m_strUser = bstrUser;
			CComBSTR bstrEncrypt;
			nodeChild.ReadAttributeByNoCase(L"encrypt",&bstrEncrypt);
			if(CDataHandler::BSTRCompare(bstrEncrypt,L"True") == 0)
			{
				//用户名是加密的，要解密
				CCrypt Crypt;
				m_strUser = Crypt.Decrypt(m_strUser,CRYPT_KEY);
			}
			
		}
		else if(CDataHandler::BSTRCompare(bstrNamVal,L"database_user_pwd") == 0)
		{
			CComBSTR bstrPwd;
			nodeChild.ReadAttributeByNoCase(L"value",&bstrPwd);
			m_strPwd = bstrPwd;
			CComBSTR bstrEncrypt;
			nodeChild.ReadAttributeByNoCase(L"encrypt",&bstrEncrypt);
			if(CDataHandler::BSTRCompare(bstrEncrypt,L"True") == 0)
			{
				//密码是加密的，要解密
				CCrypt Crypt;
				m_strPwd = Crypt.Decrypt(m_strPwd,CRYPT_KEY);
			}
		}
		else if(CDataHandler::BSTRCompare(bstrNamVal,L"database_connection_timeout") == 0)
		{
			CComBSTR bstrTimOut;
			nodeChild.ReadAttributeByNoCase(L"value",&bstrTimOut);
			m_uConectTime = CDataHandler::VariantToLong(CComVariant(bstrTimOut));
		}
		else if(CDataHandler::BSTRCompare(bstrNamVal,L"database_provider") == 0)
		{
			CComBSTR bstrProvider;
			nodeChild.ReadAttributeByNoCase(L"value", &bstrProvider);
			m_strDBProvider = bstrProvider;
		}
		else if (CDataHandler::BSTRCompare(bstrNamVal,L"database_long_connection") == 0)
		{
			//保留
			CComBSTR bstrDBLongConnection;
			nodeChild.ReadAttributeByNoCase(L"value", &bstrDBLongConnection);
			m_strDBLongConnection = bstrDBLongConnection;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	return S_OK;
}

HRESULT CDBConfigConnetParam::Write(CXmlWriter &_writer)
{
	HRESULT hr = E_FAIL;
	{
		_writer.WriteStartElement(L"PARAM");
		_writer.WriteAttributeString(L"Name",L"database_server");
		_writer.WriteAttributeString(L"value",CComBSTR(m_strSeverName));
		_writer.WriteEndElement();
	}

	{
		_writer.WriteStartElement(L"PARAM");
		_writer.WriteAttributeString(L"Name",L"database_name");
		_writer.WriteAttributeString(L"value",CComBSTR(m_strDBName));
		_writer.WriteEndElement();
	}

	{
		_writer.WriteStartElement(L"PARAM");
		_writer.WriteAttributeString(L"Name",L"dbfile_name");
		_writer.WriteAttributeString(L"value",CComBSTR(m_strDBFileName));
		_writer.WriteEndElement();
	}

	{
		_writer.WriteStartElement(L"PARAM");
		_writer.WriteAttributeString(L"Name",L"database_user");
		CCrypt Crypt;
		CString strUser = Crypt.Encrypt(m_strUser,CRYPT_KEY); 
		_writer.WriteAttributeString(L"value",CComBSTR(strUser));
		_writer.WriteAttributeString(L"encrypt",CComBSTR(_T("TRUE")));
		_writer.WriteEndElement();
	}

	{
		_writer.WriteStartElement(L"PARAM");
		_writer.WriteAttributeString(L"Name",L"database_user_pwd");
		CCrypt Crypt;
		CString strPwd = Crypt.Encrypt(m_strPwd,CRYPT_KEY);
		_writer.WriteAttributeString(L"value",CComBSTR(strPwd));
		_writer.WriteAttributeString(L"encrypt",CComBSTR(_T("TRUE")));
		_writer.WriteEndElement();
	}

	{
		_writer.WriteStartElement(L"PARAM");
		_writer.WriteAttributeString(L"Name",L"database_provider");
		_writer.WriteAttributeString(L"value",CComBSTR(m_strDBProvider));
		_writer.WriteEndElement();
	}

	{
		_writer.WriteStartElement(L"PARAM");
		_writer.WriteAttributeString(L"Name",L"database_connection_timeout");
		CString strConnectTime;
		strConnectTime.Format(_T("%d"),m_uConectTime);
		_writer.WriteAttributeString(L"value",CComBSTR(strConnectTime));
		_writer.WriteEndElement();
	}

	{
		_writer.WriteStartElement(L"PARAM");
		_writer.WriteAttributeString(L"Name",L"database_long_connection");
		_writer.WriteAttributeString(L"value",CComBSTR(m_strDBLongConnection));
		_writer.WriteEndElement();
	}


	return S_OK;
}