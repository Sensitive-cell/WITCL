// recognise_yanView.cpp : implementation of the CRecognise_yanView class
//

#include "stdafx.h"
#include "recognise_yan.h"
#include "recognise_yanDoc.h"
#include "recognise_yanView.h"
#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecognise_yanView

IMPLEMENT_DYNCREATE(CRecognise_yanView, CView)

BEGIN_MESSAGE_MAP(CRecognise_yanView, CView)
	//{{AFX_MSG_MAP(CRecognise_yanView)
	ON_COMMAND(ID_OPEN_IMAGE, OnOpenImage)
	ON_COMMAND(ID_RECOGNISE_IMAGE, OnRecogniseImage)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecognise_yanView construction/destruction

CRecognise_yanView::CRecognise_yanView()
{
	// TODO: add construction code here

}

CRecognise_yanView::~CRecognise_yanView()
{
}

BOOL CRecognise_yanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRecognise_yanView drawing

void CRecognise_yanView::OnDraw(CDC* pDC)
{
	CRecognise_yanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);    // TODO: add draw code for native data here
	if(m_dib.m_bLoaded==true)  //�ж��Ƿ����ͼ��
	{
		//��ȡͼ���͸�
		int nw=m_dib.GetDIBWidth();
		int nh=m_dib.GetDIBHeight();
		// ��ʾͼ�񣨾���Ĳ�����CDIB��ĸú���˵����
		m_dib.ShowDIB(pDC,10,10,nw,nh,m_dib.m_pDIBData,m_dib.m_pBMI);
		m_dib.ShowDIB(pDC,400,10,nw,nh,m_dib.m_pDumpDIBData,m_dib.m_pBMI);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRecognise_yanView printing

BOOL CRecognise_yanView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRecognise_yanView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRecognise_yanView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CRecognise_yanView diagnostics

#ifdef _DEBUG
void CRecognise_yanView::AssertValid() const
{
	CView::AssertValid();
}

void CRecognise_yanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRecognise_yanDoc* CRecognise_yanView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRecognise_yanDoc)));
	return (CRecognise_yanDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRecognise_yanView message handlers

void CRecognise_yanView::OnOpenImage() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	static char szFilter[]="BMP�ļ�(*.bmp)|*.bmp||";  //��������ļ�������
	CFileDialog dlg(TRUE,"bmp",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);//�����ļ��Ի������
	CString filename;
	int ret=dlg.DoModal();  //���д��ļ��Է���
	 if(ret==IDOK)
	   {		
			filename=dlg.GetFileName();     //��ȡ��ѡ��ͼ���·��
			m_dib.LoadFromFile(filename);   //����ͼ��
			if(!m_dib.m_bLoaded)            //�ж��Ƿ����ͼ��ɹ�
			{
				AfxMessageBox("ͼ��򲻿�");
				return;
			}
		}
	Invalidate(1);	//ˢ����Ļ
}

void CRecognise_yanView::OnRecogniseImage() 
{
	// TODO: Add your command handler code here
	if(!m_dib.m_bLoaded)                         //�ж�ͼ���Ƿ�򿪣���ʧ�ܣ��򵯳��Ի����˳�����
	{
		AfxMessageBox("opening the image is not success!");
		return;
	}
	int w1=m_dib.GetDIBWidth();  //��ȡԴͼ��Ŀ�
	int h1=m_dib.GetDIBHeight(); //��ȡԴͼ��ĸ�
	BYTE* temp=new BYTE[w1*h1];   //������ʱ�ڴ�
	memcpy(temp,m_dib.m_pdata,w1*h1); //��Դͼ���Ƶ���ʱ���ٵ�temp��
	int thr=150; //���巧ֵ����ֵΪ150
	int i,j,k,q,n,m;
	//��ԭͼ��ÿһ�����ؽ��ж�ֵ������
	for(i=0;i<h1;i++) 
		for(j=0;j<w1;j++)
		{
			k=i*w1+j;  //ͼ���i�С���j�е�����
			if(temp[k]>thr)
				temp[k]=255;
			else
				temp[k]=0;
		}
	char sq[17][10]={"A","2","3","4","5","6","7","8","9","10","J","Q","K","����","����","÷��","����"};//����ÿ��Ԫ����������8���ֽ�
 
	CString filenames0[17]={"A.bmp","2.bmp","3.bmp","4.bmp","5.bmp","6.bmp","7.bmp","8.bmp","9.bmp","10.bmp","J.bmp",
							"Q.bmp","K.bmp","����.bmp","����.bmp","÷��.bmp","����.bmp"}; //����ģ���ļ�����
	double MR[17]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};  //��ʼ��������ƶȵ�ֵ��������ֵΪ0
	for(q=0;q<17;q++)
	{
		m_dib.LoadFromFile(filenames0[q]);   //����ͼ��
		if(!m_dib.m_bLoaded)  //�ж�ģ����Ƿ�ɹ���û������ֶԻ��򣬲��˳�����
		{
			AfxMessageBox("open the image is error!");
			return;
		}
		int w2=m_dib.GetDIBWidth();  //��ȡģ��ͼ��Ŀ�
		int h2=m_dib.GetDIBHeight(); //��ȡģ��ͼ��ĸ�
		for(i=0;i<h2;i++)             //��ԭͼ��ÿһ�����ؽ��ж�ֵ������
			for(j=0;j<w2;j++)
			{
				k=i*w2+j;             //ͼ���i�С���j�е�����
				if(m_dib.m_pdata[k]>thr)
					m_dib.m_pdata[k]=255;
				else
					m_dib.m_pdata[k]=0;
			}
		double T=0;
		for(i=0;i<h2;i++)
			for(j=0;j<w2;j++)
			{
				k=i*w2+j;
				T=T+m_dib.m_pdata[k]*m_dib.m_pdata[k];  //����ģ�����ص�ƽ����
			}
		double R=0.0;
		for(i=0;i<(2*h1/5)+1-h2;i++)    //ɨ���з�Χ(��Ե�����)
		{
			for(j=w1/2;j<w1+1-w2;j++)  //ɨ���з�Χ(��Ե�����)
			{
				double ST=0;
				double S=0;
				for(n=0;n<h2;n++)
					for(m=0;m<w2;m++)
					{
						ST=ST+m_dib.m_pdata[n*w2+m]*temp[(i+n)*w1+(j+m)];
						S=S+temp[(i+n)*w1+(j+m)]*temp[(i+n)*w1+(j+m)];
					}
				R=ST/(sqrt(S)*sqrt(T));   //��ÿ��ģ�����������R
				if(R>MR[q])
					MR[q]=R;
			}
		}
	}
	double R1=0;
	int F;
	for(n=0;n<13;n++)   //�жϳ�ֽ�Ƶĵ���
	{
		if(MR[n]>R1)
		{
			R1=MR[n];
			F=n;
		}
	}
	double R2=0;
	int G;
	for(m=13;m<17;m++)   //�жϳ�ֽ�ƵĻ�ʽ
	{
		if(MR[m]>R2)
		{
			R2=MR[m];
			G=m;
		}
	}
	MessageBox(sq[G],sq[F]); //�������ڣ���ʾ���
	return;
	delete temp;
	delete m_dib.m_pdata;
}
