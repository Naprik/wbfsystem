// DlgVaultLevelConfig.cpp : implementation file
//

#include "stdafx.h"
#include "DlgVaultLevelConfig.h"
#include "afxdialogex.h"
#include "../ObjHelper\StaticObjHelper.h"
#include "../ObjRef\PropQueryContidition.h"
#include "../Base\AutoClean.h"
#include "../ObjRef/YDObjectRef.h"
#include "../Base/DataHandler.h"
#include "../DBBase/DBTransactionRef.h"
#include "../ObjRef/YDVaultLevel.h"

const int cColSerial = 0;//序号
const int cColLevelDesc = 1;//等级描述
const int cColCondition = 2;//正确率

// CDlgVaultLevelConfig dialog

IMPLEMENT_DYNAMIC(CDlgVaultLevelConfig, CDialogEx)

CDlgVaultLevelConfig::CDlgVaultLevelConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgVaultLevelConfig::IDD, pParent)
{
	m_pVault = NULL;
}

CDlgVaultLevelConfig::~CDlgVaultLevelConfig()
{
	CListAutoClean<CYDObjectRef> clr(m_lstVaultLevel);
}

void CDlgVaultLevelConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgVaultLevelConfig, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgVaultLevelConfig::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgVaultLevelConfig::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDOK, &CDlgVaultLevelConfig::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgVaultLevelConfig message handlers


BOOL CDlgVaultLevelConfig::OnInitDialog()
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
	
	m_Grid.InsertColumn(cColSerial, L"序号", 100);
	m_Grid.InsertColumn(cColLevelDesc, L"等级描述", 150);
	m_Grid.InsertColumn(cColCondition, L"正确率", 100);

	OBJID vaultID = 0;
	HRESULT hr = E_FAIL;
	ASSERT(m_pVault);
	hr = m_pVault->GetID(&vaultID);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	CString strVaultID;
	strVaultID.Format(_T("%d"),vaultID);
	std::list<CYDObjectRef*> lstLevel;
	std::list<CPropQueryContidition*> lstQueryCon;
	CListAutoClean<CPropQueryContidition> clr2(lstQueryCon);
	CPropQueryContidition* pQueryConditon = new CPropQueryContidition();
	lstQueryCon.push_back(pQueryConditon);
	pQueryConditon->m_uOpType = Q_EQUAL;
	pQueryConditon->m_strFiledName = FIELD_VAULTLEVEL_VAULTID;
	pQueryConditon->m_strConVal = strVaultID;
	pQueryConditon->m_uFieldType = VT_I4; 

	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);

	hr = CStaticObjHelper::GetObjRef(DB_VAULTLEVEL,pDB,&m_lstVaultLevel,&lstQueryCon);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = m_lstVaultLevel.begin();
		itr != m_lstVaultLevel.end();++itr)
	{
		CString strLevelDesc;
		hr = (*itr)->GetPropVal(FIELD_VAULTLEVEL_LEVEL,strLevelDesc);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		CBCGPGridRow* pRow = m_Grid.CreateRow(m_Grid.GetColumnCount());
		m_Grid.AddRow(pRow);
		CString strSerial;
		strSerial.Format(_T("%d"),m_Grid.GetRowCount());
		pRow->GetItem(cColSerial)->SetValue(CComVariant(strSerial));
		pRow->GetItem(cColSerial)->Enable(FALSE);
		pRow->GetItem(cColLevelDesc)->SetValue(CComVariant(strLevelDesc));
		long lCondition = 0;
		hr= (*itr)->GetPropVal(FIELD_VAULTLEVEL_CONDITION,&lCondition);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		pRow->GetItem(cColCondition)->SetValue(CComVariant(lCondition));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgVaultLevelConfig::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	CBCGPGridRow* pRow = m_Grid.CreateRow(m_Grid.GetColumnCount());
	m_Grid.AddRow(pRow);
	CString strSerial;
	strSerial.Format(_T("%d"),m_Grid.GetRowCount());
	pRow->GetItem(cColSerial)->SetValue(CComVariant(strSerial));
	pRow->GetItem(cColSerial)->Enable(FALSE);
	pRow->GetItem(cColLevelDesc)->SetValue(CComVariant(L""));
	pRow->GetItem(cColCondition)->SetValue(CComVariant(0));
}


void CDlgVaultLevelConfig::OnBnClickedButtonDel()
{
	// TODO: Add your control notification handler code here
	CBCGPGridRow *pSelRow = 	m_Grid.GetCurSel();
	if(pSelRow == NULL)
	{
		AfxMessageBox(_T("请至少选中一条要选中的记录！"));
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

HRESULT CDlgVaultLevelConfig::ValidateData(BOOL &_bValidate)
{
	HRESULT hr = E_FAIL;
	_bValidate = FALSE;
	for(int i = 0 ; i <m_Grid.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_Grid.GetRow(i);
		CComVariant valSerial = pRow->GetItem(cColSerial)->GetValue();
		CString strSerial = CDataHandler::VariantToString(valSerial);
		CComVariant valDesc = pRow->GetItem(cColLevelDesc)->GetValue();
		CString strDesc = CDataHandler::VariantToString(valDesc);
		if(strDesc.IsEmpty())
		{
			CString strMsg;
			strMsg.Format(_T("第%s行请输入等级描述！"),strSerial);
			AfxMessageBox(strMsg);
			return S_OK;
		}
		CComVariant valCondition = pRow->GetItem(cColCondition)->GetValue();
		CString strCondition = CDataHandler::VariantToString(valCondition);
		if(strCondition.IsEmpty())
		{
			CString strMsg;
			strMsg.Format(_T("第%s行请输入正确率！"),strSerial);
			AfxMessageBox(strMsg);
			return S_OK;
		}
		if(!CDataHandler::StringIsDigit(strCondition))
		{
			CString strMsg;
			strMsg.Format(_T("第%s行正确率请输入数值型！"),strSerial);
			AfxMessageBox(strMsg);
			return S_OK;
		}
		long lConditio = CDataHandler::VariantToLong(valCondition);
		if(lConditio < 0 || lConditio > 100)
		{
			CString strMsg;
			strMsg.Format(_T("第%s行正确率应该在0到100之间！"),strSerial);
			AfxMessageBox(strMsg);
			return S_OK;
		}
	}
	_bValidate = TRUE;
	return S_OK;
}


void CDlgVaultLevelConfig::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	HRESULT hr = E_FAIL;
	BOOL bValidate = FALSE;
	hr = ValidateData(bValidate);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	if(!bValidate)
	{
		return;
	}
	ASSERT(m_pVault);
	OBJID vaultID = 0;
	hr = m_pVault->GetID(&vaultID);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CComVariant valVaultID(vaultID);
	//启动事务，先将原来的删除，插入新的
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);

	CDBTransactionRef TransRef(pDB,TRUE);
	CWaitCursor cursor;
	for(std::list<CYDObjectRef*>::const_iterator itr = m_lstVaultLevel.begin();
		itr != m_lstVaultLevel.end();++itr)
	{
		hr = (*itr)->Remove();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
	}
	for(int i = 0 ; i <m_Grid.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_Grid.GetRow(i);
		CComVariant valDesc = pRow->GetItem(cColLevelDesc)->GetValue();
		CComVariant valCondition = pRow->GetItem(cColCondition)->GetValue();
		long lCondition = CDataHandler::VariantToLong(valCondition);
		CYDVaultLevel YDVaultLevel(pDB);
		YDVaultLevel.SetPropVal(FIELD_VAULTLEVEL_VAULTID,&valVaultID);
		YDVaultLevel.SetPropVal(FIELD_VAULTLEVEL_LEVEL,&valDesc);
		YDVaultLevel.SetPropVal(FIELD_VAULTLEVEL_CONDITION,&valCondition);
		hr = YDVaultLevel.Save();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
	}

	hr = TransRef.Commit();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	CDialogEx::OnOK();
}
