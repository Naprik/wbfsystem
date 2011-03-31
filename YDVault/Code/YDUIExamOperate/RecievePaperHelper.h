#pragma once
//收卷的一些辅助
class BASE_DLL_API CRecievePaperHelper
{
public:
	CRecievePaperHelper(void);
	~CRecievePaperHelper(void);
public:
	HRESULT PrepareRecievePaper();//收卷的预处理
};
