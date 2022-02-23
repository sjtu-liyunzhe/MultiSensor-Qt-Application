#ifndef IMUPROCESSER_H
#define IMUPROCESSER_H

#include <QThread>
#include <QSerialPort>
#include <vector>
#include <queue>

class IMUBuffer
{
public:
	IMUBuffer(int channelSize, int dataSize, int bufferSize);
	int channelSize, dataSize, bufferSize;		// len1, len2, len3
	std::vector<std::vector<std::queue<double>>> array;
	std::vector<std::vector<std::vector<double>>> vectorArray;
};

class IMUProcesser : public QThread
{
	Q_OBJECT
private:
	// 4 * 3 * 1000 4通道IMU的三个欧拉角数据
	// 通道号 * roll, pitch, yaw * 1000
	//std::vector<std::vector<std::vector<float>>> IMUDataPackage;
	
	std::vector<double> tempData;
	int channelCount;
	bool stopFlag;
	int dataLength;
protected:
	void run();
public:
	IMUBuffer* imuBuffer;
	QSerialPort* IMUSerialPort;
	QByteArray IMUSerialPortBuffer;
	QByteArray receiveBuffer;
	explicit IMUProcesser(QSerialPort* serialPort, QObject* parent = nullptr);
	void dataSave(QByteArray buffer);		// 数据缓存
	void dataUnpackage();		// 数据解包
	bool copyData(std::vector<std::vector<std::vector<float>>>& inputData);
	void stopThread();		// 停止线程
public slots:
	void onReadyProcessing();
};

#endif