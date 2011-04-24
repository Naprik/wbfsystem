#include "StdAfx.h"
#include "InputQuestionHelper.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YDObjectRef.h"
#include "../ObjHelper/FactorInfoHelper.h"


CInputQuestionHelper::CInputQuestionHelper(OBJID _IDVault,OBJID _IDType)
{
	m_IDVault = _IDVault;
	m_IDType = _IDType;
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
	CListAutoClean<CYDObjectRef> clr(m_lstFactorInfo);
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

HRESULT CInputQuestionHelper::IsFactorName(CString _strName,VARIANT_BOOL* _bIs)
{
	HRESULT hr = E_FAIL;
	*_bIs = VARIANT_FALSE;
	if(m_lstFactorInfo.size() <= 0)
	{
		//读取数据库得到当前题型的指标名
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CFactorInfoHelper helper;
		hr = helper.GetFactorInfoByVaultQType(pDB,m_IDVault,m_IDType,&m_lstFactorInfo);
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