#pragma once
#include <list>
#include <map>
#include "../ObjRef/YDObjectRef.h"

class CPaperErrorResult;
class CStudev;

class BASE_DLL_API CPaperAnsysHelperResult
{
public:
	UINT	m_iRight; //正确的人数
	UINT	m_iError; //错误的人数
	std::map<CString, UINT>	m_mapAnswer; //各种答案的个数
	CString	m_strRightAnswer;//标准答案
};


class BASE_DLL_API CPaperAnsysHelper
{
public:
	CPaperAnsysHelper(void);
	~CPaperAnsysHelper(void);

	//根据数据库中的内容分析试卷
	HRESULT Ansys(OBJID paperID, 
		std::list<CYDObjectRef*>& lstMarks, 
		std::map<UINT, CPaperAnsysHelperResult*>& mapRes);

	//更加教师机中存的答案信息分析试卷内容
	HRESULT Ansys(OBJID paperID, 
		std::list<CString>& lstStrAnswer,  
		std::map<UINT, CPaperAnsysHelperResult*>& mapRes);

	HRESULT ErrorReport(std::list<CYDObjectRef*>& lstMarks,
		std::list<CPaperErrorResult*>& _lstErrorResult);

private:
	HRESULT AnsysOne(CYDObjectRef* pObjAnswer, 
		std::list<CYDObjectRef*>& lstMarks, 
		std::map<UINT, CPaperAnsysHelperResult*>& mapRes);
	HRESULT AnsysOne(CYDObjectRef* pObjAnswer, 
		std::list<CString>& lststrAnswer,  
		std::map<UINT, CPaperAnsysHelperResult*>& mapRes);
	HRESULT AnsysOne(CYDObjectRef* pObjAnswer,//标准答案
					std::map<int, CString> mapAnswer,/*题号与答案的映射*/
					std::map<UINT, CPaperAnsysHelperResult*>& mapRes/*分析的结果*/);
	HRESULT CreateAnsysHelperResultByPaperID(OBJID paperID, 
											std::list<CYDObjectRef*> &lstAnswer,
											std::map<UINT, CPaperAnsysHelperResult*>& mapRes);
};
