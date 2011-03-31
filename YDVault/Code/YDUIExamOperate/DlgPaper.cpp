// DlgPaper.cpp : implementation file
//

#include "stdafx.h"
#include "DlgPaper.h"
#include "../Base\AutoClean.h"
#include "../ObjRef\YDObjectRef.h"
#include "../ObjHelper\StaticObjHelper.h"
#include "../ObjRef\YDPaper.h"
#include "DlgPaperEditAnswer.h"
#include "../DBBase\DBTransactionRef.h"
#include "../YDExamObjRef\YDSubjectRef.h"
#include "../ObjRef\YDAnswer.h"


// CDlgPaper dialog

IMPLEMENT_DYNAMIC(CDlgPaper, CDialog)

CDlgPaper::CDlgPaper(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPaper::IDD, pParent)
	, m_strPaperCode(_T(""))
	, m_strPaperName(_T(""))
	, m_uBegin1(0)
	, m_uEnd1(0)
	, m_uCount1(3)
	, m_uBegin2(0)
	, m_uEnd2(0)
	, m_uCount2(3)
	, m_uBegin3(0)
	, m_uEnd3(0)
	, m_uCount3(3)
{
	
}

CDlgPaper::~CDlgPaper()
{
	CListAutoClean<CYDObjectRef> clr(m_lstClr);
	CListAutoClean<CAnswerRecord> clr2(m_lstAnswerRecord);
}

void CDlgPaper::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_IN_SUBJECT, m_cmbInSubject);
	DDX_Text(pDX, IDC_DM_PAPER_CODE, m_strPaperCode);
	DDX_Text(pDX, IDC_DM_PAPER_NAME, m_strPaperName);
	DDX_Text(pDX, IDC_EDIT_BEGIN1, m_uBegin1);
	DDX_Text(pDX, IDC_EDIT_END1, m_uEnd1);
	DDX_Text(pDX, IDC_EDIT_COUNT1, m_uCount1);
	DDX_Text(pDX, IDC_EDIT_BEGIN2, m_uBegin2);
	DDX_Text(pDX, IDC_EDIT_END2, m_uEnd2);
	DDX_Text(pDX, IDC_EDIT_COUNT2, m_uCount2);
	DDX_Text(pDX, IDC_EDIT_BEGIN3, m_uBegin3);
	DDX_Text(pDX, IDC_EDIT_END3, m_uEnd3);
	DDX_Text(pDX, IDC_EDIT_COUNT3, m_uCount3);
	DDX_Control(pDX, IDC_CMB_MULTI1, m_cmbMulti1);
	DDX_Control(pDX, IDC_CMB_MULTI2, m_cmbMulti2);
	DDX_Control(pDX, IDC_CMB_MULTI3, m_cmbMulti3);
}


BEGIN_MESSAGE_MAP(CDlgPaper, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgPaper::OnBnClickedOk)
	ON_BN_CLICKED(ID_BUTTON_EDIT_ANSWER, &CDlgPaper::OnBnClickedButtonEditAnswer)
END_MESSAGE_MAP()


// CDlgPaper message handlers
HRESULT CDlgPaper::InitSubject()
{
	HRESULT hr = E_FAIL;
	m_cmbInSubject.ResetContent();
	std::list<CYDObjectRef*> lstSubject;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	hr = CStaticObjHelper::GetObjRef(DB_YDSUBJECT, pDB, &lstSubject);
	if(FAILED(hr))
	{
		return hr;
	}
	for(std::list<CYDObjectRef*>::const_iterator itr = lstSubject.begin();
		itr != lstSubject.end();++itr)
	{
		CString strName;
		hr = (*itr)->GetPropVal(FIELD_YDSUBJECT_NAME,strName);
		if(FAILED(hr))
		{
			return hr;
		}
		int index = m_cmbInSubject.InsertString(0,strName);
		m_cmbInSubject.SetItemData(index,DWORD_PTR(*itr));
		m_lstClr.push_back(*itr);
	}
	if(lstSubject.size() > 0)
	{
		m_cmbInSubject.SetCurSel(0);
	}
	return S_OK;
}
BOOL CDlgPaper::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	HRESULT hr = E_FAIL;
	hr = InitSubject();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HRESULT CDlgPaper::ValidateData(VARIANT_BOOL* _bValidate)
{
	HRESULT hr = E_FAIL;
	*_bValidate = VARIANT_FALSE;
	if(m_cmbInSubject.GetCurSel() == CB_ERR)
	{
		AfxMessageBox(_T("请选择考试科目！"));
		return S_FALSE;
	}
	if(m_strPaperCode.IsEmpty())
	{
		AfxMessageBox(_T("试卷编号不能为空！"));
		return S_FALSE;
	}
	if(m_strPaperName.IsEmpty())
	{
		AfxMessageBox(_T("试卷名称不能为空！"));
		return S_FALSE;
	}
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	{
		BOOL bExist;
		CYDPaper paperRef(pDB);
		paperRef.IsFieldExist(FIELD_YDPAPER_NAME, m_strPaperName, &bExist);
		if (bExist)
		{
			AfxMessageBox(L"试卷名称已存在，请重新输入！");
			return S_FALSE;
		}
	}

	{
		BOOL bExist;
		CYDPaper paperRef(pDB);
		paperRef.IsFieldExist(FIELD_YDPAPER_CODE, m_strPaperCode, &bExist);
		if (bExist)
		{
			AfxMessageBox(L"试卷编号已存在，请重新输入！");
			return S_FALSE;
		}
	}

	//考试段数的验证
	if(m_uBegin1 != 0 && m_uEnd1 != 0)
	{
		if(m_uBegin1 > m_uEnd1)
		{
			AfxMessageBox(_T("段一的结束数不能低于开始数！"));
			return S_FALSE;
		}
		if(m_uCount1 ==0 || m_uCount1 > 10)
		{
			AfxMessageBox(_T("段一的选项数格式不正确！"));
			return S_FALSE;
		}
		if(m_cmbMulti1.GetCurSel() == CB_ERR)
		{
			AfxMessageBox(_T("请选择段一的多选选项！"));
			return S_FALSE;
		}
	}
	if(m_uBegin2 != 0 && m_uEnd2 != 0)
	{
		if(m_uBegin2 > m_uEnd2)
		{
			AfxMessageBox(_T("段二的结束数不能低于开始数！"));
			return S_FALSE;
		}
		if(m_uCount2 ==0 || m_uCount2 > 10)
		{
			AfxMessageBox(_T("段二的选项数格式不正确！"));
			return S_FALSE;
		}
		if(m_cmbMulti2.GetCurSel() == CB_ERR)
		{
			AfxMessageBox(_T("请选择段二的多选选项！"));
			return S_FALSE;
		}
	}
	if(m_uBegin3 != 0 && m_uEnd3 != 0)
	{
		if(m_uBegin3 > m_uEnd3)
		{
			AfxMessageBox(_T("段三的结束数不能低于开始数！"));
			return S_FALSE;
		}
		if(m_uCount3 ==0 || m_uCount3 > 10)
		{
			AfxMessageBox(_T("段三的选项数格式不正确！"));
			return S_FALSE;
		}
		if(m_cmbMulti3.GetCurSel() == CB_ERR)
		{
			AfxMessageBox(_T("请选择段三的多选选项！"));
			return S_FALSE;
		}
	}
	*_bValidate = VARIANT_TRUE;
	return S_OK;
}
void CDlgPaper::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	UpdateData();
	VARIANT_BOOL bValidate = VARIANT_FALSE;
	hr = ValidateData(&bValidate);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	if(!bValidate)
	{
		return;
	}
	hr = CreateAnswerRecord();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	//将当前试卷的信息插入数据库
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	CYDObjectRef* pPaperRef = new CYDPaper(pDB);
	CPtrAutoClean<CYDObjectRef> clr(pPaperRef);
	CComVariant varId(m_strPaperCode);
	pPaperRef->SetPropVal(FIELD_YDPAPER_CODE, &varId);
	CComVariant varName(m_strPaperName);
	pPaperRef->SetPropVal(FIELD_YDPAPER_NAME, &varName);

	CComVariant valBegin1(m_uBegin1);

	hr = pPaperRef->SetPropVal(FIELD_YDPAPER_BEGIN1,&valBegin1);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CComVariant valEnd1(m_uEnd1);
	hr = pPaperRef->SetPropVal(FIELD_YDPAPER_END1,&valEnd1);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CComVariant valCount1(m_uCount1);
	hr = pPaperRef->SetPropVal(FIELD_YDPAPER_ANSWERCHOICE1,&valCount1);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CComVariant valMuti1(m_cmbMulti1.GetCurSel());
	hr = pPaperRef->SetPropVal(FIELD_YDPAPER_MULTIFLAG1,&valMuti1);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}

	CComVariant valBegin2(m_uBegin2);
	hr = pPaperRef->SetPropVal(FIELD_YDPAPER_BEGIN2,&valBegin2);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CComVariant valEnd2(m_uEnd2);
	hr = pPaperRef->SetPropVal(FIELD_YDPAPER_END2,&valEnd2);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CComVariant valCount2(m_uCount2);
	hr = pPaperRef->SetPropVal(FIELD_YDPAPER_ANSWERCHOICE2,&valCount2);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CComVariant valMuti2(m_cmbMulti2.GetCurSel());
	hr = pPaperRef->SetPropVal(FIELD_YDPAPER_MULTIFLAG2,&valMuti2);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}

	CComVariant valBegin3(m_uBegin3);
	hr = pPaperRef->SetPropVal(FIELD_YDPAPER_BEGIN3,&valBegin3);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CComVariant valEnd3(m_uEnd3);
	hr = pPaperRef->SetPropVal(FIELD_YDPAPER_END3,&valEnd3);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CComVariant valCount3(m_uCount3);
	hr = pPaperRef->SetPropVal(FIELD_YDPAPER_ANSWERCHOICE3,&valCount3);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CComVariant valMuti3(m_cmbMulti3.GetCurSel());
	hr = pPaperRef->SetPropVal(FIELD_YDPAPER_MULTIFLAG3,&valMuti3);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}

	CDBTransactionRef trans(pDB, TRUE);
	hr = pPaperRef->Save();
	if (FAILED(hr))
	{
		trans.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	hr = CreatePaperInSubject(pPaperRef);
	if (FAILED(hr))
	{
		trans.Rollback();
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return;
	}
	hr = CreateAnswer(pPaperRef);
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
	OnOK();
}

HRESULT CDlgPaper::CreatePaperInSubject(CYDObjectRef* _pPaper)
{
	HRESULT hr = E_FAIL;
	int index = m_cmbInSubject.GetCurSel();
	ASSERT(index >= 0);
	CYDSubjectRef* pSubject = (CYDSubjectRef*)m_cmbInSubject.GetItemData(index);
	ASSERT(pSubject);
	hr = pSubject->AddPaper(_pPaper);
	if(FAILED(hr))
	{
		return hr;
	}
	return S_OK;
}
HRESULT CDlgPaper::CreateAnswer(CYDObjectRef* _pPaper)
{
	HRESULT hr = E_FAIL;
	CDatabaseEx* pDB = (CDatabaseEx*)AfxGetMainWnd()->SendMessage(WM_YD_GET_DB);
	ASSERT(pDB);
	for(std::list<CAnswerRecord*>::const_iterator itr = m_lstAnswerRecord.begin();
		itr != m_lstAnswerRecord.end();++itr)
	{
		CYDAnswer* pAnswer = new CYDAnswer(pDB);
		ASSERT(pAnswer);
		CPtrAutoClean<CYDAnswer> clr1(pAnswer);
		CComVariant valNo((*itr)->m_strNO);
		hr = pAnswer->SetPropVal(FIELD_YDANSWER_NO,&valNo);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valAnswer((*itr)->m_strAnswer);
		hr = pAnswer->SetPropVal(FIELD_YDANSWER_ANSWER,&valAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
		CComVariant valMark((*itr)->m_strMark);
		hr = pAnswer->SetPropVal(FIELD_YDANSWER_MARK,&valMark);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pAnswer->Save();
		if(FAILED(hr))
		{
			return hr;
		}
		CYDLinkRef* pLinkRef  = new CYDLinkRef(pDB, DB_PAPERANSWER);
		hr = pLinkRef->PutObjRef(_pPaper, pAnswer);
		if(FAILED(hr))
		{
			return hr;
		}
		hr = pLinkRef->Save();
		if(FAILED(hr))
		{
			return hr;
		}
		CPtrAutoClean<CYDLinkRef> clr2(pLinkRef);
	}
	return S_OK;
}

void CDlgPaper::OnBnClickedButtonEditAnswer()
{
	// TODO: Add your control notification handler code here
	HRESULT hr = E_FAIL;
	UpdateData();
	VARIANT_BOOL bValidate = VARIANT_FALSE;
	hr = ValidateData(&bValidate);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	if(!bValidate)
	{
		return;
	}
	hr = CreateAnswerRecord();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	CDlgPaperEditAnswer dlg(&m_lstAnswerRecord);
	dlg.DoModal();
}

HRESULT CDlgPaper::CreateAnswerRecord()
{
	HRESULT hr = E_FAIL;
	std::list<CAnswerRecord*> lstAnswerRecord;
	hr = CreateAnswerRecordByBeginEnd(m_uBegin1,m_uEnd1,lstAnswerRecord);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = CreateAnswerRecordByBeginEnd(m_uBegin2,m_uEnd2,lstAnswerRecord);
	if(FAILED(hr))
	{
		return hr;
	}
	hr = CreateAnswerRecordByBeginEnd(m_uBegin3,m_uEnd3,lstAnswerRecord);
	if(FAILED(hr))
	{
		return hr;
	}
	{
		CListAutoClean<CAnswerRecord> clr(m_lstAnswerRecord);
	}
	for(std::list<CAnswerRecord*>::const_iterator itr = lstAnswerRecord.begin();
		itr != lstAnswerRecord.end();++itr)
	{
		m_lstAnswerRecord.push_back(*itr);
	}
	lstAnswerRecord.clear();
	return S_OK;
}

HRESULT CDlgPaper::CreateAnswerRecordByBeginEnd(UINT uBegin,UINT uEnd,
									 std::list<CAnswerRecord*>& _lstRecord)
{
	HRESULT hr = E_FAIL;
	if(uBegin != 0 && uEnd != 0)
	{
		for(UINT i = uBegin; i <= uEnd;i++)
		{
			CString strNo;
			strNo.Format(_T("%d"),i);
			CString strAnswer;
			CString strMark = _T("0.00");
			for(std::list<CAnswerRecord*>::const_iterator itr = m_lstAnswerRecord.begin();
				itr != m_lstAnswerRecord.end();++itr)
			{
				if((*itr)->m_strNO.CompareNoCase(strNo) == 0)
				{
					strAnswer = (*itr)->m_strAnswer;
					strMark = (*itr)->m_strMark;
					break;
				}
			}
			CAnswerRecord* pRecord = new CAnswerRecord(strNo,strAnswer,strMark);
			ASSERT(pRecord);
			_lstRecord.push_back(pRecord);
		}
	}
	return S_OK;
}