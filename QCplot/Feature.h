#include "qcplot.h"

/*******************************************
如下几个函数主要用于特征提取，第一个函数
提取整个图像的特征，其中它又通过调用第二
个和第三个函数来提取各个子窗口的特征。也
可以建立一个类将他们写成静态方法。
********************************************/
void getCoefficientImageFeature(const Mat & ,const QCplot::parametersSettings & ,Mat &);
Mat getEnvelopFeature(const Mat &);
void getWindowFeatureMeanVar(const Mat & windowData, Mat & windowFeature);
void getWindowFeatureCoefficient(const Mat & windowData, Mat & windowFeature);
void getWindowFeatureMean(const Mat& windowData, Mat& windowFeature);
//调试使用
void writeMatToFile(cv::Mat& matData, const char* filename);