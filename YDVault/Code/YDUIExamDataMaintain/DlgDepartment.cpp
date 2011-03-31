// DlgDpartment.cpp : implementation file
//

#include "stdafx.h"
#include "DlgDepartment.h"
#include "../DBBase\DatabaseEx.h"
#include "../ObjRef\YDObjectRef.h"
#include "../Base\AutoClean.h"
#include "../ObjHelper\StaticObjHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDepartment dialog


CDlgDepartment::CDlgDepartment(OPERATION op, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDepartment::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDepartment)
	m_head = _T("");
	m_tel = _T("");
	m_depart = _T("");
	m_departid = _T("");
	m_opMode = op;
	m_pDepart = NULL;
	//}}AFX_DATA_INIT
}


void CDlgDepartment::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDepartment)
	DDX_Text(pDX, IDC_HEAD, m_head);
	DDX_Text(pDX, IDC_TEL, m_tel);
	DDX_Text(pDX, IDC_DEPART, m_depart);
	DDX_Text(pDX, IDC_DEPARTID, m_departid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDepartment, CDialog)
	//{{AFX_MSG_MAP(CDlgDepartment)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CDlgDepartment::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgDepartment::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDepartment message handlers


BOOL CDlgDepartment::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (m_opMode == OP_VIEW)
	{
		GetDlgItem(IDC_DEPARTID)->EnableWindow(FALSE);
		GetDlgItem(IDC_DEPART)->EnableWindow(FALSE);
		GetDlgItem(IDC_HEAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_TEL)->EnableWindow(FALSE);
	}

	return TRUE;
}

void CDlgDepartment::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CDlgDepartment::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_departid.Trim();
	m_depart.Trim();
	m_head.Trim();
	m_tel.Trim();
	if(m_depart.IsEmpty() && m_departid.IsEmpty())
	{
		AfxMessageBox(_T("院系名称或院系编号不能为空！"));	
		return;
	}
	HRESULT hr = E_FAIL;
	if(m_opMode == OP_NEW || m_opMode == OP_EDIT)
	{
		CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
		ASSERT(pDB);
		//要验证院系编号不能重复
		std::list<CYDObjectRef*> lstDepart;
		std::list<CPropQueryContidition*> lstQueryCon;
		CListAutoClean<CYDObjectRef> clr1(lstDepart);
		CListAutoClean<CPropQueryContidition> clr2(lstQueryCon);
		CPropQueryContidition* pQueryConditon = new CPropQueryContidition();
		lstQueryCon.push_back(pQueryConditon);
		pQueryConditon->m_uOpType = Q_EQUAL;
		pQueryConditon->m_strFiledName = FIELD_YDDEPARTMENT_ID;
		pQueryConditon->m_strConVal = m_departid;
		pQueryConditon->m_uFieldType = VT_BSTR;
		hr = CStaticObjHelper::GetObjRef(DB_YDDEPARTMENT,pDB,&lstDepart,&lstQueryCon);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		if(m_opMode == OP_NEW)
		{
			if(lstDepart.size() > 0)
			{
				AfxMessageBox(_T("院系编号不能重复！"));
				return;
			}
		}
		else if(m_opMode == OP_EDIT)
		{
			ASSERT(m_pDepart);
			OBJID IDDepart = 0;
			hr = m_pDepart->GetID(&IDDepart);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return;
			}
			BOOL bFind = FALSE;
			for(std::list<CYDObjectRef*>::const_iterator itr = lstDepart.begin();
				itr != lstDepart.end();++itr)
			{
				OBJID itrDepartID = ID_EMPTY;
				hr = (*itr)->GetID(&itrDepartID);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return;
				}
				if(IDDepart != itrDepartID)
				{
					bFind = TRUE;
					break;
				}
			}
			if(bFind)
			{
				AfxMessageBox(_T("院系编号不能重复！"));
				return;
			}
		}
	}
	
	if(m_head.IsEmpty() || m_tel.IsEmpty())
	{
		CString msg;
		msg.Format(_T("基本信息未填写完整是否继续？"));
		if ( MessageBox(msg,_T("确认"),MB_OKCANCEL) != IDOK)
		{
			return;
		}
	}
	UpdateData(FALSE);
	OnOK();
	
}

INT_PTR CDlgDepartment::DoModal()
{
	CExtDllState extdllstate;
	return CDialog::DoModal();
}
