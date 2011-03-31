/*****************************************************************************
类名：    CRQWhere
说明：    生成SQL语句的类
帮助：
CRQWhere w("col", 100, ">") 表示 col > 100
CRQWhere w("col", "text")   表示 col = 'test'
CRQWhere w
w.Add("col")                表示col IS NULL
w.AddBetween("col",1, 100)  表示col BETWEEN 1 AND 100
w.Add("col", "a%", "LIKE")  表示col LIKE 'a%'
CWhere w1,w2
w1  &  w2                   表示 w1 AND w2
w1  |  w2		            表示 w1 OR  w2
w1 &=  w2 	    	        表示 w1 = w1 AND w2
w1 |=  w2                   表示 w1 = w1 OR W2 
******************************************************************************/
#pragma once
#include <comdef.h>

class CRQWhere
{
private:
	enum
	{ 
		cwNone, cwOr, cwAnd, cwOrGroup, cwAndGroup
	};
public://构造
	CRQWhere();
	CRQWhere(const CString& field);
	CRQWhere(TCHAR* field) ;
	CRQWhere(const CRQWhere& a)	; 
	template <class T> CRQWhere(const TCHAR* field, T vt, const TCHAR* exp = _T(" = ")){_variant_t v((T)vt); Add(field, v, exp); };
    ~CRQWhere();
public://属性
	CRQWhere& operator=(const CRQWhere& a);
	CRQWhere& operator&(const CRQWhere& a);
	CRQWhere& operator|(const CRQWhere& a);
	CRQWhere& operator&=(const CRQWhere& a);
	CRQWhere& operator|=(const CRQWhere& a);
public://方法
	static    _variant_t GetString(CString str,BOOL isLike= FALSE);
	static    _variant_t GetLong(DWORD val);
	void      Add(const TCHAR* field);
	void      Add(const TCHAR* field, const _variant_t& vt, const TCHAR* exp=_T("="));
    void      AddBetween(const TCHAR* field, const _variant_t& vt1, const _variant_t& vt2);
	CString   GetClausePlain();
	CString   GetClause();
private:
	CStringArray	m_sWhere;
	CWordArray		m_nOp;
};
