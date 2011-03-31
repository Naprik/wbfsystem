// Setpass.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"

CSetpass::CSetpass(CWnd* pParent /*=NULL*/)
	: CDialog(CSetpass::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetpass)
	m_setpass1 = _T("");
	m_setpass2 = _T("");
	m_name = _T(nametemp);
	//}}AFX_DATA_INIT
}


void CSetpass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetpass)
	DDX_Text(pDX, IDC_EDIT1, m_setpass1);
	DDX_Text(pDX, IDC_EDIT2, m_setpass2);
	DDX_Text(pDX, IDC_STATICN, m_name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetpass, CDialog)
	//{{AFX_MSG_MAP(CSetpass)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetpass message handlers

void CSetpass::OnOK() 
{
	// TODO: Add extra validation here
	
	UpdateData( TRUE );
	m_setpass1.TrimLeft();
	m_setpass1.TrimRight();
	m_setpass2.TrimLeft();
	m_setpass2.TrimRight();
	if (m_setpass1 == "" || m_setpass2 == "")
	{
		AfxMessageBox("����ȷ��д����!");
		return;
	}
	if (m_setpass1 != m_setpass2)
	{
		AfxMessageBox("������������벻ͬ!");
		return;
	}
//    CoInitialize(NULL);
// 	CDbUser user;
// 	if ( !user.Connect())
// 	{
// 		CoUninitialize();
// 		AfxMessageBox("�޷����ӵ����ݿ������!");
// 		return;
// 	}
// 	user.Update(m_setpass1,nametemp);
// 	CoUninitialize();
	AfxMessageBox("�ɹ��޸�����!");
	CDialog::OnOK();

}

void CSetpass::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
