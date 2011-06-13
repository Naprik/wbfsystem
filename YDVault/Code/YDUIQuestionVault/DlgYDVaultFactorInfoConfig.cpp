// YDVaultFactorInfoConfig.cpp : implementation file
//

#include "stdafx.h"
#include "DlgYDVaultFactorInfoConfig.h"
#include "afxdialogex.h"
#include <list>
#include "../ObjRef/YDQuestionType.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YDQuestionVault.h"
#include "../Include/ShowErr.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef/PropQueryContidition.h"
#include "../ObjRef/YdFactorInfoItemObjRef.h"
#include "../Base/DataHandler.h"
#include "../ObjHelper/FactorInfoHelper.h"
#include "../DBBase/DBTransactionRef.h"



// CDlgYDVaultFactorInfoConfig dialog
const int cColQTypeID = 0;//题型ID
const int cColFactorName = 1;//指标名称
const int cColFieldName = 2;//预留字段名称
const int cColMin = 3;//数值型下界
const int cColMax = 4;//数值型上界
const int cColDes = 5;//描述

IMPLEMENT_DYNAMIC(CDlgYDVaultFactorInfoConfig, CDialogEx)

CDlgYDVaultFactorInfoConfig::CDlgYDVaultFactorInfoConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgYDVaultFactorInfoConfig::IDD, pParent)
{
	m_pVault = NULL;
}

CDlgYDVaultFactorInfoConfig::~CDlgYDVaultFactorInfoConfig()
{
	m_pVault = NULL;
	CListAutoClean<CYDQuestionType> clr(m_lstType);
	CListAutoClean<CYDObjectRef> clr2(m_lstOldFactorInfoItem);
}

void CDlgYDVaultFactorInfoConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgYDVaultFactorInfoConfig, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgYDVaultFactorInfoConfig::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgYDVaultFactorInfoConfig::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgYDVaultFactorInfoConfig::OnBnClickedButtonDel)
	ON_REGISTERED_MESSAGE(BCGM_GRID_ITEM_CHANGED,OnItemChanged)
END_MESSAGE_MAP()


// CDlgYDVaultFactorInfoConfig message handlers


void CDlgYDVaultFactorInfoConfig::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	if(!ValidateData())
	{
		return;
	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef trans(pDB, TRUE);
	trans.Begin();
	hr = DelOldItem();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	//将列表中的对象插入到数据库中
	for(int i = 0; i < m_Grid.GetRowCount();++i)
	{
		CBCGPGridRow* pRow = m_Grid.GetRow(i);
		ASSERT(pRow);
		CBCGPGridRow* pParentRow = pRow->GetParent();
		if(pParentRow )
		{
			continue;
		}
		hr = InsertItemByRowType(pRow,pDB);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}	
	}
	hr = trans.Commit();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	CDialogEx::OnOK();
}

HRESULT CDlgYDVaultFactorInfoConfig::InsertItemByRowType(CBCGPGridRow* _pRowType,CDatabaseEx* pDb)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pRowType);
	CYDQuestionType* pType = (CYDQuestionType*)_pRowType->GetData();
	ASSERT(pType);
	OBJID idType = 0;
	hr = pType->GetID(&idType);
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valIDType(idType);
	ASSERT(m_pVault);
	OBJID idVault = 0;
	hr = m_pVault->GetID(&idVault);
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valIDVault(idVault);
	CFactorInfoHelper helper;
	//插入记录
	for(int i = 0 ; i < _pRowType->GetSubItemsCount();++i)
	{
		CBCGPGridRow *pSubRow = _pRowType->GetSubItem(i);
		ASSERT(pSubRow);
		CYDObjectRef* pFactorInfoItem = new CYdFactorInfoItemObjRef(pDb);
		m_lstOldFactorInfoItem.push_back(pFactorInfoItem);
		hr = pFactorInfoItem->SetPropVal(FIELD_YDFACTORINFOITEM_QTYPEID,&valIDType);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pFactorInfoItem->SetPropVal(FIELD_YDFACTORINFOITEM_VAULTID,&valIDVault);
		if(FAILED(hr))
		{
			return hr;
		} 
		CComVariant valFactorName = pSubRow->GetItem(cColFactorName)->GetValue();
		hr = pFactorInfoItem->SetPropVal(FIELD_YDFACTORINFOITEM_FACTORNAME,&valFactorName);
		if(FAILED(hr))
		{
			return hr;
		} 
		CComVariant valFieldName = pSubRow->GetItem(cColFieldName)->GetValue();
		hr = pFactorInfoItem->SetPropVal(FIELD_YDFACTORINFOITEM_FIELDNAME,&valFieldName);
		if(FAILED(hr))
		{
			return hr;
		} 
		CString strFieldName = CDataHandler::VariantToString(valFieldName);
		if(helper.IsNumberFieldName(strFieldName))
		{
			CComVariant valMin = pSubRow->GetItem(cColMin)->GetValue();
			long lMin =  CDataHandler::VariantToLong(valMin);
			valMin = lMin;
			hr = pFactorInfoItem->SetPropVal(FIELD_YDFACTORINFOITEM_MIN,&valMin);
			if(FAILED(hr))
			{
				return hr;
			} 
			CComVariant valMax = pSubRow->GetItem(cColMax)->GetValue();
			long lMax = CDataHandler::VariantToLong(valMax);
			valMax = lMax;
			hr = pFactorInfoItem->SetPropVal(FIELD_YDFACTORINFOITEM_MAX,&valMax);
			if(FAILED(hr))
			{
				return hr;
			} 
		}
		
		CComVariant valDes = pSubRow->GetItem(cColDes)->GetValue();
		hr = pFactorInfoItem->SetPropVal(FIELD_YDFACTORINFOITEM_DESCRIPTION,&valDes);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pFactorInfoItem->Save();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}
HRESULT CDlgYDVaultFactorInfoConfig::DelOldItem()
{
	HRESULT hr = E_FAIL;
	for(std::list<CYDObjectRef*>::const_iterator itr =	m_lstOldFactorInfoItem.begin();
		itr != m_lstOldFactorInfoItem.end();++itr)
	{
		hr = (*itr)->Remove();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	CListAutoClean<CYDObjectRef> clr(m_lstOldFactorInfoItem);
	return S_OK;
}

BOOL CDlgYDVaultFactorInfoConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
		CRect rectBK;
	GetDlgItem(IDC_STATIC_BK)->GetWindowRect(&rectBK);
	GetDlgItem(IDC_STATIC_BK)->ShowWindow(SW_HIDE);
	// TODO:  Add extra initialization here
	ScreenToClient(&rectBK);
	if (!m_Grid.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE, rectBK, this,
		-1))
	{
		return FALSE;
	}
	m_Grid.EnableDragHeaderItems(FALSE);
	//m_Grid.EnableColumnAutoSize (TRUE);
	m_Grid.EnableGroupByBox (FALSE);
	m_Grid.SetWholeRowSel(FALSE);
	m_Grid.EnableHeader (TRUE, 0);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_Grid.SetColorTheme (colors);
	int iWidthGrid = rectBK.Width();
	m_Grid.InsertColumn(cColQTypeID, L"题型", 100);
	m_Grid.InsertColumn(cColFactorName, L"指标名称", 100);
	m_Grid.InsertColumn(cColFieldName, L"预留字段名称", 100);
	m_Grid.InsertColumn(cColMin, L"最小值", 100);
	m_Grid.InsertColumn(cColMax, L"最大值", 100);
	m_Grid.InsertColumn(cColDes, L"描述", 100);
	
	ASSERT(m_pVault);
	HRESULT hr = E_FAIL;
	
	
	hr = m_pVault->GetAllQuestionType(&m_lstType);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return FALSE;
	}
	for(std::list<CYDQuestionType*>::const_iterator itr = m_lstType.begin();
		itr != m_lstType.end();++itr)
	{
		hr = InsertByQuestionType(*itr);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return FALSE;
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CDlgYDVaultFactorInfoConfig::InsertByQuestionType(CYDQuestionType* _pQType)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pQType);
	CBCGPGridRow* pRowQType = m_Grid.CreateRow(m_Grid.GetColumnCount());
	CString strLabel;
	hr = _pQType->GetLabel(&strLabel);
	if(FAILED(hr))
	{
		return hr;
	}
	//题型
	pRowQType->GetItem(cColQTypeID)->SetValue(CComVariant(strLabel));
	pRowQType->GetItem(cColQTypeID)->SetBackgroundColor(RGB(110,180,200));
	pRowQType->GetItem(cColQTypeID)->Enable(FALSE);

	pRowQType->GetItem(cColFactorName)->SetBackgroundColor(RGB(110,180,200));
	pRowQType->GetItem(cColFactorName)->Enable(FALSE);
	pRowQType->GetItem(cColFieldName)->SetBackgroundColor(RGB(110,180,200));
	pRowQType->GetItem(cColFieldName)->Enable(FALSE);
	pRowQType->GetItem(cColMin)->SetBackgroundColor(RGB(110,180,200));
	pRowQType->GetItem(cColMin)->Enable(FALSE);
	pRowQType->GetItem(cColMax)->SetBackgroundColor(RGB(110,180,200));
	pRowQType->GetItem(cColMax)->Enable(FALSE);
	pRowQType->GetItem(cColDes)->SetBackgroundColor(RGB(110,180,200));
	pRowQType->GetItem(cColDes)->Enable(FALSE);
	pRowQType->AllowSubItems();
	pRowQType->SetData(DWORD_PTR(_pQType));
	m_Grid.AddRow(pRowQType);

	//得到原来的，在当前题库下题型为_pQType的所有的CYdFactorInfoItemObjRef
	
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	std::list<CYDObjectRef*> lstFactorInfoItem;
	CFactorInfoHelper helper;
	hr = helper.GetFactorInfoByVaultQType(pDB,m_pVault,_pQType,&lstFactorInfoItem);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = lstFactorInfoItem.begin();
		itr != lstFactorInfoItem.end();++itr)
	{
		m_lstOldFactorInfoItem.push_back(*itr);
		hr = InsertRowByFactorInfoItem(pRowQType,*itr);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CDlgYDVaultFactorInfoConfig::InsertRowByFactorInfoItem(CBCGPGridRow* _pParentRow,
															CYDObjectRef* _pFactorInfoItem)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pParentRow);
	CBCGPGridRow* pChildRow = NULL;
	hr = CreateRowFactorInfoItem(_pParentRow,pChildRow);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pChildRow);
	ASSERT(_pFactorInfoItem);
	CComVariant valFactorName;
	hr = _pFactorInfoItem->GetPropVal(FIELD_YDFACTORINFOITEM_FACTORNAME,&valFactorName);
	if(FAILED(hr))
	{
		return hr;
	}
	pChildRow->GetItem(cColFactorName)->SetValue(valFactorName);

	CComVariant valFieldName;
	hr = _pFactorInfoItem->GetPropVal(FIELD_YDFACTORINFOITEM_FIELDNAME,&valFieldName);
	if(FAILED(hr))
	{
		return hr;
	}
	pChildRow->GetItem(cColFieldName)->SetValue(valFieldName);
	CString strFieldName = CDataHandler::VariantToString(valFieldName);
	CFactorInfoHelper helper;
	if(helper.IsNumberFieldName(strFieldName))
	{
		pChildRow->GetItem(cColMin)->Enable(TRUE);
		pChildRow->GetItem(cColMax)->Enable(TRUE);
	}
	else
	{
		pChildRow->GetItem(cColMin)->Enable(FALSE);
		pChildRow->GetItem(cColMax)->Enable(FALSE);
	}
	CComVariant valMin;
	hr = _pFactorInfoItem->GetPropVal(FIELD_YDFACTORINFOITEM_MIN,&valMin);
	if(FAILED(hr))
	{
		return hr;
	}
	pChildRow->GetItem(cColMin)->SetValue(valMin);

	CComVariant valMax;
	hr = _pFactorInfoItem->GetPropVal(FIELD_YDFACTORINFOITEM_MAX,&valMax);
	if(FAILED(hr))
	{
		return hr;
	}
	pChildRow->GetItem(cColMax)->SetValue(valMax);

	CComVariant valDesc;
	hr = _pFactorInfoItem->GetPropVal(FIELD_YDFACTORINFOITEM_DESCRIPTION,&valDesc);
	if(FAILED(hr))
	{
		return hr;
	}
	pChildRow->GetItem(cColDes)->SetValue(valDesc);
	return S_OK;
}

HRESULT CDlgYDVaultFactorInfoConfig::CreateRowFactorInfoItem(CBCGPGridRow* _pParentRow,
														CBCGPGridRow* &_pChildRow)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pParentRow);
	_pChildRow = m_Grid.CreateRow(m_Grid.GetColumnCount());
	_pChildRow->GetItem(cColQTypeID)->Enable(FALSE);

	_pChildRow->GetItem(cColFactorName)->Enable(TRUE);
	CBCGPGridItem* pItem = new CBCGPGridItem("");
	for(int i = 1; i <= 25;i++)
	{
		CString strName;
		strName.Format(_T("C%d"),i);
		pItem->AddOption(strName);
	}
	for(int i = 1; i <= 25;i++)
	{
		CString strName;
		strName.Format(_T("D%d"),i);
		pItem->AddOption(strName);
	}
	_pChildRow->ReplaceItem (cColFieldName, pItem);
	_pChildRow->GetItem(cColFieldName)->Enable(TRUE);
//	pItem->
	_pChildRow->GetItem(cColMin)->Enable(TRUE);
	//_pChildRow->GetItem(cColMin)->
	_pChildRow->GetItem(cColMax)->Enable(TRUE);
	_pChildRow->GetItem(cColDes)->Enable(TRUE);
	_pChildRow->AllowSubItems();
	_pParentRow->AddSubItem(_pChildRow);
	return S_OK;
}


void CDlgYDVaultFactorInfoConfig::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	CBCGPGridRow *pSelRow = 	m_Grid.GetCurSel();
	if(pSelRow == NULL)
	{
		AfxMessageBox(_T("请至少选中一条要选中的记录！"));
		return ;
	}
	CBCGPGridRow *pParentRow = pSelRow->GetParent();
	if(pParentRow == NULL)
	{
		pParentRow = pSelRow;
	}
	HRESULT hr = E_FAIL;
	CBCGPGridRow *pChildRow = NULL;
	hr = CreateRowFactorInfoItem(pParentRow,pChildRow);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
}


void CDlgYDVaultFactorInfoConfig::OnBnClickedButtonDel()
{
	// TODO: Add your control notification handler code here
	CBCGPGridRow *pSelRow = 	m_Grid.GetCurSel();
	if(pSelRow == NULL)
	{
		AfxMessageBox(_T("请至少选中一条要选中的记录！"));
		return ;
	}
	if(pSelRow->GetParent() == NULL)
	{
		AfxMessageBox(_T("题型不能删除!"));
		return ;
	}

	if(AfxMessageBox(_T("你确定要删除选择的行吗？"),MB_YESNO) != IDYES)
	{
		return;
	}
	for(int i = 0 ; i <m_Grid.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_Grid.GetRow(i);
		if(pRow == pSelRow)
		{
			m_Grid.RemoveRow(i);
			break;
		}
	}
}

BOOL CDlgYDVaultFactorInfoConfig::ValidateData()
{
	for(int i = 0; i < m_Grid.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_Grid.GetRow(i);
		ASSERT(pRow);
		CBCGPGridRow* pParentRow = pRow->GetParent();
		if(pParentRow)
		{
			continue;
		}
		if(!ValidateDataByRowQType(pRow))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDlgYDVaultFactorInfoConfig::ValidateDataByRowQType(CBCGPGridRow* _pRowQType)
{
	ASSERT(_pRowQType);
	CYDQuestionType* pQType = (CYDQuestionType*)_pRowQType->GetData();
	ASSERT(pQType);
	std::list<CString> lstFactorName,lstFieldName;//用了校正指标名称和预留字段名称不能重复
	CFactorInfoHelper helper;
	for(int i = 0; i < _pRowQType->GetSubItemsCount();i++)
	{
		CBCGPGridRow* pRow = _pRowQType->GetSubItem(i);
		ASSERT(pRow);
		CComVariant valFactorName = pRow->GetItem(cColFactorName)->GetValue();
		CString strFactorName = CDataHandler::VariantToString(valFactorName);
		if(strFactorName.IsEmpty())
		{
			CString strMsg = CreateInvalidateMsg(pQType,i+1,_T("指标名称不能为空！"));
			AfxMessageBox(strMsg);
			return FALSE;
		}
		for(std::list<CString>::const_iterator itr = lstFactorName.begin();
			itr != lstFactorName.end();++itr)
		{
			if((*itr).CompareNoCase(strFactorName) == 0)
			{
				CString strLabel;
				pQType->GetLabel(&strLabel);
				CString strMsg;
				strMsg.Format(_T("%s中第%d行指标名称%s不能重复！"),strLabel,i+1,strFactorName);
				AfxMessageBox(strMsg);
				return FALSE;
			}
		}
		lstFactorName.push_back(strFactorName);
		CComVariant valFieldName = pRow->GetItem(cColFieldName)->GetValue();
		CString strFiledName = CDataHandler::VariantToString(valFieldName);
		if(strFiledName.IsEmpty())
		{
			CString strMsg = CreateInvalidateMsg(pQType,i+1,_T("预留字段名称不能为空！"));
			AfxMessageBox(strMsg);
			return FALSE;
		}
		for(std::list<CString>::const_iterator itr = lstFieldName.begin();
			itr != lstFieldName.end();++itr)
		{
			if((*itr).CompareNoCase(strFiledName) == 0)
			{
				CString strLabel;
				pQType->GetLabel(&strLabel);
				CString strMsg;
				strMsg.Format(_T("%s中第%d行预留字段名称%s不能重复！"),strLabel,i+1,strFiledName);
				AfxMessageBox(strMsg);
				return FALSE;
			}
		}
		lstFieldName.push_back(strFiledName);
		//要判断strFiledName是否为D1--D25,当时数值型的时候，要判断最小值和最大值
		if(!helper.IsNumberFieldName(strFiledName))
		{
			//字符串型，不用校验最大最小值
			continue;
		}
		CComVariant valMin = pRow->GetItem(cColMin)->GetValue();
		CString strMin = CDataHandler::VariantToString(valMin);
		if(strMin.IsEmpty())
		{
			CString strMsg = CreateInvalidateMsg(pQType,i+1,_T("最小值不能为空！"));
			AfxMessageBox(strMsg);
			return FALSE;
		}
		if(!CDataHandler::StringIsDigit(strMin))
		{
			CString strMsg = CreateInvalidateMsg(pQType,i+1,_T("最小值应该为数值型！"));
			AfxMessageBox(strMsg);
			return FALSE;
		}
		
		CComVariant valMax = pRow->GetItem(cColMax)->GetValue();
		CString strMax = CDataHandler::VariantToString(valMax);
		if(strMax.IsEmpty())
		{
			CString strMsg = CreateInvalidateMsg(pQType,i+1,_T("最大值不能为空！"));
			AfxMessageBox(strMsg);
			return FALSE;
		}
		if(!CDataHandler::StringIsDigit(strMin))
		{
			CString strMsg = CreateInvalidateMsg(pQType,i+1,_T("最大值应该为数值型！"));
			AfxMessageBox(strMsg);
			return FALSE;
		}
		long lMin = CDataHandler::VariantToLong(valMin);
		long lMax = CDataHandler::VariantToLong(valMax);
		if(lMin > lMax)
		{
			CString strMsg;
			CString strLabel;
			pQType->GetLabel(&strLabel);
			strMsg.Format(_T("%s中第%d行最小值%d不能大于最大值%d！"),strLabel,i+1,lMin,lMax);
			AfxMessageBox(strMsg);
			return FALSE;
		}
	}
	return TRUE;
}

CString	CDlgYDVaultFactorInfoConfig::CreateInvalidateMsg(CYDQuestionType* _pQType,int _iRow,CString _strName)
{
	CString strLabel;
	_pQType->GetLabel(&strLabel);
	CString strMsg;
	strMsg.Format(_T("%s中第%d行%s"),strLabel,_iRow,_strName);
	return strMsg;
}

LRESULT CDlgYDVaultFactorInfoConfig::OnItemChanged(WPARAM wParam,LPARAM lParam)
{
	BCGPGRID_ITEM_INFO* item_info = (BCGPGRID_ITEM_INFO*)lParam;
	ASSERT(item_info != NULL);
	int nCol = item_info->nCol;
	if(nCol == cColFieldName)
	{
		int nRow = item_info->nRow;
		CBCGPGridRow *pRow = m_Grid.GetRow(nRow);
		ASSERT(pRow != NULL);
		CBCGPGridItem *pItem = pRow->GetItem(cColFieldName);
		ASSERT(pItem != NULL);
		_variant_t val = pItem->GetValue();
		CString strFieldName = CDataHandler::VariantToString(val);
		CFactorInfoHelper helper;
		if(helper.IsNumberFieldName(strFieldName))
		{
			pRow->GetItem(cColMin)->Enable(TRUE);
			pRow->GetItem(cColMax)->Enable(TRUE);
		}
		else
		{
			pRow->GetItem(cColMin)->Enable(FALSE);
			pRow->GetItem(cColMax)->Enable(FALSE);
		}
	}
	return 1L;
}

