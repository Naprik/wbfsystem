#include "StdAfx.h"
#include "InvigilateInfo.h"
#include "../Base\DataHandler.h"
#include "../Base\Xml.h"
#include "../ObjRef\YDObjectRef.h"
#include "../YDExamObjRef\YDExamAddress.h"
#include "../YDExamObjRef\YDClassRef.h"
#include "../YDExamObjRef\YdStudentRef.h"
#include "../YDExamObjRef\YDSubjectRef.h"
#include "../ObjRef\YDPaper.h"
#include "../YDExamObjRef\YDExamInfo.h"
#include "../Base\AutoClean.h"
#include "../YDExamObjRef\YDEAddrUnit.h"
#include "../YDExamObjRef\YDStuMark.h"
#include "../YDExamObjRef\YDEAddrUnit.h"
#include "../Base\FilePathHelper.h"
#include "../ObjHelper\StaticObjHelper.h"

#define CONST_EXAM_PAPER_NAME      _T("试卷名称")
#define CONST_EXAM_SUBJECT         _T("科目")
#define CONST_EXAM_DEPART          _T("参考院系")
#define CONST_EXAM_CLASS           _T("参考班级")
#define CONST_EXAM_ROOM            _T("考场")
#define CONST_EXAM_STARTTIME       _T("开考时间")
#define CONST_EXAM_ENDTIME         _T("收卷时间")
#define CONST_EXAM_STARTNO         _T("开始题号")
#define CONST_EXAM_ENDNO           _T("结束题号")
#define CONST_EXAM_MAXNO           _T("最大题号")

CInvigilateInfo::CInvigilateInfo(void)
{
	m_pExamInfo = NULL;
	m_strXmlFile = _T("");
}

CInvigilateInfo::~CInvigilateInfo(void)
{
	CPtrAutoClean<CYDExamInfo> clr(m_pExamInfo);
	CListAutoClean<CYDStuMark> clr2(m_lstStuMark);
}

HRESULT CInvigilateInfo::Save(BSTR _strFile)
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	try
	{	
		CXmlWriter Writer;
		Writer.WriteStartElement(L"YDVAULT");
		hr = SaveModeInfo(Writer,pGExamStruct);
		if(FAILED(hr))
		{
			return hr;
		}
	

		//考场信息
		Writer.WriteComment(L"考场信息");
		Writer.WriteStartElement(L"EXAMROOM");
		CYDObjectRef* pExamRoom = pGExamStruct->m_pExamDetail->m_pObjAddress;
		ASSERT(pExamRoom);
		hr = pExamRoom->SaveXml(Writer);
		if(FAILED(hr))
		{
			return hr;
		}
		Writer.WriteEndElement();

		//考试班级
		Writer.WriteComment(L"考试班级");
		Writer.WriteStartElement(L"EXAMCLASS");
		for(std::list<CYDObjectRef*>::const_iterator itr = pGExamStruct->m_pExamDetail->m_lstClass.begin();
			itr != pGExamStruct->m_pExamDetail->m_lstClass.end();++itr)
		{
			hr = (*itr)->SaveXml(Writer);
			if(FAILED(hr))
			{
				return hr;
			}
		}
		Writer.WriteEndElement();

		//考场的学生信息
		Writer.WriteComment(L"考场的单元信息");
		Writer.WriteStartElement(L"EXAMROOMUNIT");
		for(std::list<CYDObjectRef*>::const_iterator itr = pGExamStruct->m_pExamDetail->m_lstUnit.begin();
			itr != pGExamStruct->m_pExamDetail->m_lstUnit.end();++itr)
		{
			hr = (*itr)->SaveXml(Writer);
			if(FAILED(hr))
			{
				return hr;
			}
		}
		Writer.WriteEndElement();

		//考试科目
		Writer.WriteComment(L"考试科目");
		Writer.WriteStartElement(L"EXAMSUBJECT");
		CYDObjectRef* pObjSubject = pGExamStruct->m_pExamDetail->m_pSubject;
		ASSERT(pObjSubject);
		hr = (pObjSubject)->SaveXml(Writer);
		if(FAILED(hr))
		{
			return hr;
		}
		Writer.WriteEndElement();

		//考试试卷
		Writer.WriteComment(L"考试试卷");
		Writer.WriteStartElement(L"EXAMPAPER");
		CYDObjectRef* pObjPaper = pGExamStruct->m_pExamDetail->m_pPaper;
		ASSERT(pObjPaper);
		hr = (pObjPaper)->SaveXml(Writer);
		if(FAILED(hr))
		{
			return hr;
		}
		Writer.WriteEndElement();

		//开始时间与结束时间
		Writer.WriteComment(L"开始时间与结束时间");
		Writer.WriteStartElement(L"ExamTime");
		Writer.WriteStartElement(L"StartTime");
		CString strStart = CDataHandler::DateTimeToStr(pGExamStruct->m_pExamDetail->m_timeStart,2);
		Writer.WriteString(CComBSTR(strStart));
		Writer.WriteEndElement();
		Writer.WriteStartElement(L"EndTime");
		CString strEnd = CDataHandler::DateTimeToStr(pGExamStruct->m_pExamDetail->m_timeEnd,2);
		Writer.WriteString(CComBSTR(strEnd));
		Writer.WriteEndElement();
		Writer.WriteEndElement();

		//听力
		hr = SaveExamListen(Writer,pGExamStruct);
		if(FAILED(hr))
		{
			return hr;
		}

		hr = SaveExamAnswer(Writer,pGExamStruct);
		if(FAILED(hr))
		{
			return hr;
		}

		Writer.WriteEndElement();

		

		Writer.Save(_strFile);
		m_strXmlFile = _strFile;
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

HRESULT CInvigilateInfo::Load(BSTR _strFile)
{
	HRESULT hr = E_FAIL;

	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	hr = pGExamStruct->m_pExamDetail->InitClearData();
	if(FAILED(hr))
	{
		return hr;
	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	try
	{
		CXmlReader reader;
		reader.LoadFile(_strFile);
		CXmlNodeList nodes;
 		reader.ReadNodeListByTagName(_T("YDVAULT"),nodes);
 		ASSERT(nodes.GetLength() == 1);
		for(long i = 0; i < nodes.GetLength(); i++)
		{
			CXmlNode node;
			nodes.GetNode(i, node);
			CXmlNodeList nodeList;
			node.GetChilds(nodeList);
			for(long j = 0; j < nodeList.GetLength();j++)
			{
				CXmlNode nodeInfo;
				nodeList.GetNode(j,nodeInfo);
				CComBSTR bstrNodeName = nodeInfo.GetNodeName();
				if(CDataHandler::BSTRCompare(bstrNodeName,L"#comment") == 0)
				{
					continue;
				}
				if(CDataHandler::BSTRCompare(bstrNodeName,L"ModeInfo") == 0)
				{
					hr = LoadModeInfo(nodeInfo,pGExamStruct);
					if(FAILED(hr))
					{
						return hr;
					}
				}
				else if(CDataHandler::BSTRCompare(bstrNodeName,L"EXAMROOM") == 0)
				{
					CXmlNodeList lstNodeObj;
					nodeInfo.GetChilds(lstNodeObj);
					ASSERT(lstNodeObj.GetLength() == 1);
					for(long iNodeObj = 0; iNodeObj < lstNodeObj.GetLength();iNodeObj++)
					{
						CXmlNode nodeObj;
						lstNodeObj.GetNode(iNodeObj,nodeObj);
						pGExamStruct->m_pExamDetail->m_pObjAddress = new CYDExamAddress(pDB);
						hr = pGExamStruct->m_pExamDetail->m_pObjAddress->LoadXml(nodeObj);
						if(FAILED(hr))
						{
							return hr;
						}
					}
				
				}
				else if(CDataHandler::BSTRCompare(bstrNodeName,L"EXAMCLASS") == 0)
				{
					hr = LoadClass(nodeInfo,pGExamStruct,pDB);
					if(FAILED(hr))
					{
						return hr;
					}
				}
				else if(CDataHandler::BSTRCompare(bstrNodeName,L"EXAMROOMUNIT") == 0)
				{
					hr = LoadExamRoomUnit(nodeInfo,pGExamStruct,pDB);
					if(FAILED(hr))
					{
						return hr;
					}
				}
				else if(CDataHandler::BSTRCompare(bstrNodeName,L"EXAMSUBJECT") == 0)
				{
					CXmlNodeList lstNodeObj;
					nodeInfo.GetChilds(lstNodeObj);
					ASSERT(lstNodeObj.GetLength() == 1);
					for(long iNodeObj = 0; iNodeObj < lstNodeObj.GetLength();iNodeObj++)
					{
						CXmlNode nodeObj;
						lstNodeObj.GetNode(iNodeObj,nodeObj);
						pGExamStruct->m_pExamDetail->m_pSubject = new CYDSubjectRef(pDB);
						hr = pGExamStruct->m_pExamDetail->m_pSubject->LoadXml(nodeObj);
						if(FAILED(hr))
						{
							return hr;
						}
					}
				}
				else if(CDataHandler::BSTRCompare(bstrNodeName,L"EXAMPAPER") == 0)
				{
					CXmlNodeList lstNodeObj;
					nodeInfo.GetChilds(lstNodeObj);
					ASSERT(lstNodeObj.GetLength() == 1);
					for(long iNodeObj = 0; iNodeObj < lstNodeObj.GetLength();iNodeObj++)
					{
						CXmlNode nodeObj;
						lstNodeObj.GetNode(iNodeObj,nodeObj);
						pGExamStruct->m_pExamDetail->m_pPaper = new CYDPaper(pDB);
						hr = pGExamStruct->m_pExamDetail->m_pPaper->LoadXml(nodeObj);
						if(FAILED(hr))
						{
							return hr;
						}
					}
				}
				else if(CDataHandler::BSTRCompare(bstrNodeName,L"ExamTime") == 0)
				{
					hr = LoadExamTime(nodeInfo,pGExamStruct);
					if(FAILED(hr))
					{
						return hr;
					}
				}
				else if(CDataHandler::BSTRCompare(bstrNodeName,L"EXAMLISTEN") == 0)
				{
					hr = LoadExamListen(nodeInfo,pGExamStruct);
					if(FAILED(hr))
					{
						return hr;
					}
				}
				else if(CDataHandler::BSTRCompare(bstrNodeName,L"ExamAnswer") ==0)
				{
					hr = LoadExamAnswer(nodeInfo,pGExamStruct);
					if(FAILED(hr))
					{
						return hr;
					}
				}
				else
				{
					ASSERT(FALSE);
				}
			}
		}
		m_strXmlFile = _strFile;
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

HRESULT CInvigilateInfo::SaveExamListen(CXmlWriter &_writer,CGExamStruct* _pGExamStruct)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pGExamStruct);
	//听力信息
	_writer.WriteComment(L"听力信息");
	_writer.WriteStartElement(L"EXAMLISTEN");
	if(_pGExamStruct->m_pExamDetail->m_bListen)
	{
		_writer.WriteAttributeString(L"HaveListen",L"Yes");
	}
	else
	{
		_writer.WriteAttributeString(L"HaveListen",L"No");
	}
	//听力频道
	//_writer.WriteComment(L"听力频道");
	_writer.WriteStartElement(L"Channel");
	_writer.WriteString(CComBSTR(_pGExamStruct->m_pExamDetail->m_strChanal));
	_writer.WriteEndElement();
	_writer.WriteStartElement(L"EXAMLISTEN1");
	if(_pGExamStruct->m_pExamDetail->m_bListenPart1)
	{
		_writer.WriteString(L"Yes");
	}
	else
	{
		_writer.WriteString(L"No");
	}
	CString strStart1 = CDataHandler::DateTimeToStr(_pGExamStruct->m_pExamDetail->m_timeStart1,2);
	_writer.WriteAttributeString(L"StartTime",CComBSTR(strStart1));
	CString strEnd1 = CDataHandler::DateTimeToStr(_pGExamStruct->m_pExamDetail->m_timeEnd1,2);
	_writer.WriteAttributeString(L"EndTime",CComBSTR(strEnd1));
	_writer.WriteEndElement();

	_writer.WriteStartElement(L"EXAMLISTEN2");
	if(_pGExamStruct->m_pExamDetail->m_bListenPart2)
	{
		_writer.WriteString(L"Yes");
	}
	else
	{
		_writer.WriteString(L"No");
	}
	CString strStart2 = CDataHandler::DateTimeToStr(_pGExamStruct->m_pExamDetail->m_timeStart2,2);
	_writer.WriteAttributeString(L"StartTime",CComBSTR(strStart2));
	CString strEnd2 = CDataHandler::DateTimeToStr(_pGExamStruct->m_pExamDetail->m_timeEnd2,2);
	_writer.WriteAttributeString(L"EndTime",CComBSTR(strEnd2));
	_writer.WriteEndElement();

	_writer.WriteStartElement(L"EXAMLISTEN3");
	if(_pGExamStruct->m_pExamDetail->m_bListenPart3)
	{
		_writer.WriteString(L"Yes");
	}
	else
	{
		_writer.WriteString(L"No");
	}
	CString strStart3 = CDataHandler::DateTimeToStr(_pGExamStruct->m_pExamDetail->m_timeStart3,2);
	_writer.WriteAttributeString(L"StartTime",CComBSTR(strStart3));
	CString strEnd3 = CDataHandler::DateTimeToStr(_pGExamStruct->m_pExamDetail->m_timeEnd3,2);
	_writer.WriteAttributeString(L"EndTime",CComBSTR(strEnd3));
	_writer.WriteEndElement();

	_writer.WriteEndElement();
	
	return S_OK;
}

HRESULT CInvigilateInfo::SaveModeInfo(CXmlWriter &_writer,CGExamStruct* _pGExamStruct)
{
	HRESULT hr = E_FAIL;
	_writer.WriteStartElement(L"ModeInfo");
	//点名模式
	_writer.WriteComment(L"点名模式");
	_writer.WriteStartElement(L"ROOLCALLMODE");
	CString strRollCallMode;//点名模式
	strRollCallMode.Format(_T("%d"),_pGExamStruct->m_pExamDetail->m_uRollCallType);
	_writer.WriteString(CComBSTR(strRollCallMode));
	_writer.WriteEndElement();

	//收卷模式
	_writer.WriteComment(L"收卷模式");
	_writer.WriteStartElement(L"ROOLINGMODE");
	CString strRollingMode;
	strRollingMode.Format(_T("%d"),_pGExamStruct->m_pExamDetail->m_uRollingType);
	_writer.WriteString(CComBSTR(strRollingMode));
	_writer.WriteEndElement();

	//考试性质
	_writer.WriteComment(L"考试性质");
	_writer.WriteStartElement(L"EXAMKIND");
	CString strExamKind;
	strExamKind.Format(_T("%d"),_pGExamStruct->m_pExamDetail->m_uExamKind);
	_writer.WriteString(CComBSTR(strExamKind));
	_writer.WriteEndElement();

	_writer.WriteEndElement();
	return S_OK;
}

HRESULT CInvigilateInfo::SaveExamAnswer(CXmlWriter &_writer,CGExamStruct* _pGExamStruct)
{
	//答案信息
	HRESULT hr = E_FAIL;
	_writer.WriteComment(L"答案信息");
	_writer.WriteStartElement(L"ExamAnswer");
// 	for(std::list<CStudev*>::const_iterator itr =  _pGExamStruct->m_lstStudev.begin();
// 		itr != _pGExamStruct->m_lstStudev.end();++itr)
// 	{
// 		hr = (*itr)->SaveXml(_writer);
// 		if(FAILED(hr))
// 		{
// 			return hr;
// 		}
// 	}
	_writer.WriteEndElement();
	return S_OK;
}

HRESULT CInvigilateInfo::LoadModeInfo(CXmlNode& _node,CGExamStruct* _pGExamStruct)
{
	HRESULT hr = E_FAIL;
	CXmlNodeList lstNode;
	_node.GetChilds(lstNode);
	for(long i = 0; i < lstNode.GetLength();i++)
	{
		CXmlNode nodeChild ;
		lstNode.GetNode(i,nodeChild);
		CComBSTR bstrNodeName = nodeChild.GetNodeName();
		if(CDataHandler::BSTRCompare(bstrNodeName,L"#comment") == 0)
		{
			continue;
		}
		CComBSTR bstrNodeVal = nodeChild.GetNodeValue();
		if(CDataHandler::BSTRCompare(bstrNodeName,L"ROOLCALLMODE") == 0)
		{
			_pGExamStruct->m_pExamDetail->m_uRollCallType = (YDEXAM_ROLLCALL_TYPE)CDataHandler::VariantToBool(CComVariant(bstrNodeVal));
		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"ROOLINGMODE") == 0)
		{
			_pGExamStruct->m_pExamDetail->m_uRollingType = (YDEXAM_ROLLING_TYPE)CDataHandler::VariantToBool(CComVariant(bstrNodeVal));

		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"EXAMKIND") == 0)
		{
			_pGExamStruct->m_pExamDetail->m_uExamKind = (YDEXAM_KIND)CDataHandler::VariantToBool(CComVariant(bstrNodeVal));

		}
		else
		{
			ASSERT(FALSE);
		}
	}
	return S_OK;
}

HRESULT CInvigilateInfo::LoadClass(CXmlNode& _node,CGExamStruct* _pGExamStruct,CDatabaseEx* pDB)
{
	HRESULT hr = E_FAIL;
	CXmlNodeList lstNodeClass;
	_node.GetChilds(lstNodeClass);
	for(long i= 0; i < lstNodeClass.GetLength();i++)
	{
		CXmlNode nodeClass;
		lstNodeClass.GetNode(i,nodeClass);
		CYDObjectRef* pObjClass = new CYDClassRef(pDB);
		hr = pObjClass->LoadXml(nodeClass);
		if(FAILED(hr))
		{
			return hr;
		}
		_pGExamStruct->m_pExamDetail->m_lstClass.push_back(pObjClass);
	}
	return S_OK;
}

HRESULT CInvigilateInfo::LoadExamRoomUnit(CXmlNode& _node,CGExamStruct* _pGExamStruct,CDatabaseEx* pDB)
{
	HRESULT hr = E_FAIL;
	CXmlNodeList lstNodeStudent;
	_node.GetChilds(lstNodeStudent);
	for(long i= 0; i < lstNodeStudent.GetLength();i++)
	{
		CXmlNode nodeStudent;
		lstNodeStudent.GetNode(i,nodeStudent);
		CYDObjectRef* pObjStudent = new CYDEAddrUnit(pDB);
		hr = pObjStudent->LoadXml(nodeStudent);
		if(FAILED(hr))
		{
			return hr;
		}
		_pGExamStruct->m_pExamDetail->m_lstUnit.push_back(pObjStudent);
	}
	return S_OK;
}

HRESULT CInvigilateInfo::LoadExamTime(CXmlNode& _node,CGExamStruct* _pGExamStruct)
{
	HRESULT hr = E_FAIL;
	CXmlNodeList lstNode;
	_node.GetChilds(lstNode);
	for(long i = 0; i < lstNode.GetLength();i++)
	{
		CXmlNode nodeChild ;
		lstNode.GetNode(i,nodeChild);
		CComBSTR bstrNodeName = nodeChild.GetNodeName();
		CComBSTR bstrNodeVal = nodeChild.GetNodeValue();
		if(CDataHandler::BSTRCompare(bstrNodeName,L"StartTime") == 0)
		{
			hr = LoadTime(_pGExamStruct->m_pExamDetail->m_timeStart,bstrNodeVal);
			if(FAILED(hr))
			{
				return hr;
			}
		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"EndTime") == 0)
		{
			hr = LoadTime(_pGExamStruct->m_pExamDetail->m_timeEnd,bstrNodeVal);
			if(FAILED(hr))
			{
				return hr;
			}
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	return S_OK;
}

HRESULT CInvigilateInfo::LoadTime(COleDateTime& _time,CComBSTR& _bstrVal)
{
	HRESULT hr = E_FAIL;
	COleDateTime t1;
	t1.ParseDateTime(_bstrVal);
	COleDateTime tCur = COleDateTime::GetCurrentTime();
	_time.SetDateTime(tCur.GetYear(),
						tCur.GetMonth(),
						tCur.GetDay(),
						t1.GetHour(),
						t1.GetMinute(),
						t1.GetSecond());
#ifdef DEBUG
	CString strTime = CDataHandler::DateTimeToStr(_time,0);
#endif
	return S_OK;
}

HRESULT CInvigilateInfo::LoadExamListen(CXmlNode& _node,CGExamStruct* _pGExamStruct)
{
	HRESULT hr = E_FAIL;
	CComBSTR bstrHaveListen ;
	_node.ReadAttributeByNoCase(L"HaveListen",&bstrHaveListen);
	if(CDataHandler::BSTRCompare(bstrHaveListen,L"Yes") == 0)
	{
		_pGExamStruct->m_pExamDetail->m_bListen = TRUE;	
	}
	else
	{
		_pGExamStruct->m_pExamDetail->m_bListen = FALSE;
	}
	CXmlNodeList lstChild;
	_node.GetChilds(lstChild);
	for(long i = 0; i < lstChild.GetLength();i++)
	{
		CXmlNode nodeChild;
		lstChild.GetNode(i,nodeChild);
		CComBSTR bstrNodeName = nodeChild.GetNodeName();
		if(CDataHandler::BSTRCompare(bstrNodeName,L"#comment") == 0)
		{
			continue;
		}
		CComBSTR bstrNodeVal = nodeChild.GetNodeValue();
		if(CDataHandler::BSTRCompare(bstrNodeName,L"Channel") == 0)
		{
			_pGExamStruct->m_pExamDetail->m_strChanal = bstrNodeVal;
		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"EXAMLISTEN1") == 0)
		{
			if(CDataHandler::BSTRCompare(bstrNodeVal,L"Yes") == 0)
			{
				_pGExamStruct->m_pExamDetail->m_bListenPart1 = TRUE;	
			}
			else
			{
				_pGExamStruct->m_pExamDetail->m_bListenPart1 = FALSE;
			}
			CComBSTR bstrNodeStartTime;
			nodeChild.ReadAttributeByNoCase(L"StartTime",&bstrNodeStartTime);
			hr = LoadTime(_pGExamStruct->m_pExamDetail->m_timeStart1,bstrNodeStartTime);
			if(FAILED(hr))
			{
				return hr;
			}
			CComBSTR bstrNodeEndTime;
			nodeChild.ReadAttributeByNoCase(L"EndTime",&bstrNodeEndTime);
			hr = LoadTime(_pGExamStruct->m_pExamDetail->m_timeEnd1,bstrNodeEndTime);
			if(FAILED(hr))
			{
				return hr;
			}
		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"EXAMLISTEN2") == 0)
		{
			if(CDataHandler::BSTRCompare(bstrNodeVal,L"Yes") == 0)
			{
				_pGExamStruct->m_pExamDetail->m_bListenPart2 = TRUE;	
			}
			else
			{
				_pGExamStruct->m_pExamDetail->m_bListenPart2 = FALSE;
			}
			CComBSTR bstrNodeStartTime;
			nodeChild.ReadAttributeByNoCase(L"StartTime",&bstrNodeStartTime);
			hr = LoadTime(_pGExamStruct->m_pExamDetail->m_timeStart2,bstrNodeStartTime);
			if(FAILED(hr))
			{
				return hr;
			}
			CComBSTR bstrNodeEndTime;
			nodeChild.ReadAttributeByNoCase(L"EndTime",&bstrNodeEndTime);
			hr = LoadTime(_pGExamStruct->m_pExamDetail->m_timeEnd2,bstrNodeEndTime);
			if(FAILED(hr))
			{
				return hr;
			}
		}
		else if(CDataHandler::BSTRCompare(bstrNodeName,L"EXAMLISTEN3") == 0)
		{
			if(CDataHandler::BSTRCompare(bstrNodeVal,L"Yes") == 0)
			{
				_pGExamStruct->m_pExamDetail->m_bListenPart3 = TRUE;	
			}
			else
			{
				_pGExamStruct->m_pExamDetail->m_bListenPart3 = FALSE;
			}
			CComBSTR bstrNodeStartTime;
			nodeChild.ReadAttributeByNoCase(L"StartTime",&bstrNodeStartTime);
			hr = LoadTime(_pGExamStruct->m_pExamDetail->m_timeStart3,bstrNodeStartTime);
			if(FAILED(hr))
			{
				return hr;
			}
			CComBSTR bstrNodeEndTime;
			nodeChild.ReadAttributeByNoCase(L"EndTime",&bstrNodeEndTime);
			hr = LoadTime(_pGExamStruct->m_pExamDetail->m_timeEnd3,bstrNodeEndTime);
			if(FAILED(hr))
			{
				return hr;
			}
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	return S_OK;
}
HRESULT CInvigilateInfo::LoadExamAnswer(CXmlNode& _node,CGExamStruct* _pGExamStruct)
{
	HRESULT hr = E_FAIL;
	CXmlNodeList lstNodeStudev;
	_node.GetChilds(lstNodeStudev);
	for(long i= 0; i < lstNodeStudev.GetLength();i++)
	{
		CXmlNode nodeStudev;
		lstNodeStudev.GetNode(i,nodeStudev);
		CStudev* pStudev = new CStudev();
		hr = pStudev->LoadXml(nodeStudev);
		if(FAILED(hr))
		{
			return hr;
		}
		//_pGExamStruct->m_lstStudev.push_back(pStudev);
	}
	return S_OK;
}

HRESULT CInvigilateInfo::SaveInvigilateDB(CDatabaseEx* _pDB)
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	hr = CreateExamInfoObj(pGExamStruct,_pDB);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(m_pExamInfo);
	//EADDREINFO 考场与考试信息关系
	CYDLinkRef* pLinRef = new CYDLinkRef(_pDB,DB_YDLINK_EADDREINFO);
	CPtrAutoClean<CYDLinkRef> clr2(pLinRef);
	hr = pLinRef->PutObjRef(pGExamStruct->m_pExamDetail->m_pObjAddress,m_pExamInfo);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = m_pExamInfo->Save();
	if(FAILED(hr))
	{
		return hr;
	}
	hr = pLinRef->Save();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CInvigilateInfo::CreateExamInfoObj(CGExamStruct* _pGExamStruct,CDatabaseEx* _pDB)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pDB);
	if(m_pExamInfo)
	{
		return S_OK;
	}
	m_pExamInfo = new CYDExamInfo(_pDB);
	{
		//试卷ID
		ASSERT(_pGExamStruct->m_pExamDetail->m_pPaper);
		OBJID PaperID = 0;
		hr = _pGExamStruct->m_pExamDetail->m_pPaper->GetID(&PaperID);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valPaperID((long)PaperID);
		hr = m_pExamInfo->SetPropVal(FIELD_YEEXAMINFO_PAPERID,&valPaperID);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	{
		////开始时间
		CComVariant valStartTime;
		valStartTime.vt = VT_DATE;
		valStartTime.date =_pGExamStruct->m_pExamDetail->m_timeStart;
	//	CDataHandler::StringToVariant(_pGExamStruct->m_pExamDetail->m_timeStart,VT_DATE,&valStartTime);
		hr = m_pExamInfo->SetPropVal(FIELD_YEEXAMINFO_STARTTIME,&valStartTime);
		if(FAILED(hr))
		{
			return hr;
		}
	}

	{
		//结束时间
		CComVariant valEndTime;
		valEndTime.vt = VT_DATE;
		valEndTime.date = _pGExamStruct->m_pExamDetail->m_timeEnd;
		hr = m_pExamInfo->SetPropVal(FIELD_YEEXAMINFO_ENDTIME,&valEndTime);
		if(FAILED(hr))
		{
			return hr;
		}
	}

	{
		//监考老师
		CComVariant valInvigilateTeacher(_pGExamStruct->m_pExamDetail->m_strInvigilateTeacher);
		hr = m_pExamInfo->SetPropVal(FIELD_YEEXAMINFO_TEACHER,&valInvigilateTeacher);
		if(FAILED(hr))
		{
			return hr;
		}
	}

	{
		//考试信息文件名
		CString strFileName,strFileExt;
		CFilePathHelper::SplitFileName(m_strXmlFile,strFileName,strFileExt);
		CComVariant valExamXml(strFileName);
		hr = m_pExamInfo->SetPropVal(FIELD_YEEXAMINFO_EXAMXML,&valExamXml);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CInvigilateInfo::SaveAnswer(CDatabaseEx* _pDB,
				   CYDEAddrUnit* _pUint,
				   CYDObjectRef* _pPaper,
				   CYDObjectRef* _pSubject,
				   CString _strAnswer)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pExamInfo);
	ASSERT(_pDB);
	ASSERT(_pUint);
	ASSERT(_pPaper);
	ASSERT(_pSubject);
	//在学生成绩表中插入一条记录
	CYDStuMark* pStuMak = new CYDStuMark(_pDB);
	{
		//学生信息表OBJID
		CYdStudentRef* pStuRef = NULL;
		hr = _pUint->GetStudent(pStuRef);
		if(FAILED(hr))
		{
			return hr;
		}
		CPtrAutoClean<CYdStudentRef> clr(pStuRef);
		ASSERT(pStuRef);
		OBJID stuID = 0;
		hr = pStuRef->GetID(&stuID);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valStuID((long)stuID);
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_STUID,&valStuID);
		if(FAILED(hr))
		{
			return hr;
		}
	} 
	{
		//设备objid
		CYDDevice* pDev = NULL;
		hr = _pUint->GetDevice(pDev);
		if(FAILED(hr))
		{
			return hr;
		}
		CPtrAutoClean<CYDDevice> clr(pDev);
		ASSERT(pDev);
		OBJID idDev = 0;
		hr = pDev->GetID(&idDev);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valDevID((long)idDev);
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_DEVICEID,&valDevID);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	{
		//准考证号
		CComVariant valExamID;
		hr = _pUint->GetPropVal(FIELD_YDEXAMADDRUNIT_EXAMID,&valExamID);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_EXAMID,&valExamID);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	{
		//考试信息ID
		OBJID idExamInfo = 0;
		hr = m_pExamInfo->GetID(&idExamInfo);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valIDExamInfo((long)idExamInfo);
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_EXAMINFOID,&valIDExamInfo);
		if(FAILED(hr))
		{
			return hr;
		}
	}

	{
		//试卷ID
		OBJID idPaper = 0;
		hr = _pPaper->GetID(&idPaper);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valIDPaper((long)idPaper);
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_PAPERID,&valIDPaper);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	{
		//科目ID
		OBJID idSubject = 0;
		hr = _pSubject->GetID(&idSubject);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valIDSubject((long)idSubject);
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_SUBJECTID,&valIDSubject);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	{
		//学生答案
		CComVariant valAnswer(_strAnswer);
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_ANSWER,&valAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	{
		//分数
		double dbMark = 0;
		hr = pStuMak->CalMark(&dbMark);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valMark(dbMark);
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_MARK,&valMark);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	{
		//开考时间
		CComVariant valStartTime;
		hr = m_pExamInfo->GetPropVal(FIELD_YEEXAMINFO_STARTTIME,&valStartTime);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pStuMak->SetPropVal(FIELD_YDSTUMARK_EXAMDATE,&valStartTime);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	hr = pStuMak->Save();
	if(FAILED(hr))
	{
		return hr;
	}
	m_lstStuMark.push_back(pStuMak);
	return S_OK;
}

HRESULT CInvigilateInfo::IsExist(CDatabaseEx* _pDB,VARIANT_BOOL* _bExist)
{
	HRESULT hr = E_FAIL;
	ASSERT(_bExist);
	*_bExist = VARIANT_FALSE;
	if(m_strXmlFile.IsEmpty())
	{
		return S_OK;
	}
	CString strFileName,strFileExt;
	CFilePathHelper::SplitFileName(m_strXmlFile,strFileName,strFileExt);

	std::list<CPropQueryContidition*> lstQueryCon;
	CListAutoClean<CPropQueryContidition> clr(lstQueryCon);
	CPropQueryContidition* pPropQueryContidition = new CPropQueryContidition();
	pPropQueryContidition->m_uFieldType = VT_BSTR;
	pPropQueryContidition->m_uOpType = Q_EQUAL;
	pPropQueryContidition->m_strFiledName  = FIELD_YEEXAMINFO_EXAMXML;
	pPropQueryContidition->m_strConVal = strFileName;
	lstQueryCon.push_back(pPropQueryContidition);
	std::list<CYDObjectRef*> lstExamInfo;
	CListAutoClean<CYDObjectRef> clr1(lstExamInfo);
	hr = CStaticObjHelper::GetObjRef(DB_YDEXAM_INFO,_pDB,&lstExamInfo,&lstQueryCon);
	if(FAILED(hr))
	{
		return hr;
	}
	if(lstExamInfo.size() > 0)
	{
		*_bExist = VARIANT_TRUE;
	}
	return S_OK;
}