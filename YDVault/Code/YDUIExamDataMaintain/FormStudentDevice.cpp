// FormPaper.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FormStudentDevice.h"
#include "DlgStudentDevice.h"

#include "../YDExamObjRef/YDDevice.h"
#include "../ObjRef/YDObjectRef.h"
#include "../base/AutoClean.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../Base/DataHandler.h"
#include "../DBBase/DBTransactionRef.h"
#include "../UIBase\ListXlsoutput.h"
#include "DlgInputDeviePreview.h"
#include "../UIBase/ListXlsInput.h"

// CFormStudentDevice

IMPLEMENT_DYNCREATE(CFormStudentDevice, CYdFormView)

CFormStudentDevice::CFormStudentDevice()
	: CYdFormView(CFormStudentDevice::IDD)
	, m_utotalNumber(0)
	, m_uPageSize(0)
	, m_uPage(0)
{

}

CFormStudentDevice::~CFormStudentDevice()
{
	CListAutoClean<CYDObjectRef> clr(m_lstDeviceCache);
	CListAutoClean<CPropQueryContidition> clr1(m_lstPropQuery);
}

void CFormStudentDevice::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DM_DEVICE, m_DeviceListCtrl);
	DDX_Text(pDX, IDC_EDIT_QDEVICE_QUERY_TOTAL, m_utotalNumber);
	DDX_Text(pDX, IDC_EDIT_QDEVICE_QUERY_PAGENUM, m_uPageSize);
}

BEGIN_MESSAGE_MAP(CFormStudentDevice, CYdFormView)
	ON_BN_CLICKED(IDC_BTN_DM_DEVICE_ADD, &CFormStudentDevice::OnBnClickedBtnDmDeviceAdd)
	ON_BN_CLICKED(IDC_BTN_DM_DEVICE_EDIT, &CFormStudentDevice::OnBnClickedBtnDmDeviceEdit)
	ON_BN_CLICKED(IDC_BTN_DM_DEVICE_DEL, &CFormStudentDevice::OnBnClickedBtnDmDeviceDel)
	ON_BN_CLICKED(IDC_BTN_DM_DEVICE_SEARCH, &CFormStudentDevice::OnBnClickedBtnDmDeviceSearch)
	ON_BN_CLICKED(IDC_BTN_DM_QDEVICE_BEGIN, &CFormStudentDevice::OnBnClickedBtnDmQDeviceBegin)
	ON_BN_CLICKED(IDC_BTN_DM_QDEVICE_PREV, &CFormStudentDevice::OnBnClickedBtnDmQDevicePrev)
	ON_BN_CLICKED(IDC_BTN_DM_QDEVICE_NEXT, &CFormStudentDevice::OnBnClickedBtnDmQDeviceNext)
	ON_BN_CLICKED(IDC_BTN_DM_QDEVICE_END, &CFormStudentDevice::OnBnClickedBtnDmQDeviceEnd)
	ON_BN_CLICKED(IDC_BTN_DM_DEVICE_OUTPUT, &CFormStudentDevice::OnBnClickedBtnDmDeviceOutput)
	ON_BN_CLICKED(IDC_BTN_DM_DEVICE_INPUT, &CFormStudentDevice::OnBnClickedBtnDmDeviceInput)
END_MESSAGE_MAP()


// CFormStudentDevice diagnostics

#ifdef _DEBUG
void CFormStudentDevice::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormStudentDevice::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormStudentDevice message handlers

void CFormStudentDevice::OnBnClickedBtnDmDeviceAdd()
{
	CDlgStudentDevice dlg(OP_NEW);

	if(dlg.DoModal() == IDOK)
	{
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CYDObjectRef* pRef = new CYDDevice(pDB);
		CComVariant varId(dlg.m_strCode);
		pRef->SetPropVal(FIELD_YDDEVICE_ID, &varId);
		CComVariant varName(dlg.m_strMac);
		pRef->SetPropVal(FIELD_YDDEVICE_MAC, &varName);

		CDBTransactionRef trans(pDB, TRUE);
		HRESULT hr = pRef->Save();
		if (FAILED(hr))
		{
			trans.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		hr = trans.Commit();
		if (FAILED(hr))
		{
			trans.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		AddItem(pRef);
	}

}

void CFormStudentDevice::OnBnClickedBtnDmDeviceEdit()
{
	HRESULT hr = E_FAIL;
	int nCount = m_DeviceListCtrl.GetSelectedCount();
	if(nCount != 1)
	{
		AfxMessageBox(_T("请在列表中选择一学生设备！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_DeviceListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_DeviceListCtrl.GetNextSelectedItem(pos);
	CYDObjectRef* pObj = (CYDObjectRef*)m_DeviceListCtrl.GetItemData(nItem);
	ASSERT(pObj != NULL);
	CComVariant valID;
	hr = pObj->GetPropVal(FIELD_YDDEVICE_ID, &valID);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strID = CDataHandler::VariantToString(valID);

	CComVariant valName;
	hr = pObj->GetPropVal(FIELD_YDDEVICE_MAC,&valName);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strName = CDataHandler::VariantToString(valName);

	
	CDlgStudentDevice	dlg(OP_EDIT, this);
	dlg.m_strCode = strID;
	dlg.m_strMac = strName;
	
	if(dlg.DoModal() == IDOK)
	{
		CYDDevice* pDeviceRef = (CYDDevice*)pObj;

		CComVariant valID(dlg.m_strCode);
		hr = pDeviceRef->SetPropVal(FIELD_YDDEVICE_ID,&valID);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CComVariant valName(dlg.m_strMac);
		hr = pDeviceRef->SetPropVal(FIELD_YDDEVICE_MAC, &valName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CDBTransactionRef transRef(pDB,TRUE);
		hr = pDeviceRef->Update();
		if(FAILED(hr))
		{
			transRef.Rollback();
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}

		hr = transRef.Commit();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		m_DeviceListCtrl.SetItemText(nItem,1,dlg.m_strCode);
		m_DeviceListCtrl.SetItemText(nItem,2,dlg.m_strMac);
	}
}

void CFormStudentDevice::OnBnClickedBtnDmDeviceDel()
{
	int nCount = m_DeviceListCtrl.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在列表中选择一学生设备！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_DeviceListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_DeviceListCtrl.GetNextSelectedItem(pos);
	CYDObjectRef* pObjRef = (CYDObjectRef*)m_DeviceListCtrl.GetItemData(nItem);
	ASSERT(pObjRef );
	if (AfxMessageBox(_T("确定要删除选择的学生设备吗？"), MB_OKCANCEL) != IDOK)
		return;	
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef TransRef(pDB,TRUE);
	hr = pObjRef->Remove();
	if(FAILED(hr))
	{
		TransRef.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	hr = TransRef.Commit();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	m_DeviceListCtrl.DeleteItem(nItem);
}

void CFormStudentDevice::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();

	//初始化查询列表
	CRect rectGrid;
	GetDlgItem(IDC_STATIC_DM_DEVICECON_GRID)->GetWindowRect(&rectGrid);
	ScreenToClient(&rectGrid);
	if (!m_QueryGrid.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rectGrid, this,
		-1))
	{
		return;
	}

	m_QueryGrid.EnableDragHeaderItems(FALSE);
	m_QueryGrid.EnableColumnAutoSize (TRUE);
	m_QueryGrid.EnableGroupByBox (FALSE);
	m_QueryGrid.SetWholeRowSel(FALSE);
	m_QueryGrid.EnableHeader (TRUE, 0);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_QueryGrid.SetColorTheme (colors);
	int iWidthGrid = rectGrid.Width();
	m_QueryGrid.InsertColumn(0, L"属性", iWidthGrid/4 - 2);
	m_QueryGrid.InsertColumn(1, L"操作", iWidthGrid/4);
	m_QueryGrid.InsertColumn(2, L"属性值", iWidthGrid/4);

	for (int iColumn = 0; iColumn < m_QueryGrid.GetColumnCount(); iColumn++)
	{
		m_QueryGrid.SetHeaderAlign(iColumn,HDF_CENTER);
		m_QueryGrid.SetColumnAlign(iColumn, HDF_LEFT);
	}

	CBCGPGridRow* pRow = m_QueryGrid.CreateRow(m_QueryGrid.GetColumnCount());
	pRow->GetItem(0)->SetValue(_T("设备编号"));
	pRow->GetItem(0)->Enable(FALSE);
	
	//pRow->GetItem(0)->Enable(FALSE);

	CBCGPGridItem* pItem = new CBCGPGridItem(_T(""));
	pItem->AddOption(_T("等于"));
	pItem->AddOption(_T("不等于"));
	pItem->AddOption(_T("包含"));

	pRow->ReplaceItem (1, pItem);
	pRow->ReplaceItem(2, new CBCGPGridItem(L""));
	m_QueryGrid.AddRow(pRow);

	pRow = m_QueryGrid.CreateRow(m_QueryGrid.GetColumnCount());
	pRow->GetItem(0)->SetValue(_T("蓝牙地址"));
	pRow->GetItem(0)->Enable(FALSE);

	//pRow->GetItem(0)->Enable(FALSE);

	pItem = new CBCGPGridItem(_T(""));
	pItem->AddOption(_T("等于"));
	pItem->AddOption(_T("不等于"));
	pItem->AddOption(_T("包含"));

	pRow->ReplaceItem (1, pItem);
	pRow->ReplaceItem(2, new CBCGPGridItem(L""));
	m_QueryGrid.AddRow(pRow);
	m_QueryGrid.AdjustLayout();

	//初始化试卷列表
	m_DeviceListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_DeviceListCtrl.GetWindowRect(rect);
	int iWidth = rect.Width();
	m_DeviceListCtrl.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/6);
	m_DeviceListCtrl.InsertColumn(1, L"设备编号", LVCFMT_LEFT,iWidth*2/6);
	m_DeviceListCtrl.InsertColumn(2, L"蓝牙地址", LVCFMT_LEFT,iWidth*3/6 -2 );
}

HRESULT CFormStudentDevice::AddItem(CYDObjectRef* _pref)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pref);

	hr = _pref->Restore();
	if(FAILED(hr))
	{
		return hr;
	}

	CComVariant valID;
	hr = _pref->GetPropVal(FIELD_YDDEVICE_ID, &valID);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strID = CDataHandler::VariantToString(valID);

	CComVariant valName;
	hr = _pref->GetPropVal(FIELD_YDDEVICE_MAC,&valName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strName = CDataHandler::VariantToString(valName);


	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_DeviceListCtrl.GetItemCount()+1);
	int index = m_DeviceListCtrl.InsertItem(m_DeviceListCtrl.GetItemCount(), strIndex);
	m_DeviceListCtrl.SetItemText(index,1, strID);
	m_DeviceListCtrl.SetItemText(index,2, strName);
	m_DeviceListCtrl.SetItemData(index, (DWORD_PTR)_pref);
	m_lstDeviceCache.push_back(_pref);

	return S_OK;
}
HRESULT CFormStudentDevice::InsertlstToListCtrl(std::list<CYDObjectRef*> &lstQuestion)
{
	HRESULT hr = E_FAIL;
	
	m_DeviceListCtrl.DeleteAllItems();
	
	std::list<CYDObjectRef*>::const_iterator itr = lstQuestion.begin();
	for (; itr != lstQuestion.end(); ++itr)
	{
		hr = AddItem(*itr);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	return S_OK;
}

void CFormStudentDevice::OnBnClickedBtnDmDeviceSearch()
{
	CWaitCursor wait; 
	HRESULT hr = E_FAIL;

	{
		CListAutoClean<CPropQueryContidition> clr(m_lstPropQuery);
	}
	CPropQueryContidition* pPropQuery = NULL;
	
	pPropQuery = new CPropQueryContidition();
	m_lstPropQuery.push_back(pPropQuery);
	pPropQuery->m_strFiledName = FIELD_YDDEVICE_ID;
	pPropQuery->m_uFieldType = VT_BSTR;

	UINT uOptype = 0;
	CBCGPGridRow* pRow = m_QueryGrid.GetRow(0);
	CBCGPGridItem* pOpItem = pRow->GetItem(1);
	ASSERT(pOpItem);
	CString strOp = pOpItem->GetLabel();
	if(strOp.CompareNoCase(_T("等于")) == 0)
	{
		pPropQuery->m_uOpType = Q_EQUAL;
	}
	else if(strOp.CompareNoCase(_T("不等于")) == 0)
	{
		pPropQuery->m_uOpType = Q_NOTEQUAL;
	}
	else if(strOp.CompareNoCase(_T("包含")) == 0)
	{
		pPropQuery->m_uOpType = Q_CONTAIN;
	}
	else
	{
		pPropQuery->m_uOpType = Q_UNKNOW;
	}
	CBCGPGridItem* pValItem = pRow->GetItem(2);
	ASSERT(pValItem);
	pPropQuery->m_strConVal = pValItem->GetLabel();


	pPropQuery = new CPropQueryContidition();
	m_lstPropQuery.push_back(pPropQuery);
	pPropQuery->m_strFiledName = FIELD_YDDEVICE_MAC;
	pPropQuery->m_uFieldType = VT_BSTR;
	pRow = m_QueryGrid.GetRow(1);
	pOpItem = pRow->GetItem(1);
	ASSERT(pOpItem);
	strOp = pOpItem->GetLabel();
	if(strOp.CompareNoCase(_T("等于")) == 0)
	{
		pPropQuery->m_uOpType = Q_EQUAL;
	}
	else if(strOp.CompareNoCase(_T("不等于")) == 0)
	{
		pPropQuery->m_uOpType = Q_NOTEQUAL;
	}
	else if(strOp.CompareNoCase(_T("包含")) == 0)
	{
		pPropQuery->m_uOpType = Q_CONTAIN;
	}
	else
	{
		pPropQuery->m_uOpType = Q_UNKNOW;
	}
	pValItem = pRow->GetItem(2);
	ASSERT(pValItem);
	pPropQuery->m_strConVal = pValItem->GetLabel();

	m_uPage = 0;

	hr = ExeQuery(TRUE);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
}

HRESULT CFormStudentDevice::ExeQuery(BOOL _bFirstQuery /*= FALSE*/)
{
	HRESULT hr = E_FAIL;
	
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);

	CYDDevice deviceRef(pDB);
	if(_bFirstQuery)
	{
		//第一次查询，要计算总数
		long lCount = 0;
		hr = deviceRef.GetObjectTotalNumber(&m_lstPropQuery, &lCount);
		if(FAILED(hr))
		{
			return hr;
		}
		m_utotalNumber = (UINT)lCount;
		m_uPageSize = m_utotalNumber/QUESTION_PAGE_COUNT;
		UpdateData(FALSE);
	}
	std::list<CYDObjectRef*> lstDevice;

	hr = deviceRef.GetObjectByCondition(m_uPage,
											&m_lstPropQuery,
											&lstDevice);
	if(FAILED(hr))
	{
		return hr;
	}
	
	hr = InsertlstToListCtrl(lstDevice);
	if(FAILED(hr))
	{
		return hr;
	}

	//按钮的可否使用
	if(m_uPage == 0)
	{
		//当前是第一页
		GetDlgItem(IDC_BTN_DM_QDEVICE_BEGIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DM_QDEVICE_PREV)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DM_QDEVICE_BEGIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DM_QDEVICE_PREV)->EnableWindow(TRUE);
	}

	if(m_uPageSize <= m_uPage)
	{
		//到达最后一页了
		GetDlgItem(IDC_BTN_DM_QDEVICE_NEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DM_QDEVICE_END)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DM_QDEVICE_NEXT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DM_QDEVICE_END)->EnableWindow(TRUE);
	}

	return S_OK;
}

void CFormStudentDevice::OnBnClickedBtnDmQDeviceBegin()
{
	HRESULT hr = E_FAIL;
	m_uPage = 0;
	hr = ExeQuery();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
}

void CFormStudentDevice::OnBnClickedBtnDmQDevicePrev()
{
	HRESULT hr = E_FAIL;
	if(m_uPage > 0)
	{
		m_uPage --;
	}
	hr = ExeQuery();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
}

void CFormStudentDevice::OnBnClickedBtnDmQDeviceNext()
{
	HRESULT hr = E_FAIL;
	m_uPage++;
	hr = ExeQuery();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
}

void CFormStudentDevice::OnBnClickedBtnDmQDeviceEnd()
{
	HRESULT hr = E_FAIL;
	m_uPage = m_uPageSize;
	hr = ExeQuery();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
}

BOOL CFormStudentDevice::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	CExtDllState extdllstate;

	return CYdFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CFormStudentDevice::OnBnClickedBtnDmDeviceOutput()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CListXlsoutput ListXlsoutput(&m_DeviceListCtrl);
	hr = ListXlsoutput.OutputExcel();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	if(hr == S_FALSE)
	{
		return ;
	}
	AfxMessageBox(_T("输出XLS成功！"));
}

void CFormStudentDevice::OnBnClickedBtnDmDeviceInput()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Excle文件(*.xls)|*.xls|所有文件(*.*)|*.*||"));
	if(dlg.DoModal() != IDOK)
	{
		return ;
	}
	HRESULT hr = E_FAIL;
	CDlgInputDeviePreview dlgPreview;
	CListXlsInput ListXlsInput(dlg.GetPathName());
	hr = ListXlsInput.Read(&dlgPreview.m_lstPropVal);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	if(dlgPreview.DoModal() != IDOK)
	{
		return;
	}
}
