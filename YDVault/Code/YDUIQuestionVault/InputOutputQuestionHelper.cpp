#include "StdAfx.h"
#include "InputOutputQuestionHelper.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YDObjectRef.h"
#include "../ObjHelper/FactorInfoHelper.h"

// word format
#define wdFormatDocument                      0
#define wdFormatDocument97                    0
#define wdFormatTemplate                      1
#define wdFormatTemplate97                    1
#define wdFormatText                          2
#define wdFormatTextLineBreaks                3
#define wdFormatDOSText                       4
#define wdFormatDOSTextLineBreaks             5
#define wdFormatRTF                           6
#define wdFormatUnicodeText                   7
#define wdFormatEncodedText                   7
#define wdFormatHTML                          8
#define wdFormatWebArchive                    9
#define wdFormatFilteredHTML                 10
#define wdFormatXML                          11
#define wdFormatXMLDocument                  12
#define wdFormatXMLDocumentMacroEnabled      13
#define wdFormatXMLTemplate                  14
#define wdFormatXMLTemplateMacroEnabled      15
#define wdFormatDocumentDefault              16
#define wdFormatPDF                          17
#define wdFormatXPS                          18
#define wdFormatFlatXML                      19
#define wdFormatFlatXMLMacroEnabled          20
#define wdFormatFlatXMLTemplate              21
#define wdFormatFlatXMLTemplateMacroEnabled  22


CInputOutputQuestionHelper::CInputOutputQuestionHelper(CYDObjectRef* _pVault,CYDObjectRef* _pType)
{
	m_pVault = _pVault;
	m_pType = _pType;
	m_bCreateWordSuc = FALSE;
}


CInputOutputQuestionHelper::~CInputOutputQuestionHelper(void)
{
	if(m_bCreateWordSuc)
	{
		 COleVariant vTrue((short)TRUE),    
               vFalse((short)FALSE);
		 m_oWordApp.Quit(vFalse,    // SaveChanges.
				 vTrue,            // OriginalFormat.
				 vFalse            // RouteDocument.
				 );
		//释放
		m_paragraphs.ReleaseDispatch();
		m_oSel.ReleaseDispatch();
		m_oDoc.ReleaseDispatch();
		m_oDocs.ReleaseDispatch();
		m_oWordApp.ReleaseDispatch();
	}
	CListAutoClean<CYDObjectRef> clr(m_lstFactorInfo);
}

HRESULT CInputOutputQuestionHelper::CreateWord(CString _strFile)
{
	HRESULT hr = E_FAIL;
	ASSERT(!m_bCreateWordSuc);
	COleVariant vTrue((short)TRUE), 
                  vFalse((short)FALSE), 
                  vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	
	CStringArray aryText;

		// 首先建立一个word实例对象 
	if (!m_oWordApp.CreateDispatch(TEXT("Word.Application")) )
	{
		DWORD dwErr = GetLastError();
		AfxMessageBox(_T("请安装Word程序！"));
		return E_FAIL; // 一定要返回，否则程序崩溃 
	}

	// 显示此对象文件 ，您也可以不调用此句，不显示对象文件 
	m_oWordApp.SetVisible(FALSE);

	// 创建文档 
	m_oDocs = m_oWordApp.GetDocuments();

	// 打开一个新文档，将e:\\BuildJobEdit.rtf文件的内容添加到新文档中 
	CComVariant tpl(_strFile), NewTemplate(false), DocType(0), Visble; 
	m_oDoc = m_oDocs.Add(&tpl,&NewTemplate,&DocType,&Visble); 
      
	// 选中文档中的所有内容 
	m_oSel = m_oWordApp.GetSelection(); 
	m_oSel.WholeStory();// 此句必须有，否则后续代码无效。

	// 得到所有段 
	m_paragraphs = m_oSel.GetParagraphs(); 


	m_bCreateWordSuc = TRUE;
	return S_OK;
}

HRESULT CInputOutputQuestionHelper::CreateBlankWord()
{
	HRESULT hr = E_FAIL;
	ASSERT(!m_bCreateWordSuc);
	COleVariant vTrue((short)TRUE), 
		vFalse((short)FALSE), 
		vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);


	CStringArray aryText;

	// 首先建立一个word实例对象 
	if (!m_oWordApp.CreateDispatch(TEXT("Word.Application")) )
	{
		DWORD dwErr = GetLastError();
		AfxMessageBox(_T("请安装Word程序！"));
		return E_FAIL; // 一定要返回，否则程序崩溃 
	}

	// 显示此对象文件 ，您也可以不调用此句，不显示对象文件 
	m_oWordApp.SetVisible(FALSE);

	// 创建文档 
	m_oDocs = m_oWordApp.GetDocuments();

	// 打开一个新文档，将e:\\BuildJobEdit.rtf文件的内容添加到新文档中 
	CComVariant tpl(_T("Normal")), NewTemplate(false), DocType(0), Visble; 
	m_oDoc = m_oDocs.Add(&tpl,&NewTemplate,&DocType,&Visble); 

	// 选中文档中的所有内容 
	m_oSel = m_oWordApp.GetSelection(); 
	m_oSel.WholeStory();// 此句必须有，否则后续代码无效。

	// 得到所有段 
	m_paragraphs = m_oSel.GetParagraphs(); 


	m_bCreateWordSuc = TRUE;
	return S_OK;
}

HRESULT CInputOutputQuestionHelper::SaveWord(CString _strFile)
{
	HRESULT hr = E_FAIL;
/*	CComVariant valFile(_strFile);
	CComVariant FileFormat(0);
	CComVariant LockComments(false);
	CComVariant Password(L"");
	CComVariant AddToRecentFiles(false);
	CComVariant WritePassword(L"");
	CComVariant ReadOnlyRecommended(false);
	CComVariant EmbedTrueTypeFonts(false);
	CComVariant SaveNativePictureFormat(false);
	CComVariant SaveFormsData(false);
	CComVariant SaveAsAOCELetter(false);
	CComVariant valFalse(false);
	m_oDoc.SaveAs(&valFile,&FileFormat,&LockComments, &Password, &AddToRecentFiles,
		&WritePassword, &ReadOnlyRecommended, &EmbedTrueTypeFonts,
		&SaveNativePictureFormat, &SaveFormsData,
		&SaveAsAOCELetter,&valFile,&valFalse,&valFalse,&valFalse,&valFalse);*/
	COleVariant vTrue((short)TRUE);
	COleVariant vFalse((short)FALSE);
	COleVariant vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);


	//保存word文档
	_Document oActiveDoc = m_oWordApp.GetActiveDocument();

	COleVariant FileName(_strFile);	//文件名
	COleVariant FileFormat((short)wdFormatRTF);
	COleVariant LockComments((short)FALSE);
	COleVariant Password(_T(""));
	COleVariant AddToRecentFiles((short)TRUE);
	COleVariant WritePassword(_T(""));
	COleVariant ReadOnlyRecommended((short)FALSE);
	COleVariant EmbedTrueTypeFonts((short)FALSE);
	COleVariant SaveNativePictureFormat((short)FALSE);
	COleVariant SaveFormsData((short)FALSE);
	COleVariant SaveAsAOCELetter((short)FALSE);

	oActiveDoc.SaveAs(&FileName,&FileFormat,&LockComments,&Password,
		&AddToRecentFiles,&WritePassword,&ReadOnlyRecommended,
		&EmbedTrueTypeFonts,&SaveNativePictureFormat,&SaveFormsData,
		&SaveAsAOCELetter, vFalse, vFalse, vFalse, vFalse, vFalse);
	return S_OK;
}

HRESULT CInputOutputQuestionHelper::IsFactorName(CString _strName,VARIANT_BOOL* _bIs)
{
	HRESULT hr = E_FAIL;
	*_bIs = VARIANT_FALSE;
	if(m_lstFactorInfo.size() <= 0)
	{
		//读取数据库得到当前题型的指标名
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CFactorInfoHelper helper;
		hr = helper.GetFactorInfoByVaultQType(pDB,m_pVault,m_pType,&m_lstFactorInfo);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = m_lstFactorInfo.begin();
		itr != m_lstFactorInfo.end();++itr)
	{
		CString strItrName ;
		hr = (*itr)->GetPropVal(FIELD_YDFACTORINFOITEM_FACTORNAME,strItrName);
		if(FAILED(hr))
		{
			return hr;
		}
		if(strItrName.CompareNoCase(_strName) == 0)
		{
			*_bIs = VARIANT_TRUE;
			break;
		}
	}
	return S_OK;
}