// YDVaultFactorInfoConfig.cpp : implementation file
//

#include "stdafx.h"
#include "YDVaultFactorInfoConfig.h"
#include "afxdialogex.h"
#include <list>
#include "../ObjRef/YDQuestionType.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YDQuestionVault.h"
#include "../Include/ShowErr.h"
#include "../ObjHelper/StaticObjHelper.h"
#include "../ObjRef/YDObjectRef.h"
#include "../ObjRef/PropQueryContidition.h"



// CYDVaultFactorInfoConfig dialog
const int cColQTypeID = 0;//题型ID
const int cColFactorName = 1;//指标名称
const int cColFieldName = 2;//预留字段名称
const int cColMin = 3;//数值型下界
const int cColMax = 4;//数值型上界
const int cColDes = 5;//描述

IMPLEMENT_DYNAMIC(CYDVaultFactorInfoConfig, CDialogEx)

CYDVaultFactorInfoConfig::CYDVaultFactorInfoConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CYDVaultFactorInfoConfig::IDD, pParent)
{
	m_pVault = NULL;
}

CYDVaultFactorInfoConfig::~CYDVaultFactorInfoConfig()
{
	m_pVault = NULL;
	CListAutoClean<CYDQuestionType> clr(m_lstType);
	CListAutoClean<CYDObjectRef> clr2(m_lstOldFactorInfoItem);
}

void CYDVaultFactorInfoConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CYDVaultFactorInfoConfig, CDialogEx)
	ON_BN_CLICKED(IDOK, &CYDVaultFactorInfoConfig::OnBnClickedOk)
END_MESSAGE_MAP()


// CYDVaultFactorInfoConfig message handlers


void CYDVaultFactorInfoConfig::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


BOOL CYDVaultFactorInfoConfig::OnInitDialog()
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

HRESULT CYDVaultFactorInfoConfig::InsertByQuestionType(CYDQuestionType* _pQType)
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
	std::list<CPropQueryContidition*> lstQueryCon;
	CListAutoClean<CPropQueryContidition> clr(lstQueryCon);
	//当前题库
	OBJID idVault = 0;
	hr = m_pVault->GetID(&idVault);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strIDVault;
	strIDVault.Format(_T("%d"),idVault);
	CPropQueryContidition* pPropQueryContidition = new CPropQueryContidition();
	pPropQueryContidition->m_uFieldType = VT_I4;
	pPropQueryContidition->m_uOpType = Q_EQUAL;
	pPropQueryContidition->m_strFiledName  = FIELD_YDFACTORINFOITEM_VAULTID;
	pPropQueryContidition->m_strConVal = strIDVault;
	lstQueryCon.push_back(pPropQueryContidition);
	//_pQType
	OBJID idType = 0;
	hr = _pQType->GetID(&idType);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strIDType;
	strIDType.Format(_T("%d"),idType);
	pPropQueryContidition = new CPropQueryContidition();
	pPropQueryContidition->m_uFieldType = VT_I4;
	pPropQueryContidition->m_uOpType = Q_EQUAL;
	pPropQueryContidition->m_strFiledName  = FIELD_YDFACTORINFOITEM_QTYPEID;
	pPropQueryContidition->m_strConVal = strIDType;
	lstQueryCon.push_back(pPropQueryContidition);

	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	std::list<CYDObjectRef*> lstFactorInfoItem;
	hr = CStaticObjHelper::GetObjRef(DB_YDFACTORINFOITEM,pDB,&lstFactorInfoItem,&lstQueryCon);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = lstFactorInfoItem.begin();
		itr != lstFactorInfoItem.end();++itr)
	{
		m_lstOldFactorInfoItem.push_back(*itr);
	}
	return S_OK;
}
