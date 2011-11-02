#include "StdAfx.h"
#include "PaperTemplate.h"
#include "../Base/DataHandler.h"
#include "../Base/AutoClean.h"
#include "../Base/Xml.h"
#include "../Base/FilePathHelper.h"

CPaperTemplate::CPaperTemplate()
{
}

CPaperTemplate::~CPaperTemplate(void)
{
	CListAutoClean<CPaperTemplateItem> clr(m_lstItem);
}

HRESULT CPaperTemplate::Load(const CString& strTempName)
{
	HRESULT hr = E_FAIL;
	if (0 == m_strTempName.CompareNoCase(strTempName))
	{
		return S_FALSE;
	}
	{
		CListAutoClean<CPaperTemplateItem> clr(m_lstItem);
	}
	m_strTempName = strTempName;
	
	try
	{
		CString strXml;
		CFilePathHelper::GetMainCfgPath(strXml);
		strXml += _T("\\template\\paper\\");
		strXml += m_strTempName;
		CXmlReader reader;
		reader.LoadFile((BSTR)(LPCTSTR)strXml);
		CXmlNodeList nodes;
		reader.ReadNodeListByTagName(_T("PAPER"),nodes);
		ASSERT(nodes.GetLength() == 1);
		for(long i = 0; i < nodes.GetLength(); i++)
		{
			CXmlNode node;
			nodes.GetNode(i, node);
			for(int j = 0; j < node.GetChildCount();j++)
			{
				CPaperTemplateItem* pTempItem = new CPaperTemplateItem();
				m_lstItem.push_back(pTempItem);
				CXmlNode nodeChild;
				node.GetChildeNode(j,nodeChild);
				CComBSTR sID;
				nodeChild.ReadAttributeByNoCase(L"id", &sID);
				pTempItem->id = (OBJID)_ttoi(sID);

				CXmlNodeList lstNodeDirection;
				nodeChild.GetChilds(lstNodeDirection);
				for(int i = 0;i<(int)lstNodeDirection.GetLength();i++)
				{
					CXmlNode nodeDirection;
					lstNodeDirection.GetNode(i, nodeDirection);
					CComBSTR sDirection = nodeDirection.GetAttributeValue();
					CString strDirection = sDirection;
					pTempItem->strDirection = strDirection;
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