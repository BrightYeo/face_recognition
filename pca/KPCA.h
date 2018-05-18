// KPCA.h

#if !defined(AFX_KPCA_H__77647ED3_9E5C_464F_889E_0C2C03898381__INCLUDED_)
#define AFX_KPCA_H__77647ED3_9E5C_464F_889E_0C2C03898381__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "resource.h"
#include "facemfcDlg.h"
#include "facemfc.h"

class CfacemfcDlg;

#include <cmath>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <cstdlib>

#include "./include/cv.h"  // include core library interface
#include "./include/highgui.h" // include GUI library interface
#include "./include/cvaux.h"
#include "./include/cxcore.h"
#include "./include/cvcam.h"

//cv.lib cxcore.lib cvaux.lib highgui.lib 
#pragma comment (lib, "./lib/cv200.lib")
#pragma comment (lib, "./lib/cxcore200.lib")
#pragma comment (lib, "./lib/cvaux200.lib")
#pragma comment (lib, "./lib/highgui200.lib")
#pragma comment (lib, "./lib/cvcam.lib")

class KPCA  
{
public:
	KPCA();
	virtual ~KPCA();
private:
	IplImage ** m_faceImgArr			; // array of face images
	CvMat    *  m_personNumTruthMat		; // array of person numbers
	int m_nTrainFaces					; // the number of training images
	int m_nEigens						; // the number of eigenvalues
	IplImage * m_pAvgTrainImg			; // the average image
	IplImage ** m_eigenVectArr			; // eigenvectors
	CvMat	 * m_eigenValMat			; // eigenvalues
	CvMat	 * m_projectedTrainFaceMat	; // projected training faces

	double m_lDist; //Least Distance
	double m_ThreshA; //threshold α
	double m_ThreshB; //threshold β

private:	
	void storeTrainingData(char* t_save);
	void doPCA();
	int loadFaceImgArray(char * filename);	 
	int findNearestNeighbor(float * projectedTestFace);
	int loadTrainingData(CvMat ** pTrainPersonNumMat,char* t_save);
	int TestThreshold(int iNearest, double leastDistSq);

public:
	int m_count;  // 데이터베이스의 갯수

public:
	void learn(char* t_file,char* t_save);
	void recognize(char* t_file, char* t_save, int a);
	void Release();
	//m_ThreshA, m_ThreshB의 값을 셋팅하기 위한 함수
	void SetThreshold(double a, double b=50);
	void Test(int count, double thresh);
	void DoPCA();
	void DoHMM();
	int m_temp_num;
	int m_succ; //Test하여 성공한 범죄자 번호
	CfacemfcDlg* pDlg;
	HANDLE ThHandle2;

};

#endif // !defined(AFX_KPCA_H__77647ED3_9E5C_464F_889E_0C2C03898381__INCLUDED_)
