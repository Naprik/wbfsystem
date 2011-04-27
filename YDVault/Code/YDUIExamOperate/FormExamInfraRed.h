
// IRTESTDlg.h : 头文件
//

#pragma once
#include "resource.h"
#include "../YDFormUIBase\YdFormView.h"
#include "../UIBase\BCGPSortListCtrl.h"
#include "afxwin.h"

class CYDInfraRedAppCom;
class CYDEAddrUnit;
class CYDStuMark;

const int cMaxComCount = 10;//可以同时打开红外的最大端口数

// CFormExamInfraRed 对话框
class BASE_DLL_API CFormExamInfraRed : public CYdFormView
{
// 构造
public:
	CFormExamInfraRed();	// 标准构造函数
	virtual ~CFormExamInfraRed();
	DECLARE_DYNCREATE(CFormExamInfraRed)

// 对话框数据
	enum { IDD = IDD_YDUIEXAM_FORM_EXAM_INFRARED };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	BOOL change_flag;


	
	BOOL m_bOpenCom;//是否打开端口

	HANDLE m_hThreadDecode[cMaxComCount];//解码线程
	HANDLE m_hThreadReadData[cMaxComCount];//读取Com数据的线程
	std::list<CYDInfraRedAppCom*> m_lstYDAppCom;

	CString m_recv;


	void ReflashList(int iIndex);
	
	BOOL CheckMac(CString strMac);
	CComboBox m_cmbExamPaper;
	CComboBox m_cmbExamSubject;
	CComboBox m_cmbExamRoom;


// 实现
protected:
	// 生成的消息映射函数
	virtual void OnInitialUpdate();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CBCGPSortListCtrl m_lstUnit;
private:
	HRESULT InitInfo();
	HRESULT InitExamClassRoom();//初始化考场
	HRESULT InitExamSubject();//初始化考试科目
	std::list<CYDObjectRef*> m_lstClear;
	HRESULT InsertUnit(CYDObjectRef* _pUnit);
	HRESULT UpdateListByMac(CString _strMac,CString _strAnswer);
	//保存list中学生成绩表到数据库
	HRESULT SaveListAnswer();
public:
	afx_msg void OnCbnSelchangeComboExamSubject();
	afx_msg void OnCbnSelchangeComboExamRoom();
	CComboBox m_cmbDevCom;
	afx_msg void OnBnClickedButtonOpenport();
private:
public:
	BOOL	m_bThreadQuit;  //线程退出的标记
	virtual HRESULT Close();
	HRESULT DecodData(CYDInfraRedAppCom* _pAppCom);
	HRESULT ReadData(CYDInfraRedAppCom* _pAppCom);
	
private:
	HRESULT CloseThread();
	HRESULT CreateStuMark(CYDEAddrUnit* _pUnit,CYDStuMark*& _pStuMark);
	HRESULT SetGStructData();//设置全局变量
	HRESULT OpenTeacherCom();//打开红外教师机端口，根据combox中的端口号，可能有多个
	CFont       m_EditFont;//设置edit的大小
public:
	COleDateTime m_timeExam;
	BOOL		 m_bNotSaveExam;//标记是否有提取到答案的学生信息未存到数据库中
private:
	std::map<CYDEAddrUnit*, CYDStuMark*> m_mapUnitToMark;//一个考场单元对应于一个考试成绩
	CCriticalSection m_cs; //临界点控制，用于多线程中控制
public:
	afx_msg void OnNMDblclkListStu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonQuerymark();
	afx_msg void OnBnClickedButtonPaperAns();
	CString m_strReceiveStuno;
	afx_msg void OnCbnSelchangeComboDevCom();
	virtual BOOL DestroyWindow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonNewPaper();
};
