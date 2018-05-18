// pcahmm.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

#include "KPCA.h"
#include "MyHMM.h"


//�̹��� �������� �̱�(1�� �� �� ���� �̹��� => �� 10��)
//PCA Train
void Test(int count, double thresh)
{
	char buf[500];

	FILE* fp = fopen("learn.txt", "w");

	sprintf(buf, "./Beta/%.2fCdata.txt", thresh);
	FILE* Cdata = fopen(buf, "a"); //Excel�� ���� Learning Data(������ ������) ���

	printf("===== Learning Data =====\n");
	fprintf(Cdata, "Test%d\t", count);

	srand(time(NULL));
	for(int i=0;i<10;i++) ////// 10ȸ
	{
		int t = 0;//rand()%4;

		sprintf(buf, "%d face/%d-%d.bmp\n", i, i, t);  //�н� ������ ���� ��� learn.txt
		printf("%s", buf);

		fprintf(fp,buf);
		fprintf(Cdata, "%d-%d\t", i, t); //���������� �н��� ��
	}

	fprintf(Cdata, "\n");

	printf("=========================\n\n");

	fclose(fp);
	fclose(Cdata);
}


//Do PCA
void DoPCA()
{
	KPCA kpca; //KPCA�� ����ϱ� ���� ��ü


	float rate = 0;
	int stotal = 0;
	int wtotal = 0;
	int ftotal = 0;

	double a = 0.58;
	double b = 0;
	double thresh = 0;
//	for (thresh=0.59; thresh<0.70; thresh+=0.01) ////// 10ȸ
//	{		
		thresh = 0.59; //temp
		b = thresh;
		kpca.SetThreshold(a, b);

		for (int count=1; count<=1;count++) ////// ���� Ƚ��!!!!!!!!
		{
			//int count = 1;	
			//Count �ʱ�ȭ
			kpca.m_Scount = 0;
			kpca.m_Fcount = 0;
			kpca.m_Wcount = 0;

			//Learning�� ������ ����
			Test(count, b);

			//Learning(�н�)
			kpca.learn("learn.txt","db.xml");

			printf("===== Recognize =====\n");

			FILE* fp = fopen("who1.txt", "r");


			for(int i=0;i<10;i++) ////// 10ȸ
			{
				int ran = rand()%3+1;
				//for(int j=1;j<4;j++) ////// 4ȸ -> 3ȸ(1, 2, 3�� �ν�)
				//{
					FILE* fp = fopen("test.txt", "w");

					printf("%d face/%d-%d.bmp\n", i , i, ran);
					fprintf(fp, "%d face/%d-%d.bmp\n", i , i, ran);

					fclose(fp);

					//Recognizing(�ν�)
					kpca.recognize("test.txt", "db.xml", "re.txt", i, ran);
				//}
			}
			

	/*
			int i[8] = {1, 4, 5, 5, 7, 8, 8, 9};
			int j[8] = {3, 3, 1, 2, 3, 2, 3, 3};

			for(int ii=0; ii<8; ii++)
			{
				FILE* fp1 = fopen("test.txt", "w");

				printf("%d face/%d-%d.bmp\n", i[ii] , i[ii], j[ii]);
				fprintf(fp1, "%d face/%d-%d.bmp\n", i[ii] , i[ii], j[ii]);

				fclose(fp1);
				kpca.recognize("test.txt", "db.xml", "re.txt", i[ii], j[ii]);
			}
	*/
			printf("=====================\n");
			
			FILE* fp2 = fopen("rate.txt", "a");
			fprintf(fp2, "S:%d, F:%d, W:%d\n", kpca.m_Scount, kpca.m_Fcount, kpca.m_Wcount);
			fclose(fp2);

			stotal = stotal + kpca.m_Scount;
			wtotal = wtotal + kpca.m_Wcount;
			ftotal = ftotal + kpca.m_Fcount;
			rate = (float)stotal/(stotal+wtotal+ftotal);
		}
//	}	

	FILE* fp3 = fopen("rate.txt", "a");
	fprintf(fp3, "\nstotal : %d\nwtotal : %d\nftotal : %d\nrate : %f\n", stotal, wtotal, ftotal, rate);
	fclose(fp3);

}


//Do HMM
void DoHMM()
{
	int i, j;
	char in_name[80];
	char out_name[80];


	srand(time(NULL));
	int num=10;

	
	for(i=0;i<num;i++)       // // // Training // // //
	{
		sprintf(in_name,"./face/%d-0.bmp",i);
		sprintf(out_name,"%s.hmm.txt",in_name);
		Train(in_name,out_name);
	}


	///////////////////////////
	// �ε� HMM
	CContEHMM* phmm=new CContEHMM[num];
	
	for(i=0;i<num;i++)         // // // Load // // //
	{
		sprintf(in_name,"./face/%d-0.bmp",i);
		sprintf(out_name,"%s.hmm.txt",in_name);
		phmm[i].Load(out_name);
	}

	srand(time(NULL));
	int randnum;
	int res[30];
//	int next[10]; // �� ����� pca�� ���� 2�� �׽�Ʈ
	
	//Ȯ�� ��� ����
	int scount = 0;  //����
	int fcount = 0;  //����
	int wcount = 0;  //�Ӱ�ġ �ʰ�. �� �����ʹ� pca��
	
	

			// // // Test // // //

	//int temp;

//	for(int k=0; k<10; k++) // Ƚ��!!!
//	{

	
//	int ii[10] = {0, 0, 5, 5, 7, 7, 8, 8, 8, 9};
//	int jj[10] = {2, 1, 2, 1, 3, 2, 2, 3, 1, 1};

	int count = 0;
		for(i=0; i<num; i++)
		{
			for(j=1; j<=3; j++)
			{
				//randnum = rand()%3+1;
				sprintf(in_name,"./face/%d-%d.bmp", i, j);
				res[count] = Test(phmm, num, in_name);

				//		if(who == 0)      //
				//		{             //
				//printf("** %d ", *result);
				//		}             //
				//		else          //
				//		{             //
				//	FILE* fp = fopen("who.txt", "a");
				//	fprintf(fp, "%d-%d ", i, randnum);
				//	fclose(fp);
				//			res[count] = 1111;  //
				//		}   //

				count++;
			}
		}


		//Ȯ�� ���
		count = 0;
		for(i=0; i<num; i++)
		{
			for(j=1; j<=3; j++)
			{
				//	if(res[count] < 1000)   //
				//	{       //
				if(i != res[count])
				{
					fcount++;
				}
				else
				{
					scount++;
				}
				//		}          //
				//		else       //
				//		{           //
				//			wcount++;     //
				//		}          //
				count++;
			}
		}
		printf("* * * success : %d,   fail : %d,   who : %d\n", scount, fcount, wcount);
		
		count = 0;
		printf("\n result : ");
		for(i=0; i<num; i++)
		{
			for(j=1; j<=3; j++)
			{
				printf("%d ", res[count]);
				if(res[count] == 1111)
				{
					//FILE* fp = fopen("who.txt", "a");
					//fprintf(fp, "%d ", i);
					//fclose(fp);
				}
				count++;
			}
		}
		printf("\n");	
		
//		FILE* fp = fopen("who.txt", "a");
//		fprintf(fp, "\n");
//		fclose(fp);
		
	//} //
	
	float pro;
	pro = (float)scount / (scount + fcount + wcount);
	printf("         Pro : %f \n\n", pro);


	delete[] phmm;
}

int _tmain(int argc, _TCHAR* argv[])
{
	DoPCA();
	//DoHMM();


	return 0;
}