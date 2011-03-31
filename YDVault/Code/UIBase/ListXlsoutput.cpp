#include "StdAfx.h"
#include "ListXlsoutput.h"
#include "../Include\YDVaultError.h"


CListXlsoutput::CListXlsoutput(CListCtrl* _pList)
{
	m_pListCtrl = _pList;
	m_bOutExcel = FALSE;
}

CListXlsoutput::~CListXlsoutput(void)
{
	if(m_bOutExcel)
	{
		//要将Excel关闭
		m_wbsMyBooks.ReleaseDispatch();
		m_ExcelApp.ReleaseDispatch(); 
	}
}

HRESULT CListXlsoutput::InitExcel()
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

HRESULT CListXlsoutput::OutputExcel(BOOL _bOpen/* = TRUE*/)
{
	HRESULT hr = E_FAIL;
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Excle文件(*.xls)|*.xls|所有文件(*.*)|*.*||"));
	if(dlg.DoModal() != IDOK)
	{
		return S_FALSE;
	}
	CString strFileName = dlg.GetPathName();
	if(::PathFileExists(strFileName))
	{
		BOOL bDelete = ::DeleteFile(strFileName);
		if(!bDelete)
		{
			CString strMsg;
			strMsg.Format(_T("%s文件正在被使用，不能替换！"),strFileName);
			AfxMessageBox(strMsg);
			return S_FALSE;
		}
	}
	hr = InitExcel();
	if(FAILED(hr))
	{
		return hr;
	}
	_variant_t vtfalse(true);
	m_wbsMyBooks.Add(vtMissing);
	_Workbook workBook = m_wbsMyBooks.GetItem(_variant_t(long(1)));
	
	Worksheets wssMysheets = workBook.GetSheets();
	_Worksheet wsMysheet = wssMysheets.GetItem(_variant_t((long)1));
	//输出标题栏信息
	hr = OutputCapitonInfo(wsMysheet);
	if(FAILED(hr))
	{
		return hr;
	}
	//输出列表头信息
	hr = OutputColumInfo(wsMysheet);
	if(FAILED(hr))
	{
		return hr;
	}
	//输出列表内容信息
	hr = OutputListContentInfo(wsMysheet);
	if(FAILED(hr))
	{
		return hr;
	}
	workBook.SaveAs(_variant_t(strFileName),
					vtMissing,
					vtMissing,
					vtMissing,
					vtMissing,
					vtMissing,
					0,
					vtMissing,
					vtMissing,
					vtMissing,
					vtMissing);
	workBook.Close(vtfalse,vtMissing,vtMissing);
	wsMysheet.ReleaseDispatch();
	wssMysheets.ReleaseDispatch();
	workBook.ReleaseDispatch();

	if(_bOpen)
	{
		//ShellExecute(NULL,_T("open"),strFileName,NULL,NULL,SW_SHOW);
	}
	return S_OK;
}

HRESULT CListXlsoutput::OutputColumInfo(_Worksheet& _wsMysheet)
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pListCtrl);
	CHeaderCtrl* pHeadCtrl = m_pListCtrl->GetHeaderCtrl();
	ASSERT(pHeadCtrl);
	HDITEM hdi;
	TCHAR  lpBuffer[MAX_PATH];
	bool   fFound = false;

	hdi.mask = HDI_TEXT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = MAX_PATH;

	for (int i =0;i < pHeadCtrl->GetItemCount();i++)
	{
		pHeadCtrl->GetItem(i, &hdi);
		CString strItemText = hdi.pszText;
		CString strColName;
		hr = GetColumnNameByIndex(i,strColName);
		if(FAILED(hr))
		{
			return hr;
		}
		int iRow = 1;
		if(m_lstCapiton.size() > 0)
		{
			iRow += 2;
		}
		CString strLocate;
		strLocate.Format(_T("%s%d"),strColName,iRow);
		ExcelRange rg = _wsMysheet.GetRange(_variant_t(strLocate),vtMissing);
		rg.SetItem(_variant_t((short)1),_variant_t((short)1),_variant_t(strItemText));
		rg.ReleaseDispatch();
	}

	return S_OK;
}

HRESULT CListXlsoutput::GetColumnNameByIndex(int _index,CString& _strCol)
{
	HRESULT hr = E_FAIL;
	TCHAR ch = _T('A');
	ch += _index;
	_strCol = ch;
	return S_OK;
}

HRESULT CListXlsoutput::OutputListContentInfo(_Worksheet& _wsMysheet)
{
	HRESULT hr = E_FAIL;
	CHeaderCtrl* pHeadCtrl = m_pListCtrl->GetHeaderCtrl();
	ASSERT(pHeadCtrl);
	int iColCount = pHeadCtrl->GetItemCount();
	for(int i = 0; i < m_pListCtrl->GetItemCount();i++)
	{
		for(int iCol = 0; iCol < iColCount;iCol++)
		{
			CString strItemText = m_pListCtrl->GetItemText(i,iCol);
			CString strColName;
			hr = GetColumnNameByIndex(iCol,strColName);
			if(FAILED(hr))
			{
				return hr;
			}
			CString strLocate;
			int iRowOffSet = 2;
			if(m_lstCapiton.size() > 0)
			{
				//标题栏信息占据两行
				iRowOffSet += 2;
			}
			strLocate.Format(_T("%s%d"),strColName,i+iRowOffSet);
			ExcelRange rg = _wsMysheet.GetRange(_variant_t(strLocate),vtMissing);
			rg.SetItem(_variant_t((short)1),_variant_t((short)1),_variant_t(strItemText));
			rg.ReleaseDispatch();
		}
	}
	return S_OK;
}

HRESULT CListXlsoutput::OutputCapitonInfo(_Worksheet& _wsMysheet)
{
	HRESULT hr = E_FAIL;
	int iCol = 0;
	for(std::list<std::pair<CString,CString> >::const_iterator itr = m_lstCapiton.begin();
		itr != m_lstCapiton.end();++itr,++iCol)
	{
		CString strColName;
		hr = GetColumnNameByIndex(iCol,strColName);
		if(FAILED(hr))
		{
			return hr;
		}
		{
			CString strLocate;
			strLocate.Format(_T("%s%d"),strColName,1);
			ExcelRange rg = _wsMysheet.GetRange(_variant_t(strLocate),vtMissing);
			_variant_t val = _variant_t((*itr).first);
			rg.SetItem(_variant_t((short)1),_variant_t((short)1),val);
			rg.ReleaseDispatch();
		}
		{
			CString strLocate;
			strLocate.Format(_T("%s%d"),strColName,2);
			ExcelRange rg = _wsMysheet.GetRange(_variant_t(strLocate),vtMissing);
			_variant_t val = _variant_t((*itr).second);
			rg.SetItem(_variant_t((short)1),_variant_t((short)1),val);
			rg.ReleaseDispatch();
		}
	}
	return S_OK;
}