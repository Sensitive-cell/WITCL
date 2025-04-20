//  dibapi.cpp
//
//  Source file for Device-Independent Bitmap (DIB) API.  Provides
//  the following functions:
//
//  PaintDIB()          - Painting routine for a DIB
//  CreateDIBPalette()  - Creates a palette from a DIB
//  FindDIBBits()       - Returns a pointer to the DIB bits
//  DIBWidth()          - Gets the width of the DIB
//  DIBHeight()         - Gets the height of the DIB
//  PaletteSize()       - Gets the size required to store the DIB's palette
//  DIBNumColors()      - Calculates the number of colors
//                        in the DIB's color table
//  CopyHandle()        - Makes a copy of the given global memory block
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "dibapi.h"
#include <io.h>
#include <errno.h>
#include <math.h>
#include "allocmem.h"
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

#ifdef _MAC
#define SWAPWORD(x) MAKEWORD(HIBYTE(x), LOBYTE(x))
#define SWAPLONG(x) MAKELONG(SWAPWORD(HIWORD(x)), SWAPWORD(LOWORD(x)))
void ByteSwapHeader(BITMAPFILEHEADER* bmiHeader);
void ByteSwapInfo(LPSTR lpHeader, BOOL fWin30Header);
#endif
#define PI 3.14159
//�Ƕȵ�����ת���ĺ�
#define RADIAN(angle) ((angle)*PI/180.0) 
/*************************************************************************
 *
 * PaintDIB()
 *
 * Parameters:
 *
 * HDC hDC          - DC to do output to
 *
 * LPRECT lpDCRect  - rectangle on DC to do output to
 *
 * HDIB hDIB        - handle to global memory with a DIB spec
 *                    in it followed by the DIB bits
 *
 * LPRECT lpDIBRect - rectangle of DIB to output into lpDCRect
 *
 * CPalette* pPal   - pointer to CPalette containing DIB's palette
 *
 * Return Value:
 *
 * BOOL             - TRUE if DIB was drawn, FALSE otherwise
 *
 * Description:
 *   Painting routine for a DIB.  Calls StretchDIBits() or
 *   SetDIBitsToDevice() to paint the DIB.  The DIB is
 *   output to the specified DC, at the coordinates given
 *   in lpDCRect.  The area of the DIB to be output is
 *   given by lpDIBRect.
 *
 ************************************************************************/


void WINAPI decomposition(double* image,double* c,double* d,int n,int s)
{    
	int i,ii;
	double* tmp;
	tmp = (double *) calloc (n, sizeof(double));
	for (i = 0; i < n; i++)
		tmp[i] = image[i];
	for (i = 0; i < n/2; i++) 
	   {
		ii = i << 1;
		c[i] = (tmp[ii] + tmp[ii+1]) / 2;
		d[i] =(tmp[ii] - tmp[ii+1]) / 2;
	}
	free((char *)tmp);
}


void WINAPI reconstruction(double* x, double* c,double* d,int n,int s)
{
	int i,  ii;
	double *tmp;
	tmp = (double *) calloc (n*2, sizeof(double));
	
	/* haar basis */
	for (i = 0; i < n; i++) 
	{
		ii = i << 1;
		tmp[ii]   = c[i] + d[i];
		tmp[ii+1] = c[i] - d[i];
	}		
	for (i = 0; i < n*2; i++)
		x[i] = tmp[i];
	free((char *)tmp);
}


















HDIB WINAPI MidFilter(HDIB hDIB,int tem_w,int tem_h)
{

    //ͳ���м�ֵ
    double mid;

	BYTE *temp=(BYTE*)malloc(tem_w*tem_h*sizeof(BYTE));

    //ָ��ͼ����ʼλ�õ�ָ��
    BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) hDIB);

	//ָ��������ʼλ�õ�ָ��
	BYTE *pScrBuff =(BYTE*)::FindDIBBits((char*)lpDIB);
   
	//��ȡͼ�����ɫ��Ϣ
    int numColors=(int) ::DIBNumColors((char *)lpDIB);

    //���ͼ����256ɫ����
     if (numColors!=256) 
	 {   
        //�������
	  	::GlobalUnlock((HGLOBAL) hDIB);

		//����
		return(hDIB);
	 }
    
    //��ָ��ͼ��������ʼλ�õ�ָ�룬��ֵ��ָ�����
    BYTE* oldbuf = pScrBuff;

    //ѭ������
    int i,j,m,n;

	int w, h, dw;

	//��ȡͼ��Ŀ��
	w = (int) ::DIBWidth((char *)lpDIB);
	
	//��ȡͼ��ĸ߶�
	h = (int) ::DIBHeight((char *)lpDIB);
	
	//����ͼ��ÿ�е��ֽ���
	dw = (w+3)/4*4;      
	
	//����һ����ԭͼ���С��ͬ��25ɫ�Ҷ�λͼ
    HDIB newhDIB=NewDIB(w,h,8);  
    
	//ָ���µ�λͼ��ָ��
	BYTE *newlpDIB=(BYTE*)::GlobalLock((HGLOBAL) newhDIB);

	//ָ���µ�λͼ��������ʼλ�õ�ָ�� 
    BYTE *destBuf = (BYTE*)FindDIBBits((char *)newlpDIB);
    
   //��ָ����ͼ��������ʼλ�õ�ָ�룬��ֵ��ָ�����
    BYTE *newbuf=destBuf; 
	
	//��ͼ�����ɨ��
   
	//�� 
    for(i=0;i<h;i++)
    {  
		//��
	   for(j=0;j<w;j++)
	   {   

		   //Ϊͳ�Ʊ�������ʼֵ
	       

         //����ͼ���4���߿�����ر���ԭ�ҶȲ���
	     if( j<((tem_w-1)/2) || j>(w-(tem_w+1)/2) || i<((tem_h-1)/2) || i>(h-(tem_h+1)/2) )
	      *(newbuf+i*dw+j)=*(oldbuf+i*dw+j);
		 
         //�������������ؽ���ģ�����
		 else 
         { 

          //���㣨i,j������Ϊģ�������
          for(m=i-((tem_h-1)/2);m<=i+((tem_h-1)/2);m++)
          {
		     for(n=j-((tem_w-1)/2);n<=j+((tem_w-1)/2);n++)
		    
             //���Ե㣨i��j��Ϊ���ģ���ģ���С��ͬ�ķ�Χ�ڵ����ش��ݵ�ģ�������
		     temp[(m-i+((tem_h-1)/2))*tem_w+n-j+((tem_w-1)/2)]=*(oldbuf+m*dw+n);
		  
		  }
          
		 //�������ݷ�������ֵ
           for(m=0;m<tem_w*tem_h-1;m++)
           {
		   
		      for(n=0;n<tem_w*tem_h-m-1;n++)
			  {
			    if(temp[n]>temp[n+1]) 
			    mid=temp[n];
                temp[n]=temp[n+1];
			    temp[n+1]=mid;
			  }
		   
		   } 

           //������Ľ���ŵ��µ�λͼ����Ӧλ��
	      *(newbuf+i*dw+j)=temp[(tem_w*tem_h-1)/2];
		 }
	   }
	} 
   
	//�������
	::GlobalUnlock((HGLOBAL)hDIB);
    
	//�����µ�λͼ�ľ��
    return(newhDIB);
}

BOOL WINAPI TranslationDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, LONG lXOffset, LONG lYOffset)
{
	// ָ��Դͼ���ָ��
	LPSTR	lpSrc;
	
	// ָ��Ҫ���������ָ��
	LPSTR	lpDst;
	
	// ָ����ͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// ��������DIB�е�����
	LONG	i;
	LONG	j;
	
	// ������ԴDIB�е�����
	LONG	i0;
	LONG	j0;
	
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// �����ڴ�
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);
	
	// ÿ��
	for(i = 0; i < lHeight; i++)
	{
		// ÿ��
		for(j = 0; j < lWidth; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			// ע������DIB��ͼ���һ����ʵ���������һ�е�λ�ã����lpDst
			// ֵ����(char *)lpNewDIBBits + lLineBytes * i + j������
			// (char *)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j
			lpDst = (char *)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// �����������ԴDIB�е�����
			i0 = i - lXOffset;
			j0 = j - lYOffset;
			
			// �ж��Ƿ���Դͼ��Χ��
			if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				// ָ��ԴDIB��i0�У���j0�����ص�ָ��
				// ͬ��Ҫע��DIB���µ��õ�����
				lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;
				
				// ��������
				*lpDst = *lpSrc;
			}
			else
			{
				// ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪ255
				* ((unsigned char*)lpDst) = 255;
			}
			
		}
	}
	
	// ����ƽ�ƺ��ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	
	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	// ����
	return TRUE;
}
HGLOBAL WINAPI RotateDIB(LPSTR lpDIB, int iRotateAngle)
{
	
	// Դͼ��Ŀ�Ⱥ͸߶�
	LONG	lWidth;
	LONG	lHeight;
	
	// ��ת��ͼ��Ŀ�Ⱥ͸߶�
	LONG	lNewWidth;
	LONG	lNewHeight;
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// ��ת��ͼ��Ŀ�ȣ�lNewWidth'��������4�ı�����
	LONG	lNewLineBytes;
	
	// ָ��Դͼ���ָ��
	LPSTR	lpDIBBits;
	
	// ָ��Դ���ص�ָ��
	LPSTR	lpSrc;
	
	// ��ת����DIB���
	HDIB	hDIB;
	
	// ָ����תͼ���Ӧ���ص�ָ��
	LPSTR	lpDst;
	
	// ָ����תͼ���ָ��
	LPSTR	lpNewDIB;
	LPSTR	lpNewDIBBits;
	
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFOHEADER lpbmi;
	
	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREHEADER lpbmc;
	
	// ѭ����������������DIB�е����꣩
	LONG	i;
	LONG	j;
	
	// ������ԴDIB�е�����
	LONG	i0;
	LONG	j0;
	
	// ��ת�Ƕȣ����ȣ�
	float	fRotateAngle;
	
	// ��ת�Ƕȵ����Һ�����
	float	fSina, fCosa;
	
	// Դͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float	fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;
	
	// ��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float	fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	
	// �����м䳣��
	float	f1,f2;
	
	// �ҵ�ԴDIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// ��ȡͼ���"���"��4�ı�����
	lWidth = ::DIBWidth(lpDIB);
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ��ȡͼ��ĸ߶�
	lHeight = ::DIBHeight(lpDIB);
	
	// ����ת�ǶȴӶ�ת��������
	fRotateAngle = (float) RADIAN(iRotateAngle);
	
	// ������ת�Ƕȵ�����
	fSina = (float) sin((double)fRotateAngle);
	
	// ������ת�Ƕȵ�����
	fCosa = (float) cos((double)fRotateAngle);
	
	// ����ԭͼ���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fSrcX1 = (float) (- (lWidth  - 1) / 2);
	fSrcY1 = (float) (  (lHeight - 1) / 2);
	fSrcX2 = (float) (  (lWidth  - 1) / 2);
	fSrcY2 = (float) (  (lHeight - 1) / 2);
	fSrcX3 = (float) (- (lWidth  - 1) / 2);
	fSrcY3 = (float) (- (lHeight - 1) / 2);
	fSrcX4 = (float) (  (lWidth  - 1) / 2);
	fSrcY4 = (float) (- (lHeight - 1) / 2);
	
	// ������ͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;
	
	// ������ת���ͼ��ʵ�ʿ��
	lNewWidth  = (LONG) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);
	
	// ������ͼ��ÿ�е��ֽ���
	lNewLineBytes = WIDTHBYTES(lNewWidth * 8);
	
	// ������ת���ͼ��߶�
	lNewHeight = (LONG) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) )  + 0.5);
	
	// �������������������Ժ�ÿ�ζ�������
	f1 = (float) (-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina
		+ 0.5 * (lWidth  - 1));
	f2 = (float) ( 0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa
		+ 0.5 * (lHeight - 1));
	
	// �����ڴ棬�Ա�����DIB
	hDIB = (HDIB) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));
	
	// �ж��Ƿ��ڴ����ʧ��
	if (hDIB == NULL)
	{
		// �����ڴ�ʧ��
		return NULL;
	}

	// �����ڴ�
	lpNewDIB =  (char * )::GlobalLock((HGLOBAL) hDIB);
	
	// ����DIB��Ϣͷ�͵�ɫ��
	memcpy(lpNewDIB, lpDIB, *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));
	
	// �ҵ���DIB������ʼλ��
	lpNewDIBBits = ::FindDIBBits(lpNewDIB);
	
	// ��ȡָ��
	lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;

	// ����DIB��ͼ��ĸ߶ȺͿ��
	if (IS_WIN30_DIB(lpNewDIB))
	{
		// ����Windows 3.0 DIB
		lpbmi->biWidth = lNewWidth;
		lpbmi->biHeight = lNewHeight;
	}
	else
	{
		// ����������ʽ��DIB
		lpbmc->bcWidth = (unsigned short) lNewWidth;
		lpbmc->bcHeight = (unsigned short) lNewHeight;
	}
	
	// ���ͼ��ÿ�н��в���
	for(i = 0; i < lNewHeight; i++)
	{
		// ���ͼ��ÿ�н��в���
		for(j = 0; j < lNewWidth; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			// ע��˴���Ⱥ͸߶�����DIB�Ŀ�Ⱥ͸߶�
			lpDst = (char *)lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;
			
			// �����������ԴDIB�е�����
			i0 = (LONG) (-((float) j) * fSina + ((float) i) * fCosa + f2 + 0.5);
			j0 = (LONG) ( ((float) j) * fCosa + ((float) i) * fSina + f1 + 0.5);
			
			// �ж��Ƿ���Դͼ��Χ��
			if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				// ָ��ԴDIB��i0�У���j0�����ص�ָ��
				lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;
				
				// ��������
				*lpDst = *lpSrc;
			}
			else
			{
				// ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪ255
				* ((unsigned char*)lpDst) = 255;
			}
			
		}
		
	}
	
	// ����
	return hDIB;
}

BYTE ** FillDibToArray(HDIB hDIB)
{
	BYTE* lpDIB=(BYTE*)::GlobalLock ((HGLOBAL)hDIB);
	BYTE* lpDIBBits=(BYTE*)::FindDIBBits((char*)lpDIB);
	BYTE* lpSrc=lpDIBBits;	
	LONG lHeight=::DIBHeight ((char*)lpDIB);
	LONG lWidth=::DIBWidth ((char*)lpDIB);
	BYTE** tempdibarray = alloc_2d_byt(lHeight,lWidth);
	LONG lLineBytes = WIDTHBYTES(lWidth * 8);
	for(int i=0;i<lHeight;i++)
		for(int j=0;j<lWidth;j++)
		{	lpSrc=(unsigned char*)lpDIBBits + lLineBytes * (lHeight- i-1) + j;
			tempdibarray[i][j]=(BYTE)(*lpSrc);
		}
	return tempdibarray;
}




HGLOBAL WINAPI ZoomDIB(LPSTR lpDIB, float fXZoomRatio, float fYZoomRatio)
{
	
	// Դͼ��Ŀ�Ⱥ͸߶�
	LONG	lWidth;
	LONG	lHeight;
	
	// ���ź�ͼ��Ŀ�Ⱥ͸߶�
	LONG	lNewWidth;
	LONG	lNewHeight;
	
	// ���ź�ͼ��Ŀ�ȣ�lNewWidth'��������4�ı�����
	LONG	lNewLineBytes;
	
	// ָ��Դͼ���ָ��
	LPSTR	lpDIBBits;
	
	// ָ��Դ���ص�ָ��
	LPSTR	lpSrc;
	
	// ���ź���DIB���
	HDIB	hDIB;
	
	// ָ������ͼ���Ӧ���ص�ָ��
	LPSTR	lpDst;
	
	// ָ������ͼ���ָ��
	LPSTR	lpNewDIB;
	LPSTR	lpNewDIBBits;
	
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFOHEADER lpbmi;
	
	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREHEADER lpbmc;
	
	// ѭ����������������DIB�е����꣩
	LONG	i;
	LONG	j;
	
	// ������ԴDIB�е�����
	LONG	i0;
	LONG	j0;
	
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// �ҵ�ԴDIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// ��ȡͼ��Ŀ��
	lWidth = ::DIBWidth(lpDIB);
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ��ȡͼ��ĸ߶�
	lHeight = ::DIBHeight(lpDIB);
	
	// �������ź��ͼ��ʵ�ʿ��
	// �˴�ֱ�Ӽ�0.5������ǿ������ת��ʱ���������룬����ֱ�ӽ�ȥС������
	lNewWidth = (LONG) (::DIBWidth(lpDIB) * fXZoomRatio + 0.5);
	
	// ������ͼ��ÿ�е��ֽ���
	lNewLineBytes = WIDTHBYTES(lNewWidth * 8);
	
	// �������ź��ͼ��߶�
	lNewHeight = (LONG) (lHeight * fYZoomRatio + 0.5);
	
	// �����ڴ棬�Ա�����DIB
	hDIB = (HDIB) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));
	
	// �ж��Ƿ��ڴ����ʧ��
	if (hDIB == NULL)
	{
		// �����ڴ�ʧ��
		return NULL;
	}
	
	// �����ڴ�
	lpNewDIB =  (char * )::GlobalLock((HGLOBAL) hDIB);
	
	// ����DIB��Ϣͷ�͵�ɫ��
	memcpy(lpNewDIB, lpDIB, *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));
	
	// �ҵ���DIB������ʼλ��
	lpNewDIBBits = ::FindDIBBits(lpNewDIB);
	
	// ��ȡָ��
	lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;
	
	// ����DIB��ͼ��ĸ߶ȺͿ��
	if (IS_WIN30_DIB(lpNewDIB))
	{
		// ����Windows 3.0 DIB
		lpbmi->biWidth = lNewWidth;
		lpbmi->biHeight = lNewHeight;
	}
	else
	{
		// ����������ʽ��DIB
		lpbmc->bcWidth = (unsigned short) lNewWidth;
		lpbmc->bcHeight = (unsigned short) lNewHeight;
	}
	
	// ���ͼ��ÿ�н��в���
	for(i = 0; i < lNewHeight; i++)
	{
		// ���ͼ��ÿ�н��в���
		for(j = 0; j < lNewWidth; j++)
		{
			
			// ָ����DIB��i�У���j�����ص�ָ��
			// ע��˴���Ⱥ͸߶�����DIB�Ŀ�Ⱥ͸߶�
			lpDst = (char *)lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;
			
			// �����������ԴDIB�е�����
			i0 = (LONG) (i / fYZoomRatio + 0.5);
			j0 = (LONG) (j / fXZoomRatio + 0.5);
			
			// �ж��Ƿ���Դͼ��Χ��
			if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				
				// ָ��ԴDIB��i0�У���j0�����ص�ָ��
				lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;
				
				// ��������
				*lpDst = *lpSrc;
			}
			else
			{
				// ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪ255
				* ((unsigned char*)lpDst) = 255;
			}
			
		}
		
	}
	
	// ����
	return hDIB;
}



HDIB  Template(HDIB hDIB,double * tem ,int tem_w,int tem_h,double xishu)
{

    //ͳ���м�ֵ
    double sum;

    //ָ��ͼ����ʼλ�õ�ָ��
    BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) hDIB);

	//ָ��������ʼλ�õ�ָ��
	BYTE *pScrBuff =(BYTE*)::FindDIBBits((char*)lpDIB);
   
	//��ȡͼ�����ɫ��Ϣ
    int numColors=(int) ::DIBNumColors((char *)lpDIB);

    //���ͼ����256ɫ����
     if (numColors!=256) 
	 {   
        //�������
	  	::GlobalUnlock((HGLOBAL) hDIB);

		//����
		return(hDIB);
	 }
    
    //��ָ��ͼ��������ʼλ�õ�ָ�룬��ֵ��ָ�����
    BYTE* oldbuf = pScrBuff;

    //ѭ������
    int i,j,m,n;

	int w, h, dw;

	//��ȡͼ��Ŀ��
	w = (int) ::DIBWidth((char *)lpDIB);
	
	//��ȡͼ��ĸ߶�
	h = (int) ::DIBHeight((char *)lpDIB);
	
	//����ͼ��ÿ�е��ֽ���
	dw = (w+3)/4*4;      
	
	//����һ����ԭͼ���С��ͬ��25ɫ�Ҷ�λͼ
    HDIB newhDIB=NewDIB(w,h,8);  
    
	//ָ���µ�λͼ��ָ��
	BYTE *newlpDIB=(BYTE*)::GlobalLock((HGLOBAL) newhDIB);

	//ָ���µ�λͼ��������ʼλ�õ�ָ�� 
    BYTE *destBuf = (BYTE*)FindDIBBits((char *)newlpDIB);
    
   //��ָ����ͼ��������ʼλ�õ�ָ�룬��ֵ��ָ�����
    BYTE *newbuf=destBuf; 
	
	//��ͼ�����ɨ��
   
	//�� 
    for(i=0;i<h;i++)
    {  
		//��
	   for(j=0;j<w;j++)
	   {   

		   //Ϊͳ�Ʊ�������ʼֵ
	       sum=0;

         //����ͼ���4���߿�����ر���ԭ�ҶȲ���
	     if( j<((tem_w-1)/2) || j>(w-(tem_w+1)/2) || i<((tem_h-1)/2) || i>(h-(tem_h+1)/2) )
	      *(newbuf+i*dw+j)=*(oldbuf+i*dw+j);
		 
         //�������������ؽ���ģ�����
		 else 
         { 

          //���㣨i,j������Ϊģ�������
          for(m=i-((tem_h-1)/2);m<=i+((tem_h-1)/2);m++)
          {
		     for(n=j-((tem_w-1)/2);n<=j+((tem_w-1)/2);n++)
		    
             //���Ե㣨i��j��Ϊ���ģ���ģ���С��ͬ�ķ�Χ�ڵ�������ģ�����λ�õ�ϵ��
			 //������˲����ε���
		     sum+=*(oldbuf+m*dw+n)* tem[(m-i+((tem_h-1)/2))*tem_w+n-j+((tem_w-1)/2)];
			
		  }

          
		  //����������ܵ�ģ��ϵ��
          sum=(int)sum*xishu;

		  //�������ֵ
		  sum = fabs(sum);
		 
		  //���С��0��ǿ�Ƹ�ֵΪ0
          if(sum<0)     
          sum=0;

		  //�������255��ǿ�Ƹ�ֵΪ255
          if(sum>255)
		  sum=255;

		  //������Ľ���ŵ��µ�λͼ����Ӧλ��
	      *(newbuf+i*dw+j)=sum;
		 }
	   }
	} 
   
	//�������
	::GlobalUnlock((HGLOBAL)hDIB);
    
	//�����µ�λͼ�ľ��
    return(newhDIB);
}
void ThinnerRosenfeld(void *image, unsigned long lx, unsigned long ly)
{
    char *f, *g;
    char n[10];
    char a[5] = {0, -1, 1, 0, 0};
    char b[5] = {0, 0, 0, 1, -1};
    char nrnd, cond, n48, n26, n24, n46, n68, n82, n123, n345, n567, n781;
    short k, shori;
    unsigned long i, j;
    long ii, jj, kk, kk1, kk2, kk3, size;
    size = (long)lx * (long)ly;

    g = (char *)malloc(size);
    if(g==NULL)
    {
        printf("error in alocating mmeory!\n");
        return;
    }

    f = (char *)image;
    for(kk=0l; kk<size; kk++)
    {
        g[kk] = f[kk];
    }

    do
    {
        shori = 0;
        for(k=1; k<=4; k++)
        {
            for(i=1; i<lx-1; i++)
            {
                ii = i + a[k];

                for(j=1; j<ly-1; j++)
                {
                    kk = i*ly + j;

                    if(!f[kk])
                        continue;

                    jj = j + b[k];
                    kk1 = ii*ly + jj;

                    if(f[kk1])
                        continue;

                    kk1 = kk - ly -1;
                    kk2 = kk1 + 1;
                    kk3 = kk2 + 1;
                    n[3] = f[kk1];
                    n[2] = f[kk2];
                    n[1] = f[kk3];
                    kk1 = kk - 1;
                    kk3 = kk + 1;
                    n[4] = f[kk1];
                    n[8] = f[kk3];
                    kk1 = kk + ly - 1;
                    kk2 = kk1 + 1;
                    kk3 = kk2 + 1;
                    n[5] = f[kk1];
                    n[6] = f[kk2];
                    n[7] = f[kk3];

                    nrnd = n[1] + n[2] + n[3] + n[4]
                        +n[5] + n[6] + n[7] + n[8];
                    if(nrnd<=1)
                        continue;

                    cond = 0;
                    n48 = n[4] + n[8];
                    n26 = n[2] + n[6];
                    n24 = n[2] + n[4];
                    n46 = n[4] + n[6];
                    n68 = n[6] + n[8];
                    n82 = n[8] + n[2];
                    n123 = n[1] + n[2] + n[3];
                    n345 = n[3] + n[4] + n[5];
                    n567 = n[5] + n[6] + n[7];
                    n781 = n[7] + n[8] + n[1];

                    if(n[2]==1 && n48==0 && n567>0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[6]==1 && n48==0 && n123>0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[8]==1 && n26==0 && n345>0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[4]==1 && n26==0 && n781>0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[5]==1 && n46==0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[7]==1 && n68==0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[1]==1 && n82==0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[3]==1 && n24==0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    cond = 1;
                    if(!cond)
                        continue;
                    g[kk] = 0;
                    shori = 1;
                }
            }

            for(i=0; i<lx; i++)
            {
                for(j=0; j<ly; j++)
                {
                    kk = i*ly + j;
                    f[kk] = g[kk];
                }
            }
        }
    }while(shori);

    free(g);
}              


void Thinning(HDIB hDIB)
{
	// ָ��DIB��ָ��
	LPSTR lpDIB=(LPSTR) ::GlobalLock((HGLOBAL)hDIB);
	
	// ָ��DIB����ָ��
	LPSTR    lpDIBBits;	

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);	
	
	// ָ��Դͼ���ָ��
	LPSTR	lpSrc;

	//ͼ��ĸ߶ȺͿ��
	LONG lWidth;
	LONG lHeight;

	//��ȡͼ��Ŀ��
	lWidth=::DIBWidth ((char*)lpDIB);

	//��ȡͼ��ĸ߶�
	lHeight=::DIBHeight ((char*)lpDIB);

    //����ÿ�е��ֽ���
    LONG  lLineBytes = (lWidth+3)*4/4;

	//ѭ������
    int i,j;

    //�����洢�����2ֵ����
	BYTE *image = (BYTE*)malloc(lWidth*lHeight*sizeof(BYTE));

	//��2ֵ����ֵ

	for(i=0;i<lHeight;i++)
	{
	    for(j=0;j<lWidth;j++)
		{
	      lpSrc=lpDIBBits+i*lLineBytes+j;
	    
	      if(*lpSrc==0)
	      image[i*lWidth+j]=1;

		  else
         image[i*lWidth+j]=0;
		}
	}

   //���ú�������ϸ��,���ֺ�������ѡ��

   ThinnerRosenfeld(image,lHeight,lWidth);

   //ThinnerHilditch(image,lHeight,lWidth);



   //�������ֵ��ԭͼ����

   for(i=0;i<lHeight;i++)
   {
     for(j=0;j<lWidth;j++)
	 {
	   lpSrc=lpDIBBits+i*lLineBytes+j;

       if(image[i*lWidth+j]==1)
	   *lpSrc=(BYTE)0;

	   else
       *lpSrc=(BYTE)255;
	 }
   
   }


 //����ڴ�
   free(image);
   ::GlobalUnlock ((HGLOBAL)hDIB);

	return;
}





HDIB  MaxTemplate(HDIB hDIB,double * tem ,int tem_w,int tem_h,double xishu,int count)
{

    //ͳ���м�ֵ
    double sum;

    //ָ��ͼ����ʼλ�õ�ָ��
    BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) hDIB);

	//ָ��������ʼλ�õ�ָ��
	BYTE *pScrBuff =(BYTE*)::FindDIBBits((char*)lpDIB);
   
	//��ȡͼ�����ɫ��Ϣ
    int numColors=(int) ::DIBNumColors((char *)lpDIB);

    //���ͼ����256ɫ����
     if (numColors!=256) 
	 {   
        //�������
	  	::GlobalUnlock((HGLOBAL) hDIB);

		//����
		return(hDIB);
	 }
    
    //��ָ��ͼ��������ʼλ�õ�ָ�룬��ֵ��ָ�����
    BYTE* oldbuf = pScrBuff;

    //ѭ������
    int i,j,m,n,k,temp=0;

	int w, h, dw;

	//��ȡͼ��Ŀ��
	w = (int) ::DIBWidth((char *)lpDIB);
	
	//��ȡͼ��ĸ߶�
	h = (int) ::DIBHeight((char *)lpDIB);
	
	//����ͼ��ÿ�е��ֽ���
	dw = (w+3)/4*4;      
	
	//����һ����ԭͼ���С��ͬ��25ɫ�Ҷ�λͼ
    HDIB newhDIB=NewDIB(w,h,8);  
    
	//ָ���µ�λͼ��ָ��
	BYTE *newlpDIB=(BYTE*)::GlobalLock((HGLOBAL) newhDIB);

	//ָ���µ�λͼ��������ʼλ�õ�ָ�� 
    BYTE *destBuf = (BYTE*)FindDIBBits((char *)newlpDIB);
    
   //��ָ����ͼ��������ʼλ�õ�ָ�룬��ֵ��ָ�����
    BYTE *newbuf=destBuf; 
	
	//��ͼ�����ɨ��
   
	//�� 
    for(i=0;i<h;i++)
    {  
		//��
	   for(j=0;j<w;j++)
	   {   
		
		   //Ϊͳ�Ʊ�������ʼֵ
	       sum=0;
		for(k=0;k<count;k++)
         //����ͼ���4���߿�����ر���ԭ�ҶȲ���
			{
				 if( j<((tem_w-1)/2) || j>(w-(tem_w+1)/2) || i<((tem_h-1)/2) || i>(h-(tem_h+1)/2) )
					*(newbuf+i*dw+j)=*(oldbuf+i*dw+j);
		 
         //�������������ؽ���ģ�����
					else 
						{ 

          //���㣨i,j������Ϊģ�������
							for(m=i-((tem_h-1)/2);m<=i+((tem_h-1)/2);m++)
							{
								for(n=j-((tem_w-1)/2);n<=j+((tem_w-1)/2);n++)
		    
             //���Ե㣨i��j��Ϊ���ģ���ģ���С��ͬ�ķ�Χ�ڵ�������ģ�����λ�õ�ϵ��
			 //������˲����ε���
									sum+=*(oldbuf+m*dw+n)* tem[k*9+(m-i+((tem_h-1)/2))*tem_w+n-j+((tem_w-1)/2)];
			
							}
        
		  //����������ܵ�ģ��ϵ��
				          sum=(int)sum*xishu;

		  //�������ֵ
							sum = fabs(sum);
		 		  //���С��0��ǿ�Ƹ�ֵΪ0
							 if(sum<0)     
							sum=0;

		  //�������255��ǿ�Ƹ�ֵΪ255
							if(sum>255)
							sum=255;
							if(sum>=temp)
								temp=sum;
					}
		  //������Ľ���ŵ��µ�λͼ����Ӧλ��
	      *(newbuf+i*dw+j)=temp;
		  temp=0;
		 }
	   }
	} 
   
	//�������
	::GlobalUnlock((HGLOBAL)hDIB);
    
	//�����µ�λͼ�ľ��
    return(newhDIB);
}













BOOL WINAPI PaintDIB(HDC     hDC,
					LPRECT  lpDCRect,
					HDIB    hDIB,
					LPRECT  lpDIBRect,
					CPalette* pPal)
{
	LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
	LPSTR    lpDIBBits;           // Pointer to DIB bits
	BOOL     bSuccess=FALSE;      // Success/fail flag
	HPALETTE hPal=NULL;           // Our DIB's palette
	HPALETTE hOldPal=NULL;        // Previous palette

	/* Check for valid DIB handle */
	if (hDIB == NULL)
		return FALSE;

	/* Lock down the DIB, and get a pointer to the beginning of the bit
	 *  buffer
	 */
	lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	lpDIBBits = ::FindDIBBits(lpDIBHdr);

	// Get the DIB's palette, then select it into DC
	if (pPal != NULL)
	{
		hPal = (HPALETTE) pPal->m_hObject;

		// Select as background since we have
		// already realized in forground if needed
		hOldPal = ::SelectPalette(hDC, hPal, TRUE);
	}

	/* Make sure to use the stretching mode best for color pictures */
	::SetStretchBltMode(hDC, COLORONCOLOR);

	/* Determine whether to call StretchDIBits() or SetDIBitsToDevice() */
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
	   (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
		bSuccess = ::SetDIBitsToDevice(hDC,                    // hDC
								   lpDCRect->left,             // DestX
								   lpDCRect->top,              // DestY
								   RECTWIDTH(lpDCRect),        // nDestWidth
								   RECTHEIGHT(lpDCRect),       // nDestHeight
								   lpDIBRect->left,            // SrcX
								   (int)DIBHeight(lpDIBHdr) -
									  lpDIBRect->top -
									  RECTHEIGHT(lpDIBRect),   // SrcY
								   0,                          // nStartScan
								   (WORD)DIBHeight(lpDIBHdr),  // nNumScans
								   lpDIBBits,                  // lpBits
								   (LPBITMAPINFO)lpDIBHdr,     // lpBitsInfo
								   DIB_RGB_COLORS);            // wUsage
   else
	  bSuccess = ::StretchDIBits(hDC,                          // hDC
							   lpDCRect->left,                 // DestX
							   lpDCRect->top,                  // DestY
							   RECTWIDTH(lpDCRect),            // nDestWidth
							   RECTHEIGHT(lpDCRect),           // nDestHeight
							   lpDIBRect->left,                // SrcX
							   lpDIBRect->top,                 // SrcY
							   RECTWIDTH(lpDIBRect),           // wSrcWidth
							   RECTHEIGHT(lpDIBRect),          // wSrcHeight
							   lpDIBBits,                      // lpBits
							   (LPBITMAPINFO)lpDIBHdr,         // lpBitsInfo
							   DIB_RGB_COLORS,                 // wUsage
							   SRCCOPY);                       // dwROP

   ::GlobalUnlock((HGLOBAL) hDIB);

	/* Reselect old palette */
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}

   return bSuccess;
}

/*************************************************************************
 *
 * CreateDIBPalette()
 *
 * Parameter:
 *
 * HDIB hDIB        - specifies the DIB
 *
 * Return Value:
 *
 * HPALETTE         - specifies the palette
 *
 * Description:
 *
 * This function creates a palette from a DIB by allocating memory for the
 * logical palette, reading and storing the colors from the DIB's color table
 * into the logical palette, creating a palette from this logical palette,
 * and then returning the palette's handle. This allows the DIB to be
 * displayed using the best possible colors (important for DIBs with 256 or
 * more colors).
 *
 ************************************************************************/


BOOL WINAPI CreateDIBPalette(HDIB hDIB, CPalette* pPal)
{
	LPLOGPALETTE lpPal;      // pointer to a logical palette
	HANDLE hLogPal;          // handle to a logical palette
	HPALETTE hPal = NULL;    // handle to a palette
	int i;                   // loop index
	WORD wNumColors;         // number of colors in color table
	LPSTR lpbi;              // pointer to packed-DIB
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPBITMAPCOREINFO lpbmc;  // pointer to BITMAPCOREINFO structure (old)
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	BOOL bResult = FALSE;

	/* if handle to DIB is invalid, return FALSE */

	if (hDIB == NULL)
	  return FALSE;

   lpbi = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

   /* get pointer to BITMAPINFO (Win 3.0) */
   lpbmi = (LPBITMAPINFO)lpbi;

   /* get pointer to BITMAPCOREINFO (old 1.x) */
   lpbmc = (LPBITMAPCOREINFO)lpbi;

   /* get the number of colors in the DIB */
   wNumColors = ::DIBNumColors(lpbi);

   if (wNumColors != 0)
   {
		/* allocate memory block for logical palette */
		hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)
									+ sizeof(PALETTEENTRY)
									* wNumColors);

		/* if not enough memory, clean up and return NULL */
		if (hLogPal == 0)
		{
			::GlobalUnlock((HGLOBAL) hDIB);
			return FALSE;
		}

		lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);

		/* set version and number of palette entries */
		lpPal->palVersion = PALVERSION;
		lpPal->palNumEntries = (WORD)wNumColors;

		/* is this a Win 3.0 DIB? */
		bWinStyleDIB = IS_WIN30_DIB(lpbi);
		for (i = 0; i < (int)wNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;
				lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
				lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;
				lpPal->palPalEntry[i].peFlags = 0;
			}
			else
			{
				lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
				lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
				lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;
				lpPal->palPalEntry[i].peFlags = 0;
			}
		}

		/* create the palette and get handle to it */
		bResult = pPal->CreatePalette(lpPal);
		::GlobalUnlock((HGLOBAL) hLogPal);
		::GlobalFree((HGLOBAL) hLogPal);
	}

	::GlobalUnlock((HGLOBAL) hDIB);

	return bResult;
}

/*************************************************************************
 *
 * FindDIBBits()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * LPSTR            - pointer to the DIB bits
 *
 * Description:
 *
 * This function calculates the address of the DIB's bits and returns a
 * pointer to the DIB bits.
 *
 ************************************************************************/


LPSTR WINAPI FindDIBBits(LPSTR lpbi)
{
	return (lpbi + *(LPDWORD)lpbi + ::PaletteSize(lpbi));
}


/*************************************************************************
 *
 * DIBWidth()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * DWORD            - width of the DIB
 *
 * Description:
 *
 * This function gets the width of the DIB from the BITMAPINFOHEADER
 * width field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER
 * width field if it is an other-style DIB.
 *
 ************************************************************************/


DWORD WINAPI DIBWidth(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether Win 3.0 and old) */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB width if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biWidth;
	else  /* it is an other-style DIB, so return its width */
		return (DWORD)lpbmc->bcWidth;
}


/*************************************************************************
 *
 * DIBHeight()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * DWORD            - height of the DIB
 *
 * Description:
 *
 * This function gets the height of the DIB from the BITMAPINFOHEADER
 * height field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER
 * height field if it is an other-style DIB.
 *
 ************************************************************************/


DWORD WINAPI DIBHeight(LPSTR lpDIB)
{
	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB

	/* point to the header (whether old or Win 3.0 */

	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	/* return the DIB height if it is a Win 3.0 DIB */
	if (IS_WIN30_DIB(lpDIB))
		return lpbmi->biHeight;
	else  /* it is an other-style DIB, so return its height */
		return (DWORD)lpbmc->bcHeight;
}


/*************************************************************************
 *
 * PaletteSize()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - size of the color palette of the DIB
 *
 * Description:
 *
 * This function gets the size required to store the DIB's palette by
 * multiplying the number of colors by the size of an RGBQUAD (for a
 * Windows 3.0-style DIB) or by the size of an RGBTRIPLE (for an other-
 * style DIB).
 *
 ************************************************************************/


WORD WINAPI PaletteSize(LPSTR lpbi)
{
   /* calculate the size required by the palette */
   if (IS_WIN30_DIB (lpbi))
	  return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBQUAD));
   else
	  return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
}


/*************************************************************************
 *
 * DIBNumColors()
 *
 * Parameter:
 *
 * LPSTR lpbi       - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - number of colors in the color table
 *
 * Description:
 *
 * This function calculates the number of colors in the DIB's color table
 * by finding the bits per pixel for the DIB (whether Win3.0 or other-style
 * DIB). If bits per pixel is 1: colors=2, if 4: colors=16, if 8: colors=256,
 * if 24, no colors in color table.
 *
 ************************************************************************/


WORD WINAPI DIBNumColors(LPSTR lpbi)
{
	WORD wBitCount;  // DIB bit count

	/*  If this is a Windows-style DIB, the number of colors in the
	 *  color table can be less than the number of bits per pixel
	 *  allows for (i.e. lpbi->biClrUsed can be set to some value).
	 *  If this is the case, return the appropriate value.
	 */

	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;

		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		if (dwClrUsed != 0)
			return (WORD)dwClrUsed;
	}

	/*  Calculate the number of colors in the color table based on
	 *  the number of bits per pixel for the DIB.
	 */
	if (IS_WIN30_DIB(lpbi))
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	else
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;

	/* return number of colors based on bits per pixel */
	switch (wBitCount)
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
	}
}


//////////////////////////////////////////////////////////////////////////
//// Clipboard support

//---------------------------------------------------------------------
//
// Function:   CopyHandle (from SDK DibView sample clipbrd.c)
//
// Purpose:    Makes a copy of the given global memory block.  Returns
//             a handle to the new memory block (NULL on error).
//
//             Routine stolen verbatim out of ShowDIB.
//
// Parms:      h == Handle to global memory to duplicate.
//
// Returns:    Handle to new global memory block.
//
//---------------------------------------------------------------------

HGLOBAL WINAPI CopyHandle (HGLOBAL h)
{
	if (h == NULL)
		return NULL;

	DWORD dwLen = ::GlobalSize((HGLOBAL) h);
	HGLOBAL hCopy = ::GlobalAlloc(GHND, dwLen);

	if (hCopy != NULL)
	{
		void* lpCopy = ::GlobalLock((HGLOBAL) hCopy);
		void* lp     = ::GlobalLock((HGLOBAL) h);
		memcpy(lpCopy, lp, dwLen);
		::GlobalUnlock(hCopy);
		::GlobalUnlock(h);
	}

	return hCopy;
}
void WINAPI ConvertShort(unsigned short *array, long length)
{
   unsigned long b1, b2;
   unsigned char *ptr;

   ptr = (unsigned char *)array;
   while (length--) {
      b1 = *ptr++;
      b2 = *ptr++;
      *array++ = (unsigned short)((b1 << 8) | (b2));
   }
}

void WINAPI ConvertLong(unsigned long  *array, long length)
{
   unsigned long b1, b2, b3, b4;
   unsigned char *ptr;

   ptr = (unsigned char *)array;
   while (length--) {
      b1 = *ptr++;
      b2 = *ptr++;
      b3 = *ptr++;
      b4 = *ptr++;
      *array++ = (b1 << 24) | (b2 << 16) | (b3 << 8) | (b4);
   }
}


/*************************************************************************
 *
 * SaveDIB()
 *
 * Saves the specified DIB into the specified CFile.  The CFile
 * is opened and closed by the caller.
 *
 * Parameters:
 *
 * HDIB hDib - Handle to the dib to save
 *
 * CFile& file - open CFile used to save DIB
 *
 * Return value: TRUE if successful, else FALSE or CFileException
 *
 *************************************************************************/


BOOL WINAPI SaveDIB(HDIB hDib, CFile& file)
{
	BITMAPFILEHEADER bmfHdr; // Header for Bitmap file
	LPBITMAPINFOHEADER lpBI;   // Pointer to DIB info structure
	DWORD dwDIBSize;

	if (hDib == NULL)
		return FALSE;

	/*
	 * Get a pointer to the DIB memory, the first of which contains
	 * a BITMAPINFO structure
	 */
	lpBI = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDib);
	if (lpBI == NULL)
		return FALSE;

	if (!IS_WIN30_DIB(lpBI))
	{
		::GlobalUnlock((HGLOBAL) hDib);
		return FALSE;       // It's an other-style DIB (save not supported)
	}

	/*
	 * Fill in the fields of the file header
	 */

	/* Fill in file type (first 2 bytes must be "BM" for a bitmap) */
	bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"

	// Calculating the size of the DIB is a bit tricky (if we want to
	// do it right).  The easiest way to do this is to call GlobalSize()
	// on our global handle, but since the size of our global memory may have
	// been padded a few bytes, we may end up writing out a few too
	// many bytes to the file (which may cause problems with some apps).
	//
	// So, instead let's calculate the size manually (if we can)
	//
	// First, find size of header plus size of color table.  Since the
	// first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains
	// the size of the structure, let's use this.

	dwDIBSize = *(LPDWORD)lpBI + ::PaletteSize((LPSTR)lpBI);  // Partial Calculation

	// Now calculate the size of the image

	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
	{
		// It's an RLE bitmap, we can't calculate size, so trust the
		// biSizeImage field

		dwDIBSize += lpBI->biSizeImage;
	}
	else
	{
		DWORD dwBmBitsSize;  // Size of Bitmap Bits only

		// It's not RLE, so size is Width (DWORD aligned) * Height

		dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;

		dwDIBSize += dwBmBitsSize;

		// Now, since we have calculated the correct size, why don't we
		// fill in the biSizeImage field (this will fix any .BMP files which
		// have this field incorrect).

		lpBI->biSizeImage = dwBmBitsSize;
	}


	// Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)

	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	/*
	 * Now, calculate the offset the actual bitmap bits will be in
	 * the file -- It's the Bitmap file header plus the DIB header,
	 * plus the size of the color table.
	 */
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize
											  + PaletteSize((LPSTR)lpBI);
#ifdef _MAC
	ByteSwapHeader(&bmfHdr);

	// First swap the size field
	*((LPDWORD)lpBI) = SWAPLONG(*((LPDWORD)lpBI));

	// Now swap the rest of the structure (we don't save < Win30 files)
	ByteSwapInfo((LPSTR)lpBI, TRUE);
#endif

	TRY
	{
		// Write the file header
		file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
		//
		// Write the DIB header and the bits
		//
		file.WriteHuge(lpBI, dwDIBSize);
	}
	CATCH (CFileException, e)
	{
#ifdef _MAC
		// Swap everything back
		*((LPDWORD)lpBI) = SWAPLONG(*((LPDWORD)lpBI));
		ByteSwapInfo((LPSTR)lpBI, TRUE);
#endif
		::GlobalUnlock((HGLOBAL) hDib);
		THROW_LAST();
	}
	END_CATCH

#ifdef _MAC
	// Swap everything back
	*((LPDWORD)lpBI) = SWAPLONG(*((LPDWORD)lpBI));
	ByteSwapInfo((LPSTR)lpBI, TRUE);
#endif

	::GlobalUnlock((HGLOBAL) hDib);
	return TRUE;
}


/*************************************************************************

  Function:  ReadDIBFile (CFile&)

   Purpose:  Reads in the specified DIB file into a global chunk of
			 memory.

   Returns:  A handle to a dib (hDIB) if successful.
			 NULL if an error occurs.

  Comments:  BITMAPFILEHEADER is stripped off of the DIB.  Everything
			 from the end of the BITMAPFILEHEADER structure on is
			 returned in the global memory handle.

*************************************************************************/


HDIB WINAPI ReadDIBFile(CFile& file)
{
	BITMAPFILEHEADER bmfHeader;
	DWORD dwBitsSize;
	HDIB hDIB;
	LPSTR pDIB;

	/*
	 * get length of DIB in bytes for use when reading
	 */

	dwBitsSize = file.GetLength();

	/*
	 * Go read the DIB file header and check if it's valid.
	 */
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return NULL;

#ifdef _MAC
	ByteSwapHeader(&bmfHeader);
#endif
	if (bmfHeader.bfType != DIB_HEADER_MARKER)
		return NULL;

	/*
	 * Allocate memory for DIB
	 */
	hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	if (hDIB == 0)
	{
		return NULL;
	}
	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

	/*
	 * Go read the bits.
	 */
	if (file.ReadHuge(pDIB, dwBitsSize - sizeof(BITMAPFILEHEADER)) !=
		dwBitsSize - sizeof(BITMAPFILEHEADER) )
	{
		::GlobalUnlock((HGLOBAL) hDIB);
		::GlobalFree((HGLOBAL) hDIB);
		return NULL;
	}
#ifdef _MAC
	// First swap the size field
	*((LPDWORD)pDIB) = SWAPLONG(*((LPDWORD)pDIB));

	// Now swap the rest of the structure
	ByteSwapInfo(pDIB, IS_WIN30_DIB(pDIB));
#endif
	::GlobalUnlock((HGLOBAL) hDIB);
	return hDIB;
}
//  add Nov.27,1999
HDIB WINAPI NewDIB(long width, long height,unsigned short biBitCount) // add "biBitCount" by hchen Jan 25,00
{
	long dwindth = (width*biBitCount/8+3)/4*4; // align 4 bytes
	WORD color_num;
	switch(biBitCount)
	{
		case 1:
			color_num=2;
			break;

		case 4:
			color_num=16;
			break;
		case 8:
			color_num=256;
			break;
		default:
			color_num=0;
			break;
	}

	long dwBitsSize = dwindth *height + 40 + color_num*4;// 40 LPBIMHEAD

	LPSTR pDIB;
	HDIB hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	if (hDIB == 0)
	{
		return NULL;
	}
	pDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	
	LPBITMAPINFO lpmf = (LPBITMAPINFO)pDIB;
	lpmf->bmiHeader.biSize = 40;//40
	lpmf->bmiHeader.biWidth = width; //i_info.width;
    lpmf->bmiHeader.biHeight = height;
    lpmf->bmiHeader.biPlanes = 1;
	lpmf->bmiHeader.biBitCount = biBitCount;// 24
	lpmf->bmiHeader.biCompression = 0;
    lpmf->bmiHeader.biSizeImage = dwindth *height;
	lpmf->bmiHeader.biXPelsPerMeter = 2925;
    lpmf->bmiHeader.biYPelsPerMeter = 2925;
    lpmf->bmiHeader.biClrUsed = 0; 
	lpmf->bmiHeader.biClrImportant= 0; 	

	if(color_num!=0)
	{
		for(int i=0;i<color_num;i++)
		{
			lpmf->bmiColors[i].rgbRed =(BYTE)i;
			lpmf->bmiColors[i].rgbGreen =(BYTE)i;
			lpmf->bmiColors[i].rgbBlue =(BYTE)i;
		}
	}

	::GlobalUnlock((HGLOBAL) hDIB);
	
	return hDIB;
}

#ifdef _MAC
void ByteSwapHeader(BITMAPFILEHEADER* bmfHeader)
{
	bmfHeader->bfType = SWAPWORD(bmfHeader->bfType);
	bmfHeader->bfSize = SWAPLONG(bmfHeader->bfSize);
	bmfHeader->bfOffBits = SWAPLONG(bmfHeader->bfOffBits);
}


void ByteSwapInfo(LPSTR lpHeader, BOOL fWin30Header)
{
	// Note this doesn't swap the bcSize/biSize field.  It assumes that the
	// size field was swapped during read or while setting the fWin30Header
	// flag.

	if (fWin30Header)
	{
		LPBITMAPINFOHEADER lpBMIH = &(LPBITMAPINFO(lpHeader)->bmiHeader);

		//lpBMIH->biSize = SWAPLONG(lpBMIH->biSize);
		lpBMIH->biWidth = SWAPLONG(lpBMIH->biWidth);
		lpBMIH->biHeight = SWAPLONG(lpBMIH->biHeight);
		lpBMIH->biPlanes = SWAPWORD(lpBMIH->biPlanes);
		lpBMIH->biBitCount = SWAPWORD(lpBMIH->biBitCount);
		lpBMIH->biCompression = SWAPLONG(lpBMIH->biCompression);
		lpBMIH->biSizeImage = SWAPLONG(lpBMIH->biSizeImage);
		lpBMIH->biXPelsPerMeter = SWAPLONG(lpBMIH->biXPelsPerMeter);
		lpBMIH->biYPelsPerMeter = SWAPLONG(lpBMIH->biYPelsPerMeter);
		lpBMIH->biClrUsed = SWAPLONG(lpBMIH->biClrUsed);
		lpBMIH->biClrImportant = SWAPLONG(lpBMIH->biClrImportant);
	}
	else
	{
		LPBITMAPCOREHEADER lpBMCH = &(LPBITMAPCOREINFO(lpHeader)->bmciHeader);

		lpBMCH->bcWidth = SWAPWORD(lpBMCH->bcWidth);
		lpBMCH->bcHeight = SWAPWORD(lpBMCH->bcHeight);
		lpBMCH->bcPlanes = SWAPWORD(lpBMCH->bcPlanes);
		lpBMCH->bcBitCount = SWAPWORD(lpBMCH->bcBitCount);
	}
}











#endif


