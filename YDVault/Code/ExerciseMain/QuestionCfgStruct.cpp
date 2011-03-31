#include "StdAfx.h"
#include "QuestionCfgStruct.h"
#include "../Base\Xml.h"
#include "../\Base\DataHandler.h"
#include "../Base\AutoClean.h"

CQuestionCfgStruct::CQuestionCfgStruct(void)
{
	m_QTypeID = 0;
}

CQuestionCfgStruct::~CQuestionCfgStruct(void)
{
	CListAutoClean<CQuestionLevelNum> clr(m_lstLevelNum);
}

HRESULT CQuestionCfgStruct::Load(CXmlNode& _node)
{
	HRESULT hr = E_FAIL;
	CComBSTR bstrName = _node.GetNodeName();
	ASSERT(CDataHandler::BSTRCompare(bstrName,L"QUESTION") == 0);
	CComBSTR bstrType;
	_node.ReadAttributeByNoCase(L"type",&bstrType);
	m_QTypeID = (OBJID)_ttoi(bstrType);
	CXmlNodeList nodeList;
	_node.GetChilds(nodeList);
	for(long i = 0; i < nodeList.GetLength();i++)
	{
		CXmlNode childNode;
		nodeList.GetNode(i,childNode);
		CComBSTR bstrChildName =	childNode.GetNodeName();
		ASSERT(CDataHandler::BSTRCompare(bstrChildName,L"HARDLEVEL") == 0);
		CComBSTR bstrLevel;
		childNode.ReadAttributeByNoCase(L"level",&bstrLevel);
		CComBSTR bstrEachNum;
		childNode.ReadAttributeByNoCase(L"eachnumber",&bstrEachNum);
		CComBSTR bstrNumber;
		childNode.ReadAttributeByNoCase(L"number",&bstrNumber);
		CComBSTR bstrMark;
		childNode.ReadAttributeByNoCase(L"mark",&bstrMark);
		if(bstrMark.Length() <= 0)
		{
			bstrMark = L"0.0";
		}
		CQuestionLevelNum* pLevelNum = new CQuestionLevelNum();
		pLevelNum->m_level = (HARDLEVEL)_ttoi(bstrLevel);
		pLevelNum->m_uEachNum = (UINT)_ttoi(bstrEachNum);
		pLevelNum->m_uNumber =  (UINT)_ttoi(bstrNumber);
		pLevelNum->m_fMark = (double)_tstof(bstrMark);

		m_lstLevelNum.push_back(pLevelNum);
	}
	return S_OK;
}
HRESULT CQuestionCfgStruct::Save(CXmlWriter& _writer)
{
	HRESULT hr = E_FAIL;
	_writer.WriteStartElement(L"QUESTION");
	CString strType;
	strType.Format(_T("%d"),m_QTypeID);
	_writer.WriteAttributeString(L"type",CComBSTR(strType));
	for(std::list<CQuestionLevelNum* >::const_iterator itr = m_lstLevelNum.begin();
		itr != m_lstLevelNum.end();++itr)
	{
		CString strLevel;
		strLevel.Format(_T("%d"),(*itr)->m_level);
		CString strEachNum;
		strEachNum.Format(_T("%d"),(*itr)->m_uEachNum);
		CString strNumber;
		strNumber.Format(_T("%d"),(*itr)->m_uNumber);
		CString strMark;
		strMark.Format(_T("%.2f"),(*itr)->m_fMark);
		_writer.WriteStartElement(L"HARDLEVEL");
		_writer.WriteAttributeString(L"level",CComBSTR(strLevel));
		_writer.WriteAttributeString(L"eachnumber",CComBSTR(strEachNum));
		_writer.WriteAttributeString(L"number",CComBSTR(strNumber));
		_writer.WriteAttributeString(L"mark",CComBSTR(strMark));
		_writer.WriteEndElement();
	}
	_writer.WriteEndElement();
	return S_OK;
}