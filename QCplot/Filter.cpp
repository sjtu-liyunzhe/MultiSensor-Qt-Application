#include "Filter.h"


extern const double EMG_IIR_B[6] = { 0.8158753202371,   -4.079376601186,    8.158753202371,   -8.158753202371,
	  4.079376601186,  -0.8158753202371 };
extern const double EMG_IIR_A[6] = { 1,   -4.593421399808,     8.45511522351,   -7.794918318044,
	  3.598902768054,  -0.6656525381714 };
// const double comb_rho = 0.4601747394;	// 带宽20
extern const double comb_rho = 0.1762159765;	// 带宽30
extern const double comb_b = (comb_rho + 1) / 2;

void EMG_IIR_Filter(EMG_IIR_FILER* iir_filter, double x_new, double& reslut)
{
	double tmp = 0;
	// 更新序列,x[0]、y[0]是最新值
	for (int i = EMG_IIR_ORDER; i > 0; --i)
	{
		iir_filter->data_x[i] = iir_filter->data_x[i - 1];
		iir_filter->data_y[i] = iir_filter->data_y[i - 1];
	}

	iir_filter->data_x[0] = x_new;
	for (int i = 0; i < EMG_IIR_ORDER + 1; ++i)
	{
		tmp += EMG_IIR_B[i] * iir_filter->data_x[i];
	}

	for (int i = 1; i < EMG_IIR_ORDER + 1; ++i)
	{
		tmp -= EMG_IIR_A[i] * iir_filter->data_y[i];
	}

	tmp /= EMG_IIR_A[0];
	iir_filter->data_y[0] = tmp;
	iir_filter->length++;
	if (iir_filter->length <= EMG_IIR_ORDER)
	{
		reslut = x_new;
		return;
	}
	reslut = iir_filter->data_y[0];
	return;
}

void EMG_COMB_Filter(EMG_COMB_FILER* comb_filter, double x_new, double& result)
{
	double tmp = 0;
	for (int i = EMG_COMB_ORDER; i > 0; --i)
	{
		comb_filter->data_x[i] = comb_filter->data_x[i - 1];
		comb_filter->data_y[i] = comb_filter->data_y[i - 1];
	}
	comb_filter->data_x[0] = x_new;
	tmp += comb_b * comb_filter->data_x[0] - comb_b * comb_filter->data_x[EMG_COMB_ORDER];
	tmp += comb_rho * comb_filter->data_y[EMG_COMB_ORDER];
	comb_filter->data_y[0] = tmp;
	comb_filter->length++;
	if (comb_filter->length <= EMG_COMB_ORDER)
	{
		result = x_new;
		return;
	}
	result = comb_filter->data_y[0];
}