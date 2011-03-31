// WordAppHelper.cpp: implementation of the CWordAppHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WordAppHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWordAppHelper::CWordAppHelper()
{
	m_bCreate = FALSE;
}

CWordAppHelper::~CWordAppHelper()
{
	if(m_bCreate)
	{
		 COleVariant vTrue((short)TRUE),    
               vFalse((short)FALSE);
		 m_wdApp.Quit(vFalse,    // SaveChanges.
				 vTrue,            // OriginalFormat.
				 vFalse            // RouteDocument.
				 );
		//释放内存申请资源
		m_wdSel.ReleaseDispatch();
		//m_wdFt.ReleaseDispatch();
		m_wdDoc.ReleaseDispatch();
		m_wdDocs.ReleaseDispatch();
		m_wdApp.ReleaseDispatch();
	}
}

HRESULT CWordAppHelper::Create()
{
	HRESULT hr = E_FAIL;
	
	COleVariant vTrue((short)TRUE),    
               vFalse((short)FALSE);


	if (FALSE == m_wdApp.CreateDispatch(_T("Word.Application")))
    {
        AfxMessageBox(_T("Application创建失败，请确保安装了word 2000或以上版本!"), MB_OK|MB_ICONWARNING);
        return E_FAIL;
    }
	m_wdDocs.AttachDispatch(m_wdApp.GetDocuments());
    if (!m_wdDocs.m_lpDispatch) 
    {
        AfxMessageBox(_T("Documents创建失败!"), MB_OK|MB_ICONWARNING);
        return E_FAIL;
    }
	COleVariant varTrue(short(1),VT_BOOL),vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
    CComVariant Template(_T(""));    //没有使用WORD的文档模板
    CComVariant NewTemplate(false),DocumentType(0),Visible;

    m_wdDocs.Add(&Template,&NewTemplate,&DocumentType,&Visible);    

    //得到document变量
    m_wdDoc = m_wdApp.GetActiveDocument();
    if (!m_wdDoc.m_lpDispatch) 
    {
        AfxMessageBox(_T("Document获取失败!"), MB_OK|MB_ICONWARNING);
        return E_FAIL;
    }

	m_wdSel = m_wdApp.GetSelection();
    if (!m_wdSel.m_lpDispatch) 
    {
        AfxMessageBox(_T("Select获取失败!"), MB_OK|MB_ICONWARNING);
        return E_FAIL;
    }
	m_bCreate = TRUE;
	return S_OK;
}

HRESULT CWordAppHelper::Write(CString _str)
{
	HRESULT hr = E_FAIL;
	//m_wdSel.MoveEnd()
	m_wdSel.TypeText(_str);
	return S_OK;
}

HRESULT CWordAppHelper::StartUnline()//开始下划线
{
	HRESULT hr = E_FAIL;
	_Font font; 
	font.AttachDispatch(m_wdSel.GetFont());
	
	font.SetUnderlineColor(0xFF000000);//wdColorAutomatic
	font.SetUnderline(1);//wdUnderlineSingle
	font.ReleaseDispatch();
// 	m_wdSel.Font.UnderlineColor = wdColorAutomatic
// 
	return S_OK;
}
HRESULT CWordAppHelper::EndUnline()//结束下划线
{
	HRESULT hr = E_FAIL;
	_Font font; 
	font.AttachDispatch(m_wdSel.GetFont());
	font.SetUnderlineColor(0xFF000000);//wdColorAutomatic
	font.SetUnderline(0);//wdUnderlineNone
	font.ReleaseDispatch();
	return S_OK;
}

HRESULT CWordAppHelper::Save(CString _strFile)
{
	HRESULT hr = E_FAIL;
	COleVariant vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	COleVariant vFileName(_strFile);
	m_wdDoc.SaveAs(
		vFileName,    //VARIANT* FileName
		vOptional,    //VARIANT* FileFormat
		vOptional,    //VARIANT* LockComments
		vOptional,    //VARIANT* Password
		vOptional,    //VARIANT* AddToRecentFiles
		vOptional,    //VARIANT* WritePassword
		vOptional,    //VARIANT* ReadOnlyRecommended
		vOptional,    //VARIANT* EmbedTrueTypeFonts
		vOptional,    //VARIANT* SaveNativePictureFormat
		vOptional,    //VARIANT* SaveFormsData
		vOptional,    //VARIANT* SaveAsAOCELetter
		vOptional,
		vOptional,
		vOptional,vOptional,vOptional
		);
	return S_OK;
}

HRESULT CWordAppHelper::AddOleObject(CString _strFile)
{
	HRESULT hr = E_FAIL;
	m_wdSel.TypeParagraph();
	InlineShapes MyInlineShapes = m_wdSel.GetInlineShapes();
	CComVariant valClassType(L"Package");
	CComVariant valLinkFile(_strFile);
	CComVariant valbLinkToFile(FALSE);
	CComVariant valbDisplayAsIcon(FALSE);
	COleVariant vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	MyInlineShapes.AddOLEObject(&valClassType,&valLinkFile,
								&valbLinkToFile,
								&valbDisplayAsIcon,
								&vOptional,
								&vOptional,
								&vOptional,
								&vOptional);
	MyInlineShapes.ReleaseDispatch();
	m_wdSel.TypeParagraph();
	return S_OK;
}