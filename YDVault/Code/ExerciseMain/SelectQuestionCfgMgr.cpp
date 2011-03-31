#include "StdAfx.h"
#include "SelectQuestionCfgMgr.h"
#include "../Base\AutoClean.h"
#include "QuestionCfgStruct.h"
#include "../Base\FilePathHelper.h"
#include "../Base\Xml.h"
#include "../Base\DataHandler.h"

CSelectQuestionCfgMgr::CSelectQuestionCfgMgr()
{
	m_uTimeSpan = 60;
	m_uSelQuesitonMode = SEL_QUESTION_EXERCISE;
	m_uQuestionVaultID = 0;
}

CSelectQuestionCfgMgr::~CSelectQuestionCfgMgr(void)
{
	Clear();
}

HRESULT CSelectQuestionCfgMgr::Load(const CString& strPath)
{
	HRESULT hr = E_FAIL;
	m_strPath = strPath;
	Clear();
	if(!CFilePathHelper::FilePathExists(m_strPath))
	{
		return S_FALSE;
	}
	try
	{
		CXmlReader reader;
		reader.LoadFile((BSTR)(LPCTSTR)strPath);
		CXmlNodeList nodes;
		reader.ReadNodeListByTagName(_T("CFG"),nodes);
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
				if(CDataHandler::BSTRCompare(bstrChildNodeName,L"PAPER") == 0)
				{
					CXmlNodeList lstNodeQuestion;
					nodeChild.GetChilds(lstNodeQuestion);
					for(long k = 0; k < lstNodeQuestion.GetLength();k++)
					{
						CXmlNode nodeQuestion;
						lstNodeQuestion.GetNode(k,nodeQuestion);
						CComBSTR bstrNodeQuestion =	nodeQuestion.GetNodeName();
						if(CDataHandler::BSTRCompare(bstrNodeQuestion,L"QUESTION") == 0)
						{
							CQuestionCfgStruct* pQStruct = new CQuestionCfgStruct();
							hr = pQStruct->Load(nodeQuestion);
							if(FAILED(hr))
							{
								return hr;
							}
							m_lstQuestionStruct.push_back(pQStruct);
						}
						else
						{
							ASSERT(FALSE);
						}
					}
				}
				else if(CDataHandler::BSTRCompare(bstrChildNodeName,L"EXAMTIME") == 0 &&
					   m_uSelQuesitonMode == SEL_QUESTION_EXAM)
				{
					//¿¼ÊÔÊ±¼ä
					CComBSTR bstrSpanTime;
					nodeChild.ReadAttributeByNoCase(L"time",&bstrSpanTime);
					m_uTimeSpan = (UINT)_ttoi(bstrSpanTime);
				}
				else if(CDataHandler::BSTRCompare(bstrChildNodeName,L"QUESTIONVAULT") == 0 )
				{
					//¿¼ÊÔÌâ¿â
					CComBSTR bstrVaultID;
					nodeChild.ReadAttributeByNoCase(L"ID",&bstrVaultID);
					m_uQuestionVaultID = _ttoi(bstrVaultID);
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

HRESULT CSelectQuestionCfgMgr::Save()
{
	HRESULT hr = E_FAIL;
	try
	{	
		CXmlWriter Writer;
		Writer.WriteStartElement(L"CFG");
		Writer.WriteStartElement(L"PAPER");

		for(std::list<CQuestionCfgStruct*>::const_iterator itr = m_lstQuestionStruct.begin();
			itr != m_lstQuestionStruct.end();++itr)
		{
			hr = (*itr)->Save(Writer);
			if(FAILED(hr))
			{
				return hr;
			}
		}

		Writer.WriteEndElement();//Paper
		if(m_uSelQuesitonMode == SEL_QUESTION_EXAM)
		{
			//¿¼ÊÔÊ±¼ä
			CString strTimeSpan;
			strTimeSpan.Format(_T("%d"),m_uTimeSpan);
			Writer.WriteStartElement(L"EXAMTIME");
			Writer.WriteAttributeString(L"time",CComBSTR(strTimeSpan));
			Writer.WriteEndElement();
		}
		//¿¼ÊÔÌâ¿â
		Writer.WriteStartElement(L"QUESTIONVAULT");
		CString strVaultID;
		strVaultID.Format(_T("%d"),m_uQuestionVaultID);
		Writer.WriteAttributeString(L"ID",CComBSTR(strVaultID));
		Writer.WriteEndElement();
		Writer.WriteEndElement();
		Writer.Save(CComBSTR(m_strPath));
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

HRESULT CSelectQuestionCfgMgr::Clear()
{
	CListAutoClean<CQuestionCfgStruct> clr(m_lstQuestionStruct);
	return S_OK;
}
