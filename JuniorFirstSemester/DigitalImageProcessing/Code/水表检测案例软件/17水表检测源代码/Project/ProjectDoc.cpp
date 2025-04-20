// ProjectDoc.cpp : implementation of the CProjectDoc class
//

#include "stdafx.h"
#include "Project.h"
#include "MainFrm.h"

#include "ProjectDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectDoc

IMPLEMENT_DYNCREATE(CProjectDoc, CDocument)

BEGIN_MESSAGE_MAP(CProjectDoc, CDocument)
	//{{AFX_MSG_MAP(CProjectDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectDoc construction/destruction

CProjectDoc::CProjectDoc()
{
	int i;
	// TODO: add one-time construction code here
	bImageLoaded = FALSE;
	pImageBak = NULL;
	if ((pBitMapInfo=(PBITMAPINFO)LocalAlloc(LMEM_FIXED, sizeof(BITMAPINFOHEADER)
		+ 256 * sizeof(RGBQUAD))) != 0)
	{
		pBitMapInfo->bmiHeader.biSize = (long)sizeof(BITMAPINFOHEADER);
		pBitMapInfo->bmiHeader.biWidth = 0L;
		pBitMapInfo->bmiHeader.biHeight = 0L;
		pBitMapInfo->bmiHeader.biPlanes = 1;
		pBitMapInfo->bmiHeader.biBitCount = 8;
		pBitMapInfo->bmiHeader.biCompression = BI_RGB;
		pBitMapInfo->bmiHeader.biSizeImage = 0L;
		pBitMapInfo->bmiHeader.biXPelsPerMeter = 0L;
		pBitMapInfo->bmiHeader.biYPelsPerMeter = 0L;
		pBitMapInfo->bmiHeader.biClrUsed = 256;
		pBitMapInfo->bmiHeader.biClrImportant = 0L;
		for (i = 0; i < 256; i ++)
		{
			pBitMapInfo->bmiColors[i].rgbRed = i;
			pBitMapInfo->bmiColors[i].rgbGreen = i;
			pBitMapInfo->bmiColors[i].rgbBlue = i;
		}
	}


}

CProjectDoc::~CProjectDoc()
{
	if (bImageLoaded == TRUE)
	{
		GlobalFree(pImage);
		//GlobalFree(pHist);
	}
	if (pImageBak)
	{
		GlobalFree(pImage);
	}
	LocalFree(pBitMapInfo);
}


/////////////////////////////////////////////////////////////////////////////
// CProjectDoc serialization

void CProjectDoc::Serialize(CArchive& ar)
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
// CProjectDoc diagnostics

#ifdef _DEBUG
void CProjectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CProjectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProjectDoc commands

BOOL CProjectDoc::ReadImage(const char *szFileName)
{
	CFile file;
	CFileException fe;
	int nReadFromFile;
	long lImageSize;
	//int aaa,bbb;
//	AfxMessageBox(szFileName);

	//DeleteContents();//�ú�������Ϊɾ������CDocument�౾��֮���������Ϣ
	BeginWaitCursor();
	((CMainFrame *)AfxGetMainWnd())->SetStatus("���ڶ���ͼ��...");
	

	if (!file.Open(szFileName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		ReportSaveLoadException(szFileName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}
	
	//һЩ���ǣ����ǲ�ͬ���ļ���ʽ����bmp��jpg�ȣ�
	//��ͨ����ͬ�ĺ�׺�����벻ͬ���Ķ����򣬴˴δ���ҵ����Ҫ
	nReadFromFile = file.Read(&BitMapFileHeader, sizeof(BITMAPFILEHEADER));
	if (nReadFromFile < sizeof(BITMAPFILEHEADER))
	{
		MessageBox(NULL, "BMP�ļ�ͷ���ݲ��㣬ͼ���ļ�����","����ͼ���ļ�ͷ", MB_OK);
		//AfxMessageBox("BMP�ļ�ͷ���ݲ��㣬ͼ���ļ�����");
		file.Close();
		return FALSE;
	}

	if (BitMapFileHeader.bfType != 'MB')//�����д��0x4D42)
	{
		MessageBox(NULL, "BMP�ļ�ͷ���ݲ��㣬ͼ���ļ�����","����ͼ���ļ�ͷ", MB_OK);
		//AfxMessageBox("BMP�ļ�ͷ���ݲ���ȷ��ͼ���ļ�����");
		file.Close();
		return FALSE;
	}

	//nReadFromFile = file.Read(&BitMapInfoHeader, sizeof(BITMAPINFOHEADER));
	nReadFromFile = file.Read(pBitMapInfo,sizeof(BITMAPINFOHEADER));
	if (nReadFromFile < sizeof(BITMAPINFOHEADER))
	{
		MessageBox(NULL, "BMP�ļ�ͷ���ݲ��㣬ͼ���ļ�����","����ͼ���ļ�ͷ", MB_OK);
		//AfxMessageBox("BMP�ļ�ͷ���ݲ��㣬ͼ���ļ�����");
		file.Close();
		return FALSE;
	}
	BmpWidth = pBitMapInfo->bmiHeader.biWidth;
	BmpHeight = pBitMapInfo->bmiHeader.biHeight;
	BitCount = pBitMapInfo->bmiHeader.biBitCount;
	if (BitCount != 8 && BitCount != 24)
	{
		MessageBox(NULL, "������ֻ֧��256ɫ��24λ���ͼ�񣡳����˳���", 
			"�������ر�����", MB_OK);
		//AfxMessageBox("������ֻ֧��256ɫ��24λ���ͼ�񣡳����˳���");
		file.Close();
		return FALSE;
	}

	Compression = pBitMapInfo->bmiHeader.biCompression;
	if (Compression != BI_RGB)  //or != 0
	{
		MessageBox(NULL, "������ֻ֧�ֲ�ѹ����BMPͼ�񣡳����˳���", "����ѹ��", MB_OK);
		//AfxMessageBox("������ֻ֧�ֲ�ѹ����BMPͼ�񣡳����˳���");
		file.Close();
		return FALSE;

	}
	// ���򵽴˴�����������ɫ������ֻ��8����24
	if (BitCount != 24)   // ����8bit��ͼ����Ҫ��ȡ��ɫ��
	{
		//��ȡ��ɫ��
		ColorUsed = pBitMapInfo->bmiHeader.biClrUsed;
		if (ColorUsed ==0)
		{
			ColorUsed = (1<<(pBitMapInfo->bmiHeader.biBitCount));
			pBitMapInfo->bmiHeader.biClrUsed = ColorUsed;
		}
		//AfxMessageBox(_ecvt(ColorUsed,10,&aaa,&bbb));

		file.Read((BYTE *)pBitMapInfo+sizeof(BITMAPINFOHEADER), 
			ColorUsed * sizeof(RGBQUAD));
	}

	//��ȡͼ������
	//����ͼ�����ݣ�����ͼ��ռ�
	if (BitCount == 8)
	{
	
		if (BmpWidth % 4 != 0)
		{
			nWidth = (BmpWidth/4 + 1) * 4;
			lImageSize = (BmpWidth/4 + 1) * 4 * BmpHeight;
		}
		else
		{
			nWidth = BmpWidth;
			lImageSize = BmpWidth * BmpHeight;
		}
		//pHist = (BYTE *)GlobalAlloc(GMEM_FIXED, 256);
	}
	else
	{
		if (BmpWidth % 4 != 0)
		{
			nWidth = (BmpWidth * 3 / 4 + 1) * 4;
			lImageSize = (BmpWidth * 3 / 4 + 1) * 4 * BmpHeight;
		}
		else
		{
			nWidth = BmpWidth;
			lImageSize = BmpWidth * BmpHeight * 3;
		}
		//pHist = (BYTE *)GlobalAlloc(GMEM_FIXED, 256 * 3);
	}

	//AfxMessageBox(_ecvt(lImageSize,10,&aaa,&bbb));//+"~r~n"+
		//"height="+_ecvt(BmpHeight,10,&aaa,&bbb));

	pImage = (BYTE *)GlobalAlloc(GMEM_FIXED, lImageSize);
	bImageLoaded = TRUE;

    long nBytesRead = file.Read(pImage, lImageSize);
	if (nBytesRead != lImageSize)
	{
		MessageBox(NULL, "ͼ���ļ����𻵣�ϵͳ���˳�����", "��ͼ���ļ�", MB_OK);
		file.Close();
		return FALSE;
	}
	//AfxMessageBox(_ecvt(BitMapInfoHeader.biCompression,10,&aaa,&bbb));


	EndWaitCursor();
	((CMainFrame *)AfxGetMainWnd())->SetStatus("����");
	//SetPathName(szFileName);

	return TRUE;
}

BOOL CProjectDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	ReadImage(lpszPathName);
	SetModifiedFlag(FALSE);

	return TRUE;
}
void CProjectDoc::SetDocument(CProjectDoc* pDoc)
{
	long i;
	BitMapFileHeader = pDoc->BitMapFileHeader;
	BmpWidth = pDoc->BmpWidth;  
	BmpHeight = pDoc->BmpHeight; 
	BitCount = pDoc->BitCount; 
	Compression = pDoc->Compression;
	ColorUsed = pDoc->Compression;
	bImageLoaded = pDoc->bImageLoaded;
	nWidth = pDoc->nWidth;

	pBitMapInfo->bmiHeader = pDoc->pBitMapInfo->bmiHeader;
	for (i = 0; i < 255; i ++)
	{
		pBitMapInfo->bmiColors[i].rgbRed = pDoc->pBitMapInfo->bmiColors[i].rgbRed;
		pBitMapInfo->bmiColors[i].rgbGreen = pDoc->pBitMapInfo->bmiColors[i].rgbGreen;
		pBitMapInfo->bmiColors[i].rgbBlue = pDoc->pBitMapInfo->bmiColors[i].rgbBlue;
	}
	
	pImage = (BYTE *)GlobalAlloc(GMEM_FIXED, pDoc->pBitMapInfo->bmiHeader.biSizeImage);
	pImageBak = (BYTE *)GlobalAlloc(GMEM_FIXED, pDoc->pBitMapInfo->bmiHeader.biSizeImage);
	for (i = 0; i < (long)pDoc->pBitMapInfo->bmiHeader.biSizeImage; i ++)
	{
		pImage[i]= pDoc->pImage[i];
		pImageBak[i] = pDoc->pImage[i];
	}

	
//	PBITMAPINFO pBitMapInfo;//ͼ���ļ���Ϣ������ͼ���ļ���Ϣͷ�͵�ɫ�壩
	//BYTE * pImage;//ͼ������
	SetModifiedFlag(TRUE);
}

BOOL CProjectDoc::SaveModified() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (IsModified())
	{
		if (AfxMessageBox("Ҫ��ͼ�α���Ϊbmp�ļ���?", MB_YESNO) == IDNO)
		{
			return TRUE;
		}
	}
	else
	{
		return TRUE;
	}
	CString strSaveFileType = "λͼ�ļ� (*.bmp)|*.bmp|�����ļ� (*.*)|*.*||";
	CFileDialog FileDlg(TRUE, "*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		strSaveFileType);

	if (FileDlg.DoModal() == IDOK)
	{
		//OpenDocumentFile(FileDlg.m_ofn.lpstrFile);
		OnSaveDocument(FileDlg.m_ofn.lpstrFile);
		return TRUE;
	}
	else
		return FALSE;
	
	//return CDocument::SaveModified();
	return TRUE;

	//return CDocument::SaveModified();
}

BOOL CProjectDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	//AfxMessageBox("sfsdf");
	WriteToFile(lpszPathName);
	//return CDocument::OnSaveDocument(lpszPathName);
	return TRUE;
}

BOOL CProjectDoc::WriteToFile(const char *szFileName)
{
	CFile file;
	CFileException fe;
	BOOL bBadpath=FALSE;
	
	fileopenbegin:;
	if (!file.Open(szFileName, CFile::modeCreate | CFile::modeWrite , &fe))
	{
		if( fe.m_cause == CFileException::badPath) {
			if(bBadpath) {
				ReportSaveLoadException(szFileName, &fe,FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
				return FALSE;
			}
			char pathname[255];
			strcpy(pathname,szFileName);
			
			char *cc=strrchr(pathname, '\\');

			cc[1]=0;
			CreateDirectory(pathname,NULL);
			bBadpath=TRUE;
			goto fileopenbegin;
		} else if(fe.m_cause == CFileException::diskFull) {
			MessageBox(NULL,"��ɾ������Ҫ���ļ���","��������",MB_OK||MB_ICONWARNING);
			return FALSE;
		} else {
			ReportSaveLoadException(szFileName, &fe,FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
			return FALSE;
		}
	}

	DeleteContents();
	BeginWaitCursor();
/*
	int width=pDibInfo->bmiHeader.biWidth;
	int height=pDibInfo->bmiHeader.biHeight;
	int biBitCount=pDibInfo->bmiHeader.biBitCount;
	if(biBitCount==8) pDibInfo->bmiHeader.biClrUsed=256;
	BitmapFileHeader.bfType=0x4d42;  // "BM"
	BitmapFileHeader.bfSize=(DWORD) (sizeof(BITMAPFILEHEADER) + 
                 pDibInfo->bmiHeader.biSize + pDibInfo->bmiHeader.biClrUsed 
                 * sizeof(RGBQUAD) + width*height*biBitCount/8);     
	BitmapFileHeader.bfReserved1=0;
	BitmapFileHeader.bfReserved1=0;
	BitmapFileHeader.bfOffBits=(DWORD) sizeof(BITMAPFILEHEADER) + 
                    pDibInfo->bmiHeader.biSize + pDibInfo->bmiHeader.biClrUsed 
                    * sizeof (RGBQUAD); 
					*/
	file.Write(&BitMapFileHeader,sizeof(BITMAPFILEHEADER));
	file.Write(pBitMapInfo,sizeof(BITMAPINFOHEADER)+ pBitMapInfo->bmiHeader.biClrUsed 
                    * sizeof (RGBQUAD));
	file.Write(pImage,nWidth * BmpHeight * BitCount/8);
	file.Close();
  
 	EndWaitCursor();
 	SetPathName(szFileName);
	SetModifiedFlag(FALSE);     

	return TRUE;
}

