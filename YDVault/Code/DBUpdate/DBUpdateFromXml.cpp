#include "StdAfx.h"
#include "DBUpdateFromXml.h"
#include "../Base/Xml.h"


CDBUpdateFromXml::CDBUpdateFromXml(void)
{
}


CDBUpdateFromXml::~CDBUpdateFromXml(void)
{
}

HRESULT CDBUpdateFromXml::Load()
{
	HRESULT hr = E_FAIL;
	//所有的升级数据库都在../cfg/UpdateDB.xml中
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
		reader.ReadNodeListByTagName(_T("DBUpdate"),nodes);
	//	ASSERT(nodes.GetLength() == 1);
		for(long i = 0; i < nodes.GetLength(); i++)
		{
			CXmlNode node;
			nodes.GetNode(i, node);
			CComBSTR bstrDesc;
			node.ReadAttributeByNoCase(L"desc",&bstrDesc);
			CString strDesc = (const TCHAR*)bstrDesc;
			CComBSTR bstrSql;
			node.ReadAttributeByNoCase(L"sql",&bstrSql);
			CString strSql = (const TCHAR*)bstrSql;
			m_lstUpdateSQL.push_back(std::make_pair(strDesc,strSql));
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

HRESULT CDBUpdateFromXml::GetXml(CString &_strXml)
{
	HRESULT hr = E_FAIL;
	TCHAR filename[MAX_PATH] = {0};
	DWORD nret = GetModuleFileName(NULL,filename,MAX_PATH);
	filename[nret] = 0;
	TCHAR* p = _tcsrchr(filename,_T('\\')); //bin/
	if(p!=NULL) *p = 0;
	for(int i=0;i<1;i++)
	{
		p = _tcsrchr(filename,_T('\\')); //bin/
		if(p!=NULL) *p = 0;
	}
	CString strModulePath = filename;
	_strXml = strModulePath;
	_strXml += _T("\\cfg\\UpdateDB.xml");
	return S_OK;
}