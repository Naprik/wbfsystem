// DlgLinkrolling.cpp : implementation file
//

#include "stdafx.h"
#include "DlgOnlineRolling.h"
#include "../\Base\AutoClean.h"
#include "../YdCom\YDStuAppCom.h"
#include "../YDExamObjRef\YDEAddrUnit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgOnlineRolling dialog


CDlgOnlineRolling::CDlgOnlineRolling(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOnlineRolling::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOnlineRolling)
	//}}AFX_DATA_INIT
}

CDlgOnlineRolling::~CDlgOnlineRolling()
{
	CListAutoClean<CYDStuAppCom> clr(m_lstStuAppCom);
}


void CDlgOnlineRolling::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOnlineRolling)
	DDX_Control(pDX, IDC_LINKLIST, m_linklist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOnlineRolling, CDialog)
	//{{AFX_MSG_MAP(CDlgOnlineRolling)
	ON_BN_CLICKED(IDC_LINKROLLING, OnLinkrolling)
	ON_BN_CLICKED(IDC_SEACH, OnSeach)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOnlineRolling message handlers
BOOL CDlgOnlineRolling::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwStyle;

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	

	dwStyle=GetWindowLong(m_linklist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_linklist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_linklist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_linklist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_linklist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_linklist.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	
	m_linklist.ClearHeadings();
	m_linklist.SetHeadings(_T("学号,150;姓名,100;班级,100;准考证号,100;当前状态,150;开考时间,150;交卷时间,150;使用端口,100;考试成绩,100"));

	GetDlgItem(IDC_LINKROLLING)->EnableWindow(FALSE);
	return TRUE;
	
}

void CDlgOnlineRolling::OnLinkrolling() 
{
	 HRESULT hr = E_FAIL;
	 CGExamStruct* pGExamStruct = NULL;
	 hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	 if(FAILED(hr))
	 {
		 DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		 return ;
	 }
	 ASSERT(pGExamStruct);
	 BOOL bCheck = FALSE;
	 for(int i = 0; i < m_linklist.GetItemCount();i++)
	 {
		 if(!m_linklist.GetCheck(i))
		 {
			 continue;
		 }
		 bCheck = TRUE;
		 CYDStuAppCom* pStuAppCom = (CYDStuAppCom*)m_linklist.GetItemData(i);
		 ASSERT(pStuAppCom);
		 BOOL bOpen = FALSE;
		 hr = pStuAppCom->GetState(bOpen);
		 if(FAILED(hr))
		 {
			 DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			 return;
		 }
		 if(!bOpen)
		 {
			 hr = pStuAppCom->OpenCom();
			 if(FAILED(hr))
			 {
				 DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				 return;
			 }
		 }
		 CString strAnswer;
		 hr = pStuAppCom->LinkRolling(strAnswer);
		 if(FAILED(hr))
		 {
			 DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			 return;
		 }
		 hr = pStuAppCom->Linksuccess();
		 if(FAILED(hr))
		 {
			 DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			 return;
		 }
		 COleDateTime current;
		 current = COleDateTime::GetCurrentTime();
		 m_linklist.SetItemText(i,6,current.Format());
		 m_linklist.SetItemText(i,4,_T("收卷成功(联机)"));
		 m_linklist.SetItemText(i,8,strAnswer);
	 }
	 if(!bCheck)
	 {
		 AfxMessageBox(_T("没有选择学生机！"));
	 }
	 else
	 {
		 AfxMessageBox(_T("收卷成功(联机)"));
	 }
}

void CDlgOnlineRolling::OnSeach() 
{
	m_linklist.DeleteAllItems();	
	{
		CListAutoClean<CYDStuAppCom> clr(m_lstStuAppCom);
	}
	HRESULT hr = E_FAIL;
	hr = CYDStuAppCom::FindStucom(m_lstStuAppCom);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	if(m_lstStuAppCom.size() <= 0)
	{
		AfxMessageBox(_T("没有搜索到学生机！"));
		return;
	}
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	for(std::list<CYDStuAppCom*>::const_iterator itr = m_lstStuAppCom.begin();
		itr != m_lstStuAppCom.end();++itr)
	{
		BOOL bOpen = FALSE;
		hr = (*itr)->GetState(bOpen);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		if(!bOpen)
		{
			hr = (*itr)->OpenCom();
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return ;
			}
		}
		CString strItrMac;
		int iType = -1;
		hr = (*itr)->GetDevMac(strItrMac,iType);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		ASSERT(iType == 1);
		for(std::list<CYDObjectRef*>::const_iterator itrUnit = pGExamStruct->m_pExamDetail->m_lstUnit.begin();
			itrUnit != pGExamStruct->m_pExamDetail->m_lstUnit.end();++itrUnit)
		{
			CYDEAddrUnit* pUnit = (CYDEAddrUnit*)(*itrUnit);
			ASSERT(pUnit);
			CString strUnitMac;
			hr = pUnit->GetDeviceMac(strUnitMac);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return ;
			}
			if(strItrMac.CompareNoCase(strUnitMac) == 0)
			{
				CYdStudentRef* pStuRef = NULL;
				hr = pUnit->GetStudent(pStuRef);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				CPtrAutoClean<CYdStudentRef> clr(pStuRef);
				ASSERT(pStuRef);
				CString strStuID;//学号
				hr = pStuRef->GetPropVal(FIELD_YDSTU_ID,strStuID);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				CString strStuName;//姓名
				hr = pStuRef->GetPropVal(FIELD_YDSTU_NAME,strStuName);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				std::list<CYDObjectRef*> lstClass;
				CListAutoClean<CYDObjectRef> clr1(lstClass);
				hr = pStuRef->GetClass(&lstClass);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				CString strClassName;//所属班级
				for(std::list<CYDObjectRef*>::const_iterator itrClass = lstClass.begin();
					itrClass != lstClass.end();++itrClass)
				{
					hr = (*itrClass)->GetPropVal(FIELD_YDCLASS_NAME,strClassName);
					if(FAILED(hr))
					{
						DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
						return ;
					}
				}

				//准考证号
				CString strExamID;
				hr = pUnit->GetPropVal(FIELD_YDEXAMADDRUNIT_EXAMID,strExamID);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				int index = m_linklist.AddItem(0,
					strStuID,
					strStuName,
					strClassName,
					strExamID,
					_T("联机成功"),
					_T(""),
					_T(""),
					(*itr)->m_strComPort,
					_T(""));
				m_linklist.SetCheck(index);
				m_linklist.SetItemData(index,DWORD_PTR(*itr));
				break;
			}
		}
	}
	GetDlgItem(IDC_LINKROLLING)->EnableWindow(TRUE);

}

void CDlgOnlineRolling::OnOK() 
{
	sStuid = m_linklist.GetItemText(0,0);
	sEndTime = m_linklist.GetItemText(0,6);
	sStat = m_linklist.GetItemText(0,4);	
	CDialog::OnOK();
}
