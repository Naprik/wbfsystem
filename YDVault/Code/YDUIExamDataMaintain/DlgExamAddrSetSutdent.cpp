// DlgExamAddrSetSutdent.cpp : implementation file
//

#include "stdafx.h"
#include "DlgExamAddrSetSutdent.h"
#include "../Base/AutoClean.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../YDExamObjRef/YDDepartmentRef.h"
#include "../YDExamObjRef/YdStudentRef.h"
#include "../YDExamObjRef/YDClassRef.h"
#include "../Base/DataHandler.h"
#include "../DBBase/DBTransactionRef.h"
#include "../YDExamObjRef/YDEAddrUnit.h"


// CDlgExamAddrSetSutdent dialog

IMPLEMENT_DYNAMIC(CDlgExamAddrSetSutdent, CDialog)

CDlgExamAddrSetSutdent::CDlgExamAddrSetSutdent(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExamAddrSetSutdent::IDD, pParent)
{
	m_pExamAddr = NULL;
}

CDlgExamAddrSetSutdent::~CDlgExamAddrSetSutdent()
{
	CListAutoClean<CYDObjectRef> clr1(m_lstDeptCache);
	CListAutoClean<CYDObjectRef> clr2(m_lstStudentsCache);
	
	CListAutoClean<CYDObjectRef> clr4(m_lstClassCache);
	CListAutoClean<CYDObjectRef> clr5(m_lstDeviceCache);
	CListAutoClean<CYdObjWrapper> clr3(m_lstUnitCache);
}

void CDlgExamAddrSetSutdent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DM_EXAMADDR_DEVICE, m_DeviceListCtrl);
	DDX_Control(pDX, IDC_LIST_DM_EXAMADDR_UNIT, m_UnitListCtrl);
	DDX_Control(pDX, IDC_LIST_DM_EXAMADDR_STUS, m_StudentListCtrl);
	DDX_Control(pDX, IDC_DM_EXAMADDR_DEPARTMENT, m_cmbDept);
	DDX_Control(pDX, IDC_DM_EXAMADDR_CLASS, m_cmbClass);
}


BEGIN_MESSAGE_MAP(CDlgExamAddrSetSutdent, CDialog)
	ON_CBN_SELCHANGE(IDC_DM_EXAMADDR_DEPARTMENT, &CDlgExamAddrSetSutdent::OnCbnSelchangeDmExamaddrDepartment)
	
	ON_BN_CLICKED(IDC_BTN_EXAMADDR_OK, &CDlgExamAddrSetSutdent::OnBnClickedBtnExamaddrOk)
	ON_CBN_SELCHANGE(IDC_DM_EXAMADDR_CLASS, &CDlgExamAddrSetSutdent::OnCbnSelchangeDmExamaddrClass)
	ON_BN_CLICKED(IDC_BTN_DM_EXAMADDR_ADD, &CDlgExamAddrSetSutdent::OnBnClickedBtnDmExamaddrAdd)
	ON_BN_CLICKED(IDC_BTN_DM_EXAMADDR_DEL, &CDlgExamAddrSetSutdent::OnBnClickedBtnDmExamaddrDel)
END_MESSAGE_MAP()


// CDlgExamAddrSetSutdent message handlers

BOOL CDlgExamAddrSetSutdent::OnInitDialog()
{
	CDialog::OnInitDialog();

	//初始化学生列表
	m_StudentListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_StudentListCtrl.GetWindowRect(rect);
	int iWidth = rect.Width();
	m_StudentListCtrl.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth/8);
	m_StudentListCtrl.InsertColumn(1, L"学号", LVCFMT_LEFT,iWidth*3/8);
	m_StudentListCtrl.InsertColumn(2, L"姓名", LVCFMT_LEFT,iWidth*3/8);
	m_StudentListCtrl.InsertColumn(3, L"性别", LVCFMT_LEFT,iWidth*1/8 -2);

	//初始化学生列表
	m_UnitListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect2;
	m_UnitListCtrl.GetWindowRect(rect2);
	int iWidth2 = rect2.Width();
	m_UnitListCtrl.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth2/12);
	m_UnitListCtrl.InsertColumn(1, L"学号", LVCFMT_LEFT,iWidth2*3/12);
	m_UnitListCtrl.InsertColumn(2, L"姓名", LVCFMT_LEFT,iWidth2*2/12);
	m_UnitListCtrl.InsertColumn(3, L"性别", LVCFMT_LEFT,iWidth2*1/12);
	m_UnitListCtrl.InsertColumn(4, L"准考证号", LVCFMT_LEFT,iWidth2*3/12);
	m_UnitListCtrl.InsertColumn(5, L"学生机设备号", LVCFMT_LEFT,iWidth2*2/12 -2);

	//初始化学生列表
	m_DeviceListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect3;
	m_DeviceListCtrl.GetWindowRect(rect3);
	int iWidth3 = rect3.Width();
	m_DeviceListCtrl.InsertColumn(0, L"序号", LVCFMT_LEFT, iWidth3/6);
	m_DeviceListCtrl.InsertColumn(1, L"设备号", LVCFMT_LEFT,iWidth3*2/6);
	m_DeviceListCtrl.InsertColumn(2, L"蓝牙地址", LVCFMT_LEFT,iWidth3*3/6-2);

	//初始化部门下拉框
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	HRESULT hr = CStaticObjHelper::GetObjRef(DB_YDDEPARTMENT, pDB, &m_lstDeptCache);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	if (m_lstDeptCache.size() > 0)
	{
		int nIndex = 0;
		for (std::list<CYDObjectRef*>::const_iterator itr=m_lstDeptCache.begin();
			itr != m_lstDeptCache.end(); ++itr, ++nIndex)
		{
			CString strDeptName;
			(*itr)->GetPropVal(FIELD_YDDEPARTMENT_NAME, strDeptName);
			m_cmbDept.InsertString(nIndex, strDeptName);
			m_cmbDept.SetItemData(nIndex, (DWORD_PTR)(*itr));
		}
	}


	hr = CStaticObjHelper::GetObjRef(DB_YDDEVICE, pDB, &m_lstDeviceCache);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	if (m_lstDeviceCache.size() > 0)
	{
		int nIndex = 0;
		for (std::list<CYDObjectRef*>::const_iterator itr=m_lstDeviceCache.begin();
			itr != m_lstDeviceCache.end(); ++itr, ++nIndex)
		{
			AddDeviceItem(*itr);
		}
	}

	std::list<CYDObjectRef*> lstStu;
	std::list<CYDLinkRef*> lstLinks;
	hr = m_pExamAddr->GetAllUnits(&lstStu, &lstLinks);
	if (FAILED(hr))
	{
		return hr;
	}
	std::list<CYDObjectRef*>::const_iterator itrObj = lstStu.begin();
	std::list<CYDLinkRef*>::const_iterator itrLinks = lstLinks.begin();
	for (; itrObj != lstStu.end(); ++itrObj, ++itrLinks)
	{
		CYdObjWrapper* pObjWrapper = new CYdObjWrapper();
		pObjWrapper->m_state = S_UNKNOW;
		pObjWrapper->m_pObjRef = *itrObj;
		pObjWrapper->m_pLinkRef = *itrLinks;

		AddExamAddrUnitItem(pObjWrapper);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

INT_PTR CDlgExamAddrSetSutdent::DoModal()
{
	CExtDllState dllExt;

	return CDialog::DoModal();
}

void CDlgExamAddrSetSutdent::OnCbnSelchangeDmExamaddrDepartment()
{
	CString strSelDept;
	m_cmbDept.GetWindowText(strSelDept);

	for (int i = m_cmbClass.GetCount() - 1; i >= 0; i--)
	{
		m_cmbClass.DeleteString(i);
	}
	{
		CListAutoClean<CYDObjectRef> clr(m_lstClassCache);
	}

	if (strSelDept.GetLength() > 0)
	{
		CYDDepartmentRef* pDept = (CYDDepartmentRef*)m_cmbDept.GetItemData(m_cmbDept.GetCurSel());
		ASSERT(pDept);
		HRESULT hr = pDept->GetAllClasses(&m_lstClassCache, NULL);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		if (m_lstClassCache.size() > 0)
		{
			int nIndex = 0;
			for (std::list<CYDObjectRef*>::const_iterator itr = m_lstClassCache.begin();
				itr != m_lstClassCache.end(); ++itr, ++nIndex)
			{
				CString strClsName;
				(*itr)->GetPropVal(FIELD_YDCLASS_NAME, strClsName);
				m_cmbClass.InsertString(nIndex, strClsName);
				m_cmbClass.SetItemData(nIndex, (DWORD_PTR)(*itr));
			}
		}
	}
	m_cmbClass.SetFocus();
}

void CDlgExamAddrSetSutdent::OnBnClickedBtnExamaddrOk()
{
	
	OnOK();
}


HRESULT CDlgExamAddrSetSutdent::AddDeptStuItem(CYDObjectRef* _pref)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pref);

	hr = _pref->Restore();
	if(FAILED(hr))
	{
		return hr;
	}

	CComVariant valID;
	hr = _pref->GetPropVal(FIELD_YDSTU_ID, &valID);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strID = CDataHandler::VariantToString(valID);

	CComVariant valName;
	hr = _pref->GetPropVal(FIELD_YDSTU_NAME,&valName);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strName = CDataHandler::VariantToString(valName);


	CString strGender;
	CYdStudentRef* pStu = dynamic_cast<CYdStudentRef*>(_pref);
	pStu->GetGender(&strGender);

	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_StudentListCtrl.GetItemCount()+1);
	int index = m_StudentListCtrl.InsertItem(m_StudentListCtrl.GetItemCount(), strIndex);
	m_StudentListCtrl.SetItemText(index,1, strID);
	m_StudentListCtrl.SetItemText(index,2, strName);
	m_StudentListCtrl.SetItemText(index,3, strGender);
	m_StudentListCtrl.SetItemData(index, (DWORD_PTR)_pref);
	
	return S_OK;
}

HRESULT CDlgExamAddrSetSutdent::AddDeviceItem(CYDObjectRef* _pref)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pref);

	hr = _pref->Restore();
	if(FAILED(hr))
	{
		return hr;
	}

	CString strID;
	hr = _pref->GetPropVal(FIELD_YDDEVICE_ID, strID);
	if(FAILED(hr))
	{
		return hr;
	}
	
	CString strMac;
	hr = _pref->GetPropVal(FIELD_YDDEVICE_MAC, strMac);
	if(FAILED(hr))
	{
		return hr;
	}
	
	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_DeviceListCtrl.GetItemCount()+1);
	int index = m_DeviceListCtrl.InsertItem(m_DeviceListCtrl.GetItemCount(), strIndex);
	m_DeviceListCtrl.SetItemText(index,1, strID);
	m_DeviceListCtrl.SetItemText(index,2, strMac);
	m_DeviceListCtrl.SetItemData(index, (DWORD_PTR)_pref);

	return S_OK;
}

HRESULT CDlgExamAddrSetSutdent::AddExamAddrUnitItem(CYdObjWrapper* _pref)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pref);

	hr = _pref->m_pObjRef->Restore();
	if(FAILED(hr))
	{
		return hr;
	}
	CYDEAddrUnit* pUnit = dynamic_cast<CYDEAddrUnit*>(_pref->m_pObjRef);
	CYdStudentRef* pStu = NULL;
	pUnit->GetStudent(pStu);
	CPtrAutoClean<CYdStudentRef> clr1(pStu);
	CString strName;
	pStu->GetPropVal(FIELD_YDSTU_NAME, strName);
	CString strStuID;
	pStu->GetPropVal(FIELD_YDSTU_ID, strStuID);
	CString strGenger;
	pStu->GetGender(&strGenger);

	CString strExamID;
	pUnit->GetPropVal(FIELD_YDEXAMADDRUNIT_EXAMID, strExamID);

	CYDDevice* pDevice = NULL;
	pUnit->GetDevice(pDevice);
	CPtrAutoClean<CYDDevice> clr2(pDevice);
	CString strDeviceID;
	pDevice->GetPropVal(FIELD_YDDEVICE_ID, strDeviceID);


	CString strIndex(_T(""));
	strIndex.Format(_T("%d"), m_UnitListCtrl.GetItemCount()+1);
	int index = m_UnitListCtrl.InsertItem(m_UnitListCtrl.GetItemCount(), strIndex);
	m_UnitListCtrl.SetItemText(index,1, strStuID);
	m_UnitListCtrl.SetItemText(index,2, strName);
	m_UnitListCtrl.SetItemText(index,3, strGenger);
	m_UnitListCtrl.SetItemText(index,4, strExamID);
	m_UnitListCtrl.SetItemText(index,5, strDeviceID);
	m_UnitListCtrl.SetItemData(index, (DWORD_PTR)_pref);

	m_lstUnitCache.push_back(_pref);
	return S_OK;
}

void CDlgExamAddrSetSutdent::OnCbnSelchangeDmExamaddrClass()
{
	{
		CListAutoClean<CYDObjectRef> clr(m_lstStudentsCache);
	}
	m_StudentListCtrl.DeleteAllItems();
	int nIndex = m_cmbClass.GetCurSel();
	if (nIndex == -1)
	{
		return;
	}
	
	CYDClassRef* pClass = (CYDClassRef*)m_cmbClass.GetItemData(nIndex);
	HRESULT hr = pClass->GetAllStudents(&m_lstStudentsCache, NULL);
	if (FAILED(hr))
	{
		return ;
	}
	for (std::list<CYDObjectRef*>::const_iterator itr = m_lstStudentsCache.begin();
		 itr != m_lstStudentsCache.end(); ++itr)
	{
		hr = AddDeptStuItem(*itr);
		if (FAILED(hr))
		{
			return;
		}
	}

}

BOOL	CDlgExamAddrSetSutdent::IsStudentExist(CYDObjectRef* pStu)
{
	OBJID objID;
	pStu->GetID(&objID);
	std::set<OBJID>::iterator itr = m_setExistStu.find(objID);
	if (itr != m_setExistStu.end())
	{
		return TRUE;
	}

	return FALSE;
}
void CDlgExamAddrSetSutdent::OnBnClickedBtnDmExamaddrAdd()
{
	UpdateData();
	POSITION posStu = m_StudentListCtrl.GetFirstSelectedItemPosition();
	if (posStu == NULL)
	{
		AfxMessageBox(L"请选择加入考场的学生！");
		return;
	}
	POSITION posDevice = m_DeviceListCtrl.GetFirstSelectedItemPosition();
	if (posDevice == NULL)
	{
		AfxMessageBox(L"请选择加入考场的设备！");
		return;
	}
	CString strExamID;
	GetDlgItem(IDC_EDIT_DM_EXAMADDR_EXAMID)->GetWindowText(strExamID);
	if (strExamID.GetLength() <= 0)
	{
		AfxMessageBox(L"请填写考生的准考证号！");
		GetDlgItem(IDC_EDIT_DM_EXAMADDR_EXAMID)->SetFocus();
		return;
	}
	
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	CYDEAddrUnit* pUnit = new CYDEAddrUnit(pDB);
	BOOL bExist = FALSE;
	pUnit->IsFieldExist(FIELD_YDEXAMADDRUNIT_EXAMID, strExamID, &bExist);
	
	if (bExist)
	{
		AfxMessageBox(L"当前的准考证号已经存在，请重新输入！");
		GetDlgItem(IDC_EDIT_DM_EXAMADDR_EXAMID)->SetWindowText(L"");
		UpdateData(FALSE);
		GetDlgItem(IDC_EDIT_DM_EXAMADDR_EXAMID)->SetFocus();
		delete pUnit;
		pUnit = NULL;
		return;
	}
	int nItemStu = m_StudentListCtrl.GetNextSelectedItem(posStu);
	CYdStudentRef* pStudent = (CYdStudentRef*)m_StudentListCtrl.GetItemData(nItemStu);
	OBJID stuID;
	pStudent->GetID(&stuID);
	

	int nItemDev = m_DeviceListCtrl.GetNextSelectedItem(posDevice);
	CYDDevice* pDevice = (CYDDevice*)m_DeviceListCtrl.GetItemData(nItemDev);
	OBJID deviceID;
	pDevice->GetID(&deviceID);
	
	CComVariant varStuId((long)(stuID));
	pUnit->SetPropVal(FIELD_YDEXAMADDRUNIT_STUID, &varStuId);
	CComVariant varDeviceID((long)(deviceID));
	pUnit->SetPropVal(FIELD_YDEXAMADDRUNIT_DEVICEID, &varDeviceID);
	CComVariant varExamID(strExamID);
	pUnit->SetPropVal(FIELD_YDEXAMADDRUNIT_EXAMID, &varExamID);
	CYDLinkRef* pLink = new CYDLinkRef(pDB, DB_YDLINK_EXAMADDR_UNIT);
	pLink->PutObjRef(m_pExamAddr, pUnit);

	CYdObjWrapper* pWrapper = new CYdObjWrapper();
	pWrapper->m_pLinkRef = pLink;
	pWrapper->m_pObjRef = pUnit;

	CDBTransactionRef trans(pDB, TRUE);
	HRESULT hr = pWrapper->Save();
	if (FAILED(hr))
	{
		return;
	}
	hr = trans.Commit();
	if (FAILED(hr))
	{
		return ;
	}
	m_StudentListCtrl.DeleteItem(nItemStu);
	m_DeviceListCtrl.DeleteItem(nItemDev);
	AddExamAddrUnitItem(pWrapper);
}

void CDlgExamAddrSetSutdent::OnBnClickedBtnDmExamaddrDel()
{
	POSITION pos = m_UnitListCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(L"请选择要从考场删除的学生！");
	}
	else
	{
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		HRESULT hr = E_FAIL;
		CDBTransactionRef trans(pDB, TRUE);
		while (pos)
		{
			int nItem = m_UnitListCtrl.GetNextSelectedItem(pos);
			CYdObjWrapper* pObjWrapper = (CYdObjWrapper*)m_UnitListCtrl.GetItemData(nItem);
			hr = pObjWrapper->Remove();
			if (FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return ;
			}
			
			m_UnitListCtrl.DeleteItem(nItem);
			pos = m_UnitListCtrl.GetFirstSelectedItemPosition();
		}
		hr = trans.Commit();
		if (FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
	}
}
