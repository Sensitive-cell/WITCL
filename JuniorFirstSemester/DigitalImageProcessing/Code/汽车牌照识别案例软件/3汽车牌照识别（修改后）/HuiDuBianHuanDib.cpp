#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "HuiDuBianHuanDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"
 
HuiDuBianHuanDib::HuiDuBianHuanDib()
{
}
HuiDuBianHuanDib::~HuiDuBianHuanDib()
{
}

///***************************************************************/           
/*�������ƣ�FanSeBianHuan()                                        
/*�������ͣ�void                                           
/*���ܣ���ͼ����з�ɫ�任                            
/***************************************************************/
void HuiDuBianHuanDib::FanSeBianHuan()//��ͼ����з�ɫ�任
{
	LPBYTE  p_data;     //ԭͼ������ָ��
	int wide,height;    //ԭͼ������
	p_data=this->GetData ();     //ȡ��ԭͼ��������ָ��
	if(m_pBitmapInfoHeader->biBitCount<9)		
		wide=this->GetWidth ();
	else
		wide=this->GetDibWidthBytes();
	height=this->GetHeight ();
	for(int i=0;i<height*wide;i++)
	{
		unsigned char  temp;
		temp=*(p_data+i);
		*(p_data+i)=255- temp;
	}	
}

///***************************************************************/           
/*�������ƣ�Fei0()												*/
/*�������ͣ�void												*/
/*���ܣ���ͼ����з���ȡһ���㡣								*/
/****************************************************************/
void HuiDuBianHuanDib::Fei0()//��ͼ����з���ȡһ����
{
	LPBYTE p_data;     //ԭͼ������ָ��
	int wide,height;    //ԭͼ������
	p_data=this->GetData ();     //ȡ��ԭͼ��������ָ��
	if(m_pBitmapInfoHeader->biBitCount<9)		
		wide=this->GetWidth ();
	else
		wide=this->GetDibWidthBytes();
	height=this->GetHeight ();
	for(int j=0;j<height;j++)
	{
		for(int i=0;i<wide;i++)    //������������ѭ��
		{
			if(*p_data!=0)          //������ֵ��Ϊ0
				*p_data=255;         //������Ϊ255
			p_data++; 				 
		}
	}
}

/***************************************************************/           
/*�������ƣ�GuDing(int YuZhi)                                  */      
/*�������ͣ�void											   */	
/*����˵����YuZhi ������ֵ                                     */       
/*���ܣ���ͼ����й̶���ֵ���㡣                               */
/***************************************************************/
void HuiDuBianHuanDib::GuDing(int YuZhi)//��ͼ����й̶���ֵ����
{
	LPBYTE  p_data;     //ԭͼ������ָ��
	int wide,height;    //ԭͼ������
	p_data=this->GetData ();     //ȡ��ԭͼ��������ָ��
	if(m_pBitmapInfoHeader->biBitCount<9)		
		wide=this->GetWidth ();
	else
		wide=this->GetDibWidthBytes();
	height=this->GetHeight ();
	for(int j=0;j<height;j++)
	{
		for(int i=0;i<wide;i++)
		{
			if(*p_data>YuZhi)          //�Ҷ�ֵ���ڸ�����ֵ����Ϊ255
				*p_data=255;
			else
				*p_data=0;             //��������Ϊ0
			p_data++;
		}
	}
}











//����Ӧ��
void HuiDuBianHuanDib::zishiying()//��ͼ����й̶���ֵ����
{
	LPBYTE  p_data;     //ԭͼ������ָ��
	int wide,height;    //ԭͼ������
	p_data=this->GetData ();     //ȡ��ԭͼ��������ָ��
/*	if(m_pBitmapInfoHeader->biBitCount<9)		
		wide=this->GetWidth ();
	else
		wide=this->GetDibWidthBytes();
	height=this->GetHeight ();*/

		// ѭ������
	int i;
	int j;
	// �Ҷȼ���
	int huidu[256];    
	//int wide,height;    //ԭͼ������
	wide=this->GetWidth ();	
	if(this->m_pBitmapInfoHeader->biBitCount<9)	{
		height=this->GetHeight ();
	}
	else
		height=(this->GetHeight ())*3;
	// ������ʼ��
	memset(huidu,0,sizeof(huidu));
	LPBYTE  temp1=new BYTE[wide*height];    //��ͼ�񻺳���
	//����ԭͼ�񵽻���ͼ��
	memcpy(temp1,m_pData,wide*height );
	// �Ը����ؽ��лҶ�ͳ��
	for (j = 0; j < height; j ++)
	{
		for (i = 0; i <wide; i ++)
		{
			unsigned char  temp = temp1[wide* j + i] ;
			// �Ҷ�ͳ�Ƽ���
			huidu[temp]++;
		}
	}

	//�󴰿ڱ任�����޺�����
	//
	long temp[16];
	int k=0;
	int a;
	for(k=0;k<16;k++)
	{
		temp[k]=0;
		for(a=k*16;a<(k+1)*16;a++)
			temp[k]+=huidu[a];
	}
	
	long max=0;
	int t=0;
	for(k=15;k>=0;k--)
	{
		if(temp[k]>max)
		{
			max=temp[k];
			t=k;
		}
		
	}
	
	int bLow=0,bUp=0;
	bLow=(t-1)*16;
	//	bUp=(t+5)*16;
	
	//	bLow=100;
	bUp=255;
	// ��ֵ
	INT	bThre;
	
	bThre=(INT)((2*bUp+bLow)/3);
     
	int b,c;
	for( b=0;b<height;b++)
	{
		for( c=0;c<wide;c++)
		{
			if(*p_data>bThre)          //�Ҷ�ֵ���ڸ�����ֵ����Ϊ255
				*p_data=255;
			else
				*p_data=0;             //��������Ϊ0
			p_data++;
		}
	}
}
///////////////////////////////////////////////////////////////////
void HuiDuBianHuanDib::bianyuanjiance()//��Ե���
{
	LPBYTE  p_data;     //ԭͼ������ָ��
	int wide,height;    //ԭͼ������
	p_data=this->GetData ();     //ȡ��ԭͼ��������ָ��
	if(m_pBitmapInfoHeader->biBitCount<9)		
		wide=this->GetWidth ();
	else
		wide=this->GetDibWidthBytes();
//	if(this->m_pBitmapInfoHeader->biBitCount==9)	{
		height=this->GetHeight ();
//	}
//	else
//		height=(this->GetHeight ())*3;
	
	BYTE *temp=new BYTE[wide*height];
	memcpy(temp,p_data,wide*height);


	int fres;

	for(int j=0;j<height-1;j++)
	{
		for(int i=0;i<wide-1;i++)
		{
			fres = p_data[wide*j+i]-p_data[wide*j+i+1];
			if(fres<0)fres=-fres;
			if(fres>255)temp[wide*i+j]=255;
			else
			{temp[wide*j+i]=(int)(fres+0.5);}

		}
	}
	memcpy(p_data,temp,wide*height);
}

/***************************************************************/
/*�������ƣ�MakegGray()                                        */
/*�������ͣ�void                                               */
/*���ܣ����ɫת����256ɫ�Ҷ�ͼ��                            */
/***************************************************************/
void HuiDuBianHuanDib::MakegGray()    //�Ҷȱ仯
{
	BYTE *p_data;     //ԭͼ������ָ��
	int wide,height,DibWidth;    //ԭͼ�������ֽڿ�
	p_data=this->GetData ();   //ȡ��ԭͼ��������ָ��
    wide=this->GetWidth ();   //ȡ��ԭͼ�����������
    height=this->GetHeight ();   //ȡ��ԭͼ���������߶�
	DibWidth=this->GetDibWidthBytes();   //ȡ��ԭͼ��ÿ���ֽ���
	for(int j=0;j<height;j++)	// ÿ��
		for(int i=0;i<DibWidth;i+=3)	// ÿ��
		{
			BYTE* pbyBlue = p_data++;   //�õ���ɫֵ
			BYTE* pbyGreen = p_data++;  //�õ���ɫֵ
			BYTE* pbyRed = p_data++;    //�õ���ɫֵ
			BYTE r = *pbyRed;
			BYTE g = *pbyGreen;
			BYTE b = *pbyBlue;
			//ȡ��ԭr,g,b�е����ֵ��Ϊ����������ֵ����ֵ
			int gray;
		/*	if(r>g)
				gray=r;
			else 
				gray=g;
			if(gray<b)
				gray=b;	*/
			gray=(r)*0.299 + (g)*0.588 + (b)*0.114 ;
       		*pbyBlue = gray;     //��ȡ�������ֵ�������ص�������
			*pbyGreen = gray;    //��ȡ�������ֵ�������ص��̷���
			*pbyRed = gray;	     //��ȡ�������ֵ�������صĺ����
		}
}

/*************************************************************************
*
* �������ƣ�
*   MedianFilter()
*
* ����:
*   LPSTR lpDIBBits		- ָ��ԴDIBͼ��ָ��
*   LONG  lWidth			- Դͼ���ȣ���������
*   LONG  lHeight			- Դͼ��߶ȣ���������
*   int   iFilterH			- �˲����ĸ߶�
*   int   iFilterW			- �˲����Ŀ��
*   int   iFilterMX		- �˲���������Ԫ��X����
*   int   iFilterMY		- �˲���������Ԫ��Y����
*
* ����ֵ:
*   BOOL					- �ɹ�����TRUE�����򷵻�FALSE��
*
* ˵��:
*   �ú�����DIBͼ�������ֵ�˲���
*
************************************************************************/

BOOL HuiDuBianHuanDib::  myMedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
						   int iFilterH, int iFilterW, 
						   int iFilterMX, int iFilterMY,long LB)
{
	
	// ָ��Դͼ���ָ��
	unsigned char*	lpSrc;
	
	// ָ��Ҫ���������ָ��
	unsigned char*	lpDst;
	
	// ָ����ͼ���ָ��
	LPSTR			lpNewDIBBits;
	HLOCAL			hNewDIBBits;
	
	// ָ���˲��������ָ��
	unsigned char	* aValue;
	HLOCAL			hArray;
	
	// ѭ������
	LONG			i;
	LONG			j;
	LONG			k;
	LONG			l;
	
	// ͼ��ÿ�е��ֽ���
	LONG			lLineBytes;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = LB;
	
	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	
	// �ж��Ƿ��ڴ����ʧ��
	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// �����ڴ�
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);
	
	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);
	
	// ��ʱ�����ڴ棬�Ա����˲�������
	hArray = LocalAlloc(LHND, iFilterH * iFilterW);
	
	// �ж��Ƿ��ڴ����ʧ��
	if (hArray == NULL)
	{
		// �ͷ��ڴ�
		LocalUnlock(hNewDIBBits);
		LocalFree(hNewDIBBits);
		
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// �����ڴ�
	aValue = (unsigned char * )LocalLock(hArray);
	
	// ��ʼ��ֵ�˲�
	// ��(��ȥ��Ե����)
	for(i = iFilterMY; i < lHeight - iFilterH + iFilterMY + 1; i++)
	{
		// ��(��ȥ��Ե����)
		for(j = iFilterMX; j < lWidth - iFilterW + iFilterMX + 1; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// ��ȡ�˲�������
			for (k = 0; k < iFilterH; k++)
			{
				for (l = 0; l < iFilterW; l++)
				{
					// ָ��DIB��i - iFilterMY + k�У���j - iFilterMX + l�����ص�ָ��
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iFilterMY - k) + j - iFilterMX + l;
					
					// ��������ֵ
					aValue[k * iFilterW + l] = *lpSrc;
				}
			}
			
			// ��ȡ��ֵ
			* lpDst = myGetMedianNum(aValue, iFilterH * iFilterW);
		}
	}
	
	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	
	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	LocalUnlock(hArray);
	LocalFree(hArray);
	
	// ����
	return TRUE;
}


/*************************************************************************
*
* �������ƣ�
*   GetMedianNum()
*
* ����:
*   unsigned char * bpArray	- ָ��Ҫ��ȡ��ֵ������ָ��
*   int   iFilterLen			- ���鳤��
*
* ����ֵ:
*   unsigned char      - ����ָ���������ֵ��
*
* ˵��:
*   �ú�����ð�ݷ���һά����������򣬲���������Ԫ�ص���ֵ��
*
************************************************************************/

unsigned char HuiDuBianHuanDib:: myGetMedianNum(unsigned char * bArray, int iFilterLen)
{
	// ѭ������
	int		i;
	int		j;
	
	// �м����
	unsigned char bTemp;
	
	// ��ð�ݷ��������������
	for (j = 0; j < iFilterLen - 1; j ++)
	{
		for (i = 0; i < iFilterLen - j - 1; i ++)
		{
			if (bArray[i] > bArray[i + 1])
			{
				// ����
				bTemp = bArray[i];
				bArray[i] = bArray[i + 1];
				bArray[i + 1] = bTemp;
			}
		}
	}
	
	// ������ֵ
	if ((iFilterLen & 1) > 0)
	{
		// ������������Ԫ�أ������м�һ��Ԫ��
		bTemp = bArray[(iFilterLen + 1) / 2];
	}
	else
	{
		// ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
		bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
	}
	
	// ������ֵ
	return bTemp;
}

/*************************************************************************
*
* �������ƣ�
*   myHprojectDIB()
*
* ����:
*   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   LONG  lWidth       - Դͼ���ȣ���������
*   LONG  lHeight      - Դͼ��߶ȣ���������
*   INT*  iTop         - �����ϱ�Ե
*   INT*  iBottom      - �����±�Ե
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
*
* ˵��:
*   �ú������ڶԺ�����ͼ�����ˮƽͶӰ���㣬��ȡ������ͼ������±�Եλ��
*
************************************************************************/
BOOL HuiDuBianHuanDib:: myHprojectDIB(LPSTR lpDIB, LONG lWidth, LONG lHeight, 
						  int* iTop, int* iBottom) 
{
	LPSTR lpDIBBits;                   //ָ��DIB�����ص�ָ��
	
    LONG lLineBytes;                   // ͼ��ÿ�е��ֽ���
	unsigned char * lpSrc;             //ָ��ԭͼ�����ص��ָ��
	
	unsigned char pixel;				//����ֵ
	
	int i,j;
	
	// ������
	INT* iResult;
	
	INT pzBottom,pzTop;
	
	bool findPZ=false;
	
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = (char*)this->GetData ();
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes =this->GetDibWidthBytes();
	
	iResult=new INT[lHeight];
	
	for(i=0;i<lHeight;i++)
		iResult[i]=0;
	
	for(j=lHeight/5;j<lHeight*0.8;j++)
	{
		iResult[j]=0;
		for(i=0;i<lWidth;i++)
		{
			lpSrc=(unsigned char*)lpDIBBits+lLineBytes*j+i;
			pixel=(unsigned char)(*lpSrc);
			if(pixel==255)
			{
				iResult[j]++;
			}
		}
		if((!findPZ)&&iResult[j]>12)
		{
			pzBottom=j;
			findPZ=true;
		}
		if(findPZ&&iResult[j]<12)
		{
			pzTop=j;
			break;
		}
		
	}
	pzTop=pzBottom+55;
	pzBottom=pzBottom-20;   //΢������
	*iBottom=lHeight-pzBottom;
	*iTop=lHeight-pzTop;
	return true;
}
/*************************************************************************
*
* �������ƣ�
*   myVprojectDIB()
*
* ����:
*   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   LONG  lWidth       - Դͼ���ȣ���������
*   LONG  lHeight      - Դͼ��߶ȣ���������
*   INT*  iLeft        - �������Ե
*   INT*  iRight       - �����ұ�Ե
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
*
* ˵��:
*   �ú������ڶԺ�����ͼ����д�ֱͶӰ���㣬��ȡ������ͼ������ұ�Եλ��
*
************************************************************************/
BOOL HuiDuBianHuanDib:: myVprojectDIB(LPSTR lpDIB, LONG lWidth, LONG lHeight, 
						  int* iLeft, int* iRight) 
{
	LPSTR lpDIBBits;                   //ָ��DIB�����ص�ָ��
	
    LONG lLineBytes;                   // ͼ��ÿ�е��ֽ���
	unsigned char * lpSrc;             //ָ��ԭͼ�����ص��ָ��
	
	unsigned char pixel;				//����ֵ
	
	int i,j;
	
	// ������
	INT* iResult;
	
	INT pzLeft,pzRight;
	
	bool findPZ=false;
	
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits =  (char*)this->GetData ();
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = this->GetDibWidthBytes();
	
	iResult=new INT[lWidth];
	
	for(i=0;i<lWidth;i++)
		iResult[i]=0;
	
	for(i=lWidth/5;i<lWidth*0.8;i++)
	{
		iResult[i]=0;
		for(j=0;j<lHeight;j++)
		{
			lpSrc=(unsigned char*)lpDIBBits+lLineBytes*j+i;
			pixel=(unsigned char)(*lpSrc);
			if(pixel==255)
			{
				iResult[i]++;
			}
		}
		if((!findPZ)&&iResult[i]>10)
		{
			pzLeft=i;
			findPZ=true;
		}
		pzRight=pzLeft+150;
	}
	
	*iLeft=pzLeft;
	*iRight=pzRight+10;
	return true;
}

//���Ʒָ�//////////////////////ˮƽͶӰ

BOOL HuiDuBianHuanDib:: myHcut(LPSTR lpDIB, LONG lWidth, LONG lHeight, 
						  int* iTop, int* iBottom) 
{
	LPSTR lpDIBBits;                   //ָ��DIB�����ص�ָ��
	
    LONG lLineBytes;                   // ͼ��ÿ�е��ֽ���
	unsigned char * lpSrc;             //ָ��ԭͼ�����ص��ָ��
	
	unsigned char pixel;				//����ֵ
	
	int i,j;
	
//	int r;                               //��¼��һ��ͶӰֵ
	// ������
	INT* iResult;
	
	INT pzBottom,pzTop;
	
	bool findPZ=false;
	
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = (char*)this->GetData ();
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes =this->GetDibWidthBytes();
	
	iResult=new INT[lHeight];
	
	for(i=0;i<lHeight;i++)
		iResult[i]=0;
	
	for(j=0;j<lHeight;j++)
	{
		iResult[j]=0;
		for(i=0;i<lWidth;i++)
		{
			lpSrc=(unsigned char*)lpDIBBits+lLineBytes*j+i;
			pixel=(unsigned char)(*lpSrc);
			if(pixel==255)
			{
				iResult[j]++;
			}
		}
/*		if((!findPZ)&&iResult[j]>(lWidth/3)&&r>(lWidth/3))
		{
			pzBottom=j;
			findPZ=true;
		}
		if(findPZ&&iResult[j]<(lWidth/6)&&r>(lWidth/5))
		{
			pzTop=j;
			break;
		}*/
//		r=iResult[j];
	}
	for(j=0;j<lHeight;j++)
	{
		if((!findPZ)&&iResult[j]>(lWidth/3)&&iResult[j+1]>(lWidth/3)&&iResult[j+2]>(lWidth/3)&&iResult[j+3]>(lWidth/3))
		{
			pzBottom=j;
			findPZ=true;
		}
		if(findPZ&&iResult[j]>(lWidth/5)&&iResult[j+1]<(lWidth/6))
		{
			pzTop=j;
			break;
		}

	}

	pzTop=pzTop+1;
	pzBottom=pzBottom-2;   //΢������
	*iBottom=pzBottom;
	*iTop=pzTop;
	return true;
}

//���Ʒָ�//////////////////////��ֱͶӰ
BOOL HuiDuBianHuanDib:: myVcut(LPSTR lpDIB, LONG lWidth, LONG lHeight, 
						  int* r1, int* l1,int* r2, int* l2,int* r3, int* l3,int* r4, int* l4,int* r5, int* l5
						  ,int* r6, int* l6,int* r7, int* l7) 
{
	LPSTR lpDIBBits;                   //ָ��DIB�����ص�ָ��
	
    LONG lLineBytes;                   // ͼ��ÿ�е��ֽ���
	unsigned char * lpSrc;             //ָ��ԭͼ�����ص��ָ��
	
	unsigned char pixel;				//����ֵ
	
	int i,j;
	
	// ������
	INT* iResult;
	
	INT pzLeft,pzRight;
	
	bool findPZ=false;
	
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits =  (char*)this->GetData ();
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = this->GetDibWidthBytes();
	
	iResult=new INT[lWidth];
	
	for(i=0;i<lWidth;i++)
		iResult[i]=0;
	
	for(i=0;i<lWidth;i++)
	{
		iResult[i]=0;
		for(j=0;j<lHeight;j++)
		{
			lpSrc=(unsigned char*)lpDIBBits+lLineBytes*j+i;
			pixel=(unsigned char)(*lpSrc);
			if(pixel==255)
			{
				iResult[i]++;
			}
		}

	}
	for(i=0;i<lWidth;i++)
	{
		if((!findPZ)&&iResult[i]>(lHeight/10)&&iResult[i+1]>(lHeight/10)&&iResult[i+2]>(lHeight/10)&&iResult[i+3]>(lHeight/10))
		{
			*l1=i;
			findPZ=true;
		}
		if(findPZ&&iResult[i]<(lHeight/11)&&iResult[i-1]>(lHeight/11)&&iResult[i-2]>(lHeight/11))
		{
			*r1=i;
			findPZ=false;
			break;
		}
	}
	for(;i<lWidth;i++)
	{
		if((!findPZ)&&iResult[i]>(lHeight/10)&&iResult[i+1]>(lHeight/10)&&iResult[i+2]>(lHeight/10)&&iResult[i+3]>(lHeight/10))
		{
			*l2=i;
			findPZ=true;
		}
		if(findPZ&&iResult[i]<(lHeight/10)&&iResult[i-1]>(lHeight/10)&&iResult[i-2]>(lHeight/10))
		{
			*r2=i;
			findPZ=false;
			break;
		}
	}
	for(;i<lWidth;i++)
	{
		if((!findPZ)&&iResult[i]>(lHeight/10)&&iResult[i+1]>(lHeight/10)&&iResult[i+2]>(lHeight/10)&&iResult[i+3]>(lHeight/10))
		{
			*l3=i;
			findPZ=true;
		}
		if(findPZ&&iResult[i]<(lHeight/10)&&iResult[i-1]>(lHeight/10)&&iResult[i-2]>(lHeight/10))
		{
			*r3=i;
			findPZ=false;
			break;
		}
	}
	for(;i<lWidth;i++)
	{
		if((!findPZ)&&iResult[i]>(lHeight/10)&&iResult[i+1]>(lHeight/10)&&iResult[i+2]>(lHeight/10)&&iResult[i+3]>(lHeight/10))
		{
			*l4=i;
			findPZ=true;
		}
		if(findPZ&&iResult[i]<(lHeight/10)&&iResult[i-1]>(lHeight/10)&&iResult[i-2]>(lHeight/10))
		{
			*r4=i;
			findPZ=false;
			break;
		}
	}
	for(;i<lWidth;i++)
	{
		if((!findPZ)&&iResult[i]>(lHeight/10)&&iResult[i+1]>(lHeight/10)&&iResult[i+2]>(lHeight/10)&&iResult[i+3]>(lHeight/10))
		{
			*l5=i;
			findPZ=true;
		}
		if(findPZ&&iResult[i]<(lHeight/10)&&iResult[i-1]>(lHeight/10)&&iResult[i-2]>(lHeight/10))
		{
			*r5=i;
			findPZ=false;
			break;
		}
	}
	for(;i<lWidth;i++)
	{
		if((!findPZ)&&iResult[i]>(lHeight/10)&&iResult[i+1]>(lHeight/10)&&iResult[i+2]>(lHeight/10)&&iResult[i+3]>(lHeight/10))
		{
			*l6=i;
			findPZ=true;
		}
		if(findPZ&&iResult[i]<(lHeight/10)&&iResult[i-1]>(lHeight/10)&&iResult[i-2]>(lHeight/10))
		{
			*r6=i;
			findPZ=false;
			break;
		}
	}
	for(;i<lWidth;i++)
	{
		if((!findPZ)&&iResult[i]>(lHeight/10)&&iResult[i+1]>(lHeight/10)&&iResult[i+2]>(lHeight/10)&&iResult[i+3]>(lHeight/10))
		{
			*l7=i;
			findPZ=true;
		}
		if(findPZ&&iResult[i]<(lHeight/11)&&iResult[i-1]>(lHeight/11)&&iResult[i-2]>(lHeight/11))
		{
			*r7=i;
			findPZ=false;
			break;
		}
	}

//	*iLeft=pzLeft;
//	*iRight=pzRight+10;
	return true;
}











/***************************************************************/           
/*�������ƣ�ShuangYu(int YuZhi1,int YuZhi2,int mode)          */
/*������int YuZhi1����ֵ1��int YuZhi2����ֵ2��int mode������ʽ*/
/*�������ͣ�void                                              */
/*���ܣ���ͼ��ʹ��˫�̶���ֵ�����ж�ֵ����                     */
/***************************************************************/
void HuiDuBianHuanDib::ShuangYu(int YuZhi1,int YuZhi2,int mode)
{
	LPBYTE  p_data;     //ԭͼ������ָ��
	int wide,height;    //ԭͼ������
	p_data=this->GetData ();
	if(m_pBitmapInfoHeader->biBitCount<9)		
		wide=this->GetWidth ();
	else
		wide=this->GetDibWidthBytes();
	height=this->GetHeight ();
	if(mode==0)    //0-255-0��
	{
		for(int j=0;j<height;j++)
			for(int i=0;i<wide;i++)
			{
				//�������صĻҶ�ֵ����������ֵ֮�䣬�⽫����Ϊ255
				if(*p_data>=YuZhi1&&*p_data<=YuZhi2)
					*p_data=255;
				else
					*p_data=0;     //������0
				p_data++; 			 		
			}	
	}
	if(mode==1)    //255-0-255��
	{
		for(int j=0;j<height;j++)
			for(int i=0;i<wide;i++)
			{
				//�������صĻҶ�ֵ����������ֵ֮�䣬�⽫����Ϊ0
				if(*p_data>=YuZhi1&&*p_data<=YuZhi2)
					*p_data=0;
				else
					*p_data=255;    //������255
				p_data++; 					
			}
	}			 
}

///***************************************************************/           
/*�������ƣ�ZhiFangTu(float *tongji)                               
/*�������ͣ�void                                         
/*����˵����tongji  �Ҷȷֲ��ܶ�ͳ��                                                        
/*���ܣ���ͼ����лҶ�ֱ��ͼͳ�ơ�                     
/***************************************************************/
void HuiDuBianHuanDib::ZhiFangTu(float *tongji)
{
	// ѭ������
	int i;
	int j;
	// �Ҷȼ���
	int huidu[256];    
	int wide,height;    //ԭͼ������
	wide=this->GetWidth ();		 
	height=this->GetHeight ();
	// ������ʼ��
	memset(huidu,0,sizeof(huidu));
	LPBYTE  temp1=new BYTE[wide*height];    //��ͼ�񻺳���
	//����ԭͼ�񵽻���ͼ��
	memcpy(temp1,m_pData,wide*height );
	// �Ը����ؽ��лҶ�ͳ��
	for (j = 0; j < height; j ++)
	{
		for (i = 0; i <wide; i ++)
		{
			unsigned char  temp = temp1[wide* j + i] ;
			if(temp==255)
			// �Ҷ�ͳ�Ƽ���
			huidu[i]++;
		}
	}


	// ����Ҷȷֲ��ܶ�
	for(i=0;i<256;i++)
		tongji[i] = huidu[i] / (height * wide *1.0f);	
}

///***************************************************************/           
/*�������ƣ�Zhexianbianhuan(BYTE bX1,BYTE bY1,BYTE bX2,BYTE bY2)                                           */
/*�������ͣ�void                                              */
/*����˵����bX1�۵�һ��ԭʼ�Ҷȣ�bX2bY1�۵����ԭʼ�Ҷ�
/*          bY1�۵�һ�ı任�Ҷȣ�bY2�۵���ı任�Ҷ�                                                       
/*���ܣ���ͼ����зֶ����Ա任��                      */
/***************************************************************/ 
void HuiDuBianHuanDib::Zhexianbianhuan(BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2)
{ 
	// ѭ������
	int i;
	int j;
	int wide;
	int height;
	LPBYTE  p_data;
	p_data=this->GetData ();     //ȡ��ԭͼ��������ָ��
	wide=this->GetWidth ();
	height=this->GetHeight ();
	LPBYTE temp=new BYTE[wide*height];    //��ͼ�񻺳���
	//����ԭͼ�񵽻���ͼ��
	memcpy(temp,p_data,wide*height);
	// �Ҷ�ӳ���
	BYTE	bMap[256];
	// ����Ҷ�ӳ���
	for (i = 0; i <= bX1; i++)	//[0 ���� X1]
	{
		// �ж�bX1�Ƿ����0����ֹ��ĸΪ0��
		if (bX1 > 0)
		{
			// ���Ա任
			bMap[i] = (BYTE) bY1 * i / bX1;
		}
		else
		{
			// ֱ�Ӹ�ֵΪ0
			bMap[i] = 0;
		}
	}
	for (; i <= bX2; i++)		//(X1 ���� X2]
	{
		// �ж�bX1�Ƿ����bX2����ֹ��ĸΪ0��
		if (bX2 != bX1)
		{
			// ���Ա任
			bMap[i] = bY1 + (BYTE) ((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
		}
		else
		{
			// ֱ�Ӹ�ֵΪbY1
			bMap[i] = bY1;
		}
	}
	for (; i < 256; i++)		//(X2 ���� 256)
	{
		// �ж�bX2�Ƿ����255����ֹ��ĸΪ0��
		if (bX2 != 255)
		{
			// ���Ա任
			bMap[i] = bY2 + (BYTE) ((255 - bY2) * (i - bX2) / (255 - bX2));
		}
		else
		{
			// ֱ�Ӹ�ֵΪ255
			bMap[i] = 255;
		}
	}
	if  (m_pBitmapInfoHeader->biBitCount<9)		//�Ҷ�ͼ��
	{
		// �Ը����ؽ��лҶ�ת��
		for (i = 0; i < height; i ++)
		{
			for (j = 0; j < wide; j ++)
			{
				// �����ؽ��лҶ�ӳ�䴦��
				unsigned char T = temp[ wide * i + j];
				temp[ wide * i + j] = bMap[T];			 
			}
		}
        memcpy(p_data, temp,height*wide); 
	}
	else		//24λ��ɫ
	{
		wide=this->GetDibWidthBytes();
		temp=new BYTE[wide*height]; 
		memcpy(temp,p_data,wide*height);
		// �Ը����ؽ��лҶ�ת��
		for (i = 0; i < height; i ++)
		{
			for (j = 0; j < wide; j ++)
			{
				// �����ؽ��лҶ�ӳ�䴦��
				unsigned char T = temp[ wide * i + j];
				temp[ wide * i + j] = bMap[T];			 
			}
		}
		memcpy(p_data, temp,height*wide);
	}
}

///***************************************************************/           
/*�������ƣ�Chuangkoubianhuan(BYTE blow,BYTE bup)                                      
/*�������ͣ�void                                          
/*����˵����blow�����½� bup�����Ͻ�                                                               
/*���ܣ���ͼ����д��ڱ任��                    
/***************************************************************/  
void HuiDuBianHuanDib::Chuangkoubianhuan(BYTE bLow, BYTE bUp)
{
    // ѭ������
	int i;
	int j;
	int wide,height;    //ԭͼ������
	// ָ��Դͼ���ָ��
	LPBYTE	lpSrc;
	// ָ��DIB����ָ��
	LPBYTE  p_data;
	p_data=this->GetData ();     //ȡ��ԭͼ��������ָ��
	if (m_pBitmapInfoHeader->biBitCount<9)		
		wide=this->GetWidth ();
	else
		wide=this->GetDibWidthBytes();
	height=this->GetHeight ();
	// �Ը����ؽ��лҶ�ת��
	for(i = 0; i < height; i++)
	{
		// ÿ��
		for(j = 0; j < wide; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (LPBYTE)p_data + wide * (height- 1 - i) + j;
			// �ж��Ƿ񳬳���Χ
			if ((*lpSrc) < bLow)
			{
				// ֱ�Ӹ�ֵΪ0
				*lpSrc = 0;
			}
			else if ((*lpSrc) > bUp)
			{
				// ֱ�Ӹ�ֵΪ255
				*lpSrc = 255;
			}
		}
	}
}

///***************************************************************/           
/*�������ƣ�Fenbujunhenghua()                                           
/*�������ͣ�void                                          
/*����˵������                                                               
/*���ܣ���ͼ����лҶȷֲ����⻯����                   
/***************************************************************/  
void HuiDuBianHuanDib::Fenbujunhenghua( )
{	
	// ѭ������
	LONG i;
	LONG j;
	//ͼ��Ŀ�͸�
	LONG wide;
	LONG height;
	// �Ҷȷֲ��ܶ�
	float fPs_R[256],fPs_G[256],fPs_B[256];
	// �м����
	float temp_r[256],temp_g[256],temp_b[256];
	int nNs_R[256],nNs_G[256],nNs_B[256];
	// ��ʼ��
	memset(temp_r, 0, sizeof(temp_r));
	memset(temp_g, 0, sizeof(temp_g));
	memset(temp_b, 0, sizeof(temp_b));
	// ָ��DIB����ָ��
	LPBYTE p_data;
	// �ҵ�DIBͼ��������ʼλ��
	p_data = this->GetData();
	wide=this->GetWidth ();	
	// DIB�ĸ߶�
	height = GetHeight();
	if(m_pBitmapInfoHeader->biBitCount<9)		//�Ҷ�ͼ��
	{
		// ��ȡͼ��ĻҶȷֲ��ܶ�
		ZhiFangTu(fPs_R);
		// ���о��⻯���� 
		for(i = 0; i < 256; i++)
		{
			if(i == 0)
			{
				temp_r[0] = fPs_R[0];	 
			}
			else
			{
				temp_r[i] = temp_r[i-1] + fPs_R[i];	 
			}
			nNs_R[i] = (int)(255.0f * temp_r[i] + 0.5f);
		}
	}
	else		//24λ��ɫ
	{
		// ��ȡͼ��ĻҶȷֲ��ܶ�
		ZhiFangTu(fPs_R,fPs_G,fPs_B);
		// ���о��⻯���� 
		for(i = 0; i < 256; i++)
		{
			if(i == 0)
			{
				temp_r[0] = fPs_R[0];
				temp_g[0] = fPs_G[0];
				temp_b[0] = fPs_B[0];
			}
			else
			{
				temp_r[i] = temp_r[i-1] + fPs_R[i];	
				temp_g[i] = temp_g[i-1] + fPs_G[i];	
				temp_b[i] = temp_b[i-1] + fPs_B[i];	
			}
			nNs_R[i] = (int)(255.0f * temp_r[i] + 0.5f);
			nNs_G[i] = (int)(255.0f * temp_g[i] + 0.5f);
			nNs_B[i] = (int)(255.0f * temp_b[i] + 0.5f);
		}
	}
	if (m_pBitmapInfoHeader->biBitCount<9)		//�Ҷ�ͼ��
	{ 
		// �Ը����ؽ��лҶ�ת��
		for (j = 0; j < height; j ++)
		{
			for (i = 0; i < wide; i ++)
			{
				// ��ת����ĻҶȷֲ�д��DIBͼ��
				unsigned char temp = *((unsigned char *)p_data + wide * j + i);
				*((unsigned char *)p_data + wide * j + i) = nNs_R[temp];			
			}
		}
	}
	else		//24λ��ɫ
	{
		// �Ը����ؽ��лҶ�ת��
		for (j = 0; j < height; j ++)
		{
			for (i = 0; i < wide; i ++)
			{
				// ��ת����ĻҶȷֲ�д��DIBͼ��
				unsigned char temp1= *((unsigned char *)p_data + wide * j*3 + i*3);
				*((unsigned char *)p_data + wide * j*3 + i*3) = nNs_R[temp1];
				unsigned char temp2 = *((unsigned char *)p_data + wide * j*3 + i*3+1);
				*((unsigned char *)p_data + wide * j*3 + i*3+1) = nNs_G[temp2];	
				unsigned char temp3 = *((unsigned char *)p_data + wide * j*3 + i*3+2);
				*((unsigned char *)p_data + wide * j*3 + i*3+2) = nNs_B[temp3];	
			}
		}
	}
}

int HuiDuBianHuanDib::GetWidth()
{
	return CDib::GetWidth();
}

///***************************************************************/           
/*�������ƣ�Pipeibianhuan(BYTE bNum,int *npNu,float*fpPu)                                            */
/*�������ͣ�void                                              */
/*����˵����bNum�涨ֱ��ͼ�Ҷȼ� npNu ֱ��ͼӳ���ϵ
/*          fpPu�Ҷȷֲ�����                                                             
/*���ܣ���ͼ����лҶ�ƥ��任��                       */
/***************************************************************/  
void HuiDuBianHuanDib::Pipeibianhuan(BYTE bNum, int *npNu, float *fpPu)
{
	// ѭ������
	LONG i;
	LONG j;
    LONG wide;
	LONG height;
	// �Ҷȷֲ��ܶ�
	int midu[256],midu2[256],midu3[256];
	// �Ҷȷֲ�����
	float gailu[256],gailu2[256],gailu3[256];
	// �м���ʱ����
	float temp[256],temp2[256],temp3[256];
	// ָ��DIB����ָ��
	LPBYTE p_data;
	// �ҵ�DIBͼ��������ʼλ��
	p_data = this->GetData();
	wide=this->GetWidth ();    
	// DIB�ĸ߶�
	height = GetHeight(); 	
    if (m_pBitmapInfoHeader->biBitCount<9)		//�Ҷ�ͼ��
	{   
		// �ԻҶ��ܶȷֲ�����ͳ��	
		Midufenbu(midu);
		// �ԻҶȷֲ����ʽ���ͳ��
		ZhiFangTu(gailu);
		
		// ����ԭʼ�ۼ�ֱ��ͼ
		for (i = 0; i < 256; i++)
		{
			if (i == 0)
			{
				temp[0] = gailu[0];			 
			}
			else
			{
				temp[i] = temp[i-1] + gailu[i];			 
			}
			gailu[i] = temp[i];
		}
		// ����涨�ۼ�ֱ��ͼ
		for (i = 0; i < bNum; i++)
		{
			if (i == 0)
			{
				temp[0] = fpPu[0];
			}
			else
			{
				temp[i] = temp[i-1] + fpPu[i];
			}
			fpPu[i] = temp[i];
		}
		// ȷ��ӳ���Ӧ��ϵ
		for (i = 0; i < 256; i++)
		{
			// ��ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
			int m_r = 0;
			// ��С��ֵ
			float min_value_r = 1.0f;
			// �Թ涨ֱ��ͼ���ҶȽ���ö��
			for (j = 0; j < bNum; j++)
			{
				// ��ǰ��ֵ
				float now_value = 0.0f;
				//  �����ֵ
				if (gailu[i] - fpPu[j] >= 0.0f)
					now_value = gailu[i] - fpPu[j];
				else
					now_value = fpPu[j] - gailu[i];
				// Ѱ����ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
				if (now_value < min_value_r)
				{
					// ��ӽ��ĻҶȼ�
					m_r = j;
					// �ݴ���С��ֵ
					min_value_r = now_value;
				}
			}
			// �����Ҷ�ӳ���
			midu[i] = npNu[m_r];
		}
		// �Ը����ؽ��д���
		for (j = 0; j < height; j ++)
		{
			for (i = 0; i < wide; i ++)
			{
				// �����ؽ��е�ӳ������ֱ��ͼ�涨��ӳ�䴦��
				unsigned char temp = *((unsigned char *)p_data + wide * j + i);
				*((unsigned char *)p_data + wide * j + i) = midu[temp];
			}
		}
	}
	else		//24λ��ɫ
	{
		// �ԻҶ��ܶȷֲ�����ͳ��	
		Midufenbu(midu,midu2,midu3);
		// �ԻҶȷֲ����ʽ���ͳ��
		ZhiFangTu(gailu,gailu2,gailu3);
		
		// ����ԭʼ�ۼ�ֱ��ͼ
		for (i = 0; i < 256; i++)
		{
			if (i == 0)
			{
				temp[0] = gailu[0];	
				temp2[0] = gailu2[0];
				temp3[0] = gailu3[0];
			}
			else
			{
				temp[i] = temp[i-1] + gailu[i];
				temp2[i] = temp2[i-1] + gailu2[i];	
				temp3[i] = temp3[i-1] + gailu3[i];	
			}
			gailu[i] = temp[i];
			gailu2[i] = temp2[i];
			gailu3[i] = temp3[i];
			
		}
		// ����涨�ۼ�ֱ��ͼ
		for (i = 0; i < bNum; i++)
		{
			if (i == 0)
			{
				temp[0] = fpPu[0];			
			}
			else
			{
				temp[i] = temp[i-1] + fpPu[i];
			}
			fpPu[i] = temp[i];
		}
		// ȷ��ӳ���Ӧ��ϵ
		for (i = 0; i < 256; i++)
		{
			// ��ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
			int m_r = 0;
			int m_g = 0;
			int m_b = 0;		 		
			// ��С��ֵ
			float min_value_r = 1.0f;
			float min_value_g = 1.0f;
			float min_value_b = 1.0f;
			// �Թ涨ֱ��ͼ���ҶȽ���ö��
			for (j = 0; j < bNum; j++)
			{
				// ��ǰ��ֵ
				float now_value = 0.0f;
				//  �����ֵ
				if (gailu[i] - fpPu[j] >= 0.0f)
					now_value = gailu[i] - fpPu[j];
				else
					now_value = fpPu[j] - gailu[i];
				// Ѱ����ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
				if (now_value < min_value_r)
				{
					// ��ӽ��ĻҶȼ�
					m_r = j;
					// �ݴ���С��ֵ
					min_value_r = now_value;
				}
				//  �����ֵ
				if (gailu2[i] - fpPu[j] >= 0.0f)
					now_value = gailu2[i] - fpPu[j];
				else
					now_value = fpPu[j] - gailu2[i];
				// Ѱ����ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
				if (now_value < min_value_g)
				{
					// ��ӽ��ĻҶȼ�
					m_g = j;
					// �ݴ���С��ֵ
					min_value_g = now_value;
				}
				//  �����ֵ
				if (gailu3[i] - fpPu[j] >= 0.0f)
					now_value = gailu3[i] - fpPu[j];
				else
					now_value = fpPu[j] - gailu3[i];
				// Ѱ����ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
				if (now_value < min_value_b)
				{
					// ��ӽ��ĻҶȼ�
					m_b = j;
					// �ݴ���С��ֵ
					min_value_b = now_value;
				}
			}
			// �����Ҷ�ӳ���
			midu[i] = npNu[m_r];
			midu2[i] = npNu[m_g];
			midu3[i] = npNu[m_b];
		}
		// �Ը����ؽ��д���
		for (j = 0; j < height; j ++)
		{
			for (i = 0; i < wide; i ++)
			{
				// �����ؽ��е�ӳ������ֱ��ͼ�涨��ӳ�䴦��
				unsigned char temp = *((unsigned char *)p_data + wide * j*3 + i*3);
				*((unsigned char *)p_data + wide * j*3 + i*3) = midu[temp];
				unsigned char temp2 = *((unsigned char *)p_data + wide * j*3 + i*3+1);
				*((unsigned char *)p_data + wide * j*3 + i*3+1) = midu[temp2];
				unsigned char temp3 = *((unsigned char *)p_data + wide * j*3 + i*3+2);
				*((unsigned char *)p_data + wide * j*3 + i*3+2) = midu[temp3];
			}
		}
	}
}

///***************************************************************/           
/*�������ƣ�Midufenbu()                                        
/*�������ͣ�void                                      
/*���ܣ���ͼ������ܶȷֲ��任��            
/***************************************************************/  
void HuiDuBianHuanDib::Midufenbu(int *nNs_R)
{
	// ѭ������
	LONG i;
	LONG j;
	
	LONG wide;
	LONG height;
	//������ʼ��
	memset(nNs_R,0,sizeof(int) * 256);
	// ָ��DIB����ָ��
	LPBYTE p_data;
	// �ҵ�DIBͼ��������ʼλ��
	p_data = this->GetData();		
    wide=this->GetWidth ();
	// DIB�ĸ߶�
	height = GetHeight();
	// �Ը����ؽ��лҶ�ת��
	for (j = 0; j < height; j ++)
	{
		for (i = 0; i <wide; i ++)
		{
			// �Ը����ؽ��лҶ�ͳ��
			unsigned char temp = *((unsigned char *)p_data + wide * j + i);
			nNs_R[temp]++;		
		}
	} 
}

///***************************************************************/           
/*�������ƣ�Shuipingtouying()                                        
/*�������ͣ�void                                      
/*���ܣ���ͼ�����ˮƽͶӰ��            
/***************************************************************/  
void HuiDuBianHuanDib::Shuipingtouying()
{
	LPBYTE  p_data, p_datatemp;     //ԭͼ������ָ��
	int wide,height;				//ԭͼ������
	int i,j;						//ѭ������
	int lBlackNum  ;
	p_datatemp=this->GetData();
	p_data=p_datatemp;
	if(m_pBitmapInfoHeader->biBitCount<9)		
		wide=this->GetWidth ();
	else
		wide=this->GetDibWidthBytes();
	height=this->GetHeight ();
	//��ͼ����ж�ֵ��
	for(j=0;j<height;j++)
	{
		for(i=0;i<wide;i++)
		{ 
			if (*p_data>128)
				*p_data=255;
			else *p_data=0;
			p_data++;
		}
	}
	p_data=p_datatemp; 
	LPBYTE   temp=new BYTE[wide*height];    //��ͼ�񻺳���
	memset( temp,255,wide*height);			//�趨��ͼ���ֵΪ255
	for(j=0;j<height;j++)
	{
		lBlackNum=0;
		for(i=0;i<wide;i++)
		{				 
			if(p_data[wide*j+i]==0)
				lBlackNum++;
		}
		for(i=0;i<lBlackNum;i++)
			temp[wide*j+i]=0;
	}
	memcpy(p_data, temp,wide*height);
	delete  temp;	
}

///***************************************************************/           
/*�������ƣ�Chuizhitouying()                                      
/*�������ͣ�void                                      
/*���ܣ���ͼ����д�ֱͶӰ��            
/***************************************************************/  
void HuiDuBianHuanDib::Chuizhitouying()
{
	LPBYTE  p_data, p_datatemp;     //ԭͼ������ָ��
	int wide,height;				//ԭͼ������
	int i,j;						//ѭ������
	long int lBlackNum;				//ͼ����ÿ���ںڵ�ĸ���
	p_datatemp=this->GetData();
	p_data=p_datatemp;
	if(m_pBitmapInfoHeader->biBitCount<9)	
		wide=this->GetWidth ();
	else
		wide=this->GetDibWidthBytes();
	height=this->GetHeight ();
	//��ͼ����ж�ֵ��
	for(j=0;j<height;j++)
	{
		for(i=0;i<wide;i++)
		{ 
			if (*p_data>128)
				*p_data=255;
			else 
				*p_data=0;
			p_data++;
		}
	}
	p_data=p_datatemp;
	LPBYTE  temp=new BYTE[wide*height];		//��ͼ�񻺳���
	memset( temp,255,wide*height);			//�趨��ͼ���ֵΪ255
	for(i=0;i<wide;i++)
	{
		lBlackNum=0;
		for(j=0;j<height;j++)
		{
			
			if(p_data[wide*j+i]==0)
				lBlackNum++;
		}
		for(j=0;j<lBlackNum;j++)
			temp[wide*j+i]=0;
	}
	memcpy(p_data, temp,wide*height);
	delete  temp;		
}

void HuiDuBianHuanDib::RedZhiFangTu(float *tongji)
{
	// ѭ������
	int i;
	int j;
	// �Ҷȼ���
	int huidu[256];    
	int wide,height;    //ԭͼ������
	// ������ʼ��
	memset(huidu,0,sizeof(huidu));
	wide=this->GetDibWidthBytes();
	height=this->GetHeight ();
	LPBYTE  temp1=new BYTE[wide*height];    //��ͼ�񻺳���
	//����ԭͼ�񵽻���ͼ��
	memcpy(temp1,m_pData,wide*height );
	// �Ը����ؽ��лҶ�ͳ��
	for (i = 0; i < height; i ++)
	{
		for (j = 0; j <wide; j=j +3)
		{
			unsigned char  temp = temp1[wide* i + j] ;
			// �Ҷ�ͳ�Ƽ���
			huidu[temp]++;		
		}
	}
	// ����Ҷȷֲ��ܶ�
	for(i=0;i<256;i++)
		tongji[i] = huidu[i] / (height * wide *1.0f);
}

void HuiDuBianHuanDib::BlueZhiFangTu(float *tongji)
{
	// ѭ������
	int i;
	int j;
	// �Ҷȼ���
	int huidu[256];    
	int wide,height;    //ԭͼ������
	// ������ʼ��
	memset(huidu,0,sizeof(huidu));
	wide=this->GetDibWidthBytes();
	height=this->GetHeight ();
	LPBYTE  temp1=new BYTE[wide*height];    //��ͼ�񻺳���
	//����ԭͼ�񵽻���ͼ��
	memcpy(temp1,m_pData,wide*height );
	// �Ը����ؽ��лҶ�ͳ��
	for (i = 0; i < height; i ++)
	{
		for (j = 2; j <wide; j=j +3)
		{
			unsigned char  temp = temp1[wide* i + j] ;
			// �Ҷ�ͳ�Ƽ���
			huidu[temp]++;		
		}
	}
	// ����Ҷȷֲ��ܶ�
	for(i=0;i<256;i++)
		tongji[i] = huidu[i] / (height * wide *1.0f);
}

void HuiDuBianHuanDib::GreenZhiFangTu(float *tongji)
{
	// ѭ������
	int i;
	int j;
	// �Ҷȼ���
	int huidu[256];    
	int wide,height;    //ԭͼ������
	// ������ʼ��
	memset(huidu,0,sizeof(huidu));
	wide=this->GetDibWidthBytes();
	height=this->GetHeight ();
	LPBYTE  temp1=new BYTE[wide*height];    //��ͼ�񻺳���
	//����ԭͼ�񵽻���ͼ��
	memcpy(temp1,m_pData,wide*height );
	// �Ը����ؽ��лҶ�ͳ��
	for (i = 0; i < height; i ++)
	{
		for (j = 1; j <wide; j=j +3)
		{
			unsigned char  temp = temp1[wide* i + j] ;
			// �Ҷ�ͳ�Ƽ���
			huidu[temp]++;		
		}
	}
	// ����Ҷȷֲ��ܶ�
	for(i=0;i<256;i++)
		tongji[i] = huidu[i] / (height * wide *1.0f);
}

//��24λ��ɫͼ����лҶȸ���ͳ��
void HuiDuBianHuanDib::ZhiFangTu(float *tongji1, float *tongji2, float *tongji3)
{
	// ѭ������
	int i;
	int j;
	// �Ҷȼ���
	int huidu1[256],huidu2[256],huidu3[256];    
	int wide,height;    //ԭͼ������
	wide=this->GetWidth ();		 
	height=this->GetHeight ();
	// ������ʼ��
	memset(huidu1,0,sizeof(huidu1));
	memset(huidu2,0,sizeof(huidu2));
	memset(huidu3,0,sizeof(huidu3));		  
	LPBYTE  temp_pData=new BYTE[wide*height*3];    //��ͼ�񻺳���
	//����ԭͼ�񵽻���ͼ��
	memcpy(temp_pData,m_pData,wide*height*3);
	// �Ը����ؽ��лҶ�ͳ��
	for (j = 0; j < height; j ++)
	{
		for (i = 0; i <wide; i ++)
		{
			unsigned char  temp1 = temp_pData[wide*j*3 + i*3] ;
			unsigned char  temp2 = temp_pData[wide*j*3 + i*3+1] ;
			unsigned char  temp3 = temp_pData[wide*j *3+ i*3+2] ;
			// �Ҷ�ͳ�Ƽ���
			huidu1[temp1]++;
			huidu2[temp2]++;
			huidu3[temp3]++;
		}
	}
	// ����Ҷȷֲ��ܶ�
	for(i=0;i<256;i++)
	{
		tongji1[i] = huidu1[i] / (height * wide *1.0f);	
		tongji2[i] = huidu2[i] / (height * wide *1.0f);	
		tongji3[i] = huidu3[i] / (height * wide *1.0f);	
	}
}

void HuiDuBianHuanDib::Midufenbu(int *nNs_R, int *nNs_G, int *nNs_B)
{
    // ѭ������
	LONG i;
	LONG j;
	LONG wide;
	LONG height;
	//������ʼ��
	memset(nNs_R,0,sizeof(int) * 256);
	memset(nNs_G,0,sizeof(int) * 256);
	memset(nNs_B,0,sizeof(int) * 256);
	// ָ��DIB����ָ��
	LPBYTE p_data;
	// �ҵ�DIBͼ��������ʼλ��
	p_data = this->GetData();		
    wide=this->GetWidth ();
	// DIB�ĸ߶�
	height = GetHeight();
	// �Ը����ؽ��лҶ�ת��
	for (j= 0;j < height;j ++)
	{
		for (i = 0; i <wide; i ++)
		{
			// �Ը����ؽ��лҶ�ͳ��
			unsigned char temp = *((unsigned char *)p_data + wide * j*3 + i*3);
			unsigned char temp2 = *((unsigned char *)p_data + wide * j*3 + i*3+1);
			unsigned char temp3 = *((unsigned char *)p_data + wide * j*3 + i*3+2);
			nNs_R[temp]++;		
			nNs_G[temp2]++;
			nNs_B[temp3]++;
		}
	} 
}
