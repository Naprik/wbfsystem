// DlgExamBlueDetail.cpp : implementation file
//

#include "stdafx.h"
#include "DlgExamBlueDetail.h"
#include "../Base\DataHandler.h"
#include "../Base\AutoClean.h"
#include "../YdCom\ExamQuestionSegment.h"


// CDlgExamBlueDetail dialog
const int CONSHOWTIME = 1000;

IMPLEMENT_DYNAMIC(CDlgExamBlueDetail, CBCGPDialog)

CDlgExamBlueDetail::CDlgExamBlueDetail(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgExamBlueDetail::IDD, pParent)
	, m_bListenOne(FALSE)
	, m_bListenTwo(FALSE)
	, m_bListenThree(FALSE)
	, m_bProgramTwo(FALSE)
	, m_bProgramThree(FALSE)
	, m_bProgramOne(TRUE)
	, m_timeListenOneStart(COleDateTime::GetCurrentTime())
	, m_timeListenOneEnd(COleDateTime::GetCurrentTime())
	, m_timeListenTwoStart(COleDateTime::GetCurrentTime())
	, m_timeListenTwoEnd(COleDateTime::GetCurrentTime())
	, m_timeListenThreeStart(COleDateTime::GetCurrentTime())
	, m_timeListenThreeEnd(COleDateTime::GetCurrentTime())
	, m_strFmFrequency(_T(""))
	, m_strStartNO1(_T(""))
	, m_strEndNO1(_T(""))
	, m_strStartNO2(_T(""))
	, m_strEndNO2(_T(""))
	, m_strStartNO3(_T(""))
	, m_strEndNO3(_T(""))
{

}

CDlgExamBlueDetail::~CDlgExamBlueDetail()
{
}

void CDlgExamBlueDetail::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_LISTEN_ONE, m_bListenOne);
	DDX_Check(pDX, IDC_CHECK_LISTEN_TWO, m_bListenTwo);
	DDX_Check(pDX, IDC_CHECK_LISTEN_THREE, m_bListenThree);
	DDX_Check(pDX, IDC_CHECK_PROGRAM_TWO, m_bProgramTwo);
	DDX_Check(pDX, IDC_CHECK_PROGRAM_THREE, m_bProgramThree);
	DDX_Check(pDX, IDC_CHECK_PROGRAM_ONE, m_bProgramOne);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_LISTEN_ONE_START, m_timeListenOneStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_LISTEN_ONE_END, m_timeListenOneEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_LISTEN_TWO_START, m_timeListenTwoStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_LISTEN_TWO_END, m_timeListenTwoEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_LISTEN_THREE_START, m_timeListenThreeStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_LISTEN_THREE_END, m_timeListenThreeEnd);
	DDX_Text(pDX, IDC_FM_FREQUENCY, m_strFmFrequency);
	DDX_Text(pDX, IDC_START_NO1, m_strStartNO1);
	DDX_Text(pDX, IDC_END_NO1, m_strEndNO1);
	DDX_Control(pDX, IDC_CHOOSE_ANSWER1, m_cmbChooseAnswer1);
	DDX_Control(pDX, IDC_CHOOSE_TYPE1, m_cmbChooseType1);
	DDX_Text(pDX, IDC_START_NO2, m_strStartNO2);
	DDX_Text(pDX, IDC_END_NO2, m_strEndNO2);
	DDX_Control(pDX, IDC_CHOOSE_ANSWER2, m_cmbChooseAnswer2);
	DDX_Control(pDX, IDC_CHOOSE_TYPE2, m_cmbChooseType2);
	DDX_Text(pDX, IDC_START_NO3, m_strStartNO3);
	DDX_Text(pDX, IDC_END_NO3, m_strEndNO3);
	DDX_Control(pDX, IDC_CHOOSE_ANSWER3, m_cmbChooseAnswer3);
	DDX_Control(pDX, IDC_CHOOSE_TYPE3, m_cmbChooseType3);
}


BEGIN_MESSAGE_MAP(CDlgExamBlueDetail, CBCGPDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_LISTEN_ONE, &CDlgExamBlueDetail::OnBnClickedCheckListenOne)
	ON_BN_CLICKED(IDC_CHECK_LISTEN_TWO, &CDlgExamBlueDetail::OnBnClickedCheckListenTwo)
	ON_BN_CLICKED(IDC_CHECK_LISTEN_THREE, &CDlgExamBlueDetail::OnBnClickedCheckListenThree)
	ON_BN_CLICKED(IDC_CHECK_PROGRAM_TWO, &CDlgExamBlueDetail::OnBnClickedCheckProgramTwo)
	ON_BN_CLICKED(IDC_CHECK_PROGRAM_THREE, &CDlgExamBlueDetail::OnBnClickedCheckProgramThree)
	ON_BN_CLICKED(IDC_CHECK_PROGRAM_ONE, &CDlgExamBlueDetail::OnBnClickedCheckProgramOne)
	ON_BN_CLICKED(IDOK, &CDlgExamBlueDetail::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgExamBlueDetail message handlers

void CDlgExamBlueDetail::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == CONSHOWTIME)
	{
		//UpdateData();
		ShowCurTime();
		//UpdateData(FALSE);
	}
	CBCGPDialog::OnTimer(nIDEvent);
}

HRESULT CDlgExamBlueDetail::ShowCurTime()
{
	HRESULT hr = E_FAIL;
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	CString strTime;
	strTime.Format(_T("%02d:%02d:%02d"),sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
	GetDlgItem(IDC_EDIT_SYSTIME)->SetWindowText(strTime);
	return S_OK;
}
BOOL CDlgExamBlueDetail::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	HRESULT hr = E_FAIL;
	hr = ShowCurTime();
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	hr = UpdateGStruct(FALSE);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	UpdateData(FALSE);
	OnBnClickedCheckProgramTwo();
	OnBnClickedCheckProgramThree();
	SetTimer(CONSHOWTIME,1000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgExamBlueDetail::OnBnClickedCheckListenOne()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_DATETIMEPICKER_LISTEN_ONE_START)->EnableWindow(m_bListenOne);
	GetDlgItem(IDC_DATETIMEPICKER_LISTEN_ONE_END)->EnableWindow(m_bListenOne);
}

void CDlgExamBlueDetail::OnBnClickedCheckListenTwo()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_DATETIMEPICKER_LISTEN_TWO_START)->EnableWindow(m_bListenTwo);
	GetDlgItem(IDC_DATETIMEPICKER_LISTEN_TWO_END)->EnableWindow(m_bListenTwo);
}

void CDlgExamBlueDetail::OnBnClickedCheckListenThree()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_DATETIMEPICKER_LISTEN_THREE_START)->EnableWindow(m_bListenThree);
	GetDlgItem(IDC_DATETIMEPICKER_LISTEN_THREE_END)->EnableWindow(m_bListenThree);
}

void CDlgExamBlueDetail::OnBnClickedCheckProgramTwo()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_START_NO2)->EnableWindow(m_bProgramTwo);
	GetDlgItem(IDC_END_NO2)->EnableWindow(m_bProgramTwo);
	GetDlgItem(IDC_CHOOSE_ANSWER2)->EnableWindow(m_bProgramTwo);
	GetDlgItem(IDC_CHOOSE_TYPE2)->EnableWindow(m_bProgramTwo);
}

void CDlgExamBlueDetail::OnBnClickedCheckProgramThree()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_START_NO3)->EnableWindow(m_bProgramThree);
	GetDlgItem(IDC_END_NO3)->EnableWindow(m_bProgramThree);
	GetDlgItem(IDC_CHOOSE_ANSWER3)->EnableWindow(m_bProgramThree);
	GetDlgItem(IDC_CHOOSE_TYPE3)->EnableWindow(m_bProgramThree);
}

void CDlgExamBlueDetail::OnBnClickedCheckProgramOne()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_START_NO1)->EnableWindow(m_bProgramOne);
	GetDlgItem(IDC_END_NO1)->EnableWindow(m_bProgramOne);
	GetDlgItem(IDC_CHOOSE_ANSWER1)->EnableWindow(m_bProgramOne);
	GetDlgItem(IDC_CHOOSE_TYPE1)->EnableWindow(m_bProgramOne);
}

void CDlgExamBlueDetail::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(!ValidateRollCallData())
	{
		return;
	}
	HRESULT hr = E_FAIL;
	hr = UpdateGStruct(TRUE);
	if(FAILED(hr))
	{
		DISPLAY_YDERROR(hr,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	return CDialog::OnOK();
}

BOOL CDlgExamBlueDetail::ValidateRollCallData()
{
	
	BOOL bHasListen = FALSE;//三个听力则只要有一个选择，则bHasListen为True
	if(m_bListenOne)
	{
		bHasListen = TRUE;
		if(m_timeListenOneStart >= m_timeListenOneEnd)
		{
			AfxMessageBox(_T("听力1开始时间应晚于结束时间!"));
			return FALSE;
		}
	}
	if(m_bListenTwo)
	{
		bHasListen = TRUE;
		if(m_timeListenTwoStart >= m_timeListenTwoEnd)
		{
			AfxMessageBox(_T("听力2开始时间应晚于结束时间!"));
			return FALSE;
		}
	}
	if(m_bListenThree)
	{
		bHasListen = TRUE;
		if(m_timeListenThreeStart >= m_timeListenThreeEnd)
		{
			AfxMessageBox(_T("听力1开始时间应晚于结束时间!"));
			return FALSE;
		}
	}
	if(bHasListen)
	{
		if(m_strFmFrequency.IsEmpty())
		{
			AfxMessageBox(_T("请输入收音机的频率!"));
			return FALSE;
		}
		if(!ValiateFMChannel(m_strFmFrequency))
		{
			AfxMessageBox(_T("收音机频率格式不正确！"));
			return FALSE;
		}
	}
	int iLast = -1;
	if(m_bProgramOne)
	{
		int iStart = CDataHandler::StringToInt(m_strStartNO1);
		if(iStart <= 0)
		{
			AfxMessageBox(_T("段落一开始题号格式不正确！"));
			return FALSE;
		}
		int iEnd = CDataHandler::StringToInt(m_strEndNO1);
		if(iEnd <= 0)
		{
			AfxMessageBox(_T("段落一结束题号格式不正确！"));
			return FALSE;
		}
		if(iStart >= iEnd)
		{
			AfxMessageBox(_T("段落一开始题号不能大于结束题号！"));
			return FALSE;
		}
		if(m_cmbChooseAnswer1.GetCurSel() == -1)
		{
			AfxMessageBox(_T("段落一没有选择答案选项！"));
			return FALSE;
		}
		if(m_cmbChooseType1.GetCurSel() == -1)
		{
			AfxMessageBox(_T("段落一没有选择选项设置！"));
			return FALSE;
		}
		iLast = iEnd;
	}
	if(m_bProgramTwo)
	{
		int iStart = CDataHandler::StringToInt(m_strStartNO2);
		if(iStart <= 0)
		{
			AfxMessageBox(_T("段落二开始题号格式不正确！"));
			return FALSE;
		}
		if(iStart <= iLast)
		{
			AfxMessageBox(_T("段落二开始题号不能小于段落一结束题号！"));
			return FALSE;
		}
		int iEnd = CDataHandler::StringToInt(m_strEndNO2);
		if(iEnd <= 0)
		{
			AfxMessageBox(_T("段落二结束题号格式不正确！"));
			return FALSE;
		}
		if(iStart >= iEnd)
		{
			AfxMessageBox(_T("段落二开始题号不能大于结束题号！"));
			return FALSE;
		}
		if(m_cmbChooseAnswer2.GetCurSel() == -1)
		{
			AfxMessageBox(_T("段落二没有选择答案选项！"));
			return FALSE;
		}
		if(m_cmbChooseType2.GetCurSel() == -1)
		{
			AfxMessageBox(_T("段落二没有选择选项设置！"));
			return FALSE;
		}
		iLast = iEnd;
	}
	if(m_bProgramThree)
	{
		int iStart = CDataHandler::StringToInt(m_strStartNO3);
		if(iStart <= 0)
		{
			AfxMessageBox(_T("段落三开始题号格式不正确！"));
			return FALSE;
		}
		if(iStart <= iLast)
		{
			AfxMessageBox(_T("段落三开始题号不能小于段落二结束题号！"));
			return FALSE;
		}
		int iEnd = CDataHandler::StringToInt(m_strEndNO3);
		if(iEnd <= 0)
		{
			AfxMessageBox(_T("段落三结束题号格式不正确！"));
			return FALSE;
		}
		if(iStart >= iEnd)
		{
			AfxMessageBox(_T("段落三开始题号不能大于结束题号！"));
			return FALSE;
		}
		if(m_cmbChooseAnswer3.GetCurSel() == -1)
		{
			AfxMessageBox(_T("段落三没有选择答案选项！"));
			return FALSE;
		}
		if(m_cmbChooseType3.GetCurSel() == -1)
		{
			AfxMessageBox(_T("段落三没有选择选项设置！"));
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CDlgExamBlueDetail::ValiateFMChannel(CString strFmFrequency)
{
	strFmFrequency.Remove(_T('.'));

	int nCount = strFmFrequency.GetLength(); // 获得字符个数
	if(nCount > 4)
	{
		return FALSE;
	}
	for ( int i = 0; i < nCount; i ++ )
	{
		if ( 0 == isdigit( strFmFrequency.GetAt(i) ) ) // 不是数字就置标志位
		{
			return FALSE;
		}
	}

	return TRUE;

}

HRESULT CDlgExamBlueDetail::UpdateGStruct(BOOL _bUpdate)
{
	HRESULT hr = E_FAIL;
	CGExamStruct* pGExamStruct = NULL;
	hr = CStaticYdComApp::Instance()->GetGExamStruct(pGExamStruct);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pGExamStruct);
	if(_bUpdate)
	{
		//当前数据--->pGExamStruct
		{
			//听力
			pGExamStruct->m_pExamDetail->m_strChanal = m_strFmFrequency;
			pGExamStruct->m_pExamDetail->m_bListenPart1 = m_bListenOne;
			if(m_bListenOne)
			{
				pGExamStruct->m_pExamDetail->m_timeStart1 = m_timeListenOneStart;
				pGExamStruct->m_pExamDetail->m_timeEnd1 = m_timeListenOneEnd;
			}
		
			pGExamStruct->m_pExamDetail->m_bListenPart2 = m_bListenTwo;
			if(m_bListenTwo)
			{
				pGExamStruct->m_pExamDetail->m_timeStart2 = m_timeListenTwoStart;
				pGExamStruct->m_pExamDetail->m_timeEnd2 = m_timeListenTwoEnd ;
			}
			pGExamStruct->m_pExamDetail->m_bListenPart3 = m_bListenThree;
			if(m_bListenThree)
			{
				pGExamStruct->m_pExamDetail->m_timeStart3 = m_timeListenThreeStart;
				pGExamStruct->m_pExamDetail->m_timeEnd3 = m_timeListenThreeEnd ;
			}
		}
		{
			//试题
			{
				CListAutoClean<CExamQuestionSegment> clr7(pGExamStruct->m_pExamDetail->m_lstQSeg);
			}
			if(m_bProgramOne)
			{
				CExamQuestionSegment* pQSeg = new CExamQuestionSegment();
				pQSeg->m_iStart = _ttoi(m_strStartNO1);
				pQSeg->m_iEnd = _ttoi(m_strEndNO1);
				pQSeg->m_iOptionNum = SelToChoiceAnswer(m_cmbChooseAnswer1.GetCurSel());
				pQSeg->m_iMultiType = m_cmbChooseType1.GetCurSel();
				pGExamStruct->m_pExamDetail->m_lstQSeg.push_back(pQSeg);
			}
			if(m_bProgramTwo)
			{
				CExamQuestionSegment* pQSeg = new CExamQuestionSegment();
				pQSeg->m_iStart = _ttoi(m_strStartNO2);
				pQSeg->m_iEnd = _ttoi(m_strEndNO2);
				pQSeg->m_iOptionNum = SelToChoiceAnswer(m_cmbChooseAnswer2.GetCurSel());
				pQSeg->m_iMultiType = m_cmbChooseType2.GetCurSel();
				pGExamStruct->m_pExamDetail->m_lstQSeg.push_back(pQSeg);
			}
			if(m_bProgramThree)
			{
				CExamQuestionSegment* pQSeg = new CExamQuestionSegment();
				pQSeg->m_iStart = _ttoi(m_strStartNO3);
				pQSeg->m_iEnd = _ttoi(m_strEndNO3);
				pQSeg->m_iOptionNum = SelToChoiceAnswer(m_cmbChooseAnswer3.GetCurSel());
				pQSeg->m_iMultiType = m_cmbChooseType3.GetCurSel();
				pGExamStruct->m_pExamDetail->m_lstQSeg.push_back(pQSeg);
			}
		}
	}
	else
	{
		//pGExamStruct--->当前数据
		{
			//听力
			m_strFmFrequency = pGExamStruct->m_pExamDetail->m_strChanal ;
			m_bListenOne = pGExamStruct->m_pExamDetail->m_bListenPart1 ;
			GetDlgItem(IDC_DATETIMEPICKER_LISTEN_ONE_START)->EnableWindow(m_bListenOne);
			GetDlgItem(IDC_DATETIMEPICKER_LISTEN_ONE_END)->EnableWindow(m_bListenOne);
			m_timeListenOneStart = pGExamStruct->m_pExamDetail->m_timeStart1;
			m_timeListenOneEnd = pGExamStruct->m_pExamDetail->m_timeEnd1;
			m_bListenTwo = pGExamStruct->m_pExamDetail->m_bListenPart2 ;
			GetDlgItem(IDC_DATETIMEPICKER_LISTEN_TWO_START)->EnableWindow(m_bListenTwo);
			GetDlgItem(IDC_DATETIMEPICKER_LISTEN_TWO_END)->EnableWindow(m_bListenTwo);
			m_timeListenTwoStart = pGExamStruct->m_pExamDetail->m_timeStart2;
			m_timeListenTwoEnd = pGExamStruct->m_pExamDetail->m_timeEnd2;
			m_bListenThree = pGExamStruct->m_pExamDetail->m_bListenPart3 ;
			GetDlgItem(IDC_DATETIMEPICKER_LISTEN_THREE_START)->EnableWindow(m_bListenThree);
			GetDlgItem(IDC_DATETIMEPICKER_LISTEN_THREE_END)->EnableWindow(m_bListenThree);
			m_timeListenThreeStart = pGExamStruct->m_pExamDetail->m_timeStart3;
			m_timeListenThreeEnd = pGExamStruct->m_pExamDetail->m_timeEnd3;
		}
		{
			//试题
			ASSERT(pGExamStruct->m_pExamDetail->m_lstQSeg.size() <= 3);
			int index = 0;
			for(std::list<CExamQuestionSegment*>::const_iterator itr = pGExamStruct->m_pExamDetail->m_lstQSeg.begin();
				itr != pGExamStruct->m_pExamDetail->m_lstQSeg.end();++itr,++index)
			{
				if(index == 0)
				{
					m_bProgramOne = TRUE;
					m_strStartNO1.Format(_T("%d"),(*itr)->m_iStart);
					m_strEndNO1.Format(_T("%d"),(*itr)->m_iEnd);
					m_cmbChooseAnswer1.SetCurSel(ChoiceAnswerToSel((*itr)->m_iOptionNum));
					m_cmbChooseType1.SetCurSel((*itr)->m_iMultiType);
				}
				else if(index == 1)
				{
					m_bProgramTwo = TRUE;
					m_strStartNO2.Format(_T("%d"),(*itr)->m_iStart);
					m_strEndNO2.Format(_T("%d"),(*itr)->m_iEnd);
					m_cmbChooseAnswer2.SetCurSel(ChoiceAnswerToSel((*itr)->m_iOptionNum));
					m_cmbChooseType2.SetCurSel((*itr)->m_iMultiType);
				}
				else if(index == 2)
				{
					m_bProgramThree = TRUE;
					m_strStartNO3.Format(_T("%d"),(*itr)->m_iStart);
					m_strEndNO3.Format(_T("%d"),(*itr)->m_iEnd);
					m_cmbChooseAnswer3.SetCurSel(ChoiceAnswerToSel((*itr)->m_iOptionNum));
					m_cmbChooseType3.SetCurSel((*itr)->m_iMultiType);
				}
				else
				{
					ASSERT(FALSE);
				}
			}
		}

	}
	return S_OK;
}

int CDlgExamBlueDetail::ChoiceAnswerToSel(int _iChoiceAnswer)
{
	if(_iChoiceAnswer == 2)//0010
	{
		//AB
		return 0;
	}
	else if(_iChoiceAnswer == 3)//0011
	{
		//ABC
		return 1;
	}
	else if(_iChoiceAnswer == 4) //0100
	{
		//ABCD
		return 2;
	}
	else if(_iChoiceAnswer == 5)//0101
	{
		//ABCDE
		return 3;
	}
	else if(_iChoiceAnswer == 15)//1111
	{
		//AB...O
		return 4;
	}
	else
	{
		ASSERT(FALSE);
	}
	return -1;
}

int CDlgExamBlueDetail::SelToChoiceAnswer(int _iSel)
{
	if(_iSel == 0)//0010
	{
		//AB
		return 2;
	}
	else if(_iSel == 1)//0011
	{
		//ABC
		return 3;
		m_cmbChooseAnswer3.SetCurSel(1);
	}
	else if(_iSel == 2) //0100
	{
		//ABCD
		return 4;
	}
	else if(_iSel == 3)//0101
	{
		//ABCDE
		return 5;
	}
	else if(_iSel == 4)//1111
	{
		//AB...O
		return 15;
	}
	else
	{
		ASSERT(FALSE);
	}
	return -1;
}
