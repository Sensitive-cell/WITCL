// ProjectDoc.h : interface of the CProjectDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTDOC_H__58E7F920_1CA8_4EF6_BA11_963B935982C9__INCLUDED_)
#define AFX_PROJECTDOC_H__58E7F920_1CA8_4EF6_BA11_963B935982C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CProjectDoc : public CDocument
{
protected: // create from serialization only
	CProjectDoc();
	DECLARE_DYNCREATE(CProjectDoc)

// Attributes
public:
	BITMAPFILEHEADER BitMapFileHeader;  //ͼ���ļ�ͷ
	//BITMAPINFOHEADER BitMapInfoHeader;  //ͼ���ļ���Ϣͷ
	PBITMAPINFO pBitMapInfo;//ͼ���ļ���Ϣ������ͼ���ļ���Ϣͷ�͵�ɫ�壩
	long BmpWidth;  //BMPͼ���ļ����ؿ�
	long nWidth;
	long BmpHeight; //BMPͼ���ļ����ظ�
	long BitCount;  //��ʾBMP�ļ�ÿ������ɫ���ñ�����
	long Compression;//��ʾBMP�ļ���ѹ����ʽ
	int ColorUsed;// ʹ�õ���ɫ��
	BOOL bImageLoaded;//�Ƿ����ڴ��ȡͼ�����ݣ����ǣ���������������Ҫ�ͷ��ڴ�
	BYTE * pImage;//ͼ������
	BYTE * pImageBak;//ͼ�񱸷�����
	//BYTE * pHist;//ֱ��ͼ����
// Operations
public:
	BOOL ReadImage(const char *szFileName);
	void SetDocument(CProjectDoc* pDoc);
	WriteToFile(const char *szFileName);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProjectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CProjectDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTDOC_H__58E7F920_1CA8_4EF6_BA11_963B935982C9__INCLUDED_)
