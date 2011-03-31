#include "stdafx.h"
#include "hardctrl.h"
#include "APPGraph.h"
//#include "Answer.h"

CAPPGraph::CAPPGraph()
{

}

CAPPGraph::~CAPPGraph()
{

}
void  CAPPGraph::ResultRange(CStringArray &arry, CFileGraph  &m_graphData)
{
	int iRang1,	iRang2,iRang3,iRang4,iRang5;
	iRang1 = 0;
	iRang2 = 0;
	iRang3 = 0;
	iRang4 = 0;
	iRang5 = 0;
	for(int i = 0; i < arry.GetSize(); i ++)
	{
		CString strPerres = arry[i];
		int iRes = atoi(strPerres);
		int temp = iRes/10;
		switch(temp)
		{
		case 10:
			iRang1++;
			break;
		case 9:
			iRang1++;
			break;
		case 8:
			iRang2++;
			break;
		case 7:
			iRang3++;
			break;
		case 6:
			iRang4++;
			break;
		default:
			iRang5++;
			break;
		}
	}
	m_graphData.m_textTitle = "班级成绩分布图  人数-分数";
	m_graphData.m_textX = "分数段";
	m_graphData.m_textY = "人数  单位：人";
	CString testx1 = "90-100";
	CString testx2 = "80-89";
	CString testx3 = "70-79";
	CString testx4 = "60-68";
	CString testx5 = "60以下";
	m_graphData.m_xList.Add(testx1);
	m_graphData.m_xList.Add(testx2);
	m_graphData.m_xList.Add(testx3);
	m_graphData.m_xList.Add(testx4);
	m_graphData.m_xList.Add(testx5);
	m_graphData.m_yList[0].Add(iRang1);
	m_graphData.m_yList[1].Add(iRang2);
	m_graphData.m_yList[2].Add(iRang3);
	m_graphData.m_yList[3].Add(iRang4);
	m_graphData.m_yList[4].Add(iRang5);
	m_graphData.m_pos[0] = 0;
	m_graphData.m_pos[1] = arry.GetSize();
	m_graphData.m_pos[2] = arry.GetSize()/10+1;
}
void CAPPGraph::Wrongstat(int startNO, int endNO, CString strStdanwer,CStringArray &arry, CFileGraph  &m_graphData)
{
/*	CArray<CAnswer, CAnswer>  arryAnswer; 	
	int i;
	for(i = 0; i < strStdanwer.GetLength(); i++)
	{
		CAnswer  an;
		an.m_no = startNO+i;
		an.m_count = 0;
		arryAnswer.Add(an);
	}
	
//	i = 0;
	for(i = 0; i<arry.GetSize(); i++)
	{
		for(int j = 0; j < strStdanwer.GetLength(); j++)
		{
			if(strStdanwer.Mid(j,1) != arry[i].Mid(j,1))
				arryAnswer[j].m_count++;	
		}
	}
	m_graphData.m_textTitle = "错误答案分布图  人数-题号";
	m_graphData.m_textX = "题号";
	m_graphData.m_textY = "人数  单位：人";	
	
	
	sortarray(arryAnswer);	
	int iCount = 0;
	if(arryAnswer.GetSize() > 20)
		iCount = 20;
	else
		iCount = arryAnswer.GetSize();

	for(i = 0; i < iCount; i++)
	{
		CString strNo;
		strNo.Format("%d", arryAnswer[i].m_no);
		m_graphData.m_xList.Add(strNo);
		m_graphData.m_yList[i].Add(arryAnswer[i].m_count);
	}
	m_graphData.m_pos[0] = 0;
	m_graphData.m_pos[1] = arry.GetSize();
	m_graphData.m_pos[2] = arry.GetSize()/10+1;*/
}

void CAPPGraph::Boxscore(CString strStuname, CStringArray &strPapername, CStringArray &score, CFileGraph  &m_graphData)
{
	m_graphData.m_textTitle = strStuname+"的成绩分布图  试卷-分数";
	m_graphData.m_textX = "试卷";
	m_graphData.m_textY = "分数  单位：";	
	for(int i = 0; i < strPapername.GetSize(); i++)
	{
	//	if(score[i] != "")
	//	{
			CString temp1 = strPapername[i];
			m_graphData.m_xList.Add(strPapername[i]);
			int temp = (int)atof(score[i]);

			m_graphData.m_yList[i].Add(temp);
	//	}
	}
	m_graphData.m_pos[0] = 0;
	m_graphData.m_pos[1] = 100;
	m_graphData.m_pos[2] = 5;
}

//void CAPPGraph::sortarray(CArray<CAnswer, CAnswer>  &arryAnswer)
//{
/*	for(int i = 0; i <arryAnswer.GetSize(); i++)
	{
		for(int j = 0; j < arryAnswer.GetSize() - i; j++)
		{
			if(arryAnswer[j].m_count < arryAnswer[j+1].m_count)
			{
				CAnswer temp;
				temp.m_no = arryAnswer[j].m_no;
				temp.m_count = arryAnswer[j].m_count;
				arryAnswer[j].m_count = arryAnswer[j+1].m_count;
				arryAnswer[j].m_no = arryAnswer[j+1].m_no;
				arryAnswer[j+1].m_count = temp.m_count;
				arryAnswer[j+1].m_no = temp.m_no;
			}
		}
	}*/
//}