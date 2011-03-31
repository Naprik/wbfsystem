#include "StdAfx.h"
#include "ListXlsInput.h"
#include "../Include\YDVaultError.h"
#include "../Base\DataHandler.h"

CListXlsInput::CListXlsInput(CString _strXlsFile)
{
	m_strXlsFile = _strXlsFile;
	m_bOutExcel = FALSE;
}

CListXlsInput::~CListXlsInput(void)
{
	if(m_bOutExcel)
	{
		//要将Excel关闭
		m_wbsMyBooks.ReleaseDispatch();
		m_ExcelApp.ReleaseDispatch(); 
	}
}

HRESULT CListXlsInput::Read(std::list<std::list<std::pair<CString,CString> > > *_pList)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pList);
	if(!::PathFileExists(m_strXlsFile))
	{
		CString strMsg;
		strMsg.Format(_T("%s文件不存在，无法读取！"),m_strXlsFile);
		AfxMessageBox(strMsg);
		return S_FALSE;
	}
	hr = InitExcel();
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t vtfalse(true);
	m_wbsMyBooks.Open(m_strXlsFile,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing,vtMissing);
	//m_wbsMyBooks.Add(vtMissing);
	_Workbook workBook = m_wbsMyBooks.GetItem(_variant_t(long(1)));

	Worksheets wssMysheets = workBook.GetSheets();
	_Worksheet wsMysheet = wssMysheets.GetItem(_variant_t((long)1));
	//读取列表头信息
	hr = InputColumInfo(wsMysheet);
	if(FAILED(hr))
	{
		return hr;
	}
	//读取列表内容信息
	hr = InputListContentInfo(wsMysheet,_pList);
	if(FAILED(hr))
	{
		return hr;
	}
	workBook.Close(vtfalse,vtMissing,vtMissing);
	wsMysheet.ReleaseDispatch();
	wssMysheets.ReleaseDispatch();
	workBook.ReleaseDispatch();

	return S_OK;
}

HRESULT CListXlsInput::InitExcel()
{
	HRESULT hr = E_FAIL;
	if(m_bOutExcel)
	{
		return S_FALSE;
	}
	m_ExcelApp.CreateDispatch(_T("Excel.Application"));
	if (m_ExcelApp == NULL)
	{
		//	CString strMsg;
		//			strMsg.LoadString(IDS_PDEMBOM_EXCEL_ERROR);
		//			AfxMessageBox(strMsg,MB_OK|MB_ICONINFORMATION);
		//			return false;


		return E_HRESULT_CREATE_EXCEL_ERROR;
	}
	m_wbsMyBooks = m_ExcelApp.GetWorkbooks();
	m_bOutExcel = TRUE;
	return S_OK;
}

HRESULT CListXlsInput::InputColumInfo(_Worksheet& _wsMysheet)
{
	HRESULT hr = E_FAIL;
	for(int i = 0; i < 26;i++)
	{
		TCHAR ch = _T('A') + i;
		CString strLocate;
		strLocate.Format(_T("%c1:%c1"),ch,ch);
		ExcelRange rg = _wsMysheet.GetRange(_variant_t(strLocate),vtMissing);
		COleVariant  val = rg.GetValue();
		rg.ReleaseDispatch();
		CString strVal = CDataHandler::VariantToString(val);
		if(strVal.IsEmpty())
		{
			break;
		}
		m_lstColumName.push_back(strVal);
	}
	return S_OK;
}
//读取列表内容信息
HRESULT CListXlsInput::InputListContentInfo(_Worksheet& _wsMysheet,
											std::list<std::list<std::pair<CString,CString> > > *_pList)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_lstColumName.size() > 0);
	int iRow = 2;
	while(TRUE)
	{
		BOOL bAllEmpty = TRUE;
		int iCol = 0;
		std::list<std::pair<CString,CString> >lst;
		for(std::list<CString>::const_iterator itr = m_lstColumName.begin();
			itr != m_lstColumName.end();++itr,iCol++)
		{
			TCHAR ch = _T('A') + iCol;
			CString strLocate;
			strLocate.Format(_T("%c%d"),ch,iRow);
			ExcelRange rg = _wsMysheet.GetRange(_variant_t(strLocate),vtMissing);
			_variant_t val = rg.GetValue();
			rg.ReleaseDispatch();
			CString strVal = CDataHandler::VariantToString(val);
			if(!strVal.IsEmpty())
			{
				bAllEmpty = FALSE;
			}
			lst.push_back(std::make_pair((*itr),strVal));
		}
		if(bAllEmpty)
		{
			break;
		}
		else
		{
			_pList->push_back(lst);
		}
		iRow++;
	}
	return S_OK;
}