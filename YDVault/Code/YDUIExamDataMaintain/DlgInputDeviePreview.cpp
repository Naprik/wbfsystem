// DlgInputDeviePreview.cpp : implementation file
//

#include "stdafx.h"
#include "DlgInputDeviePreview.h"
#include "../DBBase\DatabaseEx.h"
#include "../DBBase\DBTransactionRef.h"
#include "../YDExamObjRef\YDDevice.h"
#include "../Base\AutoClean.h"

// CDlgInputDeviePreview dialog

IMPLEMENT_DYNAMIC(CDlgInputDeviePreview, CDialog)

CDlgInputDeviePreview::CDlgInputDeviePreview(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputDeviePreview::IDD, pParent)
{

}

CDlgInputDeviePreview::~CDlgInputDeviePreview()
{
}

void CDlgInputDeviePreview::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInputDeviePreview, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgInputDeviePreview::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgInputDeviePreview::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgInputDeviePreview::OnBnClickedButtonDel)
END_MESSAGE_MAP()


// CDlgInputDeviePreview message handlers

INT_PTR CDlgInputDeviePreview::DoModal()
{
	// TODO: Add your specialized code here and/or call the base class
	CExtDllState state;
	return CDialog::DoModal();
}

BOOL CDlgInputDeviePreview::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	m_Grid.EnableColumnAutoSize (TRUE);
	m_Grid.EnableGroupByBox (FALSE);
	m_Grid.SetWholeRowSel(FALSE);
	m_Grid.EnableHeader (TRUE, 0);

	CBCGPGridColors colors;
	colors.m_LeftOffsetColors.m_clrBackground = globalData.clrWindow;
	m_Grid.SetColorTheme (colors);
	int iWidthGrid = rectBK.Width();
	//m_Grid.InsertColumn(0, L"题号", 50);
	int iColIndex = 1;
	ASSERT(m_lstPropVal.size() > 0);
	BOOL bFirst = TRUE;
	for(std::list <std::list<std::pair<CString,CString> > >::const_iterator itrLst = m_lstPropVal.begin();
		itrLst != m_lstPropVal.end();++itrLst)
	{
		std::list<std::pair<CString,CString> > lst = (*itrLst);
		int iCol = 0;
		if(bFirst)
		{
			bFirst = FALSE;
			for(std::list<std::pair<CString,CString> >::const_iterator itr = lst.begin();
				itr != lst.end();++itr)
			{
				m_Grid.InsertColumn(iCol++, (*itr).first, 80);
			}
		}
		iCol = 0;
		CBCGPGridRow* pRow = m_Grid.CreateRow(m_Grid.GetColumnCount());
		ASSERT(pRow);

		for(std::list<std::pair<CString,CString> >::const_iterator itr = lst.begin();
			itr != lst.end();++itr)
		{
			pRow->GetItem(iCol++)->SetValue(CComVariant((*itr).second));
		}
		m_Grid.AddRow(pRow);
	}
	m_Grid.AdjustLayout ();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgInputDeviePreview::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef TransRef(pDB,TRUE);
	/*
	AddPropDef(FIELD_YDDEVICE_ID, VT_BSTR);	//设备编号
	AddPropDef(FIELD_YDDEVICE_MAC,VT_BSTR);	//蓝牙地址
	*/

	for(int i = 0 ; i < m_Grid.GetRowCount();i++)
	{
		CBCGPGridRow* pRow = m_Grid.GetRow(i);
		ASSERT(pRow);
		CYDDevice* pDeviceRef = new CYDDevice(pDB);
		CPtrAutoClean<CYDDevice> clr(pDeviceRef);
		for(int iCol = 0; iCol < m_Grid.GetColumnCount();iCol++)
		{
			CString strColName = m_Grid.GetColumnName(iCol);
			CComVariant val = pRow->GetItem(iCol)->GetValue();
			if(strColName.CompareNoCase(_T("设备编号")) == 0)
			{
				hr = pDeviceRef->SetPropVal(FIELD_YDDEVICE_ID,&val);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
					return ;
				}
			}
			else if(strColName.CompareNoCase(_T("蓝牙地址")) == 0)
			{
				hr = pDeviceRef->SetPropVal(FIELD_YDDEVICE_MAC,&val);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
					return ;
				}

			}
		}
		hr = pDeviceRef->Save();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return ;
		}
	}
	hr = TransRef.Commit();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
		return ;
	}
	AfxMessageBox(_T("导入成功！"));
	OnOK();
}

void CDlgInputDeviePreview::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	CBCGPGridRow* pRow = m_Grid.CreateRow(m_Grid.GetColumnCount());
	ASSERT(pRow);

	for(int iCol = 0; iCol < m_Grid.GetColumnCount();iCol++)
	{
		pRow->GetItem(iCol++)->SetValue(L"");
	}
	m_Grid.AddRow(pRow);
}

void CDlgInputDeviePreview::OnBnClickedButtonDel()
{
	// TODO: Add your control notification handler code here
	CBCGPGridRow* pSelRow = m_Grid.GetCurSel();
	if(pSelRow == NULL)
	{
		AfxMessageBox(_T("请在列表中选择要删除的行"));
		return;
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
