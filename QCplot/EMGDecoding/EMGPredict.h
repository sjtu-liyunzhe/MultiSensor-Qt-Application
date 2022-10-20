#ifndef EMG_PREDICT_H
#define EMG_PREDICT_H
#include <iostream>
#include <vector>
#include <fstream>
#include "KDTree.h"
#include "EMGfeature.h"
#include <core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;

#ifndef EMG_TRAIN_H
const int FEATURE_LEN = 20;
typedef vector<pair<kd::Point<FEATURE_LEN>, int>> dataset;
#endif // !EMG_TRAIN_H


class EMGPredict
{
private:
	kd::KDTree<FEATURE_LEN, int> kd;
	vector<vector<double>> testData;
	int realLabel;
	vector<double> testFeature;
	kd::Point<FEATURE_LEN> predictPoint;
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
public:
	EMGPredict(vector<vector<double>> _testData, kd::KDTree<FEATURE_LEN, int> _kd, int _label)
	{
		testData = _testData;
		kd = _kd;
		realLabel = _label;
	}
	void computeFeature();
	int KNNPredict(size_t k);
};


#endif