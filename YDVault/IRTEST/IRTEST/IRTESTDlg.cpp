
// IRTESTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IRTEST.h"
#include "IRTESTDlg.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CIRTESTDlg 对话框




CIRTESTDlg::CIRTESTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIRTESTDlg::IDD, pParent)
	//, m_port(3)
	, m_port(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIRTESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STULIST, m_stulist);
	//	DDX_Text(pDX, IDC_PORT, m_port);
	//	DDV_MinMaxInt(pDX, m_port, 1, 99);
	DDX_Control(pDX, IDC_DATALIST, m_datalist);
	DDX_Text(pDX, IDC_PORT, m_port);
}

BEGIN_MESSAGE_MAP(CIRTESTDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CIRTESTDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_OPENPORT, &CIRTESTDlg::OnBnClickedOpenport)
	ON_BN_CLICKED(IDC_START, &CIRTESTDlg::OnBnClickedStart)
	ON_WM_TIMER()
//	ON_BN_CLICKED(IDC_BUTTON1, &CIRTESTDlg::OnBnClickedButton1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_STULIST, &CIRTESTDlg::OnLvnItemchangedStulist)
	ON_BN_CLICKED(IDC_SEARCH, &CIRTESTDlg::OnBnClickedSearch)
	ON_BN_CLICKED(IDC_CLEAR_LIST, &CIRTESTDlg::OnBnClickedClearList)
	ON_BN_CLICKED(IDC_CLEAR_BOX, &CIRTESTDlg::OnBnClickedClearBox)
END_MESSAGE_MAP()


// CIRTESTDlg 消息处理程序

BOOL CIRTESTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CRect rect; 
	::GetWindowRect(m_hWnd,rect); 
	ScreenToClient(rect); 
	m_nDlgWidth = rect.right - rect.left; 
	m_nDlgHeight = rect.bottom - rect.top;
	//计算分辨率
	m_nWidth = GetSystemMetrics(SM_CXSCREEN); 
	m_nHeight = GetSystemMetrics(SM_CYSCREEN);
	//计算放大倍数
	m_Multiple_width = float(m_nWidth)/float(m_nDlgWidth); 
	m_Mutiple_heith = float(m_nHeight)/float(m_nDlgHeight); 
	change_flag=TRUE;//这个是成员变量bool形，用来判断onsize执行时oninitdlg是否已经执行了

	DWORD dwStyle=GetWindowLong(m_stulist.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_stulist.GetSafeHwnd(),GWL_STYLE,dwStyle);  

	m_stulist.SetExtendedStyle(LVS_EX_GRIDLINES);
	::SendMessage(m_stulist.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	dwStyle = m_stulist.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES;
	//m_stulist.SetExtendedStyle(dwStyle);

	::SetWindowPos(m_hWnd,HWND_TOPMOST,0, 0, 0, 0,SWP_NOSIZE | SWP_NOMOVE  );

	m_stulist.ClearHeadings();
	m_stulist.SetHeadings(_T("MAC,100; 接收次数,100; 接收答案,10000"));

	m_open = FALSE;
	m_hThread = NULL;
	m_hThread1 = NULL;
	m_recv = _T("");
	SetTimer(1,200,NULL);

	//for (int i = 1; i <= 50; i++)
	//{
	//	CDev dev;
	//	dev.m_index = i;
	//	dev.m_count = 0;
	//	dev.m_str = _T("");
	//	m_devlist.Add(dev);
	//}

	//for(int i = 0; i < m_devlist.GetSize(); i++)
	//{
	//	CString strIndex = _T("");
	//	strIndex.Format(_T("%d"), m_devlist[i].m_index);
	//	CString strIcount = _T("");
	//	strIcount.Format(_T("%d"), m_devlist[i].m_count);
	//	m_stulist.AddItem(0, strIndex, strIcount, _T(""));
	//}
	m_devlist.RemoveAll();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIRTESTDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CIRTESTDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CIRTESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CIRTESTDlg::ReSize(int nID)
{
	CRect Rect; 
	GetDlgItem(nID)->GetWindowRect(Rect); 
	ScreenToClient(Rect); 
	//计算控件左上角点 
	CPoint OldTLPoint,TLPoint; 
	OldTLPoint = Rect.TopLeft(); 
	TLPoint.x = long(OldTLPoint.x *m_Multiple_width); 
	TLPoint.y = long(OldTLPoint.y * m_Mutiple_heith );
	//计算控件右下角点
	CPoint OldBRPoint,BRPoint; 
	OldBRPoint = Rect.BottomRight(); 
	BRPoint.x = long(OldBRPoint.x *m_Multiple_width); 
	BRPoint.y = long(OldBRPoint.y * m_Mutiple_heith ); 
	//移动控件到新矩形
	Rect.SetRect(TLPoint,BRPoint); 
	GetDlgItem(nID)->MoveWindow(Rect,TRUE); 
}

void CIRTESTDlg::ReSizeEx(int nID)
{
	CRect Rect; 
	GetDlgItem(nID)->GetWindowRect(Rect); 
	ScreenToClient(Rect); 
	//计算控件左上角点 
	CPoint OldTLPoint,TLPoint; 
	OldTLPoint = Rect.TopLeft(); 
	TLPoint.x = long(OldTLPoint.x *m_Multiple_width); 
	TLPoint.y = long(OldTLPoint.y * m_Mutiple_heith );
	//计算控件右下角点
	CPoint OldBRPoint,BRPoint; 
	OldBRPoint = Rect.BottomRight(); 
	BRPoint.x = TLPoint.x + 90; 
	BRPoint.y = TLPoint.y + 30; 
	//移动控件到新矩形
	Rect.SetRect(TLPoint,BRPoint); 
	GetDlgItem(nID)->MoveWindow(Rect,TRUE); 
}

void CIRTESTDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (change_flag == TRUE)//如果确定oninitdlg已经调用完毕.
	{

		ReSize(IDC_DATALIST);
		ReSize(IDC_STULIST);
		ReSizeEx(IDOK);
		ReSizeEx(IDCANCEL);
		ReSizeEx(IDC_STATIC1);
		ReSizeEx(IDC_PORT);
		ReSizeEx(IDC_OPENPORT);
		ReSizeEx(IDC_START);
		ReSizeEx(IDC_SEARCH);
		//ReSizeEx(IDC_BUTTON1);
		ReSizeEx(IDC_CLEAR_LIST);
		ReSizeEx(IDC_CLEAR_BOX);
		m_Multiple_width = float(1)/ m_Multiple_width ; 
		m_Mutiple_heith = float(1)/m_Mutiple_heith ; 
	}
}

void CIRTESTDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CIRTESTDlg::OnBnClickedOpenport()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//if (m_port <= 0 || m_port >= 99)
	//	return;

	CString strPort = _T("");
	if (m_port.GetLength() <= 0)
	{
		AfxMessageBox(_T("请搜索设备！"));
		return;
	}
	else if (m_port.GetLength() == 4)
	{
		strPort.Format(_T("%s"), m_port);
	}
	else
	{
		strPort.Format(_T("\\\\.\\%s"), m_port);
	}

	if (!m_appcom.OpenCome(strPort))
	{
		AfxMessageBox(_T("打开串口失败！"));
		return;
	}
	else
	{
		m_open = TRUE;
		AfxMessageBox(_T("打开串口成功！"));
	}
}

DWORD WINAPI Rolling(LPVOID lpParam)
{
	CIRTESTDlg *dlg = (CIRTESTDlg *)lpParam;
	while (true)
	{
		CString strRecv = _T("");
		strRecv = dlg->m_appcom.ReadData();
		if ( strRecv != _T("") )
		{
			dlg->m_datalist.AddString(strRecv);
			dlg->m_recv += strRecv;
		}
	}
	
	return 0;
}

DWORD WINAPI Decode(LPVOID lpParam)
{
	CIRTESTDlg *dlg = (CIRTESTDlg *)lpParam;
	while(true)
	{
		dlg->DecodData();
		Sleep(10);
	}

	return 0;
}

void CIRTESTDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( !m_open )
	{
		AfxMessageBox(_T("请打开串口！"));
	}
	else
	{
		if (m_hThread == NULL)
		{
			DWORD dwThreadId1;
			m_hThread = CreateThread(NULL, 0, Rolling, this, 0, &dwThreadId1);	
		}
		if (m_hThread1 == NULL)
		{
			DWORD dwThreadId2;
			m_hThread1 = CreateThread(NULL, 0, Decode, this, 0, &dwThreadId2);	
		}
	}
}

void CIRTESTDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnTimer(nIDEvent);
}

void CIRTESTDlg::DecodData()
{
	int iData = m_recv.GetLength();
	if ( iData >= 110*3 )
	{
		if(m_recv.Mid(0,5) == _T("b5 00"))
		{
			//AfxMessageBox(m_recv);
			CString strAnswer = _T("");
			CString strMac = _T("");
			if (m_appcom.DecodeAnswer(m_recv.Mid(0, 110*3), strAnswer, strMac))
			{
				if (CheckMac(strMac))
				{
					for(int i = 0; i <= m_devlist.GetSize(); i++)
					{
						if (m_devlist[i].m_mac == strMac)
						{
							m_devlist[i].m_count = m_devlist[i].m_count+1;
							m_devlist[i].m_answer = strAnswer;
							for(int j = 0; j < m_stulist.GetItemCount(); j++)
							{
								CString strTemp = m_stulist.GetItemText(j,0);
								strMac.TrimLeft();
								strMac.TrimRight();
								if (strTemp == strMac)
								{
									CString strCont = _T("");
									strCont.Format(_T("%d"),m_devlist[i].m_count );
									m_stulist.SetItemText(j,1,strCont);
									m_stulist.SetItemText(j,2,strAnswer);

									m_stulist.EnsureVisible(j,TRUE);
									m_stulist.SetItemState(j,LVIS_SELECTED,LVIS_SELECTED);
									MessageBeep(MB_ICONASTERISK);
									//break;
								}
								else
								{
									m_stulist.SetItemState(j,0,LVIS_SELECTED);
								}
							}
							break;
						}
					}
				}
				else
				{
					CDev dev;
					dev.m_count = 1;
					dev.m_mac = strMac;
					dev.m_answer = strAnswer;
					m_devlist.Add(dev);
					m_stulist.AddItem(0, strMac, _T("1"),strAnswer);
					int iItemCount = m_stulist.GetItemCount();
					for(int i = 0; i < iItemCount - 1; i++)
					{
						m_stulist.SetItemState(i,0,LVIS_SELECTED);
					}
					m_stulist.EnsureVisible(iItemCount - 1,TRUE);
					m_stulist.SetItemState(iItemCount - 1,LVIS_SELECTED,LVIS_SELECTED);
					MessageBeep(MB_ICONASTERISK);

				}
			}
			m_recv = m_recv.Mid(108*3, m_recv.GetLength());
		}
		else
		{
			m_recv = m_recv.Mid(3, iData - 3);
		}
	}
}

BOOL CIRTESTDlg::CheckMac(CString strMac)
{
	BOOL ret = FALSE;

	for(int i = 0; i < m_devlist.GetSize(); i++)
	{
		if (m_devlist[i].m_mac == strMac)
		{
			return TRUE;
		}
	}
	return ret;
}

void CIRTESTDlg::ReflashList(int iIndex)
{
	//int iTempcount = 0;
	//CString strTemp = _T("");
	////strTemp.Format(_T("%d %d %d %d"), iIndex, iIndex, iIndex, iIndex);
	//for(int i = 0; i < m_devlist.GetSize(); i++)
	//{
	//	if (m_devlist[i].m_index == iIndex)
	//	{
	//		m_devlist[i].m_count++;
	//		CString strTemp1 = _T("");
	//		strTemp1.Format(_T("%d %d %d %d "), iIndex, iIndex, iIndex, iIndex);
	//		m_devlist[i].m_str += strTemp1;
	//		iTempcount = m_devlist[i].m_count;
	//		strTemp = m_devlist[i].m_str;
	//		break;
	//	}
	//}

	//for (int i = 0; i < m_stulist.GetItemCount(); i++)
	//{
	//	CString strIndex = m_stulist.GetItemText(i, 0);
	//	int iListIndex = _ttoi(strIndex);
	//	if (iListIndex == iIndex)
	//	{
	//		CString temp = _T("");
	//		temp.Format(_T("%d"), iTempcount);
	//		m_stulist.SetItemText(i,1,temp);
	//		m_stulist.SetItemText(i,2,strTemp);
	//		m_stulist.EnsureVisible(i,TRUE);
	//		m_stulist.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
	//		//break;
	//	}
	//	else
	//	{
	//		m_stulist.SetItemState(i,0,LVIS_SELECTED);
	//	}
	//}
}

int CIRTESTDlg::Str2Dec(CString str)
{
	BYTE in[1];
	memset(in,0,sizeof(in));
	String2Hex(str,(char *)in);	
	CString strOut = _T("");
	strOut.Format(_T("%d"),in[0]);
	
	int i = _ttoi(strOut);
	return i;
}

int CIRTESTDlg::String2Hex(CString str, char *SendOut)
{
	int hexdata,lowhexdata; 
	int hexdatalen=0;
	int len=str.GetLength();
	for(int i=0;i<len;)
	{
		TCHAR lstr,hstr=str[i];
		if(hstr==' '||hstr=='\r'||hstr=='\n')
		{
			i++;
			continue;
		}
		i++;
		if (i>=len)
			break;
		lstr=str[i];
		hexdata=ConvertHexData(hstr);
		lowhexdata=ConvertHexData(lstr);
		if((hexdata==16)||(lowhexdata==16))
			break;
		else
			hexdata=hexdata*16+lowhexdata;
		i++;
		SendOut[hexdatalen]=(char)hexdata;
		hexdatalen++;
	}
	return hexdatalen;
}
TCHAR CIRTESTDlg::ConvertHexData(TCHAR ch)
{
	if((ch>='0')&&(ch<='9'))
		return ch-0x30;
	if((ch>='A')&&(ch<='F'))
		return ch-'A'+10;
	if((ch>='a')&&(ch<='f'))
		return ch-'a'+10;
	else return(-1);
}
/*
void CIRTESTDlg::OnBnClickedButton1()
{
	//CString str1 = _T("B5 00 01 88 2A 10 33 03 04 05 06 86 08 09 06 13 0C 0D 0E 8E 10 11 12 0B 24 15 16 96 18 19 16 1B 2C 1D 1E 9E 20 21 22 23 14 45 26 A6 28 29 26 33 1C 4D 2E AE 30 31 32 2B 34 55 36 B6 38 39 36 3B 3C 5D 3E BE 40 41 42 43 44 25 86 C6 48 49 46 53 4C 2D 8E CE 50 51 52 4B 64 35 96 D6 58 59 56 5B 6C 3D 9E DE 60 61 62 63 54 ");
	//CString strOP = _T("00");

	//CString strTemp = _T("");

	//for(int i = 0; i < (str1.GetLength()/3); i++)
	//{
	//	CString strTT = str1.Mid(i*3, 2);
	//	int i1 = Str2Dec(strTT);
	//	int i2 = Str2Dec(strOP);

	//	int i3 = i1 ^ i2;
	//	strOP.Format(_T("%x"), i3);
	//	strOP = FormatNO(strOP);

	//}
	//AfxMessageBox(strOP);
	//int i1 = Str2Dec(str1);
	//int i2 = Str2Dec(str2);

	//int t3 = i1 ^ i2;

	HKEY hKey;
	
	if(::RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		_T("Hardware\\DeviceMap\\SerialComm"),
		NULL,
		KEY_READ,
		&hKey ) == ERROR_SUCCESS )
	{
		int i=0;
		TCHAR portName[256],commName[256];
		DWORD dwLong,dwSize;

		while(true)
		{
			dwLong = dwSize = sizeof(portName);
			if(::RegEnumValue( hKey,
				i,
				portName,
				&dwLong,
				NULL,
				NULL,
				(PUCHAR)commName,
				&dwSize ) == ERROR_NO_MORE_ITEMS )// 枚举串口
				break;
			i++;
			CString strPortName = portName;
			CString strCommName = commName;
			AfxMessageBox(portName);
			AfxMessageBox(commName);

		}
	}
}
*/
CString CIRTESTDlg::FormatNO(CString strNO)
{
	int iLength = strNO.GetLength();
	if(iLength == 2)
	{
		strNO = strNO+_T(" ");
	}
	else if(iLength == 1)
	{
		//if(strNO == "0")
		//	strNO = "ff ";
		//	else
		strNO = _T("0") + strNO+ _T(" ");
	}
	else 
		strNO = _T("ff ");

	return strNO;
}

void CIRTESTDlg::OnLvnItemchangedStulist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CIRTESTDlg::OnBnClickedSearch()
{
	HKEY hKey;

	if(::RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		_T("Hardware\\DeviceMap\\SerialComm"),
		NULL,
		KEY_READ,
		&hKey ) == ERROR_SUCCESS )
	{
		int i=0;
		TCHAR portName[256],commName[256];
		DWORD dwLong,dwSize;

		while(true)
		{
			dwLong = dwSize = sizeof(portName);
			if(::RegEnumValue( hKey,
				i,
				portName,
				&dwLong,
				NULL,
				NULL,
				(PUCHAR)commName,
				&dwSize ) == ERROR_NO_MORE_ITEMS )// 枚举串口
				break;
			i++;
			CString strPortName = portName;
			if(strPortName.Find(_T("Serial")) >= 0)
				m_port = commName;
		}
	}
	UpdateData(FALSE);
}

void CIRTESTDlg::OnBnClickedClearList()
{
	m_stulist.DeleteAllItems();
	m_devlist.RemoveAll();
}

void CIRTESTDlg::OnBnClickedClearBox()
{
	m_datalist.ResetContent();
}
