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
#define E_HRESULT_UNKNOWN_ERROR              0x81040700  //δ֪���쳣���ߴ�����������������ϵ��
#endif

//ϵͳͳһ������ʾ����
//hr : ϵͳ����COM����Ĵ�����,�μ�YDError.h��YDMsg.h��msg.xml
//nIcon : ͼ��Ͱ�ť���� ͼ��֧��MB_ICONWARNING,MB_ICONQUESTION,MB_ICONSTOP,MB_ICONERROR
//        ��ť֧��MB_OK,MB_OKCANCEL,MB_YESNO
//s1,s2,s3 ���� ���ϵͳ����Ĵ�����Ϣ����1-3��%s(ֻ֧��),�ú�������Щ�����ϲ�����Ϣ��
//����:IDOK,IDCANCEL
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