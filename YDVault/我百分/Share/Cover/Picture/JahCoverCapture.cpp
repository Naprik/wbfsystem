#include "stdafx.h"
#include <windowsx.h>
#include "jahcovercapture.h"

//##ModelId=4140A4D303AA
CJahCoverCapture::CJahCoverCapture()
{
	m_bmpFile = ".\\current.bmp";
}
//##ModelId=4140A4D303A9
CJahCoverCapture::~CJahCoverCapture()
{
}
///////////////////////////////////////////////////////////////////
//截屏并生成BMP文件                                              //
///////////////////////////////////////////////////////////////////
//##ModelId=4140A4D303B4
bool CJahCoverCapture::GetCapture()
{
	CDC         m_dc;
	CDC         m_tdc;
	CBitmap     m_bm;
	CBitmap     *m_pOld;
	int         m_width;
	int         m_height;
	m_dc.CreateDC("DISPLAY",NULL,NULL,NULL);
	m_width=GetSystemMetrics(SM_CXSCREEN);
	m_height=GetSystemMetrics(SM_CYSCREEN);
	m_bm.CreateCompatibleBitmap(&m_dc,m_width,m_height);
	m_tdc.CreateCompatibleDC(&m_dc);
	m_pOld=m_tdc.SelectObject(&m_bm);
	m_tdc.BitBlt(0,0,m_width,m_height,&m_dc,0,0,SRCCOPY);
	m_tdc.SelectObject(m_pOld);
	BITMAP      m_btm;
	BITMAPINFOHEADER bih;
	BITMAPFILEHEADER bfh;
	m_bm.GetBitmap(&m_btm);
	DWORD size=m_btm.bmWidthBytes*m_btm.bmHeight;
	LPSTR lpData=(LPSTR)GlobalAllocPtr(GPTR,size);

	bih.biBitCount=m_btm.bmBitsPixel;
	bih.biClrImportant=0;
	bih.biClrUsed=0;
	bih.biCompression=0;
	bih.biHeight=m_btm.bmHeight;
	bih.biPlanes=1;
	bih.biSize=sizeof(BITMAPINFOHEADER);
	bih.biSizeImage=size;
	bih.biWidth=m_btm.bmWidth;
	bih.biXPelsPerMeter=0;
	bih.biYPelsPerMeter=0;

	GetDIBits(m_dc,m_bm,0,bih.biHeight,lpData,(BITMAPINFO*)&bih,DIB_RGB_COLORS);
//	bm.GetBitmapBits(size,lpData);	//此函数在处理5-5-5模式的16位色下会出现颜色混乱
	bfh.bfReserved1=bfh.bfReserved2=0;
	bfh.bfType=((WORD)('M'<< 8)|'B');
	bfh.bfSize=54+size;
	bfh.bfOffBits=54;
	CFile bf;
	if(bf.Open(m_bmpFile,CFile::modeCreate|CFile::modeWrite))
	{
		bf.WriteHuge(&bfh,sizeof(BITMAPFILEHEADER));
		bf.WriteHuge(&bih,sizeof(BITMAPINFOHEADER));
		bf.WriteHuge(lpData,size);
		bf.Close();
	}
	GlobalFreePtr(lpData);
	m_dc.DeleteDC();
	return true;
}
///////////////////////////////////////////////////////////////////
//装载图片文件                                                   //
///////////////////////////////////////////////////////////////////
//##ModelId=4140A4D303AB
bool CJahCoverCapture::LoadBMPImage(LPCTSTR sBMPFile)
{
	CFile file;
	if( !file.Open( sBMPFile, CFile::modeRead) )
		return FALSE;
	BITMAPFILEHEADER bmfHeader;
	// Read file header
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return FALSE;
	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
		return FALSE;
	// Get length of the remainder of the file and allocate memory
	DWORD nPackedDIBLen = file.GetLength() - sizeof(BITMAPFILEHEADER);
	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nPackedDIBLen);
	if (hDIB == 0)
		return FALSE;
	// Read the remainder of the bitmap file.
	if (file.ReadHuge((LPSTR)hDIB, nPackedDIBLen) != nPackedDIBLen )
	{
		::GlobalFree(hDIB);
		return FALSE;
	}
	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;
	// If bmiHeader.biClrUsed is zero we have to infer the number
    // of colors from the number of bits used to specify it.
    int nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed : 
                                    1 << bmiHeader.biBitCount;
    LPVOID lpDIBBits;
	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) + 
		((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);
	// Create the logical palette
	if( &m_Palette != NULL )
	{
		// Create the palette
		if( nColors <= 256 )
		{
			UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
			LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
			pLP->palVersion = 0x300;
			pLP->palNumEntries = nColors;
			for( int i=0; i < nColors; i++)
			{
				pLP->palPalEntry[i].peRed = bmInfo.bmiColors[i].rgbRed;
				pLP->palPalEntry[i].peGreen = bmInfo.bmiColors[i].rgbGreen;
				pLP->palPalEntry[i].peBlue = bmInfo.bmiColors[i].rgbBlue;
				pLP->palPalEntry[i].peFlags = 0;
			}
			m_Palette.CreatePalette( pLP );
			delete[] pLP;
		}
	}
	CClientDC dc(NULL);
	CPalette* pOldPalette = NULL;
	if( &m_Palette )
	{
		pOldPalette = dc.SelectPalette( &m_Palette, FALSE );
		dc.RealizePalette();
	}
	HBITMAP hBmp = CreateDIBitmap( dc.m_hDC,      // handle to device context 
                                &bmiHeader,       // pointer to bitmap size and format data 
                                CBM_INIT,         // initialization flag 
                                lpDIBBits,        // pointer to initialization data 
                                &bmInfo,          // pointer to bitmap color-format data 
                                DIB_RGB_COLORS);  // color-data usage 
    m_bitmap.Attach( hBmp );
	if( pOldPalette )
		dc.SelectPalette( pOldPalette, FALSE );
	::GlobalFree(hDIB);
	return TRUE;
}
///////////////////////////////////////////////////////////////////
//显示图片文件                                                   //
///////////////////////////////////////////////////////////////////
//##ModelId=4140A4D303AD
void CJahCoverCapture::DrawBitmap(CPaintDC *pDC, CRect &rc)
{
	// Create a compatible memory DC
	CDC memDC;
	memDC.CreateCompatibleDC( pDC );
	CBitmap* pOldBmp = memDC.SelectObject( &m_bitmap );
    // Select and realize the palette
	CPalette* pOldPal;
	if( (&m_Palette) != NULL && pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
	{
		pOldPal = pDC->SelectPalette( &m_Palette, FALSE );
		pDC->RealizePalette();
	}
    BITMAP bm;
	m_bitmap.GetBitmap( &bm );
	pDC->StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(),
		           &memDC,0, 0, bm.bmWidth, bm.bmHeight,SRCCOPY);
	// Select and realize the palette
	if( (&m_Palette) != NULL && pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
	{
		pDC->SelectPalette( pOldPal, FALSE );
	}
	memDC.SelectObject (pOldBmp);
}
