
/******************************************************************
** 文件名:	xml.h


** 目的及主要功能:	该类主要用来顺序写入XML字节流的类

** 创建人:		
** 创建日期:	
** 修改人:		
** 修改日期:
** 描 述:		
** 版 本:		
** 说明：按顺序读取属性时得到的属性名都转换为小写字母
** 使用举例：
	_bstr_t filename ;//xml文件名
	_bstr_t xmlstring;//xml串
	CXmlReader reader ;
	reader.LoadFile(filename);//从文件读取
	reader.LoadXml(xmlstring);//从字符串读取
	CXmlNode root;
	reader.GetRootNode(root);//获取根节点
	CXmlNodeList nodes;
	root.GetChilds(nodes);//获取子节点
	_bstr_t attrVal(_T(""));
	root.ReadAttribute(_T("attrname"),attrVal);//根据属性名读取属性
	while(root.ReadAttribute())//按顺序读取所有属性
	{
	_bstr_t attrname = root.AttributeName;
	_bstr_t attrval = root.AttributeValue;
	}
	root.Value;//节点的内容
******************************************************************/

#pragma once

//#include <afx.h>
#include "xmlexception.h"

#include "../include/msxml4.tlh"
class CXmlNodeList ;
//该类用来表示一个节点
class BASE_DLL_API  CXmlNode 
{
public:
	CXmlNode ();
	virtual ~CXmlNode ();

	void	SetDOMNode(MSXML2::IXMLDOMNodePtr	pDOMNode ) ;
	BOOL	ReadAttribute() ;
	HRESULT ReadAttribute(BSTR atrname,BSTR* atrvalue);
	HRESULT ReadAttributeByNoCase(BSTR atrname,BSTR* atrvalue);
	HRESULT AddAttribute(BSTR name,BSTR value);
	CXmlNode& operator=(const CXmlNode& node);
	BOOL GetChilds(CXmlNodeList &lstChilds);
	int GetChildCount();
	BOOL GetChildeNode(int nIndex,CXmlNode& node);
	_bstr_t ToString();
	BSTR GetNodeName(){return (BSTR)Name;}
	BSTR GetNodeValue(){return (BSTR)Value;}
	BSTR GetAttributeName(){return (BSTR)AttributeName;}
	BSTR GetAttributeValue(){return (BSTR)AttributeValue;}
public:
	MSXML2::DOMNodeType		NodeType ;
	short			AttributeCount ;
private:
	_bstr_t	AttributeName , AttributeValue ;
	_bstr_t	Name , Value ;
	MSXML2::IXMLDOMNodePtr	m_pDOMNode ;
	MSXML2::IXMLDOMNamedNodeMapPtr m_pAttribute ;
};


//该类用来表示一个节点序列
class BASE_DLL_API  CXmlNodeList
{
public:
	CXmlNodeList();
	virtual ~CXmlNodeList();

	void	SetDOMNodeList(MSXML2::IXMLDOMNodeList*	pDOMNodeList );
	long	GetLength() ;
	BOOL	GetNode(long iIndex , CXmlNode& xmlNode ) ;
private:
	MSXML2::IXMLDOMNodeListPtr	m_pDOMNodeList ;
};

class BASE_DLL_API CXmlWriter  
{
public:
	CXmlWriter();
	virtual ~CXmlWriter();
	
	void	WriteStartDocument();
	void	WriteEndDocument();
	void	WriteComment(BSTR strComment);
	void	WriteStartElement(BSTR strElement);
	void	WriteString(BSTR strText);
	void	WriteInnerXml(BSTR strXml);
	void	WriteEndElement();
	void	WriteAttributeString(BSTR strAttrName , BSTR strAttrValue);
	_bstr_t	ToString();
	void	Save(BSTR strFileName);
	void WriteProcessingInstruction(BSTR s1,const BSTR s2);

private:
	void	Close() ;	
	void	ReleaseAll(MSXML2::IXMLDOMNodePtr pParent) ;
private:
	MSXML2::IXMLDOMDocumentPtr	m_pDocument ;
	MSXML2::IXMLDOMElementPtr	m_pCurElement ;
};

class BASE_DLL_API  CXmlReader  
{
public:
	CXmlReader();
	virtual ~CXmlReader();

	void	Close() ;
	void	LoadFile(BSTR filename) ;
	void	LoadXml(BSTR	bstrXml);
	BOOL	Read();
	BOOL	ReadNodeListByTagName(BSTR strTagName , CXmlNodeList &Node);
	BOOL	FindSingleNode(BSTR nodename,CXmlNode& node);
	BOOL	GetRootNode(CXmlNode& node);
	BSTR GetNodeName(){return (BSTR)Name;}
	BSTR GetNodeValue(){return (BSTR)Value;}
	BSTR GetAttrbuteName(){return (BSTR)AttributeName;}
	BSTR GetAttrbuteValue(){return (BSTR)AttributeValue;}
	_bstr_t	FindSingleNode(_bstr_t strNodeName);
	BOOL	ReadAttribute() ;
	void	ReleaseAll(MSXML2::IXMLDOMNodePtr pParent) ;

public:
	MSXML2::DOMNodeType		NodeType ;
	short			AttributeCount ;
private:
	MSXML2::IXMLDOMDocumentPtr	m_pDocument ;
	MSXML2::IXMLDOMNodePtr		m_pCurNode ;
	HRESULT				Init();

	MSXML2::IXMLDOMNamedNodeMapPtr m_pAttribute ;
	_bstr_t			Name ;
	_bstr_t			Value ;
	_bstr_t			AttributeName ;
	_bstr_t			AttributeValue ;

};
/*字符串到长整型*/
long __stdcall S2L(LPCTSTR sVal);

/*长整型到字符串*/
CString __stdcall L2S(long iVal);
long __stdcall BSTR2L(BSTR sVal);
BSTR __stdcall L2BSTR(long iVal);
VARIANT_BOOL __stdcall BSTR2BOOL(BSTR str);
BSTR __stdcall BOOL2BSTR(VARIANT_BOOL b);
void MakeUpper(_bstr_t& bstrval);
void MakeLower(_bstr_t& bstrval);
