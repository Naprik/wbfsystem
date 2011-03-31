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
	//�ر�֮ǰѯ��Ҫ��������
	HRESULT Remove(CObjPropertySheet* pSheet);
	//ǿ�ƹرգ���ѯ��Ҫ��������
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
	std::map<CYdObjWrapper*,CObjPropertySheet*> m_mapObjSheet;//�������ص�sheet
	std::map<CString,CObjPropertySheet*> m_mapStrSheet;//������޹ص�sheet
};

#endif // !defined(AFX_OBJPROPVIEWMANAGER_H__FCC6CD78_A83C_4A80_8B10_5F373DABE097__INCLUDED_)
