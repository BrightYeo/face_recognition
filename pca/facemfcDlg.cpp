// facemfcDlg.cpp : 구현 파일
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
static CvHaarClassifierCascade* cascade;		// 케스케이드를 미리 읽어 들임
static CvMemStorage* storage;					// 메모리 스토리지를 미리 생성함



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

//thread
UINT ShowCam(LPVOID para)
{
	CfacemfcDlg* pView = (CfacemfcDlg*)para;

	while(!pView->m_isStop)
	{
		if( g_capture )
		{ 
			g_image = cvRetrieveFrame( g_capture );
			
			CClientDC dc1(g_pWnd);      // 윈도우 DC 생성
			g_pWnd->GetClientRect(&g_rect);    // 윈도우 크기 받아오기
			g_cImage.CopyOf(g_image, 1);
			g_cImage.DrawToHDC(dc1.m_hDC, &g_rect);

			cvWaitKey(10);
		}

		//Sleep(pView->m_Time); //주기 설정
	}
	return 0;
}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CfacemfcDlg 대화 상자

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


// CfacemfcDlg 메시지 처리기

BOOL CfacemfcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	/////////////////////////////////////////////////////////////////////////////////////

	g_kpca.pDlg = this;
	m_isStop = false; //start
	
	AddMsg("초기화 되었습니다.");

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

	//테스트파일이 존재하면 지운다
	system("del ..\\test.bmp");






	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CfacemfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	//Run Thread(View Cam)
	m_ThHandle = AfxBeginThread(ShowCam, this);
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CfacemfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CfacemfcDlg::AddMsg(CString msg)
{
	m_List.AddString(msg);
	m_List.SetCurSel(m_List.GetCount()-1);
}

//PCA 인식 실행
void CfacemfcDlg::OnBnClickedButton2()
{
	IplImage* input_image;
	if(NULL == cvLoadImage("../test.bmp"))
	{
		AfxMessageBox("입력 영상이 없습니다.");
		return;
	}
	g_kpca.DoPCA();
	//g_kpca.DoHMM(); //test

/*
	//찍은 사진을 아래 picture control에 넣는다.
	m_capture2 = g_capture;
	
	cvGrabFrame(m_capture2);
	m_gray = cvRetrieveFrame(m_capture2);

	//gray scale
	m_saveimage = cvCreateImage( cvSize(m_gray->width, m_gray->height), 8, 1 );
	cvCvtColor( m_gray, m_saveimage, CV_BGR2GRAY );

	CClientDC dc(g_pWnd2);      // 윈도우 DC 생성

	RECT rect2;
	g_pWnd2->GetClientRect(&rect2);    // 윈도우 크기 받아오기

	CvvImage image;
	image.CopyOf(m_saveimage, 1);
	image.DrawToHDC(dc.m_hDC, &rect2);

	cvSaveImage("abc.bmp", m_saveimage);
*/
	
	
}

//윈도우 창 소멸 시
BOOL CfacemfcDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	//카메라 끔
	cvReleaseCapture(&g_capture);
	WaitForSingleObject(m_ThHandle, INFINITE);
	m_ThHandle = NULL;

	return CDialog::DestroyWindow();
}




// @param image = 메인에서넘어온 이미지
// @param xml = haar xml 선택하기위한 index 변수

void CfacemfcDlg::FaceDetection(IplImage* img, double scale)
{
	static CvScalar colors[] = {
		{{0,0,255}}, {{0,128,255}}, {{0,255,255}}, {{0,255,0}},
		{{255,128,0}}, {{255,255,0}}, {{255,0,0}}, {{255,0,255}}
	};  // 그리기 작업에서 사용할 색상 정의

	// 영상 준비
	//
	IplImage* temp = NULL;
	m_copy = NULL;
	IplImage* gray = cvCreateImage( cvSize(img->width, img->height), 8, 1 );
	IplImage* small_img = cvCreateImage(cvSize( cvRound(img->width/scale), cvRound(img->height/scale)), 8, 1);
	cvCvtColor( img, gray, CV_BGR2GRAY );
	cvResize( gray, small_img, CV_INTER_LINEAR );
	cvEqualizeHist( small_img, small_img );

	// 객체검출
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

	// 객체를 찾고 박스를 그린다.
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

		//CvPoint center; //원 그리기 위한 변수
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
	
	//사각형 그리기
	//cvRectangle(img, cvPoint(pt1.x, pt1.y),	cvPoint(pt2.x, pt2.y), colors[1%8], 3, 8, 0);
	
	//얼굴 부분 출력
	temp = cvCreateImage(cvSize(nwidth,nheight),IPL_DEPTH_8U, 3 );
	temp = cvCloneImage(img);
	temp->origin = img->origin;

	//만약 pt1에 값이 없으면(얼굴영역 못찾으면) 이 부분에서 에러 난다.
	cvSetImageROI(temp, cvRect(pt1.x, pt1.y, nwidth, nheight) );
	
	m_copy = cvCreateImage(cvSize(60,70),IPL_DEPTH_8U, 3 );
	
	cvResize(temp, m_copy, CV_INTER_LINEAR);  // For Resize
	cvSaveImage("../test.bmp", m_copy);
	m_copy = cvLoadImage("../test.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	cvSaveImage("../test.bmp", m_copy);

	CClientDC dc(g_pWnd3);  // 윈도우 DC 생성

	g_pWnd3->GetClientRect(&g_rect2);    // 윈도우 크기 받아오기

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
		MessageBox("사진이 입력되지 않았습니다.");
        return;
	}

	//cvNamedWindow( "Face_detection", 1 );

	//int64 tStart = cvGetTickCount();

	double scale = 1.3;
	FaceDetection( image, scale );

	//int64 tEnd = cvGetTickCount();
	//cout << "\n* Elapsed time : " <<  0.001 * (tEnd - tStart) / cvGetTickFrequency() << "ms" << endl;

	/*
	CClientDC dc(g_pWnd3);  // 윈도우 DC 생성

	g_pWnd3->GetClientRect(&g_rect2);    // 윈도우 크기 받아오기

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
		AfxMessageBox("얼굴 검출 된 이미지가 없습니다");
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
