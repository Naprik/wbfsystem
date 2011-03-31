#pragma once
#include <list>

class CYdObjWrapper;
class BASE_DLL_API CListCtrlOperate
{
public:
	CListCtrlOperate(CListCtrl* _pListCtrl);
	~CListCtrlOperate(void);
	HRESULT AddItem(CYdObjWrapper* _pWrapper);
	HRESULT ModifyItem(int _index,CYdObjWrapper* _pWrapper);
	HRESULT DeleteItem(int _index);

	HRESULT ClearDataCache();
	HRESULT RemoveAllItem();
private:
	CListCtrl* m_pListCtrl;
	std::list<CYdObjWrapper*> m_lstClear;
};
