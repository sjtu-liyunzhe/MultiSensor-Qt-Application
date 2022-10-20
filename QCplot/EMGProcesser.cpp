#include "EMGProcesser.h"
#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <QDebug>

#define CIRCLE_SIZE 50
#define EMG_QUEUE_SIZE 50
#define IMU_QUEUE_SIZE 10
#define EMG_SEND_PACKAGE_SIZE 67
#define BUFFER_SIZE 6700

using namespace std;

extern const int oriSize = 1000;
extern const unsigned short int packageSize = EMG_SEND_PACKAGE_SIZE;

EMGBuffer::EMGBuffer(int channelSize, int bufferSize)
{
	this->channelSize = channelSize;
	this->bufferSize = bufferSize;
	array.resize(this->channelSize);
	vectorArray.resize(this->channelSize);
	deArray.resize(this->channelSize);
	for (auto it = deArray.begin(); it != deArray.end(); ++it)
	{
		it->resize(bufferSize);
	}
	//for (auto it = deArray[0].begin(); it != deArray[0].end(); ++it)
	//{
	//	qDebug() << *it << endl;
	//}
	for (int i = 0; i != bufferSize; ++i)
	{
		Data_x.push_back(i);
	}
}
void EMGBuffer::saveData(const int& channel, double data)
{
	array[channel].push(data);
	vectorArray[channel].push_back(data);
	deArray[channel].push_back(data);
	deArray[channel].pop_front();
	//qDebug() << data << endl;
}
UnionIMUBuffer::UnionIMUBuffer(int channelSize, int dataSize, int bufferSize)
{
	this->channelSize = channelSize;
	this->dataSize = dataSize;
	this->bufferSize = bufferSize;
	array.resize(this->channelSize);
	vectorArray.resize(this->channelSize);
	deArray.resize(this->channelSize);
	for (int i = 0; i < this->channelSize; i++)
	{
		array[i].resize(this->dataSize);
		vectorArray[i].resize(this->dataSize);
		deArray[i].resize(this->dataSize);
		for (auto it = deArray[i].begin();it != deArray[i].end(); ++it)
		{
			it->resize(bufferSize);
		}
	}
	for (int i = 0; i != bufferSize; ++i)
	{
		Data_x.push_back(i);
	}
}
void UnionIMUBuffer::saveData(const int& channel, const int& dataNum, double data)
{
	array[channel][dataNum].push(data);
	vectorArray[channel][dataNum].push_back(data);
	deArray[channel][dataNum].push_back(data);
	deArray[channel][dataNum].pop_front();
}
EMGProcesser::EMGProcesser(QSerialPort* serialPort, QObject* parent) : QThread(parent)
{
	EMGSerialPort = new QSerialPort();
	EMGSerialPort = serialPort;
	tempData.resize(1000);
	channelCount = 1;
	dataLength = 8;
	emgBuffer = new EMGBuffer(4, 10000);
	imuBuffer = new UnionIMUBuffer(4, 3, 1000);
}
EMGProcesser::~EMGProcesser()
{
	delete EMGSerialPort;
	delete emgBuffer;
	delete imuBuffer;
}
void EMGProcesser::dataUnpackage()
{
	vector<int16_t> tempData(4);
	if (EMGSerialPortBuffer[6].operator==(0x0D) && EMGSerialPortBuffer[7].operator==(0x0A))
	{
		unsigned char sum = 0;
		for (int i = 0; i < 4; ++i)
		{
			sum += EMGSerialPortBuffer[i];
		}
		/*c_1_temp_data = int16_t(EMGSerialPortBuffer[0] << 4);
		c_2_temp_data = int16_t(EMGSerialPortBuffer[1] << 4);
		c_3_temp_data = int16_t(EMGSerialPortBuffer[2] << 4);
		c_4_temp_data = int16_t(EMGSerialPortBuffer[3] << 4);*/
		//emgBuffer->array[0].push(double(c_1_temp_data));
		for (int i = 0; i != emgBuffer->channelSize; ++i)
		{
			tempData[i] = int16_t(EMGSerialPortBuffer[i] << 4);
			// 抄自EMG_Acq
			if ((i % 2) != 0)
			{
				tempData[i] += int16_t(EMGSerialPortBuffer[4 + i / 2] & 0x0F);
			}
			else
			{
				tempData[i] += int16_t((EMGSerialPortBuffer[4 + i / 2] & 0xF0) >> 4);
			}// 转换为有符号数，抄自EMG_Acq，感觉没有必要
			if ((tempData[i] & 0x0800) != 0)
				tempData[i] = int16_t(tempData[i] | 0xF000);
			/*if (tempData[i] > 500 || tempData[i] < -500)
				tempData[i] = 0;*/
			emgBuffer->array[i].push(double(tempData[i]));
			//qDebug() << tempData[i] << endl;
		}

	}
}
void EMGProcesser::unionDataUnpackage()
{
	/*int16_t c_1_temp_data = 0, c_2_temp_data = 0,
		c_3_temp_data = 0, c_4_temp_data = 0;*/
	vector<int16_t> tempData(4);
	//tempData.resize(4);
	if (EMGSerialPortBuffer[0].operator==(0x0D) && EMGSerialPortBuffer[1].operator==(0x0A))
	{
		unsigned char sum = 0;
		unsigned char rollHigh = 0, rollLow = 0, pitchHigh = 0, pitchLow = 0, yawHigh = 0, yawLow = 0;
		float tempRoll = 0, tempPitch = 0, tempYaw = 0;
		for (int i = 0; i < 4; ++i) 
		{
			sum += EMGSerialPortBuffer[i];
		}
		/*c_1_temp_data = int16_t(EMGSerialPortBuffer[0] << 4);
		c_2_temp_data = int16_t(EMGSerialPortBuffer[1] << 4);
		c_3_temp_data = int16_t(EMGSerialPortBuffer[2] << 4);
		c_4_temp_data = int16_t(EMGSerialPortBuffer[3] << 4);*/
		//emgBuffer->array[0].push(double(c_1_temp_data));
		for (int i = 0; i != emgBuffer->channelSize; ++i)
		{
			tempData[i] = int16_t(EMGSerialPortBuffer[2 + i] << 4);
			// 抄自EMG_Acq
			if ((i % 2) != 0)
			{
				tempData[i] += int16_t(EMGSerialPortBuffer[6 + i / 2] & 0x0F);
			}
			else
			{
				tempData[i] += int16_t((EMGSerialPortBuffer[6 + i / 2] & 0xF0) >> 4);
			}// 转换为有符号数，抄自EMG_Acq，感觉没有必要
			if ((tempData[i] & 0x0800) != 0)
				tempData[i] = int16_t(tempData[i] | 0xF000);
			/*if (tempData[i] > 500 || tempData[i] < -500)
				tempData[i] = 0;*/
			emgBuffer->array[i].push(double(tempData[i]));
			//qDebug() << tempData[i] << endl;
		}
		for (int i = 1; i != imuBuffer->channelSize; ++i)
		{
			rollHigh =  EMGSerialPortBuffer[6 * i + 2];
			rollLow =   EMGSerialPortBuffer[6 * i + 3];
			pitchHigh = EMGSerialPortBuffer[6 * i + 4];
			pitchLow =  EMGSerialPortBuffer[6 * i + 5];
			yawHigh =   EMGSerialPortBuffer[6 * i + 6];
			yawLow =    EMGSerialPortBuffer[6 * i + 7];
			tempRoll = float(int16_t((rollHigh << 8) | rollLow)) / 100;
			tempPitch = float(int16_t((pitchHigh << 8)) | pitchLow) / 100;
			tempYaw = float(int16_t((yawHigh << 8) | yawLow)) / 100;
			imuBuffer->array[i - 1][0].push(double(tempRoll));
			imuBuffer->array[i - 1][1].push(double(tempPitch));
			imuBuffer->array[i - 1][2].push(double(tempYaw));
		}

	}
}
void EMGProcesser::dataBufferUnpackage()
{
	int8_t emgChannel = 0, imuChannel = 0;
	unsigned char rollHigh = 0, rollLow = 0, pitchHigh = 0, pitchLow = 0, yawHigh = 0, yawLow = 0;
	float tempRoll = 0, tempPitch = 0, tempYaw = 0;
	// EMG data
	if (EMGSerialPortBuffer[0].operator==(0x0D) && EMGSerialPortBuffer[packageSize - 1].operator==(0x0A))
	{
		emgChannel = EMGSerialPortBuffer[1];
		//for(int)
	}
	// IMU Roll data
	else if(EMGSerialPortBuffer[0].operator==(0x0E) && EMGSerialPortBuffer[packageSize - 1].operator==(0x0B))
	{
		imuChannel = EMGSerialPortBuffer[1];
		for (int i = 0; i != IMU_QUEUE_SIZE; ++i)
		{
			rollHigh = EMGSerialPortBuffer[2 * (i + 1)];
			rollLow = EMGSerialPortBuffer[2 * (i + 1) + 1];
			tempRoll = float(int16_t((rollHigh << 8) | rollLow)) / 100;
			imuBuffer->array[imuChannel][0].push(double(tempRoll));
		}
	}
	// IMU Pitch data
	else if (EMGSerialPortBuffer[0].operator==(0x0F) && EMGSerialPortBuffer[packageSize - 1].operator==(0x0C))
	{
		imuChannel = EMGSerialPortBuffer[1];
		for (int i = 0; i != IMU_QUEUE_SIZE; ++i)
		{
			pitchHigh = EMGSerialPortBuffer[2 * (i + 1)];
			pitchLow = EMGSerialPortBuffer[2 * (i + 1) + 1];
			tempPitch = float(int16_t((pitchHigh << 8)) | pitchLow) / 100;
			imuBuffer->array[imuChannel][1].push(double(tempPitch));
		}
	}
	// IMU Yaw data
	else if (EMGSerialPortBuffer[0].operator==(0x10) && EMGSerialPortBuffer[packageSize - 1].operator==(0x0D))
	{
		imuChannel = EMGSerialPortBuffer[1];
		for (int i = 0; i != IMU_QUEUE_SIZE; ++i)
		{
			yawHigh = EMGSerialPortBuffer[2 * (i + 1)];
			yawLow = EMGSerialPortBuffer[2 * (i + 1) + 1];
			tempYaw = float(int16_t((yawHigh << 8) | yawLow)) / 100;
			imuBuffer->array[imuChannel][2].push(double(tempYaw));
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
int onCount = 0;
unsigned char dataBuffer[BUFFER_SIZE];
int receiveCount = 0;
void EMGProcesser::onReadyProcessing()
{
	uint8_t emgChannel = 0, imuChannel = 0;
	int16_t tempEMG = 0;
	uint8_t EMGPackageNum = 0;
	uint8_t IMUPackageNum = 0;
	unsigned char rollHigh = 0, rollLow = 0, pitchHigh = 0, pitchLow = 0, yawHigh = 0, yawLow = 0;
	float tempRoll = 0, tempPitch = 0, tempYaw = 0;
	//disconnect(EMGSerialPort, SIGNAL(readyRead()), this, SLOT(onReadyProcessing()));
	//while (EMGSerialPort->waitForReadyRead(10) == true);
	//connect(EMGSerialPort, SIGNAL(readyRead()), this, SLOT(onReadyProcessing()));
	int emgNum = 4;
	bool ok = 1;
	//qDebug() << "run" << endl;
	//EMGSerialPort->setReadBufferSize(53);

	receiveBuffer = EMGSerialPort->readAll();
	int dataCount = receiveBuffer.size();
	//qDebug() << EMGSerialPortBuffer.size() << endl;
	//receiveBuffer = EMGSerialPort->read(53);
	for (int i = 0; i != dataCount; ++i)
	{
		dataBuffer[receiveCount] = receiveBuffer.data()[i];
		++receiveCount;
		if (receiveCount == BUFFER_SIZE)
			receiveCount = 0;
		if (receiveCount >= EMG_SEND_PACKAGE_SIZE)
		{
			if (dataBuffer[receiveCount - 2] == 0xFF && dataBuffer[receiveCount - 3] == 0x88)
			{
				// EMG data
				if (dataBuffer[receiveCount - 1] == 0x0A &&
					dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE] == 0x0D &&
					dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 2] == 0x32)
				{
					emgChannel = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 1];
					if (emgChannel <= 3)
					{
						EMGPackageNum = uint8_t(dataBuffer[receiveCount - 4]);
						for (int i = 0; i != EMG_QUEUE_SIZE; ++i)
						{
							tempEMG = int16_t(dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 3 + i] << 4);
		
							if ((tempEMG & 0x0800) != 0)
								tempEMG = int16_t(tempEMG | 0xF000);
							//emgBuffer->array[emgChannel].push(double(tempEMG));
							////qDebug() << double(tempEMG) << endl;
							//emgBuffer->vectorArray[emgChannel].push_back(double(tempEMG));
							emgBuffer->saveData(emgChannel, double(tempEMG));
						}
						emgBuffer->packageNumArray.push_back(EMGPackageNum);
					}
				}
				else if (dataBuffer[receiveCount - 1] == 0x0E &&
					dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE] == 0x0B &&
					dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 2] == 0x3C)
				{
					imuChannel = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 1];
					if (imuChannel <= 3)
					{
						IMUPackageNum = uint8_t(dataBuffer[receiveCount - 4]);
						for (int i = 0; i != IMU_QUEUE_SIZE; ++i)
						{
							rollHigh = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * i + 3];
							rollLow = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * i + 4];
							tempRoll = float(int16_t((rollHigh << 8) | rollLow)) / 100;
							//imuBuffer->array[imuChannel][0].push(double(tempRoll));
							imuBuffer->saveData(imuChannel, 0, tempRoll);
							pitchHigh = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * i + 23];
							pitchLow = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * i + 24];
							tempPitch = float(int16_t((pitchHigh << 8)) | pitchLow) / 100;
							//imuBuffer->array[imuChannel][1].push(double(tempPitch));
							imuBuffer->saveData(imuChannel, 1, tempPitch);
							yawHigh = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * i + 43];
							yawLow = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * i + 44];
							tempYaw = float(int16_t((yawHigh << 8)) | yawLow) / 100;
							//imuBuffer->array[imuChannel][2].push(double(tempYaw));
							imuBuffer->saveData(imuChannel, 2, tempYaw);
						}
						imuBuffer->packageNumArray.push_back(IMUPackageNum);
					}
				}
				// IMU Roll data
				//else if (dataBuffer[receiveCount - 1] == 0x0B && dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE] == 0x0E)
				//{
				//	if (dataBuffer[receiveCount - 2] == 0xFF && dataBuffer[receiveCount - 3] == 0x88)
				//	{
				//		//qDebug() << "2" << endl;
				//		imuChannel = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 1];
				//		for (int i = 0; i != IMU_QUEUE_SIZE; ++i)
				//		{
				//			rollHigh = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * (i + 1)];
				//			rollLow = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * (i + 1) + 1];
				//			tempRoll = float(int16_t((rollHigh << 8) | rollLow)) / 100;
				//			imuBuffer->array[imuChannel][0].push(double(tempRoll));
				//		}
				//	}
				//}
				//// IMU Pitch data
				//else if (dataBuffer[receiveCount - 1] == 0x0C && dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE] == 0x0F)
				//{
				//	if (dataBuffer[receiveCount - 2] == 0xFF && dataBuffer[receiveCount - 3] == 0x88)
				//	{
				//		//qDebug() << "3" << endl;
				//		imuChannel = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 1];
				//		for (int i = 0; i != IMU_QUEUE_SIZE; ++i)
				//		{
				//			pitchHigh = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * (i + 1)];
				//			pitchLow = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * (i + 1) + 1];
				//			tempPitch = float(int16_t((pitchHigh << 8)) | pitchLow) / 100;
				//			imuBuffer->array[imuChannel][1].push(double(tempPitch));
				//		}
				//	}
				//}
				//// IMU Yaw data
				//else if (dataBuffer[receiveCount - 1] == 0x0D && dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE] == 0x10)
				//{
				//	if (dataBuffer[receiveCount - 2] == 0xFF && dataBuffer[receiveCount - 3] == 0x88)
				//	{
				//		//qDebug() << "4" << endl;
				//		imuChannel = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 1];
				//		for (int i = 0; i != IMU_QUEUE_SIZE; ++i)
				//		{
				//			yawHigh = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * (i + 1)];
				//			yawLow = dataBuffer[receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * (i + 1) + 1];
				//			tempYaw = float(int16_t((yawHigh << 8)) | yawLow) / 100;
				//			imuBuffer->array[imuChannel][2].push(double(tempYaw));
				//		}
				//	}
				//}
			}
		}
		else
		{
			//qDebug() << "********************" << endl;
			//qDebug() << receiveCount << endl;
			//int coutIndex = ((receiveCount - EMG_SEND_PACKAGE_SIZE) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE;
			//qDebug() << "index: " << coutIndex << endl;
			// EMG data
			if (((receiveCount - 2) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE == 0xFF
				&& ((receiveCount - 3) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE == 0x88)
			{
				if (dataBuffer[((receiveCount - 1) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE] == 0x0A &&
					dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE] == 0x0D &&
					dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 2) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE] == 0x32)
				{
					//qDebug() << "5" << endl;
					emgChannel = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 1) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
					if (emgChannel <= 3)
					{
						EMGPackageNum = dataBuffer[((receiveCount - 4) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
						for (int i = 0; i != EMG_QUEUE_SIZE; ++i)
						{
							tempEMG = int16_t(dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 3 + i) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE] << 4);
							if ((tempEMG & 0x0800) != 0)
								tempEMG = int16_t(tempEMG | 0xF000);
							//emgBuffer->array[emgChannel].push(double(tempEMG));
							//emgBuffer->vectorArray[emgChannel].push_back(double(tempEMG));
							emgBuffer->saveData(emgChannel, double(tempEMG));
						}
						emgBuffer->packageNumArray.push_back(EMGPackageNum);
					}
				}
				else if (dataBuffer[((receiveCount - 1) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE] == 0x0E &&
					dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE] == 0x0B &&
					dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 2) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE] == 0x3C)
				{
					imuChannel = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 1) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
					if (imuChannel <= 3)
					{
						IMUPackageNum = dataBuffer[((receiveCount - 4) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
						for (int i = 0; i != IMU_QUEUE_SIZE; ++i)
						{
							rollHigh = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * i + 3) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
							rollLow = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * i + 4) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
							tempRoll = float(int16_t((rollHigh << 8) | rollLow)) / 100;
							//imuBuffer->array[imuChannel][0].push(double(tempRoll));
							imuBuffer->saveData(imuChannel, 0, tempRoll);
							pitchHigh = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * i + 23) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
							pitchLow = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * i + 24) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
							tempPitch = float(int16_t((pitchHigh << 8) | pitchLow)) / 100;
							//imuBuffer->array[imuChannel][1].push(double(tempPitch));
							imuBuffer->saveData(imuChannel, 1, tempPitch);
							yawHigh = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * i + 43) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
							yawLow = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * i + 44) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
							tempYaw = float(int16_t((yawHigh << 8) | yawLow)) / 100;
							//imuBuffer->array[imuChannel][2].push(double(tempYaw));
							imuBuffer->saveData(imuChannel, 2, tempYaw);
						}
						imuBuffer->packageNumArray.push_back(IMUPackageNum);
					}
					
				}
				// IMU Roll data
				//else if (dataBuffer[((receiveCount - 1) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE] == 0x0B &&
				//	dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE] == 0x0E)
				//{
				//	if (((receiveCount - 2) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE == 0xFF &&
				//		((receiveCount - 3) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE == 0x88)
				//	{
				//		//qDebug() << "6" << endl;
				//		//uint8_t zeroIndex = ((receiveCount - EMG_SEND_PACKAGE_SIZE) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE;
				//		imuChannel = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 1) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
				//		for (int i = 0; i != IMU_QUEUE_SIZE; ++i)
				//		{
				//			rollHigh = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * (i + 1)) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
				//			rollLow = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * (i + 1) + 1) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
				//			tempRoll = float(int16_t((rollHigh << 8) | rollLow)) / 100;
				//			imuBuffer->array[imuChannel][0].push(double(tempRoll));
				//		}
				//	}
				//}
				//// IMU Pitch data
				//else if (dataBuffer[((receiveCount - 1) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE] == 0x0C &&
				//	dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE] == 0x0F)
				//{
				//	if (((receiveCount - 2) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE == 0xFF &&
				//		((receiveCount - 3) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE == 0x88)
				//	{
				//		//qDebug() << "7" << endl;
				//		imuChannel = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 1) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
				//		for (int i = 0; i != IMU_QUEUE_SIZE; ++i)
				//		{
				//			pitchHigh = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * (i + 1)) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
				//			pitchLow = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * (i + 1) + 1) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
				//			tempPitch = float(int16_t((pitchHigh << 8) | pitchLow)) / 100;
				//			imuBuffer->array[imuChannel][1].push(double(tempPitch));
				//		}
				//	}
				//}
				//// IMU Yaw data
				//else if (dataBuffer[((receiveCount - 1) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE] == 0x0D &&
				//	dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE] == 0x10)
				//{
				//	if (((receiveCount - 2) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE == 0xFF &&
				//		((receiveCount - 3) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE == 0x88)
				//	{
				//		//qDebug() << "8" << endl;
				//		imuChannel = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 1) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
				//		for (int i = 0; i != IMU_QUEUE_SIZE; ++i)
				//		{
				//			yawHigh = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * (i + 1)) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
				//			yawLow = dataBuffer[((receiveCount - EMG_SEND_PACKAGE_SIZE + 2 * (i + 1) + 1) % BUFFER_SIZE + BUFFER_SIZE) % BUFFER_SIZE];
				//			tempYaw = float(int16_t((yawHigh << 8) | yawLow)) / 100;
				//			imuBuffer->array[imuChannel][2].push(double(tempYaw));
				//		}
				//	}
				//}
			}
		}
	}
	QByteArray tempHexBuffer = receiveBuffer.toHex();
	EMGSerialPortBuffer.clear();
	for (int i = 0; i < tempHexBuffer.length(); i += 2)
	{
		EMGSerialPortBuffer += tempHexBuffer.mid(i, 2).toInt(&ok, 16);
	}
	//this->unionDataUnpackage();
	//dataUnpackage();
	//dataBufferUnpackage();
	if (onCount > 10)
	{
		updateEMGSignal();
		onCount = 0;
	}
	++onCount;

}
void EMGProcesser::stopThread()
{
	stopFlag = true;
}