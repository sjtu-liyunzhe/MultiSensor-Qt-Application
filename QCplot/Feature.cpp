#include "Feature.h"
/*******************************************
�������ƣ�getImageFeature
��    �ܣ����������ͼ������Ӧͼ�������
��    ����ͼ�����������     
����ֵ  ������ͼ�������������
********************************************/

void getCoefficientImageFeature(const Mat & grayImage, const QCplot::parametersSettings & currentParametersSettings, Mat & featureVector)
{
	//1ʱ�����油��
	float alpha = 0.05;
	float fre = 5;					//MHz
	int sample_length = 1000;		
	int chnum = 8;
	int start = 20, endo = 20;		//ͷβ��ȥ��20���� 
	float sample_depth = 3.85;		//cm
	Mat tgc(1,sample_length ,CV_32FC1);
	for (int i = 0; i<sample_length; i++)
	{
		float temp = exp(alpha*fre*sample_depth*i/sample_length);
		tgc.at<float>(0,i) = temp;
	}
	Mat tgcMatrix(chnum, sample_length, CV_32FC1);
	repeat(tgc,currentParametersSettings.channelNumber, 1,tgcMatrix);
	Mat averageMatrix(chnum, sample_length, CV_32FC1, 128.0);					//��Matlab�У���ֵʹ��128��Ч������   
	Mat tgcData = tgcMatrix.mul(grayImage - averageMatrix); 
	//2�Ľװ�����˹��ͨ�˲����������״��Ӷ���
	//double SOS[2][6] = {1, 0, -1, 1, 1.1201,0.4425, 1, 0,-1, 1, -1.1201, 0.4425};
	//double Gain[2] = {0.6256, 0.6256};
	//double order = 2;
	//Mat ButterworthTemp(chnum, sample_length, CV_32FC1);
	//for (int i = 0; i<chnum; i++)																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																				
	//{
	//	for (int j=0; j<sample_length; j++)
	//	{
	//		if (j<order)
	//		{
	//			ButterworthTemp.at<float>(i,j) = tgcData.at<float>(i,j);
	//		}
	//		else
	//		{
	//			ButterworthTemp.at<float>(i,j) = Gain[0]*(SOS[0][0]*tgcData.at<float>(i,j)+SOS[0][1]*tgcData.at<float>(i,j-1)+SOS[0][2]*tgcData.at<float>(i,j-2))-SOS[0][4]*ButterworthTemp.at<float>(i,j-1)-SOS[0][5]*ButterworthTemp.at<float>(i,j-2);
	//		}
	//	}
	//	for (int j=0; j<sample_length; j++)
	//	{
	//		if (j<order)
	//		{
	//			tgcData.at<float>(i,j) = ButterworthTemp.at<float>(i,j);
	//		}
	//		else
	//		{
	//			tgcData.at<float>(i,j) = Gain[1]*(SOS[1][0]*ButterworthTemp.at<float>(i,j)+SOS[1][1]*ButterworthTemp.at<float>(i,j-1)+SOS[1][2]*ButterworthTemp.at<float>(i,j-2))-SOS[1][4]*tgcData.at<float>(i,j-1)-SOS[1][5]*tgcData.at<float>(i,j-2);
	//		}
	//	}
	//}
	//3����ֵ����
	Mat envelopeData = abs(tgcData);
	//4ƽ���˲�			//~ ��ͼ���У����Կ����Ǹ�˹�˲���ÿһ�����ص��ֵ�������䱾��������ڵ���������ֵ������Ȩƽ����õ���
	//��ǰ5���ƽ��ֵ���浱ǰֵ
	int filter_points = 5;
	for (int i = 0; i<chnum; i++)
	{
		Mat temp = envelopeData.row(i);
		for (int j = filter_points; j< sample_length - filter_points; j++)
		{
			float sum =0;
			int n = j;
			for (int p =0; p < filter_points; p++)
			{
				sum += temp.at<float>(0, n--);
			}
			envelopeData.at<float>(i, j) = sum/filter_points;
		}
	}
	//5����ѹ��
	Mat log_comp_Matrix(chnum, sample_length, CV_32FC1);
	float log_comp_ratio = 0.4;
	for (int i = 0;i< chnum; i++)
	{
		for (int j = 0; j<sample_length; j++)
		{
			log_comp_Matrix.at<float>(i, j) = log10f(1 + log_comp_ratio*envelopeData.at<float>(i, j))/log10f(1+log_comp_ratio);
		}
	}

	Mat rawData1 = log_comp_Matrix.colRange(start,sample_length - endo);						//ǰ���ȥ��20���㣬colRange ȡ�Ҳ�ȡ��
	if (currentParametersSettings.featureType == "envelop feature")
	{
		featureVector = getEnvelopFeature(rawData1);
	}
	else
	{
		int step = currentParametersSettings.windowLength-currentParametersSettings.overlap;		//��ȡ�ִ��Ĳ���
		int windowNumber = (rawData1.cols - currentParametersSettings.windowLength)/step + 1;		//��ȡÿ�еĴ�������
		Mat currentImageFeature(currentParametersSettings.channelNumber,2 * windowNumber,CV_32FC1); //��������ͼ����������� k/b
		//Mat currentImageFeature(currentParametersSettings.channelNumber,windowNumber,CV_32FC1); //��������ͼ����������� mean only
		for (int i = 0; i < currentParametersSettings.channelNumber; i ++)							//���forѭ�����㣨ÿһͨ�������ݵ�����
		{
			Mat currentRowData = rawData1.row(i);													//Yang��ֱ���滻ΪA��������
			Mat currentColumnFeature(2,windowNumber,CV_32FC1);
			for (int j=0; j < windowNumber; j++ )													//�ڲ�forѭ������ÿһ���������е�����
			{
				//Mat windowData = currentRowData(Range(j * step, j * step + 
					//currentParametersSettings.windowLength),Range::all());		
				Mat windowData = currentRowData.colRange(j * step, j * step + 
					currentParametersSettings.windowLength);										//ȡ��j��������һ����Ϊ1 - 20
				Mat windowFeature;
				if (currentParametersSettings.featureType == "mean and variance")					//�ж�ʹ������������Ҳ���Լ����Զ����������ȡ������
				{
					getWindowFeatureMeanVar(windowData, windowFeature);
				}
				else if(currentParametersSettings.featureType == "fitting coefficient")
				{
					getWindowFeatureCoefficient(windowData, windowFeature);
				}
				else if (currentParametersSettings.featureType == "mean")
				{
					getWindowFeatureMean(windowData, windowFeature);
				}
				windowFeature.copyTo(currentColumnFeature.col(j));
			}
			Mat currentColumnFeatureRow = currentColumnFeature.reshape(0,1);						//��������������һ�У�����ƴ��
			//Mat currentColumnFeatureRow = currentColumnFeature.row(0);						//ֻ������һ�еľ�ֵ
			currentColumnFeatureRow.copyTo(currentImageFeature.row(i));		
		}
		featureVector = currentImageFeature.reshape(0,1);											//������ƴ�ӻ����������������أ�����Ϊ10������
		//writeMatToFile(featureVector, ".\\Mat_file\\featureVector.txt");
	}
}


/*******************************************
�������ƣ�getWindowFeatureMeanVar
��    �ܣ����������ͼ��ĸ������е����ݼ����Ӧ�õ��������˴�������Ǿ�ֵ�뷽������
��    �����������е�Mat����     
����ֵ  ������������������
********************************************/
void getWindowFeatureMeanVar(const Mat & windowData, Mat & windowFeature)
{
	/*
	Mat windowMean,windowStd;
	meanStdDev(windowData,windowMean,windowStd);
	//vconcat������������ƴ�ӣ�hconcat��������ƴ��
	vconcat(windowMean,windowStd,windowFeature);
	*/
	//����ķ�������ʱ���е㳤���Լ�дһ������
	float mean  = 0;
	float var = 0;
	float sum = 0;
	float sqare_sum = 0;
	int windowDataLength = windowData.cols;
	for (int i=0; i<windowDataLength; i++)
	{
		sum += windowData.at<float>(0, i);
	}
	mean = sum/windowDataLength;
	for (int i=0; i<windowDataLength; i++)
	{
		float xi = windowData.at<float>(0, i);
		sqare_sum += (xi - mean)*(xi - mean);
	}
	var = sqare_sum/(windowDataLength - 1);
	windowFeature = (Mat_<float>(2,1) << mean, var);
}

/*******************************************
�������ƣ�getWindowFeatureMean
��    �ܣ����������ͼ��ĸ������е����ݼ����Ӧ�õ��������˴�������Ǿ�ֵ����
��    �����������е�Mat����
����ֵ  ������������������
********************************************/
void getWindowFeatureMean(const Mat& windowData, Mat& windowFeature)
{
	float mean = 0;
	float sum = 0;
	int windowDataLength = windowData.cols;
	for (int i = 0; i < windowDataLength; i++)
	{
		sum += windowData.at<float>(0, i);
	}
	mean = sum / windowDataLength;
	
	windowFeature = (Mat_<float>(1, 1) << mean);
}



/*******************************************
�������ƣ�getWindowFeatureMeanVar
��    �ܣ����������ͼ��ĸ������е����ݼ����Ӧ�õ��������˴�����������ϵ��������
��    �����������е�Mat����     
����ֵ  ������������������
********************************************/
void getWindowFeatureCoefficient(const Mat & windowData, Mat & windowFeature)
{

	float x_mean = 0;
	float x_sum = 0;
	for (int i=0;i<windowData.cols;i++)
	{
		x_sum += (i + 1);
	}
	x_mean = x_sum/windowData.cols;
	float y_mean = 0;
	float y_sum = 0;
	for (int i=0;i<windowData.cols;i++)
	{
		y_sum += windowData.at<float>(0, i);
	}
	y_mean = y_sum/windowData.cols;
	float lxy = 0,lxx = 0;
	for (int i=0;i<windowData.cols;i++)
	{
		float yi = windowData.at<float>(0, i);
		lxy += ((i+1)-x_mean) * (yi-y_mean);
		lxx += ((i+1)-x_mean) * ((i+1)-x_mean);
	}
	float k = lxy/lxx;
	float b = y_mean - k*x_mean;																//���չ�ʽ�������ֱ��y=kx+b��ϵ��k��b
	windowFeature = (Mat_<float>(2,1) << k,b);													//�򵥾صĳ�ʼ��,�õ���k��bֵ��matlab polyfit�õ���ֵ��ͬ
	//writeMatToFile(windowFeature, "windowFeature.txt");
}

/*******************************************
�������ƣ�writeMatToFile
��    �ܣ����Ե�ʱ���ӡ������
��    ������    
����ֵ  ����
********************************************/
void writeMatToFile(cv::Mat& matData, const char* filename)
{
	std::ofstream fout(filename,std::ios::out/*,std::ios_base::app*/);
	if(!fout)
	{
		std::cout<<"File Not Opened"<<endl;  return;
	}

	//fout << "begin of file\n";

	for(int i=0; i<matData.rows; i++)
	{
		for(int j=0; j<matData.cols; j++)
		{
			float signalData = matData.at<float>(i,j);	
			fout<<signalData<<'\t';						//��һ����
		}
		fout<<std::endl;
	}
	//fout << "end of file";
	fout.close();

}

Mat getEnvelopFeature(const Mat & EnvelopMatrix)
{
	Mat featureVector;
	Mat EnvelopMatrix_resize;
	//ѹ����������
	cv::resize(EnvelopMatrix, EnvelopMatrix_resize, Size(400,8),0,0,CV_INTER_LINEAR);
	//������������һ��
	featureVector = EnvelopMatrix_resize.reshape(0,1);	
	//writeMatToFile(featureVector, ".\\Mat_file\\featureVector.txt");
	return featureVector;
}