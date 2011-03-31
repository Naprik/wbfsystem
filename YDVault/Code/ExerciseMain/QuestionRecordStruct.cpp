#include "StdAfx.h"
#include "QuestionRecordStruct.h"
#include "../Base\Xml.h"
#include "../\Base\DataHandler.h"

CQuestionRecordStruct::CQuestionRecordStruct(void)
{
	m_QTypeID = ID_EMPTY;
	m_QNo = 0;
	m_uObjID = 0;
	m_fMark = 0.0;
}

CQuestionRecordStruct::~CQuestionRecordStruct(void)
{
}

HRESULT CQuestionRecordStruct::Load(CXmlNode& _node)
{
	HRESULT hr = E_FAIL;
	CComBSTR bstrName = _node.GetNodeName();
	ASSERT(CDataHandler::BSTRCompare(bstrName,L"QUESTION") == 0);
	CComBSTR bstrNo;
	_node.ReadAttributeByNoCase(L"no",&bstrNo);
	m_QNo = _ttoi(bstrNo);
	CComBSTR bstrType;
	_node.ReadAttributeByNoCase(L"type",&bstrType);
	m_QTypeID = (UINT)_ttoi(bstrType);
	CComBSTR bstrObjID;
	_node.ReadAttributeByNoCase(L"objid",&bstrObjID);
	m_uObjID = _ttoi(bstrObjID);
	CComBSTR bstrMark;
	_node.ReadAttributeByNoCase(L"mark",&bstrMark);
	m_fMark = (float)_tstof(bstrMark);
	CXmlNodeList nodeList;
	_node.GetChilds(nodeList);
	for(long i = 0; i < nodeList.GetLength();i++)
	{
		CXmlNode childNode;
		nodeList.GetNode(i,childNode);
		CComBSTR bstrChildName = childNode.GetNodeName();
		if(CDataHandler::BSTRCompare(bstrChildName,L"userAnswer") == 0)
		{
			hr = LoadUserAnswer(childNode);

		}
		else if(CDataHandler::BSTRCompare(bstrChildName,L"stdAnswer") == 0)
		{
			m_strStdAnswer = childNode.GetNodeValue();
		}
		else if(CDataHandler::BSTRCompare(bstrChildName,L"description") == 0)
		{
			m_strDesc = childNode.GetNodeValue();
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	return S_OK;
}
HRESULT CQuestionRecordStruct::Save(CXmlWriter& _writer)
{
	HRESULT hr = E_FAIL;
	_writer.WriteStartElement(L"QUESTION");
	CString strQNo;
	strQNo.Format(_T("%d"),m_QNo);
	_writer.WriteAttributeString(L"no",CComBSTR(strQNo));
	CString strType;
	strType.Format(_T("%d"),m_QTypeID);
	_writer.WriteAttributeString(L"type",CComBSTR(strType));
	CString strObjID;
	strObjID.Format(_T("%d"),m_uObjID);
	_writer.WriteAttributeString(L"objid",CComBSTR(strObjID));
	CString strMark;
	strMark.Format(_T("%.2f"),m_fMark);
	_writer.WriteAttributeString(L"mark",CComBSTR(strMark));
	_writer.WriteStartElement(L"userAnswer");
	hr = SaveUserAnswer(_writer);
	if(FAILED(hr))
	{
		return hr;
	}
	_writer.WriteEndElement();
	_writer.WriteStartElement(L"stdAnswer");
	_writer.WriteString(CComBSTR(m_strStdAnswer));
	_writer.WriteEndElement();
	_writer.WriteStartElement(L"description");
	_writer.WriteString(CComBSTR(m_strDesc));
	_writer.WriteEndElement();
	_writer.WriteEndElement();
	return S_OK;
}

HRESULT CQuestionRecordStruct::LoadUserAnswer(CXmlNode& _node)
{
	HRESULT hr = E_FAIL;
	CXmlNodeList lstNodeAnswer;
	_node.GetChilds(lstNodeAnswer);
	for(long i = 0; i < lstNodeAnswer.GetLength();i++)
	{
		CXmlNode nodeAnswer;
		lstNodeAnswer.GetNode(i,nodeAnswer);
		CComBSTR bstrAnswerName =	nodeAnswer.GetNodeName();
		if(CDataHandler::BSTRCompare(bstrAnswerName,L"Answer") == 0)
		{
			CString strUserAnswer = nodeAnswer.GetNodeValue();
			m_listUserAnswers.push_back(strUserAnswer);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	return S_OK;
}

HRESULT CQuestionRecordStruct::SaveUserAnswer(CXmlWriter& _writer)
{
	HRESULT hr = E_FAIL;
	for(std::list<CString>::const_iterator itr = m_listUserAnswers.begin();
		itr != m_listUserAnswers.end();++itr)
	{
		_writer.WriteStartElement(L"Answer");
		_writer.WriteString(CComBSTR(*itr));
		_writer.WriteEndElement();
	}
	return S_OK;
}

HRESULT CQuestionRecordStruct::ComputeMark(double* _dbMark)
{
	HRESULT hr = E_FAIL;
	*_dbMark = 0.0;
	ASSERT(m_pQuestion);
	std::list<CString> lstStdAnswer;
	hr = m_pQuestion->GetStandardAnswer(lstStdAnswer);
	if(FAILED(hr))
	{
		return hr;
	}
	std::list<CString>::const_iterator itrStd = lstStdAnswer.begin();
	std::list<CString>::const_iterator itrUser = m_listUserAnswers.begin();
	for(;itrStd != lstStdAnswer.end() && itrUser != m_listUserAnswers.end();
		++itrStd,++itrUser)
	{
		if((*itrStd).CompareNoCase(*itrUser) == 0)
		{
			*_dbMark = m_fMark;
		}
	}
	
	return S_OK;
}