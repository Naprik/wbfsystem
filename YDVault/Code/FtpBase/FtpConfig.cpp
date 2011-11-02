#include "StdAfx.h"
#include "FtpConfig.h"
#include "../Base/Xml.h"
#include "../base/DataHandler.h"
#include "../Base/FilePathHelper.h"

CFtpConfig::CFtpConfig(void)
{
	m_serverMode = CONFIG_SEVER_MODE(-1);
}

CFtpConfig::~CFtpConfig(void)
{
}

HRESULT CFtpConfig::Read()
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
		reader.ReadNodeListByTagName(_T("FTPESERVER"),nodes);
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
				if(CDataHandler::BSTRCompare(bstrChildNodeName,L"SERVER_TYPE") == 0)
				{
					CComBSTR bstrServerType;
					bstrServerType = nodeChild.GetNodeValue();
					m_serverMode = (CONFIG_SEVER_MODE)CDataHandler::VariantToLong(CComVariant(bstrServerType));
				}
				else if(CDataHandler::BSTRCompare(bstrChildNodeName,L"SERVERPARAM") == 0)
				{
					hr = m_FtpServerParam.Read(nodeChild);
					if(FAILED(hr))
					{
						return hr;
					}
				}
				else 
				{
					ASSERT(FALSE);
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

HRESULT CFtpConfig::Write()
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
		Writer.WriteStartElement(L"FTPESERVER");

		Writer.WriteStartElement(L"SERVER_TYPE");
		CString strServerType;
		strServerType.Format(_T("%d"),m_serverMode);
		Writer.WriteString(CComBSTR(strServerType));
		Writer.WriteEndElement();

	
		Writer.WriteStartElement(L"SERVERPARAM");
		hr = m_FtpServerParam.Write(Writer);
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

HRESULT CFtpConfig::GetXml(CString &_strXml)
{
	HRESULT hr = E_FAIL;
	CFilePathHelper::GetMainCfgPath(_strXml);
	_strXml += _T("\\cfg\\FtpServerCfg.xml");
	return S_OK;
}
