
// WallsRecognitionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WallsRecognition.h"
#include "WallsRecognitionDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWallsRecognitionDlg dialog




CWallsRecognitionDlg::CWallsRecognitionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWallsRecognitionDlg::IDD, pParent)
	, patch(0)
	, threshold(0)
	, itera(0)
	, joindist(0)
	, maxSeek(0)
	, minLineLength(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	patch = 0;
	threshold = 0;
	itera = 0;
	joindist = 0;
	maxSeek = 0;
	minLineLength = 0;
}

void CWallsRecognitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, patch);
	DDX_Text(pDX, IDC_EDIT3, threshold);
	DDX_Text(pDX, IDC_EDIT4, itera);
	DDX_Text(pDX, IDC_EDIT5, joindist);
	DDX_Text(pDX, IDC_EDIT6, maxSeek);
	DDX_Text(pDX, IDC_EDIT7, minLineLength);
}

BEGIN_MESSAGE_MAP(CWallsRecognitionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CWallsRecognitionDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CWallsRecognitionDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CWallsRecognitionDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &CWallsRecognitionDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CWallsRecognitionDlg message handlers

BOOL CWallsRecognitionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	filename = "";
	ImgPro = 0;
	img = 0;
	patch = 7;
	threshold = 4800;
	itera = 1;
	joindist = 12;
	maxSeek = 2;
	minLineLength = 16;
	UpdateData(false);
	/*SetDlgItemText(IDC_EDIT2, _T("7"));
	SetDlgItemText(IDC_EDIT3, _T("3800"));
	SetDlgItemText(IDC_EDIT4, _T("2"));
	SetDlgItemText(IDC_EDIT5, _T("10"));
	SetDlgItemText(IDC_EDIT6, _T("2"));
	SetDlgItemText(IDC_EDIT7, _T("20"));*/

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWallsRecognitionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWallsRecognitionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWallsRecognitionDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString strFile;
	CString defExe("jpg");
	CString defFileName("*.jpg");
	CString defFilter("JPG文件(*.jpg)|*.jpg|PNG文件(*.png)|*.png|所有文件(*.*)|*.*|");
	CFileDialog dlg(true,defExe,defFileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,defFilter);
	//下面可以指定文件夹，如果不用，默认打开”我的文档“
	//dlg.m_ofn.lpstrInitialDir= "c:\\windows\\"; 
	if(dlg.DoModal()==IDOK)
	{
	   strFile = dlg.GetPathName();
	}
	SetDlgItemText(IDC_EDIT1, strFile);
	UpdateData(false);
}


void CWallsRecognitionDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialogEx::PostNcDestroy();

	if(ImgPro != 0) delete ImgPro;
	if(img != 0) cvReleaseImage(&img);
}


void CWallsRecognitionDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
	CString strFile;
	CString defExe("xml");
	CString defFileName("lines.xml");
	CString defFilter("XML文件(*.xml)|*.xml");
	CFileDialog dlg(false,defExe,defFileName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,defFilter);
	//下面可以指定文件夹，如果不用，默认打开”我的文档“
	//dlg.m_ofn.lpstrInitialDir= "c:\\windows\\"; 
	if(dlg.DoModal()==IDOK)
	{
	   strFile = dlg.GetPathName();
	}

	if(ImgPro != 0) delete ImgPro;
	if(img != 0) cvReleaseImage(&img);
	GetDlgItemText(IDC_EDIT1,filename);
	img = cvLoadImage((bstr_t)filename,1);
	if(img == 0) 
	{
		MessageBox(_T("Error！Can not open image."));
		return;
	}
	ImgPro = new ImageProcess();
	UpdateData(true);
	ImgPro->setPatch(patch);
	ImgPro->setThreshold(threshold);
	ImgPro->setItera(itera);
	ImgPro->setJoindist(joindist);
	ImgPro->setMaxSeek(maxSeek);
	ImgPro->setminLineLength(minLineLength);
	ImgPro->processImage((bstr_t)filename,(bstr_t)strFile);
	MessageBox(_T("OK!"));
}


void CWallsRecognitionDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
	
}


void CWallsRecognitionDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	if(ImgPro == 0 || img == 0) 
	{
		MessageBox(_T("Error! NO Image."));
		return;
	}
	for(ImgPro->iter=ImgPro->vec_begin(); ImgPro->iter!=ImgPro->vec_end(); ++ImgPro->iter)
	{
		CvPoint p1;
		p1.x=(*ImgPro->iter).x1;
		p1.y=(*ImgPro->iter).y1;
		CvPoint p2;
		p2.x=(*ImgPro->iter).x2;
		p2.y=(*ImgPro->iter).y2;
		cvLine( img, p1, p2, CV_RGB(255,0,0), 2, CV_AA, 0 );
	}

	for(ImgPro->iter1=ImgPro->vec_begin1(); ImgPro->iter1!=ImgPro->vec_end1(); ++ImgPro->iter1)
	{
		CvPoint p;
		p.x=(*ImgPro->iter1).x;
		p.y=(*ImgPro->iter1).y;
		int radius=(int)((*ImgPro->iter1).length/2.0);
		CString s;
		s.Format(L"%d,%d",p.x,p.y);
		if(radius<0)
		{
		MessageBox(s);
		cvCircle(img, p, -radius, CV_RGB(255,0,0),
               1, 8, 0 );
		}
		else if(radius==0)
			MessageBox(_T("Error! =0."));
		else 
		cvCircle(img, p, radius, CV_RGB(255,0,0),
               1, 8, 0 );
	}
	
	/*for(ImgPro->iter1=ImgPro->flag-ImgPro->max; ImgPro->iter1!=ImgPro->flag; ++ImgPro->iter1)
	{
		CvPoint p;
		p.x=(*ImgPro->iter1).x;
		p.y=(*ImgPro->iter1).y;
		int radius=(int)((*ImgPro->iter1).length/2.0);
		cvCircle(img, p, radius, CV_RGB(255,0,0),
               1, 8, 0 );
	}*/
    
   //
	CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* lines = 0;
	 IplImage* dst;
	  dst = cvCreateImage( cvGetSize(ImgPro->getPreImage()), 8, 1 );
	  cvCanny( ImgPro->getPreImage(), dst, 50, 200, 3 );
	lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 50, 50, 10 );
    for( int i = 0; i < lines->total; i++ )
    {
        CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
		if(line[0].x!=line[1].x&&line[0].y!=line[1].y)
        cvLine( img, line[0], line[1], CV_RGB(255,0,0), 3, CV_AA, 0 );
    }

	cvNamedWindow("Result",1);
	cvShowImage("Result",img);
	cvNamedWindow("preImg",1);
	cvShowImage("preImg",ImgPro->getPreImage());
	cvWaitKey(0);
	cvDestroyWindow("Result");
	cvDestroyWindow("preImg");
}

