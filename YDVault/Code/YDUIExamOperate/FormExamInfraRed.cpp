
// IRTESTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FormExamInfraRed.h"
#include <string>
#include "../\ObjHelper\StaticObjHelper.h"
#include "../Base\AutoClean.h"
#include "../ObjRef\YDObjectRef.h"
#include "../Base\DataHandler.h"
#include "../YDExamObjRef\YDSubjectRef.h"
#include "../YDExamObjRef\YDExamAddress.h"
#include "../YDExamObjRef\YDEAddrUnit.h"
#include "../YdCom\SearchDevCom.h"
#include "../ydCom/YDInfraRedAppCom.h"
#include "../YDExamObjRef\YDStuMark.h"
#include "DlgResDetail.h"
#include "../DBBase\DBTransactionRef.h"
#include "DlgPaper.h"
#include "../Base/CriticalSectionControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CONST_COL_STUID = 0 ;//学号
const int CONST_COL_STUNAME = 1 ;//姓名
const int CONST_COL_CLASS = 2 ;//班级
const int CONST_COL_DEVICEID = 3 ;//设备号
const int CONST_COL_RECEIVE_STATE = 4 ;//考试状态
const int CONST_COL_ANSWER_MARK = 5 ;//分数


// CFormExamInfraRed 对话框


IMPLEMENT_DYNCREATE(CFormExamInfraRed, CYdFormView)

CFormExamInfraRed::CFormExamInfraRed()
	: CYdFormView(CFormExamInfraRed::IDD)
	, m_timeExam(COleDateTime::GetCurrentTime())
	, m_strReceiveStuno(_T(""))
{
	m_bOpenCom = FALSE;
	m_bThreadQuit = TRUE;
	for(int i = 0; i < cMaxComCount;i++)
	{
		m_hThreadDecode[i] = NULL;
		m_hThreadReadData[i] = NULL;
	}
	m_bNotSaveExam = FALSE;
}
CFormExamInfraRed::~CFormExamInfraRed()
{
	CListAutoClean<CYDObjectRef> clr(m_lstClear);
	CListAutoClean<CYDInfraRedAppCom> clr2(m_lstYDAppCom);
}

void CFormExamInfraRed::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STU, m_lstUnit);
	DDX_Control(pDX, IDC_COMBO_EXAM_ROOM, m_cmbExamRoom);
	DDX_Control(pDX, IDC_COMBO_EXAM_PAPER, m_cmbExamPaper);
	DDX_Control(pDX, IDC_COMBO_EXAM_SUBJECT, m_cmbExamSubject);

	DDX_Control(pDX, IDC_COMBO_DEV_COM, m_cmbDevCom);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END, m_timeExam);
	DDX_Text(pDX, IDC_EDIT_RECEIVE_STUNO, m_strReceiveStuno);
}

BEGIN_MESSAGE_MAP(CFormExamInfraRed, CYdFormView)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_EXAM_SUBJECT, &CFormExamInfraRed::OnCbnSelchangeComboExamSubject)
	ON_CBN_SELCHANGE(IDC_COMBO_EXAM_ROOM, &CFormExamInfraRed::OnCbnSelchangeComboExamRoom)
	ON_BN_CLICKED(IDC_BUTTON_OPENPORT, &CFormExamInfraRed::OnBnClickedButtonOpenport)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_STU, &CFormExamInfraRed::OnNMDblclkListStu)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CFormExamInfraRed::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_QUERYMARK, &CFormExamInfraRed::OnBnClickedButtonQuerymark)
	ON_BN_CLICKED(IDC_BUTTON_PAPER_ANS, &CFormExamInfraRed::OnBnClickedButtonPaperAns)
	ON_CBN_SELCHANGE(IDC_COMBO_DEV_COM, &CFormExamInfraRed::OnCbnSelchangeComboDevCom)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_NEW_PAPER, &CFormExamInfraRed::OnBnClickedButtonNewPaper)
END_MESSAGE_MAP()


// CFormExamInfraRed 消息处理程序

void CFormExamInfraRed::OnInitialUpdate()
{
	CYdFormView::OnInitialUpdate();

	// TODO: 在此添加额外的初始化代码

	
	DWORD dwStyle=GetWindowLong(m_lstUnit.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_lstUnit.GetSafeHwnd(),GWL_STYLE,dwStyle);  

	m_lstUnit.SetExtendedStyle(LVS_EX_GRIDLINES);
	::SendMessage(m_lstUnit.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_lstUnit.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
	//m_stulist.SetExtendedStyle(dwStyle);

	::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );

	m_lstUnit.InsertColumn(CONST_COL_STUID,_T("学号"),LVCFMT_LEFT,80);
	m_lstUnit.InsertColumn(CONST_COL_STUNAME,_T("姓名"),LVCFMT_LEFT,80);
	m_lstUnit.InsertColumn(CONST_COL_CLASS,_T("班级"),LVCFMT_LEFT,80);
	m_lstUnit.InsertColumn(CONST_COL_DEVICEID,_T("设备号"),LVCFMT_LEFT,80);
	m_lstUnit.InsertColumn(CONST_COL_RECEIVE_STATE,_T("考试状态"),LVCFMT_LEFT,80);
	m_lstUnit.InsertColumn(CONST_COL_ANSWER_MARK,_T("分数"),LVCFMT_LEFT,10000);
	
	m_bOpenCom = FALSE;
	for(int i = 0; i < cMaxComCount;i++)
	{
		m_hThreadDecode[i] = NULL;
		m_hThreadReadData[i] = NULL;
	}
	m_recv = _T("");
	SetTimer(1,200,NULL);

	//设置Edit的字体，变大一点
	CWnd       *pEdit       =       GetDlgItem(IDC_EDIT_RECEIVE_STUNO); 
	CFont       *pfont       =       pEdit-> GetFont(); 
	LOGFONT       logfont; 
	pfont-> GetLogFont(&logfont); 
	logfont.lfHeight       =       -40; 
	m_EditFont.CreateFontIndirect(&logfont); 
	pEdit-> SetFont(&m_EditFont); 


	HRESULT hr = E_FAIL;
	hr = InitInfo();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	return ;  
}




void CFormExamInfraRed::OnSize(UINT nType, int cx, int cy)
{
	CYdFormView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	
}




DWORD WINAPI ThreadDecode(LPVOID lpParam)
{
	CFormExamInfraRed *pForm = (CFormExamInfraRed *)lpParam;
	ASSERT(pForm);
	//list中的最后一个是当前线程中的pAppCom
	ASSERT(pForm->m_lstYDAppCom.size() > 0);
	std::list<CYDInfraRedAppCom*>::const_reverse_iterator itr = pForm->m_lstYDAppCom.rbegin();
	CYDInfraRedAppCom* pAppCom = (*itr);
	ASSERT(pAppCom);
	HRESULT hr = E_FAIL;
	while(!pForm->m_bThreadQuit)
	{
		hr = pForm->DecodData(pAppCom);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return hr;
		}
		Sleep(500);
	}

	return 0;
}

DWORD WINAPI ThreadReadData(LPVOID lpParam)
{
	CFormExamInfraRed *pForm = (CFormExamInfraRed *)lpParam;
	ASSERT(pForm);
	//list中的最后一个是当前线程中的pAppCom
	ASSERT(pForm->m_lstYDAppCom.size() > 0);
	std::list<CYDInfraRedAppCom*>::const_reverse_iterator itr = pForm->m_lstYDAppCom.rbegin();
	CYDInfraRedAppCom* pAppCom =  (*itr);
	ASSERT(pAppCom);
	HRESULT hr = E_FAIL;
	while(!pForm->m_bThreadQuit)
	{
		hr = pForm->ReadData(pAppCom);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return hr;
		}
		Sleep(500);
	}

	return 0;
}





BOOL CFormExamInfraRed::CheckMac(CString strMac)
{
	BOOL ret = FALSE;

	return ret;
}

void CFormExamInfraRed::ReflashList(int iIndex)
{
	//int iTempcount = 0;
	//CString strTemp = _T("");
	////strTemp.Format(_T("%d %d %d %d"), iIndex, iIndex, iIndex, iIndex);
	//for(int i = 0; i < m_devlist.GetSize(); i++)
	//{
	//	if (m_devlist[i].m_index == iIndex)
	//	{
	//		m_devlist[i].m_count++;
	//		CString strTemp1 = _T("");
	//		strTemp1.Format(_T("%d %d %d %d "), iIndex, iIndex, iIndex, iIndex);
	//		m_devlist[i].m_str += strTemp1;
	//		iTempcount = m_devlist[i].m_count;
	//		strTemp = m_devlist[i].m_str;
	//		break;
	//	}
	//}

	//for (int i = 0; i < m_stulist.GetItemCount(); i++)
	//{
	//	CString strIndex = m_stulist.GetItemText(i, 0);
	//	int iListIndex = _ttoi(strIndex);
	//	if (iListIndex == iIndex)
	//	{
	//		CString temp = _T("");
	//		temp.Format(_T("%d"), iTempcount);
	//		m_stulist.SetItemText(i,1,temp);
	//		m_stulist.SetItemText(i,2,strTemp);
	//		m_stulist.EnsureVisible(i,TRUE);
	//		m_stulist.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
	//		//break;
	//	}
	//	else
	//	{
	//		m_stulist.SetItemState(i,0,LVIS_SELECTED);
	//	}
	//}
}




HRESULT CFormExamInfraRed::InitInfo()
{
	HRESULT hr = E_FAIL;
	//考试科目，考场
	hr = InitExamSubject();
	if(FAILED(hr))
	{
		return hr;
	}
	hr = InitExamClassRoom();
	if(FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT CFormExamInfraRed::InitExamClassRoom()//初始化考场
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


HRESULT CFormExamInfraRed::InitExamSubject()//初始化考试科目
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



void CFormExamInfraRed::OnCbnSelchangeComboExamSubject()
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
}

void CFormExamInfraRed::OnCbnSelchangeComboExamRoom()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	int iSel = m_cmbExamRoom.GetCurSel();
	if(iSel == -1)
	{
		return ;
	}
	m_lstUnit.DeleteAllItems();
	//得到考场的学生信息
	CYDExamAddress* pExamAddress = (CYDExamAddress*)m_cmbExamRoom.GetItemData(iSel);
	ASSERT(pExamAddress);
	std::list<CYDObjectRef*> lstExamUnit;
	//CListAutoClean<CYDObjectRef> clr(lstExamUnit);
	hr = pExamAddress->GetAllUnits(&lstExamUnit);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = lstExamUnit.begin();
		itr != lstExamUnit.end();++itr)
	{
		hr = InsertUnit(*itr);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return;
		}
		m_lstClear.push_back(*itr);
	}
	return ;
}

HRESULT CFormExamInfraRed::InsertUnit(CYDObjectRef* _pUnit)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pUnit);
	int iCount = m_lstUnit.GetItemCount();
	CYDEAddrUnit* pUnit = (CYDEAddrUnit*)_pUnit;
	CYdStudentRef* pStuRef = NULL;
	hr = pUnit->GetStudent(pStuRef);
	if(FAILED(hr))
	{
		return hr;
	}
	CPtrAutoClean<CYdStudentRef> clr(pStuRef);
	CString strStuID;//学号
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

	CYDDevice* pDevice = NULL;
	hr = pUnit->GetDevice(pDevice);
	if(FAILED(hr))
	{
		return hr;
	}
	CPtrAutoClean<CYDDevice> clr2(pDevice);
	CString strStuEvid;//设备号
	hr = pDevice->GetPropVal(FIELD_YDDEVICE_ID,strStuEvid);
	if(FAILED(hr))
	{
		return hr;
	}
	int index = m_lstUnit.InsertItem(m_lstUnit.GetItemCount(),strStuID);
	m_lstUnit.SetItemText(index,CONST_COL_STUNAME,strStuName);
	m_lstUnit.SetItemText(index,CONST_COL_CLASS,strClassName);
	m_lstUnit.SetItemText(index,CONST_COL_DEVICEID,strStuEvid);
	m_lstUnit.SetItemData(index,DWORD_PTR(_pUnit));
	return S_OK;
}



void CFormExamInfraRed::OnBnClickedButtonOpenport()
{
	// TODO: Add your control notification handler code here
	m_cmbDevCom.ResetContent();
	CSearchDevCom SearchDevCom;
	std::list<CString> lstCom;
	HRESULT hr = E_FAIL;
	hr = SearchDevCom.SearchDev(lstCom);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	for(std::list<CString>::const_iterator itr = lstCom.begin();
		itr != lstCom.end();++itr)
	{
		m_cmbDevCom.InsertString(m_cmbDevCom.GetCount(),(*itr));
	}
	if(m_cmbDevCom.GetCount() >0)
	{
		m_cmbDevCom.SetCurSel(0);
		hr = OpenTeacherCom();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
	}
}

HRESULT CFormExamInfraRed::Close()
{
	HRESULT hr = E_FAIL;
	if(m_bNotSaveExam)
	{
		//给出提示
		if(AfxMessageBox(_T("当前有提取到答案信息，但未保存到数据库，是否继续退出？"),MB_YESNO|MB_ICONINFORMATION) != IDYES)
		{
			return S_FALSE;
		}
	}
	CWaitCursor wait;
	hr = CloseThread();
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CFormExamInfraRed::CloseThread()
{
	m_bThreadQuit = TRUE;
	for(int i = 0;  i < cMaxComCount;i++)
	{
		if(m_hThreadReadData[i])
		{
			WaitForSingleObject(m_hThreadReadData[i],15000);
			m_hThreadReadData[i] = NULL;
		}
		if(m_hThreadDecode[i])
		{
			WaitForSingleObject(m_hThreadDecode[i],15000);
			m_hThreadDecode[i] = NULL;
		}
	}
	return S_OK;
}

HRESULT CFormExamInfraRed::DecodData(CYDInfraRedAppCom* _pAppCom)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pAppCom);
	std::list<std::pair<CString,CString> > lstMacAnswer;
	hr = _pAppCom->DecodData(lstMacAnswer);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<std::pair<CString,CString> >::const_iterator itr = lstMacAnswer.begin();
		itr != lstMacAnswer.end();++itr)
	{
		CString strMac = (*itr).first;
		CString strAnswer = (*itr).second;
		hr = UpdateListByMac(strMac,strAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CFormExamInfraRed::ReadData(CYDInfraRedAppCom* _pAppCom)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pAppCom);
	if(!m_bOpenCom)
	{
		return S_FALSE;
	}
	hr = _pAppCom->ReadData();
	if(FAILED(hr))
	{
		return hr;
	}
	
	return S_OK;
}

HRESULT CFormExamInfraRed::UpdateListByMac(CString _strMac,CString _strAnswer)
{
	HRESULT hr = E_FAIL;
	for(int i = 0; i < m_lstUnit.GetItemCount();i++)
	{	
		CYDEAddrUnit* pUint = (CYDEAddrUnit*)m_lstUnit.GetItemData(i);
		ASSERT(pUint);
		CString strItemMac;
		hr= pUint->GetDeviceMac(strItemMac);
		if(FAILED(hr))
		{
			return hr;
		}
		if(strItemMac.CompareNoCase(_strMac) != 0)
		{
			continue;
		}
		//更新数据,修改答案，计算分数
		std::map<CYDEAddrUnit*, CYDStuMark*>::iterator itr = m_mapUnitToMark.find(pUint);
		CYDStuMark* pStuMark = NULL;
		if (itr == m_mapUnitToMark.end())
		{
			hr = CreateStuMark(pUint,pStuMark);
			if(FAILED(hr))
			{
				return hr;
			}
		}
		else
		{
			pStuMark = (*itr).second;
		}
		ASSERT(pStuMark);
		//学生答案
		CComVariant valAnswer;
		CDataHandler::StringToVariant(_strAnswer,VT_BSTR,&valAnswer);
		hr = pStuMark->SetPropVal(FIELD_YDSTUMARK_ANSWER,&valAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
		double dbMark = 0;//成绩
		hr = pStuMark->CalMarkByString(&dbMark);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valMark(dbMark);
		hr = pStuMark->SetPropVal(FIELD_YDSTUMARK_MARK,&valMark);
		if(FAILED(hr))
		{
			return hr;
		}
		CString strMark = CDataHandler::VariantToString(valMark);
		m_lstUnit.SetItemText(i,CONST_COL_RECEIVE_STATE,_T("已收卷"));
		m_lstUnit.SetItemText(i,CONST_COL_ANSWER_MARK,strMark);
		//设置这一行的颜色
		for(int j = 0; j < m_lstUnit.GetHeaderCtrl().GetItemCount();j++)
		{
			m_lstUnit.SetItemColor(i,j,RGB(255,0,0),RGB(0,0,255));
		}
		m_bNotSaveExam = TRUE;
		CYdStudentRef* pStuRef = NULL;
		hr = pUint->GetStudent(pStuRef);
		if(FAILED(hr))
		{
			return hr;
		}
		ASSERT(pStuRef);
		CPtrAutoClean<CYdStudentRef> clrStu(pStuRef);
		//hr = pStuRef->GetPropVal(FIELD_YDSTU_ID,m_strReceiveStuno);
		hr = pStuRef->GetPropVal(FIELD_YDSTU_NAME,m_strReceiveStuno);
		
		if(FAILED(hr))
		{
			return hr;
		}
		GetDlgItem(IDC_EDIT_RECEIVE_STUNO)->SetWindowText(m_strReceiveStuno);
		m_lstUnit.SetItemState(i,   LVIS_SELECTED,   LVIS_SELECTED); 
		ASSERT(m_lstUnit.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED);
		Beep(1000, 100); //发个系统声音，代表收卷
		break;
	}
	return S_OK;
}

HRESULT CFormExamInfraRed::CreateStuMark(CYDEAddrUnit* _pUnit,CYDStuMark*& _pStuMark)
{
	HRESULT hr = E_FAIL;
	ASSERT(_pUnit);
	ASSERT(_pStuMark == NULL);
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	ASSERT(pGExamStruct->m_pMainWnd);
	CDatabaseEx* pDB = (CDatabaseEx*)pGExamStruct->m_pMainWnd->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	_pStuMark = new CYDStuMark(pDB);
	m_lstClear.push_back(_pStuMark);
	CYdStudentRef* pStuRef = NULL;
	hr = _pUnit->GetStudent(pStuRef);
	if(FAILED(hr))
	{
		return hr;
	}
	CPtrAutoClean<CYdStudentRef> clr1(pStuRef);
	OBJID idStu = ID_EMPTY;
	hr = pStuRef->GetID(&idStu);
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valIdStu(idStu);
	hr = _pStuMark->SetPropVal(FIELD_YDSTUMARK_STUID,&valIdStu);
	if(FAILED(hr))
	{
		return hr;
	}
	CYDDevice* pDev = NULL;
	hr = _pUnit->GetDevice(pDev);
	if(FAILED(hr))
	{
		return hr;
	}
	CPtrAutoClean<CYDDevice> clrDev(pDev);
	OBJID idDev = ID_EMPTY;
	hr = pDev->GetID(&idDev);
	CComVariant valIDDev(idDev);
	hr = _pStuMark->SetPropVal(FIELD_YDSTUMARK_DEVICEID,&valIDDev);
	if(FAILED(hr))
	{
		return hr;
	}
	//准考证号
	CComVariant valExamID;
	hr = _pUnit->GetPropVal(FIELD_YDEXAMADDRUNIT_EXAMID,&valExamID);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = _pStuMark->SetPropVal(FIELD_YDSTUMARK_EXAMID,&valExamID);
	if(FAILED(hr))
	{
		return hr;
	}
	//试卷ID
	CYDObjectRef* pPaper = (CYDObjectRef*)m_cmbExamPaper.GetItemData(m_cmbExamPaper.GetCurSel());
	ASSERT(pPaper);
	OBJID idPaper = ID_EMPTY;
	hr = pPaper->GetID(&idPaper);
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valPaperID(idPaper);
	hr = _pStuMark->SetPropVal(FIELD_YDSTUMARK_PAPERID,&valPaperID);
	if(FAILED(hr))
	{
		return hr;
	}
	//科目id
	CYDObjectRef* pSubject = (CYDObjectRef*)m_cmbExamSubject.GetItemData(m_cmbExamSubject.GetCurSel());
	ASSERT(pSubject);
	OBJID idSubject = ID_EMPTY;
	hr = pSubject->GetID(&idSubject);
	if(FAILED(hr))
	{
		return hr;
	}
	CComVariant valSubjectID(idSubject);
	hr = _pStuMark->SetPropVal(FIELD_YDSTUMARK_SUBJECTID,&valSubjectID);
	if(FAILED(hr))
	{
		return hr;
	}

	//考试时间
	CComVariant valExamTime(m_timeExam);
	hr = _pStuMark->SetPropVal(FIELD_YDSTUMARK_EXAMDATE,&valExamTime);
	if(FAILED(hr))
	{
		return hr;
	}
	m_mapUnitToMark.insert(std::map<CYDEAddrUnit*, CYDStuMark*>::value_type(_pUnit,_pStuMark));
	return S_OK;
}

HRESULT CFormExamInfraRed::SaveListAnswer()
{
	HRESULT hr = E_FAIL;
	for(std::map<CYDEAddrUnit*, CYDStuMark*>::const_iterator itr = m_mapUnitToMark.begin();
		itr != m_mapUnitToMark.end();++itr)
	{
		CYDStuMark* pStuMark = (*itr).second;
		hr = pStuMark->Save();
		if(FAILED(hr))
		{
			return hr;
		}
	}
	return S_OK;
}
void CFormExamInfraRed::OnNMDblclkListStu(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem <0)
		return;
	HRESULT hr = E_FAIL;
	CYDEAddrUnit* pUnit = (CYDEAddrUnit*)m_lstUnit.GetItemData(pNMListView->iItem );
	ASSERT(pUnit);
	std::map<CYDEAddrUnit*, CYDStuMark*>::iterator itr = m_mapUnitToMark.find(pUnit);
	if (itr == m_mapUnitToMark.end())
	{
		//当前还没有收卷
		return ;
	}
	CYDStuMark* pStuMark = itr->second;
	ASSERT(pStuMark);
	CString strAnswer;
	hr = pStuMark->GetPropVal(FIELD_YDSTUMARK_ANSWER,strAnswer);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	CYDSubjectRef* pSubject = (CYDSubjectRef*)m_cmbExamSubject.GetItemData(m_cmbExamSubject.GetCurSel());
	ASSERT(pSubject);
	CYDPaper* pPaper = (CYDPaper*)m_cmbExamPaper.GetItemData(m_cmbExamPaper.GetCurSel());
	ASSERT(pPaper);
	CDlgResDetail dlg;
	dlg.m_ResultType = FROM_INRARED_EXAM;
	dlg.m_pUnit = pUnit;
	dlg.m_pSubject = pSubject;
	dlg.m_pPaper = pPaper;
	dlg.m_strAnswer = strAnswer;
	//dlg.m_pStudev = pStudev;
	dlg.DoModal();
}


void CFormExamInfraRed::OnBnClickedButtonSave()
{
	// TODO: Add your control notification handler code here
	if(!m_bNotSaveExam)
	{
		AfxMessageBox(_T("当前还没有提取到答案信息，不能保存到数据库！"));
		return;
	}
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	ASSERT(pGExamStruct);
	ASSERT(pGExamStruct->m_pMainWnd);
	CDatabaseEx* pDB = (CDatabaseEx*)pGExamStruct->m_pMainWnd->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CDBTransactionRef trans(pDB, TRUE);
	hr = SaveListAnswer();
	if (FAILED(hr))
	{
		trans.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	hr = trans.Commit();
	if (FAILED(hr))
	{
		trans.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	m_bNotSaveExam = FALSE;
	AfxMessageBox(_T("保存成功！"));
}

void CFormExamInfraRed::OnBnClickedButtonQuerymark()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	hr = SetGStructData();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
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


void CFormExamInfraRed::OnBnClickedButtonPaperAns()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	
	hr = SetGStructData();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
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

HRESULT CFormExamInfraRed::SetGStructData()
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	CYDObjectRef* pPaper = (CYDObjectRef*)m_cmbExamPaper.GetItemData(m_cmbExamPaper.GetCurSel());
	ASSERT(pPaper);
	OBJID idPaper = 0;
	hr = pPaper->GetID(&idPaper);
	if(FAILED(hr))
	{
		return hr;
	}
	pGExamStruct->m_InfraredStruct.m_idPaper = idPaper;
	pGExamStruct->m_InfraredStruct.m_mapUnitToMark.clear();
	for(std::map<CYDEAddrUnit*, CYDStuMark*>::const_iterator itr = m_mapUnitToMark.begin();
		itr != m_mapUnitToMark.end();++itr)
	{
		CYDEAddrUnit* pUnit = (*itr).first;
		CYDStuMark* pStuMark = (*itr).second;
		pGExamStruct->m_InfraredStruct.m_mapUnitToMark.insert(std::map<CYDEAddrUnit*, CYDStuMark*>::value_type(pUnit,pStuMark));
	}
	pGExamStruct->m_TeacherType = GFROM_INFRARED;
	return S_OK;
}

HRESULT CFormExamInfraRed::OpenTeacherCom()
{
	HRESULT hr = E_FAIL;
	//要验证科目，试卷及考场是否已经选择
	UpdateData();
	if(m_cmbExamSubject.GetCurSel() == -1)
	{
		AfxMessageBox(_T("请选择考试科目！"));
		return S_FALSE;
	}
	if(m_cmbExamPaper.GetCurSel() == -1)
	{
		AfxMessageBox(_T("请选择考试试卷！"));
		return S_FALSE;
	}
	if(m_cmbExamRoom.GetCurSel() == -1)
	{
		AfxMessageBox(_T("请选择考试考场！"));
		return S_FALSE;
	}
	CWaitCursor wait;
	hr = CloseThread();
	if(FAILED(hr))
	{
		return hr;
	}
	{
		CListAutoClean<CYDInfraRedAppCom> clr2(m_lstYDAppCom);
	}
	for(int i = 0 ;i < m_cmbDevCom.GetCount();i++)
	{
		CString strPort;
		m_cmbDevCom.GetLBText(i,strPort);
		if (strPort.GetLength() > 4 && 
			strPort.Find(_T("\\\\.\\")) == -1) 
		{
			strPort = _T("\\\\.\\") + strPort;
		}

		CYDInfraRedAppCom* pYDAppCom = new CYDInfraRedAppCom(strPort);
		hr = pYDAppCom->OpenCom(cInfraRedRate);//红外的波特率是2400
		if(FAILED(hr))
		{
			return hr;
		}
		m_lstYDAppCom.push_back(pYDAppCom);
		m_bThreadQuit = FALSE;
		if (m_hThreadDecode[i] == NULL)
		{
			DWORD dwThreadId2;
			m_hThreadDecode[i] = CreateThread(NULL, 0, ThreadDecode, this, 0, &dwThreadId2);	
		}

		if (m_hThreadReadData[i] == NULL)
		{
			DWORD dwThreadId;
			m_hThreadReadData[i] = CreateThread(NULL, 0, ThreadReadData, this, 0, &dwThreadId);	
		}
		Sleep(100);//稍微等一下，让线程可以将新创建的pYDAppCom值存入到线程临时变量中
	}
	
	m_bOpenCom = TRUE;
	if ( !m_bOpenCom  )
	{
		AfxMessageBox(_T("请打开串口！"));
		return S_FALSE;
	}
	
	
	Sleep(100);
	//将其它按钮变灰
	GetDlgItem(IDC_COMBO_EXAM_SUBJECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_EXAM_PAPER)->EnableWindow(FALSE);
	GetDlgItem(IDC_DATETIMEPICKER_END)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_EXAM_ROOM)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OPENPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_DEV_COM)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_QUERYMARK)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_PAPER_ANS)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_NEW_PAPER)->EnableWindow(FALSE);
	AfxMessageBox(_T("打开串口成功！"));
#ifdef DEBUG
	GetDlgItem(IDC_COMBO_DEV_COM)->EnableWindow(TRUE);
#endif
	return S_OK;
}

void CFormExamInfraRed::OnCbnSelchangeComboDevCom()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	
}

BOOL CFormExamInfraRed::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	m_EditFont.DeleteObject();
	return CYdFormView::DestroyWindow();
}

HBRUSH CFormExamInfraRed::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
// 	if(nCtlColor == CTLCOLOR_EDIT)
// 	{
// 		COLORREF  cf=RGB(255,0,0);
// 		cf= SetTextColor(pDC->m_hDC,RGB(255,0,0));
// 		SetBkColor   (pDC->m_hDC,RGB(0,0,255))   ; 
// 	}
	HBRUSH hbr = CYdFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	//设置Edit的字体颜色
	CWnd       *pEdit       =       GetDlgItem(IDC_EDIT_RECEIVE_STUNO); 
	ASSERT(pEdit);
	if(pWnd->m_hWnd == pEdit->m_hWnd)
	{
		COLORREF  cf=RGB(255,0,0);
		cf= SetTextColor(pDC->m_hDC,RGB(255,0,0));
		SetBkColor   (pDC->m_hDC,RGB(255,255,255))   ;
	}
	
  
	//return (LONG)hBrush;

	return hbr;
}

void CFormExamInfraRed::OnBnClickedButtonNewPaper()
{
	CDlgPaper dlg;
	dlg.DoModal();
}
