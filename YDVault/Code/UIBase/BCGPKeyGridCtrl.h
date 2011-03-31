#pragma once
#include "bcgpgridctrl.h"

class BASE_DLL_API CBCGPKeyGridCtrl : //����֧�����������ƶ�Item�Ĺ���
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

