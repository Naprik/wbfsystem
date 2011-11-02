#include "StdAfx.h"
#include "DBConfig.h"
#include "../Base/Xml.h"
#include "../Base/DataHandler.h"
#include "../Base/FilePathHelper.h"

CDBConfig::CDBConfig(void)
{
	m_uDBType = CONFIG_DB_TYPE(-1);
	m_uDBConnetMode = CONFIG_DB_CONNET_MODE(-1);
}

CDBConfig::~CDBConfig(void)
{
}

HRESULT CDBConfig::Read()
{
	HRESULT hr = E_FAIL;
	try
	{
		CString strXml;
		hr = GetXml(strXml);
		if(FAILED(hr))
		{
			return hr;
		}
		CXmlReader reader;
		reader.LoadFile((BSTR)(LPCTSTR)strXml);
		CXmlNodeList nodes;
		reader.ReadNodeListByTagName(_T("DATABASESERVER"),nodes);
		ASSERT(nodes.GetLength() == 1);
		for(long i = 0; i < nodes.GetLength(); i++)
		{
			CXmlNode node;
			nodes.GetNode(i, node);
			for(int j = 0; j < node.GetChildCount();j++)
			{
				CXmlNode nodeChild;
				node.GetChildeNode(j,nodeChild);
				CComBSTR bstrChildNodeName = nodeChild.GetNodeName();
				if(CDataHandler::BSTRCompare(bstrChildNodeName,L"CONNECTION_MODE") == 0)
				{
					CComBSTR bstrConnetMode;
					bstrConnetMode = nodeChild.GetNodeValue();
					m_uDBConnetMode = (CONFIG_DB_CONNET_MODE)CDataHandler::VariantToLong(CComVariant(bstrConnetMode));
				}
				else if(CDataHandler::BSTRCompare(bstrChildNodeName,L"CONNECTIONPARAM") == 0)
				{
					hr = m_DBConnetParam.Read(nodeChild);
					if(FAILED(hr))
					{
						return hr;
					}
				}
				else if(CDataHandler::BSTRCompare(bstrChildNodeName,L"DATABASE_TYPE") == 0)
				{
					CComBSTR bstrDBType;
					bstrDBType = nodeChild.GetNodeValue();
					m_uDBType = (CONFIG_DB_TYPE)CDataHandler::VariantToLong(CComVariant(bstrDBType));
				}
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

HRESULT CDBConfig::Write()
{
	HRESULT hr = E_FAIL;
	try
	{	
		CString strXml;
		hr = GetXml(strXml);
		if(FAILED(hr))
		{
			return hr;
		}
	
		CXmlWriter Writer;
		Writer.WriteStartElement(L"YDVAULT");
		Writer.WriteStartElement(L"DATABASESERVER");
		
		Writer.WriteStartElement(L"DATABASE_TYPE");
		CString strDBType;
		strDBType.Format(_T("%d"),m_uDBType);
		Writer.WriteString(CComBSTR(strDBType));
		Writer.WriteEndElement();

		Writer.WriteStartElement(L"CONNECTION_MODE");
		CString strConnetionMode;
		strConnetionMode.Format(_T("%d"),m_uDBConnetMode);
		Writer.WriteString(CComBSTR(strConnetionMode));
		Writer.WriteEndElement();

		Writer.WriteStartElement(L"CONNECTIONPARAM");
		hr = m_DBConnetParam.Write(Writer);
		if(FAILED(hr))
		{
			return hr;
		}
		Writer.WriteEndElement();

		Writer.WriteEndElement();
		Writer.WriteEndElement();
		Writer.Save(CComBSTR(strXml));
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

HRESULT CDBConfig::GetXml(CString &_strXml)
{
	HRESULT hr = E_FAIL;
	CFilePathHelper::GetMainCfgPath(_strXml);
	_strXml += _T("\\cfg\\DBCfg.xml");
	return S_OK;
}