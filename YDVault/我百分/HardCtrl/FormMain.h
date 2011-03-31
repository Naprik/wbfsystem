#if !defined(AFX_DLGMAIN_H__40C8C595_2397_4B9C_9D30_07A53F1989C6__INCLUDED_)
#define AFX_DLGMAIN_H__40C8C595_2397_4B9C_9D30_07A53F1989C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "cdxCSizingDialog.h"
#include "FormExm.h"
#include "FormDepartment.h"
#include "FormAnswer.h"
#include "FormResult.h"
#include "FormlLink.h"
#include "FormStuinfo.h"
#include "FormSubject.h"
#include "FormExamaddr.h"
#include "FormClassResult.h"
#include "FormTeacherDev.h"
#include "FormStuinfoSet.h"
#include "FormPaper.h"
#include "FormPCoffRoll.h"
#include "FormPCoffRolling.h"
#include "FormFM.h"
#include "FormImport.h"
#include "FormExamMode.h"
#include "FormExamSet.h"
#include "FormListen.h"
#include "FormImage.h"
#include "FormExamBlue.h"
#include "FormExamInfrar.h"
#include "FormExamUSB.h"
#include "FormExamTime.h"
#include "FormStuResultQuery.h"
#include "FormClassResultQuey.h"
#include "FormPaperAnaly.h"
#include "FromImageread.h"
#include "FormExamInfoBack.h"
#include "FormCreatFile.h"
#include "FormExamDetail.h"
#include "FormExamImport.h"
#include "FormExamInrarOff.h"
#include "FormExamRollingAnswer.h"

class CFormMain : public cdxCSizingDialog
{
// Construction
public:
	DECLARE_DYNCREATE(CFormMain)
	CFormMain(CWnd* pParent = NULL);   // standard constructor
	CBitmap        m_bmpBackground;
	//{{AFX_DATA(CFormMain)
	enum { IDD = IDD_FORM_MAIN };
	CButton	m_group;
	//}}AFX_DATA
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormMain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CFormMain)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void            ShowDlg(DWORD id);
public:
	CFormExm        m_exam;
	CFormDepartment m_department;
	CFormAnswer		m_stdanswer;
	CFormResult     m_result;
	CFormlLink      m_link;
	CFormStuinfo	m_stuinfo;
	CFormSubject    m_subject;
	CFormExamaddr   m_addr;
	CFormClassResult m_classresult;
	CFormTeacherDev  m_teacherdev;
	CFormStuinfoSet  m_stuinfoset;
	CFormPaper      m_paper;
	CFormPCoffRoll  m_pcoffroll;
	CFormPCoffRolling  m_pcoffrolling;
	CFormFM        m_fm;
	CFormImport    m_import;
	CFormExamMode  m_exammode;
	CFormExamSet   m_examset;
	CFormListen    m_listen;
	CFormImage     m_image;
	CFormExamBlue  m_examblue;
	CFormExamInfrar m_examinfrar;
	CFormExamUSB   m_examusb;
	CFormExamTime  m_examtime;
	CFormStuResultQuery   m_sturesquery;
	CFormClassResultQuey  m_classquery;
	CFormPaperAnaly       m_paperanaly;
	CFromImageread        m_imageread;
	CFormExamInfoBack     m_examinfoback;
	CFormCreatFile        m_creatfile;
	CFormExamDetail       m_examdetail;
	CFormExamImport       m_examimport;
	CFormExamInrarOff     m_examintatoff;
	CFormExamRollingAnswer  m_examrollinganswer;
 
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_DLGMAIN_H__40C8C595_2397_4B9C_9D30_07A53F1989C6__INCLUDED_)
