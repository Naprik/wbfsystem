#pragma once
class CYDObjectRef;
class CYDLinkRef;

enum OBJWRAPPERSTATE
{
	S_UNKNOW = 0,
	S_NEED_UPDATE_LINK = 1,
	S_NEED_UPDATE_OBJECT = 2,
	S_NEED_UPDATE_BOTH = 3,//1&2
	S_NEED_SAVE_LINK = 4,
	S_NEED_SAVE_OBJECT = 8,
	S_NEED_SAVE_BOTH = 12,
	S_NEED_DELETE_LINK = 16,
	S_NEED_DELETE_OBJECT = 32,
	S_NEED_DELETE_BOTH = 48
};

class BASE_DLL_API CYdObjWrapper//对YD对象的包装
{
public:
	CYdObjWrapper(void);
	~CYdObjWrapper(void);
	virtual HRESULT Save();
	virtual HRESULT Update();
	virtual HRESULT Remove();

	virtual HRESULT Persist();
public:
	CYDObjectRef* m_pObjRef;
	CYDLinkRef*   m_pLinkRef;
	CWnd*		  m_pRelatedWnd;//主要指的是关联的树
	HTREEITEM     m_hRelatedItem;
	OBJWRAPPERSTATE		m_state;
};
