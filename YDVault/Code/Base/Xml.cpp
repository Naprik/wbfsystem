/******************************************************************
** 文件名:	xml.cpp

** 

** 目的及主要功能:	该类主要用来顺序写入XML字节流的类

** 创建人:		
** 创建日期:	
** 修改人:		
** 修改日期:
** 描 述:		
** 版 本:		1.0.*
******************************************************************/

#include "stdafx.h"
#include "resource.h"

#include "Xml.h"
#include "xmlException.h"
#include <shlwapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*字符串到长整型*/
long __stdcall S2L(LPCTSTR sVal)
{
	return _tstol(sVal);
}

/*长整型到字符串*/
CString __stdcall L2S(long iVal)
{
	CString s;
	s.Format(_T("%d"),iVal);
	return s;
}

/*字符串到长整型*/
long __stdcall BSTR2L(BSTR sVal)
{
	if (sVal == NULL)
	{
		return 0L;
	}
	_bstr_t temp(sVal,true);
	return _tstol((LPCTSTR)temp);
}

/*长整型到字符串*/
BSTR __stdcall L2BSTR(long iVal)
{
	CString s;
	s.Format(_T("%d"),iVal);
	return _bstr_t((LPCTSTR)s).copy();
}

VARIANT_BOOL __stdcall BSTR2BOOL(BSTR str)
{
	CComBSTR s = str;
	s.ToLower();
	if (s == L"true")
	{
		return VARIANT_TRUE;
	}
	else
	{
		return VARIANT_FALSE;
	}
}
BSTR __stdcall BOOL2BSTR(VARIANT_BOOL b)
{
	if(!b)
	{
		return _bstr_t(L"false").copy();
	}
	else
	{
		return _bstr_t(L"true").copy();
	}
}

void MakeUpper(_bstr_t& bstrval)
{
	CComBSTR strtemp;
	strtemp.Attach(bstrval.GetBSTR());
	strtemp.ToUpper();
	strtemp.Detach();
}
void MakeLower(_bstr_t& bstrval)
{
	CComBSTR strtemp;
	strtemp.Attach(bstrval.GetBSTR());
	strtemp.ToLower();
	strtemp.Detach();
}

void TrimString(_bstr_t& bstrval,TCHAR c)
{
	CString sztemp = (BSTR)bstrval;
	sztemp.TrimLeft(c);
	sztemp.TrimRight(c);
	bstrval = sztemp;
}

//自动缩进，格式化xml用
void feedtabs(MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMNodePtr pParentNode, MSXML2::IXMLDOMNodePtr pRefNode, int tabs)
{
	CString s("\n");
	if(pRefNode)//第一子之前加nttt...(tabs个)
	{
		for(int i=0; i<tabs; i++) s += "\t";
		pParentNode->insertBefore(pDoc->createTextNode(_bstr_t(s)),(IUnknown *)pRefNode);
	}
	else{//最后一子后加nttt...((tabs-1个))
		for(int i=0; i<tabs-1; i++) s += "\t";
		pParentNode->appendChild( pDoc->createTextNode(_bstr_t(s)) );
	}
}
void WellFormXml(MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMNodePtr pParentNode, int tabs)
{
#ifdef _DEBUG 
	_bstr_t bstrName;
	bstrName = pParentNode->GetnodeName();
#endif

	if(pParentNode->hasChildNodes())
	{
		// 第一个孩子之前加"nt.."(tabs个't')
		IXMLDOMNodePtr pFirstChild = pParentNode->GetfirstChild();
		feedtabs(pDoc,pParentNode,pFirstChild,tabs);
		// 最后一个孩子之后加"nt.."(tabs-1个't')
		feedtabs(pDoc,pParentNode,NULL,tabs);
		// 递归孩子
		WellFormXml(pDoc,pFirstChild,tabs+1);
	}
	// 递归兄弟
	MSXML2::IXMLDOMNodePtr pNextSib = pParentNode->GetnextSibling();
	if( pNextSib ) 
	{
		MSXML2::IXMLDOMNodePtr pGrandParent = pParentNode->GetparentNode();
		if( (pGrandParent != NULL) && wcscmp(pNextSib->GetnodeTypeString(),L"text") ) 
		{
			// 在下一个兄弟之前加"nt.."(tabs-1个't')
			feedtabs(pDoc,pGrandParent,pNextSib,tabs-1);
			WellFormXml(pDoc,pNextSib,tabs);
		}
	}
}

/*****************************************************************
** 函数名:	构造函数
** 功能描述:	创建文档实例,初始化成员变量
****************************************************************/
CXmlWriter::CXmlWriter()
{
	HRESULT hr = m_pDocument.CreateInstance (__uuidof(MSXML2::DOMDocument));
	if(FAILED(hr))
	{
		CXmlException *e = new CXmlException();
		CString msg1;
		msg1.LoadString(IDS_BASE_XML4);
		e->SetAdditionalMsg(msg1);
		throw e;
	}
}

CXmlWriter::~CXmlWriter()
{
	Close();
}
/*****************************************************************
** 函数名:		Close
** 功能描述:	释放接口指针,释放内存,重新初始化成员变量
****************************************************************/

void CXmlWriter::Close() 
{
	//释放文档对象,清除内存
	ReleaseAll(m_pDocument);
	if(m_pDocument )
	{
	//	m_pDocument->removeChild (m_pCurElement);
		m_pDocument=NULL;
	}
}
/*****************************************************************
**	功能:		开始写文档, 应当与WriteEndDocument对称调用
**	输入函数	: 无
**	输出函数	: 无
**	返回值		: 	无
**	备注		: 应当对称调用并且WriteStartDocument()只应出现在最前面, WriteEndDocument() 只应出现在末尾
**	异常	: CpdemException
****************************************************************/
void CXmlWriter::WriteStartDocument()
{
	MSXML2::IXMLDOMProcessingInstructionPtr p = m_pDocument->createProcessingInstruction("xml", 
		" version='1.0' encoding='utf-8'");
	if (p != NULL) {
		m_pDocument->appendChild(p);
	}
}

void CXmlWriter::WriteEndDocument()
{
}

/*****************************************************************
**	功能:		写入一个注释节点
**	输入函数	: strComment . 注释值
**	输出函数	: 无
**	返回值		: 无
**	备注		: 
**	异常	: CpdemException
****************************************************************/
void CXmlWriter::WriteComment(BSTR strComment)
{
	if(m_pDocument)
	{
		MSXML2::IXMLDOMNodePtr pNode = m_pDocument->createNode (_variant_t((short)MSXML2::NODE_COMMENT) , "" , "" );
		pNode->put_text (_bstr_t(strComment));
		if(m_pCurElement == NULL)
		{
			//创建一个新的节点
			m_pDocument->appendChild (pNode);
			
		}
		else
		{
			//作为当前节点的子节点加入
			m_pCurElement->appendChild (pNode);
		}	
		pNode=NULL;
	}
	else
	{
	}
}
/*****************************************************************
**	功能	: 该组函数应当配套使用
			WriteStartElement() 开始写点元素
			WriteString () 往该节点元素写入字符串值
			WriteEndElement() 	结束写该节点元素

**	输入参数	: 
			_bstr_t strEement  : 该元素节点的名称
			_bstr_t 	strText :		要写入的值
**	输出参数: 无
**	返回值	: 无
**	备注	: 该组函数要成组调用,可以嵌套 .
**	异常	: CPdemException
****************************************************************/
void CXmlWriter::WriteStartElement(BSTR strElement)
{
	if(m_pDocument)
	{
		if(m_pCurElement == NULL)
		{//创建一个新的节点
			m_pCurElement = m_pDocument->createElement (_bstr_t(strElement));
			m_pDocument->appendChild (m_pCurElement);
		}
		else
		{//作为当前节点的子节点加入
			MSXML2::IXMLDOMElementPtr pElement = m_pDocument->createElement (_bstr_t(strElement));
			m_pCurElement->appendChild (pElement);
			m_pCurElement = pElement ;
		}	
	}
	else
	{
	}
}

void CXmlWriter::WriteString(BSTR strText)
{
	if(m_pCurElement )
	{	
		MSXML2::IXMLDOMTextPtr txt;
		txt = m_pDocument->createTextNode(strText);
		m_pCurElement->appendChild (txt);
	}
	else
	{
		//throw Exception ;
	}
}

void CXmlWriter::WriteInnerXml(BSTR strXml)
{
	MSXML2::IXMLDOMDocumentPtr doc ;
	doc.CreateInstance (__uuidof(MSXML2::DOMDocument));
	doc->loadXML(strXml);
	if(m_pCurElement)
	{
		m_pCurElement->appendChild(	doc->documentElement);
	}
	else
	{
		m_pDocument->appendChild(doc->documentElement);
	}
}

void CXmlWriter::WriteEndElement()
{//结束对当前节点的操作
	if(m_pCurElement)
	{//释放当前节点并且从量标尾部取出一个节点作为当前节点
		m_pCurElement = m_pCurElement->GetparentNode();
	}
	else
	{
	}
}
/*****************************************************************
**	功能	:	往当前元素加入一个属性
**	输入参数:	strAttrName :	属性名称
				strAttrValue		: 属性值
**	输出参数:	无
**	返回值	:	无
**	备注	: 	输入参数不得为空字符串
**	异常	:	CpdemException
****************************************************************/
void CXmlWriter::WriteAttributeString(BSTR strAttrName , BSTR strAttrValue)
{
	if(m_pCurElement)
	{//设置当前节点的属性
		m_pCurElement->setAttribute (	_bstr_t(strAttrName) ,
										_variant_t(strAttrValue));
	}
}

/*****************************************************************
**	功能	: 把XML文档装化为字符串
**	输入参数	: 无
**	输出参数	: 无
**	返回值		: 一个XML格式的字节流
**	备注		: 无
**	异常		: CPdemException
****************************************************************/
_bstr_t CXmlWriter::ToString()
{//直接返回文档接口的xml成员变量
	return m_pDocument->xml;
}
//MB_CUR_MAX 
/*****************************************************************
**	功能	:	把当前文档输出到硬盘
**	输入参数	: strFileName 	: 目标文件名
**	输出参数	: 无
**	备注		: 无
**	异常		: CPdemException
****************************************************************/
void CXmlWriter::Save(BSTR strFileName)
{
	//直接调用文档对象的save 函数
	try
	{
		_bstr_t filename(strFileName);	
		CFileFind finder;
		if (finder.FindFile(filename)) {
			DWORD dwAttrs = GetFileAttributes(filename); 
			if ((dwAttrs & FILE_ATTRIBUTE_READONLY)) 
			{ 
				SetFileAttributes(filename, 
					dwAttrs & ~FILE_ATTRIBUTE_READONLY); 
			} 
		}
		finder.Close();
#ifdef _DEBUG
	
		IXMLDOMElementPtr   pElemRoot   =   NULL;   
		pElemRoot   =   m_pDocument->documentElement;
		MSXML2::IXMLDOMNodePtr rootNode = pElemRoot; //你的xml根节点
		if(rootNode != NULL) 
		{
			WellFormXml(m_pDocument,rootNode,1);
		}
#endif
		m_pDocument->save (_variant_t(filename));
	}
	catch(_com_error &e)
	{
		CString strErr = e.ErrorMessage(); 
		CXmlException *xE = new CXmlException();
		CString strMsg ;
		CString strTemp;
		strTemp.LoadString(IDS_BASE_XML_SAVEER);
		strMsg.Format(strTemp,strFileName);
		xE->SetAdditionalMsg(strMsg,strErr);
		throw xE;
	}
}

/*****************************************************************
**	功能:		递归调用以释放所有的节点 .
**	输入参数	:无
**	输出参数	: 无
**	返回值		:无
**	备注		:私有函数,析构是调用.
**	异常		:无
****************************************************************/
void CXmlWriter::ReleaseAll(MSXML2::IXMLDOMNodePtr pParent)
{
	//该函数调用递归遍历每一个节点,如果发现该节点时文本节点那么移除该节点以保证内存的释放
	// simple for loop to get all children
	for (MSXML2::IXMLDOMNodePtr pChild = pParent->firstChild;
	NULL != pChild;
	pChild = pChild->nextSibling)
	{
		// for each child, call this function so that we get 
		// its children as well
		
		if (MSXML2::NODE_TEXT == pChild->nodeType)
		{
			pParent->removeChild (pChild);
		
		}
		else
			ReleaseAll(pChild);
	}
}
//构造函数,初始化所有成员变量
CXmlReader::CXmlReader()
{
	Init();
}
//析构函数,释放所有对象指针和内存 
CXmlReader::~CXmlReader ()
{
	Close();
}

HRESULT CXmlReader::Init()
{
	NodeType = (MSXML2::DOMNodeType)MSXML2::NODE_INVALID ;
	Value = _T("");
	Name = _T("");
	AttributeCount =0 ;
	AttributeName = _T("");
	AttributeValue= _T("");

	m_pDocument = NULL ;

	HRESULT hr = m_pDocument.CreateInstance (__uuidof(MSXML2::DOMDocument));
	if(FAILED(hr))
	{
		CXmlException *e = new CXmlException ();
		CString msg1;
		msg1.LoadString(IDS_BASE_XML_INITERR);
		e->SetAdditionalMsg(msg1);
		throw e;
	}
	m_pAttribute = NULL ;
	return hr ;
}

void CXmlReader::Close ()
{
	if(m_pDocument)
	{
		if(m_pAttribute)
			m_pAttribute=NULL;
		ReleaseAll(m_pDocument);
		m_pDocument= NULL ;
	}
	m_pCurNode = NULL ;
	NodeType = (MSXML2::DOMNodeType)MSXML2::NODE_INVALID;
}

/*****************************************************************
**	函数名称	:LoadFile
**	功能:		从文件中载入XML流 .
**	输入参数	:strFileName 要载入的文件名称
**	输出参数	:无
**	返回值		:无
**	异常		:CPdemException 
****************************************************************/
void CXmlReader::LoadFile(BSTR filename) 
{//要重新初始化个成员变量
	Close();
	Init();
	//USES_CONVERSION ;
	try
	{//直接调用文档对象的Load成员方法即可
		if(!PathFileExists(filename))
		{
			_ASSERT(FALSE);
		}
		_variant_t varOut((bool)TRUE);
		varOut = m_pDocument->load(filename);
		if((bool)varOut == FALSE)
		{//解析时出错
			MSXML2::IXMLDOMParseErrorPtr errPtr = m_pDocument->GetparseError();
			CString strMsg ;
			CString strTemp;
			strTemp.LoadString(IDS_BASE_XML_LOADERR);
			strMsg.Format(strTemp , filename );
			//抛出异常
			CXmlException *xE = new CXmlException () ;
			xE->SetAdditionalMsg( strMsg , (TCHAR*)errPtr->reason );
			throw xE ;
		}
		m_pCurNode = m_pDocument->documentElement ;
	}
	catch(_com_error &e)
	{
		Close();

		CString strMsg ;
		CString strTemp;
		strTemp.LoadString(IDS_BASE_XML_LOADERR);
		strMsg.Format(strTemp , filename );

		CString msg2 = e.ErrorMessage ();
		CXmlException *xE = new CXmlException () ;
		xE->SetAdditionalMsg(strMsg , msg2);
		throw xE ;

		TRACE("%s" , e.ErrorMessage ());
	}
}
/*****************************************************************
**	函数名	:	LoadXml
**	功能:		从双字节字节流中载入XML流 .
**	输入参数	:bstrXml要载入的字节流
**	输出参数	:无
**	返回值		:无
**	异常		:CPdemException 
****************************************************************/
void CXmlReader::LoadXml(BSTR	bstrXml)
{
	Close() ;
	Init();
	try
	{	
		_variant_t varOut((bool)TRUE);
		
		varOut = m_pDocument->loadXML (bstrXml);

		if((bool)varOut == FALSE)
		{
			MSXML2::IXMLDOMParseErrorPtr errPtr = m_pDocument->GetparseError();
			CString msg2 = (TCHAR*)errPtr->reason ;
			CString msg1;
			msg1.LoadString(IDS_BASE_XML_BYTEER);
			CXmlException *xE = new CXmlException ();
			xE->SetAdditionalMsg(msg1,msg2);
			throw xE;
		}
		m_pCurNode = m_pDocument->documentElement ;
	}
	catch(_com_error &e)
	{
		Close();
		TRACE("%s" , e.ErrorMessage ());
		CString msg2 = e.ErrorMessage ();
		CString msg1;
		msg1.LoadString(IDS_BASE_XML_BYTEER);
		CXmlException *xE = new CXmlException ();
		xE->SetAdditionalMsg(msg1,msg2);
		throw xE;
	}
}

/*****************************************************************
**	函数名称	: Read
**	功能:		读取到下一个节点
**	输入参数:	无
**	输出参数:	无
**	返回值:		如果为假,那么已经到达文档的结尾
**	备注:		循环调用该函数可以遍历每个节点
**	异常:		无
****************************************************************/
BOOL CXmlReader::Read ()
{
	if(m_pDocument == NULL)
		return FALSE;

	if(m_pCurNode )
	{
		//得取该节点得值
		NodeType = m_pCurNode->nodeType ;
		Name = m_pCurNode ->nodeName;
		MakeLower(Name);
		Value = m_pCurNode ->text ;
		m_pAttribute = NULL ;
		if(SUCCEEDED( m_pCurNode->get_attributes (&m_pAttribute ) )
			&& m_pAttribute != NULL)
		{//读取属性数目
			AttributeCount = (short)m_pAttribute->length ;
		}
		else
		{//否则清空属性信息
			AttributeCount = 0 ;
			AttributeName = _T("");
			AttributeValue = _T("");
		}
		if(m_pCurNode->firstChild != NULL)
		{//存在子元素节点或文本节点和注释节点
			m_pCurNode = m_pCurNode->firstChild ;
		}
		else if(m_pCurNode->nextSibling != NULL)
		{//如果存在同级别的节点
			m_pCurNode = m_pCurNode->nextSibling ;
		}
		else
		{//向上回溯
			while(m_pCurNode->parentNode!= NULL)
			{
				//if(m_pCurNode->parentNode != m_pDocument ->documentElement )
				if(m_pCurNode->parentNode != m_pDocument ->documentElement &&
					m_pCurNode->parentNode->nodeType != MSXML2::NODE_DOCUMENT)
				{//还没有回溯到根节点
					m_pCurNode= m_pCurNode->parentNode ; 
				}
				else
				{//当回溯到根节点时,直接指向下一个节点,而不比判断节点是否有效
					m_pCurNode = m_pCurNode->nextSibling ;
					break;
				}
				if(m_pCurNode->nextSibling!= NULL)
				{//如果该节点的存在有效的并列节点,那么指向该并列节点
					m_pCurNode = m_pCurNode->nextSibling ;
					break;
				}
			}
		}
		return TRUE ;
	}
	else
	{
		return FALSE;
	}
	
	return FALSE;
}

/*****************************************************************
**	函数名称	: ReadAttribute
**	功能:	读取当前节点的下一个属性
**	输入参数	:	无
**	输出参数	: 	无
**	返回值	:		如果为假,那么已经达到属性链表末尾
**	备注:			循环调用可以遍历某个节点的所有属性
**	异常:			无
****************************************************************/
BOOL CXmlReader::ReadAttribute() 
{
	if(m_pAttribute == NULL)
		return FALSE;
	MSXML2::IXMLDOMNodePtr pNode = NULL ;
	pNode = m_pAttribute->nextNode () ;
	if(pNode!= NULL)
	{
		AttributeName = pNode->GetnodeName ();
		MakeLower(AttributeName);
		CComBSTR bstrval;
		TrimString(AttributeName,' ');
		AttributeValue = pNode->GetnodeValue();
		TrimString(AttributeValue,' ');

		return TRUE ;
	}
	else
	{
		m_pAttribute = NULL ;
		return FALSE;
	}
		
	return FALSE;
}
/************************************************
类似于CXmlWriter的ReleaseAll()
**********************************************/
void CXmlReader::ReleaseAll(MSXML2::IXMLDOMNodePtr pParent)
{
	for (MSXML2::IXMLDOMNodePtr pChild = pParent->firstChild;
		NULL != pChild;
		pChild = pChild->nextSibling)
	{
		// for each child, call this function so that we get 
		// its children as well
		if (MSXML2::NODE_TEXT == pChild->nodeType)
		{
			pParent->removeChild (pChild);
		}
		else
			ReleaseAll(pChild);
	}
}

/*****************************************************************
**	函数名称	: FindSingleNode
**	功能:	依据特定节点名称查找节点值
**	输入参数	:	
**	Cstring strNodeName : 要查询的节点名称
**	输出参数	: 
**	Cstring : 该节点的值
****************************************************************/
_bstr_t CXmlReader::FindSingleNode(_bstr_t nodename)
{
	nodename = L"/" + nodename ;
	if(m_pDocument)
	{//调用文档对象的selectSingleNode函数直接找到节点值
		MSXML2::IXMLDOMNodePtr node = m_pDocument->selectSingleNode (nodename);
		if(node)
		{
			return  node->text ;
		}
	}
	return "";
}

BOOL CXmlReader::FindSingleNode(BSTR nodename,CXmlNode& node)
{
	_bstr_t bstrnodename = _T("/");
	bstrnodename += nodename ;
	if(m_pDocument)
	{//调用文档对象的selectSingleNode函数直接找到节点值
		MSXML2::IXMLDOMNodePtr _node = m_pDocument->selectSingleNode (nodename);
		if(_node)
		{
			node.SetDOMNode(_node);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CXmlReader::GetRootNode(CXmlNode& node)
{
	if(m_pDocument)
	{
		node.SetDOMNode(m_pDocument->documentElement);
		return TRUE;
	}
	return FALSE;
}

/*读取当前节点下子节点序列
*/
BOOL CXmlReader::ReadNodeListByTagName(BSTR strTagName , CXmlNodeList &Node )
{
	//MSXML2::IXMLDOMNodeList* pNodeList =m_pCurNode->GetchildNodes();
	MSXML2::IXMLDOMNodeListPtr pNodeList =m_pDocument->getElementsByTagName(strTagName);
	
	if(pNodeList == NULL )
	{
		return false;
	}
	else
	{
		Node.SetDOMNodeList(pNodeList.Detach());//.GetInterfacePtr());
		return true ;
	}
}
/*********************************************************************/

CXmlNode::CXmlNode()
{
	m_pDOMNode = NULL ;
	m_pAttribute = NULL ;
}

CXmlNode::~CXmlNode()
{
}

CXmlNode& CXmlNode::operator=(const CXmlNode& node)
{
	this->SetDOMNode(node.m_pDOMNode);
	return *this;
}

void CXmlNode::SetDOMNode(MSXML2::IXMLDOMNodePtr pDOMNode )
{
	m_pDOMNode = pDOMNode ;
	Name = m_pDOMNode->nodeName ;
	Value = m_pDOMNode->text ;
	NodeType = m_pDOMNode->nodeType ;
	m_pDOMNode->get_attributes(&m_pAttribute ) ;
	if(m_pAttribute )
	{
		AttributeCount = (short)m_pAttribute->length ;
	}
	else
	{
		AttributeCount = 0 ;
		AttributeName = "" ;
		AttributeValue = "" ; 
	}
}

/*****************************************************************
**	函数名称	: ReadAttribute
**	功能		: 读取当前节点的下一个属性
**	输入参数	:	无
**	输出参数	: 	无
**	返回值	:		如果为假,那么已经达到属性链表末尾
**	备注:			循环调用可以遍历某个节点的所有属性
**	异常:			无
****************************************************************/
BOOL CXmlNode::ReadAttribute() 
{
	//USES_CONVERSION ;
	if(m_pAttribute == NULL)
		return FALSE;
	MSXML2::IXMLDOMNodePtr pNode = NULL ;
	pNode = m_pAttribute->nextNode () ;
	if(pNode!= NULL)
	{
		//AttributeName = W2A(pNode->GetnodeName ());
		AttributeName = (pNode->GetnodeName ());
		MakeLower(AttributeName);
		TrimString(AttributeName,' ');
		//AttributeValue = W2A(_bstr_t(pNode->GetnodeValue()));
		AttributeValue = (pNode->GetnodeValue());
		TrimString(AttributeValue,' ');		
		return TRUE ;
	}
	else
	{
		m_pAttribute = NULL ;
		return FALSE;
	}
		
	return FALSE;
}

/******************
获取该节点下的子节点列表
*/
BOOL CXmlNode::GetChilds(CXmlNodeList &lstChilds)
{
	MSXML2::IXMLDOMNodeListPtr pLstChilds = NULL ;
	HRESULT hr = m_pDOMNode->get_childNodes(&pLstChilds);
	if(pLstChilds)
	{
		lstChilds.SetDOMNodeList(pLstChilds);
		return true ;
	}
	else 
		return false;
}
int CXmlNode::GetChildCount()
{
	MSXML2::IXMLDOMNodeListPtr pLstChilds = NULL ;
	HRESULT hr = m_pDOMNode->get_childNodes(&pLstChilds);
	if(pLstChilds)
	{
		return pLstChilds->Getlength();
	}
	else 
		return 0;
}
BOOL CXmlNode::GetChildeNode(int nIndex,CXmlNode& node)
{
	MSXML2::IXMLDOMNodeListPtr pLstChilds = NULL ;
	HRESULT hr = m_pDOMNode->get_childNodes(&pLstChilds);
	if(pLstChilds)
	{
		if(nIndex >= pLstChilds->Getlength())
		{
			return FALSE;
		}
		node.SetDOMNode(pLstChilds->Getitem(nIndex));
		return TRUE;
	}
	else 
		return FALSE;
}

_bstr_t CXmlNode::ToString()
{
	if (m_pDOMNode == NULL) {
		return L"";
	}
	else
	{
		return m_pDOMNode->xml;
	}
}
/*********************************************************************/

CXmlNodeList::CXmlNodeList()
{
	m_pDOMNodeList = NULL ;
}


CXmlNodeList::~CXmlNodeList()
{
}

BOOL CXmlNodeList::GetNode(long iIndex , CXmlNode& xmlNode ) 
{
	MSXML2::IXMLDOMNode* pNode = NULL ;
	m_pDOMNodeList->get_item(iIndex , &pNode);
	if(pNode== NULL)
	{
		return false;
	}
	else
	{
		xmlNode.SetDOMNode(pNode);
		return true ;
	}
}

void CXmlNodeList::SetDOMNodeList(MSXML2::IXMLDOMNodeList*	pDOMNodeList )
{
	if(m_pDOMNodeList )
	{
		m_pDOMNodeList = NULL ;
	}
	m_pDOMNodeList = pDOMNodeList ;
}

long CXmlNodeList::GetLength() 
{
	return m_pDOMNodeList->Getlength();
}

void CXmlWriter::WriteProcessingInstruction(BSTR s1, BSTR s2)
{
	MSXML2::IXMLDOMProcessingInstructionPtr p = m_pDocument->createProcessingInstruction(s1,s2);
	if (p != NULL) {
		m_pDocument->appendChild(p);
	}
}

HRESULT CXmlNode::AddAttribute(BSTR name, BSTR value)
{
	if(m_pDOMNode == NULL)
	{
		return S_FALSE;
	}
	return S_FALSE;
}

HRESULT CXmlNode::ReadAttribute(BSTR atrname,BSTR* atrvalue)
{
	*atrvalue = NULL;
	if(m_pDOMNode == NULL)
	{
		return S_FALSE;
	}
	HRESULT hr = S_FALSE;
	MSXML2::IXMLDOMNamedNodeMapPtr atrs = m_pDOMNode->Getattributes();
	if (atrs == NULL)
	{
		return hr;
	}
	IXMLDOMNodePtr atrnode = atrs->getNamedItem(atrname);
	if (atrnode != NULL)
	{
		_variant_t vtval;
		hr = atrnode->get_nodeValue(&vtval);
		if (FAILED(hr))
		{
			return hr;
		}
		*atrvalue = ((_bstr_t)vtval).Detach();
		return S_OK;
	}
	return hr;
}

HRESULT CXmlNode::ReadAttributeByNoCase(BSTR atrname,BSTR* atrvalue)
{
	if(m_pDOMNode == NULL)
	{
		return S_FALSE;
	}
	HRESULT hr = S_FALSE;
	MSXML2::IXMLDOMNamedNodeMapPtr atrs = m_pDOMNode->Getattributes();
	if (atrs == NULL)
	{
		return hr;
	}
	_bstr_t _atrname(atrname);
	MakeLower(_atrname);
	while(true)
	{
		MSXML2::IXMLDOMNodePtr pNode = NULL ;
		pNode = atrs->nextNode () ;
		if(pNode== NULL)
		{
			break;
		}
		_bstr_t tempatrname = pNode->GetnodeName ();
		MakeLower(tempatrname);
		TrimString(tempatrname,_T(' '));
		if(tempatrname == _atrname)
		{
			_bstr_t _atrvalue = _bstr_t(pNode->GetnodeValue());
			TrimString(_atrvalue,_T(' '));	
			*atrvalue = _atrvalue.Detach();
			return S_OK;
		}
	}
	return S_FALSE;
}