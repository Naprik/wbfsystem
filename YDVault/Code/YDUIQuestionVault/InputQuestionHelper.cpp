#include "StdAfx.h"
#include "InputQuestionHelper.h"


CInputQuestionHelper::CInputQuestionHelper(QTYPE qType)
{
	m_qType = qType;
	m_bCreateWordSuc = FALSE;
}


CInputQuestionHelper::~CInputQuestionHelper(void)
{
	if(m_bCreateWordSuc)
	{
		//释放
		m_paragraphs.ReleaseDispatch();
		m_oDoc.ReleaseDispatch();
		m_oDocs.ReleaseDispatch();
		m_oSel.ReleaseDispatch();
		m_oWordApp.ReleaseDispatch();
	}
}

HRESULT CInputQuestionHelper::CreateWord(CString _strFile)
{
	HRESULT hr = E_FAIL;
	ASSERT(!m_bCreateWordSuc);
	COleVariant vTrue((short)TRUE), 
                  vFalse((short)FALSE), 
                  vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	
	CStringArray aryText;

		// 首先建立一个word实例对象 
	if (!m_oWordApp.CreateDispatch(TEXT("Word.Application")) ){
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