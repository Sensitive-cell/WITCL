// PalmDoc.cpp : implementation of the CPalmDoc class
//

#include "stdafx.h"
#include "Palm.h"

#include "PalmDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPalmDoc

IMPLEMENT_DYNCREATE(CPalmDoc, CDocument)

BEGIN_MESSAGE_MAP(CPalmDoc, CDocument)
	//{{AFX_MSG_MAP(CPalmDoc)
	ON_COMMAND(ID_Gray, OnGray)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPalmDoc construction/destruction

CPalmDoc::CPalmDoc()
{
	// TODO: add one-time construction code here
	m_sizeDoc.cx = 800;
	m_sizeDoc.cy = 600;

	m_palDIB = NULL;
	m_hDIB = NULL;
	harris=false;
}

CPalmDoc::~CPalmDoc()
{
		if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB=NULL;
	}	
	if (m_palDIB != NULL)
	{
		delete m_palDIB;
		m_palDIB=NULL;
	}

}

BOOL CPalmDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	m_hDIB = NewDIB(m_sizeDoc.cx, m_sizeDoc.cy,8);
	InitDIBData();
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CPalmDoc serialization

void CPalmDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPalmDoc diagnostics

#ifdef _DEBUG
void CPalmDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPalmDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPalmDoc commands

BOOL CPalmDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	//*****************��ͼ���ļ���lpszPathNameΪͼ���ļ���
	BOOL b  = ReadImgFile(lpszPathName);		//ReadImgFile-<<dpiapi.h(api����)
	if(b)
	{
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
		m_sizeDoc.cx = (int) ::DIBWidth(lpDIB);         // Size of DIB - x
		m_sizeDoc.cy = (int) ::DIBHeight(lpDIB);        // Size of DIB - y
		::GlobalUnlock((HGLOBAL) m_hDIB);
	}
	POSITION pos;
	pos = GetFirstViewPosition();
    this->harris=false;
	return b;	
	return TRUE;
}

BOOL CPalmDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	// TODO: Add your specialized code here and/or call the base class
		
	return CDocument::CanCloseFrame(pFrame);
}

BOOL CPalmDoc::ReadImgFile(CString sName)
{
		if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB=NULL;
		}	
	if (m_palDIB != NULL)
	{
		delete m_palDIB;
		m_palDIB=NULL;
	}

	// replace calls to Serialize with ReadDIBFile function
	CFile nFile;
	if(!nFile.Open(sName,CFile::modeRead))
		return false;
	m_hDIB = ReadDIBFile(nFile);
	nFile.Close();
	InitDIBData();

	if (m_hDIB == NULL)
		return FALSE;
	return TRUE;	
}
BOOL CPalmDoc::InitDIBData()
{
	if (m_palDIB != NULL)
	{
		delete m_palDIB;
		m_palDIB = NULL;
	}
	if (m_hDIB == NULL)
	{
		return FALSE;
	}
	m_palDIB = new CPalette;
	if (m_palDIB == NULL)
	{
		// we must be really low on memory
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB = NULL;
		return FALSE;
	}

	if (::CreateDIBPalette(m_hDIB, m_palDIB) == NULL)
	{
		// DIB may not have a palette
		delete m_palDIB;
		m_palDIB = NULL;
	}
	return TRUE;
}	

BOOL CPalmDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
		CFile nFile;
	if(m_hDIB == NULL)
		return  false;
	nFile.Open(lpszPathName,CFile::modeWrite | CFile::modeCreate);
	SaveDIB(m_hDIB, nFile);
	nFile.Close();
	return true;
	return CDocument::OnSaveDocument(lpszPathName);
}

void CPalmDoc::OnGray() 
{
	// TODO: Add your command handler code here

			int i,j;
    unsigned char *lpSrc,*lpDst;//һ��ָ��Դ��Ŀ�����ص��ƶ�ָ��

	//��Դͼ����в���
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)m_hDIB);
	int cxDIB = (int) ::DIBWidth(lpDIB);         
	int cyDIB = (int) ::DIBHeight(lpDIB);        
	LPSTR lpDIBBits=::FindDIBBits (lpDIB);	//�ҵ�Դͼ����ͼ����������ʼλ��
	long lLineBytesSrc = WIDTHBYTES(cxDIB * 8 * 3);// ����Դ24λ���ͼ��ÿ�е��ֽ���

	int numColors=(int) ::DIBNumColors((char *)lpDIB);
    if (numColors!=0)  //���numColors��0�����ʾĿǰͼ��Ϊ24λ���ͼ
	{
		::GlobalUnlock((HGLOBAL) m_hDIB);
		return;
	}

	//�´���һ��8λ��256���Ҷȣ���DIB���
	HDIB grayhDIB=NewDIB(cxDIB, cyDIB,8);
	LPSTR glpDIB=(LPSTR)::GlobalLock((HGLOBAL)grayhDIB);
	LPSTR glpDIBBits=::FindDIBBits (glpDIB);
	long lLineBytesDst = WIDTHBYTES(cxDIB * 8);// ����Ŀ��8λ�Ҷ�ͼ��ÿ�е��ֽ���
	// ÿ��
	for(i = 0; i < cyDIB; i++)
	{
		//ÿ��
		for(j = 0; j < cxDIB; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ�루�������Ϊ���ϵ��µģ�
			lpSrc = (unsigned char*)lpDIBBits + lLineBytesSrc * (cyDIB - 1 - i) + j*3;
			lpDst = (unsigned char*)glpDIBBits + lLineBytesDst * (cyDIB - 1 - i) + j;
			*lpDst=(*lpSrc)/3+(*(lpSrc+1))/3+(*(lpSrc+2))/3;
		}
	}
	m_hDIB=grayhDIB;
	::GlobalUnlock((HGLOBAL) m_hDIB);
	::GlobalUnlock((HGLOBAL) grayhDIB);
	UpdateAllViews(NULL, 0, NULL);	
}

void CPalmDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here

}
