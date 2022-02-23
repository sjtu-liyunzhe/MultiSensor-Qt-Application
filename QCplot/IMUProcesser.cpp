#include "IMUProcesser.h"
#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <QDebug>
using namespace std;

IMUBuffer::IMUBuffer(int channelSize, int dataSize, int bufferSize)
{
	this->channelSize = channelSize;
	this->dataSize = dataSize;
	this->bufferSize = bufferSize;
	array.resize(this->channelSize);
	vectorArray.resize(this->channelSize);
	for (int i = 0; i < this->channelSize; i++)
	{
		array[i].resize(this->dataSize);
		vectorArray[i].resize(this->dataSize);
	}
	// ²»ÐèÒª
	/*for (int i = 0; i < this->channelSize; i++)
	{
		for (int j = 0; j < this->dataSize; j++)
		{
			vectorArray[i][j].resize(this->bufferSize);
		}
	}*/
}

IMUProcesser::IMUProcesser(QSerialPort* serialPort, QObject* parent) : QThread(parent)
{
	IMUSerialPort = new QSerialPort();
	IMUSerialPort = serialPort;
	tempData.resize(1000);
	channelCount = 1;
	dataLength = 22;
	imuBuffer = new IMUBuffer(4, 3, 100);
}
void IMUProcesser::dataSave(QByteArray buffer)
{
	;
}
void IMUProcesser::dataUnpackage()
{
	int16_t tempRoll = 0, tempPitch = 0, tempYaw = 0;
	unsigned char rollHigh = 0, rollLow = 0, pitchHigh = 0, pitchLow = 0, yawHigh = 0, yawLow = 0;
	int channel = 0;
	if (IMUSerialPortBuffer[0].operator == (0x88))
	{
		unsigned char sum = 0;
		for (int i = 0; i < IMUSerialPortBuffer.length(); i++)
		{
			sum += IMUSerialPortBuffer[i];
		}
		if (IMUSerialPortBuffer[1].operator== (0xA1) || IMUSerialPortBuffer[1].operator== (0xA2) || IMUSerialPortBuffer[1].operator== (0xA3) || IMUSerialPortBuffer[1].operator== (0xA4))
		//if (IMUSerialPortBuffer[dataLength - 1].operator== (sum))
		{
			channel = IMUSerialPortBuffer[1];
			rollHigh = IMUSerialPortBuffer[15];
			rollLow = IMUSerialPortBuffer[16];
			pitchHigh = IMUSerialPortBuffer[17];
			pitchLow = IMUSerialPortBuffer[18];
			yawHigh = IMUSerialPortBuffer[19];
			yawHigh = IMUSerialPortBuffer[20];
			
			tempRoll = ((rollHigh << 8) | rollLow) / 100;
			tempPitch = ((pitchHigh << 8) | pitchLow) / 100;
			tempYaw = ((yawHigh << 8) | yawLow) / 100;

			if (IMUSerialPortBuffer[1].operator== (0xA1))
			{
				channel = 0;
			}
			else if (IMUSerialPortBuffer[1].operator== (0xA2))
			{
				channel = 1;
			}
			else if (IMUSerialPortBuffer[1].operator== (0xA3))
			{
				channel = 2;
			}
			else if (IMUSerialPortBuffer[1].operator== (0xA4))
			{
				channel = 3;
			}
			imuBuffer->array[channel][0].push(tempRoll);
			imuBuffer->array[channel][1].push(tempPitch);
			imuBuffer->array[channel][2].push(tempYaw);
			//qDebug() << tempRoll << endl;
		}
	}
}
bool IMUProcesser::copyData(std::vector<std::vector<std::vector<float>>>& inputData)
{
	return true;
}
void IMUProcesser::run()
{

	QObject::connect(IMUSerialPort, SIGNAL(readyRead()), this, SLOT(onReadyProcessing()));
	stopFlag = false;
	while (!stopFlag)
	{
		;
	}
}
void IMUProcesser::onReadyProcessing()
{
	int imuNum = 4;
	bool ok = 1;
	//qDebug() << "run" << endl;
	//qDebug() << IMUSerialPort->bytesAvailable() << endl;
	//if (IMUSerialPort->bytesAvailable() >= dataLength)
	if (true)
	{
		IMUSerialPort->setReadBufferSize(22);
		receiveBuffer = IMUSerialPort->readAll();
		bool ok = 1;
		QByteArray tempHexBuffer = receiveBuffer.toHex();
		IMUSerialPortBuffer.clear();
		for (int i = 0; i < tempHexBuffer.length(); i += 2)
		{
			IMUSerialPortBuffer += tempHexBuffer.mid(i, 2).toInt(&ok, 16);
		}
		this->dataUnpackage();
	}
}
void IMUProcesser::stopThread()
{
	stopFlag = true;
}