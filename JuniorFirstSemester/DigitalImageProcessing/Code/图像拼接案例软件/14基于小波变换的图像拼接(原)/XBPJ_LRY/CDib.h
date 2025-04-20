// CDib.h: interface for the CDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIB_H__1A2F121B_2FC3_4D06_9B9A_D8CF4F0619D2__INCLUDED_)
#define AFX_DIB_H__1A2F121B_2FC3_4D06_9B9A_D8CF4F0619D2__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//�ȴ�����
#define WaitCursorBegin()	HCURSOR hcURSOR = SetCursor(LoadCursor(NULL, IDC_WAIT))
#define WaitCursorEnd()		SetCursor(hcURSOR) 

// �����������Ŀ��

#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)

// �����������ĸ߶�
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

class CDib  
{

public:
	BITMAPINFO			*m_pBMI;//λͼ��Ϣͷ�ṹָ��
	BITMAPFILEHEADER	*m_pBMFHeader;//λͼ�ļ�ͷ�ṹָ��   

	BYTE				*m_pDIBData;//DIBͼ������ָ��
	BYTE				*m_pFileHeader; //λͼ�ļ�ͷָ��
	
	DWORD				m_ImageData;//λͼͼ��ʵ�����ݴ�С
	DWORD          		m_ImageCount;//λͼ�ļ���С

	int					m_ByteWidth;//ͼ��ÿ�е��ֽ�����ͼ���ɨ����
public:  // ���캯������������
	CDib();
	CDib(BYTE *pSrc);
	CDib(CDib &dib);
	CDib(LPCTSTR lpszPathName);
	void operator = (CDib &dib);
	virtual ~CDib();
	
	UINT GetW();//��ȡDIBͼ��Ŀ��
	UINT GetH();//��ȡDIBͼ��ĸ߶�
	void updatedata(void); // ��m_pDIBData�����ݸ���m_pFILEHeader�е�ͼ����������
	BOOL SaveFile(CString filename);  // ����DIBͼ���ļ�
	BOOL LoadFromFile(LPCTSTR lpszFileName);    //��DIB�����ļ��ж����ڴ��У��Դ�����
   	void ShowDIB(CDC  *pDC,int nLeft,int nTop); //��ʾDIBλͼ	

 };



#endif // !defined(AFX_DIB_H__1A2F121B_2FC3_4D06_9B9A_D8CF4F0619D2__INCLUDED_)
