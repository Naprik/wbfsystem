#pragma once

class CStudev;
class CDatabaseEx;
class CInvigilateInfo;
class CFormExamBlue;

//所有修改studev信息的都在下面这个类中，因为线程修改要考虑通过临界点
class CThreadFunHelper//线程中会调用下面的一些函数
{
public:
	CThreadFunHelper(void);
	~CThreadFunHelper(void);
public:
	//开考是根据_pStuDev修改列表上的状态
	//bError = True:表示开考不成功也要显示状态，否则：开考不成功不显示
	HRESULT UpdateListBeginExamByStudev(CStudev* _pStuDev,CFormExamBlue* _pForm,BOOL bError);
	//交卷时根据_pStuDev修改列表上的状态
	HRESULT UpdateListRollingByStudev(CStudev* _pStuDev,CFormExamBlue* _pForm,BOOL bError);
	//交卷时保存答案到数据库中
	HRESULT SaveStudevAnswer(CStudev* _pStuDev,CFormExamBlue* _pForm,
		CInvigilateInfo* _pInvigilateInfo,CDatabaseEx* _pDB);
	//计算已经开考的学术机数量
	HRESULT GetBeginExamStudev(CFormExamBlue* _pForm,std::list<CStudev*>& _lstStudev);

	//得到已经交卷的学生机
	HRESULT GetRollingStudev(CFormExamBlue* _pForm,std::list<CStudev*>& _lstStudev);

};
