#include "qcplot.h"

/*******************************************
���¼���������Ҫ����������ȡ����һ������
��ȡ����ͼ�����������������ͨ�����õڶ�
���͵�������������ȡ�����Ӵ��ڵ�������Ҳ
���Խ���һ���ཫ����д�ɾ�̬������
********************************************/
void getCoefficientImageFeature(const Mat & ,const QCplot::parametersSettings & ,Mat &);
Mat getEnvelopFeature(const Mat &);
void getWindowFeatureMeanVar(const Mat & windowData, Mat & windowFeature);
void getWindowFeatureCoefficient(const Mat & windowData, Mat & windowFeature);
void getWindowFeatureMean(const Mat& windowData, Mat& windowFeature);
//����ʹ��
void writeMatToFile(cv::Mat& matData, const char* filename);