// FormTeacherDevice.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FormTeacherDevice.h"


#include "../YDExamObjRef/YDTeacherDevRef.h"
#include "../base/AutoClean.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../Base/DataHandler.h"
#include "../DBBase/DBTransactionRef.h"
#include "../YDExamObjRef/YDClassRef.h"

#include "DlgTeacherdev.h"
#include "../UIBase\ListXlsoutput.h"

// CFormTeacherDevice

IMPLEMENT_DYNCREATE(CFormTeacherDevice, CYdFormView)

CFormTeacherDevice::CFormTeacherDevice()
	: CYdFormView(CFormTeacherDevice::IDD)
{

}

CFormTeacherDevice::~CFormTeacherDevice()
{
	CListAutoClean<CYDObjectRef> clr(m_lstCache);
}

void CFormTeacherDevice::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DM_TEA_DEVICE, m_TDeviceListCtrl);
}

BEGIN_MESSAGE_MAP(CFormTeacherDevice, CYdFormView)
	ON_BN_CLICKED(BTN_DM_TDEVICE_NEW, &CFormTeacherDevice::OnBnClickedDmTdeviceNew)
	ON_BN_CLICKED(BTN_DM_TDEVICE_EDIT, &CFormTeacherDevice::OnBnClickedDmTdeviceEdit)
	ON_BN_CLICKED(BTN_DM_TDEVICE_DEL, &CFormTeacherDevice::OnBnClickedDmTdeviceDel)
	ON_BN_CLICKED(BTN_DM_TDEVICE_OUTPUT, &CFormTeacherDevice::OnBnClickedDmTdeviceOutput)
END_MESSAGE_MAP()


// CFormTeacherDevice diagnostics

#ifdef _DEBUG
void CFormTeacherDevice::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormTeacherDevice::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormTeacherDevice message handlers

void CFormTeacherDevice::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();
	m_TDeviceListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_TDeviceListCtrl.GetWindowRect(rect);
	int iWidth = rect.Width();
	m_TDeviceListCtrl.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/8);
	m_TDeviceListCtrl.InsertColumn(1, L"编号", LVCFMT_LEFT,iWidth*2/8);
	m_TDeviceListCtrl.InsertColumn(2, L"使用教师", LVCFMT_LEFT,iWidth*3/8);
	m_TDeviceListCtrl.InsertColumn(3, L"教师机蓝牙地址", LVCFMT_LEFT,iWidth*2/8);

	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	std::list<CYDObjectRef*> lstTDevice;
	HRESULT hr = CStaticObjHelper::GetObjRef(DB_YDTEACHERDEV, pDB, &lstTDevice);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}

	for(std::list<CYDObjectRef*>::const_iterator itr = lstTDevice.begin();
		itr != lstTDevice.end();++itr)
	{
		hr = AddTeacherDeviceItem(*itr);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
	// TODO: Add your specialized code here and/or call the base class
}

void CFormTeacherDevice::OnBnClickedDmTdeviceNew()
{
	CDlgTeacherdev	dlg(OP_NEW);

	if(dlg.DoModal() == IDOK)
	{
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CYDObjectRef* pObjRef = new CYDTeacherDevRef(pDB);
		CComVariant varId(dlg.m_devid);
		pObjRef->SetPropVal(FIELD_YDTEACHERDEV_TEACHID, &varId);
		CComVariant varName(dlg.m_teachername);
		pObjRef->SetPropVal(FIELD_YDTEACHERDEV_NAME, &varName);
		CComVariant varDevice(dlg.m_devmac);
		pObjRef->SetPropVal(FIELD_YDTEACHERDEV_DEVID, &varDevice);
		CDBTransactionRef trans(pDB, TRUE);
		HRESULT hr = pObjRef->Save();
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
		AddTeacherDeviceItem(pObjRef);
	}
}

void CFormTeacherDevice::OnBnClickedDmTdeviceEdit()
{
	HRESULT hr = E_FAIL;
	int nCount = m_TDeviceListCtrl.GetSelectedCount();
	if(nCount != 1)
	{
		AfxMessageBox(_T("请在列表中选择一个教师设备！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_TDeviceListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_TDeviceListCtrl.GetNextSelectedItem(pos);
	CYDObjectRef* pObjSubject = (CYDObjectRef*)m_TDeviceListCtrl.GetItemData(nItem);
	ASSERT(pObjSubject != NULL);
	CComVariant valID;
	hr = pObjSubject->GetPropVal(FIELD_YDTEACHERDEV_TEACHID, &valID);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strID = CDataHandler::VariantToString(valID);

	CComVariant valName;
	hr = pObjSubject->GetPropVal(FIELD_YDTEACHERDEV_NAME,&valName);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strName = CDataHandler::VariantToString(valName);

	CComVariant valDevice;
	hr = pObjSubject->GetPropVal(FIELD_YDTEACHERDEV_DEVID,&valDevice);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString strDevice = CDataHandler::VariantToString(valDevice);

	CDlgTeacherdev	dlg(OP_EDIT, this);
	dlg.m_devid = strID;
	dlg.m_teachername = strName;
	dlg.m_devmac = strDevice;
	if(dlg.DoModal() == IDOK)
	{
		CYDTeacherDevRef* pRef = (CYDTeacherDevRef*)pObjSubject;

		CComVariant valID(dlg.m_devid);
		hr = pRef->SetPropVal(FIELD_YDDEPARTMENT_ID,&valID);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CComVariant valName(dlg.m_teachername);
		hr = pRef->SetPropVal(FIELD_YDDEPARTMENT_NAME,&valName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

		CComVariant varDevice(dlg.m_devmac);
		hr = pRef->SetPropVal(FIELD_YDTEACHERDEV_DEVID,&varDevice);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		CDBTransactionRef transRef(pDB,TRUE);
		hr = pRef->Update();
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
		m_TDeviceListCtrl.SetItemText(nItem,1,dlg.m_devid);
		m_TDeviceListCtrl.SetItemText(nItem,2,dlg.m_teachername);
		m_TDeviceListCtrl.SetItemText(nItem,3,dlg.m_devmac);
	}
}

void CFormTeacherDevice::OnBnClickedDmTdeviceDel()
{
	int nCount = m_TDeviceListCtrl.GetSelectedCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_T("请在列表中选择一个教师设备！"),MB_OK|MB_ICONWARNING);
		return;
	}
	POSITION pos = m_TDeviceListCtrl.GetFirstSelectedItemPosition();
	ASSERT(pos);
	int nItem = m_TDeviceListCtrl.GetNextSelectedItem(pos);
	CYDObjectRef* pObjRef = (CYDObjectRef*)m_TDeviceListCtrl.GetItemData(nItem);
	ASSERT(pObjRef );
	if (AfxMessageBox(_T("确定要删除选择的教师设备吗？"), MB_OKCANCEL) != IDOK)
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
	m_TDeviceListCtrl.DeleteItem(nItem);
}

HRESULT CFormTeacherDevice::AddTeacherDeviceItem(CYDObjectRef* _pref)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pref);

	hr = _pref->Restore();
	if(FAILED(hr))
	{
		return hr;
	}

	CComVariant valID;
	hr = _pref->GetPropVal(FIELD_YDTEACHERDEV_TEACHID, &valID);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strID = CDataHandler::VariantToString(valID);

	CComVariant valName;
	hr = _pref->GetPropVal(FIELD_YDTEACHERDEV_NAME,&valName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strName = CDataHandler::VariantToString(valName);

	CComVariant valDevice;
	hr = _pref->GetPropVal(FIELD_YDTEACHERDEV_DEVID,&valDevice);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strDevice = CDataHandler::VariantToString(valDevice);


	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_TDeviceListCtrl.GetItemCount()+1);
	int index = m_TDeviceListCtrl.InsertItem(m_TDeviceListCtrl.GetItemCount(), strIndex);
	m_TDeviceListCtrl.SetItemText(index,1, strID);
	m_TDeviceListCtrl.SetItemText(index,2, strName);
	m_TDeviceListCtrl.SetItemText(index,3, strDevice);
	m_TDeviceListCtrl.SetItemData(index, (DWORD_PTR)_pref);
	m_lstCache.push_back(_pref);

	return S_OK;
}
BOOL CFormTeacherDevice::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	CExtDllState extdllstate;

	return CYdFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CFormTeacherDevice::OnBnClickedDmTdeviceOutput()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CListXlsoutput ListXlsoutput(&m_TDeviceListCtrl);
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
