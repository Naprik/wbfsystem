#include "StdAfx.h"
#include "InitInfoXml.h"
#include "../Base\FilePathHelper.h"
#include "../Base\Xml.h"

CInitInfoXml::CInitInfoXml(void)
{
}

CInitInfoXml::~CInitInfoXml(void)
{
}

HRESULT CInitInfoXml::LoadInitXmlInfo(CString &_strXmlFile)
{
	HRESULT hr = E_FAIL;
	_strXmlFile = _T("");
	CString strFile;
	hr = GetInitXmlFile(strFile);
	if(FAILED(hr))
	{
		return hr;
	}
	if(!CFilePathHelper::FilePathExists(strFile))
	{
		return S_OK;
	}
	try
	{
		CXmlReader reader;
		reader.LoadFile((BSTR)(LPCTSTR)strFile);
		CXmlNodeList nodes;
		reader.ReadNodeListByTagName(_T("InitFile"),nodes);
		ASSERT(nodes.GetLength() == 1);
		for(long i = 0; i < nodes.GetLength(); i++)
		{
			CXmlNode node;
			nodes.GetNode(i, node);
			CComBSTR sFile;
			node.ReadAttributeByNoCase(L"Name",&sFile);
			_strXmlFile = (const TCHAR*)sFile;

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

HRESULT CInitInfoXml::SaveInitXmlInfo(CString &_strXmlFile)
{
	HRESULT hr = E_FAIL;
	CString strFile;
	hr = GetInitXmlFile(strFile);
	if(FAILED(hr))
	{
		return hr;
	}
	try
	{	
		CXmlWriter Writer;
		Writer.WriteStartElement(L"InitFile");
		Writer.WriteAttributeString(L"Name",CComBSTR(_strXmlFile));
		Writer.WriteEndElement();
		Writer.Save(CComBSTR(strFile));
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

HRESULT CInitInfoXml::GetInitXmlFile(CString &_strFile)
{
	HRESULT hr = E_FAIL;
	_strFile = _ModulePath;
	_strFile += _T("\\Info\\Init.Xml");
	return S_OK;
}
