#pragma once
#include "../YDFormUIBase\YdFormView.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "../UIBase\BCGPSortListCtrl.h"
#include "resource.h"
//#include "../UIBase\ComboBoxSelChange.h"


// CFormExamBlue form view

class CTeacherAppToMacList;
class CYDStuAppCom;
class BASE_DLL_API CFormExamBlue : public CYdFormView
{
	DECLARE_DYNCREATE(CFormExamBlue)

protected:
	CFormExamBlue();           // protected constructor used by dynamic creation
	virtual ~CFormExamBlue();

public:
	enum { IDD = IDD_YDUIEXAM_FORM_EXAM_BLUE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual HRESULT Close();
private:
	HRESULT InitExamClassRoom();//初始化考场
	HRESULT InitExamSubject();//初始化考试科目
	HRESULT ShowXmlInfo(CString _strXmlFile);
	//_bUpdate= True将当前数据存到pGStruct中
	//_bUpdate=False,将pGStruct中数据存到当前的变量中
	HRESULT UpdateRollCallUI(BOOL _bUpdate);
	//根据试卷设置考试的段数
	HRESULT UpdateLstSeqByPaperObj(CYDObjectRef* _pPaper);
	HRESULT InitTeacherList();
	HRESULT InitStudentList();
	HRESULT InsertUnit(CYDObjectRef* _pUnit);
	HRESULT ClearStu();
	HRESULT EnableExamInfoBtn(BOOL _bEnable);//使考试信息的按钮是否可用
	HRESULT EnableTeacherInfoBtn(BOOL _bEnable);
	HRESULT EnableStudentInfoBtn(BOOL _bEnable);
	BOOL	ValidateData(OPERATION nOp);
	//只是提取教师机答案时，读取学生机信息
	HRESULT ReadStudevOnlyFromTeacher();
	HRESULT UpdateListByStudev(CStudev* _pStudev);
	HRESULT DevideMacToTeacherApp(std::list<CString>  &_lstStuDevMac);
	//有考试单元得到他对应的学生机
	HRESULT GetStudevByUnit(CYDEAddrUnit* _pUnit,
		CGExamStruct* _pGExamStruct,
		CStudev* &_pStudev);
	HRESULT SetProgressEditTxt();
	//_pStuCom是USB打开的学生机，根据其修改列表上的进入考试状态
	HRESULT UpdateListBeginExamState(CYDStuAppCom* _pStuCom);
	//_pStuCom是USB打开的学生机，根据其修改列表上的已收卷状态
	HRESULT UpdateListRollingState(CYDStuAppCom* _pStuCom);
	//根据_pStuCom的Mac地址得到列表上的对应Item，如果没有则_nItem返回-1
	HRESULT GetItemByStuCom(CYDStuAppCom* _pStuCom,int& _nItem);
	//检测USB中教师机连接情况的线程
	HRESULT StartThreadTeacherComJump();
	HRESULT EndThreadTeacherComJum();
	//显示交卷的按钮的一些信息
	HRESULT ShowRollInfoButton();
public:
	CComboBox m_cmbExamKind;
	CComboBox m_cmbExamRoom;
	CComboBox m_cmbExamPaper;
	CComboBox m_cmbExamSubject;
	//m_lstAppToMacList:将不同的学生机Mac地址分配到不同的教师机的结果
	std::list<CTeacherAppToMacList*> m_lstAppToMacList;
	BOOL	m_bCancelThread;//退出线程的标记
	BOOL	m_bCanClose;//当启动线程后，当前窗口不能关闭，这是标记，线程结束时会将其设为TRUE
private:
	std::list<CYDObjectRef*> m_lstClear;
	std::list<CYDObjectRef*> m_lstStu;//当前考场的学生信息
	BOOL	m_bTimeProgressStatus;//是否通过时间来控制进度条
	CWinThread*		m_pJumbThread;//检测USB口上是否连接教师机
	CString         m_strFileXml;//收卷的时候会自动生成，只会生成文件名为一次
public:
	
	afx_msg void OnCbnSelchangeComboExamSubject();

	COleDateTime m_timeStart;
	COleDateTime m_timeEnd;
	afx_msg void OnBnClickedButtonDetail();
	afx_msg void OnCbnSelchangeComboExamPaper();
	CBCGPSortListCtrl m_lstCtrlTeacher;
	CBCGPSortListCtrl m_lstCtrlUnit;
	afx_msg void OnCbnSelchangeComboExamRoom();
	afx_msg void OnBnClickedCheckSelAll();
	afx_msg void OnBnClickedCheckSelInverse();
	afx_msg void OnBnClickedTeacherTest();
	afx_msg void OnBnClickedTeacherChoose();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonBeginexam();
	afx_msg void OnBnClickedButtonRolling();
	afx_msg void OnBnClickedButtonCancelProgress();
	afx_msg void OnBnClickedButtonQuerymark();
	afx_msg void OnBnClickedButtonPaperAnsys();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNMDblclkListStudent2(NMHDR *pNMHDR, LRESULT *pResult);
	CProgressCtrl m_ctrlProgressStaus;
	afx_msg void OnNMRClickListStudent2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMiUSBRolling();//通过USB控制收卷学生机
	afx_msg void OnMiUSBBeginExam();//通过USB控制开考学生机
	afx_msg void OnMiStudentDetailInfo();//查看学生详情
	afx_msg void OnMiContinueBeginExam();//对当前列表选中的学生继续考试

	//控制进度条
	afx_msg HRESULT OnProgressShow(WPARAM wParam,LPARAM lParam);
	afx_msg HRESULT OnProgressRange(WPARAM wParam,LPARAM lParam);
	afx_msg HRESULT OnProgressStepIt(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnProgressShowCancel(WPARAM wParam,LPARAM lParam);
	CStatic m_WaitTxt;
	CString m_strProgress;
	afx_msg void OnBnClickedButtonNewPaper();
};


