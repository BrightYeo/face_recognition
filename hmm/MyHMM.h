
//#include <Afxtempl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "hmminclude/cv.h"
#include "hmminclude/highgui.h"

#pragma comment(lib,"./hmminclude/cv.lib")
#pragma comment(lib,"./hmminclude/cvaux.lib")
#pragma comment(lib,"./hmminclude/highgui.lib")

#include "contehmm.h"  //HMM

static int result; // 정답 결과. main에서 쓰임
static int who;

//method
void DEBUG(IplImage* ipl);
void Train(char* in_name,char* out_name);
int Test(CContEHMM* phmm, int num, char* in_name);
