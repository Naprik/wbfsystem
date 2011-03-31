#include "StdAfx.h"
#include "CfgTree.h"
#include "../Base/Xml.h"
#include "../Base\FilePathHelper.h"
#include "../Base/DataHandler.h"

CCfgTree::CCfgTree(void)
{
	m_bShowQuestionTree = TRUE;// ‘Ã‚ø‚ «∑Òœ‘ æ
	m_bShowPaperTree = TRUE;
	m_bShowExamTree = TRUE;
	m_bShowDataMaintainTree = TRUE;
}

CCfgTree::~CCfgTree(void)
{

}

void CCfgTree::Init()
{
	HRESULT hr = E_FAIL;
	TCHAR	 ModulePath[MAX_PATH] = {0};
	DWORD nret = GetModuleFileName(NULL,ModulePath,MAX_PATH);
	ModulePath[nret] = 0;
	TCHAR* p = _tcsrchr(ModulePath,_T('\\'));
	if(p!=NULL) *p = 0;
	CString strXml = ModulePath;
	strXml += _T("\\..\\cfg\\TreeLayout.xml");
	if(!CFilePathHelper::FilePathExists(strXml))
	{
		return ;
	}
	CXmlReader reader;
	reader.LoadFile((BSTR)(LPCTSTR)strXml);
	CXmlNodeList nodes;
	reader.ReadNodeListByTagName(_T("YDVAULT_TREE"),nodes);
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
			if(CDataHandler::BSTRCompare(bstrChildNodeName,L"QuestionTree") == 0)
			{
				CComBSTR bstrVal = nodeChild.GetNodeValue();
				if(CDataHandler::BSTRCompare(bstrVal,L"YES") != 0)
				{
					m_bShowQuestionTree = FALSE;
				}
			}
			else if(CDataHandler::BSTRCompare(bstrChildNodeName,L"PaperTree") == 0)
			{
				CComBSTR bstrVal = nodeChild.GetNodeValue();
				if(CDataHandler::BSTRCompare(bstrVal,L"YES") != 0)
				{
					m_bShowPaperTree = FALSE;
				}
			}
			else if(CDataHandler::BSTRCompare(bstrChildNodeName,L"ExamTree") == 0)
			{
				CComBSTR bstrVal = nodeChild.GetNodeValue();
				if(CDataHandler::BSTRCompare(bstrVal,L"YES") != 0)
				{
					m_bShowExamTree = FALSE;
				}
			}
			else if(CDataHandler::BSTRCompare(bstrChildNodeName,L"DataMaintainTree") == 0)
			{
				CComBSTR bstrVal = nodeChild.GetNodeValue();
				if(CDataHandler::BSTRCompare(bstrVal,L"YES") != 0)
				{
					m_bShowDataMaintainTree = FALSE;
				}
			}
			else
			{
				ASSERT(FALSE);
			}
		}

	}
}
