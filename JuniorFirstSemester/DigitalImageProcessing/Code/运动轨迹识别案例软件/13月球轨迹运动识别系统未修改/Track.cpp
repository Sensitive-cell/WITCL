// Track.cpp : implementation file
//

#include "stdafx.h"
#include "ydpd.h"
#include "Track.h"
#include "ydgj.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Track dialog


Track::Track(CWnd* pParent /*=NULL*/)
	: CDialog(Track::IDD, pParent)
{
	//{{AFX_DATA_INIT(Track)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	this->m_YDGJ.m_Len = 0;
}


void Track::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Track)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Track, CDialog)
	//{{AFX_MSG_MAP(Track)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Track message handlers
//����Ĵ���ʵ������켣�Ļ���
void Track::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	int i;
	CPen Pen;
	//��������
	Pen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	//ѡ�л���
	dc.SelectObject(&Pen);
	CWnd	*Chart;
	CRect m_ChartRect;
	//�õ�����ָ��
	Chart = GetDlgItem(IDC_STATIC_GUIJI);
    //�õ����ڵľ��δ�С
	Chart->GetWindowRect(&m_ChartRect);
	//������Ļ���꣬ʹ֮��ͻ������Ӧ
	ScreenToClient(&m_ChartRect);
	//���������ĵ�
	for(i =0 ;i<this->m_YDGJ.m_Len;i++)
	{
		CPoint po;
		//�õ��������ĵ�ĺ����ꡢ������
		po.x = m_YDGJ.m_Guiji[i].x+40;
		po.y = m_mapheight - m_YDGJ.m_Guiji[i].y+50;
		//����
		dc.SetPixel(po,RGB(255,0,0));
	}
	//��������˶��켣����
	for( i=0;i<this->m_YDGJ.m_Len-1;i++)
	{   //�ƶ�����ʼ��
		dc.MoveTo(m_YDGJ.m_Guiji[i].x+40,m_mapheight - m_YDGJ.m_Guiji[i].y+50);
		//����
		dc.LineTo(m_YDGJ.m_Guiji[i+1].x+40,m_mapheight - m_YDGJ.m_Guiji[i+1].y+50);
	}
	
	Pen.DeleteObject();
	
}
