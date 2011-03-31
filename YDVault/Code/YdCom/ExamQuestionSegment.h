 #pragma once
// XX XX XX 第一段题目段数设置 
// 第一个XX代表第一段题号的开始题号，
// 第二个XX代表第一段题号的结束题号。
// 第三个XX：高4位代表第一段的题目是否多选例如：0000 代表多选 0001 代表单选。
// 低四位代码选项的个数例如：0011 代表三个选项ABC  1111 代表15个选项A-O的题目。

class AFX_EXT_CLASS CExamQuestionSegment //题目段数的设置
{
public:
	CExamQuestionSegment(void);
	~CExamQuestionSegment(void);
public:
	int		m_iStart;//开始题号
	int     m_iEnd;  //结束题号
	int     m_iMultiType; // 0 :代表多选 1:代表单选
	int		m_iOptionNum;//选项的个数
	HRESULT CreateRollcallCmd(CString &_strCmd);//生成点名的信息
};
