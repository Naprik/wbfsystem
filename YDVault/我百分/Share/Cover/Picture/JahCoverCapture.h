#pragma once
///***********************************************************
//                            屏幕截取类 
//************************************************************
//##ModelId=4140A4D3038B
class CJahCoverCapture  
{
public:
	//##ModelId=4140A4D303B4
	bool     GetCapture();	                      //截屏并生成BMP文件 
	//##ModelId=4140A4D303AD
	void     DrawBitmap(CPaintDC* pDC, CRect& rc);//装载图片文件        
	//##ModelId=4140A4D303AB
	bool     LoadBMPImage( LPCTSTR sBMPFile );    //显示图片文件      
	//##ModelId=4140A4D303AA
	CJahCoverCapture();
	//##ModelId=4140A4D303A9
	~CJahCoverCapture();
public:
	//##ModelId=4140A4D303A4
	CString     m_bmpFile;
	//##ModelId=4140A4D303A1
	CBitmap     m_bitmap;
private:
	//##ModelId=4140A4D30397
	CPalette    m_Palette ;
};
