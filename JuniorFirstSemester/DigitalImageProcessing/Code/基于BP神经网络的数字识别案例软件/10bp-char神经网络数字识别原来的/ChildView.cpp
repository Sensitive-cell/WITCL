//����BP����ĺ��������ֵ�ʶ��(noised digital number recognition based on BPnet)
//Developed by �Ի�(H.ZHAO)
//Shandong University,JInan,ChinaPRC
//finished 2004-03-23
//the memory used dynamicly has been optimized ,2004-3-27
//ChildView.cpp : implementation of the CChildView class
#include "stdafx.h"
#include "BPnet.h"
#include "ChildView.h"
#include "matrix_op.h"
#include "math.h"
#include "time.h"
#include <sys/timeb.h>
#define SP1 1.1
#define SP2 0.7
#define SP3 0.5
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
int mark_wb=0;
int errorfilemark=0;//�ļ���������
int dongliangmark=0;//�Ƿ�ʹ�ö������ӵı�־
//��ѵ���������м���������������������Ӧ����
void jiazao()
{   int i,j,k,*np;
    for(i=0;i<Ns/Nsext;i++)
	   for(j=0;j<N_P;j++) 
		  for(k=1;k<Nsext;k++)
			P_samples[i+k*Ns/Nsext][j]=P_samples[i][j];
    //��ѵ����������4%��15%
    for(i=0;i<Ns/Nsext;i++)
	   for(k=1;k<Nsext;k++)
		{	np=Rand2(N_P,int(N_P*(0.04+k*0.03)));//0.04,0.02
	        for(j=0;j<int(N_P*(0.04+k*0.03));j++)
	           P_samples[i+k*Ns/Nsext][np[j]]=(P_samples[i+k*Ns/Nsext][np[j]]==1)?0:1;
			free(np);
		}
    
	for(i=0;i<Ns/Nsext;i++)
		for(j=0;j<N_Out;j++) 
	       for(k=1;k<Nsext;k++)
			 T_output[i+k*Ns/Nsext][j]=T_output[i][j];
}

//����ǰ��Ȩֵfinal��ֵ��ʼ������Ϊinitֵ������initֵΪ��������ĳ�ʼȨֵ��ƫ��
void CChildView::initwb()
{   int i,j;
	for(i=0;i<s1;i++) 
	{   B1_final[i]=B1_init[i];
		for(j=0;j<r;j++) 
			W1_final[i*r+j]=W1_init[i*r+j];
	}
    for(i=0;i<s2;i++) 
	{   B2_final[i]=B2_init[i];
		for(j=0;j<s1;j++) 
			W2_final[i*s1+j]=W2_init[i*s1+j];
	}
}

//����ǰ��Ȩֵfinal�����һ��ʱ����W1��W2��B1��B2���Ա�����Ժ�����㡣���ﶯ̬�������ڴ�
void CChildView::loadwb()
{   int i,j;
    W1=(double*)malloc(s1*r*sizeof(double));
    W2=(double*)malloc(s2*s1*sizeof(double));
	B1=(double*)malloc(s1*1*sizeof(double));
	B2=(double*)malloc(s2*1*sizeof(double));
	for(i=0;i<s1;i++) 
	{   B1[i]=B1_final[i];
		for(j=0;j<r;j++) 
			W1[i*r+j]=W1_final[i*r+j];
	}
    for(i=0;i<s2;i++) 
	{   B2[i]=B2_final[i];
		for(j=0;j<s1;j++) 
			W2[i*s1+j]=W2_final[i*s1+j];
	}
}


//��������ʱ�ڴ�����W1��W2��B1��B2��ֵ�洢�ڵ�ǰȨֵfinal��˲��Ժ�Ϳ���ɾ����ʱ�ڴ������Ż��ڴ�
void CChildView::savewb()
{   int i,j;
	for(i=0;i<s1;i++) 
	{   B1_final[i]=B1[i];
		for(j=0;j<r;j++) 
			W1_final[i*r+j]=W1[i*r+j];
	}
    for(i=0;i<s2;i++) 
	{   B2_final[i]=B2[i];
		for(j=0;j<s1;j++) 
			W2_final[i*s1+j]=W2[i*s1+j];
	}
}

//��point�㴦���ƻҶ�Ϊgray���߳�Ϊarea�ľ���
CChildView::DrawPoint(CPoint point, int gray,int area)
{
   CClientDC dc(this);
   CPen pen;
   pen.CreatePen (PS_NULL,1,RGB(0,0,0));
   dc.SelectObject (&pen);
   CBrush brush(RGB(gray*SP1<255?gray*SP1:255,gray*SP2<255?gray*SP2:255,gray*SP3<255?gray*SP3:255));
   dc.SelectObject (&brush);
   dc.Rectangle (point.x-area/2,point.y -area/2,point.x +area/2,point.y +area/2);
   
}

//�����֣�pָ��64��Ԫ�ص����飬pcenterָ���ڴ��ڿͻ������ĸ��ط�������һ��CPoint�������ֵ
void CChildView::DrawNumber(double p[],CPoint pcenter)
{
	CPoint point(20,20);
	int i,j,gray;
	//double* p=&P_samples[0][0];
	for(i=0;i<8;i++)
	{  for(j=0;j<8;j++)
	   {
	       gray=p[i*8+j]==1?8:252;
		   point.x =pcenter.x +j*6;
		   point.y =pcenter.y +i*6;
	       DrawPoint(point,gray,6);
	   }
	}
}

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{   jiazao();
 	::AfxMessageBox("�������л���BP���������ʶ�����\n\n(1)�밴�ղ˵��ϱ궨�����������ִ��BP�����ѵ����ʶ������!  \n\n(2)��������ǰ����ý�����������������1024��768�ķֱ��ʣ����򲿷���Ϣ������ʾ��ȫ!   \n");
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_COMMAND(ID_BPN_TRAIN, OnBpnTrain)
	ON_COMMAND(ID_DRAW_ERROR, OnDrawError)
	ON_COMMAND(ID_PREWB, OnPrewb)
	ON_COMMAND(ID_DISP_SAMPLES, OnDispSamples)
	ON_COMMAND(ID_BPN_RUN, OnBpnRun)
	ON_COMMAND(ID_BPN_Verify, OnBPNVerify)
	ON_COMMAND(ID_BPTRAIN_Optimal1, OnBPTRAINOptimal1)
	ON_COMMAND(ID_SAVEERROR, OnSaveerror)
	ON_COMMAND(ID_SAVEWEIGHT, OnSaveweight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	GetClientRect(&rect);	
	// Do not call CWnd::OnPaint() for painting messages
}


//��60����֪����������ѵ��BP���硣���У�10�������Ǵ�����δ����ģ�50������������
void CChildView::OnBpnTrain() 
{   dongliangmark=0;  //�������
	errorfilemark++;
	if(mark_wb==0) OnPrewb();//���������ʼȨֵ��ƫ�ô����init
	int i,current,loops,trainok;
	int mark[Ns];//ѵ���ɹ���־
	//numE=(double*)malloc(N_Err*sizeof(double));//������N_Err��ľ������
	/////////////////////////////////////////////////////////////////////
	current=loops=trainok=0;
	for(i=0;i<N_Err;i++) numE[i]=0;
	for(i=0;i<Ns;i++) mark[i]=0;
	initwb();//��ʼ��Ȩֵ��ƫ�ã���init��ֵ��final��
	/////////////////////////////////////////////////////////////////////
	CClientDC dc(this);
	dc.Rectangle (rect);
	CString str;
	CFont font;
	font.CreatePointFont (350,_TEXT("������"));
	dc.SelectObject (&font);
	dc.DrawText (_T("����ѵ������...���Ե�...."),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	double* temp[15];
	CTime startTime = CTime::GetCurrentTime();
	struct _timeb timebuffer;
	int mills1,mills2;
    _ftime(&timebuffer);
	mills1=timebuffer.millitm;
step2:
	loadwb();//��final��ֵ���·����ڴ�ռ��W1,W2,B1,B2
	current=loops%(Nsext*10);
	P=P_samples[current];//currentΪ0��9����ѭ��ָ������0��9�ı�׼��������
	T=T_output[current];
	/******************************�������򴫲���Ϣ********************************/
	N1=MatrixAdd((temp[0]=MatrixMul(W1,P,s1,r,1)),B1,s1,1);  //N1: s1��1
	A1=f1(N1,s1);//f1���Զ���ĺ������߱���ʸ����������Ĺ���
	N2=MatrixAdd((temp[1]=MatrixMul(W2,A1,s2,s1,1)),B2,s2,1);//N2: s2��1
	A2=f2(N2,s2);
	/******************************���򴫲����ļ���******************************/
	E=MatrixSub(T,A2,s2,1);//T��A2��EΪs2��1
	numE[loops]=0;	//ע�⣬��ʱ������һ�����ˣ��Ǹ�����
	for(i=0;i<s2;i++)
        numE[loops]=numE[loops]+E[i]*E[i];
	//-------����S2��S1��ע��˳��
	S2=MatrixMulNum((temp[3]=MatrixMul((temp[2]=F2(N2,s2)),E,s2,s2,1)),s2,1,-2.0);
	S1=MatrixMul(temp[6]=(MatrixMul((temp[4]=F1(N1,s1)),(temp[5]=MatrixInver(W2,s2,s1)),s1,s1,s2)),S2,s1,s2,1);
	//�����������Ԫ��Ȩֵ��ƫ��
	dW2=MatrixMul(S2,(temp[7]=MatrixInver(A1,s1,1)),s2,1,s1);//dW2: s2��s1
	dW2=MatrixMulNum((temp[8]=dW2),s2,s1,-learnspeed);
	W2=MatrixAdd((temp[9]=W2),dW2,s2,s1);
	dB2=MatrixMulNum(S2,s2,1,-learnspeed);
	B2=MatrixAdd((temp[10]=B2),dB2,s2,1);
	//����������ԪȨֵ��ƫ��
	dW1=MatrixMul(S1,(temp[11]=MatrixInver(P,r,1)),s1,1,r);//dW1: s1��r
    dW1=MatrixMulNum((temp[12]=dW1),s1,r,-learnspeed);
	W1=MatrixAdd((temp[13]=W1),dW1,s1,r);
	dB1=MatrixMulNum(S1,s1,1,-learnspeed);
	B1=MatrixAdd((temp[14]=B1),dB1,s1,1);
    dc.DrawText (_T("����ѵ������...���Ե�...."),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	TRACE("%d loops, error is: %f  \n",loops,numE[loops]);
	savewb();//����ʱ�ռ�W��B�е����ݴ���final
	//�ͷŶ�̬������ڴ���
	free(W1);free(B1);free(N1);free(A1);free(dW1);free(dB1);free(S1);
	free(W2);free(B2);free(N2);free(A2);free(dW2);free(dB2);free(S2);
	for(i=0;i<15;i++) free(temp[i]);
	if(loops>N_Err) goto step3;
	if(numE[loops]>error_accept)  //˵����������Ҫ�󡣼�������
	{   mark[current]=0; //��֤��Чλ���¼���
		++loops;
	    goto step2;
	}
	else mark[current]=1;//ִ�е��˲�˵���ڸôε����У�������Ҫ�󣬼ǹ�һ��
	trainok=0;
	for(i=0;i<Ns;i++)
		trainok+=mark[i];
	if(trainok>=Ns)
	{   //ѵ�����Խ�����;
	    goto step3;
	}
	else {  ++loops; goto step2;}
step3:
	learncount=loops-1;//�����ܵ�ѧϰ�����������ȫ�ֱ���
    dc.Rectangle (rect);
	dc.TextOut (300,150,_T("ѵ����ɣ�"));
	str.Format ("%d",learncount);
	dc.TextOut (80,400,"�ܹ�ѵ���ˣ�");
	dc.TextOut (360,400,str);
	dc.TextOut (490,400,"��");
	str.Format ("%f",numE[loops-1]);
	dc.TextOut (80,500,"������");
	dc.TextOut (340,500,str);
	stre=str;

	//����ѵ������ʱ��
	CTime endTime = CTime::GetCurrentTime();
	CTimeSpan elapsedTime = endTime - startTime;
    _ftime(&timebuffer);
	mills2=timebuffer.millitm;
	long int timeused;
	timeused=elapsedTime.GetTotalSeconds();
	if (mills1<=mills2)
		timeused=timeused*1000+(mills2-mills1);//������ѵ������ʱ�任��ɺ���
	else
		timeused=(timeused-1)*1000+(mills1-mills2);
	strt.Format ("ѵ����������ʱ�䣺%d ����",timeused);

    ::AfxMessageBox(strt); 
}

//f1��f2Ϊ�����
double* CChildView::f1(double A[],int m)
{   int i;
    double *B=NULL;
	B=(double*)malloc(m*1*sizeof(double));//B��һ��������
	for(i=0;i<m;i++)
		B[i]=(double)1/(1+exp(-A[i]));
	return B;
}

double* CChildView::f2(double A[],int m)
{   int i;
    //double c=1.0;
    double *B=NULL;
	B=(double*)malloc(m*1*sizeof(double));
	for(i=0;i<m;i++)
		B[i]=A[i];
	return B;
}

//F1��F2Ϊ��������󵼣�������ʽ��
double* CChildView::F1(double N[],int m)
{   int i,j;
    double *B=NULL;
	B=(double*)malloc(m*m*sizeof(double));
	for(i=0;i<m;i++)
		for(j=0;j<m;j++)
		   B[i*m+j]=0;
	for(i=0;i<m;i++)
		B[i*m+i]=exp(-N[i])/((1+exp(-N[i]))*(1+exp(-N[i])));
	return B;
}

double* CChildView::F2(double N[],int m)
{   int i,j;
    double *B=NULL;
	B=(double*)malloc(m*m*sizeof(double));
	for(i=0;i<m;i++)
		for(j=0;j<m;j++)
		   B[i*m+j]=0;
	for(i=0;i<m;i++)
		B[i*m+i]=1;
	return B;
}

//����ŷʽ����ƥ��BP����������������ĸ�����
int CChildView::TargetTest(double A[])
{   double a,min=100;
    int index=-1,i,j;
	double* Temp;
    for(i=0;i<Ns/Nsext;i++)
	{   Temp=MatrixSub(A,T_output[i],s2,1);
		a=0;
	    for(j=0;j<s2;j++)
		    a=a+Temp[j]*Temp[j];
        if(a<min)
		{
			min=a;
			index=i;
		}
		free(Temp);
	}
	return index;
}

//����ѧϰ�������
void CChildView::OnDrawError() 
{ 	CClientDC dc(this);	
	dc.Rectangle (rect);
	CPen pen(PS_SOLID,3,RGB(105,90,170));
	dc.SelectObject(&pen);
	int n,i;
	double numEsum[2000]={0};
	for(n=0;n<learncount;n++)
	    numEsum[(int)(n/10)]+=numE[n];
	for(i=0;i<int(learncount/10);i++)
	{   dc.MoveTo (30+i ,(int)((float)(rect.bottom-rect.top)*0.98));
	    dc.LineTo (30+i ,(int)((float)(rect.bottom-rect.top)*0.98-15*numEsum[i]));
	}
	//Invalidate(TRUE);
}


//������ó�ʼȨֵ��ƫ�á�����һ��ȡ��1��1֮��������
//��ʼ��Ȩֵ��ƫ�ô����init
void CChildView::OnPrewb() 
{   mark_wb=1;//��ʾ�Ѿ������������ʼȨֵ��ƫ��
	W1=MatrixRand(s1,r);  B1=MatrixRand(s1,1);
	W2=MatrixRand(s2,s1); B2=MatrixRand(s2,1);	
	int i,j;
	for(i=0;i<s1;i++) 
	{   B1_init[i]=B1[i];
		for(j=0;j<r;j++) 
			W1_init[i*r+j]=W1[i*r+j];
	}
    for(i=0;i<s2;i++) 
	{   B2_init[i]=B2[i];
		for(j=0;j<s1;j++) 
			W2_init[i*s1+j]=W2[i*s1+j];
	}
	free(W1);
	free(W2);
	free(B1);
	free(B2);
    CClientDC dc(this);
	CString str;
	CFont font;
	font.CreatePointFont (300,_TEXT("������"));
	dc.SelectObject (&font);
	dc.Rectangle (rect);
	dc.DrawText (_T("�������������ʼȨֵ��ƫ��.....OK��"),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
}


//��ʾ��׼��0��9�������ݣ���δ�������ı�׼�������ݣ�
void CChildView::OnDispSamples() 
{
	CClientDC dc(this);	
	dc.Rectangle (rect);
	CPen pen(PS_SOLID,3,RGB(105,90,170));
	dc.SelectObject(&pen);
	CPoint pcenter(20,20);
	int i;
	CFont font1;
	font1.CreatePointFont (250,_T("������"));
	dc.SelectObject (&font1);
	dc.TextOut (30,80,"��������Ϊ����0��9�ı�׼�����������������ݣ�");
	for(i=0;i<10;i++)
	{   pcenter.x =30+i*90;
	    pcenter.y =150;
	    DrawNumber(P_samples[i],pcenter);
	}

}


//���Ѿ�ѵ����ϵ�BP����Ӧ���ڣ���������δ֪���ֵ�ʶ��
void CChildView::OnBpnRun() 
{   int targetindex,i=5,j,k,sbl=0,*np;
    double* temp1[40];	double* temp2[40];	double* temp3[40];
	double* temp4[40];	double* temp5[40];	double* temp6[40];
	CClientDC dc(this);
	dc.Rectangle (rect);
	CString str;
	CPoint pcenter(40,40);
	CFont font;
	font.CreatePointFont (120,_TEXT("������"));
	dc.SelectObject (&font);
	dc.Rectangle (rect);
	dc.TextOut(129,20,"--------------Ӧ��BP�����������ʶ��ʶ�����Ϊ��������40��δ֪���ݣ���-------------------");	
	for(i=0;i<40;i++)
	   for(j=0;j<N_P;j++)	
		   P_withnoise[i][j]=P_samples[i%10][j];
	for(i=0;i<10;i++)
	   for(k=0;k<=4;k++)
		{	np=Rand2(N_P,k+2);
	        for(j=0;j<k+2;j++)
	           P_withnoise[i+k*10][np[j]]=(P_withnoise[i+k*10][np[j]]==1)?0:1;
		}

	for(i=0;i<40;i++)//����40������
	{   P=P_withnoise[i];
		temp2[i]=N1=MatrixAdd((temp1[i]=MatrixMul(W1_final,P,s1,r,1)),B1_final,s1,1);  //N1: s1��1
		temp3[i]=A1=f1(N1,s1);
		temp5[i]=N2=MatrixAdd((temp4[i]=MatrixMul(W2_final,A1,s2,s1,1)),B2_final,s2,1);//N2: s2��1
		temp6[i]=A2=f2(N2,s2);	
		targetindex=TargetTest(A2);    
	    pcenter.x =40+(i%(Ns/Nsext))*98;
	    pcenter.y =80+95*int(i/(Ns/Nsext));
		DrawNumber(P,pcenter);
		dc.TextOut(pcenter.x-15,pcenter.y+50,"ʶ����:");
	    str.Format("%d ",targetindex);
	    dc.TextOut (pcenter.x+60,pcenter.y+50,str);
		if(i%10==targetindex) sbl++;
		//TRACE("It is number %d ! %s!",targetindex,(i==targetindex)?"Right":"Wrong");
		//if(i==targetindex) {sbl++;TRACE("\n");}
	    //else TRACE(" Ӧ���� %d \n",i);
	}
    dc.TextOut (45,pcenter.y+160,"��ǰBP�����δ֪���֣���3��15%�������������ʶ���ʴﵽ��");	
	str.Format ("%3.2f",100*(double)sbl/40);
	dc.TextOut (490,pcenter.y+160,str);
	dc.TextOut (537,pcenter.y+160,"%");
    for(i=0;i<40;i++)
	{ free(temp1[i]);free(temp2[i]);free(temp3[i]);free(temp4[i]);free(temp5[i]);free(temp6[i]);}
	//TRACE("����Ӧ�ò��ԣ�ʶ����Ϊ: �ٷ�֮%2.2f\n ",Nsext*100*(double)sbl/(double)Ns);
}


//BP�����У�飺ʹ����֪����(���������롢����)���з��࿴�Ƿ���ȷ
void CChildView::OnBPNVerify() 
{   int targetindex,i,sbl=0,k;
    double* temp1[Ns];	double* temp2[Ns];	double* temp3[Ns];
	double* temp4[Ns];	double* temp5[Ns];	double* temp6[Ns];
	CClientDC dc(this);
	dc.Rectangle (rect);
	CString str;
	CPoint pcenter(40,40);
	CFont font;
	font.CreatePointFont (120,_TEXT("������"));
	dc.SelectObject (&font);
	dc.Rectangle (rect);
	dc.TextOut(30,20,"------------У����-----------------");	
	dc.TextOut(40,50,"��һ�в��������������֣������зֱ������3��15%�����������");
	for(i=0;i<Ns;i++)
	{  k=i-10*int(i/10);
       P=P_samples[i];
	   temp2[i]=N1=MatrixAdd((temp1[i]=MatrixMul(W1_final,P,s1,r,1)),B1_final,s1,1);  //N1: s1��1
	   temp3[i]=A1=f1(N1,s1);
       temp5[i]=N2=MatrixAdd((temp4[i]=MatrixMul(W2_final,A1,s2,s1,1)),B2_final,s2,1);//N2: s2��1
	   temp6[i]=A2=f2(N2,s2);	
	   targetindex=TargetTest(A2);    
	   TRACE("It is number %d ! %s!",targetindex,(k==targetindex)?"Right":"Wrong");
	   if(k==targetindex) {sbl++;TRACE("\n");}
	   else TRACE(" Ӧ���� %d \n",k);
	   pcenter.x =40+(i%10)*98;
	   pcenter.y =80+90*int(i/10);
	   DrawNumber(P,pcenter);
	   dc.TextOut(pcenter.x-15,pcenter.y+50,"ʶ����:");
	   str.Format("%d ",k);
	   dc.TextOut (pcenter.x+60,pcenter.y+50,str);
	}	
	TRACE("ʶ����Ϊ: �ٷ�֮%2.2f\n ",100*(double)sbl/(double)Ns);
    dc.TextOut (60,pcenter.y+100,"��ǰ�������֪���ݵ�ʶ�����Ѵﵽ��");
	str.Format ("%3.2f",100*(double)sbl/(double)Ns);
	dc.TextOut (360,pcenter.y+100,str);
	dc.TextOut (400,pcenter.y+100," %");
	for(i=0;i<Ns;i++)
	{ free(temp1[i]);free(temp2[i]);free(temp3[i]);free(temp4[i]);free(temp5[i]);free(temp6[i]);}
}

//���붯�����ӵ�ѵ������
void CChildView::OnBPTRAINOptimal1() 
{   dongliangmark=1;
	errorfilemark++;
	if(mark_wb==0) OnPrewb();//���������ʼȨֵ��ƫ�ô����init
	int i,j,current,loops,trainok;
	int mark[Ns];//ѵ���ɹ���־
	//numE=(double*)malloc(N_Err*sizeof(double));//������N_Err��ľ������
	/////////////////////////////////////////////////////////////////////
	current=loops=trainok=0;
	for(i=0;i<N_Err;i++) numE[i]=0;
	for(i=0;i<Ns;i++) mark[i]=0;
	initwb();//��ʼ��Ȩֵ��ƫ�ã���init��ֵ��final��
	/////////////////////////////////////////////////////////////////////
	CClientDC dc(this);
	dc.Rectangle (rect);
	CString str;
	CFont font;
	font.CreatePointFont (350,_TEXT("������"));
	dc.SelectObject (&font);
	dc.DrawText (_T("����ѵ������...���Ե�...."),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	double* temp[23];
	double* dw1=(double*)malloc(s1*r*sizeof(double));
	double* dw2=(double*)malloc(s2*s1*sizeof(double));
	double* db1=(double*)malloc(s1*1*sizeof(double));
	double* db2=(double*)malloc(s2*1*sizeof(double));
	for(i=0;i<s1;i++)
	{   db1[i]=0.1;
	   for(j=0;j<r;j++)
		  dw1[i*r+j]=0.1;
	}
	for(i=0;i<s2;i++)
	{   db2[i]=0.1;
	   for(j=0;j<s1;j++)
		  dw2[i*s1+j]=0.1;
	}
	CTime startTime = CTime::GetCurrentTime();
	struct _timeb timebuffer;
	int mills1,mills2;
    _ftime(&timebuffer);
	mills1=timebuffer.millitm;
step2:
	loadwb();//��final��ֵ���·����ڴ�ռ��W1,W2,B1,B2
	current=loops%(Nsext*10);
	P=P_samples[current];//currentΪ0��9����ѭ��ָ������0��9�ı�׼��������
	T=T_output[current];
	/******************************�������򴫲���Ϣ********************************/
	N1=MatrixAdd((temp[0]=MatrixMul(W1,P,s1,r,1)),B1,s1,1);  //N1: s1��1
	A1=f1(N1,s1);//f1���Զ���ĺ������߱���ʸ����������Ĺ���
	N2=MatrixAdd((temp[1]=MatrixMul(W2,A1,s2,s1,1)),B2,s2,1);//N2: s2��1
	A2=f2(N2,s2);
	/******************************���򴫲����ļ���******************************/
	E=MatrixSub(T,A2,s2,1);//T��A2��EΪs2��1
	numE[loops]=0;	//ע�⣬��ʱ������һ�����ˣ��Ǹ�����
	for(i=0;i<s2;i++)
        numE[loops]=numE[loops]+E[i]*E[i];
	//-------����S2��S1��ע��˳��
	S2=MatrixMulNum((temp[3]=MatrixMul((temp[2]=F2(N2,s2)),E,s2,s2,1)),s2,1,-2.0);
	S1=MatrixMul(temp[6]=(MatrixMul((temp[4]=F1(N1,s1)),(temp[5]=MatrixInver(W2,s2,s1)),s1,s1,s2)),S2,s1,s2,1);
	//�����������Ԫ��Ȩֵ��ƫ��
	dW2=MatrixMul(S2,(temp[7]=MatrixInver(A1,s1,1)),s2,1,s1);//dW2: s2��s1
	dW2=MatrixMulNum((temp[8]=dW2),s2,s1,(rmobp-1)*learnspeed);
	temp[15]=MatrixMulNum(dw2,s2,s1,rmobp);
	temp[16]=MatrixAdd(temp[15],dW2,s2,s1);
	W2=MatrixAdd((temp[9]=W2),temp[16],s2,s1);
	dB2=MatrixMulNum(S2,s2,1,(rmobp-1)*learnspeed);
	temp[17]=MatrixMulNum(db2,s2,1,rmobp);
	temp[18]=MatrixAdd(temp[17],dB2,s2,1);
	B2=MatrixAdd((temp[10]=B2),temp[18],s2,1);
	//����������ԪȨֵ��ƫ��
	dW1=MatrixMul(S1,(temp[11]=MatrixInver(P,r,1)),s1,1,r);//dW1: s1��r
    dW1=MatrixMulNum((temp[12]=dW1),s1,r,(rmobp-1)*learnspeed);
	temp[19]=MatrixMulNum(dw1,s1,r,rmobp);
	temp[20]=MatrixAdd(temp[19],dW1,s1,r);
	W1=MatrixAdd((temp[13]=W1),temp[20],s1,r);
	dB1=MatrixMulNum(S1,s1,1,(rmobp-1)*learnspeed);
	temp[21]=MatrixMulNum(db1,s1,1,rmobp);
	temp[22]=MatrixAdd(temp[21],dB1,s1,1);
	B1=MatrixAdd((temp[14]=B1),temp[22],s1,1);
    dc.DrawText (_T("����ѵ������...���Ե�...."),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	TRACE("%d loops, error is: %f  \n",loops,numE[loops]);
	savewb();//����ʱ�ռ�W��B�е����ݴ���final
	for(i=0;i<s1;i++)
	{   db1[i]=dB1[i];
	   for(j=0;j<r;j++)
		  dw1[i*r+j]=dW1[i*r+j];
	}
	for(i=0;i<s2;i++)
	{   db2[i]=dB2[i];
	   for(j=0;j<s1;j++)
		  dw2[i*s1+j]=dW2[i*s1+j];
	}
	//�ͷŶ�̬������ڴ���
	free(W1);free(B1);free(N1);free(A1);free(dB1);free(S1);free(dW1);
	free(W2);free(B2);free(N2);free(A2);free(dB2);free(S2);free(dW2);
	for(i=0;i<23;i++) free(temp[i]);
	if(loops>N_Err) goto step3;
	if(numE[loops]>error_accept)  //˵����������Ҫ�󡣼�������
	{   mark[current]=0; //��֤��Чλ���¼���
		++loops;
	    goto step2;
	}
	else mark[current]=1;//ִ�е��˲�˵���ڸôε����У�������Ҫ�󣬼ǹ�һ��
	trainok=0;
	for(i=0;i<Ns;i++)
		trainok+=mark[i];
	if(trainok>=Ns)
	{   //ѵ�����Խ�����;
	    goto step3;
	}
	else {  ++loops; goto step2;}
step3:
	learncount=loops-1;//�����ܵ�ѧϰ�����������ȫ�ֱ���
    dc.Rectangle (rect);
	dc.TextOut (300,150,_T("ѵ����ɣ�"));
	str.Format ("%d",learncount);
	dc.TextOut (80,400,"�ܹ�ѵ���ˣ�");
	dc.TextOut (360,400,str);
	dc.TextOut (490,400,"��");
	str.Format ("%f",numE[loops-1]);
	dc.TextOut (80,500,"������");
	dc.TextOut (340,500,str);
	stre=str;
	//����ѵ������ʱ��
	CTime endTime = CTime::GetCurrentTime();
	CTimeSpan elapsedTime = endTime - startTime;
    _ftime(&timebuffer);
	mills2=timebuffer.millitm;
	long int timeused;
	timeused=elapsedTime.GetTotalSeconds();
	if (mills1<=mills2)
		timeused=timeused*1000+(mills2-mills1);//������ѵ������ʱ�任��ɺ���
	else
		timeused=(timeused-1)*1000+(mills1-mills2);
	strt.Format ("���붯�����Ӻ�ѵ����������ʱ��Ϊ��%d ����    (��ʼȨֵ����������)",timeused);
    ::AfxMessageBox(strt); 
}

void CChildView::OnSaveerror() 
{
	//д����ļ���ѵ�������е����
	int i;
	//errorfilemark++;
	CString filename;
	CString str;
	filename.Format ("ѵ������¼%d.txt",errorfilemark);
	CFile file (filename, CFile::modeReadWrite|CFile::modeCreate);
	CString str_file;
	char buffer[2]={13,10};
	if(dongliangmark==0) str_file.Format ("ѵ����������¼��û�м��붯�����ӣ���");
	else str_file.Format ("ѵ����������¼��ʹ���˶������ӣ���");
	file.Write (str_file, str_file.GetLength());
    file.Write (buffer,sizeof(buffer));
	for(i=0;i<learncount;i++)
	{
		str_file.Format ("loos%5d,���Ϊ: %4.4f",i+1,numE[i]);
		//str_file.GetLength();
		file.Write (str_file, str_file.GetLength());
		file.Write (buffer,sizeof(buffer));
	}
	str_file.Format ("ѵ�����!");
	file.Write (str_file,str_file.GetLength());	
	file.Write (buffer,sizeof(buffer));
	file.Write (strt,strt.GetLength ());
	file.Write (buffer,sizeof(buffer));
	str_file.Format ("������ ");
	file.Write (str_file,str_file.GetLength ());
	file.Write (stre,stre.GetLength ());
	file.Close ();
	str.Format ("ѵ�����������Ϣ�����: ѵ������¼%d.txt ��",errorfilemark);
	::AfxMessageBox(str);
}

void CChildView::OnSaveweight() 
{
	int i,j;
	//errorfilemark++;
	CString filename;
	CString str;
	filename.Format ("��������Ȩֵ%d.txt",errorfilemark);
	CFile file (filename, CFile::modeReadWrite|CFile::modeCreate);
	CString str_file;
	char buffer[2]={13,10};
	str_file.Format ("����ѵ�����ս���������������Ȩֵ��ƫ�ã�");
	file.Write (str_file, str_file.GetLength());
    file.Write (buffer,sizeof(buffer));
	str_file.Format ("*************************����ģ�*****************************");
	file.Write (str_file, str_file.GetLength());
    file.Write (buffer,sizeof(buffer));
	for(i=0;i<s1;i++)
	   for(j=0;j<r;j++)
	   {   str_file.Format ("W1(%2d,%2d): %3.4f",i,j,W1_final[i*r+j]);
		   file.Write (str_file, str_file.GetLength());
		   file.Write (buffer,sizeof(buffer));
	   }
    for(i=0;i<s1;i++)
	   {   str_file.Format ("B1(%d): %3.4f",i,B1_final[i]);
		   file.Write (str_file, str_file.GetLength());
		   file.Write (buffer,sizeof(buffer));
	   }
    file.Write (buffer,sizeof(buffer));    file.Write (buffer,sizeof(buffer));
	str_file.Format ("*************************�����ģ�*****************************");
	file.Write (str_file, str_file.GetLength());
    file.Write (buffer,sizeof(buffer));
    for(i=0;i<s2;i++)
	   for(j=0;j<s1;j++)
	   {   str_file.Format ("W2(%2d,%2d): %3.4f",i,j,W2_final[i*s1+j]);
		   file.Write (str_file, str_file.GetLength());
		   file.Write (buffer,sizeof(buffer));
	   }
	for(i=0;i<s2;i++)
	   {   str_file.Format ("B2(%d): %3.4f",i,B2_final[i]);
		   file.Write (str_file, str_file.GetLength());
		   file.Write (buffer,sizeof(buffer));
	   }
	file.Write (buffer,sizeof(buffer));
	str_file.Format ("����˳���Դ���룬����������ϵ��bugzhao@sohu.com ");
	file.Write (str_file, str_file.GetLength());
    file.Write (buffer,sizeof(buffer));
	str.Format ("ѵ�������������Ȩֵ��ƫ�������: ��������Ȩֵ%d.txt �� ",errorfilemark);
	::AfxMessageBox(str);	
}
