// DSplit.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DSplit.h"

#include "MainFrm.h"
#include "DSplitDoc.h"
#include "DSplitView.h"
#include "DynSplitView2.h"

#include "CDib.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include <vector>




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CDSplitApp

BEGIN_MESSAGE_MAP(CDSplitApp, CWinApp)
	//{{AFX_MSG_MAP(CDSplitApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDSplitApp construction

CDSplitApp::CDSplitApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDSplitApp object

CDSplitApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDSplitApp initialization

BOOL CDSplitApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDSplitDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CDSplitView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CDSplitApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDSplitApp commands
// FingerprintDlg.cpp : implementation file
//


////////////////////////////////////////////////////////////////////////////
//�������Ͷ��壺
/* Basic Types */
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef short sint16;
typedef unsigned long uint32;
typedef int sint32;
//typedef long sint32;
typedef char sint8;

typedef uint32 VF_RETURN;

typedef uint8  VF_FLAG;


#define VF_OK (0)
typedef struct tagMatchResult { 
	sint32    Similarity; 
	sint32    Rotation; 
	sint32    TransX;
	sint32    TransY;
	sint32	MMCount;
} MATCHRESULT, *PMATCHRESULT; 


#define VF_MINUTIA_END        1	// �˵�
#define VF_MINUTIA_FORK       2	// ���
#define VF_MINUTIA_CORE		  3 //���ĵ�
#define VF_MINUTIA_DELTA	  4 //���ǵ�

// ��ȷ�ȶ�ģʽ����Ҫ���������ȶԴ�������
#define	VF_MATCHMODE_VERIFY			1
// ���ٱȶ�ģʽ����Ҫ���ڴ����ȶԴ�������
#define	VF_MATCHMODE_IDENTIFY		2

////////////////////////////////////////////////////////
#define		MAXIMGW				640		// ����ͼ�������
#define		MAXIMGH				480		// ����ͼ�����߶�
#define		MAXIMGSIZE			(MAXIMGW*MAXIMGH) // ����ͼ������С

sint32  	IMGW = 256;		// ��ǰ����ͼ��Ŀ��
sint32      IMGH = 360;		// ��ǰ����ͼ��ĸ߶�
sint32      IMGSIZE = (IMGW*IMGH);	// ��ǰ����ͼ��Ĵ�С


uint8		g_OrgFinger[MAXIMGSIZE];	// ����ͼ�����ݻ�����
uint8		g_Orient[MAXIMGSIZE];		// �������ݻ�����
uint8		g_Divide[MAXIMGSIZE];		// �ָ�ͼ���ݻ�����
uint8		g_Temp[MAXIMGSIZE];			// ��ʱ���ݻ�����

uint8		*g_lpOrgFinger = &g_OrgFinger[0];	// ����ͼ�����ݻ������׵�ַ
uint8		*g_lpOrient = &g_Orient[0];			// �������ݻ������׵�ַ
uint8		*g_lpDivide = &g_Divide[0];			// �ָ�ͼ���ݻ������׵�ַ
uint8		*g_lpTemp = &g_Temp[0];				// ��ʱ���ݻ������׵�ַ

#define     MAXMINUTIANUM			60			// �����������
#define     MAXRAWMINUTIANUM        100			// �������������ĳ�ʼ�����������Ŀ

sint32      g_MinutiaNum = 0;					// ��������֮��

#define  PI          3.141593
#define  EPI         57.29578

// ��ĳ��Ϊ���ĵ�12������������7����λ�õĺ���ƫ��
sint32  g_DSite[12][7][2] = {
	-3, 0,  -2, 0,  -1, 0,   0, 0,   1, 0,   2, 0,   3, 0,
		-3,-1,  -2,-1,  -1, 0,   0, 0,   1, 0,   2, 1,   3, 1,
		-3,-2,  -2,-1,  -1,-1,   0, 0,   1, 1,   2, 1,   3, 2,
		-3,-3,  -2,-2,  -1,-1,   0, 0,   1, 1,   2, 2,   3, 3,
		-2,-3,  -1,-2,  -1,-1,   0, 0,   1, 1,   1, 2,   2, 3,
		-1,-3,  -1,-2,   0,-1,   0, 0,   0, 1,   1, 2,   1, 3,
		0,-3,   0,-2,   0,-1,   0, 0,   0, 1,   0, 2,   0, 3,
		-1, 3,  -1, 2,   0, 1,   0, 0,   0,-1,   1,-2,   1,-3,
		-2, 3,  -1, 2,  -1, 1,   0, 0,   1,-1,   1,-2,   2,-3,
		-3, 3,  -2, 2,  -1, 1,   0, 0,   1,-1,   2,-2,   3,-3,
		-3, 2,  -2, 1,  -1, 1,   0, 0,   1,-1,   2,-1,   3,-2,
		-3, 1,  -2, 1,  -1, 0,   0, 0,   1, 0,   2,-1,   3,-1
};

// Ϊ��ʡʱ�䣬�̾��������������ò��(���ڶԱ�ƥ��)
// DisTbl[m][n] = (int)(sqrt(m*m+n*n)+0.5)
sint32  DisTbl[10][10] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
		1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
		2, 2, 2, 3, 4, 5, 6, 7, 8, 9, 
		3, 3, 3, 4, 5, 5, 6, 7, 8, 9, 
		4, 4, 4, 5, 5, 6, 7, 8, 8, 9, 
		5, 5, 5, 5, 6, 7, 7, 8, 9, 10, 
		6, 6, 6, 6, 7, 7, 8, 9, 10, 10, 
		7, 7, 7, 7, 8, 8, 9, 9, 10, 11, 
		8, 8, 8, 8, 8, 9, 10, 10, 11, 12, 
		9, 9, 9, 9, 9, 10, 10, 11, 12, 12
};

// ������ṹ
typedef struct tagMinutiae { 
	sint32    x;				// ������
	sint32    y;				// ������
	sint32    Direction;		// ����
	sint32	Triangle[3];	// ������Ϊ�������Բ�뾶Ϊ��ֵ������������������ķ���
	sint32    Type;			// ����
} MINUTIA, *MINUTIAPTR; 

// ָ������(ģ��)�ṹ
typedef struct tagFeature{
	sint32		MinutiaNum;					// ��������
	MINUTIA		MinutiaArr[MAXMINUTIANUM];	// ����������
} FEATURE, *FEATUREPTR;

FEATURE    g_Feature;		// ��ǰָ�Ƶ�ָ������

#define SAFE_FREE(x) { if (x != NULL) free((void *)x); x = NULL; }

#define WIDTHBYTES(i)    ((i+3)/4*4)
///////////////////////////////////////////////////////////////
typedef struct DblPont
{
	double x;
	double y;
}DBLPOINT;

/*
typedef struct tagPOINT
{
	long x;
	long y;

}POINT,*PPOINT,NEAR *NPPOINT,FAR *LPPOINT;
*/
#define	MAX_SINGULARYNUM	30
/////////////////////////////////////////////////////////////////
#define		VF_BASE_ERR		10000

#define     VF_LOADFINGER_ERR			(VF_BASE_ERR+100)
#define     VF_SAVEFINGER_ERR			(VF_BASE_ERR+200)
#define     VF_IMPORTFINGER_ERR			(VF_BASE_ERR+300)

#define     VF_PROCESS_ERR				(VF_BASE_ERR+400)
#define     VF_EXTRACT_ERR				(VF_BASE_ERR+500)
#define     VF_FEATUREENCODE_ERR		(VF_BASE_ERR+600)
#define     VF_FEATUREDECODE_ERR		(VF_BASE_ERR+700)
#define     VF_VERIFYMATCH_ERR			(VF_BASE_ERR+800)
#define     VF_SAVEFEATURE_ERR			(VF_BASE_ERR+900)
#define     VF_LOADFEATURE_ERR			(VF_BASE_ERR+1000)


#define SizeCDib 32
/////////////////////////////////////////////////////////////////////////////
//�������壺
int FPA_CreateImage(LPVOID lpDib);
int FPA_LoadImage(CString filename, LPVOID lpDib, BYTE **lpData, int *lpWidth, int *lpHeight);
int FPA_AnalyzeFeature_File(CString FileName, BYTE *lpFeature, int *lpSize);
int AnalyzeFromFile(LPCSTR lpszFileName, BYTE *lpFeature, int *lpSize);
VF_RETURN	VF_LoadFinger(LPCSTR lpszFileName);
sint32	loadBitmap(sint8 *lpszFileName, uint8 *lpBitmap, sint32 *lpWidth, sint32 *lpHeight);
VF_RETURN	VF_Process();
void smooth(uint8 *lpInBuffer, uint8 *lpOutBuffer, sint32 r, sint32 d);
void zoomout();
void getOrientMap(sint32  r);
sint32	saveBitmap(uint8 *lpBitmap, sint32 Width, sint32 Height, sint8 *lpszFileName);
sint32 divide(sint32  r, sint32 threshold);
void orientFilter();
sint32  DIndex(sint32 angle);
sint32 binary();
void binaryClear();
void clearEdge();
sint32 thin();
sint32  thinClear(sint32  len);
sint32 imageThin(uint8 *lpBits, sint32 Width, sint32 Height);
bool   IsFork(uint8 *lpNow);
sint32  GetNext(uint8 *lpNow, uint8 *lpLast, uint8 **lppNext);
sint32  getMinutia();
bool   IsEnd(uint8 *lpNow);
sint32  PX(uint8 *lpPos);
sint32  PY(uint8 *lpPos);
sint32  GetJiajiao(sint32 angle1, sint32 angle2);
sint32  GetByDis(uint8 *lpEnd, uint8 **lppPos, sint32 d);
sint32  GetAngleDis(sint32 angleBegin, sint32 angleEnd);
sint32  GetAngle(sint32 x0, sint32 y0, sint32 x1, sint32 y1);
int getSingular(BYTE *lpOrient, int Width, int Height, char flag);
int getOriChange(int angle1, int angle2, char flag);
VF_RETURN VF_FeatureEncode(FEATUREPTR lpFeature, uint8 *lpData, sint32 *lpSize);
sint32	EncodeFeature(FEATUREPTR lpFeature, uint8 *lpData);
int FPA_PatternMatch(BYTE *lpFeature1, BYTE *lpFeature2, int *lpScore);
int PatternMatch(BYTE *lpFeature1, BYTE *lpFeature2, int *lpScore);
VF_RETURN VF_FeatureDecode(uint8 *lpData, FEATUREPTR lpFeature);
sint32	DecodeFeature(uint8 *lpData, FEATUREPTR lpFeature);
BOOL	VF_VerifyMatch(FEATUREPTR lpFeature1, FEATUREPTR lpFeature2,PMATCHRESULT lpMatchResult, VF_FLAG matchMode);
void	verify(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, PMATCHRESULT lpMatchResult,VF_FLAG matchMode);
void  globalMatch(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, PMATCHRESULT lpMatchResult, VF_FLAG matchMode);
void align(FEATUREPTR lpFeature, FEATUREPTR lpAlignedFeature, MINUTIAPTR lpFeatureCore,sint32 rotation, sint32 transx, sint32 transy);
void alignMatch(FEATUREPTR lpAlignFeature, FEATUREPTR lpTemplate, PMATCHRESULT lpMatchResult, VF_FLAG matchMode);
sint32  AngleAbs360(sint32 angle1, sint32 angle2);




/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

LPVOID lpImage1 = new BYTE[SizeCDib];
LPVOID lpImage2 = new BYTE[SizeCDib];

BYTE *lpData1;
BYTE *lpData2;

int Width1, Height1;
int Width2, Height2;

CString ImagePathName1, ImagePathName2;

BYTE lpFeature1[430];
BYTE lpFeature2[430];

 


////////////////////////////////////////////////////////////////
//�Զ��庯��ʵ��
int FPA_CreateImage(LPVOID lpDib)
{
	CDib dib;
	memcpy(lpDib, &dib, sizeof(CDib));
	
	return 0;
}



int FPA_LoadImage(CString filename, LPVOID lpDib, BYTE **lpData, int *lpWidth, int *lpHeight)
{
	CDib *dib = (CDib*)(lpDib);
	
	char *path = new char[filename.GetLength()];
	path = filename.GetBuffer(0);
	dib->Open(path);
	
	*lpData = dib->m_pDibBits;
	*lpWidth = dib->GetWidth();
	*lpHeight = dib->GetHeight();
	
	return 0;
}

int FPA_AnalyzeFeature_File(CString FileName, BYTE *lpFeature, int *lpSize)
{
	return AnalyzeFromFile((LPCSTR)FileName, lpFeature, lpSize);

}


//
// ExtractEx: ��ָ��ͼ��BMP�ļ�����ȡָ������
//
int AnalyzeFromFile(LPCSTR lpszFileName, BYTE *lpFeature, int *lpSize)
{
	//////////////////////////////////////////////////////////////////////////
	//  lpszFileName: [in] ָ��ͼ���ļ�·��
	//  Resolution: [in] ָ��ͼ��ֱ���
	//  lpFeature: [out] ��ȡ��ָ���������ݻ�����ָ��
	//  lpSize: [out] ��ȡ��ָ���������ݴ�С
	//////////////////////////////////////////////////////////////////////////
	// TODO: Add your implementation code here
	VF_RETURN	re;
	
	// ��ȡָ��ͼ������
	re = VF_LoadFinger(lpszFileName);
	if(re != VF_OK)
		return re;
	
	// ����ָ��ͼ����ȡָ������
	re = VF_Process();
	//if(re != VF_OK)
//		return re;
	  
	// ��ָ���������б���
	re = VF_FeatureEncode(&g_Feature, lpFeature, lpSize);
	if(re != VF_OK)
		return re;
	
	return 0;
}


VF_RETURN	VF_LoadFinger(LPCSTR lpszFileName)
{
	///////////////////////////////////////////////////////////////////////
	//	lpszFileName: [in] ָ��ͼ���ļ���
	///////////////////////////////////////////////////////////////////////
	sint32  bufferSize = 0;
	sint32  Width = 0;
	sint32  Height = 0;
	sint32  re;
	
	//	��ȡ��Ҫ�ڴ�ռ��С�������ڴ�
	re = loadBitmap((char *)lpszFileName, NULL, &Width, &Height);
	if(re != 0)
	{
		return VF_LOADFINGER_ERR;
	}
	
	if(Width > MAXIMGW || Height > MAXIMGH)
	{
		return VF_LOADFINGER_ERR;
	}
	//	��ȡ���ݵ�ͼ��������
	re = loadBitmap((char *)lpszFileName, g_lpOrgFinger, &Width, &Height);
	if(re != 0)
	{
		return VF_LOADFINGER_ERR;
	}
	
	// ���ô����ͼ���С
	IMGW = Width;
	IMGH = Height;
	IMGSIZE = (sint32)(IMGW * IMGH);
	
	return	VF_OK;
}



//
//	loadBitmap: ��BMP�ļ��ж�ȡͼ������
//
sint32	loadBitmap(sint8 *lpszFileName, uint8 *lpBitmap, sint32 *lpWidth, sint32 *lpHeight)
{
	/////////////////////////////////////////////////////////////////////
	//  lpszFileName: [in] BMP�ļ���
	//	lpBitmap: [out] ������ͼ������
	//  lpWidth: [out] ͼ����
	//	lpHeight: [out] ͼ��߶�
	/////////////////////////////////////////////////////////////////////
   	FILE               *fp = NULL;
	uint8			   palatte[1024];
	BITMAPFILEHEADER   bf;
	BITMAPINFOHEADER   bi;
	sint32			   i;
	sint32             lineBytes = 0;
	sint32             Width;
	sint32             Height;
	sint32			   Size = 0;
	
	// ���ļ�
	fp = fopen(lpszFileName, "rb");
	if(fp == NULL)
	{
		return 1;
	}
	// ���ļ�ͷ
	fread((void *)&bf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread((void *)&bi, sizeof(BITMAPINFOHEADER), 1, fp);
	// ����Ƿ�ΪBMP�ļ�
	if(bf.bfType != 19778)
	{
		fclose(fp);
		return 2;
	}
	// ����Ƿ�Ϊ256ɫ�Ҷ�ͼ��
	if(bi.biBitCount != 8 && bi.biClrUsed != 256)
	{
		fclose(fp);
		return 3;
	}
	// �õ�ͼ���С
	Height = bi.biHeight;
	Width = bi.biWidth;
	
	// ������ָ�벻Ϊ�����ȡͼ�����ݵ�������
	if(lpBitmap != NULL)
	{
		lineBytes = (sint32)WIDTHBYTES(Width);
		Size = lineBytes*Height;
		
		fread(palatte, 1, 1024, fp);
		// ���ж�ȡ��ÿ��ֻ��ȡWidth���ֽ�
		for(i = 0; i < Height; i++)
		{
			fseek(fp, 1078+lineBytes*i, SEEK_SET);
			fread((void *)(lpBitmap+i*Width), 1, Width, fp);
		}
	}
	fclose(fp);	
	
	*lpWidth = Width;
	*lpHeight = Height;
	
	return 0;
}


//
// ��ָ��ͼ����д�������ȡָ������
//
VF_RETURN	VF_Process()
{
	sint32	re = 0;
	
	// ��ָ��ͼ���������ƽ������������
	smooth(g_lpOrgFinger, g_lpTemp, 1, 1);
	memcpy(g_lpOrgFinger, g_lpTemp, IMGSIZE);
	smooth(g_lpOrgFinger, g_lpTemp, 1, 1);
	memcpy(g_lpOrgFinger, g_lpTemp, IMGSIZE);
	
	// ����ͼ���Ƶ���ּ��㷽�򳡣�����ٶ�
	zoomout();
	// ���㷽��
	getOrientMap(6);
	saveBitmap(g_lpOrient, IMGW, IMGH, "c:\\0.bmp");
	// �ָ�ǰ������
	re = divide(12, 35);
	if(re != 0)
		return VF_PROCESS_ERR;
	saveBitmap(g_lpDivide, IMGW, IMGH, "c:\\00.bmp");
	
	// ���ζ����˲�
	orientFilter();
//	orientFilter();
	saveBitmap(g_lpOrgFinger, IMGW, IMGH, "c:\\1.bmp");
	
	// ��ֵ��
	binary();
	saveBitmap(g_lpOrgFinger, IMGW, IMGH, "c:\\2.bmp");
	
	// ��ֵ��ͼ��ȥ����
	binaryClear();
	saveBitmap(g_lpOrgFinger, IMGW, IMGH, "c:\\3.bmp");
	
	// �������
	clearEdge();
	saveBitmap(g_lpOrgFinger, IMGW, IMGH, "c:\\4.bmp");
	
	// ϸ��
	thin();
	// ϸ��ͼ��ȥ�̰���ë��
	thinClear(12);
	saveBitmap(g_lpOrgFinger, IMGW, IMGH, "c:\\5.bmp");

	// ��ȡָ������
	re = getMinutia();
	int re2 = getSingular(g_lpOrient, IMGW, IMGH, -1);
	//int re3 = getSingular(g_lpOrient, IMGW, IMGH, 1);
	
//	if(re != 0 && re2 != 0)
		return VF_PROCESS_ERR;
	
	return VF_OK;
}


//
//	smooth: ��ͼ����о�ֵƽ���˲�,��������
//
void smooth(uint8 *lpInBuffer, uint8 *lpOutBuffer, sint32 r, sint32 d)
{
	////////////////////////////////////////////////////////////////////////
	//	lpInBuffer: [in] Ҫƽ����ͼ�����ݻ�����
	//	lpOutBuffer: [out] ƽ�����ͼ�����ݻ�����
	//	r: [in] ƽ���˲����뾶
	//	d: [in] ƽ���˲�������
	////////////////////////////////////////////////////////////////////////
	sint32	x, y, i, j;
	uint8	*lpSrc;
	uint8	*lpRst;
	sint32	sum, num;
	
	if(d == 2)
	{
		for(y = 0; y < IMGH; y++)
		{
			for(x = 0; x < IMGW; x++)
			{
				lpSrc = lpInBuffer + y*IMGW + x;//ԭͼ��ָ��
				lpRst = lpOutBuffer + y*IMGW + x;//Ŀ��ͼ��ָ��
				sum = 0;						//���غ�����
				num = 0;						//������
				for(i = -r; i <= r; i+=2)		//������Ϊ��x��y���߳�Ϊ2r�ľ���������
				{
					if(i+y<0 || i+y>=IMGH)
						continue;
					for(j = -r; j <= r; j+=2)
					{
						if(j+x<0 || j+x>=IMGW)
							continue;
						sum += *(lpSrc + i*IMGW + j);//�����غ�
						num++;						//�����ظ���
					}
				}
				*lpRst = (uint8)(sum/num);			//��ƽ��
			}
		}
	}
	else
	{
		for(y = 0; y < IMGH; y++)
		{
			for(x = 0; x < IMGW; x++)
			{
				lpSrc = lpInBuffer + y*IMGW + x;
				lpRst = lpOutBuffer + y*IMGW + x;
				sum = 0; 
				num = 0;
				for(i = -r; i <= r; i++)
				{
					if(i+y<0 || i+y>=IMGH)
						continue;
					for(j = -r; j <= r; j++)
					{
						if(j+x<0 || j+x>=IMGW)
							continue;
						sum += *(lpSrc + i*IMGW + j);
						num++;
					}
				}
				*lpRst = (uint8)(sum/num);
			}
		}
	}
	
}


//
// ��ȡָ��ͼ���Ƶ��Ϣ�������㷽��
//
void zoomout()
{
	sint32	x, y;
	uint8	*lpSrc;
	uint8	*lpRst;
	sint32	sum;
	sint32	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};
	
	// ��Ե����
	for(y = 0; y < IMGH; y+=2)
	{
		lpSrc = g_lpOrgFinger + y*IMGW;
		lpRst = g_lpTemp + (y/2)*(IMGW/2);
		*lpRst = *lpSrc;
		lpSrc = g_lpOrgFinger + y*IMGW + IMGW - 1;
		lpRst = g_lpTemp + (y/2)*(IMGW/2) + (IMGW/2 - 1);
		*lpRst = *lpSrc;
	}
	for(x = 0; x < IMGW; x+=2)
	{
		lpSrc = g_lpOrgFinger + x;
		lpRst = g_lpTemp + x/2;
		*lpRst = *lpSrc;
		lpSrc = g_lpOrgFinger + (IMGH-1)*IMGW + x;
		lpRst = g_lpTemp + (IMGH/2-1)*(IMGW/2) + x/2;
		*lpRst = *lpSrc;
	}
	
	// �Ǳ�Ե�����ø�˹ģ����ȡ��Ƶ��Ϣ
	for(y = 2; y < IMGH-2; y+=2)
	{
		for(x = 2; x < IMGW-2; x+=2)
		{
			lpSrc = g_lpOrgFinger + y*IMGW + x;
			lpRst = g_lpTemp + (y/2)*(IMGW/2) + x/2;
			sum = *lpSrc*4 + *(lpSrc + SiteD8[0]) +
				*(lpSrc + SiteD8[1])*2 + *(lpSrc + SiteD8[2]) +
				*(lpSrc + SiteD8[3])*2 + *(lpSrc + SiteD8[4]) +
				*(lpSrc + SiteD8[5])*2 + *(lpSrc + SiteD8[6]) +
				*(lpSrc + SiteD8[7])*2;
			sum = sum>>4;
			*lpRst = (uint8)sum;
		}
	}	
}



//
//	getOrientMap: ��ָ�Ʒ���
//
void getOrientMap(sint32  r)
{
	/////////////////////////////////////////////////////////////////////////
	//	r: [in] ��ʽ�������߷���Ĵ��ڰ뾶�������(8 - 16)
	/////////////////////////////////////////////////////////////////////////
	sint32	x, y, i, j;
	sint32  vx, vy, lvx, lvy;
	uint8   *lpSrc = NULL;
	uint8   *lpDiv = NULL;
	uint8   *lpOri = NULL;
	sint32	angle, grad, gradSum, num;
	double  fAngle;
	
	for(y = 0; y < IMGH/2; y++)
	{
		for(x = 0; x < IMGW/2; x++)
		{
			lpDiv = g_lpDivide + 2*y*IMGW + 2*x;
			lpOri = g_lpOrient + 2*y*IMGW + 2*x;
			lvx = 0;
			lvy = 0;
			num = 0;
			gradSum = 0;
			for(i = -r; i <= r; i++)	// Ϊ����ٶȣ�����Ϊ2
			{
				if(y+i<1 || y+i>=IMGH/2-1)
					continue;
				for(j = -r; j <= r; j++)	// Ϊ����ٶȣ�����Ϊ2
				{
					if(x+j<1 || x+j>=IMGW/2-1)
						continue;
					lpSrc = g_lpTemp + (y+i)*(IMGW/2) + x+j;
					
					vx = *(lpSrc + IMGW/2 + 1) - *(lpSrc + IMGW/2 - 1) +
						*(lpSrc + 1)*2 - *(lpSrc - 1)*2 +
						*(lpSrc - IMGW/2 + 1) - *(lpSrc - IMGW/2 - 1);
					
					vy = *(lpSrc + IMGW/2 - 1) - *(lpSrc - IMGW/2 - 1) +
						*(lpSrc + IMGW/2)*2 - *(lpSrc - IMGW/2)*2 +
						*(lpSrc + IMGW/2 + 1) - *(lpSrc - IMGW/2 + 1);
					
					gradSum += (abs(vx)+abs(vy));
					lvx += vx * vy * 2;
					lvy += vx*vx - vy*vy;
					num++;
				}
			}
			if(num == 0)
				num = 1;
			// ���ֵ�����浽g_lpDivide�У����ڷָ�ǰ������
			grad = gradSum/num;
			
			if(grad > 255)
				grad = 255;
			*lpDiv = (uint8)grad;
			*(lpDiv + 1) = (uint8)grad;
			*(lpDiv + IMGW) = (uint8)grad;
			*(lpDiv + IMGW + 1) = (uint8)grad;
			
			// �󻡶�
			fAngle = atan2(lvy, lvx);
			// �任��(0 - 2*pi)
			if(fAngle < 0)
			{
				fAngle += 2*PI;
			}
			// �����߽Ƕ�
			fAngle = (fAngle*EPI*0.5 + 0.5);
			angle = (sint32)fAngle;
			
			// ��Ϊ���� sobel���ӣ����ԽǶ�ƫת��135�ȣ�����Ҫ��ת��õĽǶ�
			angle -= 135;
			// �Ƕȱ任����0-180��
			if(angle <= 0)
			{
				angle += 180;
			}
			angle = 180-angle;
			// �������߽Ƕ�
			*lpOri = (uint8)angle;
			*(lpOri + 1) = (uint8)angle;
			*(lpOri + IMGW) = (uint8)angle;
			*(lpOri + IMGW + 1) = (uint8)angle;
			
		}
	}
	
}


//
//	saveBitmap: ��ͼ�����ݱ����BMP�ļ�
//
sint32	saveBitmap(uint8 *lpBitmap, sint32 Width, sint32 Height, sint8 *lpszFileName)
{
	/////////////////////////////////////////////////////////////////////
	//	lpBitmap: [in] Ҫ�����ͼ������
	//  Width: [in] ͼ����
	//	Height: [in] ͼ��߶�
	//  lpszFileName: [in] BMP�ļ���
	/////////////////////////////////////////////////////////////////////
   	FILE               *fp = NULL;
	uint8				palatte[1024];
	BITMAPFILEHEADER   bf;
	BITMAPINFOHEADER   bi;
	sint32             lineBytes = 0;
	sint32             i;
	
	lineBytes = (sint32)WIDTHBYTES(Width);
	
	//����bmp�ļ�ͷ��Ϣ
	bf.bfType=19778;
	bf.bfSize = lineBytes*Height+1078;
	bf.bfReserved1=0;
	bf.bfReserved2=0;
	bf.bfOffBits=1078;
	bi.biSize=40;
	bi.biWidth = Width;
	bi.biHeight = Height;
	bi.biPlanes=1;
	bi.biBitCount=8;
	bi.biCompression=0;
	bi.biSizeImage= lineBytes*Height;
	bi.biXPelsPerMeter=0;
	bi.biYPelsPerMeter=0;
	bi.biClrUsed=256;
	
	bi.biClrImportant=0;
	//�����ɫ������
	for(i=0;i<256;i++){
		palatte[i*4]=(unsigned char)i;
		palatte[i*4+1]=(unsigned char)i;
		palatte[i*4+2]=(unsigned char)i;
		palatte[i*4+3]=0;
	}	
	// �����ļ�
	fp = fopen(lpszFileName, "wb");
	if(fp == NULL)
	{
		return 1;
	}
	// д�ļ�ͷ
	fwrite((char *)&bf,sizeof(BITMAPFILEHEADER), 1, fp); 
	// дBMPͼ����Ϣͷ
	fwrite((char *)&bi, sizeof(BITMAPINFOHEADER), 1, fp);
	// д��ɫ��
	fwrite((char *)palatte, sizeof(palatte), 1, fp);
	// ����д��ÿ��дWIDTHBYTES(Width)���ֽ�
	for(i = 0; i < Height; i++)
		fwrite((char *)(lpBitmap+i*Width), lineBytes, 1, fp);
	
	fclose(fp);	
	
	return 0;
}


//
// divide: ���ݷ��򳡷�ֵ���ָ�ָ��ͼ��ǰ������
//
sint32 divide(sint32  r, sint32 threshold)
{
	///////////////////////////////////////////////////////////////////////
	//	r: [in] �Է�ֵͼ��߶�ƽ���˲����˲����뾶
	//  threshold: [in] �ָ����ֵ
	///////////////////////////////////////////////////////////////////////
	sint32	x, y;
	sint32  num = 0;
	
	// �Է��򳡷�ֵͼ����и߶�ƽ���˲�
	smooth(g_lpDivide, g_lpTemp, r, 2);
	
	// ͼ���Ե������Ϊ����
	for(y = 0; y < IMGH; y++)
	{
		*(g_lpDivide + y*IMGW) = 255;
		*(g_lpDivide + y*IMGW + IMGW - 1) = 255;
	}
	for(x = 0; x < IMGW; x++)
	{
		*(g_lpDivide + x) = 255;
		*(g_lpDivide + (IMGH-1)*IMGW + x) = 255;
	}
	
	for(y = 1; y < IMGH-1; y++)
	{
		for(x = 1; x < IMGW-1; x++)
		{
			// ���ݷ�ֵ����ֵ��С�ж��Ƿ�Ϊ��������
			if(*(g_lpTemp + y*IMGW + x) < threshold)
			{
				*(g_lpDivide + y*IMGW + x) = 255;
			}
			else
			{
				*(g_lpDivide + y*IMGW + x) = 0;
				num++;
			}
		}
	}
	
	// ���ǰ���������С���������ʮ��֮һ�����ʾǰ������̫С�����ش���
	if(num < IMGSIZE/10)
		return 1;
	else
		return 0;
	
}


//
//	orientFilter: ��ָ��ͼ����ж����˲���ǿ
//
void orientFilter()
{
	sint32 x, y;
	sint32 i;
	sint32 d = 0;
	sint32 sum = 0;
	// ���߷����Ͻ���ƽ���˲���ƽ���˲���
	sint32 Hw[7] = {1, 1, 1, 1, 1, 1, 1};
	// ���߷���Ĵ�ֱ�����Ͻ������˲������˲���
	sint32 Vw[7] = {-3, -1, 3, 9, 3, -1, -3};
	sint32 hsum = 0;
	sint32 vsum = 0;
	sint32 temp = 0;
	uint8  *lpSrc = NULL;
	uint8  *lpDir = NULL;

	// ���߷����Ͻ���ƽ���˲�
	temp = 0;
	for(y = 0; y < IMGH; y++)
	{
		for(x = 0; x < IMGW; x++)
		{
			lpDir = g_lpOrient + temp + x;
			lpSrc = g_lpOrgFinger + temp + x;
			// ���߷��������
			d = DIndex(*lpDir);
			sum = 0;
			hsum = 0;
			for(i = 0; i < 7; i++)
			{
				if(y+g_DSite[d][i][1] < 0 || y+g_DSite[d][i][1] >= IMGH ||
				   x+g_DSite[d][i][0] < 0 || x+g_DSite[d][i][0] >= IMGW)
				{
					continue;
				}
				sum += Hw[i]*(*(lpSrc + g_DSite[d][i][1]*IMGW + g_DSite[d][i][0]));
				hsum += Hw[i];
			}
			if(hsum != 0)
			{
				*(g_lpTemp + temp + x) = (uint8)(sum/hsum);
			}
			else
			{
				*(g_lpTemp + temp + x) = 255;
			}
		}
		temp += IMGW;
	}

	// ���߷���Ĵ�ֱ�����Ͻ������˲�
	temp = 0;
	for(y = 0; y < IMGH; y++)
	{
		for(x = 0; x < IMGW; x++)
		{
			lpDir = g_lpOrient + temp + x;
			lpSrc = g_lpTemp + temp + x;
			
			// ���߷���Ĵ�ֱ���������
			d = (DIndex(*lpDir)+6) % 12;

			sum = 0;
			vsum = 0;
			for(i = 0; i < 7; i++)
			{
				if(y+g_DSite[d][i][1] < 0 || y+g_DSite[d][i][1] >= IMGH ||
				   x+g_DSite[d][i][0] < 0 || x+g_DSite[d][i][0] >= IMGW)
				{
					continue;
				}
				sum += Vw[i]*(*(lpSrc + g_DSite[d][i][1]*IMGW + g_DSite[d][i][0]));
				vsum += Vw[i];
			}
			if(vsum > 0)
			{
				sum /= vsum;
				if(sum > 255)
				{
					*(g_lpOrgFinger + temp + x) = 255;
				}
				else if(sum < 0)
				{
					*(g_lpOrgFinger + temp + x) = 0;
				}
				else
				{
					*(g_lpOrgFinger + temp + x) = (uint8)sum;
				}
			}
			else
			{
				*(g_lpOrgFinger + temp + x) = 255;
			}
		}
		temp += IMGW;
	}

}


//
//	DIndex: �ԽǶȽ����������õ�������������(0-12)
//
sint32  DIndex(sint32 angle)
{
	/////////////////////////////////////////////////////////////////////////
	//	angle: [in] �Ƕ� ��0 - 180��
	/////////////////////////////////////////////////////////////////////////
	if(angle >= 173 || angle < 8)
	{
		return 0;
	}
	else
	{
		return ((angle-8)/15 + 1);
	}
}


//
//	binary: ��ָ��ͼ����ж�ֵ��
//
sint32 binary()
{
	sint32 x, y;
	sint32 i;
	sint32 d = 0;
	sint32 sum = 0;
	// ���߷����ϵ�7�����Ȩֵ
	sint32 Hw[7] = {2, 2, 3, 4, 3, 2, 2};
	// ���߷���Ĵ�ֱ�����ϵ�7�����Ȩֵ
	sint32 Vw[7] = {1, 1, 1, 1, 1, 1, 1};
	sint32 hsum = 0;	// ���߷����ϵ�7����ļ�Ȩ��
	sint32 vsum = 0;	// ���߷���Ĵ�ֱ�����ϵ�7����ļ�Ȩ��
	sint32 Hv = 0;		// ���߷����ϵ�7����ļ�Ȩƽ��ֵ
	sint32 Vv = 0;		// ���߷���Ĵ�ֱ�����ϵ�7����ļ�Ȩƽ��ֵ
	sint32 temp = 0;
	uint8  *lpSrc = NULL;	// ָ��ͼ�����ص�ָ��
	uint8  *lpDir = NULL;	// ���߷���ָ��

	temp = 0;
	for(y = 0; y < IMGH; y++)
	{
		for(x = 0; x < IMGW; x++)
		{
			lpDir = g_lpOrient + temp + x;
			lpSrc = g_lpOrgFinger + temp + x;
			
			// ����õ�ǳ��ڣ�������ʱ���������øõ�Ϊ�ڵ㣬ֵΪ0
			if(*lpSrc < 4)
			{
				*(g_lpTemp + temp + x) = 0;
				continue;
			}
			// ���㷽������������Ϊ12������
			d = DIndex(*lpDir);
			
			// ���㵱ǰ�������߷����ϵļ�Ȩƽ��ֵ
			sum = 0;
			hsum = 0;
			for(i = 0; i < 7; i++)
			{
				// �����Ƿ�Խ��
				if(y+g_DSite[d][i][1] < 0 || y+g_DSite[d][i][1] >= IMGH ||
				   x+g_DSite[d][i][0] < 0 || x+g_DSite[d][i][0] >= IMGW)
				{
					continue;
				}
				sum += Hw[i]*(*(lpSrc + g_DSite[d][i][1]*IMGW + g_DSite[d][i][0]));
				hsum += Hw[i];
			}
			if(hsum != 0)
			{
				Hv = sum/hsum;
			}
			else
			{
				Hv = 255;
			}

			// ���߷���Ĵ�ֱ���������
			d = (d+6)%12;

			// ���㵱ǰ�������߷���Ĵ�ֱ�����ϵļ�Ȩƽ��ֵ
			sum = 0;
			vsum = 0;
			for(i = 0; i < 7; i++)
			{
				if(y+g_DSite[d][i][1] < 0 || y+g_DSite[d][i][1] >= IMGH ||
				   x+g_DSite[d][i][0] < 0 || x+g_DSite[d][i][0] >= IMGW)
				{
					continue;
				}
				sum += Vw[i]*(*(lpSrc + g_DSite[d][i][1]*IMGW + g_DSite[d][i][0]));
				vsum += Vw[i];
			}
			if(vsum != 0)
			{
				Vv = sum/vsum;
			}
			else
			{
				Vv = 255;
			}

			
			if(Hv < Vv)
			{
				// ���߷����ϼ�Ȩƽ��ֵ��С���õ�ǰ��Ϊ�ڵ�
				*(g_lpTemp + temp + x) = 0;
			}
			else
			{
				// ���߷����ϼ�Ȩƽ��ֵ�ϴ����õ�ǰ��Ϊ�׵�
				*(g_lpTemp + temp + x) = 255;
			}
		}
		temp += IMGW;
	}

	// ����ʱ�����������ݿ�����ԭʼͼ�����ݻ�����
	memcpy((void *)g_lpOrgFinger, (void *)g_lpTemp, IMGSIZE);
	

	return 0;

}


//
//	binary: �Զ�ֵ��ָ��ͼ�����ȥ����
//
void binaryClear()
{
	sint32 x, y, i;
	sint32 num = 0;
	bool   bWorking;
	sint32 temp;
	uint8  *lpSrc = NULL;
	bWorking = true;
	sint32  n = 0;
	// ĳ����Χ8����ĵ�ַƫ��
	sint32	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};
	
	// ��ʼ����ʱ������
	memset((void *)g_lpTemp, 255, IMGSIZE);
	
	// ѭ������ֱ��������ϻ��ߴ����������8
	while(bWorking && n < 8)
	{
		bWorking = false;
		n++;
		temp = IMGW;
		for(y = 1; y < IMGH-1; y++)
		{
			for(x = 1; x < IMGW-1; x++)
			{
				// �����ĵ㲻����
				if(*(g_lpDivide + temp + x) == 255)
				{
					continue;
				}
				// ͳ�Ƶ�ǰ����Χ������ͬ���͵�ĸ���
				num = 0;
				lpSrc = g_lpOrgFinger + temp + x;
				for(i = 0; i < 8; i++)
				{
					if(*(lpSrc+SiteD8[i]) == *lpSrc)
					{
						num++;
					}
				}
				// ��ͬ�����С�ڶ���ı䵱ǰ������
				if(num < 2)
				{
					*(g_lpTemp+temp+x) = 255 - *lpSrc;
					bWorking = true;
				}
				else
				{
					*(g_lpTemp+temp+x) = *lpSrc;
				}
				
			}
			temp += IMGW;
		}
		// ��������������ԭʼͼ�񻺳���
		memcpy((void *)g_lpOrgFinger, (void *)g_lpTemp, IMGSIZE);
	}
	
}


//
//	clearEdge: �������
//
void clearEdge()
{
	sint32	x, y;
	sint32  temp;
	temp = 0;
	
	for(y = 0; y < IMGH; y++)
	{
		for(x = 0; x < IMGW; x++)
		{	
			// ����Ǳ����������øõ�Ϊ�׵�
			if(*(g_lpDivide + temp + x) == 255)
			{
				*(g_lpOrgFinger + temp + x) = 255;
				*(g_lpOrient + temp + x) = 255;
			}
		}
		temp += IMGW;
	}
}


//
//	thin : �Ե�ǰָ��ͼ�����ϸ������
//
sint32 thin()
{
	imageThin(g_lpOrgFinger, IMGW, IMGH);
	
	return 0;
}

//
//	thinClear: ���ϸ��ͼ���ж̰���ë��
//
sint32  thinClear(sint32  len)
{
	/////////////////////////////////////////////////////////////////
	//	len: [in] �̰���ë�̵���󳤶�
	/////////////////////////////////////////////////////////////////
	sint32	x, y;
	sint32  i, n, num;
	sint32  temp;
	uint8   *Line[25];
	uint8   *lpNow = NULL;
	uint8   *lpLast = NULL;
	uint8   *lpNext = NULL;
	uint8   *tempPtr[8];
	sint32	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};
	
	temp = 0;
	for(y = 0; y < IMGH; y++)
	{
		for(x = 0; x < IMGW; x++)
		{
			lpNow = g_lpOrgFinger + temp + x;
			if(*lpNow != 0)
			{
				continue;
			}
			Line[0] = lpNow;
			// ͳ�Ƶ�ǰ�����Χ�ڵ����
			n = 0;
			for(i = 0; i < 8; i++)
			{
				lpNext = lpNow + SiteD8[i];
				if(*lpNext == 0)
				{
					tempPtr[n] = lpNext;
					n++;
				}
			}
			// �ڵ����Ϊ�㣬��ʾ��ǰ���ǹµ㣬��Ϊ��ɫ
			if(n == 0)
			{
				*lpNow = 255;
				continue;
			}
			// �ڵ����Ϊ1����ʾΪ�˵�
			else if(n == 1)
			{
				num = 0;
				lpLast = lpNow;
				lpNow = tempPtr[0];
				// �����߸���len����
				for(i = 0; i < len; i++)
				{
					// ����������������ѭ��
					if(IsFork(lpNow))
					{
						break;
					}
					num++;
					Line[num] = lpNow;
					if(GetNext(lpNow, lpLast, &lpNext) == 0)
					{
						lpLast = lpNow;
						lpNow = lpNext;
					}
					else // ��������쳣����ѭ��
					{
						break;
					}
					
				}
				
				// ���߽϶̣���ʾΪ�̰�������ë��
				if(num < len)
				{
					for(i = 0; i <= num; i++)
					{
						*Line[i] = 255;
					}
				}
				
			}
		}
		temp += IMGW;
	}
	
	return 0;
}


//
//	imageThin: ͼ��ϸ������
//
sint32 imageThin(uint8 *lpBits, sint32 Width, sint32 Height)
{
/////////////////////////////////////////////////////////////////
//	lpBits: [in, out] Ҫϸ����ͼ�����ݻ����� 
//	Width: [in] Ҫϸ����ͼ����
//	Height: [in] Ҫϸ����ͼ��߶�
/////////////////////////////////////////////////////////////////
	uint8  erasetable[256]={
			0,0,1,1,0,0,1,1,             1,1,0,1,1,1,0,1,
			1,1,0,0,1,1,1,1,             0,0,0,0,0,0,0,1,
			0,0,1,1,0,0,1,1,             1,1,0,1,1,1,0,1,
			1,1,0,0,1,1,1,1,             0,0,0,0,0,0,0,1,
			1,1,0,0,1,1,0,0,             0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,             0,0,0,0,0,0,0,0,
			1,1,0,0,1,1,0,0,             1,1,0,1,1,1,0,1,
			0,0,0,0,0,0,0,0,             0,0,0,0,0,0,0,0,
			0,0,1,1,0,0,1,1,             1,1,0,1,1,1,0,1,
			1,1,0,0,1,1,1,1,             0,0,0,0,0,0,0,1,
			0,0,1,1,0,0,1,1,             1,1,0,1,1,1,0,1,
			1,1,0,0,1,1,1,1,             0,0,0,0,0,0,0,0,
			1,1,0,0,1,1,0,0,             0,0,0,0,0,0,0,0,
			1,1,0,0,1,1,1,1,             0,0,0,0,0,0,0,0,
			1,1,0,0,1,1,0,0,             1,1,0,1,1,1,0,0,
			1,1,0,0,1,1,1,0,             1,1,0,0,1,0,0,0
	};
	sint32		x,y;
	sint32      num;
	BOOL        Finished;
	uint8       nw,n,ne,w,e,sw,s,se;
	uint8       *lpPtr = NULL;
	uint8       *lpTempPtr = NULL;	

	memcpy((void *)g_lpTemp, (void *)lpBits, Width*Height);
	
	//������־�óɼ�
	Finished=FALSE;
	while(!Finished){ //��û�н���
              //������־�óɼ�
            Finished=TRUE;
       //�Ƚ���ˮƽ�����ϸ��
              for (y=1;y<Height-1;y++)
			  { //ע��Ϊ��ֹԽ�磬y�ķ�Χ��1���߶�-2
                     //lpPtrָ��ԭͼ���ݣ�lpTempPtrָ����ͼ����

                     lpPtr=(uint8 *)lpBits+y*Width;
                     lpTempPtr=(uint8 *)g_lpTemp+y*Width;

                     x=1; //ע��Ϊ��ֹԽ�磬x�ķ�Χ��1�����-2

                     while(x<Width-1)
					 {
                            if(*(lpPtr+x)==0)
							{ //�Ǻڵ��������
                                   w = *(lpPtr+x-1);  //���ڵ�
                                   e = *(lpPtr+x+1);  //���ڵ�

                                   if( (w==255)|| (e==255)){ 

									//������������ھ���������һ���ǰ׵�Ŵ���

                                          nw=*(lpPtr+x+Width-1); //�����ڵ�

                                          n=*(lpPtr+x+Width); //���ڵ�

                                          ne=*(lpPtr+x+Width+1); //�����ڵ�

                                          sw=*(lpPtr+x-Width-1); //�����ڵ�

                                          s=*(lpPtr+x-Width); //���ڵ�

                                          se=*(lpPtr+x-Width+1); //�����ڵ�

                                          //��������

                            num=nw/255+n/255*2+ne/255*4+w/255*8+e/255*16+
								sw/255*32+s/255*64+se/255*128;

                                          if(erasetable[num]==1){ //���������ɾ��

												//��ԭͼ�������н��úڵ�ɾ��

                                                 *(lpPtr+x)=255; 

													//���ͼ�иúڵ�Ҳɾ��

                                                 *(lpTempPtr+x)=255; 

                                                 Finished=FALSE; //�иĶ���������־�óɼ�

                                                 x++; //ˮƽ��������һ������

                                          }

                                   }

                            }

                            x++; //ɨ����һ������

                     }

              }

       //�ٽ��д�ֱ�����ϸ��

              for (x=1;x<Width-1;x++){ //ע��Ϊ��ֹԽ�磬x�ķ�Χ��1�����-2

                     y=1; //ע��Ϊ��ֹԽ�磬y�ķ�Χ��1���߶�-2

                     while(y<Height-1){

                            lpPtr=lpBits+y*Width;

                            lpTempPtr=g_lpTemp+y*Width;

                            if(*(lpPtr+x)==0){ //�Ǻڵ��������

                                   n=*(lpPtr+x+Width);

                                   s=*(lpPtr+x-Width);

                                   if( (n==255)|| (s==255)){

//������������ھ���������һ���ǰ׵�Ŵ���

                                          nw=*(lpPtr+x+Width-1);

                                          ne=*(lpPtr+x+Width+1);

                                          w=*(lpPtr+x-1);

                                          e=*(lpPtr+x+1);

                                          sw=*(lpPtr+x-Width-1);

                                          se=*(lpPtr+x-Width+1);

                                          //��������

										num=nw/255+n/255*2+ne/255*4+w/255*8+e/255*16+

											sw/255*32+s/255*64+se/255*128;

                                          if(erasetable[num]==1){ //���������ɾ��

//��ԭͼ�������н��úڵ�ɾ��

                                                 *(lpPtr+x)=255; 

//���ͼ�иúڵ�Ҳɾ��

                                                 *(lpTempPtr+x)=255; 

                                                 Finished=FALSE; //�иĶ���������־�óɼ�

                                                 y++;//��ֱ��������һ������

                                          }

                                   }

                            }

                            y++; //ɨ����һ������

                     }

              } 

	}
	
	memcpy((void *)lpBits, (void *)g_lpTemp, Width*Height);

	return 0;
}


//
//	IsFork: �ж�ĳ���Ƿ�Ϊ���
//
bool   IsFork(uint8 *lpNow)
{
	/////////////////////////////////////////////////////////////////////////////
	//	lpNow : [in] ��ǰ��ĵ�ַ
	/////////////////////////////////////////////////////////////////////////////
	sint32 i, sum;
	// ĳ����Χ8����ĵ�ַƫ��
	sint32	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};
	
	// 8������������������Ĳ�ľ���ֵ�ĺ����Ϊ6*255��Ϊ���
	sum = 0;
	for(i = 0; i < 8; i++)
	{
		sum += abs(*(lpNow + SiteD8[(i+1)%8]) - *(lpNow + SiteD8[i]));
	}
	if(sum == 255*6)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//
//	GetNext: �������ϸ��ݵ�ǰ���ǰ�����ַ�ҵ���һ����ĵ�ַ
//
sint32  GetNext(uint8 *lpNow, uint8 *lpLast, uint8 **lppNext)
{
	/////////////////////////////////////////////////////////////////////////////
	//	lpNow : [in] ��ǰ��ĵ�ַ
	//	lpLast : [in] ǰ����ĵ�ַ
	//	lppNext : [out] ��һ����ĵ�ַ��ָ��
	/////////////////////////////////////////////////////////////////////////////
	sint32  i;
	uint8   *lpSrc = NULL;
	sint32  n;
	uint8   *Temp[8];	// ĳ����Χ8����ĵ�ַ
	sint32	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};
	
	*lppNext = NULL;
	
	// �ҵ���ǰ����Χ���Ǻڵ㲢�Ҳ���ǰ����ĺڵ㣬���浽����
	n = 0;
	for(i = 0; i < 8; i++)
	{
		lpSrc = lpNow + SiteD8[i];
		if(*lpSrc == 0 && lpSrc != lpLast)
		{
			Temp[n] = lpSrc;
			n++;
		}
	}
	
	if(n == 0)	// û���ҵ������ڵ����ʾû�к�̵�
	{
		*lppNext = NULL;
		return 1;
	}
	else if(n == 1) // �ҵ���һ��������Ϊ��̵�
	{
		*lppNext = Temp[0];
		return 0;
	}
	else // �ҵ�������򷵻ش���
	{
		*lppNext = NULL;
		return 1;
	}
	
	return 0;
}


//
//	getMinutia: ��ȡָ������
//
sint32  getMinutia()
{
	sint32	x, y, i, j, k, n;
	sint32  temp;
	sint32  sum;
	uint8   *lpNow;
	uint8   *lpDis;
	sint32  r = 16;		// ���������㿿��ǰ����Ե���������
	double  d, d1, d2, d0, a;
	sint32  dGate = 16;
	bool    flag;
	sint32  x11, y11;
	sint32  x21, y21;
	sint32  angle1, angle2, angle3, angle4;
	uint8   *tempPtr[8];
	uint8   *disPtr[3];
	uint8   *lpOri;
	sint32  tempForkNum;
	sint32  tempEndNum;
	sint32  ForkNum;
	sint32  EndNum;
	// ��ʱ����������
	MINUTIA     tempFork[MAXRAWMINUTIANUM];
	MINUTIA     tempEnd[MAXRAWMINUTIANUM];
	MINUTIA     ForkArr[MAXRAWMINUTIANUM];
	MINUTIA     EndArr[MAXRAWMINUTIANUM];
	// ĳ��ΪԲ�ģ��뾶Ϊ5��Բ�����е�ĵ�ַƫ��
	sint32	SiteR5[28] = {
		-5, IMGW-5, 2*IMGW-5, 3*IMGW-4, 4*IMGW-3, 5*IMGW-2, 5*IMGW-1, 5*IMGW,
		5*IMGW+1, 5*IMGW+2, 4*IMGW+3, 3*IMGW+4, 2*IMGW+5, IMGW+5, 5, -IMGW+5,
		-2*IMGW+5, -3*IMGW+4, -4*IMGW+3, -5*IMGW+2, -5*IMGW+1, -5*IMGW,
		-5*IMGW-1, -5*IMGW-2, -4*IMGW-3, -3*IMGW-4, -2*IMGW-5, -IMGW-5
	}; 
	// ĳ����Χ8����ĵ�ַƫ��
	sint32	SiteU8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};

	// ��ʼ����ʱ����������
	memset((void *)&tempFork[0], 0, MAXRAWMINUTIANUM*sizeof(MINUTIA));
	memset((void *)&tempEnd[0], 0, MAXRAWMINUTIANUM*sizeof(MINUTIA));
	memset((void *)&ForkArr[0], 0, MAXRAWMINUTIANUM*sizeof(MINUTIA));
	memset((void *)&EndArr[0], 0, MAXRAWMINUTIANUM*sizeof(MINUTIA));


	bool    bGood = false;	// �����ֵ�Ƿ�����ã�ʹ��������Ŀ������Χ��
	sint32  densD = 7;	// �������������������ľ���
	sint32  loopnum;	// ѭ������
	sint32  trilen = 16;
	// ��һ��Ѱ�����ж˵�Ͳ��	
	ForkNum = 0; 
	EndNum = 0; 
	temp = 17 * IMGW;
	for(y = 17; y < IMGH-17; y++)
	{
		for(x = 17; x < IMGW-17; x++)
		{
			lpNow = g_lpOrgFinger + temp + x;//ԭͼ��ָ��
			lpOri = g_lpOrient + y*IMGW + x;//ԭͼ����ָ��
			// ���Ǻڵ��򲻿���
			if(*lpNow != 0)
			{
				continue;
			}
			// �ǲ��
			if(IsFork(lpNow))
			{
				// ����Ƿ񿿽���Ե, ������Ե�򲻿���
				flag = true;
				for(i = -r; i <= r && flag; i++)
				{
					for(j = -r; j <= r && flag; j++)
					{
						if(y+i<0 || y+i>=IMGH || x+j<0 || x+j>=IMGW)
						{
							continue;
						}

						if(*(g_lpOrient + temp + i*IMGW + x + j) == 255)
						{
							flag = false;
							break;
						}
					}
				}
				// ���õ���Χ�����Ƿ�仯���ң��仯�����򲻿��ǣ���Ϊ�ܿ�������ٵ�
				sum = 0;
				for(i = 0; i < 28; i++)
				{
					sum += GetJiajiao(*(lpOri+SiteR5[(i+1)%28]), *(lpOri+SiteR5[i]));//������������ļн�
				}
				if(sum > 96)
					flag = false;
				
				// �ﵽ����Ҫ�����¼�����õ�
				if(flag)
				{
					ForkArr[ForkNum].x = x;
					ForkArr[ForkNum].y = y;
					ForkNum++;
					// ��������Ѿ��������������Ŀ���򷵻ش���
					if(ForkNum >= MAXRAWMINUTIANUM)
					{
						ForkNum = 0;
						return 1;
					}

				}

			}
			else if(IsEnd(lpNow))	// ����Ƕ˵�
			{
				// ����Ƿ񿿽���Ե, ������Ե�򲻿���
				flag = true;
				for(i = -r; i <= r && flag; i++)
				{
					for(j = -r; j <= r && flag; j++)
					{
						if(y+i<0 || y+i>=IMGH || x+j<0 || x+j>=IMGW)
						{
							continue;
						}

						if(*(g_lpOrient + temp + i*IMGW + x + j) == 255)
						{
							flag = false;
							break;
						}
					}
				}

				// ���õ���Χ�����Ƿ�仯���ң��仯�����򲻿��ǣ���Ϊ�ܿ�������ٵ�
				sum = 0;
				for(i = 0; i < 28; i++)
				{
					sum += GetJiajiao(*(lpOri+SiteR5[(i+1)%28]), *(lpOri+SiteR5[i]));
				}
				if(sum > 96)
					flag = false;

				// �ﵽ����Ҫ�����¼�����õ�
				if(flag)
				{
					EndArr[EndNum].x = x;
					EndArr[EndNum].y = y;
					EndNum++;
					// ��������Ѿ��������������Ŀ���򷵻ش���
					if(EndNum >= MAXRAWMINUTIANUM)
					{
						EndNum = 0;
						return 1;
					}

				}

			}
		}
		temp += IMGW;
	}

	// ��ʼ����ʱ��������׼��ѭ������������������������С���룬ʹ֮����һ�����ʵ�ֵ
	// ��Ϊ���������ǳ��ܼ��ĵط����п����������ܲ�ĵط�
	for(i = 0; i < MAXRAWMINUTIANUM; i++)
	{
		tempEnd[i] = EndArr[i];
	}
	for(i = 0; i < MAXRAWMINUTIANUM; i++)
	{
		tempFork[i] = ForkArr[i];
	}
	tempForkNum = ForkNum;
	tempEndNum = EndNum;

	// ѭ������
	bGood = false;
	loopnum = 0;	// �����Ĵ���
	while(!bGood && loopnum < 32) // ������32��
	{
		loopnum++;	// ����������һ
		// �õ��µ�����������
		for(i = 0; i < MAXRAWMINUTIANUM; i++)
		{
			EndArr[i] = tempEnd[i];
		}
		for(i = 0; i < MAXRAWMINUTIANUM; i++)
		{
			ForkArr[i] = tempFork[i];
		}
		// �µ���������
		ForkNum = tempForkNum;	// ���
		EndNum = tempEndNum;	// �˵�

		// ȥ����ٵĶ˵�
		bGood = true;
		for(i = 0; i < EndNum-1; i++)
		{
			flag = false;
			for(j = i+1; j < EndNum; j++)
			{
				// �������˵�ľ���
				d = sqrt((double)((EndArr[i].x-EndArr[j].x)*(EndArr[i].x-EndArr[j].x) +
					(EndArr[i].y-EndArr[j].y)*(EndArr[i].y-EndArr[j].y)));
				// �����㹻��������һ���˵�
				if(d > dGate && d > densD)
				{
					continue;
				}
				// ����̫С������������Ϊ��0��0��
				if(d <= densD)
				{
					EndArr[j].x = 0;
					EndArr[j].y = 0;
					flag = true;
					continue;
				}
				// ���һ���˵��������ߵĽǶ�
				lpNow = g_lpOrgFinger + EndArr[i].y*IMGW + EndArr[i].x;
				if(GetByDis(lpNow, &lpDis, 8) != 0)
				{
					break;
				}
				angle1 = GetAngle(EndArr[i].x, EndArr[i].y, PX(lpDis), PY(lpDis));
				
				// ��ڶ����˵��������ߵĽǶ�
				lpNow = g_lpOrgFinger + EndArr[j].y*IMGW + EndArr[j].x;
				if(GetByDis(lpNow, &lpDis, 8) != 0)
				{
					continue;
				}
				angle2 = GetAngle(EndArr[j].x, EndArr[j].y, PX(lpDis), PY(lpDis));

				// �������Ƕȼ�ľ���
				angle3 = GetAngleDis(angle1, angle2);
				
				// ��������Ƕȼ����ǣ��������������
				if(angle3 > 270 || angle3 < 90)
				{
					continue;
				}

				// �������˵����ߵĽǶ�
				angle3 = GetAngle(EndArr[i].x, EndArr[i].y, EndArr[j].x, EndArr[j].y);
				// ���һ���˵����������ߵļн�
				angle3 = GetAngleDis(angle1, angle3);
				// ����нǽϴ�������ٵ�
				if(angle3 < 150 || angle3 > 210)
				{
					continue;
				}

				// ��ڶ����˵����������ߵļн�
				angle4 = GetAngle(EndArr[j].x, EndArr[j].y, EndArr[i].x, EndArr[i].y);
				angle4 = GetAngleDis(angle2, angle4);
				
				// ����нǽϴ�������ٵ�
				if(angle4 < 150 || angle4 > 210)
				{
					continue;
				}
				// �����ʾ����������ͬһ�������ϵĶ��Ѵ��������˵�,������ԭ��
				EndArr[j].x = 0;
				EndArr[j].y = 0;
				flag = true;
			}
			if(flag)// ��ʾ����������ͬһ�������ϵĶ��Ѵ��������˵�,������ԭ��
			{
				EndArr[i].x = 0;
				EndArr[i].y = 0;
			}
		}

		// ͳ���µĶ˵���Ŀ
		j = 0;
		for(i = 0; i < EndNum; i++)
		{
			if(EndArr[i].x == 0 || EndArr[i].y == 0)
			{
				continue;
			}
			lpNow = g_lpOrgFinger + EndArr[i].y*IMGW + EndArr[i].x;
			if(GetByDis(lpNow, &lpDis, 8) != 0)
			{
				continue;
			}

			j++;
			// ����˵���Ŀ���ǹ��࣬����Ҫ����densD
			if(j >= MAXMINUTIANUM)
			{
				bGood = false;
				break;
			}

		}
		// ����densD
		if(!bGood)
		{
			densD++; // ��һ
			continue;
		}

		// ���˵�װ�䵽g_Feature
		j = 0;
		for(i = 0; i < EndNum; i++)
		{
			if(EndArr[i].x == 0 || EndArr[i].y == 0)
			{
				continue;
			}
			// �������ҵ���8����
			lpNow = g_lpOrgFinger + EndArr[i].y * IMGW + EndArr[i].x;
			if(GetByDis(lpNow, &lpDis, 8) != 0)
			{
				continue;
			}
			// ��õ���˵����ߵĽǶ�
			angle1 = GetAngle(EndArr[i].x, EndArr[i].y, PX(lpDis), PY(lpDis));
			angle2 = *(g_lpOrient + EndArr[i].y*IMGW + EndArr[i].x);
			angle3 = GetAngleDis(angle1, angle2);
			// ����ɶ۽ǣ����ʾ�������㷽����õ㷽�򳡷����෴
			if(angle3 > 90 && angle3 < 270)
			{
				g_Feature.MinutiaArr[j].Direction = angle2 + 180;
			}
			else // �������ǣ����ʾ�������㷽����õ㷽�򳡷�����ͬ
			{
				g_Feature.MinutiaArr[j].Direction = angle2;
			}

			//�����㻥��ƥ�䣬��������Χ����Է�������
			a = g_Feature.MinutiaArr[j].Direction / EPI + PI/4.0;
			x11 = EndArr[i].x + (sint32)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (sint32)(trilen*sin(a)+ 0.5);
			g_Feature.MinutiaArr[j].Triangle[0] = *(g_lpOrient + y11*IMGW + x11);
			a += PI*2/3.0;
			x11 = EndArr[i].x + (sint32)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (sint32)(trilen*sin(a)+ 0.5);
			g_Feature.MinutiaArr[j].Triangle[1] = *(g_lpOrient + y11*IMGW + x11);
			a += PI*2/3.0;
			x11 = EndArr[i].x + (sint32)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (sint32)(trilen*sin(a)+ 0.5);
			g_Feature.MinutiaArr[j].Triangle[2] = *(g_lpOrient + y11*IMGW + x11);

			g_Feature.MinutiaArr[j].x = EndArr[i].x;	//������
			g_Feature.MinutiaArr[j].y = EndArr[i].y;	//������
			g_Feature.MinutiaArr[j].Type = VF_MINUTIA_END;	//����

			j++;

		}

		EndNum = j;	// �˵���Ŀ


		// ȥ������̫���Ĳ��
		for(i = 0; i < ForkNum-1; i++)
		{
			flag = false;
			for(j = i+1; j < ForkNum; j++)
			{
				d = sqrt((double)((ForkArr[i].x-ForkArr[j].x)*(ForkArr[i].x-ForkArr[j].x) +
					(ForkArr[i].y-ForkArr[j].y)*(ForkArr[i].y-ForkArr[j].y)));

				if(d <= densD-2)
				{
					ForkArr[j].x = 0;
					ForkArr[j].y = 0;
					flag = true;
				}
			}
			if(flag)
			{
				ForkArr[i].x = 0;
				ForkArr[i].y = 0;
			}
		}

		// ͳ���µ������Ĳ��
		j = 0;
		for(i = 0; i < ForkNum; i++)
		{
			if(ForkArr[i].x == 0 || ForkArr[i].y == 0)
			{
				continue;
			}
			lpNow = g_lpOrgFinger + ForkArr[i].y*IMGW + ForkArr[i].x;

			tempPtr[0] = lpNow;
			*tempPtr[0] = 255;
			// ����Ƿ��������
			n = 0;
			for(k = 0; k < 8; k++)
			{
				if(*(lpNow + SiteU8[k]) == 0)
				{
					tempPtr[n+1] = lpNow + SiteU8[k];
					*tempPtr[n+1] = 255;
					n++;
				}
			}
			for(k = 0; k < 4; k++)
			{
				*tempPtr[k] = 0;
			}

			if(n != 3)
			{
				continue;
			}
			j++;
			// �����������������������Ŀ��Ҫ����densD
			if(EndNum + j >= MAXMINUTIANUM)
			{
				densD++;
				bGood = false;
				break;
			}

		}

		if(!bGood)
		{
			continue;
		}

		// װ���㵽g_Feature
		j = 0;
		for(i = 0; i < ForkNum; i++)
		{
			if(ForkArr[i].x == 0 || ForkArr[i].y == 0)
			{
				continue;
			}
			lpNow = g_lpOrgFinger + ForkArr[i].y*IMGW + ForkArr[i].x;

			tempPtr[0] = lpNow;
			*tempPtr[0] = 255;
			n = 0;
			for(k = 0; k < 8; k++)
			{
				if(*(lpNow + SiteU8[k]) == 0)
				{
					tempPtr[n+1] = lpNow + SiteU8[k];
					*tempPtr[n+1] = 255;
					n++;
				}
			}
			if(n != 3)
			{
				continue;
			}
			// �ҵ�����������������Ͼ���Ϊ8�ĵ�
			for(k = 0; k < 3; k++)
			{
				GetByDis(tempPtr[k+1], &disPtr[k], 8);
			}
			for(k = 0; k < 4; k++)
			{
				*tempPtr[k] = 0;
			}
			// ���������������ľ���
			d0 = sqrt((double)((PX(disPtr[1]) - PX(disPtr[2]))*(PX(disPtr[1]) - PX(disPtr[2])) +
				(PY(disPtr[1]) - PY(disPtr[2]))*(PY(disPtr[1]) - PY(disPtr[2]))));
			d1 = sqrt((double)((PX(disPtr[0]) - PX(disPtr[2]))*(PX(disPtr[0]) - PX(disPtr[2])) +
				(PY(disPtr[0]) - PY(disPtr[2]))*(PY(disPtr[0]) - PY(disPtr[2]))));
			d2 = sqrt((double)((PX(disPtr[1]) - PX(disPtr[0]))*(PX(disPtr[1]) - PX(disPtr[0])) +
				(PY(disPtr[1]) - PY(disPtr[0]))*(PY(disPtr[1]) - PY(disPtr[0]))));

			// ������̵ı߶�Ӧ���ǲ��߷���
			if(d0 < d1 && d0 < d2)
			{
				x11 = PX(disPtr[1]);
				y11 = PY(disPtr[1]);
				x21 = PX(disPtr[2]);
				y21 = PY(disPtr[2]);
				lpDis = disPtr[0];
			}
			else if(d1 < d0 && d1 < d2)
			{
				x11 = PX(disPtr[0]);
				y11 = PY(disPtr[0]);
				x21 = PX(disPtr[2]);
				y21 = PY(disPtr[2]);
				lpDis = disPtr[1];
			}
			else
			{
				x11 = PX(disPtr[0]);
				y11 = PY(disPtr[0]);
				x21 = PX(disPtr[1]);
				y21 = PY(disPtr[1]);
				lpDis = disPtr[2];
			}
			// ����̱��е��������ߵķ�����õ㷽�򳡷���ļн�
			angle1 = GetAngle(ForkArr[i].x, ForkArr[i].y, (x11+x21)/2, (y11+y21)/2);
			angle2 = *(g_lpOrient + ForkArr[i].y*IMGW + ForkArr[i].x);
			angle3 = GetAngleDis(angle1, angle2);
			// ����ɶ۽ǣ����ʾ�������㷽����õ㷽�򳡷����෴
			if(angle3 > 90 && angle3 < 270)
			{
				g_Feature.MinutiaArr[EndNum + j].Direction = angle2 + 180;
			}
			else // �������ǣ����ʾ�������㷽����õ㷽�򳡷�����ͬ
			{
				g_Feature.MinutiaArr[EndNum + j].Direction = angle2;
			}
			// ��¼��������Ϊ�������Բ�뾶Ϊtrilen������������������ķ���
			a = g_Feature.MinutiaArr[EndNum + j].Direction / EPI + PI/4.0;
			x11 = EndArr[i].x + (sint32)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (sint32)(trilen*sin(a)+ 0.5);
			g_Feature.MinutiaArr[EndNum + j].Triangle[0] = *(g_lpOrient + y11*IMGW + x11);
			a += PI*2/3.0;
			x11 = EndArr[i].x + (sint32)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (sint32)(trilen*sin(a)+ 0.5);
			g_Feature.MinutiaArr[EndNum + j].Triangle[1] = *(g_lpOrient + y11*IMGW + x11);
			a += PI*2/3.0;
			x11 = EndArr[i].x + (sint32)(trilen*cos(a)+ 0.5);
			y11 = EndArr[i].y + (sint32)(trilen*sin(a)+ 0.5);
			g_Feature.MinutiaArr[EndNum + j].Triangle[2] = *(g_lpOrient + y11*IMGW + x11);

			g_Feature.MinutiaArr[EndNum + j].x = ForkArr[i].x; //������
			g_Feature.MinutiaArr[EndNum + j].y = ForkArr[i].y; //������
			g_Feature.MinutiaArr[EndNum + j].Type = VF_MINUTIA_FORK; // ����


			j++;
		}
		ForkNum = j; // �����Ŀ


	}

	// ���ѭ��32�ζ����ܵõ������������ʾͼ����������ش���
	if(loopnum >= 32)
	{
		EndNum = 0;
		ForkNum = 0;
		return 1;
	}
	// ��������Ŀ���ڶ˵����Ӳ����
	g_Feature.MinutiaNum = (uint8)(EndNum+ForkNum);

	//  ��������Ŀ̫���򷵻ش���
	if(ForkNum + EndNum < 8)
	{
		EndNum = 0;
		ForkNum = 0;
		return 1;
	}

	return 0;
}


//
//	IsEnd: �ж�ĳ���Ƿ�Ϊ�˵�
//
bool   IsEnd(uint8 *lpNow)
{
	/////////////////////////////////////////////////////////////////////////////
	//	lpNow : [in] ��ǰ��ĵ�ַ
	/////////////////////////////////////////////////////////////////////////////
	sint32 i, sum;
	// ĳ����Χ8����ĵ�ַƫ��
	sint32	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};
	
	// 8������������������Ĳ�ľ���ֵ�ĺ����Ϊ2*255��Ϊ�˵�
	sum = 0;
	for(i = 0; i < 8; i++)
	{
		sum += abs(*(lpNow + SiteD8[(i+1)%8]) - *(lpNow + SiteD8[i]));
	}
	if(sum == 255*2)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//
//	GetByDis: ���������ҵ����뵱ǰ�˵�Ϊd�ĵ�ĵ�ַ
//
sint32  GetByDis(uint8 *lpEnd, uint8 **lppPos, sint32 d)
{
/////////////////////////////////////////////////////////////////////////////
//	lpEnd : [in] ��ǰ�˵�ĵ�ַ
//	lppPos : [out] Ŀ�ĵ�ĵ�ַ(����ھ���d���ж˵���߲�㣬�򷵻����ǵ��ڵ�)
//	d : [in] ���루������
/////////////////////////////////////////////////////////////////////////////
	sint32  i, n;
	uint8   *lpNow = NULL;
	uint8   *lpLast = NULL;
	uint8   *lpNext = NULL;
	sint32	SiteD8[8] = {IMGW-1, IMGW, IMGW+1, 1, -IMGW+1, -IMGW, -IMGW-1, -1};

	*lppPos = NULL;
	
	// �ҵ���ǰ����Χ���Ǻڵ㲢�Ҳ���ǰ����ĺڵ㣬���浽����
	n = 0;
	for(i = 0; i < 8; i++)
	{
		if(*(lpEnd + SiteD8[i]) == 0)
		{
			lpNow = lpEnd + SiteD8[i];
			n++;
		}
	}
	if(n != 1) // ��Ŀ�������1�򷵻ش���
	{
		return 1;
	}
	// �ҵ��ĺڵ��ַΪlpNow
	// �˵���Ϊǰ����,��ַ���浽lpLast
	lpLast = lpEnd; 
	// ѭ����d�κ�̵�
	for(i = 0; i < d; i++)
	{
		// ����ǲ��������ѭ��
		if(IsFork(lpNow))
		{
			break;
		}
		// �ɹ��ҵ���һ��������ĵ�ǰ���ǰ����
		if(GetNext(lpNow, lpLast, &lpNext) == 0)
		{
			lpLast = lpNow;
			lpNow = lpNext;
		}
		else
		{
			break;
		}

	}
	// ����ҵ��ĵ�Ϊ��㣬�򷵻���ǰ����
	if(IsFork(lpNow))
	{
		*lppPos = lpLast;
	}	
	else	// ���򷵻ص�ǰ��
	{
		*lppPos = lpNow;
	}

	return 0;
}

//
//	PX: ���ݵ�ַ�õ���ǰ��ĺ�����
//
sint32  PX(uint8 *lpPos)
{
/////////////////////////////////////////////////////////////////////////
//	lpPos: [in] ��ǰ���ַ
/////////////////////////////////////////////////////////////////////////
	return ((lpPos - g_lpOrgFinger) % IMGW);
}

//
//	PY: ���ݵ�ַ�õ���ǰ���������
//
sint32  PY(uint8 *lpPos)
{
/////////////////////////////////////////////////////////////////////////
//	lpPos: [in] ��ǰ���ַ
/////////////////////////////////////////////////////////////////////////
	return ((lpPos - g_lpOrgFinger) / IMGW);
}

//
//	GetJiajiao: �������Ƕȵļн�(0 - 90)
//
sint32  GetJiajiao(sint32 angle1, sint32 angle2)
{
/////////////////////////////////////////////////////////////////////////
//	angle1: [in] �Ƕ�һ ��0 - 180��
//	angle2: [in] �Ƕȶ� ��0 - 180��
/////////////////////////////////////////////////////////////////////////
	sint32  a;

	a = abs(angle1-angle2);
	if(a > 90)
	{
		a = 180-a;
	}
	return a;
}


//
//	GetAngleDis�� �����Ƕȼ�ľ���(0-360)  (��ʱ�뷽��)
//
sint32  GetAngleDis(sint32 angleBegin, sint32 angleEnd)
{
	/////////////////////////////////////////////////////////////////////////
	//	angleBegin: [in] ��ʼ�Ƕ� ��0 - 360��
	//	angleEnd: [in] �ս�Ƕ� ��0 - 360��
	/////////////////////////////////////////////////////////////////////////
	sint32  a;
	
	a = angleEnd-angleBegin;
	if(a < 0)
	{
		a += 360;
	}
	return a;
}

//
//	GetAngle�� ������������x�᷽��ĽǶ�
//
sint32  GetAngle(sint32 x0, sint32 y0, sint32 x1, sint32 y1)
{
	/////////////////////////////////////////////////////////////////////////
	//	x0: [in] ��һ���������
	//	y0: [in] ��һ����������
	//	x1: [in] �ڶ����������
	//	y1: [in] �ڶ�����������
	/////////////////////////////////////////////////////////////////////////
	double angle;
	
	angle = atan2((double)(y1-y0), (double)(x1-x0));
	// ����ת����0 - 2*PI
	if(angle < 0)
	{
		angle += 2*PI;
	}
	// ����ת��Ϊ�Ƕ�
	return (sint32)(angle*EPI + 0.5);
}



int getSingular(BYTE *lpOrient, int Width, int Height, char flag)
{
	int    x, y, i;
	int    num;
	int    sum1, sum2;
	int    d;
	unsigned char *pOriMap;

	int    oriV;
	int    a1, a2;
	DBLPOINT  singuArr[30];

	int    value;
	double dis;
	bool   bFound = false;
	bool   fg = false;
	int    D3[8][2] = {
		-1,-1, -1,0,  -1,1,  0,1,  1,1,  1,0,  1,-1,  0,-1
	};
	int    D5[12][2] = {
		-2,-1, -2,0,  -2,1, -1,2,  0,2,  1,2, 2,1,  2,0,  2,-1, 1,-2, 0,-2, -1,-2
	};
	int    D7[24][2] = {
		-3,-3, -3,-2, -3,-1, -3,0, -3,1, -3,2, -3,3, -2,3, -1,3, 0,3, 1,3, 2,3, 3,3, 3,2, 3,1, 3,0,
			3,-1, 3,-2, 3,-3, 2,-3, 1,-3, 0,-3, -1,-3, -2,-3
	};
	

	num = 0;

	memset(singuArr, 0, sizeof(singuArr));				//��ʼ����������飬����

	for(y = 3; y < Height-3; y++)
	{
		for(x = 3; x < Width-3; x++)					//��������ͼ
		{
			pOriMap = lpOrient + y*Width + x;			//���ĳ��ķ���ָ��
			oriV = *pOriMap;							//��ȡĳ��ķ��򳡵�ֵ

			if(oriV == 255)								//���Ǳ���������������һ��ѭ��
			{
				continue;
			}

			fg = false;									
			for(i = 0; i < 24; i++)
			{
				if(pOriMap[D7[i][1]*Width + D7[i][0]]==255)
				{
					fg = true;
					break;
				}
			}
			if(fg)
			{
				continue;
			}

			sum1 = 0;
			for(i = 0; i < 8; i++)
			{
				a1 = pOriMap[D3[i][1]*Width + D3[i][0]]/24;
				a2 = pOriMap[D3[(i+1)%8][1]*Width + D3[(i+1)%8][0]]/24;
				d = getOriChange(a1, a2, flag);
				
				if(abs(d) > 5)
				{
					break;
				}
				
				sum1 += d;
			}
			sum2 = 0;
			for(i = 0; i < 12; i++)
			{
				a1 = pOriMap[D5[i][1]*Width + D5[i][0]]/24;
				a2 = pOriMap[D5[(i+1)%12][1]*Width + D5[(i+1)%12][0]]/24;
				d = getOriChange(a1, a2, flag);
				
				if(abs(d) > 5)
				{
					break;
				}
				
				sum2 += d;
			}
		    
			if(flag == -1)									//���ĵ�
			{
				value = -10;
			}
			else if(flag == 1)								//���ǵ�
			{
				value = 10;
			}
			if(sum2 == value && sum1 == value)
			{
				bFound = false;
				for(i = 0; i < num; i++)
				{
					dis = sqrt((double)((x - singuArr[i].x)*(x - singuArr[i].x) +
						(y - singuArr[i].y)*(y - singuArr[i].y)));
					if(dis < 4)
					{
						singuArr[i].x = (singuArr[i].x + x)/2.0;
						singuArr[i].y = (singuArr[i].y + y)/2.0;
						bFound = true;
						break;
					}

				}
				if(!bFound)
				{
					singuArr[num].x = x;
					singuArr[num].y = y;
					num++;
					if(num >= MAX_SINGULARYNUM)				//���������������������ƣ���ֹͣѰ�������
					{
						goto next;
					}
				}

			}
		}
	}

next:

	int  trilen = 16;
	int N = 0;											//�����ĸ���
	for(i = 0; i < num; i++)								//���ҵ�������������������
	{
		//lpArr[*lpNum + i].x = (long)singuArr[i].x;
		//lpArr[*lpNum + i].y = (long)singuArr[i].y;	

		int x = (long)singuArr[i].x;
		int y = (long)singuArr[i].y;
		
		if (*(g_lpDivide + y * Width + x) == 255) 
			continue;

		if (((x + 0.0) / Width < 0.1) || ((x + 0.0) / Width > 0.9)) 
			continue;

		if (((y + 0.0) / Height < 0.1 ) || ((y + 0.0) / Height > 0.9))
			continue;

		

		g_Feature.MinutiaArr[g_Feature.MinutiaNum + N].x = x;
		g_Feature.MinutiaArr[g_Feature.MinutiaNum + N].y = y;

		g_Feature.MinutiaArr[g_Feature.MinutiaNum + N].Type = (flag == -1) ? VF_MINUTIA_CORE : VF_MINUTIA_DELTA;				
		g_Feature.MinutiaArr[g_Feature.MinutiaNum + N].Direction = 0;				

		N++;
	}

	g_Feature.MinutiaNum += N;

	return 0;
}


int getOriChange(int angle1, int angle2, char flag)
{
	int  d;
	
	d = angle2 - angle1;
	if(flag >= 1)
	{
		if(d < 0)
		{
			d += 10;
		}
		
	}
	else if(flag <= -1)
	{
		if(d > 0)
		{
			d -= 10;
		}
	}
	
	return d;
}


//
//	VF_FeatureEncode: ��ָ����������ѹ������
//
VF_RETURN VF_FeatureEncode(FEATUREPTR lpFeature, uint8 *lpData, sint32 *lpSize)
{
	///////////////////////////////////////////////////////////////////////
	//	lpFeature: [in] Ҫ�����ָ������ָ��
	//  lpData:	[out] ������ָ���������ݻ�����ָ��
	//  lpSize:	[out] ������ָ��������С
	///////////////////////////////////////////////////////////////////////
	
	sint32 re;
	
	// ѹ������
	re = EncodeFeature(lpFeature, lpData);
	if(re == 0)
		return VF_FEATUREENCODE_ERR;
	
	*lpSize = re;
	
	return VF_OK;
}

//
//	EncodeFeature: ��ָ����������ѹ������
//
sint32	EncodeFeature(FEATUREPTR lpFeature, uint8 *lpData)
{
	/////////////////////////////////////////////////////////////////////
	//	lpFeature: [in] Ҫ�����ָ������ָ��
	//  lpData: [out] ѹ��������ָ������������ָ��
	/////////////////////////////////////////////////////////////////////
	sint32	i;
	sint32	totalLen = 0;
	sint32	len = 0;
	sint32	x, y, o;
	uint8	v, t, mn;
	sint32  boSize = 0;
	
	if(lpData == NULL || lpFeature == NULL)
		return 0;
	
	// ��������Ŀ
	mn = (uint8)lpFeature->MinutiaNum;
	// �����ܳ���
	totalLen = 4 + 7*mn;////////7*mn
	// ��һ���ֽ�Ϊ�����ܳ���
	lpData[0] = (uint8)(totalLen / 256);
	lpData[1] = (uint8)(totalLen-lpData[0]*256);
	
	// �ڶ����ֽ�Ϊ��������Ŀ
	lpData[2] = mn;
	
	len = 3;
	
	// ��ÿ�����������ѹ��
	for(i = 0; i < mn; i++)
	{
		x = lpFeature->MinutiaArr[i].x;		// 10λ��ʾ
		y = lpFeature->MinutiaArr[i].y;		// 10λ��ʾ
		o = lpFeature->MinutiaArr[i].Direction; // 9λ��ʾ
		t = (uint8)lpFeature->MinutiaArr[i].Type;	// 3λ��ʾ
		lpData[len] = (uint8)(x >> 2);
		lpData[len+1] = (uint8)((x & 0x3) << 6) + (uint8)(y >> 4);
		lpData[len+2] = (uint8)((y & 0xF) << 4) + (uint8)(o >> 5);
		lpData[len+3] = (uint8)((o & 0x1F) << 3) + (uint8)(t & 0x7);
		lpData[len+4] = (uint8)lpFeature->MinutiaArr[i].Triangle[0];
		lpData[len+5] = (uint8)lpFeature->MinutiaArr[i].Triangle[1];
		lpData[len+6] = (uint8)lpFeature->MinutiaArr[i].Triangle[2];
		
		len += 7;
	}
	
	// ����У��λ
	v = 0;
	for(i = 0; i < totalLen-1; i++)
	{
		v = v ^ lpData[i];
	}
	// ���һ���ֽڷ�У���ֽ�
	lpData[len] = v;
	
	
	return totalLen;
}


int FPA_PatternMatch(BYTE *lpFeature1, BYTE *lpFeature2, int *lpScore)
{
	return PatternMatch(lpFeature1, lpFeature2, lpScore);	
}


//
// VerifyMatch: ����ָ��������ıȶ�
//
 int PatternMatch(BYTE *lpFeature1, BYTE *lpFeature2, int *lpScore)
{
	//////////////////////////////////////////////////////////////////////////
	//	lpFeature1:		[in] ��һ��ָ������
	//	lpFeature2:		[in] �ڶ���ָ������
	//	lpScore:		[out] �ȶԵ����ƶ�
	//	FastMode:		[in] �Ƿ���п���ģʽ�ȶ�
	//////////////////////////////////////////////////////////////////////////
	// TODO: Add your implementation code here
	VF_RETURN	re;
	MATCHRESULT mr;
	FEATURE		feat1, feat2;
	
	// ��һ��ָ�������Ľ���
	re = VF_FeatureDecode(lpFeature1, &feat1);
	if(re != VF_OK)
		return re;
	
	// �ڶ���ָ�������Ľ���
	re = VF_FeatureDecode(lpFeature2, &feat2);
	if(re != VF_OK)
		return re;
	
	*lpScore = 0;
	
	bool FastMode = true;
	
	if(FastMode)
	{
		// ����ģʽ�ıȶ�
		VF_VerifyMatch(&feat1, &feat2, &mr, VF_MATCHMODE_IDENTIFY);
	}
	else
	{
		// ��ȷģʽ�ıȶ�
//		VF_VerifyMatch(&feat1, &feat2, &mr, VF_MATCHMODE_VERIFY);
	}
	
	// ƥ������ƶ�
	if(mr.MMCount < 8)
		*lpScore = 0;
	else
		*lpScore = mr.Similarity;
	
	return 0;
}


 //
 //	VF_FeatureDecode: ��ָ���������н���
 //
 VF_RETURN VF_FeatureDecode(uint8 *lpData, FEATUREPTR lpFeature)
 {
	 ///////////////////////////////////////////////////////////////////////
	 //  lpData:	[in] Ҫ�����ָ���������ݻ�����ָ��
	 //  lpFeature:	[out] ������ָ������
	 ///////////////////////////////////////////////////////////////////////
	 
	 sint32 re;
	 // ����
	 re = DecodeFeature(lpData, lpFeature);
	 
	 if(re != 0)
		 return VF_FEATUREDECODE_ERR;
	 
	 return VF_OK;
}

 //
 //	DecodeFeature: ��ָ���������н���
 //
 sint32	DecodeFeature(uint8 *lpData, FEATUREPTR lpFeature)
 {
	 /////////////////////////////////////////////////////////////////////
	 //  lpData: [in] ѹ�������ָ������������ָ��
	 //	lpFeature: [out] ������ָ������ָ��
	 /////////////////////////////////////////////////////////////////////
	 sint32	i;
	 sint32	totalLen = 0;
	 sint32	len = 0;
	 uint8	v;
	 sint32	boSize = 0;
	 if(lpData == NULL || lpFeature == NULL)
		 return 1;
	 
	 memset((void *)lpFeature, 0, sizeof(FEATURE));
	 // ��һ���ֽ�Ϊ�����ܳ�
	 totalLen = (sint32)(lpData[0]*256 + lpData[1]);
	 // ����У��λ
	 v = 0;
	 for(i = 0; i < totalLen; i++)
	 {
		 v = v ^ lpData[i];
	 }
	 if(v != 0)	// У��ʧ���򷵻ش���
		 return 2;
	 
	 // �ڶ����ֽ�Ϊָ����������Ŀ
	 lpFeature->MinutiaNum = (uint8)lpData[2]; 
	 
	 len = 3;
	 
	 // ����
	 for(i = 0; i < lpFeature->MinutiaNum; i++)
	 {
		 lpFeature->MinutiaArr[i].x = (sint32)(((sint32)lpData[len])<<2) + 
			 (sint32)(((sint32)(lpData[len+1] & 0xC0))>>6);
		 lpFeature->MinutiaArr[i].y = (sint32)(((sint32)(lpData[len+1] & 0x3F))<<4) + 
			 (sint32)(((sint32)(lpData[len+2] & 0xF0))>>4);
		 lpFeature->MinutiaArr[i].Direction = (sint32)(((sint32)(lpData[len+2] & 0xF))<<5) + 
			 (sint32)(((sint32)(lpData[len+3] & 0xF8))>>3);
		 lpFeature->MinutiaArr[i].Type = (uint8)(lpData[len+3] & 0x7);
		 lpFeature->MinutiaArr[i].Triangle[0] = lpData[len+4];
		 lpFeature->MinutiaArr[i].Triangle[1] = lpData[len+5];
		 lpFeature->MinutiaArr[i].Triangle[2] = lpData[len+6];
		 
		 len += 7;
	 }
	 
	 return 0;
 }


 //
 // VF_VerifyMatch: ����ָ�������ıȶ�
 //
 BOOL	VF_VerifyMatch(FEATUREPTR lpFeature1, FEATUREPTR lpFeature2, 
	 PMATCHRESULT lpMatchResult, VF_FLAG matchMode)
 {
	 ///////////////////////////////////////////////////////////////////////
	 //  lpFeature1:	[in] ��һ��ָ������
	 //  lpFeature2:	[in] �ڶ���ָ������
	 //  lpMatchResult:	[out] �ȶԽ��
	 //  matchMode:	[in] �ȶ�ģʽ
	 ///////////////////////////////////////////////////////////////////////
	 
	 // �ȶ�
	 verify(lpFeature1, lpFeature2, lpMatchResult, matchMode);
	 
	 // Ĭ����ֵΪ50�����ƶ�>=50����Ϊ�ȶԳɹ�������ȶ�ʧ��,
	 // һ������£���35���ϼ�����Ϊ����, Ϊ��ȫ����������50-100
	 if(lpMatchResult->Similarity >= 50)
		 return TRUE;
	 else
		 return FALSE;
 }

 //
 //	verify : ָ�Ʊȶ�
 //
 void	verify(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, PMATCHRESULT lpMatchResult,
	 VF_FLAG matchMode)
 {
	 /////////////////////////////////////////////////////////////////////////////
	 //	lpFeature: [in] Ҫ�ȶԵĵ�һ��ָ������ָ��
	 //	lpTemplate: [in] Ҫ�ȶԵĵڶ���ָ������ָ��
	 //	lpMatchResult: [out] �ȶԽ��ָ��
	 //	matchMode: [in] �ȶ�ģʽ
	 /////////////////////////////////////////////////////////////////////////////
	 
	 MATCHRESULT globalMatchResult;
	 sint32		score = 0;
	 
	 //ȫ�ֱȶ�
	 globalMatch(lpFeature, lpTemplate, &globalMatchResult, matchMode);
	 //patternMatch(lpFeature, lpTemplate, &globalMatchResult, matchMode);
	 *lpMatchResult = globalMatchResult;
	 //���ƥ�������������٣���ԱȶԽ�����ƶȽ�������
	 if(matchMode != VF_MATCHMODE_IDENTIFY && lpMatchResult->MMCount < 15)
	 {
		 score = (sint32)(lpMatchResult->Similarity*sin(lpMatchResult->MMCount*PI/30.0) + 0.5);
		 lpMatchResult->Similarity = score;
	 }
	 
}


 //
//	globalMatch: ָ��������ȫ�ֱȶ�
//
void  globalMatch(FEATUREPTR lpFeature, FEATUREPTR lpTemplate, 
				  PMATCHRESULT lpMatchResult, VF_FLAG matchMode)
{
/////////////////////////////////////////////////////////////////////////////
//	lpFeature: [in] Ҫ�ȶԵĵ�һ��ָ������ָ��
//	lpTemplate: [in] Ҫ�ȶԵĵڶ���ָ������ָ��
//	lpMatchResult: [out] �ȶԽ��ָ��
//	matchMode: [in] �ȶ�ģʽ
/////////////////////////////////////////////////////////////////////////////
	sint32	m, n, a1, a2;
	sint32	rotation;		// ��ת�Ƕ�
	sint32	transx, transy;	// λ��ƫ��
	FEATURE	alignFeature;	// ������ָ������
	MATCHRESULT alignMax;	// ���ƶ����ıȶԽ��
	MATCHRESULT globalMatchResult;	// �ȶԽ��
	sint32  agate = 8;		// �������˽ṹ�Ƕ����
	sint32  num = 0;
	// ��ʼ����õıȶԽ��
	alignMax.Similarity = 0;
	alignMax.MMCount = 0;
	alignMax.Rotation = 0;
	alignMax.TransX = 0;
	alignMax.TransY = 0;


	// ����ͬ���͵�ָ������������Ϊͬһ��ָ���������ж���ȶ�
	for(n = 0; n < lpTemplate->MinutiaNum; n++)
	{
		for(m = 0; m < lpFeature->MinutiaNum; m++)
		{
			// ��ͬ�����򲻱ȶ�
			if(lpFeature->MinutiaArr[m].Type != lpTemplate->MinutiaArr[n].Type)
				continue;
			if(matchMode == VF_MATCHMODE_IDENTIFY)
			{
				// �������������˽ṹ�ȶԣ������������׼
				if(lpFeature->MinutiaArr[m].Triangle[0] != 255 && lpTemplate->MinutiaArr[n].Triangle[0] != 255)
				{
					a1 = GetJiajiao(lpFeature->MinutiaArr[m].Triangle[0], lpFeature->MinutiaArr[m].Direction % 180);
					a2 = GetJiajiao(lpTemplate->MinutiaArr[n].Triangle[0], lpTemplate->MinutiaArr[n].Direction % 180);
					if(abs(a1-a2)>agate)
						continue;
				}
				if(lpFeature->MinutiaArr[m].Triangle[0] != 255 && lpTemplate->MinutiaArr[n].Triangle[0] != 255 &&
					lpFeature->MinutiaArr[m].Triangle[1] != 255 && lpTemplate->MinutiaArr[n].Triangle[1] != 255)
				{
					a1 = GetJiajiao(lpFeature->MinutiaArr[m].Triangle[0], lpFeature->MinutiaArr[m].Triangle[1]);
					a2 = GetJiajiao(lpTemplate->MinutiaArr[n].Triangle[0], lpTemplate->MinutiaArr[n].Triangle[1]);
					if(abs(a1-a2)>agate)
						continue;
				}
				if(lpFeature->MinutiaArr[m].Triangle[2] != 255 && lpTemplate->MinutiaArr[n].Triangle[2] != 255 &&
					lpFeature->MinutiaArr[m].Triangle[1] != 255 && lpTemplate->MinutiaArr[n].Triangle[1] != 255)
				{
					a1 = GetJiajiao(lpFeature->MinutiaArr[m].Triangle[1], lpFeature->MinutiaArr[m].Triangle[2]);
					a2 = GetJiajiao(lpTemplate->MinutiaArr[n].Triangle[1], lpTemplate->MinutiaArr[n].Triangle[2]);
					if(abs(a1-a2)>agate)
						continue;
				}
				if(lpFeature->MinutiaArr[m].Triangle[0] != 255 && lpTemplate->MinutiaArr[n].Triangle[0] != 255 &&
					lpFeature->MinutiaArr[m].Triangle[2] != 255 && lpTemplate->MinutiaArr[n].Triangle[2] != 255)
				{
					a1 = GetJiajiao(lpFeature->MinutiaArr[m].Triangle[0], lpFeature->MinutiaArr[m].Triangle[2]);
					a2 = GetJiajiao(lpTemplate->MinutiaArr[n].Triangle[0], lpTemplate->MinutiaArr[n].Triangle[2]);
					if(abs(a1-a2)>agate)
						continue;
				}
			}
			alignFeature.MinutiaNum = 0;
			// ��ת�Ƕ�
			rotation = GetAngleDis(lpFeature->MinutiaArr[m].Direction, 
									lpTemplate->MinutiaArr[n].Direction);
			// λ��ƫ��
			transx = (lpTemplate->MinutiaArr[n].x - lpFeature->MinutiaArr[m].x);
			transy = (lpTemplate->MinutiaArr[n].y - lpFeature->MinutiaArr[m].y);
			
			// ��lpFeature��lpTemplate����
			align(lpFeature, &alignFeature, &lpFeature->MinutiaArr[m],
					rotation, transx, transy);
			// �����������ָ���������бȶ�
			alignMatch(&alignFeature, lpTemplate, &globalMatchResult, matchMode);
			// ����ȶԽ������õıȶԽ�����ã������alignMax
			if(globalMatchResult.Similarity > alignMax.Similarity)
			{
				alignMax.MMCount = globalMatchResult.MMCount;
				alignMax.Similarity = globalMatchResult.Similarity;
				alignMax.Rotation = rotation;
				alignMax.TransX = transx;
				alignMax.TransY = transy;
				
				// ����ǿ��ٱȶ�ģʽ�������ƶȴﵽһ���̶����˳�
				if(matchMode == VF_MATCHMODE_IDENTIFY && alignMax.MMCount >= 8 )
				{
					if(alignMax.Similarity > 100)
					{
						*lpMatchResult = alignMax;
						return;
					}
				}
			}
		}
	}
	
	//���ձȶԽ��
	*lpMatchResult = alignMax;
}


//
//	align: ��ָ��������һ���ĽǶȺ�λ��ƫ�ƽ�������任
//
void align(FEATUREPTR lpFeature, FEATUREPTR lpAlignedFeature, MINUTIAPTR lpFeatureCore,
				  sint32 rotation, sint32 transx, sint32 transy)
{
///////////////////////////////////////////////////////////////////////
//  lpFeature:	[in] Ҫ�任��ָ������
//  lpAlignedFeature:	[out] ��������任���ָ������
//  lpFeatureCore:	[in] ��ת�任������������
//  rotation:	[in] ��ת�Ƕ�
//  transx:	[in] ˮƽƫ��
//  transy: [in] ��ֱƫ��
///////////////////////////////////////////////////////////////////////

	sint32  i;
	sint32  x, y;
	sint32  cx, cy;
	double  rota, sinv, cosv;

	// ���������ṹ��Ϣ
	*lpAlignedFeature = *lpFeature;
	
	// ����ת�������ĵ�����
	cx = lpFeatureCore->x;
	cy = lpFeatureCore->y;
	// ��ת�Ļ���
	rota = rotation/EPI;
	// ��ת���ȵ�sinֵ
	sinv = sin(rota);
	// ��ת���ȵ�cosֵ
	cosv = cos(rota);
	for(i = 0; i < lpFeature->MinutiaNum; i++)
	{
		x = lpFeature->MinutiaArr[i].x;
		y = lpFeature->MinutiaArr[i].y;
		//  ����ת�����������
		lpAlignedFeature->MinutiaArr[i].x = (sint32)(cx + cosv*(x-cx) - sinv*(y-cy) + transx + 0.5);
		lpAlignedFeature->MinutiaArr[i].y = (sint32)(cy + sinv*(x-cx) + cosv*(y-cy) + transy + 0.5);
		// ��ת����������·���
		lpAlignedFeature->MinutiaArr[i].Direction = (lpFeature->MinutiaArr[i].Direction + rotation) % 360;
	}

}

//
// alignMatch: ��������ϵ�����ָ���������бȶ�
//
void alignMatch(FEATUREPTR lpAlignFeature, FEATUREPTR lpTemplate, 
				PMATCHRESULT lpMatchResult, VF_FLAG matchMode)
{
	sint32	i, j;
	uint8	flagA[MAXMINUTIANUM];	// ���lpAlignFeature���������Ƿ��Ѿ���ƥ�����
	uint8	flagT[MAXMINUTIANUM];	// ���lpTemplate���������Ƿ��Ѿ���ƥ�����
	sint32	x1, y1, x2, y2;
	sint32  dis, angle;
	sint32  score, matchNum, s;
	sint32	num1, num2;
	sint32  step = 1;
	num1 = lpAlignFeature->MinutiaNum;
	num2 = lpTemplate->MinutiaNum;
	// �������
	memset(flagA, 0, MAXMINUTIANUM);
	memset(flagT, 0, MAXMINUTIANUM);

	score = 0;		// �ܷ�����
	matchNum = 0;	// ƥ��������������



	// ��ͬ�����������ƥ�����
	for(i = 0; i < lpTemplate->MinutiaNum; i++)
	{
		if(flagT[i])	// �Ƿ�����ƥ�����
			continue;
		for(j = 0; j < lpAlignFeature->MinutiaNum; j++)
		{
			if(flagA[j])	// �Ƿ�����ƥ�����
				continue;
			// �����������Ƿ���ͬ
			if(lpTemplate->MinutiaArr[i].Type != lpAlignFeature->MinutiaArr[j].Type)
				continue;
			// �����㷽��н�
			angle = AngleAbs360(lpTemplate->MinutiaArr[i].Direction, 
								lpAlignFeature->MinutiaArr[j].Direction);
			// �н�>=10��ƥ��
			if(angle >= 10)
				continue;
			x1 = lpTemplate->MinutiaArr[i].x;
			y1 = lpTemplate->MinutiaArr[i].y;
			x2 = lpAlignFeature->MinutiaArr[j].x;
			y2 = lpAlignFeature->MinutiaArr[j].y;
			// ˮƽ����>=10��ƥ��
			if(abs(x1-x2) >= 10)
				continue;
			// ��ֱ����>=10��ƥ��
			if(abs(y1-y2) >= 10)
				continue;
			// ���������ľ���
			dis = DisTbl[abs(y1-y2)][abs(x1-x2)];
			// ����>=10��ƥ��
			if(dis >= 10)
				continue;
			// ������������������ǣ���ʾ�Ѿ���ƥ�����
			flagA[j] = 1;
			flagT[i] = 1;
			// �ּܷ��ϴ������������ƥ�����
			// �˱���dis,angleԽ�󣬷���ԽС
			score += (10-angle);
			score += (10-dis);
			// ƥ������������һ
			matchNum++;
			
			// ����ǿ��ٱȶ�ģʽ
			if(matchMode == VF_MATCHMODE_IDENTIFY && matchNum >= 8)
			{
				// �������ƶ�
				s = 4 * score * matchNum * MAXMINUTIANUM / ((num1 + num2) * ( num1 + num2));
				if(s > 100)  // ���ƶ��㹻���򷵻رȶԽ��
				{
					lpMatchResult->MMCount = matchNum;
					lpMatchResult->Rotation = 0;
					lpMatchResult->Similarity = s;
					lpMatchResult->TransX = 0;
					lpMatchResult->TransX = 0;
					return;
				}
			}

		}
	}

	if(matchMode != VF_MATCHMODE_IDENTIFY)
	{
		// ����ͼ��������¿��ܶ˵㴦��ɲ����㴦��ɶ˵㣬�������Ϊ50%������
		// ��������ķ���
		for(i = 0; i < lpTemplate->MinutiaNum; i++)
		{
			if(flagT[i])	// �Ƿ�����ƥ�����
				continue;
			for(j = 0; j < lpAlignFeature->MinutiaNum; j++)
			{
				if(flagA[j])	// �Ƿ�����ƥ�����
					continue;
				// �Ƿ����Ͳ�ͬ
				if(lpTemplate->MinutiaArr[i].Type == lpAlignFeature->MinutiaArr[j].Type)
					continue; 
				// �����㷽��н�
				angle = AngleAbs360(lpTemplate->MinutiaArr[i].Direction, 
									lpAlignFeature->MinutiaArr[j].Direction);
				// �н�>=10��ƥ��
				if(angle >= 10)
					continue;
				x1 = lpTemplate->MinutiaArr[i].x;
				y1 = lpTemplate->MinutiaArr[i].y;
				x2 = lpAlignFeature->MinutiaArr[j].x;
				y2 = lpAlignFeature->MinutiaArr[j].y;

				// ˮƽ����>=10��ƥ��
				if(abs(x1-x2) >= 10)
					continue;
				// ��ֱ����>=10��ƥ��
				if(abs(y1-y2) >= 10)
					continue;
				// ���������ľ���
				dis = DisTbl[abs(y1-y2)][abs(x1-x2)];
				// ����>=10��ƥ��
				if(dis >= 10)
					continue;
				// ������������������ǣ���ʾ�Ѿ���ƥ�����
				flagA[j] = 1;
				flagT[i] = 1;
				// �ּܷ��ϴ������������ƥ�����
				score += ((10-angle)/2);
				score += ((10-dis)/2);
				// ƥ������������һ
				matchNum++;

			}
		}								
	}
	// �������ƶȣ����رȶԽ��
	s = 4 * score * matchNum * MAXMINUTIANUM / ((num1+num2)*(num1+num2));

	lpMatchResult->MMCount = matchNum;
	lpMatchResult->Rotation = 0;
	lpMatchResult->Similarity = s;
	lpMatchResult->TransX = 0;
	lpMatchResult->TransX = 0;

}


//
//	AngleAbs360: �������Ƕȵļн�(0 - 180)
//
sint32  AngleAbs360(sint32 angle1, sint32 angle2)
{
	/////////////////////////////////////////////////////////////////////////
	//	angle1: [in] �Ƕ�һ ��0 - 360��
	//	angle2: [in] �Ƕȶ� ��0 - 360��
	/////////////////////////////////////////////////////////////////////////
	sint32 a;
	a = abs(angle1 - angle2);
	if(a > 180)
	{
		return (360 - a);
	}
	else
	{
		return a;
	}
}


void CDSplitView::OnOpenFileOne() 
{
	// TODO: Add your command handler code here
		CFileDialog dlg(TRUE,"bmp",".bmp",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"Bitmap (*.BMP)|*.BMP||");/////////���ļ�ѡ��Ի���
//	HBITMAP m_hImage;//////����λͼ����

	if (dlg.DoModal() != IDOK) return;//////////�����ȡ����

		ImagePathName1 = dlg.GetPathName();				/////////////�����ȷ����
		
	
	int lpSize1;
	FPA_CreateImage(lpImage1);
	FPA_LoadImage(ImagePathName1, lpImage1, &lpData1, &Width1, &Height1);
	

	FPA_AnalyzeFeature_File(ImagePathName1, lpFeature1, &lpSize1);
	int i;
	CString   stemp,str;   
	
	str   =   "";   
	for(i=0;i<lpSize1;i++)
	{
		stemp.Format("%d ",lpFeature1[i]);
		str+=stemp;
	}
		filename = ImagePathName1;
		Invalidate();
}



void CDynSplitView2::OnOpenFileTwo() 
{
	// TODO: Add your command handler code here
		CFileDialog dlg(TRUE,"bmp",".bmp",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"Bitmap (*.BMP)|*.BMP||");

	if (dlg.DoModal() != IDOK) return;

		ImagePathName2 = dlg.GetPathName();				


   int lpSize2;
	FPA_CreateImage(lpImage2);
	FPA_LoadImage(ImagePathName2, lpImage2, &lpData2, &Width2, &Height2);			
 
	
	FPA_AnalyzeFeature_File(ImagePathName2, lpFeature2, &lpSize2);
	int j;
	CString   stemp2,str2;   
	
	str2   =   "";   
	for(j=0;j<lpSize2;j++)
	{
		stemp2.Format("%d ",lpFeature2[j]);
		str2+=stemp2;
	}
	filename = ImagePathName2;
	Invalidate();
	
}





void CDynSplitView2::OndeleteNoise2() 
{
	// TODO: Add your command handler code here
	filename = "C:\\3.bmp";
	Invalidate();
}

void CDynSplitView2::Onthin2() 
{
	// TODO: Add your command handler code here
	filename = "C:\\5.bmp";
	Invalidate();
	
}

void CDynSplitView2::OnLvBo2() 
{
	// TODO: Add your command handler code here
	filename = "C:\\1.bmp";
		Invalidate();
	
}

void CDynSplitView2::Ondirection2() 
{
	// TODO: Add your command handler code here
	filename = "C:\\0.bmp";
	Invalidate();
	
}


void CDynSplitView2::Onmatch() 
{
	// TODO: Add your command handler code here

	int score;

	FPA_PatternMatch(lpFeature1, lpFeature2, &score);
	//FPA_Match(lpFeature1, lpFeature2, &score);


	
	if (score > 70)
	{
		AfxMessageBox("Same Fingerprint!");
	}
	else
	{
		AfxMessageBox("Different Fingerprints");
	}
	Invalidate();
	
}







void CDSplitView::OnLvBo1() 
{
	// TODO: Add your command handler code here
	filename = "C:\\1.bmp";
	Invalidate();
}

void CDSplitView::Onthin1() 
{
	// TODO: Add your command handler code here
	filename = "C:\\5.bmp";
	Invalidate();
}


void CDSplitView::Ondirection1() 
{
	// TODO: Add your command handler code here
	filename = "C:\\0.bmp";
	Invalidate();
	
}



