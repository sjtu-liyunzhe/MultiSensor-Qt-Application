#ifndef EMG_TRAIN_H
#define EMG_TRAIN_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <Qstring>
#include "KDTree.h"
#include "EMGfeature.h"
#include <core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

#ifndef EMG_PREDICT_H
const int FEATURE_LEN = 20;
typedef vector<pair<kd::Point<FEATURE_LEN>, int>> dataset;
#endif // !EMG_PREDICT_H

class EMGTrain
{
private:
	int channelNum;
	int timeWindow;
	int strideWindow;
	int classes;
	vector<int> classes_vec;
	vector<vector<double>> EMGData;
	vector<int> labels;
	vector<vector<double>> trainData;
	dataset trainSet;
	vector<double> plus(vector<double>& v1, vector<double>& v2, vector<double>& v3, vector<double>& v4, vector<double>& v5)
	{
		vector<double> res;
		for (int i = 0; i < v1.size(); ++i) res.push_back(v1[i]);
		for (int i = 0; i < v2.size(); ++i) res.push_back(v2[i]);
		for (int i = 0; i < v3.size(); ++i) res.push_back(v3[i]);
		for (int i = 0; i < v4.size(); ++i) res.push_back(v4[i]);
		for (int i = 0; i < v5.size(); ++i) res.push_back(v5[i]);
		return res;
	}
	void transformData();
	kd::KDTree<FEATURE_LEN, int> kd;
	void buildKDTree();

	// OpenCV∑÷¿‡
	cv::Mat trainMat, labelsMat;
	void CVTransformData();
	// LDA
	cv::Ptr<CvSVM> svm;
	cv::Ptr<cv::LDA> lda;
	cv::Ptr<cv::KNearest> knn;
	//CvSVM svm;
public:
	EMGTrain()
	{
		channelNum = 4;
		timeWindow = 150;
		strideWindow = 100;
		classes = 6;
		EMGData.resize(4);
		for (int i = 1; i <= classes; ++i) classes_vec.push_back(i);
	}
	EMGTrain(int _timeWindow, int _strideWindow)
	{
		timeWindow = _timeWindow;
		strideWindow = _strideWindow;
		EMGData.resize(4);
		for (int i = 1; i <= classes; ++i) classes_vec.push_back(i);
	}
	void readData(QString fileName);
	void computeFeature();
	kd::KDTree<FEATURE_LEN, int> myKNNTrain();
	void LDATrain();
	void KNNTrain();
	void SVMTrain();
};
#endif