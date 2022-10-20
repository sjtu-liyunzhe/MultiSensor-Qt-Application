#include "EMGTrain.h"

void EMGTrain::readData(QString fileName)
{
	vector<string> EMGFile(channelNum);
	vector<string> labelFile(channelNum);
	for (int i = 0; i < channelNum; ++i)
	{
		EMGFile[i] = ("TrainData\\" + fileName + "\\Channel_" + QString(i, 10) + "\\EMG " + fileName + " Exp.txt").toStdString();
		labelFile[i] = ("TrainData\\" + fileName + "\\Channel_" + QString(i, 10) + "\\EMG_Label " + fileName + " Exp.txt").toStdString();
	}
	// 读取4通道的肌电数据
	for (int i = 0; i < channelNum; ++i)
	{
		ifstream infile;
		infile.open(EMGFile[i]);
		double tmp = 0;
		while (!infile.eof())
		{
			infile >> tmp;
			EMGData[i].push_back(tmp);
		 }
		infile.close();
	}
	int minIndex = 0;
	int minLen = LLONG_MAX;
	for (int i = 0; i < channelNum; ++i)
	{
		if (EMGData[i].size() < minLen)
		{
			minIndex = i;
			minLen = EMGData[i].size();
		}
	}
	// 读取长度最短的标签
	vector<int> labelPoint;
	ifstream infile;
	infile.open(labelFile[minIndex]);
	double tmp = 0;
	while (!infile.eof())
	{
		infile >> tmp;
		labelPoint.push_back((int)tmp);
	}
	infile.close();

	for (int i = 0; i < minLen; ++i)
	{
		if (i < labelPoint[1]) labels.push_back(1);
		else if (i < labelPoint[2]) labels.push_back(2);
		else if (i < labelPoint[3]) labels.push_back(3);
		else if (i < labelPoint[4]) labels.push_back(4);
		else if (i < labelPoint[5]) labels.push_back(5);
		else if (i < labelPoint[6]) labels.push_back(6);
		else if (i < labelPoint[7]) labels.push_back(0);
		else if (i < labelPoint[8]) labels.push_back(1);
		else if (i < labelPoint[9]) labels.push_back(2);
		else if (i < labelPoint[10]) labels.push_back(3);
		else if (i < labelPoint[11]) labels.push_back(4);
		else if (i < labelPoint[12]) labels.push_back(5);
		else labels.push_back(6);
	}
}

void EMGTrain::computeFeature()
{
	for (int _name = 0; _name < classes_vec.size(); ++_name)
	{
		auto name = classes_vec[_name];
		vector<int> index;
		for (int i = 0; i < labels.size(); ++i)
		{
			if (labels[i] == name) index.push_back(i);
		}
		vector<vector<double>> iemg(channelNum);
		for (int i = 0;i < index.size(); ++i)
		{
			for (int c = 0; c < channelNum; ++c)
			{
				iemg[c].push_back(EMGData[c][index[i]]);
			}
		}
		int len = (iemg[0].size() - timeWindow) / strideWindow;
		for (int j = 0; j < len; ++j)
		{
			vector<vector<double>> temp(channelNum);
			for (int c = 0; c < channelNum; ++c)
			{
				temp[c] = vector<double>(iemg[c].begin() + strideWindow * j, iemg[c].begin() + strideWindow * j + timeWindow);
			}
			EMGfeature tempFeature = EMGfeature(temp);
			auto rms = tempFeature.RMS();
			auto mav = tempFeature.MAV();
			auto zc = tempFeature.ZC();
			auto ssc = tempFeature.SSC();
			auto wl = tempFeature.WL();
			auto featureStack = plus(rms, mav, wl, zc, ssc);
			featureStack.push_back(name);
			trainData.push_back(featureStack);
		}

	}
}

void EMGTrain::transformData()
{
	for (int _row = 0; _row < trainData.size(); ++_row)
	{
		auto& row = trainData[_row];
		vector<double> tmp(FEATURE_LEN);
		int tmpLabel = 0;
		for (int i = 0; i < FEATURE_LEN; ++i)
		{
			tmp[i] = row[i];
		}
		tmpLabel = (int)row[FEATURE_LEN];
		kd::Point<FEATURE_LEN> newPoint;
		copy(tmp.begin(), tmp.end(), newPoint.begin());
		//trainSet.push_back({ newPoint, tmpLabel });
		trainSet.push_back(make_pair(newPoint, tmpLabel));
	}
}

void EMGTrain::buildKDTree()
{
	kd = kd::KDTree<FEATURE_LEN, int>(trainSet);
	bool sanityPass = true;
	for (int i = 0; i < trainSet.size(); ++i)
	{
		if (!kd.contains(trainSet[i].first) || kd.kNNValue(trainSet[i].first, 1) != trainSet[i].second)
		{
			sanityPass = false;
			break;
		}
	}
	if (sanityPass) cout << "Sanity check PASSED!" << endl;
	else cout << "Sanity check FAILED!" << endl;
}

kd::KDTree<FEATURE_LEN, int> EMGTrain::myKNNTrain()
{
	transformData();
	buildKDTree();
	return kd;
}

void EMGTrain::CVTransformData()
{
	trainMat = cv::Mat(trainData.size(), trainData[0].size(), CV_32FC1);
	for (int i = 0; i < trainData.size(); ++i)
		for (int j = 0; j < trainData[0].size(); ++j)
			trainMat.at<float>(i, j) = (float)trainData[i][j];
	labelsMat = cv::Mat(labels.size(), 1, CV_32SC1);
	for (int i = 0; i < labels.size(); ++i)
		labelsMat.at<int>(i) = labels[i];
}

void EMGTrain::LDATrain()
{
	CVTransformData();
	lda = new cv::LDA(trainMat, labelsMat);
}

void EMGTrain::KNNTrain()
{
	CVTransformData();
	knn = new cv::KNearest();
	knn->train(trainMat, labelsMat);
}

void EMGTrain::SVMTrain()
{
	CVTransformData();
	CvSVMParams params = CvSVMParams();
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	svm = new CvSVM();
	svm->train_auto(trainMat, labelsMat, cv::Mat(), cv::Mat(), params);
}