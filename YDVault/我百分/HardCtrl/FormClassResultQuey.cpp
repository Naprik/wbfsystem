#include "stdafx.h"
#include "hardctrl.h"
#include "FormClassResultQuey.h"
#include "MainFrm.h"
#include "DlgClassResult.h"
#include "DlgResDetail.h"
#include "ExportToExcel.h"
#include "ListCtrlPrint.h"

#include "VcPlot.h"
#include "VcAxis.h"
#include "VcValueScale.h"
#include "VcSeriesCollection.h"
#include "VcSeries.h"
#include "VcPen.h"
#include "VcCategoryScale.h"
#include "VcColor.h"
#include "VcDataGrid.h"
#include "VcBackdrop.h"
#include "VcFill.h"
#include "VcBrush.h"
#include "VcDataPoints.h"
#include "VcDataPoint.h"
#include "VcDataPointLabel.h"
#include "VcAxisTitle.h"
#include "VcShadow.h"
#include "DBClass.h"

extern CHardCtrlApp theApp;
IMPLEMENT_DYNCREATE(CFormClassResultQuey, CDialog)


CFormClassResultQuey::CFormClassResultQuey(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormClassResultQuey::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormClassResultQuey)
	//}}AFX_DATA_INIT
}


void CFormClassResultQuey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormClassResultQuey)
	DDX_Control(pDX, IDC_QUERY, m_query);
	DDX_Control(pDX, IDC_PRINT_REPORT, m_printreport);
	DDX_Control(pDX, IDC_PRINT_CHART, m_printchart);
	DDX_Control(pDX, IDC_EXPORT_REPORT, m_exportreport);
	DDX_Control(pDX, IDC_EXPORT_CHART, m_exportchart);
	DDX_Control(pDX, IDC_TYPE, m_type);
	DDX_Control(pDX, IDC_STULIST, m_stulist);
	DDX_Control(pDX, IDC_CHART, m_chart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormClassResultQuey, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormClassResultQuey)
	ON_BN_CLICKED(IDC_QUERY, OnQuery)
	ON_NOTIFY(NM_DBLCLK, IDC_STULIST, OnDblclkStulist)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	ON_BN_CLICKED(IDC_PRINT_CHART, OnPrintChart)
	ON_BN_CLICKED(IDC_PRINT_REPORT, OnPrintReport)
	ON_BN_CLICKED(IDC_EXPORT_REPORT, OnExportReport)
	ON_BN_CLICKED(IDC_EXPORT_CHART, OnExportChart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormClassResultQuey message handlers

BOOL CFormClassResultQuey::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();
	
	AddSzControl(*GetDlgItem(IDC_CHART),mdResize,mdResize);
	AddSzControl(*GetDlgItem(IDC_TYPE),mdRepos,mdRepos);
	AddSzControl(*GetDlgItem(IDC_QUERY),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_PRINT_CHART),mdRepos,mdRepos);
	AddSzControl(*GetDlgItem(IDC_EXPORT_CHART),mdRepos,mdRepos);
	AddSzControl(*GetDlgItem(IDC_PRINT_REPORT),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_EXPORT_REPORT),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdRepos,mdRepos);
	
	AddSzControl(m_stulist,mdResize,mdResize);


	DWORD dwStyle=GetWindowLong(m_stulist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_stulist.GetSafeHwnd(),GWL_STYLE,dwStyle);  

	m_stulist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_stulist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);


    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );

 	m_stulist.ClearHeadings();
	m_stulist.SetHeadings("学号,100; 姓名,100; 科目,100;试卷名称,150;开考时间,150;收卷时间,150;正确题数,100;错误题数,100;分数,100"); 

	m_chart.SetChartType(1|0);	

	m_chart.SetTitleText("班级成绩分布图");
	m_chart.GetBackdrop().GetFill().SetStyle(1);
	m_chart.GetBackdrop().GetFill().GetBrush().GetFillColor().Set(255, 255, 255);

	m_chart.SetShowLegend(FALSE);

	VARIANT var;
	m_chart.GetPlot().GetAxis(1,var).GetValueScale().SetAuto(FALSE);	// 不自动标注Y轴刻度
	m_chart.GetPlot().GetAxis(1,var).GetValueScale().SetMaximum(100);	// Y轴最大刻度
	m_chart.GetPlot().GetAxis(1,var).GetValueScale().SetMinimum(0);		// Y轴最小刻度
	m_chart.GetPlot().GetAxis(1,var).GetValueScale().SetMajorDivision(5);	// Y轴刻度5等分
	m_chart.GetPlot().GetAxis(1,var).GetValueScale().SetMinorDivision(1);	// 每刻度一个刻度线
	m_chart.GetPlot().GetAxis(1,var).GetAxisTitle().SetText("人数");	// Y轴名称

	m_chart.SetColumnCount(1); 

	VARIANT var1;
	m_chart.GetPlot().GetAxis(0,var1).GetCategoryScale().SetAuto(FALSE);			// 不自动标注X轴刻度
	m_chart.GetPlot().GetAxis(0,var1).GetCategoryScale().SetDivisionsPerLabel(1);// 每刻度一个标注
	m_chart.GetPlot().GetAxis(0,var1).GetCategoryScale().SetDivisionsPerTick(1); // 每刻度一个刻度线
	m_chart.GetPlot().GetAxis(0,var1).GetAxisTitle().SetText("分数段");			// X轴名称

	int nRowCount = 0;
	m_chart.SetRowCount(nRowCount);

	return TRUE; 
}

void CFormClassResultQuey::OnQuery() 
{
	CDlgClassResult  dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_class = dlg.strClass;
		m_stulist.DeleteAllItems();
		CStringArray arryStuid,arryStuname;
		arryStuid.RemoveAll();
		arryStuname.RemoveAll();
		BOOL bMark = FALSE;


		CoInitialize(NULL);
		CString strClassid = "";
		CDBClass m_classdb;
		if (!m_classdb.Connect())
			AfxMessageBox("m_classdb:无法连接到数据库服务器");
		else
		{
			BOOL flag = m_classdb.LoadByClassname(m_class);
			if (flag)
			{
				strClassid = m_classdb.m_classid;
			}
			m_classdb.Close();
		}

		CDBStuInfo m_StuInfodb;
		if(!m_StuInfodb.Connect())
		{
			AfxMessageBox("StuInfodb:无法连接到数据库服务器");
		}
		else
		{
			BOOL flag = m_StuInfodb.LoadByClassid(strClassid);
			if(flag)
			{
				while(flag)
				{
					arryStuid.Add(m_StuInfodb.m_stuid);
					arryStuname.Add(m_StuInfodb.m_stuname);
					flag = m_StuInfodb.Move();
				}
				bMark = TRUE;
			}
			else
			{
				AfxMessageBox("该班级无学生信息!");
			}
			m_StuInfodb.Close();
		}
		CoUninitialize();
		if(!bMark)
			return;

		bMark = FALSE; 
		CoInitialize(NULL);
		CDBPaper m_Paperdb;
		if(!m_Paperdb.Connect() )
		{
			AfxMessageBox("Paperdb:无法连接到数据库服务器");
		}
		else
		{
			BOOL flag = m_Paperdb.LoadByPaperName(dlg.strPaper);
			if(flag)
			{
				while(flag)
				{
					m_paperid = m_Paperdb.m_ydid;
					m_startno = m_Paperdb.m_startNO;
					m_endno = m_Paperdb.m_endNO;
					flag = m_Paperdb.Move();
				}
				bMark = TRUE; 
			}
			else
			{
				AfxMessageBox("无该门试卷信息!");
			}
			m_Paperdb.Close();
		}
		CoUninitialize();
		if(!bMark)
			return;

		CoInitialize(NULL);
		CDBResult m_Resultdb;
		if(!m_Resultdb.Connect())
		{
			AfxMessageBox("Resultdb:无法连接到数据库服务器");
		}
		else
		{
			for(int i = 0; i < arryStuid.GetSize(); i++)
			{
				BOOL flag = m_Resultdb.LoadByQuery(arryStuid[i], m_paperid);
				if(flag)
				{
					int right = 0;
					int worng = 0;
					double score = 0;
					calculate(m_Resultdb.m_stuanswer, m_startno,m_endno,m_paperid,right,worng,score);
					CString sRight,sWorng,sSocrt;
					sRight.Format("%d",right);
					sWorng.Format("%d",worng);
					sSocrt.Format("%0.2f",score);						
					m_stulist.AddItem(0,arryStuid[i],arryStuname[i],
											dlg.strSubject, dlg.strPaper,
											m_Resultdb.m_starttime.Format(),
											m_Resultdb.m_endtime.Format(),
											sRight,sWorng,sSocrt);
				}
			}
			m_Resultdb.Close();
		}
		CoUninitialize();
	}

	if(m_stulist.GetItemCount() == 0)
	{
		AfxMessageBox("没有查到相应内容！");

	}
	else
	{
		VARIANT var;
		m_chart.GetPlot().GetAxis(1,var).GetValueScale().SetAuto(TRUE);	
		CString sPerpaName = m_stulist.GetItemText(0,3);
		CString temp = "";
		temp.Format("%s %s的成绩分布图", m_class, sPerpaName);
		m_chart.SetTitleText(temp);
		int n0_59, n60_69, n70_79, n80_89, n90_100;
		n0_59 = 0;
		n60_69 = 0;
		n70_79 = 0;
		n80_89 = 0;
		n90_100 = 0;
		for(int i = 0; i < m_stulist.GetItemCount(); i++)
		{
			CString sSocer = m_stulist.GetItemText(i,8);
			int nSocer = atoi(sSocer);
			if(nSocer<60)
				n0_59++;
			else if(nSocer>=60 && nSocer <70)
				n60_69++;
			else if(nSocer >= 70 && nSocer < 80)
				n70_79++;
			else if(nSocer >= 80 && nSocer < 90)
				n80_89++;
			else
				n90_100++;
		}

		int nRowCount = 5;
		m_chart.SetRowCount(nRowCount);
		for(int row = 1; row <= nRowCount; ++row)
		{
			CString temp = "";
			int data = 0;
			m_chart.SetRow(row);
			if(row == 1)
			{
				temp = "90-100";
				m_chart.SetRowLabel(temp);
				m_chart.GetDataGrid().SetData(row, 1, n90_100, 0);
			}
			else if(row == 2)
			{
				temp = "80-89";
				m_chart.SetRowLabel(temp);
				m_chart.GetDataGrid().SetData(row, 1, n80_89, 0);
			}
			else if(row == 3)
			{
				temp = "70-79";
				m_chart.SetRowLabel(temp);
				m_chart.GetDataGrid().SetData(row, 1, n70_79, 0);
			}
			else if(row == 4)
			{
				temp = "60-69";
				m_chart.SetRowLabel(temp);
				m_chart.GetDataGrid().SetData(row, 1, n60_69, 0);
			}
			else
			{
				temp = "60以下";
				m_chart.SetRowLabel(temp);
				m_chart.GetDataGrid().SetData(row, 1, n0_59, 0);
			}
		}
	}
}

BOOL CFormClassResultQuey::calculate(CString strStuanswer, int start, int end, int paperid, int &right, int &worng, double &score)
{
	BOOL ret = TRUE;
	CDBStdanswer m_Stdanswerdb;
	if(!m_Stdanswerdb.Connect())
	{
		ret = FALSE;
		AfxMessageBox("StuInfodb:无法连接到数据库服务器");
	}
	else
	{
		BOOL flag = m_Stdanswerdb.LoadByPaperID(paperid);
		if(flag)
		{
			int temp = start;
			for(temp; temp <= end; temp++)
			{
				flag = m_Stdanswerdb.LoadByQuery(paperid, temp);
				if(flag)
				{
					CString key = m_Stdanswerdb.m_res;
//					stdanswer += key;
					key.TrimLeft();
					key.TrimRight();
					CString stukey = strStuanswer.Mid(temp-start,1);
					if(stukey == key)
					{
						right++;
						score += m_Stdanswerdb.m_score;
					}
					else
						worng++;
				}
			}
		}
		else
		{
			ret = FALSE;
			AfxMessageBox("没有该门科目的标准答案，成绩将不做比对！");
		}
	}
	return ret;
}

void CFormClassResultQuey::OnDblclkStulist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item =pNMListView->iItem;	
	CString strStuNO = m_stulist.GetItemText(item,0);
	if(strStuNO != "")
	{
		CDlgResDetail  dlg;
		dlg.iPaperID = m_paperid;
		dlg.strStuid = m_stulist.GetItemText(item,0);
		dlg.strStuname = m_stulist.GetItemText(item,1);
		dlg.strSubject = m_stulist.GetItemText(item,2);
		dlg.strPaper = m_stulist.GetItemText(item,3);
		dlg.strRight = m_stulist.GetItemText(item,6);
		dlg.strWorng = m_stulist.GetItemText(item,7);
		dlg.strSocrt = m_stulist.GetItemText(item,8);
		dlg.DoModal();
	}
	*pResult = 0;
}

void CFormClassResultQuey::OnSelchangeType() 
{
	if ( m_type.GetCurSel() >= 0 )
	{
		int iType = m_type.GetCurSel();
		switch(iType)
		{
			case 0:
				m_chart.SetChartType(1|0);
				break;
			case 1:
				m_chart.SetChartType(1|2);
				break;
			case 2:
				m_chart.SetChartType(1|4);
				break;
			case 3:
				m_chart.SetChartType(1|6);
				break;
			default:
				m_chart.SetChartType(1|0);
				break;	
		}
	}
	
}

void CFormClassResultQuey::OnPrintChart() 
{
	HANDLE hDib;
	HPALETTE hPalette;
	//m_chart.GetPlot().GetBackdrop().GetFill().GetBrush().GetFillColor().Set(0, 0, 0);
	//m_chart.GetPlot().GetBackdrop().GetShadow().SetStyle(FALSE);// 阴影

	m_chart.EditCopy(); 
	if(OpenClipboard())//m_mschart.
	{
		hDib = GetClipboardData(CF_DIB);
		hPalette = (HPALETTE)GetClipboardData(CF_PALETTE);
		CloseClipboard();
	}
	else
		return;

	//m_chart.GetPlot().GetBackdrop().GetFill().GetBrush().GetFillColor().Set(192, 192, 192);
	//m_chart.GetPlot().GetBackdrop().GetShadow().SetStyle(TRUE);// 阴影

	CPrintDialog mypd(FALSE);// 构造打印设置对话框对象
	mypd.GetDefaults();  // 检取缺省打印设备的设备环境
	DEVMODE *dm = mypd.GetDevMode();// 提取缺省打印机的DEVMODE数据
	dm->dmOrientation = 2;	
// dm->dmPrintQuality = 1200;
	PRINTDLG pd;
	{
	pd.lStructSize = sizeof(PRINTDLG);
	pd.Flags = PD_RETURNDC;
	pd.hDC = NULL;
	pd.hwndOwner = NULL;
	pd.hInstance = NULL;
	pd.nMaxPage = 256;
	pd.nMinPage = 1;
	pd.nFromPage = 1;
	pd.nToPage  = 256;
	pd.nCopies = 1;
	pd.hDevMode = dm;
	pd.hDevNames = NULL;
	}

	if(!PrintDlg(&pd))
	{
		return;
	}
	ASSERT(pd.hDC != NULL);

	CDC pDC;
	pDC.Attach(pd.hDC);
	int nHorRes = GetDeviceCaps(pd.hDC, HORZRES);//返回设备的物理显示宽度mm 
	int nVerRes = GetDeviceCaps(pd.hDC, VERTRES);//返回设备的物理显示高度mm
	
	if(pd.hDC != NULL)
	{
		DOCINFO di;   
		di.cbSize = sizeof(DOCINFO);  
		di.lpszDocName = "图象打印";
		di.lpszOutput = (LPTSTR)NULL;  
		di.lpszDatatype = (LPTSTR)NULL; 
		di.fwType = 0;
		pDC.StartDoc(&di); //file:通知打印机驱动程序执行一新的打印任务  
		pDC.StartPage();   //file1://通知打印机驱动程序打印新页  
		SetMapMode(pd.hDC, MM_TEXT);//设置当前影射模式为：单位0.001英寸
		CRect rectPrint(0, 0, nHorRes, nVerRes);  

		//file2://打印图形
		BITMAPINFO* pbi = (BITMAPINFO*)GlobalLock(hDib);
		if (pbi != NULL)
		{
		BYTE* Data = (BYTE*)(pbi->bmiColors);
		if (pbi->bmiHeader.biBitCount <= 8)
		{
		int nColor = pbi->bmiHeader.biClrUsed
		 ? pbi->bmiHeader.biClrUsed : (1<<pbi->bmiHeader.biBitCount);
		Data += sizeof(RGBQUAD) *nColor;
		}
		if (hPalette != NULL)
		SelectPalette(pd.hDC, hPalette, TRUE);
		StretchDIBits(pd.hDC, rectPrint.left, rectPrint.top, rectPrint.Width(),
		rectPrint.Height(), 0, 0, pbi->bmiHeader.biWidth,
		pbi->bmiHeader.biHeight, Data, pbi, DIB_RGB_COLORS, SRCCOPY);
		GlobalUnlock(hDib);
		}

		EndPage(pd.hDC); //file3://通知打印机驱动程序页结束  
		EndDoc(pd.hDC); 
		pDC.DeleteDC();
	}		
}

void CFormClassResultQuey::OnPrintReport() 
{
	if(m_stulist.GetItemCount()<= 0)
	{
		AfxMessageBox("报表内容为空，无法打印！");
		return;
	}
	CString sPerpaName = m_stulist.GetItemText(0,3);
	CString temp = "";
	temp.Format("%s %s的成绩表", m_class, sPerpaName);
	CListCtrlPrint cJob;
	cJob.csPageHead = temp;
	cJob.csPageFooter = "";
	cJob.csPage = "第 %d 共 %d";
	cJob.pList = &m_stulist;
	cJob.OnFilePrint();	
}


void CFormClassResultQuey::OnExportReport() 
{
	CExportToExcel  ete;
	ete.ExportListToExcel(&m_stulist,"成绩分布");	
}


void CreateBMPFile(HWND   hwnd,   LPTSTR   pszFile,   PBITMAPINFO   pbi,   HBITMAP   hBMP,   HDC   hDC)
{

    HANDLE   hf; //   file   handle
    BITMAPFILEHEADER   hdr; //   bitmap   file-header
    PBITMAPINFOHEADER   pbih; //   bitmap   info-header
    LPBYTE   lpBits; //   memory   pointer
    DWORD   dwTotal; //   total   count   of   bytes
    DWORD   cb; //   incremental   count   of   bytes
    BYTE   *hp; //   byte   pointer
    DWORD   dwTmp;

    pbih =(PBITMAPINFOHEADER)pbi;
    lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

        //   Retrieve   the   color   table   (RGBQUAD   array)   and   the   bits
        //   (array   of   palette   indices)   from   the   DIB.
    if(!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS))
    {
    }

        //   Create   the   .BMP   file.
    hf = CreateFile(pszFile, GENERIC_READ | GENERIC_WRITE, (DWORD)0, NULL, CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL, (HANDLE)   NULL );
    hdr.bfType = 0x4d42;                 //   0x42   =   "B "   0x4d   =   "M "
    //   Compute   the   size   of   the   entire   file.
    hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + pbih-> biSize + pbih->biClrUsed*sizeof(RGBQUAD) + pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

    //   Compute   the   offset   to   the   array   of   color   indices.
    hdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed*sizeof(RGBQUAD);

    //   Copy   the   BITMAPFILEHEADER   into   the   .BMP   file.
    if(!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), (LPDWORD)&dwTmp, NULL))
    {
    }

    //   Copy   the   BITMAPINFOHEADER   and   RGBQUAD   array   into   the   file.
    if(!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed*sizeof(RGBQUAD), (LPDWORD)&dwTmp,   (   NULL   )   )   )
	{
	}

    //   Copy   the   array   of   color   indices   into   the   .BMP   file.
    dwTotal = cb = pbih->biSizeImage;
    hp   =   lpBits;
    if(!WriteFile(hf,(LPSTR)hp, (int)cb, (LPDWORD)&dwTmp,NULL))
	{
	}

    //   Close   the   .BMP   file.
    if(!CloseHandle(hf))
	{
	}

     //   Free   memory.
     GlobalFree((HGLOBAL)lpBits);
}

PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{

	BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD cClrBits;

    //   Retrieve   the   bitmap 's   color   format,   width,   and   height.
    if(!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
	{
	}

     //   Convert   the   color   format   to   a   count   of   bits.
    cClrBits = (WORD)(bmp.bmPlanes*bmp.bmBitsPixel);
    if(cClrBits == 1 )
        cClrBits = 1;
    else if(cClrBits <= 4)
        cClrBits = 4;
    else if(cClrBits <= 8)
        cClrBits = 8;
    else if(cClrBits <= 16)
        cClrBits = 16;
    else if(cClrBits <= 24)
        cClrBits = 24;
    else
		cClrBits = 32;

    //   Allocate   memory   for   the   BITMAPINFO   structure.   (This   structure
    //   contains   a   BITMAPINFOHEADER   structure   and   an   array   of   RGBQUAD
    //   data   structures.)
	if(cClrBits != 24)
	pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*(1 << cClrBits));

	//   There   is   no   RGBQUAD   array   for   the   24-bit-per-pixel   format.
	else
	pbmi =(PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));

    //   Initialize   the   fields   in   the   BITMAPINFO   structure.
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
    if(cClrBits < 24)
        pbmi->bmiHeader.biClrUsed = ( 1 << cClrBits );

    //   If   the   bitmap   is   not   compressed,   set   the   BI_RGB   flag.
    pbmi->bmiHeader.biCompression = BI_RGB;

    //   Compute   the   number   of   bytes   in   the   array   of   color
    //   indices   and   store   the   result   in   biSizeImage.
    pbmi->bmiHeader.biSizeImage = (pbmi-> bmiHeader.biWidth + 7)/8*pbmi->bmiHeader.biHeight*cClrBits;
//   Set   biClrImportant   to   0,   indicating   that   all   of   the
        //   device   colors   are   important.
    pbmi->bmiHeader.biClrImportant = 0;
    return pbmi;
}


void CFormClassResultQuey::OnExportChart() 
{
	CString strFilter= "BMP文件(*.bmp)|(*.bmp)|| ";
	CFileDialog m_cFileDlg(FALSE,"bmp ","chart ", OFN_OVERWRITEPROMPT,
	"Save MSChart to BMP(*.bmp)|*.bmp|| ", this);
	//获取MSCHART的图片
	CDC* pChartDC;

	//   Get   device   context   from   MSChart
	pChartDC = m_chart.GetDC();

	//   Get   dimensions   of   MSChart
	RECT   mschartRect;
	m_chart.GetClientRect(&mschartRect);
	int mschartWidth = mschartRect.right - mschartRect.left;
	int mschartHeight = mschartRect.bottom - mschartRect.top;

	//   Create   CBitmap
	CBitmap myBitmap;

    //   Create   Compatible   Bitmap   for   MSChart
	myBitmap.CreateCompatibleBitmap(pChartDC, mschartWidth, mschartHeight);

	//   Define   device-context   object
	CDC myCopy;
	myCopy.CreateCompatibleDC(pChartDC);

	//   Get   pointer   to   object   being   replaced
	myCopy.SelectObject(myBitmap);

	myCopy.BitBlt(0, 0, mschartWidth, mschartHeight, pChartDC, 0, 0, SRCCOPY);

	//   Retrieve   information   about   the   CBitmap
	BITMAP   bits;
	myBitmap.GetBitmap(&bits);

	//Open clipboard   and   empty   its   contents
	OpenClipboard();
	EmptyClipboard();

	//   Copy   our   new   MSChart   bitmap   to   clipboard   and   close   it
	SetClipboardData(CF_BITMAP, myBitmap.GetSafeHandle());
	CloseClipboard();
	// m_cMsChart.EditCopy();
	//
    if(m_cFileDlg.DoModal()==IDOK)
	{
		CString   filePath   =   m_cFileDlg.GetPathName();
		if(   !OpenClipboard()   )
			return;

		//   Is   the   object   stored   in   the   clipboard   of   type   CF_BITMAP?   If   not,   something   odd   happened
		//   and   perhaps   some   other   application   erased/replaced   the   contents   of   the   clipboard?
		if(   !IsClipboardFormatAvailable(   CF_BITMAP   )   )
		{
			AfxMessageBox(   "CF_BITMAP   not   available "   );
			CloseClipboard();
			return;
		}

		//   Create   and   copy   the   handle   to   the   BITMAP   in   the   clipboard   memory
		HBITMAP   hData   =   (HBITMAP)GetClipboardData(CF_BITMAP);

	//   Close   the   clipboard
		CloseClipboard();
	//
		PBITMAPINFO   pbi   =   CreateBitmapInfoStruct(   m_hWnd,   hData);
	//save   to   file
		CreateBMPFile(   m_hWnd,   filePath.GetBuffer(0),   pbi,   hData,   ::GetDC(   m_hWnd   )   ); 	
	}	
}
