#pragma once
#include <list>
class AFX_EXT_CLASS CSearchDevCom	//通过注册表读取当前教师机或学生机的连接端口
{
public:
	CSearchDevCom(void);
	~CSearchDevCom(void);
public:
	//读取注册表得到所有的连接Com的机器
	HRESULT SearchDev(std::list<CString> &_lstCom);
};
