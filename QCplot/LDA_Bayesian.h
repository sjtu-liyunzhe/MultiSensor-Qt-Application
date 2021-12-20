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
 //没有不行，不明白
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
	//形参为vector
	bool Bayestrain(vector<vector<double> >& feature, vector<int>& label);
	int Bayespredict( vector<double>& x );
	//形参为mat
	bool Bayestrain(Mat& feature, Mat &label);
	int Bayespredict( Mat& x );
};