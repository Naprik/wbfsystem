// FormExamRollingAnswer.cpp : implementation file
//

#include "stdafx.h"
#include "FormExamRollingAnswer.h"
#include "../ObjRef\YDObjectRef.h"
#include "../YDExamObjRef\YDExamAddress.h"
#include "../\Base\AutoClean.h"
#include "../\YDExamObjRef\YDEAddrUnit.h"
#include "InvigilateInfo.h"
#include "../Base\DataHandler.h"
#include "../DBBase\DBTransactionRef.h"



IMPLEMENT_DYNCREATE(CFormExamRollingAnswer, CYdFormView)


CFormExamRollingAnswer::CFormExamRollingAnswer()
	: CYdFormView(CFormExamRollingAnswer::IDD)
{
	//{{AFX_DATA_INIT(CFormExamRollingAnswer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormExamRollingAnswer::DoDataExchange(CDataExchange* pDX)
{
	CYdFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormExamRollingAnswer)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormExamRollingAnswer, CYdFormView)
	//{{AFX_MSG_MAP(CFormExamRollingAnswer)
	ON_BN_CLICKED(IDC_FETCH1, OnFetchAnswer)
	ON_BN_CLICKED(IDC_SAVE, OnSaveAnswer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormExamRollingAnswer message handlers

void CFormExamRollingAnswer::OnInitialUpdate() 
{
	CYdFormView::OnInitialUpdate();
	

	DWORD dwStyle=GetWindowLong(m_list.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_list.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_list.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_list.SetExtendedStyle(dwStyle);
	
    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	
	m_list.ClearHeadings();
	
	m_list.SetHeadings(_T("学号,150;姓名,100;班级,100;准考证号,100;开考时间,150;交卷时间,150;学生答案,200"));
	HRESULT hr = E_FAIL;
	hr = SetInfo();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	
	return ;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CFormExamRollingAnswer::SetKind(int kind)
{
	m_kind = kind;
}

HRESULT CFormExamRollingAnswer::SetInfo()
{
	// 	GetDlgItem(IDC_STATIC2)->SetWindowText(m_examdeteil.sSubject);
	// 	GetDlgItem(IDC_STATIC4)->SetWindowText(m_examdeteil.sPapername);
	// 	GetDlgItem(IDC_STATIC6)->SetWindowText(m_examdeteil.sStart);
	// 	GetDlgItem(IDC_STATIC8)->SetWindowText(m_examdeteil.sEnd);
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	if (pGExamStruct->m_pExamDetail == NULL)
	{
		AfxMessageBox(_T("没有试卷信息请载入！"));
		return S_FALSE;
	}
	if(pGExamStruct->m_pExamDetail->m_pPaper == NULL)
	{
		AfxMessageBox(_T("没有试卷信息请载入！"));
		return S_FALSE;
	}
	else
	{
		{
			//试卷名称
			CString strPaperName;
			hr = pGExamStruct->m_pExamDetail->m_pPaper->GetPropVal(FIELD_YDPAPER_NAME,strPaperName);
			if(FAILED(hr))
			{
				return hr;
			}
			GetDlgItem(IDC_STATIC3)->SetWindowText(strPaperName);
		}
		{
			//科目名称
			CString strSubjectName;
			hr = pGExamStruct->m_pExamDetail->m_pSubject->GetPropVal(FIELD_YDSUBJECT_NAME,strSubjectName);
			if(FAILED(hr))
			{
				return hr;
			}
			GetDlgItem(IDC_STATIC5)->SetWindowText(strSubjectName);
		}
		{
			//考场地址
			CString strAddress;
			hr = pGExamStruct->m_pExamDetail->m_pObjAddress->GetPropVal(FIELD_YDEXAMADDR_ADDR,strAddress);
			if(FAILED(hr))
			{
				return hr;
			}
			GetDlgItem(IDC_STATIC7)->SetWindowText(strAddress);
		}
		
		{
			//考试班级
			CString sClass = _T("");
			for(std::list<CYDObjectRef*>::const_iterator itr = pGExamStruct->m_pExamDetail->m_lstClass.begin();
				itr != pGExamStruct->m_pExamDetail->m_lstClass.end();++itr)
			{
				CString strItrClass;
				hr = (*itr)->GetPropVal(FIELD_YDCLASS_NAME,strItrClass);
				if(FAILED(hr))
				{
					return hr;
				}
				sClass += strItrClass;
				sClass += _T(";");
			}
			GetDlgItem(IDC_STATIC9)->SetWindowText(sClass);

		}
		{
			//开考和交卷时间
			CString strStart = CDataHandler::DateTimeToStr(pGExamStruct->m_pExamDetail->m_timeStart,2);
			GetDlgItem(IDC_STATIC13)->SetWindowText(strStart);
			CString strEnd =CDataHandler::DateTimeToStr(pGExamStruct->m_pExamDetail->m_timeEnd,2);
			GetDlgItem(IDC_STATIC15)->SetWindowText(strEnd);
		}

	}
	return S_OK;
}

void CFormExamRollingAnswer::OnFetchAnswer() 
{
	m_list.DeleteAllItems();
	HRESULT hr = E_FAIL;
	BOOL bSuccess = FALSE;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct);
	if (pGExamStruct->m_pExamDetail == NULL)
	{
		AfxMessageBox(_T("没有试卷信息请载入！"));
		return ;
	}
	if(pGExamStruct->m_pExamDetail->m_pPaper == NULL)
	{
		AfxMessageBox(_T("没有试卷信息请载入！"));
		return ;
	}
	BOOL bOpenTeacher = FALSE;//至少要选择一个教师机
	for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
		itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
	{
		BOOL bOpen = FALSE;
		hr = (*itr)->GetState(bOpen);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		if(bOpen)
		{
			bOpenTeacher = TRUE;
			break;
		}
	}
	if(!bOpenTeacher)
	{
		AfxMessageBox(_T("请至少选择一个教师机进行提取答案！"));
		return;
	}
	for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
		itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
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
			continue;
		}
		CString strAnswer;
		hr = (*itr)->FetchAnswer(strAnswer);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
 		bSuccess = TRUE;
 	}
	if(!bSuccess)
	{
		AfxMessageBox(_T("提取答案失败！"));
		return ;
	}
	for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
		itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
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
			continue;
		}
		ASSERT(pGExamStruct->m_pExamDetail->m_pObjAddress);
		CYDExamAddress* pAddress = (CYDExamAddress*)pGExamStruct->m_pExamDetail->m_pObjAddress;
		ASSERT(pAddress);
		std::list<CYDObjectRef*> lstUnit;
		CListAutoClean<CYDObjectRef> clr(lstUnit);
		hr = pAddress->GetAllUnits(&lstUnit);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		for(std::list<CStudev*>::const_iterator itrStudev = (*itr)->m_lstStuDev.begin();
			itrStudev != (*itr)->m_lstStuDev.end();++itrStudev)
		{
			for(std::list<CYDObjectRef*>::const_iterator itrUnit = lstUnit.begin();
				itrUnit != lstUnit.end();++itrUnit)
			{
				CYDEAddrUnit* pUnit = (CYDEAddrUnit*)(*itrUnit);
				ASSERT(pUnit);
				CYdStudentRef* pStuRef = NULL;
				hr = pUnit->GetStudent(pStuRef);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				ASSERT(pStuRef);
				CPtrAutoClean<CYdStudentRef> clr1(pStuRef);
				CYDDevice* pDevice = NULL;
				hr = pUnit->GetDevice(pDevice);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				ASSERT(pDevice);
				CPtrAutoClean<CYDDevice> clr2(pDevice);
				CString strDeviceMac;
				hr = pDevice->GetPropVal(FIELD_YDDEVICE_MAC,strDeviceMac);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				CString strItrMac;
				hr = (*itrStudev)->GetMac(strItrMac);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				if(strDeviceMac.CompareNoCase(strItrMac) == 0)
				{
					CString strStuId;//学号
					hr = pStuRef->GetPropVal(FIELD_YDSTU_ID,strStuId);
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
					//班级
					CString strClassName;
					std::list<CYDObjectRef*> lstClass;
					CListAutoClean<CYDObjectRef> clr1(lstClass);
					hr = pStuRef->GetClass(&lstClass);
					if(FAILED(hr))
					{
						DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
						return ;
					}
					for(std::list<CYDObjectRef*>::const_iterator itr = lstClass.begin();
						itr != lstClass.end();++itr)
					{
						hr = (*itr)->GetPropVal(FIELD_YDCLASS_NAME,strClassName);
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
					CString strStart;
					COleDateTime tItrStart;
					hr = (*itrStudev)->GetStart(tItrStart);
					if(FAILED(hr))
					{
						DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
						return ;
					}
					tItrStart.ParseDateTime(strStart);
					COleDateTime tItrEnd;
					hr = (*itrStudev)->GetEnd(tItrEnd);
					if(FAILED(hr))
					{
						DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
						return ;
					}
					CString strEnd;
					tItrEnd.ParseDateTime(strEnd);
					CString strItrAnswer;
					hr = (*itrStudev)->GetAnswer(strItrAnswer);
					if(FAILED(hr))
					{
						DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
						return ;
					}
					m_list.AddItem(0,
						strStuId,
						strStuName,
						strClassName,
						strExamID,
						strStart,
						strEnd,
						strItrAnswer
						);
					break;
				}
			}

		
		}
	}
	
}




void CFormExamRollingAnswer::OnSaveAnswer() 
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
	if (pGExamStruct->m_pExamDetail == NULL)
	{
		AfxMessageBox(_T("没有试卷信息请载入！"));
		return ;
	}
	if(pGExamStruct->m_pExamDetail->m_strXmlFile.IsEmpty())
	{
		AfxMessageBox(_T("没有试卷信息请载入！"));
		return ;
	}
	BOOL bOpenTeacher = FALSE;//至少要选择一个教师机
	for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
		itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
	{
		BOOL bOpen = FALSE;
		hr = (*itr)->GetState(bOpen);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		if(bOpen)
		{
			bOpenTeacher = TRUE;
			break;
		}
	}
	if(!bOpenTeacher)
	{
		AfxMessageBox(_T("请至少选择一个教师机进行提取答案！"));
		return;
	}
	CInvigilateInfo InvigilateInfo;
	CYDObjectRef* pObjSubject = pGExamStruct->m_pExamDetail->m_pSubject;//考试科目
	ASSERT(pObjSubject);
	CString strSubjectName;
	hr = pObjSubject->GetPropVal(FIELD_YDSUBJECT_NAME,strSubjectName);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CYDObjectRef* pObjPaper = pGExamStruct->m_pExamDetail->m_pPaper;//考试试卷
	ASSERT(pObjPaper);
	CComVariant valPaperId;
	hr = pObjPaper->GetPropVal(FIELD_YDRESULT_PAPERID,&valPaperId);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CYDObjectRef* pObjAddress = pGExamStruct->m_pExamDetail->m_pObjAddress;//考场
	ASSERT(pObjAddress);
	CString strAddress;
	hr = pObjAddress->GetPropVal(FIELD_YDEXAMADDR_ADDR,strAddress);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	int nPaperID = CDataHandler::VariantToLong(valPaperId);
	pGExamStruct->m_pExamDetail->m_bIsRollingDone = TRUE;
	//将成绩保存到xml
	ASSERT(!pGExamStruct->m_pExamDetail->m_strXmlFile.IsEmpty());
	hr = InvigilateInfo.Save(CComBSTR(pGExamStruct->m_pExamDetail->m_strXmlFile));
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	{
		//状态更新成功
		//把监考信息数据插入到数据库中
		//先判断这个监考信息表在数据库中是否已经存在
		VARIANT_BOOL bExist = VARIANT_FALSE;
		hr = InvigilateInfo.IsExist(pDB,&bExist);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		if(bExist)
		{
			if (AfxMessageBox(_T("当前监考信息已经在数据库中存在，是否继续导入？"),MB_YESNO|MB_ICONINFORMATION) != IDYES)
			{
				return ;
			}

		}
	}
	
	CDBTransactionRef TransRef(pDB,TRUE);
	hr = InvigilateInfo.SaveInvigilateDB(pDB);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	ASSERT(pGExamStruct->m_pExamDetail->m_pObjAddress);
	CYDExamAddress* pAddress = (CYDExamAddress*)pGExamStruct->m_pExamDetail->m_pObjAddress;
	ASSERT(pAddress);
	std::list<CYDObjectRef*> lstUnit;
	CListAutoClean<CYDObjectRef> clr(lstUnit);
	hr = pAddress->GetAllUnits(&lstUnit);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	for(std::list<CYDTeacherAppCom*>::const_iterator itr = pGExamStruct->m_lstTacherAppCom.begin();
		itr != pGExamStruct->m_lstTacherAppCom.end();++itr)
	{
		BOOL bOpen = FALSE;
		hr = (*itr)->GetState(bOpen);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		//将列表上所有的学生机用新取到的成绩写入到数据库
		for(std::list<CStudev*>::const_iterator itrDev = (*itr)->m_lstStuDev.begin();
			itrDev != (*itr)->m_lstStuDev.end();++itrDev)
		{
			for(std::list<CYDObjectRef*>::const_iterator itrUnit = lstUnit.begin();
				itrUnit != lstUnit.end();++itrUnit)
			{
				CYDEAddrUnit* pUnit = (CYDEAddrUnit*)(*itrUnit);
				ASSERT(pUnit);
				CYdStudentRef* pStuRef = NULL;
				hr = pUnit->GetStudent(pStuRef);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				ASSERT(pStuRef);
				CPtrAutoClean<CYdStudentRef> clr1(pStuRef);
				CYDDevice* pDevice = NULL;
				hr = pUnit->GetDevice(pDevice);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				ASSERT(pDevice);
				CPtrAutoClean<CYDDevice> clr2(pDevice);
				CString strDeviceMac;
				hr = pDevice->GetPropVal(FIELD_YDDEVICE_MAC,strDeviceMac);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				CString strItrMac;
				hr = (*itrDev)->GetMac(strItrMac);
				if(FAILED(hr))
				{
					DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
					return ;
				}
				if(strDeviceMac.CompareNoCase(strItrMac) == 0)
				{
					CString strItrAnswer;
					hr = (*itrDev)->GetAnswer(strItrAnswer);
					if(FAILED(hr))
					{
						DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
						return ;
					}
					hr = InvigilateInfo.SaveAnswer(pDB,pUnit,
							 					pGExamStruct->m_pExamDetail->m_pPaper,
												pGExamStruct->m_pExamDetail->m_pSubject,
												strItrAnswer);
					if(FAILED(hr))
					{
						DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
						return ;
					}
					break;
				}
			}
		}
		hr = TransRef.Commit();
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
	}
	AfxMessageBox(_T("保存答案成功！"));
}

