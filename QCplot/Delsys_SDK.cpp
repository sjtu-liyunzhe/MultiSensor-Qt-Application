#include "Delsys_SDK.h"
#include <stdio.h>

Delsys_read_Thread::Delsys_read_Thread(QObject* parent) : QThread(parent)
{
	Delsys_Flag = false;
}


void Delsys_read_Thread::run()//~ �������̣߳�wifi_Tread->start()����ִ�иú���
{
	delsys_collectionTime = 1;
	start_save_delsys = false;
	while (Delsys_Flag)	//loop until all samples acquired
	{
		while (!start_save_delsys)
		{
			delsys_k = recv(delsys_emgSock, delsys_emgbuf, sizeof(delsys_emgbuf), 0);
		}

		//See if we have enough data
		ioctlsocket(delsys_emgSock, FIONREAD, &delsys_bytesRead);	//peek at data available
		while (delsys_bytesRead >= sizeof(delsys_emgbuf))
		{
			delsys_k = recv(delsys_emgSock, delsys_emgbuf, sizeof(delsys_emgbuf), 0);//~ ����Ӧ���ǽ�16��ͨ�������ݶ���ȡ�����ˣ������һ��ѭ����Ӧ�þͿ��Դ洢��������

			for (int i = 0; i < 15; i++)
			{
				//process data //~ ���ƻ��������ݣ�
				memcpy(&delsys_emgsample, &delsys_emgbuf[4 * (i)], sizeof(delsys_emgsample));//~ memcpy�ǽ�emgbuf���Ƹ�emgsample������һ�仰ֻ��ȡ��һ��float��4���ֽڣ������ݣ�Ҳ��һ��ͨ��
				//Write to file
				fprintf(delsys_emgdata, "%e,", delsys_emgsample);
			}
			//process data //~ ���ƻ��������ݣ�
			memcpy(&delsys_emgsample, &delsys_emgbuf[4 * (16 - 1)], sizeof(delsys_emgsample));//~ memcpy�ǽ�emgbuf���Ƹ�emgsample������һ�仰ֻ��ȡ��һ��float��4���ֽڣ������ݣ�Ҳ��һ��ͨ��
			//Write to file
			fprintf(delsys_emgdata, "%e\n", delsys_emgsample);


			delsys_collectionTime++;
			if (delsys_collectionTime % 1926 == 0)
				printf(".");	//indicate progress to user
			ioctlsocket(delsys_emgSock, FIONREAD, &delsys_bytesRead);	//peek at data available
		}
		Sleep(10);	//let other threads run //~ delsys�л�����������Ҳ����Ҫһֱ��ȡ����
	}
}


int Delsys_read_Thread::Init_delsys_socket()
{
	// Initialize Winsock
	delsys_iResult = WSAStartup(MAKEWORD(2, 2), &delsys_wsaData);
	if (delsys_iResult != 0) {
		printf("WSAStartup failed: %d\n", delsys_iResult);
		return 0;	//could not open Windows sockets library
	}
	// Declare and initialize variables used for DNS resolution.
	char* ip = "127.0.0.1";
	char* port = NULL;
	struct addrinfo aiHints;
	struct addrinfo* aiList = NULL;
	int retVal;

	// Setup the hints address info structure
	// which is passed to the getaddrinfo() function
	memset(&aiHints, 0, sizeof(aiHints));
	aiHints.ai_family = AF_INET;
	aiHints.ai_socktype = SOCK_STREAM;
	aiHints.ai_protocol = IPPROTO_TCP;
	// Call getaddrinfo(). If the call succeeds,
	// the aiList variable will hold a linked list
	// of addrinfo structures containing response
	// information about the host
	if ((retVal = getaddrinfo(ip, port, &aiHints, &aiList)) != 0)
	{//~ ��������д��IP��ַ�Ƿ�Ϊdelsys�豸��server��ַ
		printf("Invalid URL.\n");
		return 0;	//could not resolve URL
	}
	// Set up the connection to server for communication
	delsys_commSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in sinRemote;
	sinRemote.sin_family = AF_INET;
	sinRemote.sin_addr.s_addr = ((sockaddr_in*)(aiList->ai_addr))->sin_addr.s_addr;
	sinRemote.sin_port = htons(50040);

	//Try to connect
	if (::connect(delsys_commSock, (sockaddr*)&sinRemote, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		printf("Can't connect to Trigno Server!\n");
		return 0;	//server is not responding
	}
	else
	{
		printf("Connected to Trigno Server.\n");
	}
	//~ ��ʾ������Ϣ�����統ǰ���ӵ�delsys server����İ汾�ŵ�
	int n;	//byte count
	//Get response from the server after connecting
	n = recv(delsys_commSock, delsys_tmp, sizeof(delsys_tmp), 0);
	delsys_tmp[n - 2] = 0;	//back up over second CR/LF
	printf(delsys_tmp);	//display to user.

	// Set up the connection to server for EMG data
	delsys_emgSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sinRemote.sin_port = htons(50041);
	if (::connect(delsys_emgSock, (sockaddr*)&sinRemote, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		printf("Error initializing Trigno EMG data connection (can't connect).\n");
		return 0;
	}

	//Send command to start data acquisition  //~ ��delsys��server����ָ��׼���ɼ��������͸�ָ�������û�취��ȡ����
	SendCommandWithResponse("UPSAMPLE OFF\r\n\r\n", delsys_tmp);
	SendCommandWithResponse("START\r\n\r\n", delsys_tmp);


	for (int i = 0; i < 105; i++)
	{//~ �������ڵ�ǰ105��������0����ȡ�����󣬲��洢������Ϊ����
		delsys_k = recv(delsys_emgSock, delsys_emgbuf, sizeof(delsys_emgbuf), 0);
	}

	Delsys_Flag = true;
	//~ �����ͣ��һ��ʱ�䣬���ǵ����ʼ֮��������ܿ�ʼ������ʱ��Ҫע�⣬��������Ҫ���߳̿���ǰ����һ������и�������־������matlabһ�£�����ǰ100�����ݣ��Ƿ���Ҫȥ��������
	printf("delsys is ready! ");

	return 1;

}

//Prompt for an input line using a default value
void Delsys_read_Thread::QueryResponse(char* prompt, char* default, char* response)
{
	printf("%s [%s]: ", prompt, default);
	gets(response);
	if (strlen(response) == 0)	//use default value if empty line
		strcpy(response, default);
}

//Send a command to the server and wait for the response
void Delsys_read_Thread::SendCommandWithResponse(char* command, char* response)
{
	int n;
	char tmp[128];

	strcpy(tmp, command);
	tmp[strlen(command) - 2] = 0;	//trip last CR/LF
	printf("Command: %s", tmp);	//display to user

	if (send(delsys_commSock, command, strlen(command), 0) == SOCKET_ERROR)
	{
		printf("Can't send command.\n");
		_exit(1);	//bail out on error
	}

	//wait for response
	//note - this implementation assumes that the entire response is in one TCP packet
	n = recv(delsys_commSock, tmp, sizeof(tmp), 0);
	tmp[n - 2] = 0;	//strip last CR/LF
	printf("Response: %s", tmp);	//display to user
	strcpy(response, tmp);	//return the response line

	return;
}

void Delsys_read_Thread::close_delsys_socket()
{	
	//Close sockets and files
	Delsys_Flag = false;
	closesocket(delsys_commSock);
	closesocket(delsys_emgSock);
}

void Delsys_read_Thread::create_delsys_file()
{
	gettime();
	delsys_emgdata = fopen(save_delsys, "w");
	if (delsys_emgdata == NULL)
	{
		printf("Can't open data files.\n");
	}
	else
	{
		//Create headers for CSV files
		fprintf(delsys_emgdata, "delsys1,delsys2,delsys3,delsys4,delsys5,delsys6,delsys7,delsys8,delsys9,delsys10,delsys11,delsys12,delsys13,delsys14,delsys15,delsys16\n");
	}
}

void Delsys_read_Thread::start_save()
{
	start_save_delsys = true;
	Delsys_Flag = true;
}


void Delsys_read_Thread::stop_read_delsys()
{	
	start_save_delsys = false;
	
	fclose(delsys_emgdata);
}

void Delsys_read_Thread::gettime()
{
	char file_date[20];
	char buff[9];
	sprintf(file_date, "2008.12.26_09.32.27");					//LB ��file_data�ĳ�ϵͳʱ��
	_strdate_s(buff, 9);									//LB���Ƶ�ǰϵͳ���ڵ������� _strdate_s( tmpbuf, 128 ); printf( "OS date:\t\t\t\t%s\n", tmpbuf ); OS date: 04/25/03  2003��4��25��
	file_date[2] = buff[6];
	file_date[3] = buff[7];
	file_date[5] = buff[0];
	file_date[6] = buff[1];
	file_date[8] = buff[3];
	file_date[9] = buff[4];
	_strtime_s(buff, 9);									//LBд��ʱ�䵽������      _strtime_s( tmpbuf, 128 );   printf( "OS time:\t\t\t\t%s\n", tmpbuf );    OS time: 13:51:23
	file_date[11] = buff[0];
	file_date[12] = buff[1];
	file_date[14] = buff[3];
	file_date[15] = buff[4];								//LB��file_date��Ϊϵͳ��ǰʱ��
	//ʱ�侫ȷ����s
	file_date[17] = buff[6];
	file_date[18] = buff[7];

	sprintf_s(save_delsys, "Mat_file//%s_delsys.csv", file_date);
}