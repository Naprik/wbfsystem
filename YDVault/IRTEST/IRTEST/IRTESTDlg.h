
// IRTESTDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "SortListCtrl.h"
#include "YDAppCom.h"
#include "afxwin.h"
#include "Dev.h"


// CIRTESTDlg 对话框
class CIRTESTDlg : public CDialog
{
	// 构造
public:
	CIRTESTDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_IRTEST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	BOOL change_flag;

	int m_nDlgWidth; 
	int m_nDlgHeight;
	//计算分辨率
	int m_nWidth; 
	int m_nHeight;
	//计算放大倍数
	float m_Multiple_width; 
	float m_Mutiple_heith; 

	void ReSize(int nID);
	void ReSizeEx(int nID);

	CYDAppCom   m_appcom;

	BOOL m_open;

	HANDLE m_hThread;
	HANDLE m_hThread1;

	CString m_recv;

	CArray<CDev, CDev> m_devlist;

	void ReflashList(int iIndex);
	void DecodData();
	int Str2Dec(CString str);
	int String2Hex(CString str, char *SendOut);
	TCHAR ConvertHexData(TCHAR ch);
	CString FormatNO(CString strNO);

	BOOL CheckMac(CString strMac);


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOk();
	CSortListCtrl m_stulist;
	//	int m_port;
	afx_msg void OnBnClickedOpenport();
	afx_msg void OnBnClickedStart();
	CListBox m_datalist;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLvnItemchangedStulist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSearch();
	CString m_port;
	afx_msg void OnBnClickedClearList();
	afx_msg void OnBnClickedClearBox();
};
