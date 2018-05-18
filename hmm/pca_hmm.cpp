// pcahmm.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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
	
	int num = 10; //�ν� �� ������ ����

	for(i=0;i<num;i++)       // // // Training // // //
	{
		sprintf(in_name,"./face/%d-0.bmp", i);
		sprintf(out_name,"%s.hmm.txt",in_name);
		Train(in_name,out_name);
	}
	
	///////////////////////////
	// �ε� HMM
	CContEHMM* phmm=new CContEHMM[num];
	
	for(i=0;i<num;i++)         // // // Load // // //
	{
		sprintf(in_name,"./face/%d-0.bmp", i);
		sprintf(out_name,"%s.hmm.txt",in_name);
		phmm[i].Load(out_name);
	}
	

	srand(time(NULL));
	
	//Ȯ�� ��� ����
	int scount = 0;  //����
	int fcount = 0;  //����
	int wcount = 0;  //�Ӱ�ġ �ʰ�. �� �����ʹ� pca��
	

		// // // Test // // //

	//FindFirstFile(L"aaaa", NULL); //���ϰ˻� API

	bool is_Load = false;  //file�� ������ true�� ����
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
			//������ ������ ����

			//��̷� �����
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

				//printf("\n�ٽ� �����Ͻðڽ��ϱ�? (Y/N) : ");
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
					printf("\n\n�����մϴ�.\n");
					is_Load = true;
					break;
				case 'N':
					printf("\n\n�����մϴ�.\n");
					is_Load = true;
					break;
				
				case 'q':
					printf("\n\n�����մϴ�.\n");
					is_Load = true;
					break;
				default:
					//printf("\n\nY(y) Ȥ�� N(n)�� �Է����ּ���\n");
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