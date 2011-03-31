/*****************************************************************************
������    CRQWhere
˵����    ����SQL������
������
CRQWhere w("col", 100, ">") ��ʾ col > 100
CRQWhere w("col", "text")   ��ʾ col = 'test'
CRQWhere w
w.Add("col")                ��ʾcol IS NULL
w.AddBetween("col",1, 100)  ��ʾcol BETWEEN 1 AND 100
w.Add("col", "a%", "LIKE")  ��ʾcol LIKE 'a%'
CWhere w1,w2
w1  &  w2                   ��ʾ w1 AND w2
w1  |  w2		            ��ʾ w1 OR  w2
w1 &=  w2 	    	        ��ʾ w1 = w1 AND w2
w1 |=  w2                   ��ʾ w1 = w1 OR W2 
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
public://����
	CRQWhere();
	CRQWhere(const CString& field);
	CRQWhere(TCHAR* field) ;
	CRQWhere(const CRQWhere& a)	; 
	template <class T> CRQWhere(const TCHAR* field, T vt, const TCHAR* exp = _T(" = ")){_variant_t v((T)vt); Add(field, v, exp); };
    ~CRQWhere();
public://����
	CRQWhere& operator=(const CRQWhere& a);
	CRQWhere& operator&(const CRQWhere& a);
	CRQWhere& operator|(const CRQWhere& a);
	CRQWhere& operator&=(const CRQWhere& a);
	CRQWhere& operator|=(const CRQWhere& a);
public://����
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
