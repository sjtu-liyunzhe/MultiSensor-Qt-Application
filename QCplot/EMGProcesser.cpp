#include "EMGProcesser.h"
#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <QDebug>
using namespace std;

EMGBuffer::EMGBuffer(int channelSize, int bufferSize)
{
	this->channelSize = channelSize;
	this->bufferSize = bufferSize;
	array.resize(this->channelSize);
	vectorArray.resize(this->channelSize);
}
EMGProcesser::EMGProcesser(QSerialPort* serialPort, QObject* parent) : QThread(parent)
{
	EMGSerialPort = new QSerialPort();
	EMGSerialPort = serialPort;
	tempData.resize(1000);
	channelCount = 1;
	dataLength = 8;
	emgBuffer = new EMGBuffer(4, 1000);
}
void EMGProcesser::dataUnpackage()
{
	/*int16_t c_1_temp_data = 0, c_2_temp_data = 0,
		c_3_temp_data = 0, c_4_temp_data = 0;*/
	vector<int16_t> tempData;
	tempData.resize(4);
	if (EMGSerialPortBuffer[6].operator==(0x0D) && EMGSerialPortBuffer[7].operator==(0x0A))
	{
		unsigned char sum = 0;
		for (int i = 0; i < 4; i++)
		{
			sum += EMGSerialPortBuffer[i];
		}
		/*c_1_temp_data = int16_t(EMGSerialPortBuffer[0] << 4);
		c_2_temp_data = int16_t(EMGSerialPortBuffer[1] << 4);
		c_3_temp_data = int16_t(EMGSerialPortBuffer[2] << 4);
		c_4_temp_data = int16_t(EMGSerialPortBuffer[3] << 4);*/
		//emgBuffer->array[0].push(double(c_1_temp_data));
		for (int i = 0; i < emgBuffer->channelSize; i++)
		{
			tempData[i] = int16_t(EMGSerialPortBuffer[i] << 4);
			// 转换为有符号数，抄自EMG_Acq，感觉没有必要
			if ((tempData[i] & 0x0800) != 0)
				tempData[i] = int16_t(tempData[i] | 0xF000);
			emgBuffer->array[i].push(double(tempData[i]));
			//qDebug() << tempData[i] << endl;
		}
	}
}
void EMGProcesser::run()
{
	//qDebug() << "run" << endl;
	QObject::connect(EMGSerialPort, SIGNAL(readyRead()), this, SLOT(onReadyProcessing()));
	stopFlag = false;
	while (!stopFlag)
	{
		;
	}
}
void EMGProcesser::onReadyProcessing()
{
	int emgNum = 4;
	bool ok = 1;
	//qDebug() << "run" << endl;
	EMGSerialPort->setReadBufferSize(8);
	receiveBuffer = EMGSerialPort->readAll();
	QByteArray tempHexBuffer = receiveBuffer.toHex();
	EMGSerialPortBuffer.clear();
	for (int i = 0; i < tempHexBuffer.length(); i += 2)
	{
		EMGSerialPortBuffer += tempHexBuffer.mid(i, 2).toInt(&ok, 16);
	}
	this->dataUnpackage();

}
void EMGProcesser::stopThread()
{
	stopFlag = true;
}