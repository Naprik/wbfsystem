// ChoiceAnsys.cpp : implementation file
//

#include "stdafx.h"
#include "ChoiceAnsys.h"


// CChoiceAnsys dialog

IMPLEMENT_DYNAMIC(CChoiceAnsys, CBCGPDialog)

CChoiceAnsys::CChoiceAnsys(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CChoiceAnsys::IDD, pParent)
{

}

CChoiceAnsys::~CChoiceAnsys()
{
}

void CChoiceAnsys::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA_OP_CHOICEANSYS, m_ChoiceAnsysList);
}


BEGIN_MESSAGE_MAP(CChoiceAnsys, CBCGPDialog)
END_MESSAGE_MAP()


// CChoiceAnsys message handlers

BOOL CChoiceAnsys::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	CRect rect1;
	m_ChoiceAnsysList.GetWindowRect(rect1);
	int iWidth = rect1.Width();
	m_ChoiceAnsysList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ChoiceAnsysList.InsertColumn(0, L"选项", LVCFMT_LEFT, iWidth*2/6);
	m_ChoiceAnsysList.InsertColumn(1, L"选择人数", LVCFMT_LEFT,iWidth*4/6 - 4);
	
	std::map<CString, UINT>::iterator itr = m_pRes->m_mapAnswer.begin();
	for (; itr != m_pRes->m_mapAnswer.end(); ++itr)
	{
		int index = m_ChoiceAnsysList.InsertItem(m_ChoiceAnsysList.GetItemCount(), itr->first);
		CString strCount;
		strCount.Format(L"%d", itr->second);
		m_ChoiceAnsysList.SetItemText(index,1, strCount);
		m_ChoiceAnsysList.SetItemData(index, (DWORD_PTR)(index));
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
