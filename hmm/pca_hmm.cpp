// pcahmm.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "MyHMM.h"
#include <windows.h>
#include <conio.h>

//Do 2D-HMM
void DoHMM()
{
	int i, j;
	char in_name[80];
	char out_name[80];

	srand(time(NULL));
	
	int num = 10; //인식 한 영상의 개수

	for(i=0;i<num;i++)       // // // Training // // //
	{
		sprintf(in_name,"./face/%d-0.bmp", i);
		sprintf(out_name,"%s.hmm.txt",in_name);
		Train(in_name,out_name);
	}
	
	///////////////////////////
	// 로드 HMM
	CContEHMM* phmm=new CContEHMM[num];
	
	for(i=0;i<num;i++)         // // // Load // // //
	{
		sprintf(in_name,"./face/%d-0.bmp", i);
		sprintf(out_name,"%s.hmm.txt",in_name);
		phmm[i].Load(out_name);
	}
	

	srand(time(NULL));
	
	//확률 찍는 변수
	int scount = 0;  //성공
	int fcount = 0;  //실패
	int wcount = 0;  //임계치 초과. 이 데이터는 pca로
	

		// // // Test // // //

	//FindFirstFile(L"aaaa", NULL); //파일검색 API

	bool is_Load = false;  //file이 들어오면 true로 변경
	char* file_name = "../hmm_test.bmp";
	int star = 1;
	int mode = 0;

	while(1)
	{
		if(is_Load)
		{
			break;
		}

		if(!cvLoadImage(file_name, 0))
		{
			//파일이 들어오지 않음

			//재미로 점찍기
			if(mode == 0)
			{
				for(int i=0; i<star; i++)
				{
					printf(".");
				}
				star++;

				if(star == 5)
					mode = 1;
			}
			else
			{
				for(int i=0; i<star; i++)
				{
					printf(".");
				}
				star--;

				if(star == 1)
					mode = 0;
			}

			printf("\n");
			Sleep(1000);
			continue;
		}
		else
		{
			if( !is_Load )
			{
				printf("\n");
				sprintf(in_name, file_name);
				Test(phmm, num, in_name);

				//printf("\n다시 수행하시겠습니까? (Y/N) : ");
				/*
				switch(getch())
				{
				
				case 'y':
					printf("\n");
					is_Load = false;
					break;
				case 'Y':
					printf("\n");
					is_Load = false;
					break;
				case 'n':
					printf("\n\n종료합니다.\n");
					is_Load = true;
					break;
				case 'N':
					printf("\n\n종료합니다.\n");
					is_Load = true;
					break;
				
				case 'q':
					printf("\n\n종료합니다.\n");
					is_Load = true;
					break;
				default:
					//printf("\n\nY(y) 혹은 N(n)을 입력해주세요\n");
					break;
				}
				*/
			}
		}
	}

	delete[] phmm;
}

int _tmain(int argc, _TCHAR* argv[])
{
	DoHMM();

	return 0;
}