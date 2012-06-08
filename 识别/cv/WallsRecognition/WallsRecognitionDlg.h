
// WallsRecognitionDlg.h : header file
//

#pragma once
#include "ImageProcess.h"

// CWallsRecognitionDlg dialog
class CWallsRecognitionDlg : public CDialogEx
{
private:
	ImageProcess * ImgPro;
	CString filename;
	IplImage * img;

// Construction
public:
	CWallsRecognitionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_WALLSRECOGNITION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton2();
private:
//	int patch;
//	int threshold;
//	int itera;
//	int joindist;
//	int maxSeek;
//	int minLineLength;
public:
	int patch;
	int threshold;
	int itera;
	int joindist;
	int maxSeek;
	int minLineLength;
};
