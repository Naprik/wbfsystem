#pragma once

class CDatabaseEx;
class BASE_DLL_API CPropQueryContidition
{
public:
	CPropQueryContidition(void);
	~CPropQueryContidition(void);
public:
	CString			m_strFiledName;//字段名
	UINT			m_uFieldType;//字段的类型
	QUERYOPERATE	m_uOpType;  //条件的比较，有等于，不等于和包含等
	CString			m_strConVal;//条件值

	HRESULT	CreateContidition(CString &_strContidition);//生成条件表达式
	HRESULT CreateParamter(CDatabaseEx* _pDB);
};
