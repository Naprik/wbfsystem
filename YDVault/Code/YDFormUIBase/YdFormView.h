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
	//bUpdate= True:��Obj�е�������ؼ��У�bUpdate = False,���ؼ��е�ֵ�Obj��������
	virtual HRESULT UpdateProp(BOOL bUpdate) {return S_OK;}
	virtual HRESULT Update(){return S_OK;}
	virtual HRESULT Save(){return S_OK;}
	virtual HRESULT Close(){return S_OK;}
	//ExeSpecial��Ҫ�������ڳɼ���ѯ���Ծ����ʱ��Ҫ���⴦��
	//������ʾ��ǰ�Ծ����Ϣ
	virtual HRESULT ExeSpecial(){return S_OK;}
	virtual BOOL	ValidateData(OPERATION nOp){return TRUE;}//��������ʱҪУ�����ݣ������Ʋ���Ϊ�յ�
private:
};


