#include "StdAfx.h"
#include "FtpServerParam.h"
#include "../Base/Xml.h"
#include "../Base/DataHandler.h"
#include "../Base/Crypt.h"
#include "../Include/YDVaultComm.h"

CFtpServerParam::CFtpServerParam(void)
{
	m_strFtpAddr     = _T("");
	m_uPort          = 21;
	m_strUser		 = _T("");
	m_strPwd         = _T("");
}

CFtpServerParam::~CFtpServerParam(void)
{
}

HRESULT CFtpServerParam::Read(CXmlNode &_node)
{
	HRESULT hr = E_FAIL;
	for(int i = 0; i< _node.GetChildCount();i++)
	{
		CXmlNode nodeChild;
		_node.GetChildeNode(i,nodeChild);
		CComBSTR bstrNamVal;
		nodeChild.ReadAttributeByNoCase(L"name",&bstrNamVal);
		if(CDataHandler::BSTRCompare(bstrNamVal,L"ServerAddr") == 0)
		{
			CComBSTR bstrServerAddr;
			nodeChild.ReadAttributeByNoCase(L"value",&bstrServerAddr);
			m_strFtpAddr = bstrServerAddr;
		}
		else if(CDataHandler::BSTRCompare(bstrNamVal,L"ServerPort") == 0)
		{
			CComBSTR bstrServerPort;
			nodeChild.ReadAttributeByNoCase(L"value",&bstrServerPort);
			m_uPort = CDataHandler::VariantToLong(CComVariant(bstrServerPort));

		}
		else if(CDataHandler::BSTRCompare(bstrNamVal,L"ServerUser") == 0)
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
		else if(CDataHandler::BSTRCompare(bstrNamVal,L"ServerPwd") == 0)
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
		else
		{
			ASSERT(FALSE);
		}
	}
	return S_OK;
}
HRESULT CFtpServerParam::Write(CXmlWriter &_writer)
{
	HRESULT hr = E_FAIL;
	{
		_writer.WriteStartElement(L"PARAM");
		_writer.WriteAttributeString(L"Name",L"ServerAddr");
		_writer.WriteAttributeString(L"value",CComBSTR(m_strFtpAddr));
		_writer.WriteEndElement();
	}

	{
		_writer.WriteStartElement(L"PARAM");
		_writer.WriteAttributeString(L"Name",L"ServerPort");
		CString strPort;
		strPort.Format(_T("%d"),m_uPort);
		_writer.WriteAttributeString(L"value",CComBSTR(strPort));
		_writer.WriteEndElement();
	}

	{
		_writer.WriteStartElement(L"PARAM");
		_writer.WriteAttributeString(L"Name",L"ServerUser");
		CCrypt Crypt;
		CString strUser = Crypt.Encrypt(m_strUser,CRYPT_KEY); 
		_writer.WriteAttributeString(L"value",CComBSTR(strUser));
		_writer.WriteAttributeString(L"encrypt",CComBSTR(_T("TRUE")));
		_writer.WriteEndElement();
	}

	{
		_writer.WriteStartElement(L"PARAM");
		_writer.WriteAttributeString(L"Name",L"ServerPwd");
		CCrypt Crypt;
		CString strPwd = Crypt.Encrypt(m_strPwd,CRYPT_KEY); 
		_writer.WriteAttributeString(L"value",CComBSTR(strPwd));
		_writer.WriteAttributeString(L"encrypt",CComBSTR(_T("TRUE")));
		_writer.WriteEndElement();
	}


	return S_OK;
}