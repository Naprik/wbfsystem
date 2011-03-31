#pragma once
#include "ObjPropertySheet.h"


// CYdFormView form view
#include "Resource.h"

class BASE_DLL_API CYdFormView : public CFormView
{
	DECLARE_DYNCREATE(CYdFormView)
	CYdFormView(UINT nIDTemplate);
protected:
	CYdFormView();           // protected constructor used by dynamic creation
	virtual ~CYdFormView();
protected:
	CObjPropertySheet*       m_pSheet;
public:
	void SetPropertySheet(CObjPropertySheet* pSheet){m_pSheet=pSheet;}
	CObjPropertySheet* GetPropertySheet(){return m_pSheet;}

public:
	enum { IDD = IDD_YDFORMUIBASE_VIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	virtual void ReadonlyPage(BOOL bReadonly){}
	virtual UINT GetPropertyCount();
	virtual HRESULT GetProperty(UINT _uIndex,BSTR* _bstrName,VARIANT* _val);
	//bUpdate= True:将Obj中的属性填到控件中，bUpdate = False,将控件中的值填到Obj的属性中
	virtual HRESULT UpdateProp(BOOL bUpdate) {return S_OK;}
	virtual HRESULT Update(){return S_OK;}
	virtual HRESULT Save(){return S_OK;}
	virtual HRESULT Close(){return S_OK;}
	//ExeSpecial主要是用来在成绩查询和试卷分析时，要特殊处理
	//用来显示当前试卷的信息
	virtual HRESULT ExeSpecial(){return S_OK;}
	virtual BOOL	ValidateData(OPERATION nOp){return TRUE;}//保存或更新时要校验数据，如名称不能为空等
private:
};


