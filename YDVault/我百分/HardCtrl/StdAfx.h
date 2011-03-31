// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__29CCEAE0_A1DD_48D6_A44C_2693AEACD12E__INCLUDED_)
#define AFX_STDAFX_H__29CCEAE0_A1DD_48D6_A44C_2693AEACD12E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include "sizecbar.h" 
#include "scbarg.h" 
#include "Resource.h"
#include <afxdb.h>
#include <winsock2.h>			// Winsock2
#include "..\\Share\\index.h"

//#include "SkinMagicLib.h"

#include <WINSPOOL.H>


#define	 B5_W			182				//B5纸宽度mm
#define	 B5_H			257				//B5纸高度mm
#define	 B5_ONELINE		29				//B5纸第一页行数
#define	 B5_OTHERLINE	30				//B5纸其它页行数

//打印结构
typedef struct
{
	int 	nMaxLine;			//最大行数
	int		nCountPage;			//一共页数
	int		nCurPage;			//当前页码
	BOOL	IsPrint;			//是否打印
	HWND	hWnd;				//窗口句柄
	HWND	hListView;			//列表控件句柄
	TCHAR	szTag[256];			//其它数据
	int		nTag;				//其它数据
	LPVOID	lpVoid;				//其它数据
}PRNINFO, *PPRNINFO;

typedef void(*PRINTPREVIEW) (CDC &MemDC, PRNINFO PrnInfo);
#include "Resource.h"

/*
#pragma warning(disable:4146)
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" named_guids rename("EOF","adoEOF"), rename("BOF","adoBOF")
#pragma warning(default:4146)
using namespace ADODB;  
*/
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

extern  CString resetCmd;
extern  CString getteacherdevCmd;
extern	CString searchCmd;
extern  CString getaddrCmd;
extern	CString querysendexammessageCmd;
extern	CString startexmaCmd;
extern	CString statCmd;
extern  CString queryexamstatCmd;
extern	CString queryrollingCmd;
extern	CString getanswerCmd;
extern  CString recvResetCmd;
extern	CString recvSearchCmd;
extern	CString recvSendlegaladdrCmd;
extern	CString	recvSendexammessageCmd;
extern	CString recvSendsingleinfoCmd;
extern	CString recvSendstepexammessageCmd;
extern	CString recvUpdatestudev;
extern	CString recvAddmac;
extern	CString recvSinglemacsuccess;
extern	CString recvSinglemacfail;
extern	CString recvRollingCmd;
extern  BOOL	b_isSetexam;
extern  int		i_examMode;
#endif // !defined(AFX_STDAFX_H__29CCEAE0_A1DD_48D6_A44C_2693AEACD12E__INCLUDED_)
