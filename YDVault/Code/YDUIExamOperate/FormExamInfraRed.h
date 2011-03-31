
// IRTESTDlg.h : ͷ�ļ�
//

#pragma once
#include "resource.h"
#include "../YDFormUIBase\YdFormView.h"
#include "../UIBase\BCGPSortListCtrl.h"
#include "afxwin.h"

class CYDInfraRedAppCom;
class CYDEAddrUnit;
class CYDStuMark;

// CFormExamInfraRed �Ի���
class BASE_DLL_API CFormExamInfraRed : public CYdFormView
{
// ����
public:
	CFormExamInfraRed();	// ��׼���캯��
	virtual ~CFormExamInfraRed();
	DECLARE_DYNCREATE(CFormExamInfraRed)

// �Ի�������
	enum { IDD = IDD_YDUIEXAM_FORM_EXAM_INFRARED };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	BOOL change_flag;


	CYDInfraRedAppCom*   m_pYDAppCom;

	BOOL m_bOpenCom;//�Ƿ�򿪶˿�

	HANDLE m_hThreadDecode;//�����߳�
	HANDLE m_hThreadReadData;//��ȡCom���ݵ��߳�

	CString m_recv;


	void ReflashList(int iIndex);
	
	BOOL CheckMac(CString strMac);
	CComboBox m_cmbExamPaper;
	CComboBox m_cmbExamSubject;
	CComboBox m_cmbExamRoom;


// ʵ��
protected:
	// ���ɵ���Ϣӳ�亯��
	virtual void OnInitialUpdate();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CBCGPSortListCtrl m_lstUnit;
private:
	HRESULT InitInfo();
	HRESULT InitExamClassRoom();//��ʼ������
	HRESULT InitExamSubject();//��ʼ�����Կ�Ŀ
	std::list<CYDObjectRef*> m_lstClear;
	HRESULT InsertUnit(CYDObjectRef* _pUnit);
	HRESULT UpdateListByMac(CString _strMac,CString _strAnswer);
	//����list��ѧ���ɼ������ݿ�
	HRESULT SaveListAnswer();
public:
	afx_msg void OnCbnSelchangeComboExamSubject();
	afx_msg void OnCbnSelchangeComboExamRoom();
	CComboBox m_cmbDevCom;
	afx_msg void OnBnClickedButtonOpenport();
private:
public:
	BOOL	m_bThreadQuit;  //�߳��˳��ı��
	virtual HRESULT Close();
	HRESULT DecodData();
	HRESULT ReadData();
private:
	HRESULT CloseThread();
	HRESULT CreateStuMark(CYDEAddrUnit* _pUnit,CYDStuMark*& _pStuMark);
	HRESULT SetGStructData();//����ȫ�ֱ���
	CFont       m_EditFont;//����edit�Ĵ�С
public:
	COleDateTime m_timeExam;
	BOOL		 m_bNotSaveExam;//����Ƿ�����ȡ���𰸵�ѧ����Ϣδ�浽���ݿ���
private:
	std::map<CYDEAddrUnit*, CYDStuMark*> m_mapUnitToMark;//һ��������Ԫ��Ӧ��һ�����Գɼ�
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
