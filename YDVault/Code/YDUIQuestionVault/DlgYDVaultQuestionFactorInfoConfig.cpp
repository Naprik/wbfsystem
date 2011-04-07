// DlYDVaultQuestionFactorInfoConfig.cpp : implementation file
//

#include "stdafx.h"
#include "DlgYDVaultQuestionFactorInfoConfig.h"
#include "afxdialogex.h"
#include "../Base/AutoClean.h"
#include "../ObjRef/YDObjectRef.h"
#include "../ObjHelper/FactorInfoHelper.h"
#include "../Include/ShowErr.h"
#include "../Base/DataHandler.h"
#include "../DBBase/DBTransactionRef.h"


// CDlgYDVaultQuestionFactorInfoConfig dialog
const int cColPropName = 0;//������
const int cColPropVal = 1;//����ֵ

IMPLEMENT_DYNAMIC(CDlgYDVaultQuestionFactorInfoConfig, CDialogEx)

CDlgYDVaultQuestionFactorInfoConfig::CDlgYDVaultQuestionFactorInfoConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgYDVaultQuestionFactorInfoConfig::IDD, pParent)
{
	m_pVault = NULL;
	m_pQType = NULL;
	m_pQuestion = NULL;
}

CDlgYDVaultQuestionFactorInfoConfig::~CDlgYDVaultQuestionFactorInfoConfig()
{
	CListAutoClean<CYDObjectRef> clr(m_lstFactorInfoItem);
}

void CDlgYDVaultQuestionFactorInfoConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgYDVaultQuestionFactorInfoConfig, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgYDVaultQuestionFactorInfoConfig::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgYDVaultQuestionFactorInfoConfig message handlers


BOOL CDlgYDVaultQuestionFactorInfoConfig::OnInitDialog()
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
	m_Grid.InsertColumn(cColPropName, L"������", 150);
	m_Grid.InsertColumn(cColPropVal, L"����ֵ", 150);
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
		CYDPropDef* pPropDef = NULL;
		hr = m_pQuestion->GetPropDef(CComBSTR(strFieldName),pPropDef);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return FALSE ;
		}
		CComVariant val;
		hr = m_pQuestion->GetPropVal(CComBSTR(strFieldName),&val);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return FALSE;
		}
		if(pPropDef->m_uDataType == VT_BSTR)
		{
			//�ַ����ͣ����valΪ�գ�Ҫ�����޸�Ϊ" ",�м�Ӹ��ո񣬷�����Grid�в�������ֵ
			CString strVal = CDataHandler::VariantToString(val);
			if(strVal.IsEmpty())
			{
				strVal = _T(" ");
				CDataHandler::StringToVariant(strVal,VT_BSTR,&val);
			}
		}
		else if(pPropDef->m_uDataType == VT_I4)
		{
			//��ֵ�ͣ����valΪ�գ�Ҫ�����޸�Ϊ0,������Grid�в�������ֵ
			long lVal = CDataHandler::VariantToLong(val);
			val = lVal;
		}
		//�����¼
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


void CDlgYDVaultQuestionFactorInfoConfig::OnBnClickedOk()
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
		CYDObjectRef* pFactorInfo = NULL;
		hr = GetFactorInfoItemByFactorName(strFactorName,pFactorInfo);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}
		ASSERT(pFactorInfo);
		CString strFieldName;
		hr = pFactorInfo->GetPropVal(FIELD_YDFACTORINFOITEM_FIELDNAME,strFieldName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}
		ASSERT(!strFieldName.IsEmpty());
		//У��ֵ
		CYDPropDef* pPropDef = NULL;
		hr = m_pQuestion->GetPropDef(CComBSTR(strFieldName),pPropDef);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}
		ASSERT(pPropDef);
		if(pPropDef->m_uDataType == VT_I4)
		{
			//ҪУ��ֵ�����ֵ����Сֵ֮��
			long lPropVal = CDataHandler::VariantToLong(valPropVal);
			long lValMin  = 0;
			hr = pFactorInfo->GetPropVal(FIELD_YDFACTORINFOITEM_MIN,&lValMin);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
				return ;
			} 
			long lValMax  = 0;
			hr = pFactorInfo->GetPropVal(FIELD_YDFACTORINFOITEM_MAX,&lValMax);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
				return ;
			} 
			if(lPropVal < lValMin || lPropVal > lValMax)
			{
				CString strMsg;
				strMsg.Format(_T("%s��ֵӦ����%d��%d֮��"),strFactorName,lValMin,lValMax);
				AfxMessageBox(strMsg);
				return;
			}
		}
		hr = m_pQuestion->SetPropVal(CComBSTR(strFieldName),&valPropVal);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}
	}
	//���浽���ݿ�
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

HRESULT CDlgYDVaultQuestionFactorInfoConfig::GetFactorInfoItemByFactorName(CString _strFactorName,CYDObjectRef* &_pFactorInfo)
{
	HRESULT hr = E_FAIL;
	for(std::list<CYDObjectRef*>::const_iterator itr = m_lstFactorInfoItem.begin();
		itr != m_lstFactorInfoItem.end();++itr)
	{
		CString strItrFactorName;
		hr = (*itr)->GetPropVal(FIELD_YDFACTORINFOITEM_FACTORNAME,strItrFactorName);
		if(FAILED(hr))
		{
			return hr;
		}
		if(strItrFactorName.CompareNoCase(_strFactorName) == 0)
		{
			_pFactorInfo = *itr;
			break;
		}
	}
	return S_OK;
}