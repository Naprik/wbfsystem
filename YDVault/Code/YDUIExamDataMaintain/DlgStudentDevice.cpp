// DlgSubject.cpp : implementation file
//

#include "stdafx.h"
#include "DlgStudentDevice.h"
#include "../DBBase/DatabaseEx.h"
#include "../YDExamObjRef/YDDevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubject dialog


CDlgStudentDevice::CDlgStudentDevice(OPERATION op, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStudentDevice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSubject)
	m_strCode = _T("");
	m_strMac = _T("");
	m_op = op;
	//}}AFX_DATA_INIT
}


void CDlgStudentDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubject)
	DDX_Text(pDX, IDC_DM_DEVICE_CODE, m_strCode);
	DDX_Text(pDX, IDC_DM_DEVICE_MAC, m_strMac);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStudentDevice, CDialog)
	//{{AFX_MSG_MAP(CDlgSubject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubject message handlers
BOOL CDlgStudentDevice::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (m_op == OP_VIEW)
	{
		GetDlgItem(IDC_DM_DEVICE_CODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_DM_DEVICE_MAC)->EnableWindow(FALSE);
	}

	return TRUE;
}

void CDlgStudentDevice::OnOK() 
{
	// TODO: Add extra validation here
	CString strTmpCode = m_strCode;
	CString strTmpAddr = m_strMac;
	UpdateData(TRUE);

	m_strCode.Trim();
	if(m_strCode.GetLength() == 0)
	{
		AfxMessageBox(L"设备编号不能为空！");
		GetDlgItem(IDC_DM_DEVICE_CODE)->SetFocus();
		return;
	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	BOOL bExist;
	CYDDevice deviceRef(pDB);
	if (strTmpCode != m_strCode || m_op == OP_NEW)
	{
		deviceRef.IsFieldExist(FIELD_YDDEVICE_ID, m_strCode, &bExist);
		if (bExist)
		{
			AfxMessageBox(L"设备编号已存在，请重新输入！");
			m_strCode = m_strCode;
			UpdateData(FALSE);
			GetDlgItem(IDC_DM_DEVICE_CODE)->SetFocus();
			return;
		}
	}
		
	m_strMac.Trim();
	if(m_strMac.GetLength() == 0)
	{
		AfxMessageBox(L"蓝牙地址不能为空！");
		GetDlgItem(IDC_DM_DEVICE_MAC)->SetFocus();
		return;
	}
	if (m_strMac != strTmpAddr || m_op == OP_NEW)
	{
		deviceRef.IsFieldExist(FIELD_YDDEVICE_MAC, m_strMac, &bExist);
		if (bExist)
		{
			AfxMessageBox(L"蓝牙地址已存在，请重新输入！");
			m_strMac = strTmpAddr;
			UpdateData(FALSE);
			GetDlgItem(IDC_DM_DEVICE_MAC)->SetFocus();
			return;
		}
	}

	CDialog::OnOK();
}

INT_PTR CDlgStudentDevice::DoModal()
{
	CExtDllState extdllstate;

	return CDialog::DoModal();
}
