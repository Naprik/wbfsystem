// DlYDVaultQuestionFactorInfoConfig.cpp : implementation file
//

#include "stdafx.h"
#include "DlYDVaultQuestionFactorInfoConfig.h"
#include "afxdialogex.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YDObjectRef.h"
#include "FactorInfoHelper.h"
#include "../Include/ShowErr.h"
#include "../Base/DataHandler.h"
#include "../DBBase/DBTransactionRef.h"


// CDlYDVaultQuestionFactorInfoConfig dialog
const int cColPropName = 0;//属性名
const int cColPropVal = 1;//属性值

IMPLEMENT_DYNAMIC(CDlYDVaultQuestionFactorInfoConfig, CDialogEx)

CDlYDVaultQuestionFactorInfoConfig::CDlYDVaultQuestionFactorInfoConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlYDVaultQuestionFactorInfoConfig::IDD, pParent)
{
	m_pVault = NULL;
	m_pQType = NULL;
	m_pQuestion = NULL;
}

CDlYDVaultQuestionFactorInfoConfig::~CDlYDVaultQuestionFactorInfoConfig()
{
	CListAutoClean<CYDObjectRef> clr(m_lstFactorInfoItem);
}

void CDlYDVaultQuestionFactorInfoConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlYDVaultQuestionFactorInfoConfig, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlYDVaultQuestionFactorInfoConfig::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlYDVaultQuestionFactorInfoConfig message handlers


BOOL CDlYDVaultQuestionFactorInfoConfig::OnInitDialog()
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
	m_Grid.InsertColumn(cColPropName, L"属性名", 150);
	m_Grid.InsertColumn(cColPropVal, L"属性值", 150);
	ASSERT(m_pVault);
	ASSERT(m_pQType);
	ASSERT(m_pQuestion);
	CFactorInfoHelper helper;
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	hr = helper.GetFactorInfoByVaultQType(pDB,m_pVault,m_pQType,&m_lstFactorInfoItem);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return FALSE;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = m_lstFactorInfoItem.begin();
		itr != m_lstFactorInfoItem.end();++itr)
	{
		CComVariant valFactorName;
		hr = (*itr)->GetPropVal(FIELD_YDFACTORINFOITEM_FACTORNAME,&valFactorName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return FALSE;
		}
		CString strFieldName;
		hr = (*itr)->GetPropVal(FIELD_YDFACTORINFOITEM_FIELDNAME,strFieldName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return FALSE;
		}
		CComVariant val;
		hr = m_pQuestion->GetPropVal(CComBSTR(strFieldName),&val);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return FALSE;
		}
		//插入记录
		CBCGPGridRow* pRow = m_Grid.CreateRow(m_Grid.GetColumnCount());
		pRow->GetItem(cColPropName)->SetValue(valFactorName);
		pRow->GetItem(cColPropName)->Enable(FALSE);
		pRow->GetItem(cColPropVal)->SetValue(val);
		pRow->GetItem(cColPropVal)->Enable(TRUE);
		m_Grid.AddRow(pRow);
	}
	m_Grid.SetSpaceRowColor(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlYDVaultQuestionFactorInfoConfig::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	HRESULT hr = E_FAIL;
	ASSERT(m_pQuestion);
	for(int i = 0; i < m_Grid.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_Grid.GetRow(i);
		ASSERT(pRow);
		CComVariant valFactorName = pRow->GetItem(cColPropName)->GetValue();
		CComVariant valPropVal = pRow->GetItem(cColPropVal)->GetValue();
		CString strFactorName = CDataHandler::VariantToString(valFactorName);
		CString strFieldName;
		for(std::list<CYDObjectRef*>::const_iterator itr = m_lstFactorInfoItem.begin();
			itr != m_lstFactorInfoItem.end();++itr)
		{
			CString strItrFactorName;
			hr = (*itr)->GetPropVal(FIELD_YDFACTORINFOITEM_FACTORNAME,strItrFactorName);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
				return ;
			}
			if(strItrFactorName.CompareNoCase(strFactorName) == 0)
			{
				hr = (*itr)->GetPropVal(FIELD_YDFACTORINFOITEM_FIELDNAME,strFieldName);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
					return ;
				}
				break;
			}
		}
		hr = m_pQuestion->SetPropVal(CComBSTR(strFieldName),&valPropVal);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}
	}
	//保存到数据库
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef trans(pDB, TRUE);
	trans.Begin();
	hr = m_pQuestion->Update();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	hr = trans.Commit();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	CDialogEx::OnOK();
}
