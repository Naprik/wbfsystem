// DlgLinkroll.cpp : implementation file
//

#include "stdafx.h"
#include "DlgOnlineBeginExam.h"
#include "../YdCom\YDStuAppCom.h"
#include "../Base\AutoClean.h"
#include "../YDExamObjRef\YDEAddrUnit.h"
#include "../Base\DataHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgOnlineBeginExam dialog


CDlgOnlineBeginExam::CDlgOnlineBeginExam(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOnlineBeginExam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOnlineBeginExam)
	//}}AFX_DATA_INIT
}

CDlgOnlineBeginExam::~CDlgOnlineBeginExam()
{
	CListAutoClean<CYDStuAppCom> clr(m_lstStuAppCom);
}


void CDlgOnlineBeginExam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOnlineBeginExam)
	DDX_Control(pDX, IDC_EXAMLIST, m_examlist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOnlineBeginExam, CDialog)
	//{{AFX_MSG_MAP(CDlgOnlineBeginExam)
	ON_BN_CLICKED(IDC_BUTTON_ONLINE_BEGINEXAM, OnOnLineBeginExam)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOnlineBeginExam message handlers

BOOL CDlgOnlineBeginExam::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwStyle=GetWindowLong(m_examlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_examlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_examlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_examlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_examlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_examlist.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	
	m_examlist.ClearHeadings();
	m_examlist.SetHeadings(_T("学号,150;姓名,100;班级,100;准考证号,100;当前状态,150;开考时间,150;交卷时间,150;使用端口,100"));

	GetDlgItem(IDC_BUTTON_ONLINE_BEGINEXAM)->EnableWindow(FALSE);
	return true;
}

void CDlgOnlineBeginExam::OnOnLineBeginExam() 
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
	for(int i = 0; i < m_examlist.GetItemCount();i++)
	{
		if(!m_examlist.GetCheck(i))
		{
			continue;
		}
		bCheck = TRUE;
		CYDStuAppCom* pStuAppCom = (CYDStuAppCom*)m_examlist.GetItemData(i);
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
 		hr = pStuAppCom->Sendexaminfo();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CString strStart = CDataHandler::DateTimeToStr(pGExamStruct->m_pExamDetail->m_timeStart,2);
		m_examlist.SetItemText(i,5,strStart);
		m_examlist.SetItemText(i,4,_T("开考成功(联机)"));
	}
	if(!bCheck)
	{
		AfxMessageBox(_T("没有选择学生机！"));
	}
	else
	{
		AfxMessageBox(_T("开考成功"));
	}
	
}


void CDlgOnlineBeginExam::OnSearch() 
{
	{
		CListAutoClean<CYDStuAppCom> clr(m_lstStuAppCom);
	}
	
	m_examlist.DeleteAllItems();
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
				int index = m_examlist.AddItem(0,
												strStuID,
												strStuName,
												strClassName,
												strExamID,
												_T("联机成功"),
												_T(""),
												_T(""),
												(*itr)->m_strComPort);
				m_examlist.SetCheck(index);
				m_examlist.SetItemData(index,DWORD_PTR(*itr));
				break;
			}
		}
	}
	GetDlgItem(IDC_BUTTON_ONLINE_BEGINEXAM)->EnableWindow(TRUE);
}

void CDlgOnlineBeginExam::OnOK() 
{
	sStuid = m_examlist.GetItemText(0,0);
	sStartTime = m_examlist.GetItemText(0,5);
	sStat = m_examlist.GetItemText(0,4);
	
	CDialog::OnOK();
}
