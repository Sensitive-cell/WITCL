#if !defined(AFX_WAVELETPJDLG_H__352E3E91_853B_4310_A40E_EA9BFE9A8C52__INCLUDED_)
#define AFX_WAVELETPJDLG_H__352E3E91_853B_4310_A40E_EA9BFE9A8C52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaveletPJDlg.h : header file
//
#include "CDib.h"

/////////////////////////////////////////////////////////////////////////////
// CWaveletPJDlg dialog

class CWaveletPJDlg : public CDialog
{
// Construction
public:
	CWaveletPJDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWaveletPJDlg)
	enum { IDD = IDD_DIALOG2 };
	int		m_col;
	//}}AFX_DATA
    CDib m_dib1,m_dib2;

    long int Height1,Width1,m_bytewidth1;
    long int Height2,Width2,m_bytewidth2;

	BYTE  *m_pdata1,*m_pdata2;//DIBͼ������ָ��

	double *pDatewavelet1;
	double *pDatewavelet2;
    //С���任
	void WaveletTran(CDib m_dib,double *Source,int level,long int Height,long int Width,long int m_bytewidth);
    //С����ƴ��
	void WaveletPJACT(double *total,double *first,double *second,int flag);

    //����ͼС���任��ֱ�����Ĳ��֣���˲�������ƴ��
	double *Pdib_LeftUp;
	double *Pdib_LeftDown;
	double *Pdib_RightUp;
	double *Pdib_RightDown;
	//С����ƴ�ӽ��
	double *Pdib;
    //1/4����ƴ�Ӻ���¿�Ⱥ��¸߶�
	int NewHeight1,NewWidth1;
	int NewHeight2,NewWidth2;
	int NewHeight3,NewWidth3;
	int NewHeight4,NewWidth4;
	//С����ƴ�Ӻ�ĸ߶ȺͿ��
	int Height,Width;

    //����ƴ�ӽ��
	double *PdibSuccess;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveletPJDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWaveletPJDlg)
	afx_msg void OnOpenImage1();
	afx_msg void OnOpenImage2();
	afx_msg void OnImg1WaveletTran();
	afx_msg void OnImg2WaveletTran();
	afx_msg void OnWaveletPJ();
	afx_msg void OnReConstruct();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVELETPJDLG_H__352E3E91_853B_4310_A40E_EA9BFE9A8C52__INCLUDED_)
