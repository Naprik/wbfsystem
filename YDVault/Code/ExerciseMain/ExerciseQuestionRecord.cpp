#include "StdAfx.h"
#include "ExerciseQuestionRecord.h"
#include "../\Base\FilePathHelper.h"
#include "../Base\Xml.h"
#include "../Base\DataHandler.h"
#include "QuestionRecordStruct.h"

CExerciseQuestionRecord::CExerciseQuestionRecord(CString _strFileName)
:CQuestionRecord(_strFileName)
{
}

CExerciseQuestionRecord::CExerciseQuestionRecord()
:CQuestionRecord(TEXT(""))
{
}


CExerciseQuestionRecord::~CExerciseQuestionRecord(void)
{
}

HRESULT CExerciseQuestionRecord::Load()
{
	HRESULT hr = E_FAIL;
	hr = Clear();
	if(FAILED(hr))
	{
		return hr;
	}
	CString strPath ;
	hr = GetFilePath(strPath);
	if(FAILED(hr))
	{
		return hr;
	}
	if(!CFilePathHelper::FilePathExists(strPath))
	{
		return S_FALSE;
	}
	try
	{
		CXmlReader reader;
		reader.LoadFile((BSTR)(LPCTSTR)strPath);
		CXmlNodeList nodes;
		reader.ReadNodeListByTagName(_T("EXERCISE"),nodes);
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
							CQuestionRecordStruct* pQStruct = new CQuestionRecordStruct();
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
				else if(CDataHandler::BSTRCompare(bstrChildNodeName,L"MARK") == 0)
				{
					CComBSTR bstrCurNo;
					nodeChild.ReadAttributeByNoCase(L"curno",&bstrCurNo);
					m_iCurNo = _ttoi(bstrCurNo);
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
HRESULT CExerciseQuestionRecord::Save()
{
	HRESULT hr = E_FAIL;
	CString strPath ;
	hr = GetFilePath(strPath);
	if(FAILED(hr))
	{
		return hr;
	}
	try
	{	
		CXmlWriter Writer;
		Writer.WriteStartElement(L"EXERCISE");
		Writer.WriteStartElement(L"PAPER");

		for(std::list<CQuestionRecordStruct*>::const_iterator itr = m_lstQuestionStruct.begin();
			itr != m_lstQuestionStruct.end();++itr)
		{
			hr = (*itr)->Save(Writer);
			if(FAILED(hr))
			{
				return hr;
			}
		}

		Writer.WriteEndElement();//Paper
		CString strCurNo;
		strCurNo.Format(_T("%d"),m_iCurNo);
		Writer.WriteStartElement(L"MARK");
		Writer.WriteAttributeString(L"curno",CComBSTR(strCurNo));
		Writer.WriteEndElement();

		Writer.WriteEndElement();
		Writer.Save(CComBSTR(strPath));
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

HRESULT CExerciseQuestionRecord::CreateLog()
{
	//ExerciseRecord201012092034.xml
	HRESULT hr = E_FAIL;
	m_strFileName = _T("ExerciseRecord");
	COleDateTime currentime = COleDateTime::GetCurrentTime();
	CString strCurTime;
	strCurTime.Format(_T("%d%d%d%d%d%d"),currentime.GetYear(),
		currentime.GetMonth(),
		currentime.GetDay(),
		currentime.GetHour(),
		currentime.GetMinute(),
		currentime.GetSecond());
	m_strFileName += strCurTime;
	m_strFileName += _T(".XML");
	return S_OK;
}

