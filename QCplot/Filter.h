#ifndef __FILTER_H
#define __FILTER_H

#include <stdint.h>
#include <vector>

#define PLF_L 80
#define EMG_IIR_ORDER 5   
#define EMG_COMB_ORDER 20



struct EMG_IIR_FILER
{
public:
	std::vector<double> data_x;
	std::vector<double> data_y;
	bool flag;
	int index;
	int length;
	EMG_IIR_FILER()
	{
		data_x = std::vector<double>(EMG_IIR_ORDER + 1);
		data_y = std::vector<double>(EMG_IIR_ORDER + 1);
		flag = false;
		index = 0;
		length = 0;
	}
};

class EMG_COMB_FILER
{
public:
	std::vector<double> data_x;
	std::vector<double> data_y;
	bool flag;
	unsigned char index;
	int length;
	EMG_COMB_FILER()
	{
		data_x = std::vector<double>(EMG_COMB_ORDER + 1);
		data_y = std::vector<double>(EMG_COMB_ORDER + 1);
		flag = false;
		index = 0;
		length = 0;
	}
};

void EMG_IIR_Filter(EMG_IIR_FILER* iir_filter, double x_new, double& reslut);


void EMG_COMB_Filter(EMG_COMB_FILER* comb_filter, double x_new, double& result);

#endif // !_FILTER_H
