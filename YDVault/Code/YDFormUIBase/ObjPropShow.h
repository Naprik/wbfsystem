#pragma once
enum OBJ_PROP_SHOW_TYPE
{
	SHOW_OBJECT = 1, //根据对象显示
	SHOW_STRING = 2  //根据字符串显示
};
class CYdObjWrapper;
class CObjPropertySheet;
class CObjPropShow
{
public:
	CObjPropShow(void);
	~CObjPropShow(void);
public:
	OBJ_PROP_SHOW_TYPE				m_type;
	CYdObjWrapper*					m_pObjWrapper;
	CString							m_strName;
	CObjPropertySheet*				m_pSheet;
	OPERATION						m_nOp;
};
