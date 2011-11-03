#pragma once
#include <functional>

struct CStringCompareNoCase : public std::binary_function<CString, CString, bool> 
{
	bool operator()(CString cstrdest, CString cstrsrc) const
	{
		cstrdest.MakeUpper();
		cstrsrc.MakeUpper();
		return cstrsrc < cstrdest;
	}
};