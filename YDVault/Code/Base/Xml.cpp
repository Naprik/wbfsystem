/******************************************************************
** �ļ���:	xml.cpp

** 

** Ŀ�ļ���Ҫ����:	������Ҫ����˳��д��XML�ֽ�������

** ������:		
** ��������:	
** �޸���:		
** �޸�����:
** �� ��:		
** �� ��:		1.0.*
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

/*�ַ�����������*/
long __stdcall S2L(LPCTSTR sVal)
{
	return _tstol(sVal);
}

/*�����͵��ַ���*/
CString __stdcall L2S(long iVal)
{
	CString s;
	s.Format(_T("%d"),iVal);
	return s;
}

/*�ַ�����������*/
long __stdcall BSTR2L(BSTR sVal)
{
	if (sVal == NULL)
	{
		return 0L;
	}
	_bstr_t temp(sVal,true);
	return _tstol((LPCTSTR)temp);
}

/*�����͵��ַ���*/
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

//�Զ���������ʽ��xml��
void feedtabs(MSXML2::IXMLDOMDocument2Ptr pDoc, MSXML2::IXMLDOMNodePtr pParentNode, MSXML2::IXMLDOMNodePtr pRefNode, int tabs)
{
	CString s("\n");
	if(pRefNode)//��һ��֮ǰ��nttt...(tabs��)
	{
		for(int i=0; i<tabs; i++) s += "\t";
		pParentNode->insertBefore(pDoc->createTextNode(_bstr_t(s)),(IUnknown *)pRefNode);
	}
	else{//���һ�Ӻ��nttt...((tabs-1��))
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
		// ��һ������֮ǰ��"nt.."(tabs��'t')
		IXMLDOMNodePtr pFirstChild = pParentNode->GetfirstChild();
		feedtabs(pDoc,pParentNode,pFirstChild,tabs);
		// ���һ������֮���"nt.."(tabs-1��'t')
		feedtabs(pDoc,pParentNode,NULL,tabs);
		// �ݹ麢��
		WellFormXml(pDoc,pFirstChild,tabs+1);
	}
	// �ݹ��ֵ�
	MSXML2::IXMLDOMNodePtr pNextSib = pParentNode->GetnextSibling();
	if( pNextSib ) 
	{
		MSXML2::IXMLDOMNodePtr pGrandParent = pParentNode->GetparentNode();
		if( (pGrandParent != NULL) && wcscmp(pNextSib->GetnodeTypeString(),L"text") ) 
		{
			// ����һ���ֵ�֮ǰ��"nt.."(tabs-1��'t')
			feedtabs(pDoc,pGrandParent,pNextSib,tabs-1);
			WellFormXml(pDoc,pNextSib,tabs);
		}
	}
}

/*****************************************************************
** ������:	���캯��
** ��������:	�����ĵ�ʵ��,��ʼ����Ա����
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
** ������:		Close
** ��������:	�ͷŽӿ�ָ��,�ͷ��ڴ�,���³�ʼ����Ա����
****************************************************************/

void CXmlWriter::Close() 
{
	//�ͷ��ĵ�����,����ڴ�
	ReleaseAll(m_pDocument);
	if(m_pDocument )
	{
	//	m_pDocument->removeChild (m_pCurElement);
		m_pDocument=NULL;
	}
}
/*****************************************************************
**	����:		��ʼд�ĵ�, Ӧ����WriteEndDocument�ԳƵ���
**	���뺯��	: ��
**	�������	: ��
**	����ֵ		: 	��
**	��ע		: Ӧ���ԳƵ��ò���WriteStartDocument()ֻӦ��������ǰ��, WriteEndDocument() ֻӦ������ĩβ
**	�쳣	: CpdemException
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
**	����:		д��һ��ע�ͽڵ�
**	���뺯��	: strComment . ע��ֵ
**	�������	: ��
**	����ֵ		: ��
**	��ע		: 
**	�쳣	: CpdemException
****************************************************************/
void CXmlWriter::WriteComment(BSTR strComment)
{
	if(m_pDocument)
	{
		MSXML2::IXMLDOMNodePtr pNode = m_pDocument->createNode (_variant_t((short)MSXML2::NODE_COMMENT) , "" , "" );
		pNode->put_text (_bstr_t(strComment));
		if(m_pCurElement == NULL)
		{
			//����һ���µĽڵ�
			m_pDocument->appendChild (pNode);
			
		}
		else
		{
			//��Ϊ��ǰ�ڵ���ӽڵ����
			m_pCurElement->appendChild (pNode);
		}	
		pNode=NULL;
	}
	else
	{
	}
}
/*****************************************************************
**	����	: ���麯��Ӧ������ʹ��
			WriteStartElement() ��ʼд��Ԫ��
			WriteString () ���ýڵ�Ԫ��д���ַ���ֵ
			WriteEndElement() 	����д�ýڵ�Ԫ��

**	�������	: 
			_bstr_t strEement  : ��Ԫ�ؽڵ������
			_bstr_t 	strText :		Ҫд���ֵ
**	�������: ��
**	����ֵ	: ��
**	��ע	: ���麯��Ҫ�������,����Ƕ�� .
**	�쳣	: CPdemException
****************************************************************/
void CXmlWriter::WriteStartElement(BSTR strElement)
{
	if(m_pDocument)
	{
		if(m_pCurElement == NULL)
		{//����һ���µĽڵ�
			m_pCurElement = m_pDocument->createElement (_bstr_t(strElement));
			m_pDocument->appendChild (m_pCurElement);
		}
		else
		{//��Ϊ��ǰ�ڵ���ӽڵ����
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
{//�����Ե�ǰ�ڵ�Ĳ���
	if(m_pCurElement)
	{//�ͷŵ�ǰ�ڵ㲢�Ҵ�����β��ȡ��һ���ڵ���Ϊ��ǰ�ڵ�
		m_pCurElement = m_pCurElement->GetparentNode();
	}
	else
	{
	}
}
/*****************************************************************
**	����	:	����ǰԪ�ؼ���һ������
**	�������:	strAttrName :	��������
				strAttrValue		: ����ֵ
**	�������:	��
**	����ֵ	:	��
**	��ע	: 	�����������Ϊ���ַ���
**	�쳣	:	CpdemException
****************************************************************/
void CXmlWriter::WriteAttributeString(BSTR strAttrName , BSTR strAttrValue)
{
	if(m_pCurElement)
	{//���õ�ǰ�ڵ������
		m_pCurElement->setAttribute (	_bstr_t(strAttrName) ,
										_variant_t(strAttrValue));
	}
}

/*****************************************************************
**	����	: ��XML�ĵ�װ��Ϊ�ַ���
**	�������	: ��
**	�������	: ��
**	����ֵ		: һ��XML��ʽ���ֽ���
**	��ע		: ��
**	�쳣		: CPdemException
****************************************************************/
_bstr_t CXmlWriter::ToString()
{//ֱ�ӷ����ĵ��ӿڵ�xml��Ա����
	return m_pDocument->xml;
}
//MB_CUR_MAX 
/*****************************************************************
**	����	:	�ѵ�ǰ�ĵ������Ӳ��
**	�������	: strFileName 	: Ŀ���ļ���
**	�������	: ��
**	��ע		: ��
**	�쳣		: CPdemException
****************************************************************/
void CXmlWriter::Save(BSTR strFileName)
{
	//ֱ�ӵ����ĵ������save ����
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
		MSXML2::IXMLDOMNodePtr rootNode = pElemRoot; //���xml���ڵ�
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
**	����:		�ݹ�������ͷ����еĽڵ� .
**	�������	:��
**	�������	: ��
**	����ֵ		:��
**	��ע		:˽�к���,�����ǵ���.
**	�쳣		:��
****************************************************************/
void CXmlWriter::ReleaseAll(MSXML2::IXMLDOMNodePtr pParent)
{
	//�ú������õݹ����ÿһ���ڵ�,������ָýڵ�ʱ�ı��ڵ���ô�Ƴ��ýڵ��Ա�֤�ڴ���ͷ�
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
//���캯��,��ʼ�����г�Ա����
CXmlReader::CXmlReader()
{
	Init();
}
//��������,�ͷ����ж���ָ����ڴ� 
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
**	��������	:LoadFile
**	����:		���ļ�������XML�� .
**	�������	:strFileName Ҫ������ļ�����
**	�������	:��
**	����ֵ		:��
**	�쳣		:CPdemException 
****************************************************************/
void CXmlReader::LoadFile(BSTR filename) 
{//Ҫ���³�ʼ������Ա����
	Close();
	Init();
	//USES_CONVERSION ;
	try
	{//ֱ�ӵ����ĵ������Load��Ա��������
		if(!PathFileExists(filename))
		{
			_ASSERT(FALSE);
		}
		_variant_t varOut((bool)TRUE);
		varOut = m_pDocument->load(filename);
		if((bool)varOut == FALSE)
		{//����ʱ����
			MSXML2::IXMLDOMParseErrorPtr errPtr = m_pDocument->GetparseError();
			CString strMsg ;
			CString strTemp;
			strTemp.LoadString(IDS_BASE_XML_LOADERR);
			strMsg.Format(strTemp , filename );
			//�׳��쳣
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
**	������	:	LoadXml
**	����:		��˫�ֽ��ֽ���������XML�� .
**	�������	:bstrXmlҪ������ֽ���
**	�������	:��
**	����ֵ		:��
**	�쳣		:CPdemException 
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
**	��������	: Read
**	����:		��ȡ����һ���ڵ�
**	�������:	��
**	�������:	��
**	����ֵ:		���Ϊ��,��ô�Ѿ������ĵ��Ľ�β
**	��ע:		ѭ�����øú������Ա���ÿ���ڵ�
**	�쳣:		��
****************************************************************/
BOOL CXmlReader::Read ()
{
	if(m_pDocument == NULL)
		return FALSE;

	if(m_pCurNode )
	{
		//��ȡ�ýڵ��ֵ
		NodeType = m_pCurNode->nodeType ;
		Name = m_pCurNode ->nodeName;
		MakeLower(Name);
		Value = m_pCurNode ->text ;
		m_pAttribute = NULL ;
		if(SUCCEEDED( m_pCurNode->get_attributes (&m_pAttribute ) )
			&& m_pAttribute != NULL)
		{//��ȡ������Ŀ
			AttributeCount = (short)m_pAttribute->length ;
		}
		else
		{//�������������Ϣ
			AttributeCount = 0 ;
			AttributeName = _T("");
			AttributeValue = _T("");
		}
		if(m_pCurNode->firstChild != NULL)
		{//������Ԫ�ؽڵ���ı��ڵ��ע�ͽڵ�
			m_pCurNode = m_pCurNode->firstChild ;
		}
		else if(m_pCurNode->nextSibling != NULL)
		{//�������ͬ����Ľڵ�
			m_pCurNode = m_pCurNode->nextSibling ;
		}
		else
		{//���ϻ���
			while(m_pCurNode->parentNode!= NULL)
			{
				//if(m_pCurNode->parentNode != m_pDocument ->documentElement )
				if(m_pCurNode->parentNode != m_pDocument ->documentElement &&
					m_pCurNode->parentNode->nodeType != MSXML2::NODE_DOCUMENT)
				{//��û�л��ݵ����ڵ�
					m_pCurNode= m_pCurNode->parentNode ; 
				}
				else
				{//�����ݵ����ڵ�ʱ,ֱ��ָ����һ���ڵ�,�������жϽڵ��Ƿ���Ч
					m_pCurNode = m_pCurNode->nextSibling ;
					break;
				}
				if(m_pCurNode->nextSibling!= NULL)
				{//����ýڵ�Ĵ�����Ч�Ĳ��нڵ�,��ôָ��ò��нڵ�
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
**	��������	: ReadAttribute
**	����:	��ȡ��ǰ�ڵ����һ������
**	�������	:	��
**	�������	: 	��
**	����ֵ	:		���Ϊ��,��ô�Ѿ��ﵽ��������ĩβ
**	��ע:			ѭ�����ÿ��Ա���ĳ���ڵ����������
**	�쳣:			��
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
������CXmlWriter��ReleaseAll()
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
**	��������	: FindSingleNode
**	����:	�����ض��ڵ����Ʋ��ҽڵ�ֵ
**	�������	:	
**	Cstring strNodeName : Ҫ��ѯ�Ľڵ�����
**	�������	: 
**	Cstring : �ýڵ��ֵ
****************************************************************/
_bstr_t CXmlReader::FindSingleNode(_bstr_t nodename)
{
	nodename = L"/" + nodename ;
	if(m_pDocument)
	{//�����ĵ������selectSingleNode����ֱ���ҵ��ڵ�ֵ
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
	{//�����ĵ������selectSingleNode����ֱ���ҵ��ڵ�ֵ
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

/*��ȡ��ǰ�ڵ����ӽڵ�����
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
**	��������	: ReadAttribute
**	����		: ��ȡ��ǰ�ڵ����һ������
**	�������	:	��
**	�������	: 	��
**	����ֵ	:		���Ϊ��,��ô�Ѿ��ﵽ��������ĩβ
**	��ע:			ѭ�����ÿ��Ա���ĳ���ڵ����������
**	�쳣:			��
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
��ȡ�ýڵ��µ��ӽڵ��б�
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