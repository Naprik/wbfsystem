#include "StdAfx.h"
#include "WorkSpaceMgr.h"
#include "../Base/Xml.h"
#include "../Base/DataHandler.h"

CWorkSpaceMgr CWorkSpaceMgr::s_Instance;

CWorkSpaceMgr::CWorkSpaceMgr(void)
{
}

CWorkSpaceMgr::~CWorkSpaceMgr(void)
{
}

HRESULT CWorkSpaceMgr::Load(const CString& strPath)
{
	m_strPath = strPath;
	try
	{
		CXmlReader reader;
		reader.LoadFile((BSTR)(LPCTSTR)m_strPath);
		CXmlNodeList nodes;
		reader.ReadNodeListByTagName(_T("WORKSPACE"), nodes);
		ASSERT(nodes.GetLength() == 1);
		CXmlNode node;
		nodes.GetNode(0, node);
		CComBSTR sValue;
		node.ReadAttributeByNoCase(TEXT("path"), &sValue);
		m_strWorkspace = sValue;

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
HRESULT CWorkSpaceMgr::Save()
{
	HRESULT hr = E_FAIL;
	try
	{	
		CXmlWriter writer;
		writer.WriteStartDocument();
		writer.WriteStartElement(L"CFG");
		writer.WriteStartElement(TEXT("WORKSPACE"));
		writer.WriteAttributeString(L"path", CComBSTR(m_strWorkspace));
		writer.WriteEndElement();
		writer.WriteEndElement();
		writer.WriteEndDocument();
		writer.Save(CComBSTR(m_strPath));
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

HRESULT CWorkSpaceMgr::SetWorkspace(const CString& strWorkspace)
{
	m_strWorkspace = strWorkspace;
	return S_OK;
}
HRESULT CWorkSpaceMgr::GetWorkSpace(CString* pStrWorkspace) const
{
	*pStrWorkspace = m_strWorkspace;
	return S_OK;
}
