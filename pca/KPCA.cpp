//KPCA.cpp

#include "stdafx.h"
#include "KPCA.h"
#include <stdlib.h>
#include <time.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KPCA::KPCA()
{
	m_faceImgArr			= 0; // array of face images
	m_personNumTruthMat		= 0; // array of person numbers
	m_nTrainFaces			= 0; // the number of training images
	m_nEigens				= 0; // the number of eigenvalues
	m_pAvgTrainImg			= 0; // the average image
	m_eigenVectArr			= 0; // eigenvectors
	m_eigenValMat			= 0; // eigenvalues
	m_projectedTrainFaceMat	= 0; // projected training faces
	m_temp_num				= 0;
	m_lDist =	0;	// Least Distance
	//m_Mode	=	0;	// Algorithm Mode

}

KPCA::~KPCA()
{

}

IplImage *m_image;

UINT ThHMM(LPVOID para)
{
	CfacemfcDlg* pView = (CfacemfcDlg*)para;

	while(1)
	{
		if( cvLoadImage("../result/-1.bmp") )
		{
			AfxMessageBox("�Ϲ����Դϴ�.");
			system("del ..\\result\\-1.bmp");
			break;
		}
		else if( cvLoadImage("../result/0.bmp") )
		{
			AfxMessageBox("0�� ����������Դϴ�.");
			system("del ..\\result\\0.bmp");
			m_image = cvLoadImage("../db/0.bmp");
			cvNamedWindow( "Result", 1 );
			cvShowImage("Result", m_image);
			cvWaitKey(0);
			break;
		}
		else if( cvLoadImage("../result/1.bmp") )
		{
			AfxMessageBox("1�� ����������Դϴ�.");
			system("del ..\\result\\1.bmp");
			m_image = cvLoadImage("../db/1.bmp");
			cvNamedWindow( "Result", 1 );
			cvShowImage("Result", m_image);
			cvWaitKey(0);
			break;
		}
		else if( cvLoadImage("../result/2.bmp") )
		{
			AfxMessageBox("2�� ����������Դϴ�.");
			system("del ..\\result\\2.bmp");
			m_image = cvLoadImage("../db/2.bmp");
			cvNamedWindow( "Result", 1 );
			cvShowImage("Result", m_image);
			cvWaitKey(0);
			break;
		}
		else if( cvLoadImage("../result/3.bmp") )
		{
			AfxMessageBox("3�� ����������Դϴ�.");
			system("del ..\\result\\3.bmp");
			m_image = cvLoadImage("../db/3.bmp");
			cvNamedWindow( "Result", 1 );
			cvShowImage("Result", m_image);
			cvWaitKey(0);
			break;
		}
		else if( cvLoadImage("../result/4.bmp") )
		{
			AfxMessageBox("4�� ����������Դϴ�.");
			system("del ..\\result\\4.bmp");
			m_image = cvLoadImage("../db/4.bmp");
			cvNamedWindow( "Result", 1 );
			cvShowImage("Result", m_image);
			cvWaitKey(0);
			break;
		}
		else if( cvLoadImage("../result/5.bmp") )
		{
			AfxMessageBox("5�� ����������Դϴ�.");
			system("del ..\\result\\5.bmp");
			m_image = cvLoadImage("../db/5.bmp");
			cvNamedWindow( "Result", 1 );
			cvShowImage("Result", m_image);
			cvWaitKey(0);
			break;
		}
		else if( cvLoadImage("../result/6.bmp") )
		{
			AfxMessageBox("6�� ����������Դϴ�.");
			system("del ..\\result\\6.bmp");
			m_image = cvLoadImage("../db/6.bmp");
			cvNamedWindow( "Result", 1 );
			cvShowImage("Result", m_image);
			cvWaitKey(0);
			break;
		}
		else if( cvLoadImage("../result/7.bmp") )
		{
			AfxMessageBox("7�� ����������Դϴ�.");
			system("del ..\\result\\7.bmp");
			m_image = cvLoadImage("../db/7.bmp");
			cvNamedWindow( "Result", 1 );
			cvShowImage("Result", m_image);
			cvWaitKey(0);
			break;
		}
		else if( cvLoadImage("../result/8.bmp") )
		{
			AfxMessageBox("8�� ����������Դϴ�.");
			system("del ..\\result\\8.bmp");
			m_image = cvLoadImage("../db/8.bmp");
			cvNamedWindow( "Result", 1 );
			cvShowImage("Result", m_image);
			cvWaitKey(0);
			break;
		}
		else if( cvLoadImage("../result/9.bmp") )
		{
			AfxMessageBox("9�� ����������Դϴ�.");
			system("del ..\\result\\9.bmp");
			m_image = cvLoadImage("../db/9.bmp");
			cvNamedWindow( "Result", 1 );
			cvShowImage("Result", m_image);
			cvWaitKey(0);
			break;
		}
		Sleep(400);
	}

	//cvReleaseImage(&m_image);

	return 0;
}


//////////////////////////////////////////////////////////////////////////////
// loadFaceImgArray()
//
int KPCA::loadFaceImgArray(char * filename)
{
	FILE * imgListFile = 0;
	char imgFilename[512];
	int iFace, nFaces = 0;

	//CString msg;
	//msg.Format("%d", );
	//AfxMessageBox(filename);

	// open the input file
	if( !(imgListFile = fopen(filename, "r")) )
	{
		fprintf(stderr, "Can\'t open file %s\n", filename);
		return 0;
	}

	// count the number of faces
	while( fgets(imgFilename, 512, imgListFile) ) 
	{
		++nFaces;
	}
	rewind(imgListFile);

	// allocate the face-image array and person number matrix
	m_faceImgArr        = (IplImage **)cvAlloc( nFaces*sizeof(IplImage *) );
	m_personNumTruthMat = cvCreateMat( 1, nFaces, CV_32SC1 );

//	CString temp;
//	temp.Format("m_Scount : %d", m_Scount);
//	pDlg->AddMsg(imgFilename);
//	Sleep(1000);

//	cvNamedWindow( "Face_detection", 1 );
//	cvShowImage("Face_detection", m_faceImgArr[0]);

	//Debug
	//CString msg;
	//msg.Format("%d", nFaces);
	//AfxMessageBox(msg);

	// store the face images in an array
	for(iFace=0; iFace<nFaces; iFace++)
	{
		// read person number and name of image file
		fscanf(imgListFile,
			"%d %s", m_personNumTruthMat->data.i+iFace, imgFilename);

		// load the face image
		m_faceImgArr[iFace] = cvLoadImage(imgFilename, CV_LOAD_IMAGE_GRAYSCALE);

		if( !m_faceImgArr[iFace] )
		{
			fprintf(stderr, "Can\'t load image from %s\n", imgFilename);
			return 0;
		}
	}

//	cvNamedWindow( "Face_detection", 1 );
//	cvShowImage("Face_detection", m_faceImgArr[0]);
//	cvWaitKey(2000);

	fclose(imgListFile);

	return nFaces;
}

//////////////////////////////////////////////////////////////////////////////
// doPCA()
//
void KPCA::doPCA()
{
	int i;
	CvTermCriteria calcLimit;
	CvSize faceImgSize;

	// set the number of eigenvalues to use
	m_nEigens = m_nTrainFaces-1;

	// allocate the eigenvector images
	faceImgSize.width  = m_faceImgArr[0]->width;
	faceImgSize.height = m_faceImgArr[0]->height;
	m_eigenVectArr = (IplImage**)cvAlloc(sizeof(IplImage*) * m_nEigens);
	for(i=0; i<m_nEigens; i++)
		m_eigenVectArr[i] = cvCreateImage(faceImgSize, IPL_DEPTH_32F, 1);

		//CString msg;
		//msg.Format("%d", nTestFaces);
		//AfxMessageBox(msg);


	// allocate the eigenvalue array
	m_eigenValMat = cvCreateMat( 1, m_nEigens, CV_32FC1 );

	// allocate the averaged image
	m_pAvgTrainImg = cvCreateImage(faceImgSize, IPL_DEPTH_32F, 1);

	// set the PCA termination criterion
	calcLimit = cvTermCriteria( CV_TERMCRIT_ITER, m_nEigens, 1);

	// compute average image, eigenvalues, and eigenvectors
	cvCalcEigenObjects(
		m_nTrainFaces,
		(void*)m_faceImgArr,
		(void*)m_eigenVectArr,
		CV_EIGOBJ_NO_CALLBACK,
		0,
		0,
		&calcLimit,
		m_pAvgTrainImg,
		m_eigenValMat->data.fl);

	cvNormalize(m_eigenValMat, m_eigenValMat, 1, 0, CV_L1, 0);
}

//////////////////////////////////////////////////////////////////////////////
// storeTrainingData()
//
void KPCA::storeTrainingData(char* t_save)
{
	CvFileStorage * fileStorage;
	int i;

	// create a file-storage interface
	fileStorage = cvOpenFileStorage( t_save, 0, CV_STORAGE_WRITE );

	// store all the data
	cvWriteInt( fileStorage, "m_nEigens", m_nEigens );
	cvWriteInt( fileStorage, "m_nTrainFaces", m_nTrainFaces );
	cvWrite(fileStorage, "trainPersonNumMat", m_personNumTruthMat, cvAttrList(0,0));
	cvWrite(fileStorage, "m_eigenValMat", m_eigenValMat, cvAttrList(0,0));
	cvWrite(fileStorage, "m_projectedTrainFaceMat", m_projectedTrainFaceMat, cvAttrList(0,0));
	cvWrite(fileStorage, "avgTrainImg", m_pAvgTrainImg, cvAttrList(0,0));
	for(i=0; i<m_nEigens; i++)
	{
		char varname[200];
		sprintf( varname, "eigenVect_%d", i );
		cvWrite(fileStorage, varname, m_eigenVectArr[i], cvAttrList(0,0));
	}

	// release the file-storage interface
	cvReleaseFileStorage( &fileStorage );
}

//////////////////////////////////////////////////////////////////////////////
// learn()
//
void KPCA::learn(char* t_file, char* t_save)
{
	int i, offset;

	// load training data
	m_nTrainFaces = loadFaceImgArray(t_file);
	m_count = m_nTrainFaces; //�н� ���� ���� ����

//	CString temp;
//	temp.Format("m_nTrainFaces : %d", m_nTrainFaces);
//	pDlg->AddMsg(temp);
//	Sleep(1000);

	if( m_nTrainFaces < 2 )
	{
		fprintf(stderr,
		        "Need 2 or more training faces\n"
		        "Input file contains only %d\n", m_nTrainFaces);
		return;
	}

	// do PCA on the training faces
	doPCA();

	// project the training images onto the PCA subspace
	m_projectedTrainFaceMat = cvCreateMat( m_nTrainFaces, m_nEigens, CV_32FC1 );
	offset = m_projectedTrainFaceMat->step / sizeof(float);
	for(i=0; i<m_nTrainFaces; i++)
	{
		cvEigenDecomposite(
			m_faceImgArr[i],
			m_nEigens,
			m_eigenVectArr,
			0, 0,
			m_pAvgTrainImg,
			m_projectedTrainFaceMat->data.fl + i*offset);
	}

	// store the recognition data as an xml file
	storeTrainingData(t_save);
}

//////////////////////////////////////////////////////////////////////////////
// loadTrainingData()
//
int KPCA::loadTrainingData(CvMat ** pTrainPersonNumMat,char* t_save)
{
	CvFileStorage * fileStorage;
	int i;

	// create a file-storage interface
	fileStorage = cvOpenFileStorage( t_save, 0, CV_STORAGE_READ );
	if( !fileStorage )
	{
		fprintf(stderr, "Can't open facedata.xml\n");
		return 0;
	}

	m_nEigens = cvReadIntByName(fileStorage, 0, "m_nEigens", 0);
	m_nTrainFaces = cvReadIntByName(fileStorage, 0, "m_nTrainFaces", 0);
	*pTrainPersonNumMat = (CvMat *)cvReadByName(fileStorage, 0, "trainPersonNumMat", 0);
	m_eigenValMat  = (CvMat *)cvReadByName(fileStorage, 0, "m_eigenValMat", 0);
	m_projectedTrainFaceMat = (CvMat *)cvReadByName(fileStorage, 0, "m_projectedTrainFaceMat", 0);
	m_pAvgTrainImg = (IplImage *)cvReadByName(fileStorage, 0, "avgTrainImg", 0);
	m_eigenVectArr = (IplImage **)cvAlloc(m_nTrainFaces*sizeof(IplImage *));
	for(i=0; i<m_nEigens; i++)
	{
		char varname[200];
		sprintf( varname, "eigenVect_%d", i );
		m_eigenVectArr[i] = (IplImage *)cvReadByName(fileStorage, 0, varname, 0);
	}

	// release the file-storage interface
	cvReleaseFileStorage( &fileStorage );

	return 1;
}

//////////////////////////////////////////////////////////////////////////////
// SetThreshold()
//
void KPCA::SetThreshold(double a, double b)
{
	m_ThreshA = a;
	m_ThreshB = b;
}

//////////////////////////////////////////////////////////////////////////////
// TestThreshold()
//

int KPCA::TestThreshold(int iNearest, double leastDistSq)
{
	double threshold = 0;
	
	threshold = m_ThreshA;	//PCA -> EQ �� ���� threshold(��)

	if (threshold < leastDistSq)	//����(dist)�� Threshold ������ ���!(Who)
	{
		iNearest = 10000;
	}

	return iNearest;
}


//////////////////////////////////////////////////////////////////////////////
// findNearestNeighbor()
//
int KPCA::findNearestNeighbor(float * projectedTestFace)
{
	double leastDistSq = DBL_MAX;
	m_lDist = DBL_MAX;
	int i, iTrain, iNearest = 0;

/*
	if (1 == m_Mode)
	{
		printf("EQ MODE!!\n");

		//Gray �̹����� �ٲ�
		IplImage* temp = cvLoadImage(m_EQ.GetName(), CV_LOAD_IMAGE_GRAYSCALE);
		cvEigenDecomposite(temp, m_nEigens, m_eigenVectArr, 0, 0,
 							m_pAvgTrainImg, projectedTestFace);

		cvReleaseImage(&temp);
	}
*/

	for(iTrain=0; iTrain<m_nTrainFaces; iTrain++)
	{
		double distSq=0;

		for(i=0; i<m_nEigens; i++)
		{
			float d_i;
			d_i=projectedTestFace[i]-m_projectedTrainFaceMat->data.fl[iTrain*m_nEigens + i];
			
			//distSq += d_i*d_i; // Euclidean
			distSq += fabs(d_i/10000);
		}

		if(distSq < leastDistSq)
		{
			leastDistSq = distSq;
			iNearest = iTrain;
		}

		//�ش� �̹����� ����(dist)�� Threshold ���� �ִ� �� �Ǵ�
		iNearest = TestThreshold(iNearest, leastDistSq);
	}

	printf("leastDist = %.2f\n", leastDistSq);
	m_lDist = leastDistSq;

	return iNearest;
}

//////////////////////////////////////////////////////////////////////////////
// recognize()
//
void KPCA::recognize(char* t_file, char* t_save, int a)
{
	int i, nTestFaces  = 0;         // the number of test images
	CvMat * trainPersonNumMat = 0;  // the person numbers during training
	float * projectedTestFace = 0;

	//load test images and ground truth for person number
	nTestFaces = loadFaceImgArray(t_file);
	
	// load the saved training data
	if( !loadTrainingData( &trainPersonNumMat ,t_save) )
	{
		AfxMessageBox("����� �н� �����͸� ���� ���߽��ϴ�.");
		return;
	}

	//Open save file
	char filename[30];
	char buf[80];
	char temp[80];

	// project the test images onto the PCA subspace
	projectedTestFace = (float *)cvAlloc( m_nEigens*sizeof(float) );

	for(i=0; i<nTestFaces; i++)  // nTestFaces : 1
	{
		int iNearest, nearest, truth;

		//Debug
//		CString msg;
//		msg.Format("%d", nTestFaces);
//		AfxMessageBox(msg);

		//Debug
//		cvNamedWindow( "Face_detection", 1 );
//		cvShowImage("Face_detection", m_pAvgTrainImg);
//		cvWaitKey(2000);
//		cvDestroyWindow("Face_detection");

		// project the test image onto the PCA subspace
		cvEigenDecomposite(m_faceImgArr[i], m_nEigens, m_eigenVectArr, 0, 0,
							m_pAvgTrainImg, projectedTestFace);  //�̰� ����

		//Debug
		//CString msg;
		//msg.Format("%d", nTestFaces);
		//AfxMessageBox(msg);

		iNearest = findNearestNeighbor(projectedTestFace);
		truth    = m_personNumTruthMat->data.i[i];
		nearest  = iNearest;

		if(truth == nearest)
		{
			//���ν� ����, ������ ���
			m_temp_num++;
			m_succ = a;
		}
		else
		{
			if (10000 == nearest)	//threshold ���� �ʰ����� ��
			{
				continue;
			}
			else
			{
				//AfxMessageBox("���� �������� ���߽��ϴ�. 2");
			}
		}

	}

	//cvNamedWindow("����", CV_WINDOW_AUTOSIZE);
	//cvShowImage("����", select );

	Release();
}

//////////////////////////////////////////////////////////////////////////////
// Release()
//
void KPCA::Release()
{
	cvReleaseImage(m_faceImgArr);
	cvReleaseImage(&m_pAvgTrainImg);
	cvReleaseImage(m_eigenVectArr);
	cvReleaseMat(&m_personNumTruthMat);
	cvReleaseMat(&m_eigenValMat);
	cvReleaseMat(&m_projectedTrainFaceMat);
}

//�̹��� �������� �̱�(1�� �� �� ���� �̹��� => �� 10��)
void KPCA::Test(int count, double thresh)
{
	char buf[500];

	FILE* fp = fopen("learn.txt", "w");

	sprintf(buf, "./PCA/%.2fCdata.txt", thresh);
	FILE* Cdata = fopen(buf, "a"); //Excel�� ���� Learning Data(��������� ������) ���

	fprintf(Cdata, "Test%d\t", count);

	srand(time(NULL));
	for(int i=0;i<10;i++)
	{
		sprintf(buf, "%d ../db/%d.bmp\n", i, i);  //�н� ������ ���� ��� learn.txt

		fprintf(fp,buf);
		fprintf(Cdata, "%d\t", i); //���������� �н��� ��
	}

	fprintf(Cdata, "\n");

	fclose(fp);
	fclose(Cdata);
}

void KPCA::DoHMM()
{
	//HMM���� ������ �� result ������ �ִ� �����͸� �����Ѵ�.
	AfxMessageBox("Run HMM");
	ThHandle2 = AfxBeginThread(ThHMM, this);

}

void KPCA::DoPCA()
{
	float rate = 0;
	int stotal = 0;
	int wtotal = 0;
	int ftotal = 0;

	//double a = 0.58;
	double a = 0.44; //���� 58
	//double a = 0.96;
	double b = 0;
	double thresh = 0;

	//thresh = 0.59;
	thresh = 0.45; // ���� 59
	//thresh = 0.97;
	b = thresh;
	SetThreshold(a, b);

	m_count = 0; //�Ʒ� learn���� �ʱ�ȭ ��

	//Learning�� ������ ����
	Test(1, b);

	//Learning(�н�)
	learn("learn.txt","db.xml");

	for(int i=0;i<m_count;i++) ////// 10ȸ
	{
		FILE* fp = fopen("test.txt", "w");
		fprintf(fp, "%d ../test.bmp\n", i);
		fclose(fp);

		recognize("test.txt", "db.xml", i);
	}
	
	if(m_temp_num == 0)
	{
		AfxMessageBox("������ ������ �������� �ʽ��ϴ�. 2�� �ν��� �����մϴ�.");
		
		//�ӽ� ���Ϸ� ����
		IplImage* image = cvLoadImage("../test.bmp");
		cvSaveImage("../hmm_test.bmp", image);

		//HMM����

		DoHMM();
	}
	else
	{
		CString msg;
		IplImage* _image;
		char res_num[80];

		msg.Format("��������ڷ� �ǽɵǴ� ������ ����Ǿ����ϴ� : (%d)", m_succ);
		AfxMessageBox(msg);

		//���õ� image �ҷ�����
		sprintf(res_num, "../db/%d.bmp", m_succ);
		_image = cvLoadImage(res_num);
		cvNamedWindow( "���������");
		cvShowImage("���������", _image);
		cvReleaseImage(&_image);
	}

	m_temp_num = 0;
	m_succ = 0;

	pDlg->AddMsg("�Ϸ�Ǿ����ϴ�.");
	if(NULL != ThHandle2)
	{
		CloseHandle(ThHandle2);
		cvReleaseImage(&m_image);
	}
	

}