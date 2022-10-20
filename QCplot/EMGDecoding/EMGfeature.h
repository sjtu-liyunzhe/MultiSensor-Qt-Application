#ifndef EMG_FEATURE_H
#define EMG_FEATURE_H
#include <iostream>
#include <vector>


using namespace std;

class EMGfeature
{
private:
	vector<vector<double>> data;
	int channel;		// 通道数
	int length;			// 每个通道肌电数据长度
	double thresold;
public:
	EMGfeature(vector<vector<double>>& _data)
	{
		data = _data;
		channel = data.size();
		length = data[0].size();
		thresold = 10e-7;
	}
	vector<double> RMS()
	{
		vector<double> res;
		for (int i = 0; i < data.size(); ++i)
		{
			auto& row = data[i];
			double sum = 0;
			for(int j = 0; j < row.size(); ++j)
				sum += pow(row[j], 2);
			res.push_back(sqrt(sum / length));
		}
		return res;
	}
	vector<double> MAV()
	{
		vector<double> res;
		for(int i = 0; i < data.size(); ++i)
		{
			auto& row = data[i];
			double sum = 0;
			for(int j = 0; j < row.size(); ++j)
				sum += abs(row[j]);
			res.push_back(sum / length);
		}
		return res;
	}
	vector<double> WL()
	{
		vector<double> res;
		for(int i = 0; i < data.size(); ++i)
		{
			auto& row = data[i];
			double sum = 0;
			for (int i = 0; i < length - 1; ++i)
			{
				sum += abs(row[i + 1] - row[i]);
			}
			res.push_back(sum / length);
		}
		return res;
	}
	vector<double> ZC()
	{
		vector<double> res;
		for(int i = 0; i < data.size(); ++i)
		{
			auto& row = data[i];
			int count = 0;
			for (int i = 0; i < length - 1; ++i)
			{
				auto diff = row[i + 1] - row[i];
				auto multi = row[i + 1] * row[i];
				if (abs(diff) > thresold && multi < 0)
					++count;
			}
			res.push_back((double)count / length);
		}
		return res;
	}
	vector<double> SSC()
	{
		vector<double> res;
		for(int i = 0; i < data.size(); ++i)
		{
			auto& row = data[i];
			int count = 0;
			for (int i = 0; i < length - 2; ++i)
			{
				auto diff1 = row[i + 1] - row[i];
				auto diff2 = row[i + 2] - row[i + 1];
				if (abs(diff1) > thresold || abs(diff2) > thresold)
				{
					auto sign = diff1 * diff2;
					if (sign < 0)
						++count;
				}
			}
			res.push_back((double)count / length);
		}
		return res;
	}
};



#endif // !EMG_FEATURE_H

