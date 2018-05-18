// facemfcDlg.cpp : ���� ����
//

#include "stdafx.h"

#include "facemfc.h"
#include "facemfcDlg.h"

KPCA g_kpca;

IplImage *g_image;
CvCapture *g_capture;
CvvImage g_cImage;
CWnd *g_pWnd;
CWnd *g_pWnd2;
CWnd *g_pWnd3; //adaboost
CDC *g_pDC;
CDC *g_pDC2;
CDC *g_pDC3; //adaboost
RECT g_rect;
RECT g_rect2; // adaboost rect


//adaboost
static CvHaarClassifierCascade* cascade;		// �ɽ����̵带 �̸� �о� ����
static CvMemStorage* storage;					// �޸� ���丮���� �̸� ������



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

//thread
UINT ShowCam(LPVOID para)
{
	CfacemfcDlg* pView = (CfacemfcDlg*)para;

	while(!pView->m_isStop)
	{
		if( g_capture )
		{ 
			g_image = cvRetrieveFrame( g_capture );
			
			CClientDC dc1(g_pWnd);      // ������ DC ����
			g_pWnd->GetClientRect(&g_rect);    // ������ ũ�� �޾ƿ���
			g_cImage.CopyOf(g_image, 1);
			g_cImage.DrawToHDC(dc1.m_hDC, &g_rect);

			cvWaitKey(10);
		}

		//Sleep(pView->m_Time); //�ֱ� ����
	}
	return 0;
}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CfacemfcDlg ��ȭ ����

CfacemfcDlg::CfacemfcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CfacemfcDlg::IDD, pParent)
	, m_dbnum(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfacemfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT1, m_dbnum);
	DDX_Control(pDX, IDC_EDIT1, m_db_num);
}

BEGIN_MESSAGE_MAP(CfacemfcDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, &CfacemfcDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CfacemfcDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CfacemfcDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &CfacemfcDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CfacemfcDlg �޽��� ó����

BOOL CfacemfcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	/////////////////////////////////////////////////////////////////////////////////////

	g_kpca.pDlg = this;
	m_isStop = false; //start
	
	AddMsg("�ʱ�ȭ �Ǿ����ϴ�.");

	//capture for cam
    g_image = 0;
    g_capture = 0;
    g_capture = cvCaptureFromCAM(0);

    g_pWnd = this->GetDlgItem(IDC_PIC);
    g_pDC = g_pWnd->GetDC();
    g_pDC->GetSafeHdc();

	g_pWnd2 = this->GetDlgItem(IDC_PIC2);
    g_pDC2 = g_pWnd2->GetDC();
    g_pDC2->GetSafeHdc();

	g_pWnd3 = this->GetDlgItem(IDC_PIC3);
	g_pDC3 = g_pWnd3->GetDC();
	g_pDC3->GetSafeHdc();

	//�׽�Ʈ������ �����ϸ� �����
	system("del ..\\test.bmp");






	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CfacemfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CfacemfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	//Run Thread(View Cam)
	m_ThHandle = AfxBeginThread(ShowCam, this);
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CfacemfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CfacemfcDlg::AddMsg(CString msg)
{
	m_List.AddString(msg);
	m_List.SetCurSel(m_List.GetCount()-1);
}

//PCA �ν� ����
void CfacemfcDlg::OnBnClickedButton2()
{
	IplImage* input_image;
	if(NULL == cvLoadImage("../test.bmp"))
	{
		AfxMessageBox("�Է� ������ �����ϴ�.");
		return;
	}
	g_kpca.DoPCA();
	//g_kpca.DoHMM(); //test

/*
	//���� ������ �Ʒ� picture control�� �ִ´�.
	m_capture2 = g_capture;
	
	cvGrabFrame(m_capture2);
	m_gray = cvRetrieveFrame(m_capture2);

	//gray scale
	m_saveimage = cvCreateImage( cvSize(m_gray->width, m_gray->height), 8, 1 );
	cvCvtColor( m_gray, m_saveimage, CV_BGR2GRAY );

	CClientDC dc(g_pWnd2);      // ������ DC ����

	RECT rect2;
	g_pWnd2->GetClientRect(&rect2);    // ������ ũ�� �޾ƿ���

	CvvImage image;
	image.CopyOf(m_saveimage, 1);
	image.DrawToHDC(dc.m_hDC, &rect2);

	cvSaveImage("abc.bmp", m_saveimage);
*/
	
	
}

//������ â �Ҹ� ��
BOOL CfacemfcDlg::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	//ī�޶� ��
	cvReleaseCapture(&g_capture);
	WaitForSingleObject(m_ThHandle, INFINITE);
	m_ThHandle = NULL;

	return CDialog::DestroyWindow();
}




// @param image = ���ο����Ѿ�� �̹���
// @param xml = haar xml �����ϱ����� index ����

void CfacemfcDlg::FaceDetection(IplImage* img, double scale)
{
	static CvScalar colors[] = {
		{{0,0,255}}, {{0,128,255}}, {{0,255,255}}, {{0,255,0}},
		{{255,128,0}}, {{255,255,0}}, {{255,0,0}}, {{255,0,255}}
	};  // �׸��� �۾����� ����� ���� ����

	// ���� �غ�
	//
	IplImage* temp = NULL;
	m_copy = NULL;
	IplImage* gray = cvCreateImage( cvSize(img->width, img->height), 8, 1 );
	IplImage* small_img = cvCreateImage(cvSize( cvRound(img->width/scale), cvRound(img->height/scale)), 8, 1);
	cvCvtColor( img, gray, CV_BGR2GRAY );
	cvResize( gray, small_img, CV_INTER_LINEAR );
	cvEqualizeHist( small_img, small_img );

	// ��ü����
	//
	cvClearMemStorage( storage );
	CvSeq* objects = cvHaarDetectObjects(
		small_img,
		cascade,
		storage,
		1.1,
		2,
		0	/*CV_HAAR_DO_CANNY_PRUNING*/,
		cvSize(30,30)
		);

	int old_long = 0;
	CvPoint pt1, pt2;
	int nwidth, nheight;
	nwidth = 0;
	nheight = 0;

	// ��ü�� ã�� �ڽ��� �׸���.
	//
	for( int i=0; i< (objects ? objects->total : 0); i++ )
	{
		CvRect* r = (CvRect*)cvGetSeqElem( objects, i );
		pt1.x = r->x*scale;
		pt1.y = r->y*scale;
		pt2.x = (r->x + r->width)*scale;
		pt2.y = (r->y + r->height)*scale;
		nheight = abs(pt2.y - pt1.y)+10;
		nwidth = abs(pt2.x - pt1.x)+10;

		if(nwidth < old_long)
		{
			continue;
		}
		old_long = nwidth;

		//CvPoint center; //�� �׸��� ���� ����
		//int radius;
		//center.x = cvRound((r->x + r->width*0.5)*scale);
		//center.y = cvRound((r->y + r->height*0.5)*scale);
		//radius = cvRound((r->width + r->height)*0.25*scale);
		//cvCircle( img, center, radius, colors[i%8], 3, 8, 0 );

		/*
		CString tempp;
		tempp.Format("%d ", nwidth);
		MessageBox(tempp);
		*/

	}
	
	//�簢�� �׸���
	//cvRectangle(img, cvPoint(pt1.x, pt1.y),	cvPoint(pt2.x, pt2.y), colors[1%8], 3, 8, 0);
	
	//�� �κ� ���
	temp = cvCreateImage(cvSize(nwidth,nheight),IPL_DEPTH_8U, 3 );
	temp = cvCloneImage(img);
	temp->origin = img->origin;

	//���� pt1�� ���� ������(�󱼿��� ��ã����) �� �κп��� ���� ����.
	cvSetImageROI(temp, cvRect(pt1.x, pt1.y, nwidth, nheight) );
	
	m_copy = cvCreateImage(cvSize(60,70),IPL_DEPTH_8U, 3 );
	
	cvResize(temp, m_copy, CV_INTER_LINEAR);  // For Resize
	cvSaveImage("../test.bmp", m_copy);
	m_copy = cvLoadImage("../test.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	cvSaveImage("../test.bmp", m_copy);

	CClientDC dc(g_pWnd3);  // ������ DC ����

	g_pWnd3->GetClientRect(&g_rect2);    // ������ ũ�� �޾ƿ���

	CvvImage inImage;
	inImage.CopyOf(m_copy, 1);
	inImage.DrawToHDC(dc.m_hDC, &g_rect2);
	cvWaitKey(0);

	cvReleaseImage( &m_copy );
	cvReleaseImage( &gray );
	cvReleaseImage( &small_img );
	cvReleaseImage( &temp );
	
}


//face detection
void CfacemfcDlg::OnBnClickedButton4()
{
	//HaarClassifier* hc = NULL;
	cascade = (CvHaarClassifierCascade*)cvLoad( "haarcascade_frontalface_default.xml", 0, 0, 0 );
	storage = cvCreateMemStorage(0);

	IplImage* image = 0;
	//image = cvLoadImage( "finkl.jpg", CV_LOAD_IMAGE_COLOR );

	m_capture2 = g_capture;
	
	cvGrabFrame(m_capture2);
	image = cvRetrieveFrame(m_capture2);
	if( !image )
	{
		MessageBox("������ �Էµ��� �ʾҽ��ϴ�.");
        return;
	}

	//cvNamedWindow( "Face_detection", 1 );

	//int64 tStart = cvGetTickCount();

	double scale = 1.3;
	FaceDetection( image, scale );

	//int64 tEnd = cvGetTickCount();
	//cout << "\n* Elapsed time : " <<  0.001 * (tEnd - tStart) / cvGetTickFrequency() << "ms" << endl;

	/*
	CClientDC dc(g_pWnd3);  // ������ DC ����

	g_pWnd3->GetClientRect(&g_rect2);    // ������ ũ�� �޾ƿ���

	CvvImage inImage;
	inImage.CopyOf(image, 1);
	inImage.DrawToHDC(dc.m_hDC, &g_rect2);
	cvWaitKey(0);
	*/

}

void CfacemfcDlg::OnBnClickedButton1()
{
/*	if(m_copy == NULL)
	{
		AfxMessageBox("�� ���� �� �̹����� �����ϴ�");
		return;
	}
*/
	UpdateData(TRUE);
	CString str;
	m_db_num.GetWindowText(str);

	AfxMessageBox(str);
	//cvSaveImage("../db/
}

void CfacemfcDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(NULL != m_ThHandle)
	{
		CloseHandle(m_ThHandle);
	}
	if(NULL != g_kpca.ThHandle2)
	{
		CloseHandle(g_kpca.ThHandle2);
	}

	OnCancel();
}
