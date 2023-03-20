#ifndef EMGPROCESSER_H
#define EMGPROCESSER_H

#include "IMU_Calculator.h"
#include "Filter.h"
#include <QThread>
#include <QSerialPort>
#include <QVector>
#include <vector>
#include <queue>
#include <deque>
#include <stdint.h>
//#include <Qque>

//extern const int oriSize = 1000;
//extern const unsigned short int packageSize = 26;
class EMGBuffer
{
public:
	EMGBuffer(int channelSize, int bufferSize);
	int channelSize, bufferSize;
	std::vector<std::queue<double>> array;
	std::vector<std::vector<double>> vectorArray;
	std::vector<std::deque<double>> deArray;
	std::vector<uint8_t> packageNumArray;
	QVector<double> Data_x;
	void saveData(const int& channel, double data);
};
class UnionIMUBuffer
{
public:
	UnionIMUBuffer(int channelSize, int dataSize, int bufferSize);
	int channelSize, dataSize, bufferSize;		// len1, len2, len3
	std::vector<std::vector<std::queue<double>>> array;
	std::vector<std::vector<std::vector<double>>> vectorArray;
	std::vector<std::vector<std::deque<double>>> deArray;
	std::vector<uint8_t> packageNumArray;
	QVector<double> Data_x;
	void saveData(const int& channel, const int& dataNum, double data);
	void saveData(const int& channel, IMU_data& data);
	IMU_Calculator* calculator;
};
class EMGProcesser : public QThread
{
	Q_OBJECT
private:
	std::vector<double> tempData;
	int channelCount;
	bool stopFlag;
	int dataLength;
	std::vector<EMG_IIR_FILER> iir_filter_array;
	std::vector<EMG_COMB_FILER> comb_filter_array;

protected:
	void run();
public:
	EMGBuffer* emgBuffer;
	UnionIMUBuffer* imuBuffer;
	QSerialPort* EMGSerialPort;
	QByteArray EMGSerialPortBuffer;
	QByteArray receiveBuffer;
	explicit EMGProcesser(QSerialPort* serialPort, QObject* parent = nullptr);
	~EMGProcesser();
	void dataUnpackage();
	void unionDataUnpackage();
	void stopThread();
	void dataBufferUnpackage();
signals:
	void updateEMGSignal();
public slots:
	void onReadyProcessing();
	void onReadyProcessing_16bit();

};




#endif // !EMGPROCESSER_H
