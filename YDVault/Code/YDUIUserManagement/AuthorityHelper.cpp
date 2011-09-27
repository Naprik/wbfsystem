#include "StdAfx.h"
#include "AuthorityHelper.h"
#include "../ObjRef\YDUserRef.h"
#include "StaticYdUser.h"
#include "../\Base\DataHandler.h"

CAuthorityHelper::CAuthorityHelper(void)
{
}

CAuthorityHelper::~CAuthorityHelper(void)
{
}

//根据权限值判断权限
HRESULT CAuthorityHelper::ConvertByVal(long _lAuthority,
					 BOOL& _bVault,/*试题库管理*/
					 BOOL& _bPaper,/*试卷管理*/
					 BOOL& _bOperate,/*考试操作*/
					 BOOL& _bBlue,/*蓝牙*/
					 BOOL& _bRedOut,/*红外*/
					 BOOL& _bDataMaintain/*数据维护*/,
					 BOOL& _bLogin)
{
	HRESULT hr = E_FAIL;
	if((1 << SECURITY_VAULT) & _lAuthority )
	{
		_bVault = TRUE;
	}
	else
	{
		_bVault = FALSE;
	}
	if((1 << SECURITY_PAPER) & _lAuthority)
	{
		_bPaper = TRUE;
	}
	else
	{
		_bPaper = FALSE;
	}
	if((1 << SECURITY_OPERATE) & _lAuthority)
	{
		_bOperate = TRUE;
	}
	else
	{
		_bOperate = FALSE;
	}
	if((1 << SECURITY_BLUE_TOOTH) & _lAuthority)
	{
		_bBlue = TRUE;
	}
	else
	{
		_bBlue = FALSE;
	}
	if((1 << SECURITY_REDOUT) & _lAuthority)
	{
		_bRedOut = TRUE;
	}
	else
	{
		_bRedOut = FALSE;
	}
	if((1 << SECURITY_DATAMAINTAIN) & _lAuthority)
	{
		_bDataMaintain = TRUE;
	}
	else
	{
		_bDataMaintain = FALSE;
	}
	if((1 << SECURITY_LOGIN) & _lAuthority)
	{
		_bLogin = TRUE;
	}
	else
	{
		_bLogin = FALSE;
	}
	return S_OK;
}
//生成权限值
HRESULT CAuthorityHelper::CreateVal(BOOL _bVault,/*试题库管理*/
				  BOOL _bPaper,/*试卷管理*/
				  BOOL _bOperate,/*考试操作*/
				  BOOL& _bBlue,/*蓝牙*/
				  BOOL& _bRedOut,/*红外*/
				  BOOL _bDataMaintain/*数据维护*/,
				  BOOL& _bLogin,
				  long& _lAuthority)
{
	HRESULT hr = E_FAIL;
	_lAuthority = 0;
	if(_bVault)
	{
		_lAuthority |= (1<<SECURITY_VAULT);
	}
	if(_bPaper)
	{
		_lAuthority |= (1<<SECURITY_PAPER);
	}
	if(_bOperate)
	{
		_lAuthority |= (1<<SECURITY_OPERATE);
	}
	if(_bBlue)
	{
		_lAuthority |= (1<<SECURITY_BLUE_TOOTH);
	}
	if(_bRedOut)
	{
		_lAuthority |= (1<<SECURITY_REDOUT);
	}
	if(_bDataMaintain)
	{
		_lAuthority |= (1<<SECURITY_DATAMAINTAIN);
	}
	if(_bLogin)
	{
		_lAuthority |= (1<<SECURITY_LOGIN);
	}
	return S_OK;
}

//根据权限值生成字符串，显示给用户看的
HRESULT CAuthorityHelper::ConvertString(long _lAuthority,
					  CString& _strVal)
{
	HRESULT hr = E_FAIL;
	_strVal = _T("");
	BOOL bVault = FALSE;
	BOOL bPaper = FALSE;
	BOOL bOperate = FALSE;
	BOOL bBlueTooth = FALSE;
	BOOL bRedOut = FALSE;
	BOOL bDataMaintain = FALSE;
	BOOL bLogin = FALSE;
	
	hr = ConvertByVal(_lAuthority,bVault,bPaper,
						bOperate,
						bBlueTooth,bRedOut, 
						bDataMaintain, bLogin);
	if(FAILED(hr))
	{
		return hr;
	}
	if(bVault)
	{
		_strVal += _T("试题库管理;");
	}
	if(bPaper)
	{
		_strVal += _T("试卷管理;");
	}
	if(bOperate)
	{
		_strVal += _T("考试操作;");
	}
	if(bBlueTooth)
	{
		_strVal += _T("蓝牙;");
	}
	if(bRedOut)
	{
		_strVal += _T("红外;");
	}
	if(bDataMaintain)
	{
		_strVal += _T("数据维护;");
	}
	if(bLogin)
	{
		_strVal += _T("登录试题库");
	}
	return S_OK;
}

HRESULT CAuthorityHelper::CheckSecurity(SECURITY_TYPE _type,VARIANT_BOOL* _bRight)
{
	HRESULT hr = E_FAIL;
	*_bRight = VARIANT_FALSE;
	CYDUserRef* pCurUserRef = NULL;
	hr = CStaticYdUser::Instance()->GetCurUser(pCurUserRef);
	if(FAILED(hr))
	{
		return hr;
	}
	ASSERT(pCurUserRef);
	hr = pCurUserRef->Restore(TRUE);
	if(FAILED(hr))
	{
		return hr;
	}
	VARIANT_BOOL bAdmin = VARIANT_FALSE;
	hr = pCurUserRef->IsSysUser(&bAdmin);
	if(FAILED(hr))
	{
		return hr;
	}
	if (bAdmin)
	{
		*_bRight = TRUE;
	}
	else
	{
		CComVariant valAuthority;
		hr = pCurUserRef->GetPropVal(FIELD_YDUSER_AUTHORITY,&valAuthority);
		if(FAILED(hr))
		{
			return hr;
		}
		long lAuthority = CDataHandler::VariantToLong(valAuthority);
		if((1 << _type) & lAuthority)
		{
			*_bRight = TRUE;
		}
	}
	return S_OK;
}