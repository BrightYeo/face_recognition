

#include "stdafx.h"
#include "MyHMM.h"

int m_stnum[32] = {5,3,6,6,6,3};
int m_mixnum[128]; 

CvSize m_obsSize=cvSize(3,3);// ?
CvSize m_dctSize=cvSize(12,12);//  ?
CvSize m_delta=cvSize(4,4);// ?
 
void DEBUG(IplImage* ipl)
{
	cvNamedWindow("xx",0);
	cvShowImage("xx",ipl);

	cvWaitKey(0);
}



void Train(char* in_name, char* out_name)
{
	CContEHMM m_hmm;
	int i;

	//�Ķ���� ���� 
	
	int vect_len=m_obsSize.width * m_obsSize.height;  

	for(i=0;i<128;i++)
	{
		m_mixnum[i]=3;
	}

	////////////////////////////////////////
	CvEHMM* hmm = m_hmm.GetIppiEHMM();
	
	if (!hmm)
	{
		m_hmm.CreateHMM( m_stnum, m_mixnum, vect_len );
	}

	
    hmm = m_hmm.GetIppiEHMM();
 

	//Ʈ���̴� 
	int num_img = 1; //�̹��� ���� 

	CvImgObsInfo** obs_info_array; //
	//CvImgObsInfo** obs_info_array = obs_info.GetData();
	obs_info_array = new CvImgObsInfo*[num_img];
	//obs_info_array[0]=new CvImgObsInfo;
	


	IplImage* ipl_scaled;
	CvImgObsInfo* info;	
    CvSize num_obs;	

	for(i=0;i<num_img;i++)
	{		
		
		IplImage* ipl=cvLoadImage(in_name,0);
		//�̹��� ũ�� �ٽ� ���� �ϰ�, gray 
		ipl_scaled=ipl;

		CvSize roi = cvSize( ipl_scaled->roi ? ipl_scaled->roi->width : ipl_scaled->width,
                               ipl_scaled->roi ? ipl_scaled->roi->height : ipl_scaled->height); //60, 70

		//DEBUG(ipl_scaled);

        CV_COUNT_OBS( &roi, &m_dctSize, &m_delta, &num_obs ); 
		obs_info_array[i]=cvCreateObsInfo(num_obs,vect_len);
              
        info = obs_info_array[i];


		cvImgToObs_DCT( /*normalized_image*/
			ipl_scaled, 
			info->obs, 
			m_dctSize, 
			m_obsSize,
			m_delta );


		//cvReleaseImage( &ipl_scaled );
		cvUniformImgSegm( info, hmm );
	}

	cvInitMixSegm( obs_info_array, num_img, hmm );

	////////////////////////////////////////////////////////
	int max_iterations=80;

	bool trained = false;
    float old_likelihood = 0;
    
    
    int counter = 0;

    while( (!trained) && (counter < max_iterations) )
    { 
        counter++;

        int j;


		cvEstimateHMMStateParams( obs_info_array, num_img, hmm);

        cvEstimateTransProb( obs_info_array, num_img, hmm); 
         
        float likelihood = 0;     
        
		
		for( j = 0; j < num_img; j++ ) //�̹��� ������ŭ ����
        {           
            cvEstimateObsProb( obs_info_array[j], hmm );
            likelihood += cvEViterbi( obs_info_array[j], hmm );
        }


        likelihood /= num_img*obs_info_array[0]->obs_size;

        cvMixSegmL2( obs_info_array, num_img, hmm);

        trained = ( fabs(likelihood - old_likelihood) < 0.01 ); 
        old_likelihood = likelihood;        

	}

	for(i = 0; i < num_img; i++ )
   {
        cvReleaseObsInfo( &(obs_info_array[i]) );
   }

  //////////////////////////////////////////////////
  //  
  m_hmm.Save(out_name); //���Ϸ� ����
}




int Test(CContEHMM* phmm, int num, char* in_name)
{
	int i;
	CContEHMM* m_phmm = phmm; 
	
	IplImage* ipl=cvLoadImage(in_name, 0);

	CvRect rect=cvRect(0,0,ipl->width,ipl->height);//(0,0,320,240); //?? �׽�Ʈ ���󿡼� �׽�Ʈ ���� 
	
	cvSetImageROI(ipl, rect);
	
	IplImage* ipl_scaled; 	
	ipl_scaled=ipl; 
	//DEBUG(ipl_scaled);
	//return;

	/////////////////////////
	CvSize cvroi = cvSize(ipl_scaled->roi ? ipl_scaled->roi->width : ipl_scaled->width,
		ipl_scaled->roi ? ipl_scaled->roi->height : ipl_scaled->height);
    
    CvSize num_obs;
    CvImgObsInfo* info;
	
	CV_COUNT_OBS( &cvroi, &m_dctSize, &m_delta, &num_obs ); 
	
    int vect_len = m_obsSize.height*m_obsSize.width;//////////////////////
	
	info = cvCreateObsInfo( num_obs, vect_len );
	
	cvImgToObs_DCT( /*normalized_image*/ipl_scaled, info->obs, m_dctSize, m_obsSize, m_delta );
	
	//cvReleaseImage( &ipl_scaled );
	
	float max_like = -100000000; 
	int numSave = num;
	
	CvEHMM* hmm;
	float like_array[1000];
	
	for(i=0;i<numSave;i++)
	{
		hmm = m_phmm[i].GetIppiEHMM();
		cvEstimateObsProb( info, hmm ); //�� �Լ�
		like_array[i] = cvEViterbi( info, hmm ); //cvEViterbi �Լ� , �� Ȯ���� (-) �� ��������
		//printf("** like : %f **\n", like_array[i]);
	}
	cvReleaseObsInfo( &info );
	
	/////////////////////
	//���� 
	int max_index = i;
	for(i=0;i<numSave;i++)
	{
		//printf("* * like_array[max_index] : %f, like_array[i] : %f * * \n", like_array[max_index], like_array[i]); //like_array�� ���

		if(like_array[max_index] < like_array[i])
		{
			max_index = i;
			result = i;
		}
	}

	who = 0;

	char file_name[80];

	if(like_array[max_index] < -70) // �Ӱ�ġ�� -70 ���� ���� ��
	{
		// �Ӱ�ġ�� �ʰ��ϸ� -1 �̶�� ������ ����� ������ �����. => �׷� �Ϲ���
		system("copy ..\\hmm_test.bmp ..\\result\\-1.bmp");
		system("del ..\\hmm_test.bmp");
	}
	else
	{
		// �Ӱ�ġ ���� ������ max_index�� ��ȣ�� �ϴ� ������ �����.
		sprintf(file_name, "copy ..\\hmm_test.bmp ..\\result\\%d.bmp", max_index);
		system(file_name);
		system("del ..\\hmm_test.bmp");
	}
	
	printf("max index: %d pro: %.2f\n", max_index, like_array[max_index]);

	return result;
}
