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
	//����һ��ʵ�������ʾ�����ԣ������½�һ��ʵ�������ʾ�յ�����
	//pObjWrapper:һ��ʵ�����İ�װ
	//nOp����ʾ���� (OP_NEW,OP_VIEW,OP_EDIT)
	virtual HRESULT Show(CYdObjWrapper* pObjWrapper,OPERATION nOp);

	virtual HRESULT Show(CString sTipName, OPERATION nOp);

	//����ʾ���Լ������(�����ĵ�)
	virtual HRESULT ShowPropertyView(CYdObjWrapper* pObjWrapper,OPERATION nOp);

	//����һ���½���ʵ����������
	virtual HRESULT Save();

	//����һ�������༭��ʵ����������
	virtual HRESULT Update();

	//�ر�
	virtual HRESULT Close();
	//����
	virtual HRESULT UpdateShow();

	//������ʾ��ʽ
	virtual HRESULT ResetOPERATION(OPERATION nOp);
	virtual void GetOPERATION(OPERATION& nOp){nOp = m_nOPERATION;}
	virtual BOOL IsModified(){return TRUE;}
	virtual BOOL ValidateData(OPERATION nOp);
	
public:
	//���ڻ�ȡ����page��Ӧ��form��������
	CObjPropertyView* GetParentView(){return m_pPropertyView;}
	UINT GetPropertyCount();
	CYdObjWrapper* GetCurObjWrapper(){return m_pObjWrapper;}
	CYdObjWrapper* GetParentObjWrapper(); //�õ����ϵĸ��ڵ�

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
