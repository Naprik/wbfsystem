#include "stdafx.h"
#include "hardctrl.h"
#include "FormMain.h" 

IMPLEMENT_DYNCREATE(CFormMain, CDialog)

CFormMain::CFormMain(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormMain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormMain)
	//}}AFX_DATA_INIT
}
void CFormMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormMain)
	DDX_Control(pDX, IDC_GROUP, m_group);
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CFormMain, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormMain)
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFormMain::OnInitDialog() 
{
	m_bmpBackground.LoadBitmap(IDB_MAIN2);
	cdxCSizingDialog::OnInitDialog();	
	m_exam.Create(CFormExm::IDD,this);
	m_department.Create(CFormDepartment::IDD,this);
	m_stdanswer.Create(CFormAnswer::IDD,this);
	m_result.Create(CFormResult::IDD,this);
	m_link.Create(CFormlLink::IDD,this);
	m_stuinfo.Create(CFormStuinfo::IDD,this);
	m_subject.Create(CFormSubject::IDD,this);
	m_addr.Create(CFormExamaddr::IDD,this);
	m_classresult.Create(CFormClassResult::IDD,this);
	m_teacherdev.Create(CFormTeacherDev::IDD,this);
	m_stuinfoset.Create(CFormStuinfoSet::IDD,this);
	m_paper.Create(CFormPaper::IDD,this);
	m_pcoffroll.Create(CFormPCoffRoll::IDD,this);
	m_pcoffrolling.Create(CFormPCoffRolling::IDD,this);
	m_fm.Create(CFormFM::IDD,this);
	m_import.Create(CFormImport::IDD,this);
	m_exammode.Create(CFormExamMode::IDD,this);
	m_examset.Create(CFormExamSet::IDD,this);
	m_listen.Create(CFormListen::IDD,this);
	m_image.Create(CFormImage::IDD,this);
	m_examblue.Create(CFormExamBlue::IDD,this);
	m_examinfrar.Create(CFormExamInfrar::IDD,this);
	m_examusb.Create(CFormExamUSB::IDD,this);
	m_examtime.Create(CFormExamTime::IDD,this);
	m_sturesquery.Create(CFormStuResultQuery::IDD,this);
	m_classquery.Create(CFormClassResultQuey::IDD,this);
	m_paperanaly.Create(CFormPaperAnaly::IDD,this);
	m_imageread.Create(CFromImageread::IDD,this);
	m_examinfoback.Create(CFormExamInfoBack::IDD,this);
	m_creatfile.Create(CFormCreatFile::IDD,this);
	m_examdetail.Create(CFormExamDetail::IDD,this);
	m_examimport.Create(CFormExamImport::IDD,this);
	m_examintatoff.Create(CFormExamInrarOff::IDD,this);
	m_examrollinganswer.Create(CFormExamRollingAnswer::IDD,this);
	AddSzControl(m_exam,mdResize,mdResize);
	AddSzControl(m_department,mdResize,mdResize);
	AddSzControl(m_stdanswer,mdResize,mdResize);
	AddSzControl(m_result,mdResize,mdResize);
	AddSzControl(m_link,mdResize,mdResize);
	AddSzControl(m_stuinfo,mdResize,mdResize);
	AddSzControl(m_addr,mdResize,mdResize);
	AddSzControl(m_classresult,mdResize,mdResize);
	AddSzControl(m_teacherdev,mdResize,mdResize);
	AddSzControl(m_stuinfoset,mdResize,mdResize);
	AddSzControl(m_paper,mdResize,mdResize);
	AddSzControl(m_pcoffroll,mdResize,mdResize);
	AddSzControl(m_pcoffrolling,mdResize,mdResize);
	AddSzControl(m_fm,mdResize,mdResize);
	AddSzControl(m_import,mdResize,mdResize);
	AddSzControl(m_exammode,mdResize,mdResize);
	AddSzControl(m_examset,mdResize,mdResize);
	AddSzControl(m_listen,mdResize,mdResize);
	AddSzControl(m_image,mdResize,mdResize);
	AddSzControl(m_examblue,mdResize,mdResize);
	AddSzControl(m_examinfrar,mdResize,mdResize);
	AddSzControl(m_examusb,mdResize,mdResize);
	AddSzControl(m_examtime,mdResize,mdResize);
	AddSzControl(m_sturesquery,mdResize,mdResize);
	AddSzControl(m_classquery,mdResize,mdResize);
	AddSzControl(m_paperanaly,mdResize,mdResize);
	AddSzControl(m_imageread,mdResize,mdResize);
	AddSzControl(m_examinfoback,mdResize,mdResize);
	AddSzControl(m_creatfile,mdResize,mdResize);
	AddSzControl(m_examdetail,mdResize,mdResize);
	AddSzControl(m_examimport,mdResize,mdResize);
	AddSzControl(m_examintatoff,mdResize,mdResize);
	AddSzControl(m_examrollinganswer,mdResize,mdResize);

    ShowDlg(0);
	return TRUE;  
}

void CFormMain::OnSize(UINT nType, int cx, int cy) 
{
	cdxCSizingDialog::OnSize(nType, cx, cy);
	if ( ::IsWindow(m_group.m_hWnd) )
	{	
		CRect lpRect,rect1;
		GetWindowRect(&lpRect);	
		rect1.left = 0;
		rect1.top  = 0;
		rect1.right= lpRect.right - lpRect.left;
		rect1.bottom=lpRect.bottom - lpRect.top ;
		m_group.MoveWindow(rect1);
		m_group.GetWindowRect(&lpRect);
		rect1.left = 0;
		rect1.top  = 0;
		rect1.right= lpRect.right - lpRect.left;
		rect1.bottom=lpRect.bottom - lpRect.top ;
		if ( ::IsWindow(m_exam.m_hWnd))
	   		m_exam.MoveWindow(rect1);
		if ( ::IsWindow(m_department.m_hWnd))
			m_department.MoveWindow(rect1);
		if ( ::IsWindow(m_stdanswer.m_hWnd))
			m_stdanswer.MoveWindow(rect1);
		if ( ::IsWindow(m_result.m_hWnd))
			m_result.MoveWindow(rect1);
		if ( ::IsWindow(m_link.m_hWnd))
			m_link.MoveWindow(rect1);
		if ( ::IsWindow(m_stuinfo.m_hWnd))
			m_stuinfo.MoveWindow(rect1);
		if ( ::IsWindow(m_subject.m_hWnd))
			m_subject.MoveWindow(rect1);
		if ( ::IsWindow(m_addr.m_hWnd))
			m_addr.MoveWindow(rect1);
		if ( ::IsWindow(m_classresult.m_hWnd))
			m_classresult.MoveWindow(rect1);
		if ( ::IsWindow(m_teacherdev.m_hWnd))
			m_teacherdev.MoveWindow(rect1);
		if ( ::IsWindow(m_stuinfoset.m_hWnd))
			m_stuinfoset.MoveWindow(rect1);
		if ( ::IsWindow(m_paper.m_hWnd))
			m_paper.MoveWindow(rect1);
		if ( ::IsWindow(m_pcoffroll.m_hWnd))
			m_pcoffroll.MoveWindow(rect1);
		if ( ::IsWindow(m_pcoffrolling.m_hWnd))
			m_pcoffrolling.MoveWindow(rect1);
		if ( ::IsWindow(m_fm.m_hWnd))
			m_fm.MoveWindow(rect1);
		if ( ::IsWindow(m_import.m_hWnd))
			m_import.MoveWindow(rect1);
		if ( ::IsWindow(m_exammode.m_hWnd))
			m_exammode.MoveWindow(rect1);
		if ( ::IsWindow(m_examset.m_hWnd))
			m_examset.MoveWindow(rect1);
		if ( ::IsWindow(m_listen.m_hWnd))
			m_listen.MoveWindow(rect1);
		if ( ::IsWindow(m_image.m_hWnd))
			m_image.MoveWindow(rect1);
		if ( ::IsWindow(m_examblue.m_hWnd))
			m_examblue.MoveWindow(rect1);
		if ( ::IsWindow(m_examinfrar.m_hWnd))
			m_examinfrar.MoveWindow(rect1);
		if ( ::IsWindow(m_examusb.m_hWnd))
			m_examusb.MoveWindow(rect1);
		if ( ::IsWindow(m_examtime.m_hWnd))
			m_examtime.MoveWindow(rect1);
		if ( ::IsWindow(m_sturesquery.m_hWnd))
			m_sturesquery.MoveWindow(rect1);
		if ( ::IsWindow(m_classquery.m_hWnd))
			m_classquery.MoveWindow(rect1);
		if ( ::IsWindow(m_paperanaly.m_hWnd))
			m_paperanaly.MoveWindow(rect1);
		if ( ::IsWindow(m_imageread.m_hWnd))
			m_imageread.MoveWindow(rect1);
		if ( ::IsWindow(m_examinfoback.m_hWnd))
			m_examinfoback.MoveWindow(rect1);
		if ( ::IsWindow(m_creatfile.m_hWnd))
			m_creatfile.MoveWindow(rect1);
		if ( ::IsWindow(m_examdetail.m_hWnd))
			m_examdetail.MoveWindow(rect1);
		if ( ::IsWindow(m_examimport.m_hWnd))
			m_examimport.MoveWindow(rect1);
		if ( ::IsWindow(m_examintatoff.m_hWnd))
			m_examintatoff.MoveWindow(rect1);
		if ( ::IsWindow(m_examrollinganswer.m_hWnd))
			m_examrollinganswer.MoveWindow(rect1);
	}	
}
void CFormMain::ShowDlg(DWORD id)
{
	m_exam.ShowWindow(SW_HIDE);
	m_department.ShowWindow(SW_HIDE);
	m_stdanswer.ShowWindow(SW_HIDE);
	m_result.ShowWindow(SW_HIDE);
	m_link.ShowWindow(SW_HIDE);
	m_stuinfo.ShowWindow(SW_HIDE);
	m_subject.ShowWindow(SW_HIDE);	
	m_addr.ShowWindow(SW_HIDE);
	m_classresult.ShowWindow(SW_HIDE);
	m_teacherdev.ShowWindow(SW_HIDE);
	m_stuinfoset.ShowWindow(SW_HIDE);
	m_paper.ShowWindow(SW_HIDE);
	m_pcoffroll.ShowWindow(SW_HIDE);
	m_pcoffrolling.ShowWindow(SW_HIDE);
	m_fm.ShowWindow(SW_HIDE);
	m_import.ShowWindow(SW_HIDE);
	m_exammode.ShowWindow(SW_HIDE);
	m_examset.ShowWindow(SW_HIDE);
	m_listen.ShowWindow(SW_HIDE);
	m_image.ShowWindow(SW_HIDE);
	m_examblue.ShowWindow(SW_HIDE);
	m_examinfrar.ShowWindow(SW_HIDE);
	m_examusb.ShowWindow(SW_HIDE);
	m_examtime.ShowWindow(SW_HIDE);
	m_sturesquery.ShowWindow(SW_HIDE);
	m_classquery.ShowWindow(SW_HIDE);
	m_paperanaly.ShowWindow(SW_HIDE);
	m_imageread.ShowWindow(SW_HIDE);
	m_examinfoback.ShowWindow(SW_HIDE);
	m_creatfile.ShowWindow(SW_HIDE);
	m_examdetail.ShowWindow(SW_HIDE);
	m_examimport.ShowWindow(SW_HIDE);
	m_examintatoff.ShowWindow(SW_HIDE);
	m_examrollinganswer.ShowWindow(SW_HIDE);
	if ( id != 0 )
	{
		switch(id)
		{
			case CFormExm::IDD:
				m_exam.ShowWindow(SW_SHOW);
				break;
			case CFormDepartment::IDD:
				m_department.ShowWindow(SW_SHOW);
				break;
			case CFormAnswer::IDD:
				m_stdanswer.ShowWindow(SW_SHOW);
				break;
			case CFormResult::IDD:
				m_result.ShowWindow(SW_SHOW);
				break;
			case CFormlLink::IDD:
				m_link.ShowWindow(SW_SHOW);
				break;
			case CFormStuinfo::IDD:
				m_stuinfo.ShowWindow(SW_SHOW);
				break;
			case CFormSubject::IDD:
				m_subject.ShowWindow(SW_SHOW);
				break;
			case CFormExamaddr::IDD:
				m_addr.ShowWindow(SW_SHOW);
				break;
			case CFormClassResult::IDD:
				m_classresult.ShowWindow(SW_SHOW);
				break;
			case CFormTeacherDev::IDD:
				m_teacherdev.ShowWindow(SW_SHOW);
				break;
			case CFormStuinfoSet::IDD:
				m_stuinfoset.ShowWindow(SW_SHOW);
				break;
			case CFormPaper::IDD:
				m_paper.ShowWindow(SW_SHOW);
				break;
			case CFormPCoffRoll::IDD:
				m_pcoffroll.ShowWindow(SW_SHOW);
				break;
			case CFormPCoffRolling::IDD:
				m_pcoffrolling.ShowWindow(SW_SHOW);
				break;
			case CFormFM::IDD:
				m_fm.ShowWindow(SW_SHOW);
				break;
			case CFormImport::IDD:
				m_import.ShowWindow(SW_SHOW);
				break;
			case CFormExamMode::IDD:
				m_exammode.ShowWindow(SW_SHOW);
				break;
			case CFormExamSet::IDD:
				m_examset.ShowWindow(SW_SHOW);
				break;
			case CFormListen::IDD:
				m_listen.ShowWindow(SW_SHOW);
				break;
			case CFormImage::IDD:
				m_image.ShowWindow(SW_SHOW);
				break;
			case CFormExamBlue::IDD:
				m_examblue.ShowWindow(SW_SHOW);
				break;
			case CFormExamInfrar::IDD:
				m_examinfrar.ShowWindow(SW_SHOW);
				break;
			case CFormExamUSB::IDD:
				m_examusb.ShowWindow(SW_SHOW);
				break;
			case CFormExamTime::IDD:
				m_examtime.ShowWindow(SW_SHOW);
				break;
			case CFormStuResultQuery::IDD:
				m_sturesquery.ShowWindow(SW_SHOW);
				break;
			case CFormClassResultQuey::IDD:
				m_classquery.ShowWindow(SW_SHOW);
				break;
			case CFormPaperAnaly::IDD:
				m_paperanaly.ShowWindow(SW_SHOW);
				break;
			case CFromImageread::IDD:
				m_imageread.ShowWindow(SW_SHOW);
				break;
			case CFormExamInfoBack::IDD:
				m_examinfoback.ShowWindow(SW_SHOW);
				break;
			case CFormCreatFile::IDD:
				m_creatfile.ShowWindow(SW_SHOW);
				break;
			case CFormExamDetail::IDD:
				m_examdetail.ShowWindow(SW_SHOW);
				break;
			case CFormExamImport::IDD:
				m_examimport.ShowWindow(SW_SHOW);
				break;
			case CFormExamInrarOff::IDD:
				m_examintatoff.ShowWindow(SW_SHOW);
				break;
			case CFormExamRollingAnswer::IDD:
				m_examrollinganswer.ShowWindow(SW_SHOW);
				break;
			default:
				break;
		}
	}
	RedrawWindow( );
}

void CFormMain::OnPaint() 
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	CDC dcMem; 
	dcMem.CreateCompatibleDC(&dc); 
	BITMAP bitMap;
	m_bmpBackground.GetBitmap(&bitMap);
	CBitmap *pbmpOld=dcMem.SelectObject(&m_bmpBackground);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitMap.bmWidth,bitMap.bmHeight,SRCCOPY);

}
