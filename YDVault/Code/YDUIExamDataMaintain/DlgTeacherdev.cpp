// DlgTeacherdev.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTeacherdev.h"

#include "../DBBase/DatabaseEx.h"
#include "../YDExamObjRef/YDTeacherDevRef.h"
#include "../Base\DataHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTeacherdev dialog


CDlgTeacherdev::CDlgTeacherdev(OPERATION op, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTeacherdev::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTeacherdev)
	m_devmac = _T("");
	m_devid = _T("");
	m_teachername = _T("");
	m_opMode = op;
	//}}AFX_DATA_INIT
}


void CDlgTeacherdev::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTeacherdev)
	DDX_Text(pDX, IDC_TEACHERDEV, m_devmac);
	DDX_Text(pDX, IDC_TEACHERID, m_devid);
	DDX_Text(pDX, IDC_TEACHERNAME, m_teachername);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTeacherdev, CDialog)
	//{{AFX_MSG_MAP(CDlgTeacherdev)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTeacherdev message handlers

void CDlgTeacherdev::OnOK() 
{
	// TODO: Add extra validation here
	CString strTeacherID = m_devid;
	CString strDeviceID = m_devmac;
	strDeviceID.Trim();
	UpdateData(TRUE);
	m_devid.Trim();
	m_teachername.Trim();
	m_devmac.Trim();
	if(m_devid.IsEmpty())
	{
		m_devmac = strDeviceID;
		UpdateData(FALSE);
		AfxMessageBox(_T("教师编号不能为空！"));
		return;
	}
	if(m_teachername.IsEmpty())
	{
		m_devmac = strDeviceID;
		UpdateData(FALSE);
		AfxMessageBox(_T("教师姓名不能为空！"));
		return;
	}
	if(m_devmac.IsEmpty())
	{
		m_devmac = strDeviceID;
		UpdateData(FALSE);
		AfxMessageBox(_T("教师机地址不能为空"));
		return;
	}
	if(!ValiateMac())
	{
		m_devmac = strDeviceID;
		UpdateData(FALSE);
		AfxMessageBox(_T("教师机地址格式不正确！"));
		return;
	}
	
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CYDTeacherDevRef teacherDev(pDB);
	BOOL bExist;
	HRESULT hr = E_FAIL;
	if (m_opMode == OP_NEW ||
		strTeacherID != m_devid)
	{
		hr = teacherDev.IsFieldExist(FIELD_YDTEACHERDEV_TEACHID, m_devid, &bExist);
		if (FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return;
		}
		if (bExist)
		{
			AfxMessageBox(L"教师编号已存在，请重新输入！");
			m_devid = strTeacherID;
			UpdateData(FALSE);
			GetDlgItem(IDC_TEACHERID)->SetFocus();
			return;
		}
	}
	if (m_opMode == OP_NEW ||
		strDeviceID != m_devmac)
	{
		hr = teacherDev.IsFieldExist(FIELD_YDTEACHERDEV_DEVID, m_devmac, &bExist);
		if (FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_ICONINFORMATION|MB_OK);
			return;
		}
		if (bExist)
		{
			AfxMessageBox(L"设备编号已存在，请重新输入！");
			m_devmac = strDeviceID;
			UpdateData(FALSE);
			GetDlgItem(IDC_TEACHERDEV)->SetFocus();
			return;
		}
	}
	
	CDialog::OnOK();
}


BOOL CDlgTeacherdev::ValiateMac()
{
	//校验教师机格式
	//** ** **
	//教师机地址分三段，中间用空格隔开，并且并且都要是十六进制,每段两位
	CStringArray arrMac;
	CDataHandler::SplitString(m_devmac,_T(' '),arrMac);
	if(arrMac.GetCount() != 3)
	{
		return FALSE;
	}
	for(int i = 0; i < arrMac.GetCount();i++)
	{
		CString strMac = arrMac.GetAt(i);
		if(strMac.GetLength() != 2)
		{
			return FALSE;
		}
		strMac.MakeUpper();
		for(int j = 0; j < strMac.GetLength();j++)
		{
			TCHAR ch = strMac.GetAt(j);
			if((ch <= _T('9') && ch >= _T('0')) ||
				(ch <= _T('F') && ch >= _T('A'))
			  )
			{

			}
			else
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}
BOOL CDlgTeacherdev::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (m_opMode == OP_VIEW)
	{
		GetDlgItem(IDC_TEACHERDEV)->EnableWindow(FALSE);
		GetDlgItem(IDC_TEACHERID)->EnableWindow(FALSE);
		GetDlgItem(IDC_TEACHERNAME)->EnableWindow(FALSE);
	}

	return TRUE;
}
INT_PTR CDlgTeacherdev::DoModal()
{
	CExtDllState extdllstate;

	return CDialog::DoModal();
}
