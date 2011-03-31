// FormPaperAnaly.cpp : implementation file
//

#include "stdafx.h"
#include "hardctrl.h"
#include "FormPaperAnaly.h"
#include "MainFrm.h"
#include "DlgPaperAnaly.h"
#include "ListCtrlPrint.h"
#include "ExportToExcel.h"

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
#include "VcWall.h"
#include "DBClass.h"
extern CHardCtrlApp theApp;
IMPLEMENT_DYNCREATE(CFormPaperAnaly, CDialog)

CFormPaperAnaly::CFormPaperAnaly(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(CFormPaperAnaly::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormPaperAnaly)
	//}}AFX_DATA_INIT
}


void CFormPaperAnaly::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormPaperAnaly)
	DDX_Control(pDX, IDC_QUERY, m_query);
	DDX_Control(pDX, IDC_PRINTREPORT, m_printreport);
	DDX_Control(pDX, IDC_PRINT_CHART, m_printchart);
	DDX_Control(pDX, IDC_EXPORTREPORT, m_exportreport);
	DDX_Control(pDX, IDC_EXPORT_CHART, m_exportchart);
	DDX_Control(pDX, IDC_TYPE, m_type);
	DDX_Control(pDX, IDC_PAPERLIST, m_paperlist);
	DDX_Control(pDX, IDC_CHART, m_chart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormPaperAnaly, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CFormPaperAnaly)
	ON_BN_CLICKED(IDC_QUERY, OnQuery)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelchangeType)
	ON_BN_CLICKED(IDC_PRINT_CHART, OnPrintChart)
	ON_BN_CLICKED(IDC_PRINTREPORT, OnPrintreport)
	ON_BN_CLICKED(IDC_EXPORTREPORT, OnExportreport)
	ON_BN_CLICKED(IDC_EXPORT_CHART, OnExportChart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormPaperAnaly message handlers

BOOL CFormPaperAnaly::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();

	AddSzControl(*GetDlgItem(IDC_CHART),mdResize,mdResize);
	AddSzControl(*GetDlgItem(IDC_TYPE),mdRepos,mdRepos);
	AddSzControl(*GetDlgItem(IDC_QUERY),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_PRINT_CHART),mdRepos,mdRepos);
	AddSzControl(*GetDlgItem(IDC_EXPORT_CHART),mdRepos,mdRepos);
	AddSzControl(*GetDlgItem(IDC_PRINTREPORT),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_EXPORTREPORT),mdNone,mdRepos);
	AddSzControl(*GetDlgItem(IDC_STATIC1),mdRepos,mdRepos);
	
	AddSzControl(m_paperlist,mdResize,mdResize);



    DWORD dwStyle=GetWindowLong(m_paperlist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_paperlist.GetSafeHwnd(),GWL_STYLE,dwStyle);  
    
	m_paperlist.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_paperlist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
     LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_paperlist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
    m_paperlist.SetExtendedStyle(dwStyle);

    ::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );
	m_paperlist.SetHeadings("题号,100;正确人数,100;错误人数,100;正确率,100");

	m_stdanswer = "";

	m_chart.SetChartType(1|0);	

	m_chart.SetTitleText("答题对错分布图");
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
	m_chart.GetPlot().GetAxis(0,var1).GetAxisTitle().SetText("题号");			// X轴名称

	int nRowCount = 0;
	m_chart.SetRowCount(nRowCount);

	return TRUE;  
}

void CFormPaperAnaly::OnQuery() 
{
	BOOL ret = FALSE;
	CDlgPaperAnaly dlg;
	CArray<CAnswer, CAnswer>  arryAnswer;
	arryAnswer.RemoveAll();
	if(dlg.DoModal() == IDOK)
	{
		GetDlgItem(IDC_PAPERNAME)->SetWindowText(dlg.strPaper);
		m_papername = dlg.strPaper;
		CString sClass = "";
		for(int i = 0; i < dlg.arryClass.GetSize(); i++)
		{
			sClass += dlg.arryClass[i] + " ";
		}
		GetDlgItem(IDC_CLASS)->SetWindowText(sClass);

		CString sPapername = dlg.strPaper;
		CoInitialize(NULL);
		CDBPaper m_Paperdb;
		if(!m_Paperdb.Connect() )
		{
			AfxMessageBox("Paperdb:无法连接到数据库服务器");
		}
		else
		{
			BOOL flag = m_Paperdb.LoadByPaperName(sPapername);
			if(flag)
			{
				while(flag)
				{
					m_paperid = m_Paperdb.m_ydid;
					m_startno = m_Paperdb.m_startNO;
					m_endno = m_Paperdb.m_endNO;
					m_maxno = m_Paperdb.m_maxNO;
					flag = m_Paperdb.Move();
				}
				//bMark = TRUE; 
			}
			else
			{
				AfxMessageBox("无该门试卷信息!");
			}
			m_Paperdb.Close();
		}
 
		CDBStdanswer m_Stdanswerdb;
		if(!m_Stdanswerdb.Connect())
		{
			ret = FALSE;
			AfxMessageBox("StuInfodb:无法连接到数据库服务器");
		}
		else
		{
			BOOL flag = m_Stdanswerdb.LoadByPaperID(m_paperid);
			if(flag)
			{
				m_stdanswer = "";
				int temp = m_startno;
				for(temp; temp <= m_endno; temp++)
				{
					flag = m_Stdanswerdb.LoadByQuery(m_paperid, temp);
					if(flag)
					{
						CString key = m_Stdanswerdb.m_res;
						m_stdanswer += key;
					}
				}
				ret = TRUE;
			}
			else
			{
				ret = FALSE;
				AfxMessageBox("没有该门科目的标准答案，成绩将不做比对！");
			}			
			m_Stdanswerdb.Close();
		}
		CoUninitialize();
		
		if(!ret)
			return;

		ret = FALSE;

		CStringArray  arryStu;
		arryStu.RemoveAll();
		
		for(i = 0; i < m_stdanswer.GetLength(); i++)
		{
			CAnswer  an;
			an.m_no = m_startno+i;
			an.m_worng = 0;
			an.m_right = 0;
			arryAnswer.Add(an);
		}

		CoInitialize(NULL);
		CDBStuInfo m_StuInfodb;
		if(!m_StuInfodb.Connect())
		{
			AfxMessageBox("StuInfodb:无法连接到数据库服务器");
		}
		else
		{
			CDBClass m_classdb;
			if (!m_classdb.Connect())
				AfxMessageBox("StuInfodb:无法连接到数据库服务器");
			else
			{
				for(int j = 0; j < dlg.arryClass.GetSize(); j++)
				{
					CString strClass = dlg.arryClass[j];
					BOOL classflag = m_classdb.LoadByClassname(strClass);
					if (classflag)
					{
						CString strClassid = m_classdb.m_classid;
						BOOL stuinfoflag = m_StuInfodb.LoadByClassid(strClassid);
						while (stuinfoflag)
						{
							arryStu.Add(m_StuInfodb.m_stuid);
							stuinfoflag = m_StuInfodb.Move();
						}

					}
				}
				m_classdb.Close();
			}
			m_StuInfodb.Close();
		}
 		
		CDBResult m_Resultdb;
		if(!m_Resultdb.Connect())
		{
			AfxMessageBox("Resultdb:无法连接到数据库服务器");
		}
		else
		{
			for(int i = 0; i < arryStu.GetSize(); i++)
			{
				BOOL flag = m_Resultdb.LoadByQuery(arryStu[i],m_paperid);
				if(flag)
				{
					CString stuanwer = m_Resultdb.m_stuanswer;
					for(int j = 0; j < m_stdanswer.GetLength(); j++)
					{
						if(m_stdanswer.Mid(j,1) == stuanwer.Mid(j,1))
						{
							arryAnswer[j].m_right += 1;
						}
						else
						{
							arryAnswer[j].m_worng += 1;
						}
					}
				}
			}
			m_Resultdb.Close();
		}		
		CoUninitialize();
	}

	sortarray(arryAnswer);
	m_paperlist.DeleteAllItems();
	for(int i = 0; i < arryAnswer.GetSize(); i++)
	{
		CString no, worng, right, rate;
		no.Format("%d", arryAnswer[i].m_no);
		worng.Format("%d", arryAnswer[i].m_worng);
		right.Format("%d", arryAnswer[i].m_right);
		float score = (float)(arryAnswer[i].m_right)/(float)(arryAnswer[i].m_worng+arryAnswer[i].m_right);
		score = score*100;
		rate.Format("%0.2f",score);
		m_paperlist.AddItem(0, no, right, worng , rate);
		
	}

	VARIANT var;
	m_chart.GetPlot().GetAxis(1,var).GetValueScale().SetAuto(TRUE);	
	m_chart.GetPlot().GetWall().GetBrush().SetStyle(FALSE);

	CFont*       p_Font;
    p_Font = new CFont;               //生成字体对象
    p_Font->CreatePointFont( 130, _T("黑体") ); //创建字体
	m_chart.SetFont(p_Font, FALSE);

	int nRowCount;
	if(arryAnswer.GetSize() <= 20)
		nRowCount = arryAnswer.GetSize();
	else
		nRowCount = 20;
	m_chart.SetRowCount(nRowCount);	
	for(int row = 1; row <= nRowCount; ++row)
	{
		m_chart.SetRow(row);
		CString temp1 = "";
		temp1.Format("%d题", arryAnswer[row-1].m_no);
		m_chart.SetRowLabel(temp1);
		m_chart.GetDataGrid().SetData(row, 1, arryAnswer[row-1].m_worng, 0);
	}
}

void CFormPaperAnaly::sortarray(CArray<CAnswer, CAnswer>  &arryAnswer)
{
	for(int i = 0; i <arryAnswer.GetSize(); i++)
	{
		for(int j = 0; j < arryAnswer.GetSize() - i; j++)
		{
			if(arryAnswer[j].m_worng < arryAnswer[j+1].m_worng)
			{
				CAnswer temp;
				temp.m_no = arryAnswer[j].m_no;
				temp.m_worng = arryAnswer[j].m_worng;
				temp.m_right = arryAnswer[j].m_right;
				arryAnswer[j].m_worng = arryAnswer[j+1].m_worng;
				arryAnswer[j].m_no = arryAnswer[j+1].m_no;
				arryAnswer[j].m_right = arryAnswer[j+1].m_right;
				arryAnswer[j+1].m_worng = temp.m_worng;
				arryAnswer[j+1].m_no = temp.m_no;
				arryAnswer[j+1].m_right = temp.m_right;
			}
		}
	}
}

void CFormPaperAnaly::OnSelchangeType() 
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

void CFormPaperAnaly::OnPrintChart() 
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

void CFormPaperAnaly::OnPrintreport() 
{
	if(m_paperlist.GetItemCount()<= 0)
	{
		AfxMessageBox("报表内容为空，无法打印！");
		return;
	}
	CString temp = "";
	temp.Format("%s 答题分析", m_papername);
	CListCtrlPrint cJob;
	cJob.csPageHead = temp;
	cJob.csPageFooter = "";
	cJob.csPage = "第 %d 共 %d";
	cJob.pList = &m_paperlist;
	cJob.OnFilePrint();		
}

void CFormPaperAnaly::OnExportreport() 
{
	CExportToExcel  ete;
	CString temp = "";
	temp.Format("%s答题分析", m_papername);
	ete.ExportListToExcel(&m_paperlist,temp);
}

void CFormPaperAnaly::CreateBMPFile(HWND   hwnd,   LPTSTR   pszFile,   PBITMAPINFO   pbi,   HBITMAP   hBMP,   HDC   hDC)
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

PBITMAPINFO CFormPaperAnaly::CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
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



void CFormPaperAnaly::OnExportChart() 
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
