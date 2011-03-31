#include "FileGraph.h"
//#include "Answer.h"

class CAPPGraph  
{
public:
	CAPPGraph();
	virtual ~CAPPGraph();

	void  ResultRange(CStringArray &arry, CFileGraph  &m_graphData);
	void  Wrongstat(int startNO, int endNO, CString strStdanwer,CStringArray &arry, CFileGraph  &m_graphData);
	void  Boxscore(CString strStuname, CStringArray &strPapername, CStringArray &score, CFileGraph  &m_graphData);

private:
//	void sortarray(CArray<CAnswer, CAnswer>  &arryAnswer);
};
