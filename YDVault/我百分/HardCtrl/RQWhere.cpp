#include "stdafx.h"
#include "RQWhere.h"

CRQWhere::CRQWhere()
{
}
CRQWhere::CRQWhere(const CString& field)
{
	Add(field);
}
CRQWhere::CRQWhere(TCHAR* field)
{
	Add(field);	
}
CRQWhere::CRQWhere(const CRQWhere& a)	
{
	this->operator=(a); 
}
CRQWhere& CRQWhere::operator=(const CRQWhere& a)
{
	if(&a != this)
	{
		m_sWhere.RemoveAll();
		m_sWhere.Append(a.m_sWhere);
		m_nOp.RemoveAll();
		m_nOp.Append(a.m_nOp);
	}
	return *this;
}
CRQWhere& CRQWhere::operator&(const CRQWhere& a)	
{
	return this->operator&=(a); 
}
CRQWhere& CRQWhere::operator|(const CRQWhere& a)	
{
	return this->operator|=(a); 
}
CRQWhere& CRQWhere::operator&=(const CRQWhere& a)
{
	if(m_sWhere.GetSize() == 0)
		return this->operator=(a);
	if(m_sWhere.GetSize() > 1)
	{
		m_sWhere.InsertAt(0, CString(_T("("))); m_nOp.InsertAt(0, cwAndGroup);
		m_nOp[m_nOp.GetSize()-1] = cwNone;	// remove it, its the last in its group
		m_sWhere.Add(CString(_T(")"))); m_nOp.Add(cwAnd);
		m_sWhere.Add(CString(_T("("))); m_nOp.Add(cwAndGroup);
		m_sWhere.Append(a.m_sWhere);
		m_nOp.Append(a.m_nOp);
		m_nOp[m_nOp.GetSize()-1] = cwNone;	// remove it, its the last in its group
		m_sWhere.Add(CString(_T(")"))); m_nOp.Add(cwAndGroup);
	}
	else if(m_sWhere.GetSize() == 1)
	{
		m_nOp[0] = cwAnd;
		m_sWhere.Append(a.m_sWhere);
		m_nOp.Append(a.m_nOp);
	}
	return *this;
}
CRQWhere& CRQWhere::operator|=(const CRQWhere& a)
{
	if(m_sWhere.GetSize() == 0)
		return this->operator=(a);
	if(m_sWhere.GetSize() > 1)
	{
		m_sWhere.InsertAt(0, CString(_T("("))); m_nOp.InsertAt(0, cwOrGroup);
		m_nOp[m_nOp.GetSize()-1] = cwNone;	// remove it, its the last in its group
		m_sWhere.Add(CString(_T(")"))); m_nOp.Add(cwOr);
		m_sWhere.Add(CString(_T("("))); m_nOp.Add(cwOrGroup);
		m_sWhere.Append(a.m_sWhere);
		m_nOp.Append(a.m_nOp);
		m_nOp[m_nOp.GetSize()-1] = cwNone;	// remove it, its the last in its group
		m_sWhere.Add(CString(_T(")"))); m_nOp.Add(cwOrGroup);
	}
	else if(m_sWhere.GetSize() == 1)
	{
		m_nOp[0] = cwOr;
		m_sWhere.Append(a.m_sWhere);
		m_nOp.Append(a.m_nOp);
	}
	return *this;
}
CRQWhere::~CRQWhere()
{
}
void CRQWhere::Add(const TCHAR* field)	
{	
	CString w(_T("("));
	w += field;
	w += _T(" IS NULL");
	w += _T(")");
	m_sWhere.Add(w);
	m_nOp.Add(cwAnd);
}
void CRQWhere::Add(const TCHAR* field, const _variant_t& vt, const TCHAR* exp)
{
	CString w(_T("("));
	w += field;
	w += _T(" ");
	w += exp;		// to be on  the save side we enclose it in spaces
	w += _T(" ");	
	if(vt.vt == VT_BSTR)
	{
#if defined(_UNICODE) || defined(UNICODE)
		w += _T("N'");
#else
		w += _T("'");
#endif
		w += _bstr_t(vt).operator const TCHAR*();
		w += _T("'");
	}
	else
	{
		try
		{
			_variant_t vtx(vt);	vtx.ChangeType(VT_BSTR);
			w += _bstr_t(vtx).operator const TCHAR*();
		}
		catch(...)
		{
			w += _T("<conversion failed>");
		}
	}
	w += _T(")");
	m_sWhere.Add(w);
	m_nOp.Add(cwAnd);
}
void CRQWhere::AddBetween(const TCHAR* field, const _variant_t& vt1, const _variant_t& vt2)
{
	CString w(_T("("));
	w += field;
	w += _T(" BETWEEN ");
	if(vt1.vt == VT_BSTR)
	{
#if defined(_UNICODE) || defined(UNICODE)
		w += _T("N'");
#else
		w += _T("'");
#endif
		w += _bstr_t(vt1).operator const TCHAR*();
		w += _T("'");
	}
	else
	{
		try
		{
			_variant_t vtx(vt1);
			vtx.ChangeType(VT_BSTR);
			w += _bstr_t(vtx).operator const TCHAR*();
		}
		catch(...)	
		{
			w += _T("<conversion failed>");
		}
	}
	w+= _T(" AND ");
	if(vt2.vt == VT_BSTR)
	{
#if defined(_UNICODE) || defined(UNICODE)
		w += _T("N'");
#else
		w += _T("'");
#endif
		w += _bstr_t(vt2).operator const TCHAR*();
		w += _T("'");
	}
	else
	{
		try
		{
			_variant_t vtx(vt2); 
			vtx.ChangeType(VT_BSTR); 
			w += _bstr_t(vtx).operator const TCHAR*();
		}
		catch(...)
		{
			w += _T("<conversion failed>");
		}
	}		
	w += _T(")");
	m_sWhere.Add(w);
	m_nOp.Add(cwAnd);
}
CString CRQWhere::GetClausePlain()
{
	int and = m_sWhere.GetSize()-1;
	if(and < 0)
		return CString();
	CString whereclause;
	for(int i = 0; i <= and; i++)
	{
		whereclause += m_sWhere[i];
		if(i < and)
		{
			switch(m_nOp[i]) 
			{
			case cwAnd:
				whereclause += _T(" AND ");
				break;
			case cwOr:
				whereclause += _T(" OR ");
				break;
			case cwAndGroup:
				break;
			case cwOrGroup:
				break;
			default:
				break;
			}
		}
	}
	whereclause += _T(" ");
	return whereclause;
}
CString CRQWhere::GetClause()
{
	return CString(_T(" WHERE ")) + GetClausePlain();
}

_variant_t CRQWhere::GetLong(DWORD val)
{
	return _variant_t(long  (val));
}

_variant_t CRQWhere::GetString(CString str,BOOL isLike)
{
	CString tmp = "";
	if ( isLike )
    {
		tmp +="%";
	    tmp +=str;
		tmp +="%";
	}
	else
		tmp = str;
	return _variant_t(CString (tmp));
}
