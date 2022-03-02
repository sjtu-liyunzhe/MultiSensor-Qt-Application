#ifndef EMGPROCESSER_H
#define EMGPROCESSER_H

#include <QThread>
#include <QSerialPort>
#include <vector>
#include <queue>

class EMGBuffer
{
public:
	EMGBuffer(int channelSize, int bufferSize);
	int channelSize, bufferSize;
	std::vector<std::queue<double>> array;
	std::vector<std::vector<double>> vectorArray;
};

class EMGProcesser : public QThread
{
	Q_OBJECT
private:
	std::vector<double> tempData;
	int channelCount;
	bool stopFlag;
	int dataLength;
protected:
	void run();
public:
	EMGBuffer* emgBuffer;
	QSerialPort* EMGSerialPort;
	QByteArray EMGSerialPortBuffer;
	QByteArray receiveBuffer;
	explicit EMGProcesser(QSerialPort* serialPort, QObject* parent = nullptr);
	void dataUnpackage();
	void stopThread();
public slots:
	void onReadyProcessing();
};




#endif // !EMGPROCESSER_H
