// ObjPropertySheet.h: interface for the CObjPropertySheet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJPROPERTYSHEET_H__DE62BA28_0247_4FAE_962F_3AC56D9DCFF6__INCLUDED_)
#define AFX_OBJPROPERTYSHEET_H__DE62BA28_0247_4FAE_962F_3AC56D9DCFF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma  warning(disable:4786)
#include <list>


class CObjPropertyView;
class CMonikerToDBMapper;
class CCustomizedStructureTree;
class CRelObjListCtrl;
class CYdFormView;
class CYDObjectRef;
class CYdObjWrapper;


class  BASE_DLL_API CObjPropertySheet : public CObject  
{
	DECLARE_DYNCREATE(CObjPropertySheet);
public:
	CObjPropertySheet(const CObjPropertyView* pView);
	virtual ~CObjPropertySheet();

	//overrides
public:
	//给定一个实体对象显示其属性，或者新建一个实体对象显示空的属性
	//pObjWrapper:一个实体对象的包装
	//nOp：显示类型 (OP_NEW,OP_VIEW,OP_EDIT)
	virtual HRESULT Show(CYdObjWrapper* pObjWrapper,OPERATION nOp);

	virtual HRESULT Show(CString sTipName, OPERATION nOp);

	//仅显示属性及浏览器(若是文档)
	virtual HRESULT ShowPropertyView(CYdObjWrapper* pObjWrapper,OPERATION nOp);

	//保存一个新建的实体对象的属性
	virtual HRESULT Save();

	//更新一个经过编辑的实体对象的属性
	virtual HRESULT Update();

	//关闭
	virtual HRESULT Close();
	//更新
	virtual HRESULT UpdateShow();

	//设置显示方式
	virtual HRESULT ResetOPERATION(OPERATION nOp);
	virtual void GetOPERATION(OPERATION& nOp){nOp = m_nOPERATION;}
	virtual BOOL IsModified(){return TRUE;}
	virtual BOOL ValidateData(OPERATION nOp);
	
public:
	//用于获取所有page对应的form中属性项
	CObjPropertyView* GetParentView(){return m_pPropertyView;}
	UINT GetPropertyCount();
	CYdObjWrapper* GetCurObjWrapper(){return m_pObjWrapper;}
	CYdObjWrapper* GetParentObjWrapper(); //得到树上的父节点

	CYdFormView* AddPage(CRuntimeClass* pViewClass, const CString& strViewLabel,
				int iIndex = -1, CCreateContext* pContext = NULL);
	void RemovePage(CYdFormView* pPage);
	UINT GetPageCount();
	CYdFormView* GetPage(UINT nIndex);
	void ActivePage(int nIndex);
	void ReadonlyPages(BOOL bReadonly);

protected:

public:
	virtual int GetPreTabIndex();
	virtual void SetPreTabIndex(int index);
	static CMonikerToDBMapper*  m_pMapper;
	BOOL m_bDelete;
protected:
	CObjPropertySheet();

	CYdObjWrapper*              m_pObjWrapper;
	OPERATION                    m_nOPERATION;
	CObjPropertyView*			m_pPropertyView;
	std::list<CYdFormView*> m_listPages;
private:


};	

#endif // !defined(AFX_OBJPROPERTYSHEET_H__DE62BA28_0247_4FAE_962F_3AC56D9DCFF6__INCLUDED_)
