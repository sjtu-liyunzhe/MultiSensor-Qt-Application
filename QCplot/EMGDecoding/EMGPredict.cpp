#include "EMGPredict.h"

void EMGPredict::computeFeature()
{
	EMGfeature tempFeature = EMGfeature(testData);
	auto rms = tempFeature.RMS();
	auto mav = tempFeature.MAV();
	auto zc = tempFeature.ZC();
	auto ssc = tempFeature.SSC();
	auto wl = tempFeature.WL();
	testFeature = plus(rms, mav, wl, zc, ssc);
	transformData();
}
void EMGPredict::transformData()
{
	copy(testFeature.begin(), testFeature.end(), predictPoint.begin());
}

int EMGPredict::KNNPredict(size_t k)
{
	int pred = kd.kNNValue(predictPoint, k);
	return pred;
}