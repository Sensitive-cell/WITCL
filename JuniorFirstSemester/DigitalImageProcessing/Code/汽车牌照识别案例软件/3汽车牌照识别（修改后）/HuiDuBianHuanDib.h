#ifndef __HuiDuBianHuanDib_H
#define __HuiDuBianHuanDib_H
#include "Cdib.h"

class HuiDuBianHuanDib :public CDib
{
public:
	HuiDuBianHuanDib();
	~HuiDuBianHuanDib();
	
public:
	void Midufenbu(int *nNs_R,int *nNs_G,int *nNs_B);
	void ZhiFangTu(float *tongji,float *tongji2,float *tongji3);
	void GreenZhiFangTu(float *tongji);
	void BlueZhiFangTu(float *tongji);
	void RedZhiFangTu(float *tongji);
	void Fei0();//��ͼ����з���ȡһ����
	void GuDing(int YuZhi);//��ͼ����й̶���ֵ����
	void ShuangYu(int YuZhi1,int YuZhi2,int mode);//˫�̶���ֵ��
	void Chuizhitouying();//��ͼ����д�ֱͶӰ
	void Shuipingtouying();//��ͼ�����ˮƽͶӰ
	void Midufenbu(int* nNs_R);//����ͼ����ܶȷֲ�
	void Pipeibianhuan(BYTE bNum, int *npNu, float *fpPu);//���лҶ�ƥ��任
	int GetWidth();
	void Fenbujunhenghua(  );//���зֲ�����任
	void Chuangkoubianhuan(BYTE bLow, BYTE bUp);//�Ҷȴ��ڱ任
	void Zhexianbianhuan(BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2);//�Ҷ����߱任
	void ZhiFangTu(float* tongji);//�Ҷ�ֱ��ͼ��ʵ
	void FanSeBianHuan();//ͼ��ɫЧ��
	void zishiying();
	void MakegGray() ;
	void bianyuanjiance();
	unsigned char WINAPI myGetMedianNum(unsigned char * bArray, int iFilterLen);
	BOOL WINAPI myMedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
						   int iFilterH, int iFilterW, 
						   int iFilterMX, int iFilterMY,long LB);
	BOOL myHprojectDIB(LPSTR lpDIB, LONG lWidth, LONG lHeight, 
						  int* iTop, int* iBottom);
	BOOL  myVprojectDIB(LPSTR lpDIB, LONG lWidth, LONG lHeight, 
						  int* iLeft, int* iRight) ;
	BOOL myHcut(LPSTR lpDIB, LONG lWidth, LONG lHeight, 
						  int* iTop, int* iBottom);
	BOOL myVcut(LPSTR lpDIB, LONG lWidth, LONG lHeight, 
						  int* r1, int* l1,int* r2, int* l2,int* r3, int* l3,int* r4, int* l4,int* r5, int* l5
						  ,int* r6, int* l6,int* r7, int* l7) ;

protected:
};
#endif