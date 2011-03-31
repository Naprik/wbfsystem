#include "stdafx.h"
#include "Studev.h"
#include "../Base\Xml.h"
#include "../\Base\DataHandler.h"
#include "../Base\CriticalSectionControl.h"


CStudev::CStudev()
{
	m_index = -1;					//编号
	m_mac = _T("");
	m_stuid = _T("");
	m_rollcall = -1;
	m_rolling = -1;
	m_islate = FALSE;
	m_answer = _T("");
	m_islinkroll = FALSE;
	m_islinkroling = FALSE;
	m_bFetchAnswer = FALSE;
	m_bWriteDB     = FALSE;
}

CStudev::~CStudev()
{
	
}

CString  CStudev::Get_Roll_text()
{
	if(m_rollcall == 0)
		return _T("未点名");
	else if(m_rollcall == 1)
		return _T("进入考试状态");
	else if(m_rollcall == 2)
		return  _T("点名失败");
	else
		return _T("进入考试状态(迟到)");
}

CString CStudev::Get_Rolling_text()
{
	if(m_rolling == 0)
		return _T("未收卷");
	else if(m_rolling == 1)
		return _T("已收卷");
	else if(m_rolling == 2)
		return _T("收卷失败");
	else
		return _T("已收卷(提前交卷)");
}

/*BOOL CStudev::Setanswer(CString strAnswer, int start, int end)
{
	for(int i = 0; i < strAnswer.GetLength(); i++)
	{
		CStuanswer  sa;
		memset(sa.m_key, 0, sizeof(sa.m_key));
		strcpy(sa.m_key, strAnswer.Mid(i,i+1));
		sa.m_no = start;
		start++;
		m_answer.Add(sa);
	}
	return TRUE;
}*/

HRESULT	CStudev::LoadXml(CXmlNode& _node)
{
	HRESULT hr = E_FAIL;
	CComBSTR bstrName =	_node.GetNodeName();
	ASSERT(CDataHandler::BSTRCompare(bstrName,L"Studev") == 0);
	CXmlNodeList lstNode;
	_node.GetChilds(lstNode);
	for(long i = 0; i < lstNode.GetLength();i++)
	{
		CXmlNode nodeChild;
		lstNode.GetNode(i,nodeChild);
		CComBSTR bstrNodeName =  nodeChild.GetNodeName();
		CComBSTR bstrNodeVal = nodeChild.GetNodeValue();
		if(CDataHandler::BSTRCompare(bstrNodeName,L"#comment") == 0)
		{
			continue;
		}
		if(CDataHandler::BSTRCompare(bstrNodeName,L"Com") == 0)
		{
			m_com = (const TCHAR*)bstrNodeVal;
		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"Index") == 0)
		{
			m_index = (int)CDataHandler::VariantToLong(CComVariant(bstrNodeVal));
		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"MAC") == 0)
		{
			m_mac = (const TCHAR*)bstrNodeVal;
		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"StuID") == 0)
		{
			m_stuid = (const TCHAR*)bstrNodeVal;
		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"RollCall") == 0 )
		{
			m_rollcall = (int)CDataHandler::VariantToLong(CComVariant(bstrNodeVal));
		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"Rolling") == 0)
		{
			m_rolling = (int)CDataHandler::VariantToLong(CComVariant(bstrNodeVal));
		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"IsLate") == 0)
		{
			if(CDataHandler::BSTRCompare(bstrNodeVal,L"Yes") == 0)
			{
				m_islate = TRUE;
			}
			else
			{
				m_islate = FALSE;
			}
		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"Answer") == 0)
		{	
			m_answer = (const TCHAR*)bstrNodeVal;
		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"StartTime") == 0)
		{
			m_start.ParseDateTime(bstrNodeVal);
		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"EndTime") == 0)
		{
			m_end.ParseDateTime(bstrNodeVal);
		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"Islinkroll") == 0)
		{
			if(CDataHandler::BSTRCompare(bstrNodeVal,L"Yes") ==0)
			{
				m_islinkroll = TRUE;
			}
			else 
			{
				m_islinkroll = FALSE;
			}
		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"Islinkroling") == 0)
		{
			if(CDataHandler::BSTRCompare(bstrNodeVal,L"Yes") == 0)
			{
				m_islinkroling = TRUE;
			}
			else 
			{
				m_islinkroling = FALSE;
			}
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	return S_OK;
}
HRESULT CStudev::SaveXml(CXmlWriter& _writer)
{
	HRESULT hr = E_FAIL;
	_writer.WriteStartElement(L"Studev");
	_writer.WriteStartElement(L"Com");
	_writer.WriteString(CComBSTR(m_com));
	_writer.WriteEndElement();
	_writer.WriteStartElement(L"Index");	
	CString strIndex;
	strIndex.Format(_T("%d"),m_index);
	_writer.WriteString(CComBSTR(strIndex));
	_writer.WriteEndElement();

	_writer.WriteStartElement(L"Mac");
	_writer.WriteString(CComBSTR(m_mac));
	_writer.WriteEndElement();

	_writer.WriteStartElement(L"StuID");
	_writer.WriteString(CComBSTR(m_stuid));
	_writer.WriteEndElement();
	
	_writer.WriteStartElement(L"RollCall");
	CString strRollcall;
	strRollcall.Format(_T("%d"),m_rollcall);
	_writer.WriteString(CComBSTR(strRollcall));
	_writer.WriteEndElement();

	_writer.WriteStartElement(L"Rolling");
	CString strRolling;
	strRolling.Format(_T("%d"),m_rolling);
	_writer.WriteString(CComBSTR(strRolling));
	_writer.WriteEndElement();
	
	_writer.WriteStartElement(L"IsLate");
	if(m_islate)
	{
		_writer.WriteString(L"Yes");
	}
	else
	{
		_writer.WriteString(L"No");
	}
	_writer.WriteEndElement();

	_writer.WriteStartElement(L"Answer");
	_writer.WriteString(CComBSTR(m_answer));
	_writer.WriteEndElement();
	
	_writer.WriteStartElement(L"StartTime");
	_writer.WriteString(CComBSTR(m_start.Format()));
	_writer.WriteEndElement();
	
	_writer.WriteStartElement(L"EndTime");
	_writer.WriteString(CComBSTR(m_end.Format()));
	_writer.WriteEndElement();

	_writer.WriteStartElement(L"Islinkroll");
	if(m_islinkroll)
	{
		_writer.WriteString(L"Yes");
	}
	else
	{
		_writer.WriteString(L"No");
	}
	_writer.WriteEndElement();
	
	
	_writer.WriteStartElement(L"Islinkroling");
	if(m_islinkroling)
	{
		_writer.WriteString(L"Yes");
	}
	else
	{
		_writer.WriteString(L"No");
	}
	_writer.WriteEndElement();
	
	_writer.WriteEndElement();

	return S_OK;
}

HRESULT	CStudev::SetCom(CString _strCom)
{
	HRESULT hr = E_FAIL;
	CCriticalSectionControl cc(&m_cs);
	m_com = _strCom;
	return S_OK;
}
HRESULT	CStudev::GetCom(CString& _strCom)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	_strCom = m_com;
	return S_OK;
}
HRESULT	CStudev::SetIndex(int _index)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	m_index = _index;
	return S_OK;
}
HRESULT	CStudev::GetIndex(int& _index)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	_index = m_index;
	return S_OK;
}
HRESULT	CStudev::SetMac(CString _strMac)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	m_mac = _strMac;
	return S_OK;
}
HRESULT	CStudev::GetMac(CString& _strMac)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	_strMac = m_mac;
	return S_OK;
}
HRESULT	CStudev::SetStuID(CString _strStuID)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	m_stuid = _strStuID;
	return S_OK;
}
HRESULT	CStudev::GetSudID(CString& _strStuID)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	_strStuID = m_stuid;
	return S_OK;
}
HRESULT	CStudev::SetRollCall(int _iRollCall)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	m_rollcall = _iRollCall;
	return S_OK;
}
HRESULT	CStudev::GetRollCall(int &_iRollCall)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	_iRollCall = m_rollcall;
	return S_OK;
}
HRESULT	CStudev::SetRolling(int _iRolling)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	m_rolling = _iRolling;
	return S_OK;
}
HRESULT	CStudev::GetRolling(int &_iRolling)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	_iRolling = m_rolling;
	return S_OK;
}
HRESULT	CStudev::SetFetchAnswer(BOOL _bFetchAnswer)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	m_bFetchAnswer = _bFetchAnswer;
	return S_OK;
}
HRESULT	CStudev::GetFetchAnswer(BOOL& _bFetchAnswer)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	_bFetchAnswer = m_bFetchAnswer;
	return S_OK;
}
HRESULT CStudev::SetIsLate(BOOL _bIsLate)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	m_islate = _bIsLate;
	return S_OK;
}
HRESULT	CStudev::GetIsLate(BOOL &_bIsLate)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	_bIsLate = m_islate;
	return S_OK;
}
HRESULT	CStudev::SetAnswer(CString _strAnswer)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	m_answer = _strAnswer;
	return S_OK;
}
HRESULT CStudev::GetAnswer(CString& _strAnswer)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	_strAnswer = m_answer;
	return S_OK;
}
HRESULT	CStudev::SetStart(COleDateTime _tStart)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	m_start = _tStart;
	return S_OK;
}
HRESULT	CStudev::GetStart(COleDateTime& _tStart)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	_tStart = m_start;
	return S_OK;
}
HRESULT	CStudev::SetEnd(COleDateTime _tEnd)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	m_end = _tEnd;
	return S_OK;
}
HRESULT	CStudev::GetEnd(COleDateTime &_tEnd)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	_tEnd = m_end;
	return S_OK;
}
HRESULT	CStudev::SetIsLinkRoll(BOOL _bIsLinkRoll)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	m_islinkroll = _bIsLinkRoll;
	return S_OK;
}
HRESULT	CStudev::GetIsLinkRoll(BOOL& _bIsLinkRoll)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	_bIsLinkRoll = m_islinkroll;
	return S_OK;
}
HRESULT	CStudev::SetIsLinkRolling(BOOL _bIsLinkRolling)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	m_islinkroling = _bIsLinkRolling;
	return S_OK;
}
HRESULT	CStudev::GetIsLinkRolling(BOOL& _bIsLinkRolling)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	_bIsLinkRolling = m_islinkroling;
	return S_OK;
}
HRESULT	CStudev::SetIsWriteDB(BOOL _bWriteDB)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	m_bWriteDB = _bWriteDB;
	return S_OK;
}
HRESULT	CStudev::GetIsWriteDB(BOOL &_bWriteDB)
{
	CCriticalSectionControl cc(&m_cs);
	HRESULT hr = E_FAIL;
	_bWriteDB = m_bWriteDB;
	return S_OK;
}