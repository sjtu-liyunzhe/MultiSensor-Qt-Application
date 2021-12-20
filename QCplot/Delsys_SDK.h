#pragma once
#include <qtcpsocket.h>
#include <qabstractsocket.h>
#include <qhostaddress.h>
#include <QThread>
#include <queue>
#include <iostream>
#include <fstream>
#include <time.h>

#include "stdafx.h"

class Delsys_read_Thread : public QThread
{
	Q_OBJECT

public:
	explicit Delsys_read_Thread(QObject* parent = nullptr);

	//~ delsys sdk
	
	int Init_delsys_socket();
	void close_delsys_socket();
	void create_delsys_file();
	void stop_read_delsys();
	void start_save();

private:
	//~ delsys sdk
	SOCKET delsys_commSock;	//command socket
	SOCKET delsys_emgSock;		//EMG socket
	void QueryResponse(char* prompt, char* default, char* response);
	void SendCommandWithResponse(char* command, char* response);

	char delsys_response[32];	//buffer for response
	char delsys_tmp[128];	//string buffer for various uses
	char* delsys_onoff;	//used to hold the trigger state	
	int delsys_collectionTime;	//time to collect data in samples (seocnds * 2000)
	
	WSADATA delsys_wsaData;	//Windows socket data
	int delsys_iResult;	//result of call

	int delsys_k;	//byte count
	char delsys_emgbuf[4 * 16];		//buffer for one sample from each sensor //~ emg��������char�������ֽڣ����ж�ȡ������ʵ��������4���ֽڡ�����4*16��ʾ4�����ֽ�/������*16����ͨ����
	unsigned long delsys_bytesRead;	//bytes available for reading
	float delsys_emgsample;	//EMG sample //~ floatռ4���ֽڣ�32λ

	bool Delsys_Flag;//~ delsys SDK��flag
	bool start_save_delsys;
	char save_delsys[100];
	void gettime();
	FILE* delsys_emgdata;	//the files to be written with EMG and ACC data from standard snesor & EMG, ACC, GYRO and MAG data from IM sensor
	

protected:
	void run();//���߳�ִ�е����ݽ�ͨ�����¸��麯��ʵ��


signals:

public slots:


};
