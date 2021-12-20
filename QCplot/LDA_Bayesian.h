#pragma once
#include <iostream>
#include <fstream>
#include "eigen-3.2.1/Eigen/Dense"
#include<vector>
#include <string>
/*opencv  */
#include <ml.h>
#include<time.h>
#include <contrib/contrib.hpp>
#include <cxcore.hpp>
#include <core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <algorithm>
#include<vector>

using std::vector;
using std::string;
 //û�в��У�������
using namespace cv;

class LDA_Bayesian
{
private:
	vector<int> _mClassLabel;
	vector<vector<double> > ModelCov;
	vector<vector<double> > ModelMean;
public:
	LDA_Bayesian();
	~LDA_Bayesian();
	//�β�Ϊvector
	bool Bayestrain(vector<vector<double> >& feature, vector<int>& label);
	int Bayespredict( vector<double>& x );
	//�β�Ϊmat
	bool Bayestrain(Mat& feature, Mat &label);
	int Bayespredict( Mat& x );
};