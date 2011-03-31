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
	
	m_list.SetHeadings(_T("ѧ��,150;����,100;�༶,100;׼��֤��,100;����ʱ��,150;����ʱ��,150;ѧ����,200"));
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
		AfxMessageBox(_T("û���Ծ���Ϣ�����룡"));
		return S_FALSE;
	}
	if(pGExamStruct->m_pExamDetail->m_pPaper == NULL)
	{
		AfxMessageBox(_T("û���Ծ���Ϣ�����룡"));
		return S_FALSE;
	}
	else
	{
		{
			//�Ծ�����
			CString strPaperName;
			hr = pGExamStruct->m_pExamDetail->m_pPaper->GetPropVal(FIELD_YDPAPER_NAME,strPaperName);
			if(FAILED(hr))
			{
				return hr;
			}
			GetDlgItem(IDC_STATIC3)->SetWindowText(strPaperName);
		}
		{
			//��Ŀ����
			CString strSubjectName;
			hr = pGExamStruct->m_pExamDetail->m_pSubject->GetPropVal(FIELD_YDSUBJECT_NAME,strSubjectName);
			if(FAILED(hr))
			{
				return hr;
			}
			GetDlgItem(IDC_STATIC5)->SetWindowText(strSubjectName);
		}
		{
			//������ַ
			CString strAddress;
			hr = pGExamStruct->m_pExamDetail->m_pObjAddress->GetPropVal(FIELD_YDEXAMADDR_ADDR,strAddress);
			if(FAILED(hr))
			{
				return hr;
			}
			GetDlgItem(IDC_STATIC7)->SetWindowText(strAddress);
		}
		
		{
			//���԰༶
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
			//�����ͽ���ʱ��
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
		AfxMessageBox(_T("û���Ծ���Ϣ�����룡"));
		return ;
	}
	if(pGExamStruct->m_pExamDetail->m_pPaper == NULL)
	{
		AfxMessageBox(_T("û���Ծ���Ϣ�����룡"));
		return ;
	}
	BOOL bOpenTeacher = FALSE;//����Ҫѡ��һ����ʦ��
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
		AfxMessageBox(_T("������ѡ��һ����ʦ��������ȡ�𰸣�"));
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
		AfxMessageBox(_T("��ȡ��ʧ�ܣ�"));
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
					CString strStuId;//ѧ��
					hr = pStuRef->GetPropVal(FIELD_YDSTU_ID,strStuId);
					if(FAILED(hr))
					{
						DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
						return ;
					}
					CString strStuName;//����
					hr = pStuRef->GetPropVal(FIELD_YDSTU_NAME,strStuName);
					if(FAILED(hr))
					{
						DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
						return ;
					}
					//�༶
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

					//׼��֤��
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
		AfxMessageBox(_T("û���Ծ���Ϣ�����룡"));
		return ;
	}
	if(pGExamStruct->m_pExamDetail->m_strXmlFile.IsEmpty())
	{
		AfxMessageBox(_T("û���Ծ���Ϣ�����룡"));
		return ;
	}
	BOOL bOpenTeacher = FALSE;//����Ҫѡ��һ����ʦ��
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
		AfxMessageBox(_T("������ѡ��һ����ʦ��������ȡ�𰸣�"));
		return;
	}
	CInvigilateInfo InvigilateInfo;
	CYDObjectRef* pObjSubject = pGExamStruct->m_pExamDetail->m_pSubject;//���Կ�Ŀ
	ASSERT(pObjSubject);
	CString strSubjectName;
	hr = pObjSubject->GetPropVal(FIELD_YDSUBJECT_NAME,strSubjectName);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CYDObjectRef* pObjPaper = pGExamStruct->m_pExamDetail->m_pPaper;//�����Ծ�
	ASSERT(pObjPaper);
	CComVariant valPaperId;
	hr = pObjPaper->GetPropVal(FIELD_YDRESULT_PAPERID,&valPaperId);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CYDObjectRef* pObjAddress = pGExamStruct->m_pExamDetail->m_pObjAddress;//����
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
	//���ɼ����浽xml
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
		//״̬���³ɹ�
		//�Ѽ࿼��Ϣ���ݲ��뵽���ݿ���
		//���ж�����࿼��Ϣ�������ݿ����Ƿ��Ѿ�����
		VARIANT_BOOL bExist = VARIANT_FALSE;
		hr = InvigilateInfo.IsExist(pDB,&bExist);
		if(FAILED(hr))
		{
			DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
			return ;
		}
		if(bExist)
		{
			if (AfxMessageBox(_T("��ǰ�࿼��Ϣ�Ѿ������ݿ��д��ڣ��Ƿ�������룿"),MB_YESNO|MB_ICONINFORMATION) != IDYES)
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
		//���б������е�ѧ��������ȡ���ĳɼ�д�뵽���ݿ�
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
	AfxMessageBox(_T("����𰸳ɹ���"));
}

