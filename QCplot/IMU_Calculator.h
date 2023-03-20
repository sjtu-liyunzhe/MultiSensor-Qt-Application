#ifndef IMU_CALCULATOR_H
#define IMU_CALCULATOR_H

#include <vector>
#include <stdint.h>

using namespace std;

#define G_ACC 9.80
#define INT16_NUM 65536
#define RESO_ACC 4 * G_ACC / 65536
#define RESO_GYRO 4000 / 65536

#define IIR_ORDER 5 

const double IIR_B[6] = {
	 5.979578037e-05,  0.00029897890185,   0.0005979578037,   0.0005979578037,
	0.00029897890185,   5.979578037e-05
};

const double IIR_A[6] = {
				   1,   -3.984543119612,    6.434867090276,   -5.253615170352,
	  2.165132909724,  -0.3599282450636
};

struct IIR_FILER
{
	double data_x[IIR_ORDER + 1];
	double data_y[IIR_ORDER + 1];
	int index;
	int length;
	IIR_FILER()
	{
		memset(data_x, 0, sizeof data_x);
		memset(data_y, 0, sizeof data_y);
		index = 0;
		length = 0;
	}
};

struct IMU_data
{
	int16_t r_ax, r_ay, r_az;
	int16_t r_gx, r_gy, r_gz;

	double accx, accy, accz;
	double gyrox, gyroy, gyroz;
	double roll, yaw, pitch;

	IMU_data(int16_t _ax, int16_t _ay, int16_t _az)
	{
		r_ax = _ax, r_ay = _ay, r_az = _az;
	}

	IMU_data(int16_t _ax, int16_t _ay, int16_t _az, int16_t _gx, int16_t _gy, int16_t _gz)
	{
		r_ax = _ax, r_ay = _ay, r_az = _az;
		r_gx = _gx, r_gy = _gy, r_gz = _gz;
	}
};
class IMU_Calculator
{
private:
	int16_t r_ax, r_ay, r_az;
	int16_t r_gx, r_gy, r_gz;
	IIR_FILER ax_filter, ay_filter, az_filter;

	void getReal()
	{
		accx = (double)RESO_ACC * r_ax;
		accy = (double)RESO_ACC * r_ay;
		accz = (double)RESO_ACC * r_az;
		gyrox = (double)RESO_GYRO * r_gx;
		gyroy = (double)RESO_GYRO * r_gy;
		gyroz = (double)RESO_GYRO * r_gz;
	}
	double accx, accy, accz;
	double gyrox, gyroy, gyroz;
	double roll, yaw, pitch;

public:
	void push(IMU_data& imu_data)
	{
		r_ax = imu_data.r_ax, r_ay = imu_data.r_ay, r_az = imu_data.r_az;
		r_gx = imu_data.r_gx, r_gy = imu_data.r_gy, r_gz = imu_data.r_gz;
		getReal();
		imu_data.accx = accx;
		imu_data.accy = accy;
		imu_data.accz = accz;
		//lowPass(ax_filter, accx, imu_data.accx);
		//lowPass(ay_filter, accy, imu_data.accy);
		//lowPass(az_filter, accz, imu_data.accz);
	}

	void lowPass(IIR_FILER& iir_filter, double x_new, double& result)
	{
		double tmp = 0;
		// 更新序列,x[0]、y[0]是最新值
		for (int i = IIR_ORDER; i > 0; --i)
		{
			iir_filter.data_x[i] = iir_filter.data_x[i - 1];
			iir_filter.data_y[i] = iir_filter.data_y[i - 1];
		}

		iir_filter.data_x[0] = x_new;
		for (int i = 0; i < IIR_ORDER + 1; ++i)
		{
			tmp += IIR_B[i] * iir_filter.data_x[i];
		}

		for (int i = 1; i < IIR_ORDER + 1; ++i)
		{
			tmp -= IIR_A[i] * iir_filter.data_y[i];
		}

		tmp /= IIR_A[0];
		iir_filter.data_y[0] = tmp;
		iir_filter.length++;
		if (iir_filter.length <= IIR_ORDER)
		{
			result = x_new;
			return;
		}
		result = iir_filter.data_y[0];
	}
};


#endif // !IMU_CALCULATOR_H
