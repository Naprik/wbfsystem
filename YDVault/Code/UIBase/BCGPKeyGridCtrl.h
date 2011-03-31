#pragma once
#include "bcgpgridctrl.h"

class BASE_DLL_API CBCGPKeyGridCtrl : //增加支持上下左右移动Item的功能
	public CBCGPGridCtrl
{
public:
	CBCGPKeyGridCtrl(void);
	~CBCGPKeyGridCtrl(void);
public:
	void EnableKeyMove(BOOL _bEnableKeyMove)
	{
		m_bEnalbeKeyMove = _bEnableKeyMove;
	}
private:
	BOOL m_bEnalbeKeyMove;
	BOOL MoveItem(UINT _uDirection);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

};

