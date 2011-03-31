
#include <afxmt.h>
//{{AFX_INCLUDES()
#include "shockwaveflash.h"
//}}AFX_INCLUDES
#ifndef __PSTAT_H__
#define __PSTAT_H__
#ifndef __AFXMT_H__
#pragma error("PStat requires <afxmt.h> to be in your precompiled header")
#endif
//// Flags which can be used to customize the Pstat code
const DWORD PSTAT_CANCEL        = 0x0001;  // Display a cancel button on the progress dialog
const DWORD PSTAT_CONFIRMCANCEL = 0x0002;  // Confirm cancel by displaying a messagebox
///// forward declaration
class CProgressThreadDlg;
///// The function declaration of the function to be called in a secondary thread with a progress dialog
typedef ULONG (FUNCTION_WITH_PROGRESS)(void* pData, CProgressThreadDlg* pProgressDlg);
//##ModelId=401921B50237
class CProgressThreadDlg : public CDialog
{
public:
	void SetRange(long max);
	void SetStep(long pos);
	//##ModelId=401921B502EC
	void SetText(CString text);
	DECLARE_DYNAMIC(CProgressThreadDlg)
	// Constructors / Destructors
	//##ModelId=401921B502E4
	CProgressThreadDlg(BOOL bShowCancelButton, BOOL bConfirmCanel, CWnd* pParent = NULL);
	// Methods
	//##ModelId=401921B502E3
	void Close();  
	//##ModelId=401921B502E2
	CEvent& GetEventReady() { return m_EventReady; };
	//##ModelId=401921B502DC
	BOOL HasBeenCancelled() const { return m_bCancelled; };
	//##ModelId=401921B502DA
	//##ModelId=401921B5026B
	friend BOOL ExecuteFunctionWithProgressDialog(FUNCTION_WITH_PROGRESS* pfnFunction, const CString& sProgressTitle, void* pData = NULL,DWORD dwFlags = 0, const CString& sConfirmPrompt = _T(""), int nPriority = THREAD_PRIORITY_NORMAL, CWnd* pParent = NULL);
protected:
	//{{AFX_DATA(CProgressThreadDlg)
	enum { IDD = IDD_PROGRESS };
	CStatic	m_account;
	CProgressCtrl	m_ctrlProgress;
	CShockwaveFlash	m_falsh;
	//}}AFX_DATA
	//##ModelId=401921B502D8
	virtual void OnCancel();
	//{{AFX_VIRTUAL(CProgressThreadDlg)
protected:
	//##ModelId=401921B502D2
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL
	//{{AFX_MSG(CProgressThreadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelled();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	//##ModelId=401921B502C4
	CTime       m_TimeCreation;
	//##ModelId=401921B502BD
	int         m_nCurrentPercentage;
	//##ModelId=401921B502BC
	BOOL        m_bReady;
	//##ModelId=401921B502BB
	CString     m_sCaption;
	//##ModelId=401921B502BA
	CString     m_sConfirmPrompt;
	//##ModelId=401921B502B2
	BOOL        m_bShowCancelButton;
	//##ModelId=401921B502B1
	BOOL        m_bConfirmCancel;
	//##ModelId=401921B502B0
	BOOL        m_bCancelled;
	//##ModelId=401921B502AB
	BOOL        m_bOkToClose;
	//##ModelId=401921B502A8
	CEvent      m_EventReady;
	//##ModelId=401921B5029D
	CWinThread* m_pThread;
public:
	long        m_max;
	long        m_pos;
	BOOL		m_flag;
	CString           GetExePath();
};
#endif //__PSTAT_H__
