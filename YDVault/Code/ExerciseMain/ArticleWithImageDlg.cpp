// ArticleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExerciseMain.h"
#include "ArticleWithImageDlg.h"
#include "../ObjRef/YDArticleQuestionRef.h"
#include "../Base/AutoClean.h"


// CArticleDlg dialog

IMPLEMENT_DYNAMIC(CArticleWithImageDlg, CQuestionDlg)

CArticleWithImageDlg::CArticleWithImageDlg(CWnd* pParent /*=NULL*/)
	: CQuestionDlg(CArticleWithImageDlg::IDD, pParent)

{
	m_pBMPBuffer = NULL;
	m_hPhotoBitmap = NULL;
}

CArticleWithImageDlg::~CArticleWithImageDlg()
{
	delete m_pBMPBuffer;
	m_pBMPBuffer = NULL;
}

void CArticleWithImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CQuestionDlg::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_STATIC1, m_st1);
	DDX_Control(pDX, IDC_SCROLLBAR2, m_vbar);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_hbar);
}


BEGIN_MESSAGE_MAP(CArticleWithImageDlg, CQuestionDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CArticleDlg message handlers

BOOL CArticleWithImageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Area.Create(CDlgQuestionArea::IDD, this);
	CRect rect, rectSelf;
	CWnd* pTmpWnd = GetDlgItem(IDC_STATIC_QUESTION);
	pTmpWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_Area.GetWindowRect(&rectSelf);
	m_rect.left = rect.left;
	m_rect.top = rect.top;
	m_rect.right = rect.left + rectSelf.Width();
	m_rect.bottom = rect.top + rectSelf.Height();
	m_Area.MoveWindow(rect.left, rect.top, rectSelf.Width(), rectSelf.Height());
	m_Area.ShowWindow(SW_SHOW);
	CClientDC dc(this);
	m_dcMem.CreateCompatibleDC( &dc );
	m_vbar.ShowWindow(false);
	m_hbar.ShowWindow(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CArticleWithImageDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
 	if (bShow)
 	{
		HRESULT hr = E_FAIL;
		CComVariant valPhoto;
		hr = m_pRecord->m_pQuestion->GetPropVal(FIELD_ARTICLEQUESTION_IMAGETITLE, &valPhoto);
		if(valPhoto.vt == (VT_ARRAY | VT_UI1))
		{
			//说明有图像的数据
			delete m_pBMPBuffer;
			m_dwFileLen = valPhoto.parray->rgsabound[0].cElements;
			if(m_pBMPBuffer = new char[m_dwFileLen+1])
			{
				char *pBuf = NULL;
				SafeArrayAccessData(valPhoto.parray,(void **)&pBuf);
				memcpy(m_pBMPBuffer,pBuf,m_dwFileLen);				///复制数据到缓冲区m_pBMPBuffer
				SafeArrayUnaccessData (valPhoto.parray);
				if (m_hPhotoBitmap != NULL)
					DeleteObject(m_hPhotoBitmap);
				m_hPhotoBitmap = BufferToHBITMAP();	
				sourcex=sourcey=0;//
	
				m_st1.GetClientRect( &rectStaticClient );
				rectStaticClient.NormalizeRect();
				m_size.cx=rectStaticClient.Size().cx;
				m_size.cy=rectStaticClient.Size().cy;
				m_size.cx = rectStaticClient.Width();    // zero based
				m_size.cy = rectStaticClient.Height();    // zero based
				m_st1.ClientToScreen( &rectStaticClient );
				ScreenToClient( &rectStaticClient);
        
				m_pt.x = rectStaticClient.left;
				m_pt.y = rectStaticClient.top;
				GetObject( m_hPhotoBitmap , sizeof(BITMAP), &m_bmInfo );
				(HBITMAP)SelectObject(m_dcMem, m_hPhotoBitmap );
				offsetx= m_pt.x;
				offsety=m_pt.y;
				m_vbar.MoveWindow(offsetx+m_size.cx,offsety,18,m_size.cy);
				m_hbar.MoveWindow(offsetx,offsety+m_size.cy,m_size.cx,18);
				horz.cbSize = sizeof(SCROLLINFO);
				horz.fMask = SIF_ALL;
				horz.nMin = 0;
				horz.nMax = m_bmInfo.bmWidth-m_size.cx;
				horz.nPage =0;
				horz.nPos = 0;
				horz.nTrackPos=0;
				if(m_bmInfo.bmWidth<=m_size.cx)
				{
					if((m_size.cx-m_bmInfo.bmWidth)==0)
					{
						offsetx= m_pt.x;
					}
					else
					{
						offsetx= m_pt.x+((m_size.cx-m_bmInfo.bmWidth)/2);
					}
					m_vbar.MoveWindow(offsetx+m_bmInfo.bmWidth,offsety,18,m_size.cy);
					m_hbar.ShowWindow(false);
				}
				else
				{
					m_hbar.ShowWindow(true);
				}
				m_hbar.SetScrollInfo(&horz);
				vert.cbSize = sizeof(SCROLLINFO);
				vert.fMask = SIF_ALL;
				vert.nMin = 0;
				vert.nMax = m_bmInfo.bmHeight-(m_size.cy);
				vert.nPage = 0;
				vert.nTrackPos=0;
				if(m_bmInfo.bmHeight<=m_size.cy)
				{
					if((m_size.cy-m_bmInfo.bmHeight)==0)
					{
						offsety= m_pt.y;
					}
					else
					{
						offsety= m_pt.y+((m_size.cy-m_bmInfo.bmHeight)/2);
					}
					m_hbar.MoveWindow(offsetx,offsety+m_bmInfo.bmHeight,m_size.cx,18);
					m_vbar.ShowWindow(false);
				}
				else
				{
					m_vbar.ShowWindow(true);
				}
				m_vbar.SetScrollInfo(&vert);

				InvalidateRect(&rectStaticClient);
			}
		}	
		
 		m_Area.m_pRecord = m_pRecord;
		m_Area.MoveWindow(m_rect, TRUE);
		//m_Area.ShowWindow(SW_SHOW);
		m_Area.ShowQuestion();
		
// 		UpdateData(FALSE);
 	}
}

HRESULT CArticleWithImageDlg::PersistData()
{
	m_pRecord->m_listUserAnswers.clear();
	m_Area.GetStuAnswers(&(m_pRecord->m_listUserAnswers));	
	
	return S_OK;
}
void CArticleWithImageDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		//dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{	
		CPaintDC dc(this);
		dc.BitBlt(offsetx,offsety,m_size.cx,m_size.cy, 
                   &m_dcMem, sourcex, sourcey,SRCCOPY);
		 erase=false;
		CDialog::OnPaint();
		
	}
}

void CArticleWithImageDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nSBCode)
	{
		case SB_TOP:
		sourcey = 0;
		break;
		case SB_BOTTOM:
		sourcey = INT_MAX;
		break;
		case SB_THUMBTRACK:
		sourcey = nPos;
		break;
	}

//ScrollWindow(0,-(x-xOrig));
	m_vbar.SetScrollPos(sourcey);
	erase=true;
	InvalidateRect(&rectStaticClient);
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CArticleWithImageDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nSBCode)
	{
		case SB_TOP:
		sourcex = 0;
		break;
		case SB_BOTTOM:
		sourcex = INT_MAX;
		break;
		case SB_THUMBTRACK:
		sourcex= nPos;
		break;
	}	
	m_hbar.SetScrollPos(sourcex);
	//Invalidate();
	erase=true;
	InvalidateRect(&rectStaticClient);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CArticleWithImageDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	if(erase)
		return false;
	else
	return CDialog::OnEraseBkgnd(pDC);
}

HBITMAP CArticleWithImageDlg::BufferToHBITMAP()
{
	HBITMAP				hBmp;
	LPSTR				hDIB,lpBuffer = m_pBMPBuffer;
	LPVOID				lpDIBBits;
	BITMAPFILEHEADER	bmfHeader;
	DWORD				bmfHeaderLen;

	bmfHeaderLen = sizeof(bmfHeader);
	strncpy((LPSTR)&bmfHeader,(LPSTR)lpBuffer,bmfHeaderLen);
	//	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B')) return NULL;
	if (bmfHeader.bfType != (*(WORD*)"BM")) return NULL;//我copy《Windows程序设计》上的做法。
	hDIB = lpBuffer + bmfHeaderLen;
	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;
	/*	int nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed : 1 << bmiHeader.biBitCount; 
	if( bmInfo.bmiHeader.biBitCount > 8 )
	lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) + 
	((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
	lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);
	*///原来的代码。
	lpDIBBits=(lpBuffer)+((BITMAPFILEHEADER *)lpBuffer)->bfOffBits;//这行功能和上面被注释掉的代码相同，容易理解。
	CClientDC dc(this);
	hBmp = CreateDIBitmap(dc.m_hDC,&bmiHeader,CBM_INIT,lpDIBBits,&bmInfo,DIB_RGB_COLORS);
			
	return hBmp;
}