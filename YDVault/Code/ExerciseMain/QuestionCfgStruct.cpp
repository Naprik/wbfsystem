#include "StdAfx.h"
#include "QuestionCfgStruct.h"
#include "../Base/AutoClean.h"
#include "../Base/DataHandler.h"
#include "../Base/Xml.h"
#include "../ObjRef/YDQuestionType.h"
#include "../ObjRef/YDQuestionVault.h"
#include "ExerciseMain.h"

CQuestionCfgStruct::CQuestionCfgStruct(void)
{
	m_QTypeID = 0;
}

CQuestionCfgStruct::~CQuestionCfgStruct(void)
{
}

HRESULT CQuestionCfgStruct::Load(CXmlNode& _node)
{
	HRESULT hr = E_FAIL;
	CComBSTR bstrName = _node.GetNodeName();
	ASSERT(CDataHandler::BSTRCompare(bstrName,L"QUESTION") == 0);
	CComBSTR sValue;
	_node.ReadAttributeByNoCase(L"vault",&sValue);
	m_vaultID = (OBJID)_ttoi(sValue);
	_node.ReadAttributeByNoCase(L"type",&sValue);
	m_QTypeID = (OBJID)_ttoi(sValue);
	_node.ReadAttributeByNoCase(L"number", &sValue);
	m_cNum = (int)_ttoi(sValue);
	_node.ReadAttributeByNoCase(L"mark", &sValue);
	if(sValue.Length() <= 0)
	{
		sValue = L"0.0";
	}
	m_dMark = (double)_tstof(sValue);

	CXmlNodeList nodeList;
	_node.GetChilds(nodeList);
	for(long i = 0; i < nodeList.GetLength();i++)
	{
		CXmlNode childNode;
		nodeList.GetNode(i,childNode);
		CComBSTR sChildName = childNode.GetNodeName();
		ASSERT(CDataHandler::BSTRCompare(sChildName,L"FACTOR") == 0);
		CComBSTR sFactorField;
		childNode.ReadAttributeByNoCase(L"field",&sFactorField);
		CComBSTR sFactorName;
		childNode.ReadAttributeByNoCase(L"name",&sFactorName);
		CComBSTR sFactorVaule;
		childNode.ReadAttributeByNoCase(L"value",&sFactorVaule);
		m_lstFactors.push_back(CFactorValue(CString(sFactorField), CString(sFactorName), CString(sFactorVaule)));
	}
	return S_OK;
}
HRESULT CQuestionCfgStruct::Save(CXmlWriter& _writer)
{
	HRESULT hr = E_FAIL;
	_writer.WriteStartElement(L"QUESTION");
	CString strProp;
	strProp.Format(_T("%d"),m_vaultID);
	_writer.WriteAttributeString(L"vault",CComBSTR(strProp));
	strProp.Format(_T("%d"),m_QTypeID);
	_writer.WriteAttributeString(L"type",CComBSTR(strProp));
	strProp.Format(L"%d", m_cNum);
	_writer.WriteAttributeString(L"number", CComBSTR(strProp));
	strProp.Format(L"%.2f", m_dMark);
	_writer.WriteAttributeString(L"mark", CComBSTR(strProp));
	for(std::list<CFactorValue>::const_iterator itr = m_lstFactors.begin();
		itr != m_lstFactors.end(); ++itr)
	{
		_writer.WriteStartElement(L"FACTOR");
		_writer.WriteAttributeString(L"field",CComBSTR((*itr).m_field));
		_writer.WriteAttributeString(L"name",CComBSTR((*itr).m_name));
		_writer.WriteAttributeString(L"value",CComBSTR((*itr).m_value));
		_writer.WriteEndElement();
	}
	_writer.WriteEndElement();
	return S_OK;
}

HRESULT CQuestionCfgStruct::GetQuestionType(CString* questiontype)
{
	if (m_QTypeID != ID_EMPTY)
	{
		CYDQuestionType qtype(theApp.m_pDatabase);
		qtype.SetID(m_QTypeID);
		CString type;
		qtype.GetLabel(&type);
		CYDQuestionVault vault(theApp.m_pDatabase);
		vault.SetID(m_vaultID);
		vault.GetPropVal(FIELD_YDVAULT_NAME, *questiontype);
		*questiontype += type;
	}

	return S_OK;
}
HRESULT CQuestionCfgStruct::GetDescription(CString* description)
{
	*description = L"";
	CString strinfo;
	strinfo.Format(L"题数：%d ", m_cNum);
	*description += strinfo;
	if (m_dMark > 0)
	{
		strinfo.Format(L"每题%.2f分 ", m_dMark);
		*description += strinfo;
	}
	*description += L" 指标：";
	std::list<CFactorValue>::const_iterator itr
		= m_lstFactors.begin();
	for (; itr != m_lstFactors.end(); ++itr)
	{
		*description += L"(";
		*description += (*itr).m_name;
		*description += L"=";
		*description += (*itr).m_value;
		*description += L") ";
	}

	return S_OK;
}