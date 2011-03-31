// ObjPropViewManager.h: interface for the CObjPropSheetManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJPROPVIEWMANAGER_H__FCC6CD78_A83C_4A80_8B10_5F373DABE097__INCLUDED_)
#define AFX_OBJPROPVIEWMANAGER_H__FCC6CD78_A83C_4A80_8B10_5F373DABE097__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable:4786)

#include <map>
#include <comdef.h>
#include "ObjPropertySheet.h"

class CYdObjWrapper;

class BASE_DLL_API CObjPropSheetManager  
{
public:
	CObjPropSheetManager();
	virtual ~CObjPropSheetManager();

public:
	HRESULT Show(CYdObjWrapper* pWrapper, OPERATION nOp,CObjPropertySheet* &_pSheet);
	HRESULT Show(CString _sTitle,
							OPERATION nOp,
							CObjPropertySheet* &_pSheet);
	//关闭之前询问要保存或更新
	HRESULT Remove(CObjPropertySheet* pSheet);
	//强制关闭，不询问要保存或更新
	HRESULT RemoveUnderNoAsk(CObjPropertySheet* pSheet);
	CObjPropertySheet* IsExist(CYdObjWrapper* pWrapper);
	CObjPropertySheet* IsExist(CString _str);
	CObjPropertySheet* CreateEmptyPropertySheet(const CString& moniker,const CObjPropertyView* pMainView,BOOL bCustomizedTree=FALSE);
	void RemoveAll();
	void Active(CObjPropertySheet* pSheet);
	int GetSheetObjWraperCount(){return m_mapObjSheet.size();}
	void GetSheetObjWraper(int nIndex,CObjPropertySheet*& pSheet,CYdObjWrapper*& pWrapper);
	int GetSheetStrCount(){return m_mapStrSheet.size();}
	void GetSheetStr(int nIndex,CObjPropertySheet*& pSheet,CString& _str);
private:
	int m_nMaxViewNum;
	std::map<CYdObjWrapper*,CObjPropertySheet*> m_mapObjSheet;//与对象相关的sheet
	std::map<CString,CObjPropertySheet*> m_mapStrSheet;//与对象无关的sheet
};

#endif // !defined(AFX_OBJPROPVIEWMANAGER_H__FCC6CD78_A83C_4A80_8B10_5F373DABE097__INCLUDED_)
