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
	HRESULT InitExamClassRoom();//��ʼ������
	HRESULT InitExamSubject();//��ʼ�����Կ�Ŀ
	HRESULT ShowXmlInfo(CString _strXmlFile);
	//_bUpdate= True����ǰ���ݴ浽pGStruct��
	//_bUpdate=False,��pGStruct�����ݴ浽��ǰ�ı�����
	HRESULT UpdateRollCallUI(BOOL _bUpdate);
	//�����Ծ����ÿ��ԵĶ���
	HRESULT UpdateLstSeqByPaperObj(CYDObjectRef* _pPaper);
	HRESULT InitTeacherList();
	HRESULT InitStudentList();
	HRESULT InsertUnit(CYDObjectRef* _pUnit);
	HRESULT ClearStu();
	HRESULT EnableExamInfoBtn(BOOL _bEnable);//ʹ������Ϣ�İ�ť�Ƿ����
	HRESULT EnableTeacherInfoBtn(BOOL _bEnable);
	HRESULT EnableStudentInfoBtn(BOOL _bEnable);
	BOOL	ValidateData(OPERATION nOp);
	//ֻ����ȡ��ʦ����ʱ����ȡѧ������Ϣ
	HRESULT ReadStudevOnlyFromTeacher();
	HRESULT UpdateListByStudev(CStudev* _pStudev);
	HRESULT DevideMacToTeacherApp(std::list<CString>  &_lstStuDevMac);
	//�п��Ե�Ԫ�õ�����Ӧ��ѧ����
	HRESULT GetStudevByUnit(CYDEAddrUnit* _pUnit,
		CGExamStruct* _pGExamStruct,
		CStudev* &_pStudev);
	HRESULT SetProgressEditTxt();
	//_pStuCom��USB�򿪵�ѧ�������������޸��б��ϵĽ��뿼��״̬
	HRESULT UpdateListBeginExamState(CYDStuAppCom* _pStuCom);
	//_pStuCom��USB�򿪵�ѧ�������������޸��б��ϵ����վ�״̬
	HRESULT UpdateListRollingState(CYDStuAppCom* _pStuCom);
	//����_pStuCom��Mac��ַ�õ��б��ϵĶ�ӦItem�����û����_nItem����-1
	HRESULT GetItemByStuCom(CYDStuAppCom* _pStuCom,int& _nItem);
	//���USB�н�ʦ������������߳�
	HRESULT StartThreadTeacherComJump();
	HRESULT EndThreadTeacherComJum();
	//��ʾ����İ�ť��һЩ��Ϣ
	HRESULT ShowRollInfoButton();
public:
	CComboBox m_cmbExamKind;
	CComboBox m_cmbExamRoom;
	CComboBox m_cmbExamPaper;
	CComboBox m_cmbExamSubject;
	//m_lstAppToMacList:����ͬ��ѧ����Mac��ַ���䵽��ͬ�Ľ�ʦ���Ľ��
	std::list<CTeacherAppToMacList*> m_lstAppToMacList;
	BOOL	m_bCancelThread;//�˳��̵߳ı��
	BOOL	m_bCanClose;//�������̺߳󣬵�ǰ���ڲ��ܹرգ����Ǳ�ǣ��߳̽���ʱ�Ὣ����ΪTRUE
private:
	std::list<CYDObjectRef*> m_lstClear;
	std::list<CYDObjectRef*> m_lstStu;//��ǰ������ѧ����Ϣ
	BOOL	m_bTimeProgressStatus;//�Ƿ�ͨ��ʱ�������ƽ�����
	CWinThread*		m_pJumbThread;//���USB�����Ƿ����ӽ�ʦ��
	CString         m_strFileXml;//�վ��ʱ����Զ����ɣ�ֻ�������ļ���Ϊһ��
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
	afx_msg void OnMiUSBRolling();//ͨ��USB�����վ�ѧ����
	afx_msg void OnMiUSBBeginExam();//ͨ��USB���ƿ���ѧ����
	afx_msg void OnMiStudentDetailInfo();//�鿴ѧ������
	afx_msg void OnMiContinueBeginExam();//�Ե�ǰ�б�ѡ�е�ѧ����������

	//���ƽ�����
	afx_msg HRESULT OnProgressShow(WPARAM wParam,LPARAM lParam);
	afx_msg HRESULT OnProgressRange(WPARAM wParam,LPARAM lParam);
	afx_msg HRESULT OnProgressStepIt(WPARAM wParam, LPARAM lParam);
	afx_msg HRESULT OnProgressShowCancel(WPARAM wParam,LPARAM lParam);
	CStatic m_WaitTxt;
	CString m_strProgress;
	afx_msg void OnBnClickedButtonNewPaper();
};


