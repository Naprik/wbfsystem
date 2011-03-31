// FormExamBlue.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "FormExamBlue.h"
#include "InitInfoXml.h"
#include "../Base\FilePathHelper.h"
#include "InvigilateInfo.h"
#include "../ObjRef\YDObjectRef.h"
#include "../\ObjHelper\StaticObjHelper.h"
#include "../Base\AutoClean.h"
#include "../YDExamObjRef\YDSubjectRef.h"
#include "../Base\DataHandler.h"
#include "DlgExamBlueDetail.h"
#include "../YdCom\ExamQuestionSegment.h"
#include "../UIBase\ListBoxEx.h"
#include "../YDExamObjRef\YdStudentRef.h"
#include "../YDExamObjRef\YDEAddrUnit.h"
#include "../\UIBase\ControlWnd.h"
#include "YDProgressControl.h"
#include "TeacherAppToMacList.h"
#include "DlgResDetail.h"
#include "DlgOnlineBeginExam.h"
#include "DlgOnlineRolling.h"
#include "../YDUIExam\DlgStunfo.h"
#include "../Include\YDVaultError.h"
#include "../YdCom\YDStuAppCom.h"
#include "YDThreadFun.h"
#include "../Base\CriticalSectionControl.h"
#include "ThreadTeacherComJump.h"
#include "DlgPaper.h"
#include "../YDExamObjRef\YDExamAddress.h"

// CFormExamBlue

const int COL_LIST_TEACHER_ID		= 0;
const int COL_LIST_TEACHER_NAME		= 1;
const int COL_LIST_TEACHER_PORT		= 2;
const int COL_LIST_TEACHER_STATE	= 3;

const int COL_LIST_STUDENT_ID		= 0;//学号
const int COL_LIST_STUDENT_NAME		= 1;//姓名
const int COL_LIST_STUDENT_CLASS	= 2;//班级
const int COL_LIST_STUDENT_EXAM_ID	= 3;//准考证号
const int COL_LIST_STUDENT_STATE	= 4;//当前状态
const int COL_LIST_STUDENT_START_TIME = 5;//开考时间
const int COL_LIST_STUDENT_END_TIME = 6;//交卷时间
const int COL_LIST_STUDENT_MAC      =7;//蓝牙地址

IMPLEMENT_DYNCREATE(CFormExamBlue, CYdFormView)

CFormExamBlue::CFormExamBlue()
	: CYdFormView(CFormExamBlue::IDD)
	, m_timeStart(COleDateTime::GetCurrentTime())
	, m_timeEnd(COleDateTime::GetCurrentTime())
	, m_strProgress(_T(""))
{
	m_bCancelThread = FALSE;
	m_bCanClose = TRUE;
	m_bTimeProgressStatus = FALSE;
	m_pJumbThread = NULL;
}

CFormExamBlue::~CFormExamBlue()
{
	CListAutoClean<CYDObjectRef> clr1(m_lstClear);
	ClearStu();
	CListAutoClean<CTeacherAppToMacList> clr2(m_lstAppToMacList);
}

void CFormExamBlue::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_EXAM_KIND, m_cmbExamKind);
	DDX_Control(pDX, IDC_COMBO_EXAM_ROOM, m_cmbExamRoom);
	DDX_Control(pDX, IDC_COMBO_EXAM_SUBJECT, m_cmbExamSubject);
	DDX_Control(pDX, IDC_COMBO_EXAM_PAPER, m_cmbExamPaper);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_START, m_timeStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END, m_timeEnd);
	DDX_Control(pDX, IDC_TEACHER, m_lstCtrlTeacher);
	DDX_Control(pDX, IDC_LIST_STUDENT2, m_lstCtrlUnit);
	DDX_Control(pDX, IDC_PROGRESS_STATUS, m_ctrlProgressStaus);
	DDX_Control(pDX, IDC_STATIC_WAIT, m_WaitTxt);
	DDX_Text(pDX, IDC_EDIT_PROGRESS, m_strProgress);
}

BEGIN_MESSAGE_MAP(CFormExamBlue, CYdFormView)
	ON_CBN_SELCHANGE(IDC_COMBO_EXAM_SUBJECT, &CFormExamBlue::OnCbnSelchangeComboExamSubject)
	ON_BN_CLICKED(IDC_BUTTON_DETAIL, &CFormExamBlue::OnBnClickedButtonDetail)
	ON_CBN_SELCHANGE(IDC_COMBO_EXAM_PAPER, &CFormExamBlue::OnCbnSelchangeComboExamPaper)
	ON_CBN_SELCHANGE(IDC_COMBO_EXAM_ROOM, &CFormExamBlue::OnCbnSelchangeComboExamRoom)
	ON_BN_CLICKED(IDC_CHECK_SEL_ALL, &CFormExamBlue::OnBnClickedCheckSelAll)
	ON_BN_CLICKED(IDC_CHECK_SEL_INVERSE, &CFormExamBlue::OnBnClickedCheckSelInverse)
	ON_BN_CLICKED(IDC_TEACHER_TEST, &CFormExamBlue::OnBnClickedTeacherTest)
	ON_BN_CLICKED(IDC_TEACHER_CHOOSE, &CFormExamBlue::OnBnClickedTeacherChoose)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CFormExamBlue::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_BEGINEXAM, &CFormExamBlue::OnBnClickedButtonBeginexam)
	ON_BN_CLICKED(IDC_BUTTON_ROLLING, &CFormExamBlue::OnBnClickedButtonRolling)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL_PROGRESS, &CFormExamBlue::OnBnClickedButtonCancelProgress)
	ON_BN_CLICKED(IDC_BUTTON_QUERYMARK, &CFormExamBlue::OnBnClickedButtonQuerymark)
	ON_BN_CLICKED(IDC_BUTTON_PAPER_ANSYS, &CFormExamBlue::OnBnClickedButtonPaperAnsys)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_STUDENT2, &CFormExamBlue::OnNMDblclkListStudent2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_STUDENT2, &CFormExamBlue::OnNMRClickListStudent2)
	ON_MESSAGE(WM_PROGRESSCTRL_SHOW,&CFormExamBlue::OnProgressShow)
	ON_MESSAGE(WM_PROGRESSCTRL_RANGE,&CFormExamBlue::OnProgressRange)
	ON_MESSAGE(WM_PROGRESSCTRL_SETIP,&CFormExamBlue::OnProgressStepIt)
	ON_MESSAGE(WM_PROGRESSCTRL_SHOWCANCEL,&CFormExamBlue::OnProgressShowCancel)
	ON_COMMAND(ID_MI_USB_BEGIN_EXAM, &CFormExamBlue::OnMiUSBBeginExam)
	ON_COMMAND(ID_MI_USB_ROLLING, &CFormExamBlue::OnMiUSBRolling)
	ON_COMMAND(ID_MI_STU_DETAILINFO, &CFormExamBlue::OnMiStudentDetailInfo)
	ON_COMMAND(ID_MI_CONTINUE_BEGIN_EXAM, &CFormExamBlue::OnMiContinueBeginExam)
	ON_BN_CLICKED(IDC_BUTTON_NEW_PAPER, &CFormExamBlue::OnBnClickedButtonNewPaper)
END_MESSAGE_MAP()


// CFormExamBlue diagnostics

#ifdef _DEBUG
void CFormExamBlue::AssertValid() const
{
	CYdFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormExamBlue::Dump(CDumpContext& dc) const
{
	CYdFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormExamBlue message handlers

void CFormExamBlue::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	HRESULT hr = E_FAIL;
	hr = InitExamClassRoom();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	hr = InitExamSubject();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	hr = InitTeacherList();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	hr = InitStudentList();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CInitInfoXml InitInfoXml;
	CString strInitXml;
	hr = InitInfoXml.LoadInitXmlInfo(strInitXml);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	hr = ShowXmlInfo(strInitXml);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	hr = StartThreadTeacherComJump();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	hr = ShowRollInfoButton();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}

	GetDlgItem(IDC_BUTTON_ROLLING)->EnableWindow(FALSE);
	SetTimer(1,1000,NULL);
}

HRESULT CFormExamBlue::InitTeacherList()
{
	HRESULT hr = E_FAIL;
	DWORD dwStyle=GetWindowLong(m_lstCtrlTeacher.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_lstCtrlTeacher.GetSafeHwnd(),GWL_STYLE,dwStyle);  

	m_lstCtrlTeacher.SetExtendedStyle(LVS_EX_GRIDLINES);
	::SendMessage(m_lstCtrlTeacher.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_lstCtrlTeacher.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
	m_lstCtrlTeacher.SetExtendedStyle(dwStyle);

	m_lstCtrlTeacher.InsertColumn(COL_LIST_TEACHER_ID,_T("教师机编号"),LVCFMT_LEFT,80);
	m_lstCtrlTeacher.InsertColumn(COL_LIST_TEACHER_NAME,_T("使用者"),LVCFMT_LEFT,150);
	m_lstCtrlTeacher.InsertColumn(COL_LIST_TEACHER_PORT,_T("使用端口"),LVCFMT_LEFT,80);
	m_lstCtrlTeacher.InsertColumn(COL_LIST_TEACHER_STATE,_T("机状态"),LVCFMT_LEFT,150);
	return S_OK;
}
HRESULT CFormExamBlue::InitStudentList()
{
	HRESULT hr = E_FAIL;
	m_lstCtrlUnit.SetExtendedStyle(LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES|
		LVS_EX_CHECKBOXES);
	m_lstCtrlUnit.InsertColumn(COL_LIST_STUDENT_ID,_T("学号"),LVCFMT_LEFT,80);
	m_lstCtrlUnit.InsertColumn(COL_LIST_STUDENT_NAME,_T("姓名"),LVCFMT_LEFT,80);
	m_lstCtrlUnit.InsertColumn(COL_LIST_STUDENT_CLASS,_T("班级"),LVCFMT_LEFT,80);
	m_lstCtrlUnit.InsertColumn(COL_LIST_STUDENT_EXAM_ID,_T("准考证号"),LVCFMT_LEFT,80);
	m_lstCtrlUnit.InsertColumn(COL_LIST_STUDENT_STATE,_T("当前状态"),LVCFMT_LEFT,80);
	m_lstCtrlUnit.InsertColumn(COL_LIST_STUDENT_START_TIME,_T("开考时间"),LVCFMT_LEFT,80);
	m_lstCtrlUnit.InsertColumn(COL_LIST_STUDENT_END_TIME,_T("交卷时间"),LVCFMT_LEFT,80);
	m_lstCtrlUnit.InsertColumn(COL_LIST_STUDENT_MAC,_T("学生蓝牙地址"),LVCFMT_LEFT,80);
	return S_OK;
}

HRESULT CFormExamBlue::ShowXmlInfo(CString _strXmlFile)
{
	HRESULT hr = E_FAIL;
	if(!CFilePathHelper::FilePathExists(_strXmlFile))
	{
		return S_FALSE;
	}
	CInvigilateInfo InvigilateInfo;
	hr = InvigilateInfo.Load(CComBSTR(_strXmlFile));
	if(FAILED(hr))
	{
		return hr;
	}
	UpdateData();
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	hr = UpdateRollCallUI(FALSE);
	if(FAILED(hr))
	{
		return hr;
	}
	pGExamStruct->m_pExamDetail->m_strXmlFile =_strXmlFile; 
	UpdateData(FALSE);
	return hr;
}

HRESULT CFormExamBlue::UpdateRollCallUI(BOOL _bUpdate)
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	if(_bUpdate)
	{
		{
			CPtrAutoClean<CYDObjectRef> clr1(pGExamStruct->m_pExamDetail->m_pObjAddress);
			CPtrAutoClean<CYDObjectRef> clr2(pGExamStruct->m_pExamDetail->m_pSubject);
			CPtrAutoClean<CYDObjectRef> clr3(pGExamStruct->m_pExamDetail->m_pPaper);
		}
		//界面---》pGExamStruct
		{
			//点名，收卷模式和考试性质
			pGExamStruct->m_pExamDetail->m_uRollCallType = ROLLCALL_TYPE_BLUE;
			pGExamStruct->m_pExamDetail->m_uRollingType = ROLLING_TYPE_BLUE;
			pGExamStruct->m_pExamDetail->m_uExamKind = (YDEXAM_KIND)m_cmbExamKind.GetCurSel();
		}
		CYDObjectRef* pNewAddr = NULL;
		hr = CStaticObjHelper::CopyObj((CYDObjectRef*)m_cmbExamRoom.GetItemData(m_cmbExamRoom.GetCurSel()),
										pDB,
										pNewAddr);
		pGExamStruct->m_pExamDetail->m_pObjAddress = pNewAddr;
		if(FAILED(hr))
		{
			return hr;
		}
		CYDObjectRef* pNewSubject = NULL;
		hr = CStaticObjHelper::CopyObj((CYDObjectRef*)m_cmbExamSubject.GetItemData(m_cmbExamSubject.GetCurSel()),
			pDB,
			pNewSubject);
		pGExamStruct->m_pExamDetail->m_pSubject = pNewSubject;
		if(FAILED(hr))
		{
			return hr;
		}
		CYDObjectRef* pNewPaper = NULL;
		hr = CStaticObjHelper::CopyObj((CYDObjectRef*)m_cmbExamPaper.GetItemData(m_cmbExamPaper.GetCurSel()),
			pDB,
			pNewPaper);
		pGExamStruct->m_pExamDetail->m_pPaper = pNewPaper;
		if(FAILED(hr))
		{
			return hr;
		}
		pGExamStruct->m_pExamDetail->m_timeStart = m_timeStart;
		pGExamStruct->m_pExamDetail->m_timeEnd = m_timeEnd;
	}
	else
	{
		//pGExamStruct-->界面
		{
			//点名，收卷模式和考试性质
			m_cmbExamKind.SetCurSel(pGExamStruct->m_pExamDetail->m_uExamKind);
		}


		{
			//考场
			CYDObjectRef* pObjExamRoom = pGExamStruct->m_pExamDetail->m_pObjAddress;
			ASSERT(pObjExamRoom);
			OBJID idObjExamRoom = ID_EMPTY;
			hr = pObjExamRoom->GetID(&idObjExamRoom);
			if(FAILED(hr))
			{
				return hr;
			}
			BOOL bFind = FALSE;
			for(int i = 0 ;i < m_cmbExamRoom.GetCount();i++)
			{
				CYDObjectRef* pObj = (CYDObjectRef*)m_cmbExamRoom.GetItemData(i);
				ASSERT(pObj);
				OBJID idObj = ID_EMPTY;
				hr = pObj->GetID(&idObj);
				if(FAILED(hr))
				{
					return hr;
				}
				if(idObj == idObjExamRoom)
				{
					m_cmbExamRoom.SetCurSel(i);
					bFind = TRUE;
					break;
				}
			}
			if(!bFind)
			{
				CYDObjectRef* pObjNewAddress = NULL;
				hr = CStaticObjHelper::CopyObj(pObjExamRoom,pDB,pObjNewAddress);
				if(FAILED(hr))
				{
					return hr;
				}
				m_lstClear.push_back(pObjNewAddress);
				CString strAddress;
				hr = pObjNewAddress->GetPropVal(FIELD_YDEXAMADDR_ADDR,strAddress);
				if(FAILED(hr))
				{
					return hr;
				}
				int index = m_cmbExamRoom.AddString(strAddress);
				m_cmbExamRoom.SetItemData(index,(DWORD_PTR)(pObjNewAddress));
				m_cmbExamRoom.SetCurSel(index);
			}
			OnCbnSelchangeComboExamRoom();
		}

		{
			//考试科目
			CYDObjectRef* pSubjectRef = pGExamStruct->m_pExamDetail->m_pSubject;
			ASSERT(pSubjectRef);
			OBJID id = ID_EMPTY;
			hr = pSubjectRef->GetID(&id);
			if(FAILED(hr))
			{
				return hr;
			}
			BOOL bFind = FALSE;
			for(int i = 0 ; i < m_cmbExamSubject.GetCount();i++)
			{
				CYDObjectRef* pObj = (CYDObjectRef*)m_cmbExamSubject.GetItemData(i);
				ASSERT(pObj);
				OBJID idObj = ID_EMPTY;
				hr = pObj->GetID(&idObj);
				if(FAILED(hr))
				{
					return hr;
				}
				if(idObj == id)
				{
					m_cmbExamSubject.SetCurSel(i);
					bFind = TRUE;
					break;
				}
			}
			if(!bFind)
			{
				CYDObjectRef* pNewSubjectRef = NULL;
				hr = CStaticObjHelper::CopyObj(pSubjectRef,pDB,pNewSubjectRef);
				if(FAILED(hr))
				{
					return hr;
				}
				ASSERT(pNewSubjectRef);
				m_lstClear.push_back(pNewSubjectRef);
				CString strSubJectName;
				hr = pNewSubjectRef->GetPropVal(FIELD_YDSUBJECT_NAME,strSubJectName);
				if(FAILED(hr))
				{
					return hr;
				}
				int index = m_cmbExamSubject.AddString(strSubJectName);
				m_cmbExamSubject.SetItemData(index,(DWORD_PTR)(pNewSubjectRef));
				m_cmbExamSubject.SetCurSel(index);
			}
			OnCbnSelchangeComboExamSubject();

		}

		{
			//试卷名称
			CYDObjectRef* pObjPaper = pGExamStruct->m_pExamDetail->m_pPaper;
			OBJID id = ID_EMPTY;
			hr = pObjPaper->GetID(&id);
			if(FAILED(hr))
			{
				return hr;
			}
			BOOL bFind = FALSE;
			for(int i =0 ; i < m_cmbExamPaper.GetCount();i++)
			{
				CYDObjectRef* pObjRef = (CYDObjectRef*)m_cmbExamPaper.GetItemData(i);
				ASSERT(pObjRef);
				OBJID idObj = ID_EMPTY;
				hr = pObjRef->GetID(&idObj);
				if(FAILED(hr))
				{
					return hr;
				}
				if(id == idObj)
				{
					m_cmbExamPaper.SetCurSel(i);
					bFind = TRUE;
					break;
				}
			}
			if(!bFind)
			{
				CYDObjectRef* pNewObjPaper = NULL;
				hr = CStaticObjHelper::CopyObj(pObjPaper,pDB,pNewObjPaper);
				if(FAILED(hr))
				{
					return hr;
				}
				ASSERT(pNewObjPaper);
				m_lstClear.push_back(pNewObjPaper);
				CString strPaperName;
				hr = pNewObjPaper->GetPropVal(FIELD_YDPAPER_NAME,strPaperName);
				if(FAILED(hr))
				{
					return hr;
				}
				int index = m_cmbExamPaper.AddString(strPaperName);
				m_cmbExamPaper.SetItemData(index,(DWORD_PTR)(pNewObjPaper));
				m_cmbExamPaper.SetCurSel(index);
			}
			OnCbnSelchangeComboExamPaper();


		}

		{
			//开考时间 收卷时间
			m_timeStart  =pGExamStruct->m_pExamDetail->m_timeStart;
			m_timeEnd = pGExamStruct->m_pExamDetail->m_timeEnd;
		}
	}


	return S_OK;
}

void CFormExamBlue::OnCbnSelchangeComboExamSubject()
{
	// TODO: Add your control notification handler code here
	int iSel = m_cmbExamSubject.GetCurSel();
	if(iSel == -1)
	{
		return ;
	}
	UpdateData();
	HRESULT hr = E_FAIL;
	CYDSubjectRef* pSubRef = (CYDSubjectRef*)m_cmbExamSubject.GetItemData(iSel);
	ASSERT(pSubRef);
	std::list<CYDObjectRef*> lstPaper;
	hr = pSubRef->GetPapers(&lstPaper);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	m_cmbExamPaper.ResetContent();
	for(std::list<CYDObjectRef*>::const_iterator itr = lstPaper.begin();
		itr != lstPaper.end();++itr)
	{
		m_lstClear.push_back(*itr);
		CComVariant valPaperName;
		hr = (*itr)->GetPropVal(FIELD_YDPAPER_NAME,&valPaperName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CString strPaperName = CDataHandler::VariantToString(valPaperName);
		int index = m_cmbExamPaper.AddString(strPaperName);
		m_cmbExamPaper.SetItemData(index,(DWORD_PTR)(*itr));
	}
	if(lstPaper.size() > 0)
	{
		//默认将第一个考卷选择
		m_cmbExamPaper.SetCurSel(0);
		OnCbnSelchangeComboExamPaper();
	}
	
}


void CFormExamBlue::OnBnClickedButtonDetail()
{
	// TODO: Add your control notification handler code here
	CDlgExamBlueDetail dlg;
	if(dlg.DoModal() != IDOK)
	{
		return ;
	}
}



void CFormExamBlue::OnCbnSelchangeComboExamPaper()
{
	// TODO: Add your control notification handler code here
	int iSel = m_cmbExamPaper.GetCurSel();
	if(iSel == -1)
	{
		return ;
	}
	UpdateData();
	CYDObjectRef* pObjRef = (CYDObjectRef*)m_cmbExamPaper.GetItemData(iSel);
	ASSERT(pObjRef);
	HRESULT hr = E_FAIL;
	hr = UpdateLstSeqByPaperObj(pObjRef);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	UpdateData(FALSE);
}

HRESULT CFormExamBlue::UpdateLstSeqByPaperObj(CYDObjectRef* _pPaper)
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	//试题
	{
		CListAutoClean<CExamQuestionSegment> clr7(pGExamStruct->m_pExamDetail->m_lstQSeg);
	}
	
	{
		//第一段
		CComVariant valBegin;
		hr = _pPaper->GetPropVal(FIELD_YDPAPER_BEGIN1,&valBegin);
		if(FAILED(hr))
		{
			return hr;
		}
		int iBegin = CDataHandler::VariantToLong(valBegin);
		CComVariant valEnd;
		hr = _pPaper->GetPropVal(FIELD_YDPAPER_END1,&valEnd);
		if(FAILED(hr))
		{
			return hr;
		}
		int iEnd = CDataHandler::VariantToLong(valEnd);
		CComVariant valChoiceAnswer;
		hr = _pPaper->GetPropVal(FIELD_YDPAPER_ANSWERCHOICE1,&valChoiceAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
		int iChoiceAnswer = CDataHandler::VariantToLong(valChoiceAnswer);
		CComVariant valMultiType;
		hr = _pPaper->GetPropVal(FIELD_YDPAPER_MULTIFLAG1,&valMultiType);
		if(FAILED(hr))
		{
			return hr;
		}
		int iMultiType = CDataHandler::VariantToLong(valMultiType);
		if(iBegin > 0 && iEnd >= 0)
		{
			CExamQuestionSegment* pQSeg = new CExamQuestionSegment();
			pQSeg->m_iStart = iBegin;
			pQSeg->m_iEnd = iEnd;
			pQSeg->m_iOptionNum = iChoiceAnswer;
			pQSeg->m_iMultiType = iMultiType;
			pGExamStruct->m_pExamDetail->m_lstQSeg.push_back(pQSeg);
		}
	}

	{
		//第二段
		CComVariant valBegin;
		hr = _pPaper->GetPropVal(FIELD_YDPAPER_BEGIN2,&valBegin);
		if(FAILED(hr))
		{
			return hr;
		}
		int iBegin = CDataHandler::VariantToLong(valBegin);
		CComVariant valEnd;
		hr = _pPaper->GetPropVal(FIELD_YDPAPER_END2,&valEnd);
		if(FAILED(hr))
		{
			return hr;
		}
		int iEnd = CDataHandler::VariantToLong(valEnd);
		CComVariant valChoiceAnswer;
		hr = _pPaper->GetPropVal(FIELD_YDPAPER_ANSWERCHOICE2,&valChoiceAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
		int iChoiceAnswer = CDataHandler::VariantToLong(valChoiceAnswer);
		CComVariant valMultiType;
		hr = _pPaper->GetPropVal(FIELD_YDPAPER_MULTIFLAG2,&valMultiType);
		if(FAILED(hr))
		{
			return hr;
		}
		int iMultiType = CDataHandler::VariantToLong(valMultiType);
		if(iBegin > 0 && iEnd >= 0)
		{
			CExamQuestionSegment* pQSeg = new CExamQuestionSegment();
			pQSeg->m_iStart = iBegin;
			pQSeg->m_iEnd = iEnd;
			pQSeg->m_iOptionNum = iChoiceAnswer;
			pQSeg->m_iMultiType = iMultiType;
			pGExamStruct->m_pExamDetail->m_lstQSeg.push_back(pQSeg);
		}
	}

	{
		//第三段
		CComVariant valBegin;
		hr = _pPaper->GetPropVal(FIELD_YDPAPER_BEGIN3,&valBegin);
		if(FAILED(hr))
		{
			return hr;
		}
		int iBegin = CDataHandler::VariantToLong(valBegin);
		CComVariant valEnd;
		hr = _pPaper->GetPropVal(FIELD_YDPAPER_END3,&valEnd);
		if(FAILED(hr))
		{
			return hr;
		}
		int iEnd = CDataHandler::VariantToLong(valEnd);
		CComVariant valChoiceAnswer;
		hr = _pPaper->GetPropVal(FIELD_YDPAPER_ANSWERCHOICE3,&valChoiceAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
		int iChoiceAnswer = CDataHandler::VariantToLong(valChoiceAnswer);
		CComVariant valMultiType;
		hr = _pPaper->GetPropVal(FIELD_YDPAPER_MULTIFLAG3,&valMultiType);
		if(FAILED(hr))
		{
			return hr;
		}
		int iMultiType = CDataHandler::VariantToLong(valMultiType);
		if(iBegin > 0 && iEnd >= 0)
		{
			CExamQuestionSegment* pQSeg = new CExamQuestionSegment();
			pQSeg->m_iStart = iBegin;
			pQSeg->m_iEnd = iEnd;
			pQSeg->m_iOptionNum = iChoiceAnswer;
			pQSeg->m_iMultiType = iMultiType;
			pGExamStruct->m_pExamDetail->m_lstQSeg.push_back(pQSeg);
		}
	}
	return S_OK;
}

void CFormExamBlue::OnCbnSelchangeComboExamRoom()
{
	// TODO: Add your control notification handler code here
	//将所有的学生信息列出
	if(m_cmbExamRoom.GetCurSel() == CB_ERR)
	{
		return;
	}
	HRESULT hr = E_FAIL;
	m_lstCtrlUnit.DeleteAllItems();
	CYDExamAddress* pAddr = (CYDExamAddress*)m_cmbExamRoom.GetItemData(m_cmbExamRoom.GetCurSel());
	ASSERT(pAddr);
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	hr = ClearStu();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	{
		CListAutoClean<CYDObjectRef> clr(pGExamStruct->m_pExamDetail->m_lstUnit);
	}
	hr = pAddr->GetAllUnits(&(pGExamStruct->m_pExamDetail->m_lstUnit));
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = pGExamStruct->m_pExamDetail->m_lstUnit.begin();
		itr != pGExamStruct->m_pExamDetail->m_lstUnit.end();++itr)
	{
		hr = InsertUnit(*itr);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
	}
}

HRESULT CFormExamBlue::ClearStu()
{
	HRESULT hr = E_FAIL;
	CListAutoClean<CYDObjectRef> clr(m_lstStu);
	return S_OK;
}
HRESULT CFormExamBlue::InsertUnit(CYDObjectRef* _pUnit)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pUnit);
	int iCount = m_lstCtrlUnit.GetItemCount();
	CYDEAddrUnit* pUnit = (CYDEAddrUnit*)_pUnit;
	CYdStudentRef* pStuRef = NULL;
	hr = pUnit->GetStudent(pStuRef);
	if(FAILED(hr))
	{
		return hr;
	}
	CPtrAutoClean<CYdStudentRef> clr(pStuRef);
	CString strStuID;//学号
	//if (strStuID.IsEmpty())
	//{
	//	return S_FALSE;
	//}
	hr = pStuRef->GetPropVal(FIELD_YDSTU_ID,strStuID);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strStuName;//姓名
	hr = pStuRef->GetPropVal(FIELD_YDSTU_NAME,strStuName);
	if(FAILED(hr))
	{
		return hr;
	}
	std::list<CYDObjectRef*> lstClass;
	CListAutoClean<CYDObjectRef> clr1(lstClass);
	hr = pStuRef->GetClass(&lstClass);
	if(FAILED(hr))
	{
		return hr;
	}
	CString strClassName;//所属班级
	for(std::list<CYDObjectRef*>::const_iterator itr = lstClass.begin();
		itr != lstClass.end();++itr)
	{

		hr = (*itr)->GetPropVal(FIELD_YDCLASS_NAME,strClassName);
		if(FAILED(hr))
		{
			return hr;
		}
	}

	//准考证号
	CString strExamID;
	hr = pUnit->GetPropVal(FIELD_YDEXAMADDRUNIT_EXAMID,strExamID);
	if(FAILED(hr))
	{
		return hr;
	}
	CYDDevice* pDevice = NULL;
	hr = pUnit->GetDevice(pDevice);
	if(FAILED(hr))
	{
		return hr;
	}
	CPtrAutoClean<CYDDevice> clr2(pDevice);
	CString strStuEvid;//设备号
	hr = pDevice->GetPropVal(FIELD_YDDEVICE_MAC,strStuEvid);
	if(FAILED(hr))
	{
		return hr;
	}
	int index = m_lstCtrlUnit.InsertItem(iCount,strStuID);
	m_lstCtrlUnit.SetItemText(index,COL_LIST_STUDENT_NAME,strStuName);
	m_lstCtrlUnit.SetItemText(index,COL_LIST_STUDENT_CLASS,strClassName);
	m_lstCtrlUnit.SetItemText(index,COL_LIST_STUDENT_EXAM_ID,strExamID);
	m_lstCtrlUnit.SetItemText(index,COL_LIST_STUDENT_MAC,strStuEvid);
	m_lstCtrlUnit.SetItemData(index,DWORD_PTR(_pUnit));
	return S_OK;
}
void CFormExamBlue::OnBnClickedCheckSelAll()
{
	// TODO: Add your control notification handler code here
	BOOL bSel = FALSE;
	if(((CButton*)(GetDlgItem(IDC_CHECK_SEL_ALL)))->GetCheck())
	{
		bSel = TRUE;
	}
	for(int i = 0; i < m_lstCtrlUnit.GetItemCount();i++)
	{
		m_lstCtrlUnit.SetCheck(i,bSel);
	}
}

void CFormExamBlue::OnBnClickedCheckSelInverse()
{
	// TODO: Add your control notification handler code here
	for(int i = 0; i < m_lstCtrlUnit.GetItemCount();i++)
	{
		BOOL bCheck = m_lstCtrlUnit.GetCheck(i);
		m_lstCtrlUnit.SetCheck(i,!bCheck);
	}
}

HRESULT CFormExamBlue::EnableExamInfoBtn(BOOL _bEnable)
{
	HRESULT hr = E_FAIL;
	GetDlgItem(IDC_COMBO_EXAM_ROOM)->EnableWindow(_bEnable);
	GetDlgItem(IDC_COMBO_EXAM_KIND)->EnableWindow(_bEnable);
	GetDlgItem(IDC_COMBO_EXAM_SUBJECT)->EnableWindow(_bEnable);
	GetDlgItem(IDC_COMBO_EXAM_PAPER)->EnableWindow(_bEnable);
	GetDlgItem(IDC_DATETIMEPICKER_START)->EnableWindow(_bEnable);
	GetDlgItem(IDC_DATETIMEPICKER_END)->EnableWindow(_bEnable);
	GetDlgItem(IDC_BUTTON_DETAIL)->EnableWindow(_bEnable);
	GetDlgItem(IDC_BUTTON_NEW_PAPER)->EnableWindow(_bEnable);
	
	return S_OK;
}

HRESULT CFormExamBlue::EnableTeacherInfoBtn(BOOL _bEnable)
{
	GetDlgItem(IDC_TEACHER_TEST)->EnableWindow(_bEnable);
	GetDlgItem(IDC_TEACHER_CHOOSE)->EnableWindow(_bEnable);
	return S_OK;
}
HRESULT CFormExamBlue::EnableStudentInfoBtn(BOOL _bEnable)
{
	GetDlgItem(IDC_CHECK_SEL_ALL)->EnableWindow(_bEnable);
	GetDlgItem(IDC_CHECK_SEL_INVERSE)->EnableWindow(_bEnable);
	GetDlgItem(IDC_BUTTON_BEGINEXAM)->EnableWindow(_bEnable);
	GetDlgItem(IDC_BUTTON_ROLLING)->EnableWindow(_bEnable);
	GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(_bEnable);
	return S_OK;
}
void CFormExamBlue::OnBnClickedTeacherTest()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	CListBoxEx* pMsgListBox = (CListBoxEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_EXAM_MSG_LISTBOX);
	ASSERT(pMsgListBox);
	hr = CYDTeacherAppCom::FindTeacherCom();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}

	int iCount = pGExamStruct->m_lstTacherAppCom.size();
	if(iCount == 0)
	{
		AfxMessageBox(_T("没有检测到教师机！"));
		return;
	}

	CString strMessage;
	strMessage.Format(_T("共发现教师机 %d 台"), iCount);

	pMsgListBox->AddString(strMessage);

	m_lstCtrlTeacher.DeleteAllItems();
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);

	for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
		itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
	{
		CYDTeacherAppCom* pAppCom = (*itr);
		CString strMac, strCom;
		strMac.Format(_T("%s"),pAppCom->m_strMac);
		strCom.Format(_T("%s"),pAppCom->m_strComPort);
		if (strCom.GetLength() > 5 ) //如果串口号大于“com10”，去掉前面的“\\\\.\\”
		{
			strCom = strCom.Right(5);
		}
		strMac.MakeUpper();
		strMac.TrimLeft();
		strMac.TrimRight();
		std::list<CYDObjectRef*> lstTeacherDev;
		hr = CStaticObjHelper::GetObjRef(DB_YDTEACHERDEV,pDB,&lstTeacherDev);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		CListAutoClean<CYDObjectRef> clr(lstTeacherDev);

		for(std::list<CYDObjectRef*>::const_iterator itr = lstTeacherDev.begin();
			itr != lstTeacherDev.end();++itr)
		{
			CString strTeachID;
			hr = (*itr)->GetPropVal(FIELD_YDTEACHERDEV_TEACHID,strTeachID);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return;
			}
			CString strTeachName;
			hr = (*itr)->GetPropVal(FIELD_YDTEACHERDEV_NAME,strTeachName);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return;
			}
			CString strTeachMac;
			hr  = (*itr)->GetPropVal(FIELD_YDTEACHERDEV_DEVID,strTeachMac);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return;
			}
			if(strTeachMac.CompareNoCase(strMac) == 0)
			{
				int index = m_lstCtrlTeacher.InsertItem(0,strTeachID);
				m_lstCtrlTeacher.SetItemText(index,COL_LIST_TEACHER_NAME,strTeachName);
				m_lstCtrlTeacher.SetItemText(index,COL_LIST_TEACHER_PORT,strCom);
				m_lstCtrlTeacher.SetItemText(index,COL_LIST_TEACHER_STATE,_T("检测到，可使用"));
				m_lstCtrlTeacher.SetCheck(index);
				m_lstCtrlTeacher.SetItemData(index,(DWORD_PTR)(pAppCom));
			}
		}
	}
}

void CFormExamBlue::OnBnClickedTeacherChoose()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	UpdateData();
	if(!ValidateData(OP_VIEW))
	{
		return ;
	}
	hr = UpdateRollCallUI(TRUE);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	if(m_lstCtrlTeacher.GetItemCount() == 0)
	{
		AfxMessageBox(_T("没有教师机可以选择，请先点击“搜索”按钮，搜索教师机！"));
		return;
	}

	CString strTemp ;
	int i = 0;
	int iCount = 0;
	for(; i < m_lstCtrlTeacher.GetItemCount() ; i++)
	{
		if(m_lstCtrlTeacher.GetCheck(i))
		{
			CString strTeacherCOM = m_lstCtrlTeacher.GetItemText(i,COL_LIST_TEACHER_NAME);			
			strTemp += strTeacherCOM + _T(" ");
			iCount++;
		}
	}
	if(iCount == 0)
	{
		AfxMessageBox(_T("没有选择教师机，请在在要使用的教师机前打勾！"));
		return;
	}	
	CString msg;
	msg.Format(_T("确定要使用教师机 %s 参加本场考试？"), strTemp);
	if ( MessageBox(msg,_T("确认"),MB_OKCANCEL) != IDOK)
		return;	

	CListBoxEx* pMsgListBox = (CListBoxEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_EXAM_MSG_LISTBOX);
	ASSERT(pMsgListBox);
	for(int i = 0; i < m_lstCtrlTeacher.GetItemCount() ; i++)
	{
		if(m_lstCtrlTeacher.GetCheck(i))
		{
			CYDTeacherAppCom* pAppCom = (CYDTeacherAppCom*)m_lstCtrlTeacher.GetItemData(i);
			ASSERT(pAppCom);
			hr = pAppCom->OpenCom();
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return ;
			}
		}
	}
	for(int i = 0; i < m_lstCtrlTeacher.GetItemCount(); i++)
	{
		CString strCom = m_lstCtrlTeacher.GetItemText(i,2);
		CYDTeacherAppCom* pAppCom = (CYDTeacherAppCom*)m_lstCtrlTeacher.GetItemData(i);
		ASSERT(pAppCom);
		BOOL bState = FALSE;
		hr = pAppCom->GetState(bState);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		if(bState)
		{
			CString strMessage;
			strMessage.Format(_T("教师机 %s 确认被使用！"), m_lstCtrlTeacher.GetItemText(i,COL_LIST_TEACHER_ID));
			pMsgListBox->AddString(strMessage);		
			m_lstCtrlTeacher.SetItemText(i,COL_LIST_TEACHER_STATE,_T("使用中！"));
		}
		else
		{	
			CString strMessage;
			strMessage.Format(_T("教师机 %s 未被使用！"), m_lstCtrlTeacher.GetItemText(i,COL_LIST_TEACHER_ID));
			pMsgListBox->AddString(strMessage);			
			m_lstCtrlTeacher.SetItemText(i,COL_LIST_TEACHER_STATE,_T("未被使用！"));
		}
	}
	EnableExamInfoBtn(FALSE);
	EnableStudentInfoBtn(TRUE);
	hr = ShowRollInfoButton();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	if(m_strFileXml.IsEmpty())
	{
		//以试卷名+考场编号+时间来确定文件名
		CString strPaperName;
		hr = pGExamStruct->m_pExamDetail->m_pPaper->GetPropVal(L"NAME",strPaperName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		CString strExamAddrName;
		hr = pGExamStruct->m_pExamDetail->m_pObjAddress->GetPropVal(FIELD_YDEXAMADDR_ID,strExamAddrName);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		COleDateTime date = COleDateTime::GetCurrentTime();
		CString strCurTime;
		strCurTime.Format(_T("%d-%d-%d"),date.GetHour(),date.GetMinute(),date.GetSecond());

		m_strFileXml = _ModulePath;
		m_strFileXml += _T("\\Info\\");
		m_strFileXml += strPaperName;
		m_strFileXml += _T("_");
		m_strFileXml += strExamAddrName;
		m_strFileXml += _T("_");
		m_strFileXml += strCurTime;
		m_strFileXml += _T(".xml");
	}
	CInvigilateInfo InvigilateInfo;
	hr = InvigilateInfo.Save(CComBSTR(m_strFileXml));
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	pGExamStruct->m_pExamDetail->m_strXmlFile = m_strFileXml;
	CInitInfoXml InitInfoXml;
	hr = InitInfoXml.SaveInitXmlInfo(pGExamStruct->m_pExamDetail->m_strXmlFile);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
}

HRESULT CFormExamBlue::ShowRollInfoButton()
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	if(pGExamStruct->m_bExtractOnlyFromTeacher)
	{
		GetDlgItem(IDC_BUTTON_BEGINEXAM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_SEARCH)->SetWindowText(_T("学生信息"));
	}
	else
	{
		//如果是一般测试，则开考按钮不用显示

		if(pGExamStruct->m_pExamDetail->m_uExamKind == YDEXAM_KIND_NORMAL)
		{
			//正式考试显示开考按钮
			GetDlgItem(IDC_BUTTON_BEGINEXAM)->ShowWindow(SW_SHOW);
		}
		else
		{
			//一般测试不显示开考按钮
			GetDlgItem(IDC_BUTTON_BEGINEXAM)->ShowWindow(SW_HIDE);
			//一般测试可以直接收卷
			GetDlgItem(IDC_BUTTON_ROLLING)->EnableWindow(TRUE);
		}
		GetDlgItem(IDC_BUTTON_SEARCH)->SetWindowText(_T("点名"));

	}
	return S_OK;
}
BOOL CFormExamBlue::ValidateData(OPERATION nOp)
{
	if(m_cmbExamRoom.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("请选择考场！"));
		return FALSE;
	}
	if(m_cmbExamKind.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("请选择考试性质！"));
		return FALSE;
	}
	if(m_cmbExamSubject.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("请选择考试科目！"));
		return FALSE;
	}
	if(m_cmbExamPaper.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("请选择考试试卷！"));
		return FALSE;
	}
	if(m_timeEnd < m_timeStart)
	{
		AfxMessageBox(_T("开始时间不能在结束时间之后！"));
		return FALSE;
	}
	return TRUE;
}

void CFormExamBlue::OnBnClickedButtonSearch()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	if(pGExamStruct->m_bExtractOnlyFromTeacher)
	{
		//只是从教师机中读取学生机的信息
		hr = ReadStudevOnlyFromTeacher();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		GetDlgItem(IDC_BUTTON_ROLLING)->EnableWindow(TRUE);
	}
	else
	{
		m_bCancelThread = FALSE;
		{
			CWnd* pSearchButton = GetDlgItem(IDC_BUTTON_SEARCH);
			ASSERT(pSearchButton);
			CControlWnd ControlWnd(pSearchButton);
		}
		GetDlgItem(IDC_EDIT_INFO)->SetWindowText(_T(""));
		m_bCanClose = FALSE;
		CWinThread* pThread = ::AfxBeginThread((AFX_THREADPROC)SearchThread, this);
		if (!pThread)	
		{
			ASSERT(FALSE);
			return ;
		}
	}
}

HRESULT CFormExamBlue::ReadStudevOnlyFromTeacher()
{
	HRESULT hr= E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);

	UINT uCount = 0;
	for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
		itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
	{
		BOOL bOpen = FALSE;
		hr = (*itr)->GetState(bOpen);
		if(FAILED(hr))
		{
			return hr;
		}
		if(!bOpen)
		{
			continue;
		}
		hr = (*itr)->UpdateOnlyFromTeacher();
		if(FAILED(hr))
		{
			return hr;
		}
		for(std::list<CStudev*>::const_iterator itrStudev = (*itr)->m_lstStuDev.begin();
			itrStudev != (*itr)->m_lstStuDev.end();++itrStudev)
		{
			hr = UpdateListByStudev(*itrStudev);
			if(FAILED(hr))
			{
				return hr;
			}
		}
		uCount += (*itr)->m_lstStuDev.size();
	}
	CString strInfo;
	strInfo.Format(_T("找到%d台学生机信息！"),uCount);
	GetDlgItem(IDC_EDIT_INFO)->SetWindowText(strInfo);
	return S_OK;
}

HRESULT CFormExamBlue::UpdateListByStudev(CStudev* _pStudev)
{
	HRESULT hr= E_FAIL;
	for(int j = 0; j < m_lstCtrlUnit.GetItemCount();j++)
	{
		CYDEAddrUnit* pUnit = (CYDEAddrUnit*)m_lstCtrlUnit.GetItemData(j);
		ASSERT(pUnit);
		CYDDevice* pDevice = NULL;
		hr = pUnit->GetDevice(pDevice);
		if(FAILED(hr))
		{
			return hr;
		}
		CPtrAutoClean<CYDDevice> clr(pDevice);
		CString strDevMacID;
		hr = pDevice->GetPropVal(FIELD_YDDEVICE_MAC,strDevMacID);
		if(FAILED(hr))
		{
			return hr;
		}
		strDevMacID.TrimRight();
		CString strMac;
		hr = _pStudev->GetMac(strMac);
		if(FAILED(hr))
		{
			return hr;
		}
		if(strDevMacID.CompareNoCase(strMac) == 0)
		{
			int iRolling = -1;
			hr = _pStudev->GetRolling(iRolling);
			if(FAILED(hr))
			{
				return hr;
			}
			int iRollCall = -1;
			hr = _pStudev->GetRollCall(iRollCall);
			if(FAILED(hr))
			{
				return hr;
			}
			if(iRolling == 1)
			{
				m_lstCtrlUnit.SetItemText(j,COL_LIST_STUDENT_STATE,_T("已收卷！"));
			}
			else if(iRollCall == 1)
			{
				m_lstCtrlUnit.SetItemText(j,COL_LIST_STUDENT_STATE,_T("进入考试状态！"));
			}
			m_lstCtrlUnit.SetCheck(j);
			break;
		}
	}
	return S_OK;
}



void CFormExamBlue::OnBnClickedButtonBeginexam()
{
	// TODO: Add your control notification handler code here
	//开考
	std::list<CString>  lstStuDevMac;
	HRESULT hr = E_FAIL;
	{
		CWnd* pBeginExamButton = GetDlgItem(IDC_BUTTON_BEGINEXAM);
		ASSERT(pBeginExamButton);
		CControlWnd ControlWnd(pBeginExamButton);
		for(int i = 0; i < m_lstCtrlUnit.GetItemCount(); i++)
		{
			if(m_lstCtrlUnit.GetCheck(i))
			{
				CYDEAddrUnit* pObjUnit = (CYDEAddrUnit*)m_lstCtrlUnit.GetItemData(i);
				ASSERT(pObjUnit);
				CYDDevice* pDev = NULL;
				hr = pObjUnit->GetDevice(pDev);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				CPtrAutoClean<CYDDevice> clr(pDev);
				CString strMac;
				hr = pDev->GetPropVal(FIELD_YDDEVICE_MAC,strMac);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				lstStuDevMac.push_back(strMac);
			}
		}
		if(lstStuDevMac.size() <= 0)
		{
			AfxMessageBox(_T("没有需要参加考试的学生，无法开考！"));
			return;
		}
		{
			CYDProgressControl YDProgressControl(this);
			YDProgressControl.SetRange(10,TRUE);
			hr = DevideMacToTeacherApp(lstStuDevMac);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return ;
			}
			for(std::list<CTeacherAppToMacList*>::const_iterator itr = m_lstAppToMacList.begin();
				itr != m_lstAppToMacList.end();++itr)
			{
				hr = (*itr)->m_pTeacherAppCom->Reset();
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				Sleep(500);
				hr = (*itr)->m_pTeacherAppCom->Sendvalidmac((*itr)->m_lstMacStu);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				Sleep(500);
				hr = (*itr)->m_pTeacherAppCom->Sendexaminfo();
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
			}
		}
	}

	if(lstStuDevMac.size() > 0)
	{
		GetDlgItem(IDC_EDIT_INFO)->SetWindowText(_T(""));
		m_bCanClose = FALSE; 
		CWinThread* pThread = ::AfxBeginThread((AFX_THREADPROC)BeginExamThread, this);
		if (!pThread)	
		{
			ASSERT(FALSE);
			return ;
		}
	}

}

HRESULT CFormExamBlue::DevideMacToTeacherApp(std::list<CString>  &_lstStuDevMac)
{
	HRESULT hr = E_FAIL;
	{
		CListAutoClean<CTeacherAppToMacList> clr(m_lstAppToMacList);
	}
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	std::list<CYDTeacherAppCom*> lstOpenCom;
	for(std::list<CYDTeacherAppCom*>::const_iterator itrCom = pGExamStruct->m_lstTacherAppCom.begin();
		itrCom != pGExamStruct->m_lstTacherAppCom.end();++itrCom)
	{
		BOOL bOpen = FALSE;
		hr = (*itrCom)->GetState(bOpen);
		if(FAILED(hr))
		{
			return hr;
		}
		if(bOpen)
		{
			lstOpenCom.push_back(*itrCom);
		}
	}
	ASSERT(lstOpenCom.size() > 0);
	std::list<CYDTeacherAppCom*>::const_iterator itrCom = lstOpenCom.begin();
	for(std::list<CString>::const_iterator itr = _lstStuDevMac.begin();
		itr != _lstStuDevMac.end();++itr,++itrCom)
	{
		if(itrCom == lstOpenCom.end())
		{
			itrCom = lstOpenCom.begin();
		}
		CTeacherAppToMacList* pAppToMacList = NULL;
		for(std::list<CTeacherAppToMacList*>::const_iterator itrAppToMac = m_lstAppToMacList.begin();
			itrAppToMac != m_lstAppToMacList.end();++itrAppToMac)
		{
			if((*itrAppToMac)->m_pTeacherAppCom == (*itrCom))
			{
				pAppToMacList = (*itrAppToMac);
			}
		}
		if(pAppToMacList == NULL)
		{
			//没有找到，插入一个新的
			pAppToMacList = new CTeacherAppToMacList();
			pAppToMacList->m_pTeacherAppCom = (*itrCom);
			m_lstAppToMacList.push_back(pAppToMacList);
		}
		ASSERT(pAppToMacList);
		pAppToMacList->m_lstMacStu.push_back(*itr);
	}
	return S_OK;
}

void CFormExamBlue::OnBnClickedButtonRolling()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	BOOL bCheck = FALSE;
	for(int i = 0; i < m_lstCtrlUnit.GetItemCount();i++)
	{
		if(m_lstCtrlUnit.GetCheck(i))
		{
			bCheck = TRUE;
			break;
		}
	}
	if(!bCheck)
	{
		AfxMessageBox(_T("至少选择一个学生机收卷！"));
		return ;
	}
	//收卷
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	{
		CWnd* pRollingButton = GetDlgItem(IDC_BUTTON_ROLLING);
		ASSERT(pRollingButton);
		CControlWnd ControlWnd(pRollingButton);
		CYDProgressControl YDProgressControl(this);
		YDProgressControl.SetRange(10,TRUE);
		
		
		COleDateTime endtime;
		endtime = pGExamStruct->m_pExamDetail->m_timeEnd;
#ifdef DEBUG
		CString strEndTime = endtime.Format(YD_DATE_FORMAT);
#endif
		if(!pGExamStruct->m_bExtractOnlyFromTeacher && 
			pGExamStruct->m_pExamDetail->m_uExamKind == YDEXAM_KIND_NORMAL)
		{
			//正式考试时加个判断，时间未到不能收卷
			COleDateTime dtCur = COleDateTime::GetCurrentTime();
			if(endtime > dtCur)
			{
				AfxMessageBox(_T("还没有到收卷时间，不能收卷"));
				return;
			}
		}
		if(pGExamStruct->m_pExamDetail->m_uExamKind == YDEXAM_KIND_TEST
			||pGExamStruct->m_bExtractOnlyFromTeacher)
		{
			//一般测试多发一个设置有效MAC地址的指令
			std::list<CString> lstMac;
			for(int i = 0; i < m_lstCtrlUnit.GetItemCount();i++)
			{
				if(!m_lstCtrlUnit.GetCheck(i))
				{
					continue;
				}
				CYDEAddrUnit* pObjUnit = (CYDEAddrUnit*)m_lstCtrlUnit.GetItemData(i);
				ASSERT(pObjUnit);
				CYDDevice* pDev = NULL;
				hr = pObjUnit->GetDevice(pDev);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				CPtrAutoClean<CYDDevice> clr(pDev);
				CString strMac;
				hr = pDev->GetPropVal(FIELD_YDDEVICE_MAC,strMac);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				lstMac.push_back(strMac);
			}
			hr = DevideMacToTeacherApp(lstMac);
			if(FAILED(hr))
			{
				DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
				return ;
			}
			if(!pGExamStruct->m_bExtractOnlyFromTeacher)
			{
				for(std::list<CTeacherAppToMacList*>::const_iterator itr = m_lstAppToMacList.begin();
					itr != m_lstAppToMacList.end();++itr)
				{
					CYDTeacherAppCom* pAppCom = (*itr)->m_pTeacherAppCom;
					ASSERT(pAppCom);
					hr = pAppCom->TestKinkSendvalidMac((*itr)->m_lstMacStu);
					if(FAILED(hr))
					{
						DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
						return ;
					}
					Sleep(500);
				}

			}
		}
		if(!pGExamStruct->m_bExtractOnlyFromTeacher)
		{
			for(std::list<CTeacherAppToMacList*>::const_iterator itr = m_lstAppToMacList.begin();
				itr != m_lstAppToMacList.end();++itr)
			{
				CYDTeacherAppCom* pAppCom = (*itr)->m_pTeacherAppCom;
				ASSERT(pAppCom);
				hr = pAppCom->Rolling(pGExamStruct->m_pExamDetail->m_lstQSeg);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
			}
			Sleep(3000);
		}

	}
	GetDlgItem(IDC_EDIT_INFO)->SetWindowText(_T(""));
	m_bCanClose = FALSE;
	CWinThread* pThread = ::AfxBeginThread((AFX_THREADPROC)RollingThread, this);
	if (!pThread)	
	{
		ASSERT(FALSE);
		return ;
	}
}

void CFormExamBlue::OnBnClickedButtonCancelProgress()
{
	// TODO: Add your control notification handler code here
	m_bCancelThread = TRUE;
}

void CFormExamBlue::OnBnClickedButtonQuerymark()
{
	// TODO: Add your control notification handler code here
	//成绩查询
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	pGExamStruct->m_TeacherType = GFROM_TEACHER;
	COpenObjExParam OpenObjExParam;
	OpenObjExParam.m_strName = EXAM_MARK_INFO;
	OpenObjExParam.m_Op = OP_VIEW;
	OpenObjExParam.m_bUseStaicStruct = TRUE;
	hr = AfxGetMainWnd()->SendMessage(WM_YD_OPEN_OBJECT_EX,(WPARAM)(&OpenObjExParam),OP_VIEW);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
}

void CFormExamBlue::OnBnClickedButtonPaperAnsys()
{
	// TODO: Add your control notification handler code here
	//试卷分析
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	pGExamStruct->m_TeacherType = GFROM_TEACHER;
	COpenObjExParam OpenObjExParam;
	OpenObjExParam.m_strName = EXAM_PAPER_ANSYS;
	OpenObjExParam.m_Op = OP_VIEW;
	OpenObjExParam.m_bUseStaicStruct = TRUE;
	hr = AfxGetMainWnd()->SendMessage(WM_YD_OPEN_OBJECT_EX,(WPARAM)(&OpenObjExParam),OP_VIEW);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
}

void CFormExamBlue::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	if(pGExamStruct->m_pExamDetail->m_nSetp == YDEXAM_STEP_BGGINEXAM)
	{
		AfxMessageBox(_T("已经进入开考状态，系统不能退出！"));
		return;
	}
	pGExamStruct->m_pExamDetail->m_nSetp = YDEXAM_STEP_NOT;
	CYdFormView::OnClose();
}

void CFormExamBlue::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		if(m_bTimeProgressStatus)
		{
			m_ctrlProgressStaus.StepIt();
			int iLow,iHight;
			m_ctrlProgressStaus.GetRange(iLow,iHight);
			if(iHight <= m_ctrlProgressStaus.GetPos())
				m_ctrlProgressStaus.SetPos(0);
		}
	}
	CYdFormView::OnTimer(nIDEvent);
}

void CFormExamBlue::OnNMDblclkListStudent2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem <0)
		return;
	HRESULT hr = E_FAIL;
	CYDEAddrUnit* pUnit = (CYDEAddrUnit*)m_lstCtrlUnit.GetItemData(pNMListView->iItem );
	ASSERT(pUnit);
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CStudev* pStudev = NULL;
	hr = GetStudevByUnit(pUnit,pGExamStruct,pStudev);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	if(pStudev == NULL)
	{
		return ;
	}
	int iRolling = -1;
	hr = pStudev->GetRolling(iRolling);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	if(iRolling != 1)
	{
		//还没有收卷成功
		return ;
	}
	CDlgResDetail dlg;
	dlg.m_ResultType = FROM_TEACHER_EXAM;
	dlg.m_pUnit = pUnit;
	dlg.m_pStudev = pStudev;
	dlg.DoModal();
}

HRESULT CFormExamBlue::GetStudevByUnit(CYDEAddrUnit* _pUnit,
									   CGExamStruct* _pGExamStruct,
									   CStudev* &_pStudev)
{
	HRESULT hr = E_FAIL;
	CYDDevice* pDevice = NULL;
	hr = _pUnit->GetDevice(pDevice);
	if(FAILED(hr))
	{
		return hr;
	}
	CPtrAutoClean<CYDDevice> clr(pDevice);
	CString strStuEvid;//设备号
	hr = pDevice->GetPropVal(FIELD_YDDEVICE_MAC,strStuEvid);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CYDTeacherAppCom*>::const_iterator itr = _pGExamStruct->m_lstTacherAppCom.begin();
		itr != _pGExamStruct->m_lstTacherAppCom.end();++itr)
	{
		BOOL bOpen = FALSE;
		hr = (*itr)->GetState(bOpen);
		if(FAILED(hr))
		{
			return hr;
		}
		if(!bOpen)
		{
			continue;
		}
		for(std::list<CStudev*>::const_iterator itrDev = (*itr)->m_lstStuDev.begin();
			itrDev != (*itr)->m_lstStuDev.end();++itrDev)
		{
			CString strItrMac;
			hr = (*itrDev)->GetMac(strItrMac);
			if(FAILED(hr))
			{
				return hr;
			}
			CString strItrDevMac = strItrMac;
			strItrDevMac.TrimRight();
			if(strItrDevMac.CompareNoCase(strStuEvid) == 0)
			{
				_pStudev = (*itrDev);
				return S_OK;
			}
		}
	}
	return S_FALSE;
}


void CFormExamBlue::OnNMRClickListStudent2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	*pResult = 0;
	if(pNMItemActivate->iItem < 0)
	{
		return;
	}
	CPoint pt;
	GetCursorPos(&pt);
	CPoint point(pt);
	m_lstCtrlUnit.ScreenToClient(&pt);
	UINT uFlags;

	int nItem = m_lstCtrlUnit.HitTest(pt, &uFlags);
	if (nItem != -1)
	{
		m_lstCtrlUnit.SetItem(nItem, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, 
			LVIS_SELECTED, 0);

		CYDObjectRef* pObjStu = (CYDObjectRef*)m_lstCtrlUnit.GetItemData(nItem);
		ASSERT(pObjStu);
		CMenu* pSubMenu = NULL;
		CMenu menu;
		menu.LoadMenu(IDR_MENU_LIST_USB_STU);
		pSubMenu = menu.GetSubMenu(0);
		CPoint pt;
		GetCursorPos(&pt);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
		//((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
	}

}

void CFormExamBlue::OnMiUSBBeginExam()
{
	// TODO: Add your command handler code here
	CDlgOnlineBeginExam dlg(this);
	dlg.DoModal();
	HRESULT hr = E_FAIL;
	for(std::list<CYDStuAppCom*>::const_iterator itr = dlg.m_lstStuAppCom.begin();
		itr != dlg.m_lstStuAppCom.end();++itr)
	{
		hr = UpdateListBeginExamState(*itr);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

	}


}


void CFormExamBlue::OnMiUSBRolling()
{
	// TODO: Add your command handler code here
	CDlgOnlineRolling dlg(this);
	dlg.DoModal();
	HRESULT hr = E_FAIL;
	for(std::list<CYDStuAppCom*>::const_iterator itr = dlg.m_lstStuAppCom.begin();
		itr != dlg.m_lstStuAppCom.end();++itr)
	{
		hr = UpdateListRollingState(*itr);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}

	}
}


void CFormExamBlue::OnMiStudentDetailInfo()
{
	POSITION pos = m_lstCtrlUnit.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		AfxMessageBox(_T("请在列表上选中一条记录！"));
		return;
	}
	int nItem = m_lstCtrlUnit.GetNextSelectedItem(pos);
	CYDEAddrUnit* pObjUnit = (CYDEAddrUnit*)m_lstCtrlUnit.GetItemData(nItem);
	ASSERT(pObjUnit);
	HRESULT hr = E_FAIL;
	CYdStudentRef* pStuRef;
	hr = pObjUnit->GetStudent(pStuRef);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	ASSERT(pStuRef);
	CPtrAutoClean<CYdStudentRef> clr(pStuRef);
	CDlgStunfo	dlg(OP_VIEW, this);
	dlg.m_pStudent = pStuRef;
	dlg.DoModal();
}

void CFormExamBlue::OnMiContinueBeginExam()
{
	POSITION pos = m_lstCtrlUnit.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		AfxMessageBox(_T("请在列表上选中一条记录！"));
		return;
	}
	int nItem = m_lstCtrlUnit.GetNextSelectedItem(pos);
	CYDEAddrUnit* pObjUnit = (CYDEAddrUnit*)m_lstCtrlUnit.GetItemData(nItem);
	ASSERT(pObjUnit);
	HRESULT hr = E_FAIL;
	CString strMac;
	hr = pObjUnit->GetDeviceMac(strMac);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	CWaitCursor wait;
	CGExamStruct* pGExampStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExampStruct);
	ASSERT(pGExampStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}

	HRESULT hrBeginExam = E_HRESULT_EXAM_NOT_IN_STU;
	for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExampStruct->m_lstTacherAppCom.begin();
		itr != pGExampStruct->m_lstTacherAppCom.end();++itr)
	{
		BOOL bOpen = FALSE;
		hr = (*itr)->GetState(bOpen);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		if(!bOpen)
		{
			continue;
		}
		hr = (*itr)->BeginExamSingle(strMac);
		if(hr == E_HRESULT_EXAM_NOT_IN_STU)
		{
			//说明当前学生机不在当前教师机中
			continue;
		}
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		hrBeginExam = S_OK;
		break;
	}
	if(FAILED(hrBeginExam))
	{
		//当前学生机不在点名的列表中
		DISPLAY_YDERROR(hrBeginExam,MB_OK|MB_ICONINFORMATION);
		return;
	}
}

HRESULT CFormExamBlue::OnProgressShow(WPARAM wParam,LPARAM lParam)
{
	m_WaitTxt.ShowWindow(wParam);
	m_ctrlProgressStaus.ShowWindow(wParam);
	GetDlgItem(IDC_EDIT_PROGRESS)->ShowWindow(wParam);
	return S_OK;
}
HRESULT CFormExamBlue::OnProgressShowCancel(WPARAM wParam,LPARAM lParam)
{
	GetDlgItem(IDC_BUTTON_CANCEL_PROGRESS)->ShowWindow(wParam);
	return S_OK;
}

HRESULT CFormExamBlue::OnProgressRange(WPARAM wParam,LPARAM lParam)
{
	HRESULT hr = E_FAIL;
	m_ctrlProgressStaus.SetRange32(0,wParam);
	m_ctrlProgressStaus.SetPos(0);
	m_ctrlProgressStaus.SetStep(1);
	m_bTimeProgressStatus = (BOOL)lParam;
	if(m_bTimeProgressStatus)
	{
		GetDlgItem(IDC_EDIT_PROGRESS)->ShowWindow(SW_HIDE);
	}
	else
	{
		UpdateData();
		hr = SetProgressEditTxt();
		if(FAILED(hr))
		{
			return hr;
		}
		UpdateData(FALSE);
	}
	return S_OK;
}

HRESULT CFormExamBlue::SetProgressEditTxt()
{
	int nLow,nUp;
	m_ctrlProgressStaus.GetRange(nLow,nUp);
	int iTotalCount = nUp - nLow;
	int nPos = m_ctrlProgressStaus.GetPos() - nLow;
	int iCur =(int)(((float)nPos/iTotalCount)*100);
	m_strProgress.Format(_T("进度%d/%d(%d%%)"),nPos,iTotalCount,iCur);
	return S_OK;
}
HRESULT CFormExamBlue::OnProgressStepIt(WPARAM wParam, LPARAM lParam)
{
	UpdateData();
	HRESULT hr = E_FAIL;
	m_ctrlProgressStaus.StepIt();
	if(!m_bTimeProgressStatus)
	{
		hr = SetProgressEditTxt();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	UpdateData(FALSE);
	return S_OK;
}

HRESULT CFormExamBlue::UpdateListBeginExamState(CYDStuAppCom* _pStuCom)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pStuCom);
	BOOL bOpen = FALSE;
	hr = _pStuCom->GetState(bOpen);
	if(FAILED(hr))
	{
		return hr;
	}
	if(!bOpen)
	{
		return S_FALSE;
	}
	if(!_pStuCom->m_bBeginExam)
	{
		return S_FALSE;
	}
	//当前学生机开考成功
	int nItem = -1;
	hr = GetItemByStuCom(_pStuCom,nItem);
	if(FAILED(hr))
	{
		return hr;
	}
	if(nItem == -1)
	{
		return S_FALSE;
	}
	m_lstCtrlUnit.SetCheck(nItem);
	m_lstCtrlUnit.SetItemText(nItem,4,_T("进入考试状态"));
	CString strStart = CDataHandler::DateTimeToStr(_pStuCom->m_timeBeginExam,2);
	m_lstCtrlUnit.SetItemText(nItem,5,strStart);
	return S_OK;
}

HRESULT CFormExamBlue::UpdateListRollingState(CYDStuAppCom* _pStuCom)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pStuCom);
	BOOL bOpen = FALSE;
	hr = _pStuCom->GetState(bOpen);
	if(FAILED(hr))
	{
		return hr;
	}
	if(!bOpen)
	{
		return S_FALSE;
	}
	if(!_pStuCom->m_bRolling)
	{
		return S_FALSE;
	}
	//当前学生机收卷成功
	int nItem = -1;
	hr = GetItemByStuCom(_pStuCom,nItem);
	if(FAILED(hr))
	{
		return hr;
	}
	if(nItem == -1)
	{
		return S_FALSE;
	}
	m_lstCtrlUnit.SetCheck(nItem);
	m_lstCtrlUnit.SetItemText(nItem,4,_T("已收卷"));
	CString strStart = CDataHandler::DateTimeToStr(_pStuCom->m_timeRolling,2);
	m_lstCtrlUnit.SetItemText(nItem,6,strStart);
	return S_OK;
}

HRESULT CFormExamBlue::GetItemByStuCom(CYDStuAppCom* _pStuCom,int& _nItem)
{
	HRESULT hr = E_FAIL;
	_nItem = -1;
	for(int i = 0; i < m_lstCtrlUnit.GetItemCount();i++)
	{
		CYDEAddrUnit* pUnit = (CYDEAddrUnit*)m_lstCtrlUnit.GetItemData(i);
		ASSERT(pUnit);
		CString strUnitMac;
		hr =pUnit->GetDeviceMac(strUnitMac);
		if(FAILED(hr))
		{
			return hr;
		}
		if(strUnitMac.CompareNoCase(_pStuCom->m_strMac) == 0)
		{
			_nItem = i;
			return S_OK;
		}
	}
	return S_OK;
}

HRESULT CFormExamBlue::StartThreadTeacherComJump()
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pJumbThread == NULL);
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	{
		CCriticalSectionControl cc(&(pGExamStruct->m_cs));
		CListAutoClean<CYDTeacherAppCom> clr(pGExamStruct->m_lstTacherAppCom);

	}

	pGExamStruct->m_bThreadJump = TRUE;
	m_pJumbThread = ::AfxBeginThread((AFX_THREADPROC)JumpTeacherComThread,0);
	if (!m_pJumbThread)	
	{
		ASSERT(FALSE);
		return E_FAIL;
	}
	return S_OK;
}
HRESULT CFormExamBlue::EndThreadTeacherComJum()
{
	ASSERT(m_pJumbThread);
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	pGExamStruct->m_pExamDetail->m_nSetp = YDEXAM_STEP_NOT;
	pGExamStruct->m_bThreadJump = FALSE;
	WaitForSingleObject(m_pJumbThread->m_hThread,10000);
	//m_pJumbThread = NULL;
	return S_OK;
}

HRESULT CFormExamBlue::Close()
{
	HRESULT hr = E_FAIL;
	ASSERT(m_pSheet);
	if(!m_bCanClose)
	{
		DISPLAY_YDERROR(E_HRESULT_NOT_CLOSE_BY_THREAD,MB_OK|MB_ICONINFORMATION);
		return S_FALSE;
	}
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return S_FALSE;
	}
	if(pGExamStruct->m_pExamDetail->m_nSetp == YDEXAM_STEP_BGGINEXAM)
	{
		AfxMessageBox(_T("已经进入开考状态，系统不能退出！"));
		return S_FALSE;
	}
	hr = EndThreadTeacherComJum();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return S_FALSE;
	}
	return S_OK;
}

HRESULT CFormExamBlue::InitExamClassRoom()//初始化考场
{
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	std::list<CYDObjectRef*> lstExamAddr;//考场
	hr = CStaticObjHelper::GetObjRef(DB_YDEXAMADDR,pDB,&lstExamAddr);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = lstExamAddr.begin();
		itr != lstExamAddr.end();++itr)
	{
		m_lstClear.push_back(*itr);
		CComVariant valExamAddr;
		hr = (*itr)->GetPropVal(FIELD_YDEXAMADDR_ADDR,&valExamAddr);
		if(FAILED(hr))
		{
			return hr;
		}
		CString strExamAddr = CDataHandler::VariantToString(valExamAddr);
		int index = m_cmbExamRoom.AddString(strExamAddr);
		m_cmbExamRoom.SetItemData(index,(DWORD_PTR)(*itr));
	}

	return S_OK;
}

HRESULT CFormExamBlue::InitExamSubject()//初始化考试科目
{
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	std::list<CYDObjectRef*> lstExamSubject;//考场
	hr = CStaticObjHelper::GetObjRef(DB_YDSUBJECT,pDB,&lstExamSubject);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = lstExamSubject.begin();
		itr != lstExamSubject.end();++itr)
	{
		m_lstClear.push_back(*itr);
		CString strSubjectName;
		hr = (*itr)->GetPropVal(FIELD_YDSUBJECT_NAME,strSubjectName);
		if(FAILED(hr))
		{
			return hr;
		}
		int index = m_cmbExamSubject.AddString(strSubjectName);
		m_cmbExamSubject.SetItemData(index,(DWORD_PTR)(*itr));
	}

	return S_OK;
}

void CFormExamBlue::OnBnClickedButtonNewPaper()
{
	// TODO: Add your control notification handler code here
	CDlgPaper dlg;
	dlg.DoModal();
}
