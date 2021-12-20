#include "Feature.h"
/*******************************************
函数名称：getImageFeature
功    能：根据输入的图像计算对应图像的特征
参    数：图像和特征参数     
返回值  ：单幅图像的特征行向量
********************************************/

void getCoefficientImageFeature(const Mat & grayImage, const QCplot::parametersSettings & currentParametersSettings, Mat & featureVector)
{
	//1时变增益补偿
	float alpha = 0.05;
	float fre = 5;					//MHz
	int sample_length = 1000;		
	int chnum = 8;
	int start = 20, endo = 20;		//头尾各去除20个点 
	float sample_depth = 3.85;		//cm
	Mat tgc(1,sample_length ,CV_32FC1);
	for (int i = 0; i<sample_length; i++)
	{
		float temp = exp(alpha*fre*sample_depth*i/sample_length);
		tgc.at<float>(0,i) = temp;
	}
	Mat tgcMatrix(chnum, sample_length, CV_32FC1);
	repeat(tgc,currentParametersSettings.channelNumber, 1,tgcMatrix);
	Mat averageMatrix(chnum, sample_length, CV_32FC1, 128.0);					//在Matlab中，中值使用128，效果更好   
	Mat tgcData = tgcMatrix.mul(grayImage - averageMatrix); 
	//2四阶巴特沃斯带通滤波，两个二阶串接而成
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
	//3绝对值包络
	Mat envelopeData = abs(tgcData);
	//4平滑滤波			//~ 在图像中，可以看做是高斯滤波（每一个像素点的值，都由其本身和邻域内的其他像素值经过加权平均后得到）
	//用前5点的平均值代替当前值
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
	//5对数压缩
	Mat log_comp_Matrix(chnum, sample_length, CV_32FC1);
	float log_comp_ratio = 0.4;
	for (int i = 0;i< chnum; i++)
	{
		for (int j = 0; j<sample_length; j++)
		{
			log_comp_Matrix.at<float>(i, j) = log10f(1 + log_comp_ratio*envelopeData.at<float>(i, j))/log10f(1+log_comp_ratio);
		}
	}

	Mat rawData1 = log_comp_Matrix.colRange(start,sample_length - endo);						//前后各去除20个点，colRange 取右不取左
	if (currentParametersSettings.featureType == "envelop feature")
	{
		featureVector = getEnvelopFeature(rawData1);
	}
	else
	{
		int step = currentParametersSettings.windowLength-currentParametersSettings.overlap;		//获取分窗的步长
		int windowNumber = (rawData1.cols - currentParametersSettings.windowLength)/step + 1;		//获取每列的窗口数量
		Mat currentImageFeature(currentParametersSettings.channelNumber,2 * windowNumber,CV_32FC1); //构建整幅图像的特征矩阵， k/b
		//Mat currentImageFeature(currentParametersSettings.channelNumber,windowNumber,CV_32FC1); //构建整幅图像的特征矩阵， mean only
		for (int i = 0; i < currentParametersSettings.channelNumber; i ++)							//外层for循环计算（每一通道）数据的特征
		{
			Mat currentRowData = rawData1.row(i);													//Yang，直接替换为A超列数据
			Mat currentColumnFeature(2,windowNumber,CV_32FC1);
			for (int j=0; j < windowNumber; j++ )													//内层for循环计算每一窗口数据中的特征
			{
				//Mat windowData = currentRowData(Range(j * step, j * step + 
					//currentParametersSettings.windowLength),Range::all());		
				Mat windowData = currentRowData.colRange(j * step, j * step + 
					currentParametersSettings.windowLength);										//取第j个窗，第一个窗为1 - 20
				Mat windowFeature;
				if (currentParametersSettings.featureType == "mean and variance")					//判断使用哪种特征（也可以加入自定义的特征提取函数）
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
			Mat currentColumnFeatureRow = currentColumnFeature.reshape(0,1);						//把特征向量拉成一行，按行拼接
			//Mat currentColumnFeatureRow = currentColumnFeature.row(0);						//只保留第一行的均值
			currentColumnFeatureRow.copyTo(currentImageFeature.row(i));		
		}
		featureVector = currentImageFeature.reshape(0,1);											//将特征拼接汇总拉成行向量返回，长度为10倍窗数
		//writeMatToFile(featureVector, ".\\Mat_file\\featureVector.txt");
	}
}


/*******************************************
函数名称：getWindowFeatureMeanVar
功    能：根据输入的图像的给定窗中的数据计算对应得的特征，此处计算的是均值与方差数据
参    数：给定窗中的Mat数组     
返回值  ：给定窗的特征向量
********************************************/
void getWindowFeatureMeanVar(const Mat & windowData, Mat & windowFeature)
{
	/*
	Mat windowMean,windowStd;
	meanStdDev(windowData,windowMean,windowStd);
	//vconcat将两个矩阵按列拼接，hconcat将矩阵按行拼接
	vconcat(windowMean,windowStd,windowFeature);
	*/
	//上面的方法计算时间有点长，自己写一个试试
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
函数名称：getWindowFeatureMean
功    能：根据输入的图像的给定窗中的数据计算对应得的特征，此处计算的是均值数据
参    数：给定窗中的Mat数组
返回值  ：给定窗的特征向量
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
函数名称：getWindowFeatureMeanVar
功    能：根据输入的图像的给定窗中的数据计算对应得的特征，此处计算的是拟合系数的特征
参    数：给定窗中的Mat数组     
返回值  ：给定窗的特征向量
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
	float b = y_mean - k*x_mean;																//按照公式计算拟合直线y=kx+b的系数k和b
	windowFeature = (Mat_<float>(2,1) << k,b);													//简单矩的初始化,得到的k，b值和matlab polyfit得到的值相同
	//writeMatToFile(windowFeature, "windowFeature.txt");
}

/*******************************************
函数名称：writeMatToFile
功    能：调试的时候打印出矩阵
参    数：无    
返回值  ：无
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
			fout<<signalData<<'\t';						//空一个格
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
	//压缩特征矩阵
	cv::resize(EnvelopMatrix, EnvelopMatrix_resize, Size(400,8),0,0,CV_INTER_LINEAR);
	//将特征拉长成一行
	featureVector = EnvelopMatrix_resize.reshape(0,1);	
	//writeMatToFile(featureVector, ".\\Mat_file\\featureVector.txt");
	return featureVector;
}