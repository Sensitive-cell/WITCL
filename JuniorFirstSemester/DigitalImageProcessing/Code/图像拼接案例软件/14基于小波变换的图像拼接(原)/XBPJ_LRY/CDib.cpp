// CDib.cpp: implementation of the CDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CDib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define M 2
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDib::CDib()
{
	m_pBMI = NULL;
	m_pDIBData = NULL;
	m_pFileHeader = NULL;
	m_pBMFHeader = NULL;

	m_ImageData = 0;    
	m_ImageCount = 0;
	m_ByteWidth = 0;
}

CDib::CDib(BYTE *pSrc)
{
	BYTE *temp1;
	
	if(!pSrc) 
	{
		AfxMessageBox("CDib ����BYTE *pSrc����Ϊ��!");
        	return;
	}
	
	unsigned i = 0;
	m_pBMFHeader = new BITMAPFILEHEADER;
	if(!m_pBMFHeader)
	{
		AfxMessageBox("��̬�ڴ����ʧ�ܣ�");
		return;
	}

	// λͼ�ļ���Ϣͷ
	temp1 = (BYTE*)m_pBMFHeader;
	for(i = 0; i<14; i++)
	{
		*temp1++ = *(pSrc+i);
	}
	//*m_pBMFHeader = *(BITMAPFILEHEADER*)(pSrc);   

	m_ImageCount = m_pBMFHeader->bfSize;
	m_ImageData  = m_pBMFHeader->bfSize - m_pBMFHeader->bfOffBits;

	BYTE *temp;
	temp = new BYTE[m_pBMFHeader->bfOffBits - 14];
	
	if(!temp)
	{
		AfxMessageBox("��̬�ڴ����ʧ�ܣ�");
		return;
	}

	for (i = 0; i<m_pBMFHeader->bfOffBits - 14; i++ )
	{
		*(temp + i) = *(pSrc + 14 + i);
	}

	m_pBMI = (BITMAPINFO*)(temp);   // λͼ�ļ���Ϣͷ

	m_pDIBData = NULL;
	m_pDIBData = new BYTE[m_ImageData];
	if(!m_pDIBData)
	{
		AfxMessageBox("��̬�ڴ����ʧ�ܣ�");
		return;
	}
	
	for(i = 0; i<m_ImageData; i++)
	{
		*(m_pDIBData+i) = *(pSrc + i + m_pBMFHeader->bfOffBits);
	}

	m_pFileHeader = new BYTE[m_ImageCount];
	if(!m_pFileHeader)
	{
		AfxMessageBox("��̬�ڴ����ʧ�ܣ�");
		return;
	}
	
	for(i = 0; i<m_ImageCount; i++)
	{
		*(m_pFileHeader+i) = *(pSrc + i);
	}

	
}

CDib::CDib(CDib &dib)
{
	m_ImageData = dib.m_ImageData;
	m_ImageCount = dib.m_ImageCount;

	m_pBMFHeader = new BITMAPFILEHEADER;

	if(!m_pBMFHeader)
	{
		AfxMessageBox("��̬�ڴ����ʧ�ܣ�");
		return;
	}

	*m_pBMFHeader = *(dib.m_pBMFHeader);

	m_pBMI = (BITMAPINFO*)new BYTE[m_pBMFHeader->bfOffBits - sizeof(BITMAPFILEHEADER)];
	
	if(!m_pBMI)
	{
		AfxMessageBox("��̬�ڴ����ʧ�ܣ�");
		return;
	}

	*m_pBMI = *(dib.m_pBMI);

	m_pDIBData = new BYTE[m_ImageData];
	if(!m_pDIBData)
	{
		AfxMessageBox("��̬�ڴ����ʧ�ܣ�");
		return;
	}
	
	unsigned i;
	for(i = 0; i<m_ImageData; i++)
	{
		*(m_pDIBData+i) = *(dib.m_pDIBData + i);
	}


	m_pFileHeader = new BYTE[m_ImageCount];
	if(!m_pFileHeader)
	{
		AfxMessageBox("��̬�ڴ����ʧ�ܣ�");
		return;
	}
	
	for(i = 0; i<m_ImageCount; i++)
	{
		*(m_pFileHeader+i) = *(dib.m_pFileHeader + i);
	}

	m_ByteWidth=dib.m_ByteWidth;
}

CDib::CDib(LPCTSTR lpszPathName)
{
	m_pBMI = NULL;
	m_pDIBData = NULL;
	m_pFileHeader = NULL;
	m_pBMFHeader = NULL;

	m_ImageData = 0;    
	m_ImageCount = 0;
	m_ByteWidth=0;
	LoadFromFile( lpszPathName );
}


void CDib::operator = (CDib &dib)
{
	m_ImageData = dib.m_ImageData;
	m_ImageCount = dib.m_ImageCount;
	m_ByteWidth = dib.m_ByteWidth;

	if(m_pBMFHeader) delete m_pBMFHeader;
	m_pBMFHeader = new BITMAPFILEHEADER;

	if(!m_pBMFHeader)
	{
		AfxMessageBox("��̬�ڴ����ʧ�ܣ�");
		return;
	}

	*m_pBMFHeader = *(dib.m_pBMFHeader);
	
	if(m_pBMI) delete m_pBMI;
	BYTE *temp = new BYTE[m_pBMFHeader->bfOffBits - sizeof(BITMAPFILEHEADER)];
	if(!temp)
	{
		AfxMessageBox("��̬�ڴ����ʧ�ܣ�");
		exit(1);
	}
	BYTE *p = (BYTE*)dib.m_pBMI;
	for(DWORD i = 0; i<m_pBMFHeader->bfOffBits-14; i++)
	{
		*(temp+i) = *(p++);
	}
	
	m_pBMI = (BITMAPINFO*)temp;

	if(m_pDIBData) delete []m_pDIBData;	
	m_pDIBData = new BYTE[m_ImageData];
	if(!m_pDIBData)
	{
		AfxMessageBox("��̬�ڴ����ʧ�ܣ�");
		return;
	}
	
	for(i = 0; i<m_ImageData; i++)
	{
		*(m_pDIBData+i) = *(dib.m_pDIBData + i);
	}

	if(m_pFileHeader) delete []m_pFileHeader;	
	m_pFileHeader = new BYTE[m_ImageCount];
	if(!m_pFileHeader)
	{
		AfxMessageBox("��̬�ڴ����ʧ�ܣ�");
		return;
	}
	
	for(i = 0; i<m_ImageCount; i++)
	{
		*(m_pFileHeader+i) = *(dib.m_pFileHeader + i);
	}
	
}

CDib::~CDib()
{
	if(m_pBMI!=NULL)     delete m_pBMI;
	if(m_pDIBData!=NULL) delete []m_pDIBData;
	if(m_pFileHeader!=NULL) delete []m_pFileHeader;
	if(m_pBMFHeader!=NULL) delete m_pBMFHeader;
}


BOOL CDib::LoadFromFile(LPCTSTR lpszFileName)
{
	CFile file;
//λͼָ���ʼ��
	BITMAPINFO          *pBMI = NULL;
	BYTE				*pDIBData = NULL;
	BYTE				*pFileHeader = NULL;//λͼ�ļ�ָ��
	BITMAPFILEHEADER	*pBMFHeader = NULL;

	//��ָ�����ļ�
	if(!file.Open(lpszFileName,CFile::modeRead|CFile::typeBinary))
	   {
	    	AfxMessageBox("can't open this file!");
		    return FALSE;
	   }
	            
	//����λͼBITMAPFILEHEADER�ļ�  14���ֽ�
	pBMFHeader = (BITMAPFILEHEADER*) new BYTE[sizeof(BITMAPFILEHEADER)];
	if(file.Read(pBMFHeader,sizeof(BITMAPFILEHEADER)) != sizeof(BITMAPFILEHEADER))
	{
		AfxMessageBox("error");
		return FALSE;
	}
	
	//������BMPλͼ,���ڴ���
	if(pBMFHeader->bfType != 0x4d42) //0X4D42 ��'BM'��ʮ�����Ʊ�ʾ         
	{
		AfxMessageBox("isn't BMP image.");
		return FALSE;
	}

	if(m_pBMFHeader != NULL)  delete m_pBMFHeader;// m_FileHeader��¼ͼ��ʵ�ʵ�ָ��λ��
		m_pBMFHeader = pBMFHeader;

	//����λͼ�ļ���Ϣ
	pBMI=(BITMAPINFO*) new BYTE[pBMFHeader->bfOffBits - sizeof(BITMAPFILEHEADER)];
	if(!pBMI)//�����bfh.bfOffBits-sizeof(BITMAPFILEHEADER)����ֵΪ0��
	{        //��λͼ�ļ���Ϣ�ڴ�������
		AfxMessageBox(" λͼ�ļ���Ϣ�ڴ�������");	
		return FALSE;
	}
	
	if(file.Read(pBMI, pBMFHeader->bfOffBits-sizeof(BITMAPFILEHEADER)) != pBMFHeader->bfOffBits-sizeof(BITMAPFILEHEADER))
	{
		AfxMessageBox("���ļ���Ϣͷ����!");
		return FALSE;
	}

	if(m_pBMI != NULL)	delete m_pBMI;  
		m_pBMI = pBMI; 
	
	// ******��ʼ��ȡ����****************************************

	
   		m_ImageData = pBMFHeader->bfSize - pBMFHeader->bfOffBits;//λͼʵ�����ݴ�С
		pDIBData = new BYTE[m_ImageData];//�洢λͼʵ�����ݵ��ڴ�ռ�

		if(!pDIBData)// ��pDIBDataֵΪ0������
		{
			AfxMessageBox("ͼ��ʵ�������ڴ�������");
			return FALSE;
		}
	
		//ʹָ��λͼ�ļ���ָ��ָ��λͼ�ļ���ʵ�����ݲ���
		file.Seek(pBMFHeader->bfOffBits,CFile::begin);
		if(file.Read(pDIBData, m_ImageData) != m_ImageData)//��λͼʵ������ʧ��
		{
			AfxMessageBox("��λͼʵ�����ݳ���");
			return FALSE;
		}

		file.SeekToBegin();//ʹָ���ļ���ָ��ص��ļ��Ŀ�ʼλ��
		m_ImageCount = file.GetLength();
		       //λͼ�ļ���СΪImageCount        
		pFileHeader = new BYTE[m_ImageCount];
		       //FilePointerHeaderλͼ�ļ�ͷָ��,FilePointerHeader
               //��ָ��շ����ͼ���С��һ���ڴ��ָ��
		file.Read(pFileHeader,m_ImageCount);
               //����FILE�ĺ�����ͼ��             
		file.Close();	       
				//����ͼ��ÿ��ʵ�ʿ��  ��λ���ֽ�
		//m_ByteWidth=(GetW()*m_pBMI->bmiHeader.biBitCount+31)/32*4;
		m_ByteWidth=((GetW()*m_pBMI->bmiHeader.biBitCount+31)&~31)>>3;

		if(m_pDIBData != NULL)   delete []m_pDIBData; //�ͷžɵ�ָ������
		m_pDIBData = pDIBData;                    //������ֵ
	
		if(m_pFileHeader != NULL)   delete []m_pFileHeader; //�ͷžɵ�ָ������
		m_pFileHeader = pFileHeader;                    //������ֵ
		return TRUE;

}


BOOL CDib::SaveFile(CString filename)
{	
	CFile file;
	//����ָ�����ļ�
	if(!file.Open(filename, CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox("can't be saved for that it existed already !");
		return FALSE;
	}

	file.SeekToBegin();
	file.Write(m_pFileHeader,m_ImageCount);//д��ͼ���ļ�
	file.Close();
	return TRUE;
}

void CDib::ShowDIB(CDC  *pDC,int nLeft,int nTop)
{
	//pDC->SetStretchBltMode(COLORONCOLOR);//������������ģʽ
	StretchDIBits(pDC->GetSafeHdc(),//DC�ľ��
		nLeft,                      //Ŀ��������Ͻǵ�x����
		nTop,                       //Ŀ��������Ͻǵ�Y����
		GetW(),                     //Ŀ����εĿ��
 		GetH(),                     //Ŀ����εĸ߶�
		0,                          //ԭ�������Ͻǵ�x����
		0,                          //ԭ�������Ͻǵ�Y����
		GetW(),                     //ԭ���εĿ��     **
		GetH(),                     //ԭ���εĸ߶�     **
		m_pDIBData,                 //λͼͼ�����ݵĵ�ַ**
		m_pBMI,                     //λͼ��Ϣ�ṹ��ַ **
		DIB_RGB_COLORS,             //��־ѡ��
		SRCCOPY);                   //��դ������
}

UINT CDib::GetW()
{	
	return m_pBMI->bmiHeader.biWidth;
}
	

UINT CDib::GetH()
{	
	return m_pBMI->bmiHeader.biHeight;
}

void CDib::updatedata(void)
{
	unsigned long i = 0;
	BYTE *p = m_pFileHeader + m_pBMFHeader->bfOffBits;
	
	for(i = 0; i<m_ImageData; i++)
	{
		*(p++) = *(m_pDIBData + i);
	}
}
