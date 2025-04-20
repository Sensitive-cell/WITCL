// DIB.cpp: implementation of the DIB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DIB.h"
#include"math.h"
#define WIDTHBYTES(bits)  ((bits+31)/32*4)
#define RECTWIDTH(x) (x->right-x->left)
#define RECTHEIGHT(x) (x->bottom-x->top)
#define THRESHOLDCONTRAST  40
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define PI 3.1415926
extern int locax,locay;
#define m_WIDTH 600
#define m_HEIGHT 600

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////
HDIB DIB::ReadDIBFile(HANDLE hFile)
{
	BITMAPFILEHEADER bmfHeader;
	DWORD dwBitsSize;
	HANDLE hDIB;
	HANDLE hDIBtmp;
	LPBITMAPINFOHEADER lpbi;
	DWORD dwRead;
    //�õ��ļ���С
	dwBitsSize = GetFileSize(hFile,NULL);
	hDIB =  GlobalAlloc(GMEM_MOVEABLE,(DWORD)(sizeof(BITMAPINFOHEADER)));

	if(!hDIB)
		return NULL;

	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
	if(!lpbi)
	{
		GlobalFree(hDIB);
		return NULL;
	}
	
	if(!ReadFile(hFile,(LPBYTE)&bmfHeader,sizeof(BITMAPFILEHEADER),&dwRead,NULL))
		goto ErrExit;
	if(sizeof(BITMAPFILEHEADER)!=dwRead)//��ȡ�ļ�����
		goto ErrExit;
	if(bmfHeader.bfType != 0x4d42)//�ļ����Ͳ�ƥ��
		goto ErrExit;
	if(!ReadFile(hFile,(LPBYTE)lpbi,sizeof(BITMAPINFOHEADER),&dwRead,NULL))
		goto ErrExit;
	if(sizeof(BITMAPINFOHEADER)!= dwRead)//��ȡ���ݳ���
		goto ErrExit;
	
	GlobalUnlock(hDIB);
	if(lpbi->biSizeImage==0)
		lpbi->biSizeImage = (this->BytePerLine(hDIB))*lpbi->biHeight;
	hDIBtmp = GlobalReAlloc(hDIB,lpbi->biSize+lpbi->biSizeImage,0);
	if(!hDIBtmp)
		goto ErrExitNoUnlock;
	else
		hDIB = hDIBtmp;
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
	//��������趨�ļ�ָ��
	if(bmfHeader.bfOffBits != 0L)
		SetFilePointer(hFile,bmfHeader.bfOffBits,NULL,FILE_BEGIN);
    //��ȡ�ļ���������ɫ����
	if(ReadFile(hFile,(LPBYTE)lpbi+lpbi->biSize,lpbi->biSizeImage,&dwRead,NULL))
			goto OKExit;
	
	ErrExit:
		GlobalUnlock(hDIB);
	
	ErrExitNoUnlock:
		GlobalFree(hDIB); //�ͷ��ڴ�
		return NULL;

	OKExit:
		GlobalUnlock(hDIB);
		return hDIB;
		
}

HDIB DIB::LoadDIB(LPCTSTR lpFileName)
{
	HANDLE hDIB;
	HANDLE hFile;
	//�����ļ����
	if((hFile = CreateFile(lpFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,NULL))!= INVALID_HANDLE_VALUE)
	{  
		//��ȡ����
		hDIB = ReadDIBFile(hFile);
		//�ر��ļ����
		CloseHandle(hFile);
		return hDIB;
	}
	return NULL;
}

BOOL DIB::PaintDIBTrue(HDC hDC,LPRECT lpDCRect,HANDLE hDIB,LPRECT lpDIBRect ,DWORD dwRop)
{
	LPBYTE lpDIBHdr;
	LPBYTE lpDIBBits;
	BOOL bSuccess = FALSE;

	if(!hDIB)
		return FALSE;
	lpDIBHdr = (LPBYTE)GlobalLock(hDIB);
	lpDIBBits = lpDIBHdr + sizeof(BITMAPINFOHEADER);
	bSuccess = StretchDIBits(hDC,lpDCRect->left,
								 lpDCRect->top,
								 RECTWIDTH(lpDCRect),
								 RECTHEIGHT(lpDCRect),
								 lpDIBRect->left,
								 ((LPBITMAPINFOHEADER)lpDIBHdr)->biHeight-lpDIBRect->top-RECTHEIGHT(lpDIBRect),
								 RECTWIDTH(lpDIBRect),
								 RECTHEIGHT(lpDIBRect),
								 lpDIBBits,
								 (LPBITMAPINFO)lpDIBHdr,
								 DIB_RGB_COLORS,
								 SRCCOPY);
	GlobalUnlock(hDIB);
	return bSuccess;
}

WORD DIB::BytePerLine(HANDLE hDIB)
{	
	WORD i;
	LPBITMAPINFOHEADER lpbi;
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
	i = WIDTHBYTES((lpbi->biWidth)*24);
	GlobalUnlock(hDIB);
	return i;	
}


//����ʵ��ͼƬ�Ӳ�ɫ���ڰ׵�ת��
HDIB DIB::ToGray(HANDLE hDIB)
{
	HDIB hNewDIB = NULL;
	LPBITMAPINFOHEADER lpSrc,lpDest;
	LPBYTE lpS,lpD;
	DWORD dwBytesPerLine;
	DWORD dwImgSize;
	WORD wBytesPerLine;
	unsigned i ,j,height,width;
	if(!hDIB)
		return NULL;
	
	lpSrc = (LPBITMAPINFOHEADER)GlobalLock(hDIB);

	dwBytesPerLine = WIDTHBYTES(24*(lpSrc->biWidth));
	dwImgSize = lpSrc->biHeight * dwBytesPerLine;
	//�����µ��ڴ棬��С����ԭ��ͼ��Ĵ�С
	hNewDIB = GlobalAlloc(GHND,sizeof(BITMAPINFOHEADER)+dwImgSize);

	lpDest = (LPBITMAPINFOHEADER)GlobalLock(hNewDIB);
	//����ͼƬ�ĳ�����ɫ��ȵ���Ϣ
	memcpy((void*)lpDest,(void*)lpSrc,sizeof(BITMAPINFOHEADER));
	DWORD dwSBytesPerLine;
	dwSBytesPerLine = (24*(lpSrc->biWidth)+31)/32*4;
	height = lpDest->biHeight;
	width = lpDest->biWidth;
	lpS = (LPBYTE)lpSrc;
	wBytesPerLine = this->BytePerLine(hDIB);
	lpD = (LPBYTE)lpDest;	
	lpS = lpS + sizeof(BITMAPINFOHEADER);
	lpD = lpD + sizeof(BITMAPINFOHEADER);
	unsigned  r , g ,b,gray ;
	//ɨ������ͼƬ��ʵ�ֻҶȻ�
	for(i = 0 ;i<height; i++)
	{
		for(j = 0 ;j<(unsigned )lpDest->biWidth;j++)
		{
	        //���ԭ��ͼƬ����ɫֵ
			r = *(lpS++);
			g = *(lpS++);
			b  = *(lpS++);
			//����Ҷ�ֵ
			gray = (g*50+r*39+b*11)/100;
			//����Ҷ�ֵ��Ŀ��ͼƬ
			*(lpD++)=gray;
			*(lpD++) = gray;
			*(lpD++) = gray;

			
		}
		//�������ֽڶ�������
	unsigned  k ;
		for(k=0;k<dwSBytesPerLine-lpSrc->biWidth*3;k++)
		{
			lpS++;
			lpD++;
		}
		
	}

	GlobalUnlock(hDIB);
	GlobalUnlock(hNewDIB);
   	return hNewDIB;
	
	

}



LPBYTE  DIB::FindDIBBits(HANDLE hDIB)
{
	LPBYTE lpDIB,lpDIBtmp;
	LPBITMAPINFOHEADER lpbi;
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
	lpDIBtmp = (LPBYTE)lpbi;
	lpDIB = lpDIBtmp + sizeof(BITMAPINFOHEADER);
	GlobalUnlock(hDIB);
	return lpDIB;
}

long DIB::PixelOffset(int i,int j,WORD wBytePerLine)
{
	long   Offset;
	Offset = i*wBytePerLine + j*3;
	return Offset;
}





int DIB::BOUND(int a ,int b ,int rgb)
{
	if(rgb<0)
		return BOUND(a,b,abs(rgb));
	if(rgb>b)
		return b;
	return rgb;
}
//����ʵ�ֶ�ͼƬ����ֵ�˲�����
void DIB::MedianFilterDIB(HANDLE hDIB)
{
	
	HDIB hNewDIB;
	LPBYTE  lpS,lpD;
	LPBITMAPINFOHEADER lpbi;
	int r,g,b;
	DWORD width,height;
	WORD wBytesPerLine;
	long lOffset;
	if(!hDIB)
		return;
	SetCursor(LoadCursor(NULL,IDC_WAIT));
	wBytesPerLine = this->BytePerLine(hDIB);
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
	width = lpbi->biWidth;
	height = lpbi->biHeight;
	 //����һ���µ��ڴ�
	hNewDIB = GlobalAlloc(GHND,sizeof(BITMAPINFOHEADER)+lpbi->biSizeImage);
	lpD = (LPBYTE)GlobalLock(hNewDIB);
	lpS = (LPBYTE)lpbi;
	memcpy(lpD,lpS,sizeof(BITMAPINFOHEADER));
	lpS = lpS+sizeof(BITMAPINFOHEADER);
	lpD = lpD+sizeof(BITMAPINFOHEADER);
	//ɨ������ͼƬ
	for(WORD i=1;i<height-1;i++)
		for(WORD j=1;j<width-1;j++)
		{	//��ֵ�˲�
			this->DoMedianFilterDIB(&r,&g,&b,i,j,wBytesPerLine,lpS);
			lOffset = this->PixelOffset(i,j,wBytesPerLine);
			*(lpD+lOffset++) = r;
			*(lpD+lOffset++) = g;
			*(lpD+lOffset)   = b;

		}
		//����Ϣ������ԭ���ľ��
		for( i = 1;i<height-1;i++)
			for(WORD j = 1;j<width-1;j++)
			{
				lOffset = this->PixelOffset(i,j,wBytesPerLine);
				//�����ͼƬ����ɫ
				int color1 = *(lpD+lOffset++);
				int color2 = *(lpD+lOffset++);
				int color3 = *(lpD+lOffset++);
				//������ԭ��ͼƬ��
						*(lpS+lOffset++) = color1;
						*(lpS+lOffset++) = color2;
						*(lpS+lOffset++) = color3;
			}
	SetCursor(LoadCursor(NULL,IDC_ARROW));
	GlobalUnlock(hDIB);
	GlobalUnlock(hNewDIB);
	GlobalFree(hNewDIB);
	
}



void DIB::DoMedianFilterDIB(int *r,int *g,int*b,WORD i,WORD j,WORD wBytesPerLine,LPBYTE lpDIBBits)
{
	long lOffset;
	int rgb[9][3];
	//�Ѱ˸���λ�͵�ǰλ�õ�rgbֵ���浽����rgb[9][3]��
	lOffset = this->PixelOffset( i-1, j-1, wBytesPerLine); 
	rgb[0][0] = *(lpDIBBits+lOffset++);
	rgb[0][1] = *(lpDIBBits+lOffset++);
	rgb[0][2] = *(lpDIBBits+lOffset++);

	lOffset = this->PixelOffset( i-1, j, wBytesPerLine);
	rgb[1][0] = *(lpDIBBits+lOffset++);
	rgb[1][1] = *(lpDIBBits+lOffset++);
	rgb[1][2] = *(lpDIBBits+lOffset++);
	lOffset = this->PixelOffset( i-1, j+1, wBytesPerLine);
	rgb[2][0] = *(lpDIBBits+lOffset++);
	rgb[2][1] = *(lpDIBBits+lOffset++);
	rgb[2][2] = *(lpDIBBits+lOffset++);
	lOffset = this->PixelOffset( i, j-1, wBytesPerLine);
	rgb[3][0] = *(lpDIBBits+lOffset++);
	rgb[3][1] = *(lpDIBBits+lOffset++);
	rgb[3][2] = *(lpDIBBits+lOffset++);
	lOffset = this->PixelOffset( i, j, wBytesPerLine);
	rgb[4][0] = *(lpDIBBits+lOffset++);
	rgb[4][1] = *(lpDIBBits+lOffset++);
	rgb[4][2] = *(lpDIBBits+lOffset++);
	lOffset = this->PixelOffset( i, j+1, wBytesPerLine);
	rgb[5][0] = *(lpDIBBits+lOffset++);
	rgb[5][1] = *(lpDIBBits+lOffset++);
	rgb[5][2] = *(lpDIBBits+lOffset++);
	lOffset = this->PixelOffset( i+1, j-1, wBytesPerLine);
	rgb[6][0] = *(lpDIBBits+lOffset++);
	rgb[6][1] = *(lpDIBBits+lOffset++);
	rgb[6][2] = *(lpDIBBits+lOffset++);
	lOffset = this->PixelOffset( i+1, j, wBytesPerLine);
	rgb[7][0] = *(lpDIBBits+lOffset++);
	rgb[7][1] = *(lpDIBBits+lOffset++);
	rgb[7][2] = *(lpDIBBits+lOffset++);
	lOffset = this->PixelOffset( i+1, j+1, wBytesPerLine);
	rgb[8][0] = *(lpDIBBits+lOffset++);
	rgb[8][1] = *(lpDIBBits+lOffset++);
	rgb[8][2] = *(lpDIBBits+lOffset++);
	 this->MedianSearch( rgb,9,b,g,r);
	

}


int DIB::MedianSearch(int a[9][3],int n,int *r,int *g,int *b)
{
	int result,i,j,temp;
	//ð�ݷ������ҳ�rgb�м�ֵ
	for(i=0;i<9;i++)
		for(j = 0 ;j<n-i-1;j++)
		{	
			if(a[j][0]>a[j+1][0])
			{
				temp = a[j][0];
				a[j][0] = a[j+1][0];
				a[j+1][0]=temp;
			}
			if(a[j][1]>a[j+1][1])
			{
				temp = a[j][1];
				a[j][1] = a[j+1][1];
				a[j+1][1]=temp;
			}

			if(a[j][2]>a[j+1][2])
			{
				temp = a[j][2];
				a[j][2] = a[j+1][2];
				a[j+1][2]=temp;
			}


		}
		//�õ�rgb���м�ֵ
		*b = a[5][0];
		*g = a[5][1];
		*r = a[5][2];
		//������ҪҲ����ѡ�ûҶ�ֵ
		result = (a[5][0]*39 +a[5][1]*50 + a[5][2]*11)/100;
		return result;
}
//ʵ��ͼƬ�ĺڰ׶�ֵ��
void DIB::WhiteBlack(HANDLE hDIB,unsigned n)
{

	LPBITMAPINFOHEADER  lpbi;
	LPBYTE				lpS;
	int					width,height;
	long				lOffset;
	WORD                wBytesPerLine;

	if(!hDIB)
		return ;
	wBytesPerLine = this->BytePerLine(hDIB);
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
    //�õ�ͼƬ�ĳ�����Ϣ
	width = lpbi->biWidth;
	height = lpbi->biHeight;

	lpS = (LPBYTE)lpbi;
	//lpsָ��������
	lpS = lpS + sizeof(BITMAPINFOHEADER);
  	//ɨ������ͼƬ��ʵ�ֶ�ֵ��
	for(int i = 0;i<height;i++)
		for(int j = 0 ;j<width;j++)
		{   //�õ������������������е�ƫ��
			lOffset = this->PixelOffset(i,j,wBytesPerLine);
			if(*(lpS+lOffset)<n)//����ֵС���ٽ�ֵ
			{   //���������Ϊ��ɫ
				*(lpS+lOffset++) = 0;
				*(lpS+lOffset++) = 0;
				*(lpS+lOffset)   = 0;
			}
			else //����ֵ�����ٽ�ֵ
			{   
				//���������Ϊ��ɫ
				*(lpS+lOffset++) = 255;
				*(lpS+lOffset++) = 255;
				*(lpS+lOffset)   = 255;
			}
		}

		GlobalUnlock(hDIB);
		
}














DIB::DIB()
{	


}
DIB::~DIB()
{

}

CPoint DIB::NextBorderPoint(CPoint CurrentPoint,LPBYTE lpDIBBits,WORD wBytesPerLine)
{
	CPoint Pointtmp;
/*	Pointtmp.x=-1;
	Pointtmp.y=-1;
	

	if(CurrentPoint.x!=this->width-1)
	{
		Pointtmp.x=CurrentPoint.x+1;
		Pointtmp.y=CurrentPoint.y;
	
		if(this->IsBorderPoint(Pointtmp,lpDIBBits, wBytesPerLine))
		return Pointtmp;
	}
	if((CurrentPoint.y!=this->height-1)&&(CurrentPoint.x!=this->width-1))
	{
		Pointtmp.x=CurrentPoint.x+1;
		Pointtmp.y=CurrentPoint.y+1;
	
		if(this->IsBorderPoint(Pointtmp,lpDIBBits, wBytesPerLine))
		return Pointtmp;
	}
	if(CurrentPoint.y!=this->height-1)
	{
		Pointtmp.x=CurrentPoint.x;
		Pointtmp.y=CurrentPoint.y+1;
	
		if(this->IsBorderPoint(Pointtmp,lpDIBBits, wBytesPerLine))
		return Pointtmp;
	}
	if((CurrentPoint.x!=0)&&(CurrentPoint.y!=this->height-1))
	{
		Pointtmp.x=CurrentPoint.x-1;
		Pointtmp.y=CurrentPoint.y+1;
	
		if(this->IsBorderPoint(Pointtmp,lpDIBBits, wBytesPerLine))
		return Pointtmp;
	}
	if(CurrentPoint.x!=0)
	{
		Pointtmp.x=CurrentPoint.x-1;
		Pointtmp.y=CurrentPoint.y;
	
		if(this->IsBorderPoint(Pointtmp,lpDIBBits, wBytesPerLine))
		return Pointtmp;
	}
	if((CurrentPoint.x!=0)&&(CurrentPoint.y!=0))
	{
		Pointtmp.x=CurrentPoint.x-1;
		Pointtmp.y=CurrentPoint.y-1;
	
		if(this->IsBorderPoint(Pointtmp,lpDIBBits, wBytesPerLine))
		return Pointtmp;
	}
	
	if(CurrentPoint.y != 0)
	{
		Pointtmp.x = CurrentPoint.x;
		Pointtmp.y = CurrentPoint.y-1;
	
		if(this->IsBorderPoint(Pointtmp,lpDIBBits, wBytesPerLine))
			return Pointtmp;
	}
   	if((CurrentPoint.y!=0)&&(CurrentPoint.x!=this->width-1))
	{
		Pointtmp.y=CurrentPoint.y-1;
		Pointtmp.x=CurrentPoint.x+1;
	
		if(this->IsBorderPoint(Pointtmp,lpDIBBits, wBytesPerLine))
		return Pointtmp;
	}
	CPoint errorPoint;
	errorPoint.x=errorPoint.y=-2;
	return errorPoint;
	*/
	return Pointtmp;
}

BOOL DIB::IsBorderPoint(CPoint CurrentPoint ,LPBYTE lpDIBBits,WORD wBytesPerLine)
{
	long lOffset;
	lOffset = this->PixelOffset(CurrentPoint.y,CurrentPoint.x,wBytesPerLine);
	if(*(lpDIBBits+lOffset)!=0)
		return false;

	if(CurrentPoint.y!=0)
	{
		lOffset = this->PixelOffset(CurrentPoint.y-1,CurrentPoint.x,wBytesPerLine);
		if(*(lpDIBBits+lOffset)==255)
		return true;
	}

	if(CurrentPoint.x!=this->width-1)
	{
		lOffset = this->PixelOffset(CurrentPoint.y,CurrentPoint.x+1,wBytesPerLine);
		if(*(lpDIBBits+lOffset)==255)
		return true;
	}

	if(CurrentPoint.y!=this->height-1)
	{
		lOffset = this->PixelOffset(CurrentPoint.y+1,CurrentPoint.x,wBytesPerLine);
		if(*(lpDIBBits+lOffset)==255)
		return true;
	}


	if(CurrentPoint.x!=0)
	{
		lOffset = this->PixelOffset(CurrentPoint.y,CurrentPoint.x-1,wBytesPerLine);
		if(*(lpDIBBits+lOffset)==255)
		return true;
	}

	return false;
}



CPoint DIB::SearchInteriorPoint(CPoint Point,LPBYTE lpDIBBits,WORD wBytesPerLine)
{
	long lOffset;
	CPoint Pointtmp;
	Pointtmp.x=Pointtmp.y=-1;
	if((Point.x!=0)&&(Point.y!=this->height-1))
	{
		lOffset = this->PixelOffset(Point.y+1,Point.x-1,wBytesPerLine);
		if(*(lpDIBBits+lOffset)==0)
		{
			Pointtmp.x=Point.x-1;
			Pointtmp.y=Point.y+1;
			return Pointtmp;
		}
	}

	if(Point.y!=this->height-1)
	{
		lOffset = this->PixelOffset(Point.y+1,Point.x,wBytesPerLine);
		if(*(lpDIBBits+lOffset)==0)
		{
			Pointtmp.x=Point.x;
			Pointtmp.y=Point.y+1;
			return Pointtmp;
		}
	}

	if((Point.x!=this->width-1)&&(Point.y!=this->height-1))
	{
		lOffset = this->PixelOffset(Point.y+1,Point.x+1,wBytesPerLine);
		if(*(lpDIBBits+lOffset)==0)
		{
			Pointtmp.x=Point.x+1;
			Pointtmp.y=Point.y+1;
			return Pointtmp;
		}
	}

	if(Point.x!=this->width-1)
	{
		lOffset=this->PixelOffset(Point.y,Point.x+1,wBytesPerLine);
		if(*(lpDIBBits+lOffset)==0)
		{
			Pointtmp.x=Point.x+1;
			Pointtmp.y=Point.y;
			return Pointtmp;
		}
	}
	return Pointtmp;
}

//��ͼƬ�е�ͼ�ν���ϸ������
void DIB::ThinningDIB(HANDLE hDIB)
{
	
	LPBITMAPINFOHEADER lpbi;
	WORD wBytesPerLine;
	LPBYTE lpDIBBits;
	long lOffset;
	int width,height;
	//ϸ����ѯ��
	static int erasetable[256]={
								0,0,1,1,0,0,1,1,
								1,1,0,1,1,1,0,1,
								1,1,0,0,1,1,1,1,
								0,0,0,0,0,0,0,1,
								
								0,0,1,1,0,0,1,1,
								1,1,0,1,1,1,0,1,
								1,1,0,0,1,1,1,1,
								0,0,0,0,0,0,0,1,
								
								1,1,0,0,1,1,0,0,
								0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,
								
								1,1,0,0,1,1,0,0,
								1,1,0,1,1,1,0,1,
								0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,

								0,0,1,1,0,0,1,1,
								1,1,0,1,1,1,0,1,
								1,1,0,0,1,1,1,1,
								0,0,0,0,0,0,0,1,
								
								0,0,1,1,0,0,1,1,
								1,1,0,1,1,1,0,1,
								1,1,0,0,1,1,1,1,
								0,0,0,0,0,0,0,0,
								
								1,1,0,0,1,1,0,0,
								0,0,0,0,0,0,0,0,
								1,1,0,0,1,1,1,1,
								0,0,0,0,0,0,0,0,

								1,1,0,0,1,1,0,0,
								1,1,0,1,1,1,0,0,
								1,1,0,0,1,1,1,0,
								1,1,0,0,1,0,0,0
						  };
	
		lpbi=(LPBITMAPINFOHEADER)GlobalLock(hDIB);
		//�õ�ͼƬ�ĳ�����Ϣ
		width = lpbi->biWidth;
		height = lpbi->biHeight;
		lpDIBBits = this->FindDIBBits(hDIB);
	
		wBytesPerLine = this->BytePerLine(hDIB);
		//ִ��ϸ����ֱ��û�е���Ա�ȥ��Ϊֹ
		int end =0;
		while(!end)
		{
			end =1;
			//ɨ������ͼƬ����������
		for(int i=1;i<height-1;i++)
			for(int j=1;j<width-1;j++)
			{	
				//�ж���ǰ�����ɫ
				lOffset = this->PixelOffset(i,j,wBytesPerLine);
				if(*(lpDIBBits+lOffset)==0)
					continue;//����Ǻ�ɫ�ģ�ֱ�ӿ�����һ����
				else
				{
					int colorleft,colorright;
					lOffset =this->PixelOffset(i,j-1,wBytesPerLine);
					colorleft = *(lpDIBBits+lOffset);
					lOffset = this->PixelOffset(i,j+1,wBytesPerLine);
					colorright = *(lpDIBBits+lOffset);
					//�����ǰ��ĵ���ߺ��ұߵĵ����ɫ���ǰ�ɫ
					if((colorleft==255)&&(colorright==255))
						continue;//ֱ�ӽ�����һ����Ĵ���
					else
					{	
						//�����һ�δ��뿼�쵱ǰ�����Χ�˸������ϵĵ����ɫ
						//��������ɫ���ñ�־λ
						int k1,k2,k3,k4,k5,k6,k7,k8;
						//����
						lOffset = this->PixelOffset(i+1,j-1,wBytesPerLine);
						if(*(lpDIBBits+lOffset)==0)
							k1=1;
						else
							k1 =0;
						//����
						lOffset = this->PixelOffset(i+1,j,wBytesPerLine);
						if(*(lpDIBBits+lOffset)==0)
							k2=1;
						else
							k2 =0;
						//����
						lOffset = this->PixelOffset(i+1,j+1,wBytesPerLine);
						if(*(lpDIBBits+lOffset)==0)
							k3=1;
						else
							k3 =0;
						//����
						lOffset = this->PixelOffset(i,j-1,wBytesPerLine);
						if(*(lpDIBBits+lOffset)==0)
							k4=1;
						else
							k4 =0;
                        //����
						lOffset = this->PixelOffset(i,j+1,wBytesPerLine);
						if(*(lpDIBBits+lOffset)==0)
							k5=1;
						else
							k5 =0;
                        //����
						lOffset = this->PixelOffset(i-1,j-1,wBytesPerLine);
						if(*(lpDIBBits+lOffset)==0)
							k6=1;
						else
							k6 =0;
                        //����
						lOffset = this->PixelOffset(i-1,j,wBytesPerLine);
						if(*(lpDIBBits+lOffset)==0)
							k7=1;
						else
							k7 =0;
						//����
						lOffset = this->PixelOffset(i-1,j+1,wBytesPerLine);
						if(*(lpDIBBits+lOffset)==0)
							k8=1;
						else
							k8 =0;
						int judge;
						//����õ�ϸ����ѯ���е�λ��
						judge = k1+k2*2+k3*4+k4*8+k5*16+k6*32+k7*64+k8*128;
						if(erasetable[judge]==1)
						{   //ɾ��
							lOffset = this->PixelOffset(i,j,wBytesPerLine);
							*(lpDIBBits+lOffset++)=0;
							*(lpDIBBits+lOffset++)=0;
							*(lpDIBBits+lOffset++)=0;
							j++;
							end =0;//����ѭ��
						}
					}
				}
			}
   //ɨ������ͼƬ�������ң���������
	for(int j=1;j<width-1;j++)
		for(int i=1;i<height-1;i++)
			{
				lOffset = this->PixelOffset(i,j,wBytesPerLine);
				if(*(lpDIBBits+lOffset)==0)
					continue;
				else
				{
					int colorleft,colorright;
					lOffset =this->PixelOffset(i,j-1,wBytesPerLine);
					colorleft = *(lpDIBBits+lOffset);
					lOffset = this->PixelOffset(i,j+1,wBytesPerLine);
					colorright = *(lpDIBBits+lOffset);
					if((colorleft==255)&&(colorright==255))
						continue;
					else
					{
						int k1,k2,k3,k4,k5,k6,k7,k8;
						lOffset = this->PixelOffset(i+1,j-1,wBytesPerLine);
						if(*(lpDIBBits+lOffset)==0)
							k1=1;
						else
							k1 =0;

						lOffset = this->PixelOffset(i+1,j,wBytesPerLine);
						if(*(lpDIBBits+lOffset)==0)
							k2=1;
						else
							k2 =0;
						
						lOffset = this->PixelOffset(i+1,j+1,wBytesPerLine);
						if(*(lpDIBBits+lOffset)==0)
							k3=1;
						else
							k3 =0;

						lOffset = this->PixelOffset(i,j-1,wBytesPerLine);
						if(*(lpDIBBits+lOffset)==0)
							k4=1;
						else
							k4 =0;

						lOffset = this->PixelOffset(i,j+1,wBytesPerLine);
						if(*(lpDIBBits+lOffset)==0)
							k5=1;
						else
							k5 =0;

						lOffset = this->PixelOffset(i-1,j-1,wBytesPerLine);
						if(*(lpDIBBits+lOffset)==0)
							k6=1;
						else
							k6 =0;

						lOffset = this->PixelOffset(i-1,j,wBytesPerLine);
						if(*(lpDIBBits+lOffset)==0)
							k7=1;
						else
							k7 =0;
						lOffset = this->PixelOffset(i-1,j+1,wBytesPerLine);
						if(*(lpDIBBits+lOffset)==0)
							k8=1;
						else
							k8 =0;
						int judge;
						judge = k1+k2*2+k3*4+k4*8+k5*16+k6*32+k7*64+k8*128;
						if(erasetable[judge]==1)
						{
							lOffset = this->PixelOffset(i,j,wBytesPerLine);
							*(lpDIBBits+lOffset++)=0;
							*(lpDIBBits+lOffset++)=0;
							*(lpDIBBits+lOffset++)=0;
							j++;
							end =0;
						}
					}
				}
			}

		}

}





BOOL DIB:: SaveDIB(HANDLE hDib, CFile& file)
{
	// Bitmap�ļ�ͷ
	BITMAPFILEHEADER bmfHdr;
	
	// ָ��BITMAPINFOHEADER��ָ��
	LPBITMAPINFOHEADER lpBI;
	
	// DIB��С
	DWORD dwDIBSize =0;

	if (hDib == NULL)
	{
		// ���DIBΪ�գ�����FALSE
		return FALSE;
	}

	// ��ȡBITMAPINFO�ṹ��������
	lpBI = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDib);
	
	if (lpBI == NULL)
	{
		// Ϊ�գ�����FALSE
		return FALSE;
	}
	
	// �ж��Ƿ���WIN3.0 DIB
//	if (!IS_WIN30_DIB(lpBI))
//	{
		// ��֧���������͵�DIB����
		
		// �������
	//	::GlobalUnlock((HGLOBAL) hDib);
		
		// ����FALSE
	//	return FALSE;
//	}

	// ����ļ�ͷ

	// �ļ�����"BM"
	bmfHdr.bfType =  0x4d42; //DIB_HEADER_MARKER;

	// ����DIB��Сʱ����򵥵ķ����ǵ���GlobalSize()����������ȫ���ڴ��С��
	// ����DIB�����Ĵ�С�������Ƕ༸���ֽڡ���������Ҫ����һ��DIB����ʵ��С��
	
	// �ļ�ͷ��С����ɫ���С
	// ��BITMAPINFOHEADER��BITMAPCOREHEADER�ṹ�ĵ�һ��DWORD���Ǹýṹ�Ĵ�С��
//	dwDIBSize = *(LPDWORD)lpBI; //+ ::PaletteSize((LPSTR)lpBI);
	dwDIBSize = sizeof(BITMAPINFOHEADER);//+lpBI->biSizeImage;	
	// ����ͼ���С
	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
	{
		// ����RLEλͼ��û�������С��ֻ������biSizeImage�ڵ�ֵ
		dwDIBSize += lpBI->biSizeImage;
	}
	else
	{
		// ���صĴ�С
		DWORD dwBmBitsSize;

		// ��СΪWidth * Height
		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*24) * lpBI->biHeight;
		
		// �����DIB�����Ĵ�С
		dwDIBSize += dwBmBitsSize;

		// ����biSizeImage���ܶ�BMP�ļ�ͷ��biSizeImage��ֵ�Ǵ���ģ�
		lpBI->biSizeImage = dwBmBitsSize;
	}


	// �����ļ���С��DIB��С��BITMAPFILEHEADER�ṹ��С
	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	
	// ����������
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	// ����ƫ����bfOffBits�����Ĵ�СΪBitmap�ļ�ͷ��С��DIBͷ��С����ɫ���С
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize;
											 // + PaletteSize((LPSTR)lpBI);
	// ����д�ļ�
//	TRY
	{
		// д�ļ�ͷ
		file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
		
		// дDIBͷ������
		file.WriteHuge(lpBI, dwDIBSize);
	}
//	CATCH (CFileException, e)
//	{
		// �������
	//	::GlobalUnlock((HGLOBAL) hDib);
		
		// �׳��쳣
///		THROW_LAST();
//	}
//	END_CATCH
	
	// �������
	::GlobalUnlock((HGLOBAL) hDib);
	
	// ����TRUE
	return TRUE;
}




HANDLE DIB::ScaleDIB(HANDLE hDIB, float scal_x, float scal_y)
{
	if(scal_x<0 || scal_y <0)
		return NULL;

	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER) GlobalLock(hDIB);
	int w = lpbi->biWidth;
	int h = lpbi->biHeight;
	BYTE  tempmess[40];
	LPBYTE Src = this->FindDIBBits(hDIB);
	memcpy(tempmess,(LPBYTE)lpbi,40);
	int wid = ((int)(w*scal_x+0.5f)*24+31)/32*4;
	int imgsize =(int) (wid*(int)(h*scal_y+0.5f));
	
	HDIB hDIB2 = GlobalAlloc(GMEM_MOVEABLE,(DWORD)(sizeof(BITMAPINFOHEADER)+imgsize));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB2);
	memcpy((LPBYTE)lpbi,tempmess,40);
	lpbi->biWidth =(int)(w*scal_x+0.5f);
	lpbi->biHeight = (int)(h*scal_y+0.5f);
	lpbi->biSizeImage = imgsize;
	LPBYTE lpData = this->FindDIBBits(hDIB2);
	int srcWidth = w;
	int srcHeight = h;
	int dstWidth = (int)(w*scal_x+0.5f);
	int dstHeight = (int)(h*scal_y+0.5f);

	for(int i=0; i<dstHeight;i++)
	{
		float y_inverse_map = (float)i/scal_y;
		int y_lt = (int)y_inverse_map;
		float v=y_inverse_map - y_lt;
		int indexBase = i *wid;
		for(int j=0;j<dstWidth;j++)
		{
			float x_inverse_map = (float)j/scal_x;
			int x_lt = (int) x_inverse_map;
			float u=x_inverse_map - x_lt;
			int index = indexBase + j*3;
			*(lpData+index) = Src[y_lt*((w*24+31)/32*4)+x_lt*3];
			*(lpData+index+1) = Src[y_lt*((w*24+31)/32*4)+x_lt*3+1];
			*(lpData+index+2) = Src[y_lt*((w*24+31)/32*4)+x_lt*3+2];
		//	int r,g,b;
			//((interPolate(Src,x_lt,y_lt,u,v,srcWidth,srcHeight,&r,&g,&b))) ;
		//	*(lpData+temp)   = b;
			//*(lpData+index++)   = (BYTE)((interPolate(Src,x_lt,y_lt,u,v,srcWidth,srcHeight)>>16)) & 0x000000ff;
		//	(interPolate(Src,x_lt,y_lt,u,v,srcWidth,srcHeight,&r,&g,&b));
		//	*(lpData+temp+1)   = g;
		  //  (interPolate(Src,x_lt,y_lt,u,v,srcWidth,srcHeight,&r,&g,&b));
		//	 *(lpData+temp+2)   =r;
			
		}
	}
	GlobalUnlock(hDIB);
	GlobalUnlock(hDIB2);
	return hDIB2;

}

//cwh 01.6.20
/*****************************************
��������㷨
*****************************************/
void DIB::interPolate(LPBYTE Src,int x,int y, float u,float v, int scanw, int scanh,int *r,int *g,int *b)
{
//	UINT r11,g11,b11;
	//r=g=b=0;
		int red1[2][2];
		int green1[2][2];
		int blue1[2][2];
		int xx1[2];
		int yy1[2];
		xx1[0]=x*3;xx1[1]=x*3+3;
		yy1[0]=y;yy1[1]=y+1;
		if(xx1[1]>scanw-1)
			xx1[1] = scanw-1;
		if(yy1[1]>scanh-1)
			yy1[1] = scanh-1;

		for(int i=0;i<2;i++)
		{
			int indexBase1 = yy1[i]*((scanw*24+31)/32*4);
			for(int j=0;j<2;j++)
			{
				int index1 = indexBase1 + xx1[j];
				blue1[j][i] = (*(Src+index1));
			    green1[j][i] = (*(Src+index1+1));
			    red1[j][i] =  (*(Src+index1+2));

			}
		}

		//r11 = ((1-u)*(1-v)*red1[0][0]+(1-u)*v*red1[0][1]+u*(1-v)*red1[1][0]+u*v*red1[1][1]);
	//	g11 = ((1-u)*(1-v)*green1[0][0]+(1-u)*v*green1[0][1]+u*(1-v)*green1[1][0]+u*v*green1[1][1]);
		//b11 = ((1-u)*(1-v)*blue1[0][0]+(1-u)*v*blue1[0][1]+u*(1-v)*blue1[1][0]+u*v*blue1[1][1]);
  
	//	*r=r11;
		//*g=g11;
	//	*b=b11;
//	return (0x00ffffff&((b<<16)|(g<<8)|r));
}

HANDLE DIB::CopyHandle( HANDLE hSrc)
{	
	HANDLE hDst;
	LPBITMAPINFOHEADER lpbi;
	int width,height;
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hSrc);
	width = lpbi->biWidth;
	height = lpbi->biHeight;
	hDst = GlobalAlloc(GMEM_MOVEABLE,lpbi->biSize+lpbi->biSizeImage);
	if(!hDst)
		return NULL;
	LPBYTE lpDest;
	lpDest = (LPBYTE)GlobalLock(hDst);
	memcpy(lpDest,(LPBYTE)lpbi,lpbi->biSize+lpbi->biSizeImage);
	GlobalUnlock(hSrc);
	GlobalUnlock(hDst);
	return hDst;

}
HANDLE  DIB::  Gradient(HANDLE hDIB)
{
	LPBITMAPINFOHEADER lpbi;
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
	int width = lpbi->biWidth;
	int height = lpbi->biHeight;
	long lOffset;
	LPBYTE lpS,lpD;
	WORD wBytesPerLine = this->BytePerLine(hDIB);
	HANDLE hNewDIB;
	hNewDIB = GlobalAlloc(GMEM_MOVEABLE,(DWORD)(lpbi->biSize+lpbi->biSizeImage));
	if(!hNewDIB)
	{
		AfxMessageBox("�����ڴ�ʧ��");
		return NULL;
	}
	lpS = (LPBYTE)lpbi;
	lpD =(LPBYTE) GlobalLock(hNewDIB);
	memcpy(lpD,lpS,sizeof(BITMAPINFOHEADER));
	lpS = this->FindDIBBits(hDIB);
	lpD = this->FindDIBBits(hNewDIB);
	int color1,color2;
	for(int i=1;i<height-1;i++)
		for(int j=1;j<width-1;j++)
		{
			lOffset = this->PixelOffset(i+1,j+1,wBytesPerLine);
			color1 = *(lpS+lOffset);
			lOffset = this->PixelOffset(i-1,j-1,wBytesPerLine);
			color2 = *(lpS+lOffset);
			lOffset = this->PixelOffset(i,j,wBytesPerLine);
			*(lpD+lOffset++) = abs(color2-color1)*3;
			*(lpD+lOffset++) = abs(color2-color1)*3;
			*(lpD+lOffset++) = abs(color2-color1)*3;

		}
		for(i =0;i<height;i++)
		{
			lOffset = this->PixelOffset(i,0,wBytesPerLine);
			*(lpD+lOffset++) =0;
			*(lpD+lOffset++) =0;
			*(lpD+lOffset++) =0;
		}

		for(i =0;i<height;i++)
		{
			lOffset = this->PixelOffset(i,width-1,wBytesPerLine);
			*(lpD+lOffset++) =0;
			*(lpD+lOffset++) =0;
			*(lpD+lOffset++) =0;
		}
		for(i =0;i<width;i++)
		{
			lOffset = this->PixelOffset(height-1,i,wBytesPerLine);
			*(lpD+lOffset++) =0;
			*(lpD+lOffset++) =0;
			*(lpD+lOffset++) =0;
		}
		for(i =0;i<width;i++)
		{
			lOffset = this->PixelOffset(0,i,wBytesPerLine);
			*(lpD+lOffset++) =0;
			*(lpD+lOffset++) =0;
			*(lpD+lOffset++) =0;
		}

		GlobalUnlock(hNewDIB);
		GlobalUnlock(hDIB);
		return hNewDIB;
}

//����Ѱ��ͼƬ�е�������������ĵ�
void DIB::LocateImporntPoint(HANDLE hDIB, int Radius, CPoint *pPoint)
{
	LPBITMAPINFOHEADER lpbi;
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
	int width ,height;
	width = lpbi->biWidth;
	height = lpbi->biHeight;
	WORD wBytesPerLine;
	wBytesPerLine = this->BytePerLine(hDIB);
	LPBYTE lpData;
	lpData = this->FindDIBBits(hDIB);
	int tempsum=0,sum=0;//��������������������
	long lOffset;
	//ɨ������ͼƬ����Ե����⣩Ѱ����������
	for(int i=Radius;i<height-Radius;i++)
		for(int j= Radius;j<width-Radius;j++)
		{	tempsum =0;
			//ɨ����Radius��2��1Ϊ�߳�������������
			for(int k1=-Radius;k1<=Radius;k1++)
				for(int k2 =-Radius;k2<=Radius;k2++)
				{	
					lOffset = this->PixelOffset(i+k1,j+k2,wBytesPerLine);
					int color = *(lpData+lOffset);
					tempsum +=color;//�ۼ�����ֵ
				}
				if(tempsum>sum)//����õ����ۼ�����ֵ�����Ѿ��õ������ֵ
				{	
					//�����ۼ�����ֵ��С
					sum = tempsum;
					//���������������ĵ�
					(pPoint->x) = j;
					(pPoint->y) = i;
				   

				}
		}
	//����Ĵ������������ı߿����óɰ�ɫ
	for( i = -Radius;i<=Radius;i++)
	{
		lOffset = this->PixelOffset(pPoint->y-Radius,pPoint->x +i,wBytesPerLine);
		*(lpData+lOffset++) = 255;
		*(lpData+lOffset++) = 255;
		*(lpData+lOffset++) = 255;
	}
	for( i = -Radius;i<=Radius;i++)
	{
		lOffset = this->PixelOffset(pPoint->y+Radius,pPoint->x +i,wBytesPerLine);
		*(lpData+lOffset++) = 255;
		*(lpData+lOffset++) = 255;
		*(lpData+lOffset++) = 255;
	}
	for( i = -Radius;i<=Radius;i++)
	{
		lOffset = this->PixelOffset(pPoint->y+i,pPoint->x-Radius ,wBytesPerLine);
		*(lpData+lOffset++) = 255;
		*(lpData+lOffset++) = 255;
		*(lpData+lOffset++) = 255;
	}
	for( i = -Radius;i<=Radius;i++)
	{
		lOffset = this->PixelOffset(pPoint->y+i,pPoint->x+Radius,wBytesPerLine);
		*(lpData+lOffset++) = 255;
		*(lpData+lOffset++) = 255;
		*(lpData+lOffset++) = 255;
	}
	lOffset = this->PixelOffset(pPoint->y,pPoint->x ,wBytesPerLine);
	*(lpData+lOffset++) = 0;
	*(lpData+lOffset++) = 255;
	*(lpData+lOffset++) = 0;

		GlobalUnlock(hDIB);
}


#define THRESHOLD (RADIUS*2+1)*(RADIUS*2+1)*15
//������һ��ͼƬ��Ѱ��ƥ������ĵ�
BOOL DIB::MatchImportantPoint(HANDLE hDIB,int CharaterInfo[RADIUS*2+1][RADIUS*2+1][3],CPoint *ImPoint)
{
	LPBITMAPINFOHEADER lpbi;
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
	int width = lpbi->biWidth;
	int height = lpbi->biHeight;
	LPBYTE lpData = this->FindDIBBits(hDIB);
    WORD wBytesPerLine = this->BytePerLine(hDIB);
	long lOffset;
	long sum =100000,tempsum;
	//ɨ������ͼƬ����Ե�㣩����
	for(int i=RADIUS ;i<height-RADIUS;i++)
		for(int j=RADIUS;j<width-RADIUS;j++)
		{	
			tempsum =0;
			//ɨ����RADIUS*2+1Ϊ�߳�������������
			for(int k=-RADIUS;k<=RADIUS;k++)
				for(int kk=-RADIUS;kk<=RADIUS;kk++)
				{
					//���㵱ǰ�����κ���֪�����������ɫ��ֵ

				lOffset = this->PixelOffset(i+k,j+kk,wBytesPerLine);
				int colorblue = abs(*(lpData+lOffset++)-CharaterInfo[k+RADIUS][kk+RADIUS][0]);
				int colorgreen = abs(*(lpData+lOffset++)-CharaterInfo[k+RADIUS][kk+RADIUS][1]);
				int colorred = abs(*(lpData+lOffset++)-CharaterInfo[k+RADIUS][kk+RADIUS][2]);
				tempsum +=colorgreen+colorblue+colorred;
				}
				if(tempsum<sum)
				{  //���²�ֵ
					sum = tempsum;
					//�������������
					ImPoint->x = j;
					ImPoint->y = i;
				}
		}

		if(sum <THRESHOLD){//�ҵ���������������
		//����Ĵ�����ҵ�������ı߿����ó�Ϊ��ɫ
		for(i =-RADIUS;i<=RADIUS;i++)
		{
			lOffset = this->PixelOffset(ImPoint->y-RADIUS,ImPoint->x+i,wBytesPerLine);
			*(lpData+lOffset++) = 255;
			*(lpData+lOffset++) = 255;
			*(lpData+lOffset++) = 255;

		}

		for(i =-RADIUS;i<=RADIUS;i++)
		{
			lOffset = this->PixelOffset(ImPoint->y+RADIUS,ImPoint->x+i,wBytesPerLine);
			*(lpData+lOffset++) = 255;
			*(lpData+lOffset++) = 255;
			*(lpData+lOffset++) = 255;

		}
		for(i =-RADIUS;i<=RADIUS;i++)
		{
			lOffset = this->PixelOffset(ImPoint->y+i,ImPoint->x+RADIUS,wBytesPerLine);
			*(lpData+lOffset++) = 255;
			*(lpData+lOffset++) = 255;
			*(lpData+lOffset++) = 255;

		}
		for(i =-RADIUS;i<=RADIUS;i++)
		{
			lOffset = this->PixelOffset(ImPoint->y+i,ImPoint->x-RADIUS,wBytesPerLine);
			*(lpData+lOffset++) = 255;
			*(lpData+lOffset++) = 255;
			*(lpData+lOffset++) = 255;

		}
		GlobalUnlock(hDIB);
		return true;
		}
		else AfxMessageBox("Can't find the corresponding point!");
	GlobalUnlock(hDIB);
	return false;
}
//�Ƚ�����ͼƬ�����ƶ�
BOOL DIB::ComPareImg(HANDLE hDIB1, HANDLE hDIB2 ,CPoint pt1,CPoint pt2)
{
	if(abs(pt1.x-pt2.x)>3 || abs(pt1.y -pt2.y)>3)//ͼ��ƫ�����
	{	
		AfxMessageBox("Imgs Offset are too big");
		return false;
	}
	LPBITMAPINFOHEADER lpbi1,lpbi2;
	lpbi1 = (LPBITMAPINFOHEADER)GlobalLock(hDIB1);
	int width1 = lpbi1->biWidth;
	int height1 = lpbi1->biHeight;
	lpbi2 = (LPBITMAPINFOHEADER)GlobalLock(hDIB2);
	int width2 = lpbi2->biWidth;
	int height2 = lpbi2->biHeight;
	if(width1 != width2 || height1 != height2)//ͼ�󳤿�ߴ粻ͬ
	{
		GlobalUnlock(hDIB1);
		GlobalUnlock(hDIB2);
		AfxMessageBox("Img is not same size");
		return false;
	}
	LPBYTE lpData1,lpData2;
	lpData1 = this->FindDIBBits(hDIB1);
	lpData2 = this->FindDIBBits(hDIB2);
	WORD wBytesPerLine = this->BytePerLine(hDIB1);
	int xleft,xright,ytop,ybottom;
	//�����һ�δ���ʵ��ͼ�����
	if(pt1.x>=pt2.x)//��һ��ͼ���������ĵ�ȵڶ���ͼƫ��
	{	
	
		xleft = pt2.x;//Ҫ�������ߵ����ص����
		xright =width-pt1.x-1;//Ҫ������ұߵ����ص����
	}
			
	else//��һ��ͼ���������ĵ�ȵڶ���ͼƫ��
	{	
	
		xleft = pt1.x;
		xright = width1-pt2.x-1;
	}
	
	if(pt1.y >=pt2.y)//��һ��ͼ���������ĵ��λ��ƫ��
	{	
	
		ytop = pt2.y;//Ҫ��������ĵ���������ظ���
		ybottom = height1-pt1.y-1;//Ҫ��������ĵ���������ظ���

	}

	else//��һ��ͼ���������ĵ��λ��ƫ��
	{	
		
		ytop = pt1.y;
		ybottom = height1-pt2.y-1;
	}
	long sum=0;
	long lOffset;
	//��������ͼƬ������������ز�ֵ
	for(int i=-ytop;i<=ybottom;i++)
		for(int j=-xleft;j<=xright;j++)
		{	
			//��һ��ͼ
			lOffset = this->PixelOffset(i+pt1.y,j+pt1.x,wBytesPerLine);
			int c11 = *(lpData1+lOffset++);
			int c12 = *(lpData1+lOffset++);
			int c13 = *(lpData1+lOffset++);
			//�ڶ���ͼ
			lOffset = this->PixelOffset(i+pt2.y,j+pt2.x,wBytesPerLine);
			int c21 = *(lpData2+lOffset++);
			int c22 = *(lpData2+lOffset++);
			int c23 = *(lpData2+lOffset++);
			//�����ֵ
			sum += abs(c11-c21)+abs(c12-c22)+abs(c13-c23);
		}
		GlobalUnlock(hDIB1);
		GlobalUnlock(hDIB2);
	if(sum>width1*height1*3*2)//�ж��Ƿ�����
		return false;
	else 
		return true;

}

BOOL DIB::  CompareImg2(HANDLE hDIBBK,HANDLE hDIBCurrent,CPoint pt1,CPoint pt2)
{
	if(abs(pt1.x-pt2.x)>3 || abs(pt1.y -pt2.y)>3)
	{	
		AfxMessageBox("Imgs Offset are too big");
		return false;
	}
	LPBITMAPINFOHEADER lpbi1,lpbi2;
	lpbi1 = (LPBITMAPINFOHEADER)GlobalLock(hDIBBK);
	int width1 = lpbi1->biWidth;
	int height1 = lpbi1->biHeight;
	lpbi2 = (LPBITMAPINFOHEADER)GlobalLock(hDIBCurrent);
	int width2 = lpbi2->biWidth;
	int height2 = lpbi2->biHeight;
	if(width1 != width2 || height1 != height2)
	{
		GlobalUnlock(hDIBBK);
		GlobalUnlock(hDIBCurrent);
		AfxMessageBox("Img is not same size");
		return false;
	}
	LPBYTE lpData1,lpData2;
	lpData1 = this->FindDIBBits(hDIBBK);
	lpData2 = this->FindDIBBits(hDIBCurrent);
	WORD wBytesPerLine = this->BytePerLine(hDIBBK);
	int xleft,xright,ytop,ybottom;

	if(pt1.x>=pt2.x)
	{	
	
		xleft = pt2.x;
		xright =width1-pt1.x-1;
	}
			
	else
	{	
	
		xleft = pt1.x;
		xright = width1-pt2.x-1;
	}
	
	if(pt1.y >=pt2.y)
	{	
	
		ytop = pt2.y;
		ybottom = height1-pt1.y-1;

	}

	else
	{	
		
		ytop = pt1.y;
		ybottom = height1-pt2.y-1;
	}
	long sum=0;
	long lOffset;
	


	for(int i=-ytop;i<=ybottom;i++)
		for(int j=-xleft;j<=xright;j++)
		{	
		
			lOffset = this->PixelOffset(i+pt1.y,j+pt1.x,wBytesPerLine);
			int c11 = *(lpData1+lOffset++);
			int c12 = *(lpData1+lOffset++);
			int c13 = *(lpData1+lOffset++);
			lOffset = this->PixelOffset(i+pt2.y,j+pt2.x,wBytesPerLine);
			int c21 = *(lpData2+lOffset++);
			int c22 = *(lpData2+lOffset++);
			int c23 = *(lpData2+lOffset++);
			lOffset = this->PixelOffset(i+pt2.y,j+pt2.x,wBytesPerLine);
			*(lpData2+lOffset++) = abs(c11-c21);
			*(lpData2+lOffset++) = abs(c12-c22);
			*(lpData2+lOffset++) = abs(c13-c23);
		}
		for(i =0;i<height1;i++)
			for(int j=0;j<width1;j++)
			{
			if(i<pt2.y-ytop || i>pt2.y+ybottom||j<pt2.x-xleft || j >pt2.x+xright)
			{
				lOffset = this->PixelOffset(i,j,wBytesPerLine);
				*(lpData2+lOffset++) = 0;
				*(lpData2+lOffset++) = 0;
				*(lpData2+lOffset++) = 0;
			}
			}
		
		GlobalUnlock(hDIBBK);
		GlobalUnlock(hDIBCurrent);
		return true;
	
}



BOOL DIB::IsScaterPoint(int x, int y, int width, int height, LPBYTE lpData,WORD wBytesPerLine,  int threshold,bool lab[m_HEIGHT][m_WIDTH])
{
	long lOffset;
	lOffset = this->PixelOffset(y,x,wBytesPerLine);
	if(*(lpData+lOffset) == 255 && lab[y][x] == false)
	{
		this->lenth++;
		lab[y][x] = true;
	if(this->lenth >= threshold)
		return true;
	if(x+1<width && lab[y][x+1] == false)
	{	IsScaterPoint(x+1,y,width,height,lpData,wBytesPerLine,threshold,lab);
		if(this->lenth>=threshold)
			return true;
		
	}
	if(x-1>=0 && lab[y][x-1] == false)
	{
		(IsScaterPoint(x-1,y,width,height,lpData,wBytesPerLine,threshold,lab));
		if(this->lenth>=threshold)
			return true;
		
	}
	
	if(y-1>=0 && lab[y-1][x]==false)
	{
		(IsScaterPoint(x,y-1,width,height,lpData,wBytesPerLine,threshold,lab));
		if(this->lenth>=threshold)
			return true;
		
	}
	if(y+1<height && lab[y+1][x]==false)
	{	(IsScaterPoint(x,y+1,width,height,lpData,wBytesPerLine,threshold,lab));
			if(this->lenth>=threshold)
			return true;
			
	}
	if(y+1<height  && x+1 <width && lab[y+1][x+1]==false)
	{	(IsScaterPoint(x+1,y+1,width,height,lpData,wBytesPerLine,threshold,lab));
			if(this->lenth>=threshold)
			return true;
			
	}
	if(y+1<height && x-1 >=0 && lab[y+1][x-1]==false)
	{	(IsScaterPoint(x-1,y+1,width,height,lpData,wBytesPerLine,threshold,lab));
			if(this->lenth>=threshold)
			return true;
			
	}
	if(y-1>=0 && x-1 >=0 &&lab[y-1][x-1]==false)
	{	(IsScaterPoint(x-1,y-1,width,height,lpData,wBytesPerLine,threshold,lab));
			if(this->lenth>=threshold)
			return true;
			
	}
	if(y-1<height && x+1<width && lab[y+1][x]==false)
	{	(IsScaterPoint(x+1,y-1,width,height,lpData,wBytesPerLine,threshold,lab));
			if(this->lenth>=threshold)
			return true;
			
	}
	}
		return false;
}
//�����õ��߽�����ĵ�
CPoint DIB:: GetEdgeCenter(HANDLE hDIB)
{
	LPBITMAPINFOHEADER lpbi;
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
	int width = lpbi->biWidth;
	int height = lpbi->biHeight;
	WORD wBytesPerLine = this->BytePerLine(hDIB);
	LPBYTE lpData = this->FindDIBBits(hDIB);
	long lOffset;
	int x =0,y =0,num =0;
	//������ͼƬ����ɨ��
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
		{
			lOffset = this->PixelOffset(i,j,wBytesPerLine);
			if(*(lpData+lOffset) ==255)//����ǰ�ɫ����
			{
				x +=j;//���ĵ�ú��������������ϵ�ǰ�ú������������
				y +=i;
				num++;
			}
		}
		//�õ���������
		CPoint result;
		result.x = x/num;
		result.y = y/num;
		//�����ĵ�����Ϊ��ɫ
		lOffset = this->PixelOffset(result.y,result.x,wBytesPerLine);
		*(lpData+lOffset++) = 0;
		*(lpData+lOffset++) = 255;
		*(lpData+lOffset++) = 0;
		return result;
}
