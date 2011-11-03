#ifndef _YD_SHOW_ERROR_H 
#define _YD_SHOW_ERROR_H 

//////////////////////////////////////////////////
//note:
// the definition can use everywhere !!!
//////////////////////////////////////////////////
#include "../UIBase/YDErrorDlg.h"
#include "../Base/LanguageTranslator.h"
#include "YDVaultComm.h"



#ifndef E_HRESULT_UNKNOWN_ERROR 
#define E_HRESULT_UNKNOWN_ERROR              0x81040700  //未知的异常或者错误，请和软件开发商联系！
#endif

//系统统一错误显示函数
//hr : 系统或者COM定义的错误码,参见YDError.h、YDMsg.h和msg.xml
//nIcon : 图标和按钮类型 图标支持MB_ICONWARNING,MB_ICONQUESTION,MB_ICONSTOP,MB_ICONERROR
//        按钮支持MB_OK,MB_OKCANCEL,MB_YESNO
//s1,s2,s3 参数 如果系统定义的错误消息包含1-3个%s(只支持),该函数将这些参数合并到消息中
//返回:IDOK,IDCANCEL
inline int DISPLAY_YDERROR(HRESULT hr,UINT nIcon,const CString& s1=_T(""),const CString& s2=_T(""),const CString& s3=_T(""),CWnd* pParentWnd=NULL)
{
	return IDOK;
	CLanguageTranslator* pTrans = NULL;
	CWnd* pWnd = AfxGetMainWnd();
	if(pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
	{
		pTrans = (CLanguageTranslator*)pWnd->SendMessage(WM_GET_MSG_DICTIONARY);
	}
	
	CString strMsg;
	if(pTrans == NULL) 
	{
		CLanguageTranslator tempTrans;
		tempTrans.Translate(hr,strMsg);
	}
	else 
	{
		pTrans->Translate(hr,strMsg);
	}
	if(strMsg.IsEmpty())
	{
		CString str(_T("")),strFormat(_T(""));		
		str.Format(_T("hr=0x%x"),hr);
		pTrans->Translate(E_HRESULT_UNKNOWN_ERROR,strMsg);
		strFormat = strMsg;
		strMsg.Format(strFormat,(LPCTSTR)str);
	}
	CString str;
	if(!s1.IsEmpty() && s2.IsEmpty() && s3.IsEmpty())
		str.Format((LPCTSTR)strMsg,(LPCTSTR)s1);
	else if(!s1.IsEmpty() && !s2.IsEmpty() && s3.IsEmpty())
		str.Format((LPCTSTR)strMsg,(LPCTSTR)s1,(LPCTSTR)s2);
	else if(!s1.IsEmpty() && !s2.IsEmpty() && !s3.IsEmpty())
		str.Format((LPCTSTR)strMsg,(LPCTSTR)s1,(LPCTSTR)s2,(LPCTSTR)s3);
	else
		str = strMsg;

	CYdErrorDlg dlg(str,hr,nIcon,pParentWnd);
	return dlg.DoModal();
}

inline void GET_YDERROR(HRESULT hr,CString& strMsg)
{
	CLanguageTranslator* pTrans = NULL;
	CWnd* pWnd = AfxGetMainWnd();
	if(pWnd != NULL && pWnd->GetSafeHwnd() != NULL)
	{
		pTrans = (CLanguageTranslator*)pWnd->SendMessage(WM_GET_MSG_DICTIONARY);
	}
	if(pTrans == NULL) 
	{
		CLanguageTranslator tempTrans;
		tempTrans.Translate(hr,strMsg);
	}
	else 
	{
		pTrans->Translate(hr,strMsg);
	}
}

#endif