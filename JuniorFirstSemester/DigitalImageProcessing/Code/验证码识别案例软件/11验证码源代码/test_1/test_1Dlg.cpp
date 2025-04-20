// test_1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "test_1.h"
#include "test_1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString saveFilename("temp.bmp");
CString filename("");

#include "Lib/JpegToBmpLib.h"
#include "Lib/JpegToBmp.h"
#pragma comment(lib, "Lib/JpegToBmp.lib")
/////////////////////////////////////////////////////////////////////////////
// CTest_1Dlg dialog

CTest_1Dlg::CTest_1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTest_1Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTest_1Dlg)
	m_result = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTest_1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTest_1Dlg)
	DDX_Text(pDX, IDC_EDIT1, m_result);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTest_1Dlg, CDialog)
	//{{AFX_MSG_MAP(CTest_1Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnRead)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTest_1Dlg message handlers

BOOL CTest_1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically  ���ô˶Ի����ͼ�ꡣ�ÿ���Զ�ִ��
	//  when the application's main window is not a dialog     ��Ӧ�ó���������ڲ��ǶԻ���
	SetIcon(m_hIcon, TRUE);			// Set big icon   ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// Set small icon   ����Сͼ��
	 
	// TODO: Add extra initialization here     �ڴ���Ӷ���ĳ�ʼ��
	
	return TRUE;  // return TRUE  unless you set the focus to a control   ����TRUE�������㽫�������õ��ؼ�
}

// If you add a minimize button to your dialog, you will need the code below    �����ĶԻ��������С����ť���㽫��Ҫ����Ĵ���
//  to draw the icon.  For MFC applications using the document/view model,      �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵�MFCӦ�ó���
//  this is automatically done for you by the framework.                         �����Զ�Ϊ�����Ŀ�ܡ�

void CTest_1Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting  Ϳװ�豸������

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle    �ڿͻ��˾��ε�����ͼ��
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
  
		// Draw the icon     ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		InitBmp();
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags    ���û��϶�ϵͳ���ô˺���ȡ�ù����ʾ
//  the minimized window.   ��С������
HCURSOR CTest_1Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTest_1Dlg::OnRead() 
{   
	CFileDialog fg(TRUE);
	if(IDOK==fg.DoModal())
	{
       filename=fg.GetPathName();
	}
	HWND hPic=GetDlgItem(IDC_STATIC)->m_hWnd;
	HDC hPicDc=::GetWindowDC(hPic);
	RGBTRIPLE *rgb=NULL;
//    CString filename = "2489.jpg";   CString���ļ���=��2489.jpg��;
	saveFilename = _T("temp.bmp");
	BITMAPFILEHEADER filehead;
	BITMAPINFOHEADER fileinfo;
	JpegToBmp(filename, saveFilename);
	InitBmp();
    CFile ff(saveFilename,CFile::modeReadWrite);
	ff.Read(&filehead,sizeof(filehead));
	ff.Read(&fileinfo,sizeof(fileinfo));
	rgb = new RGBTRIPLE[fileinfo.biWidth*fileinfo.biHeight];
	ff.Read(rgb,fileinfo.biWidth*fileinfo.biHeight*3);
	//�ڰ״���
	for (int i=0; i<fileinfo.biHeight;i++)
	{
		for (int j=0; j<fileinfo.biWidth; j++)
		{   
  
			if((rgb[i*fileinfo.biWidth+j].rgbtRed +rgb[i*fileinfo.biWidth+j].rgbtGreen + rgb[i*fileinfo.biWidth+j].rgbtBlue)/3<=140)
			{
              rgb[i*fileinfo.biWidth+j].rgbtBlue=0;
			  rgb[i*fileinfo.biWidth+j].rgbtGreen=0;
              rgb[i*fileinfo.biWidth+j].rgbtRed=0;
			}
			else
			{
              rgb[i*fileinfo.biWidth+j].rgbtBlue=255;
			  rgb[i*fileinfo.biWidth+j].rgbtGreen=255;
              rgb[i*fileinfo.biWidth+j].rgbtRed=255;
			}
            ff.Seek(54,CFile::begin);
			ff.Write(rgb,sizeof(RGBTRIPLE)*fileinfo.biWidth*fileinfo.biHeight);
		}
	}
	ff.Close();
	InitBmp();
	//��ʼʶ��
	int Num1[4]={0};    //���н���洢
	int Num2[4]={0};    //���н���洢
	int Num[4]={0};     //�洢���ս�� 
	int Num3[4]={0};    //����5��8�ı��
	int count=0;       //������

	int m=4;                                        //��0��ʼ��
    int x=0;
	int y=0;
	
	for (int n=7; n<fileinfo.biWidth;n++)           //����һ��
	{
		if(rgb[m*fileinfo.biWidth+n].rgbtRed==0 && y<3)
		{
			x=x+1; 
		}
		else
		{
			if(x >=2)
			{
				while(Num1[y]!=0)
				{
					y=y+1;
				}
				Num1[y]=x;
				x=0;
			}
		}
	}
	
    m=fileinfo.biHeight-4;                     //����һ��
    x=0;
	y=0;
	for (int a=7; a<fileinfo.biWidth;a++)                    
	{
		if(rgb[m*fileinfo.biWidth+a].rgbtRed==0 && y<3)
		{
			x=x+1; 
		}
		else
		{  
			if(x >=2)
			{
				while(Num3[y]!=0)
				{
					y=y+1;
				}
				Num3[y]=x;
				x=0;
			}
		}
	}
	
	m=fileinfo.biHeight-5;
    x=0;
	y=0;
	
	for (int c=7; c<fileinfo.biWidth;c++)          //�м�һ��
	{
		if(rgb[m*fileinfo.biWidth+c].rgbtRed==0 && y<3)
		{
			x=x+1; 
		}
		else
		{  
			if(x >=2)
			{
				while(Num2[y]!=0)
				{
					y=y+1;
				}	
				Num2[y]=x;
				x=0;
			}
		}
	}
	
	for(int z=0;z<4;z++)
	{
		if(Num1[z]==3 &&Num3[z]==3)
		{
          Num[z]=0;
		}
        if(Num1[z]==8 &&Num3[z]==3)
		{
          Num[z]=1;
		}
		if(Num1[z]==8 && Num3[z]==5)
		{
          Num[z]=2;
		}
		if(Num1[z]==5 &&Num3[z]==5 && Num2[z]==8)
		{
          Num[z]=3;
		}
		if(Num1[z]==2 &&Num3[z]==2)
		{
          Num[z]=4;
		}
		if(Num1[z]==5 && Num3[z]==8 )
		{
          Num[z]=5;
		}
		if(Num1[z]==4 &&Num3[z]==4 && Num2[z]==6)
		{
          Num[z]=6;
		}
		if(Num1[z]==2 &&Num3[z]==8)
		{
          Num[z]=7;
		}
		if(Num1[z]==5 &&Num3[z]==5 && Num2[z]==7)
		{
          Num[z]=8;
		}
		if(Num1[z]==4 && Num3[z]==4 &&Num2[z]==7)
		{
          Num[z]=9;
		}
	}
	::DeleteFile("temp.bmp");
	CString msg;
	msg.Format("%d%d%d%d",Num[0],Num[1],Num[2],Num[3]);
	MessageBox(msg);

}

int CTest_1Dlg::InitBmp()
{
    HWND hPic=GetDlgItem(IDC_STATIC)->m_hWnd;
	RECT rt;
	::GetClientRect(hPic,&rt);
	HDC hPicDc=::GetWindowDC(hPic);
	HDC hMemDC=CreateCompatibleDC(hPicDc);
	HBITMAP hbitmap=(HBITMAP)LoadImage(GetModuleHandle(NULL),saveFilename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE | LR_CREATEDIBSECTION );
    HBITMAP hOld=(HBITMAP)::SelectObject(hMemDC,hbitmap);
    ::StretchBlt(hPicDc,0,0,rt.right-rt.left,rt.bottom-rt.top,hMemDC,0,0,60,20,SRCCOPY);
    ::SelectObject(hMemDC,hOld);
	::ReleaseDC(hPic,hPicDc);
	return 0;
}
