#pragma once

class BASE_DLL_API CAuthorityHelper
{
public:
	CAuthorityHelper(void);
	~CAuthorityHelper(void);
public:
	//根据权限值判断权限
 	HRESULT ConvertByVal(long _lAuthority,
 						BOOL& _bVault,/*试题库管理*/
						BOOL& _bPaper,/*试卷管理*/
						BOOL& _bOperate,/*考试操作*/
						BOOL& _bBlue,/*蓝牙*/
						BOOL& _bRedOut,/*红外*/
						BOOL& _bDataMaintain/*数据维护*/,
						BOOL& _bLogin/*登录*/);
	//生成权限值
	HRESULT CreateVal(BOOL _bVault,/*试题库管理*/
		BOOL _bPaper,/*试卷管理*/
		BOOL _bOperate,/*考试操作*/
		BOOL& _bBlue,/*蓝牙*/
		BOOL& _bRedOut,/*红外*/
		BOOL _bDataMaintain/*数据维护*/,
		BOOL& _bLogin,
		long& _lAuthority);
	//根据权限值生成字符串，显示给用户看的
	HRESULT ConvertString(long _lAuthority,
						  CString& _strVal);
	//判断当前用户具有操作_type的权限
	HRESULT CheckSecurity(SECURITY_TYPE _type,VARIANT_BOOL* _bRight);
};
