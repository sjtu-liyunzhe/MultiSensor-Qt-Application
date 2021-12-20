#include "afxwin.h" //~ ��Ҫ�����ͷ��λ�ã���
//~ ���ȫ�ֱ���
#include "GlobalData.h"
#include "wifi_Thread.h"

//�����ٽ籣��������
CRITICAL_SECTION g_cs2;

thread_two::thread_two(QObject* parent) : QThread(parent)
{
	wifi_tcpClient = new QTcpSocket();
	Data_tmp.resize(1000);//~ ��һ�����ӣ������޷�ֱ�Ӷ������ĳ��ֵ���и�ֵ
	wifi_isopen = false;	
	channelOneFlag = false;
	channelCount = 1;
}

void thread_two::run()//~ �������̣߳�wifi_Tread->start()����ִ�иú���
{
	//~ wifiģ�鲿��
	QObject::connect(wifi_tcpClient, SIGNAL(readyRead()), this, SLOT(onReadyRead()));//~ ��WiFi�����������µ�����ʱ�ᴥ���ۺ���
	//��ʼ���ٽ籣��������
	InitializeCriticalSection(&g_cs2);
	wifi_stop_flag = false;
	while (!wifi_stop_flag)//~ �����̺߳�����������ڽ�����ѭ�����Ͻ���wifi���ݣ���������ѭ���Ļ���run����ִ������Զ��ر��̣߳�����Ҫ�ֶ��ٴο�������ʹ�ö�ʱ������
	{
		
	}
	
}

void thread_two::onReadyRead()
{		
	int chnum = 8;
	int sample_length = 1000;
	char par[1000];//~ ��ʱ�洢1*1000��WiFi����
	int _gain = 1; //~��ʾ����
	wifi_tcpClient->setReadBufferSize(1000);//~ ����wifi���仺�����Ĵ�С
	int receiveresult = wifi_tcpClient->read(par, 1000);//~ ��ȡwifi���ݣ���1��
	
	//QByteArray wifi_buffer= wifi_tcpClient->readAll();//~ ��ȡwifi���ݣ���2��


	//~ ����WiFi���ݰ���4*1000
	std::vector<double> temp(1000, 0);//~ ��ʼ��temp����Ϊ1*1000��ȫ0����
	for (int i = 0; i < 1000; i++)
	{
		temp[i] = unsigned char(par[i]);//~ ��ȡwifi���ݣ���1��
		//temp[i] = wifi_buffer.data()[i];//~ ��ȡwifi���ݣ���2��
	}
	double ChannelNumber = temp[1];//~ ����ͨѶЭ�飬��ȡ�������ݰ�1*1000�У��ڶ���������ͨ���ţ�temp[0]=0x00��
	
	if (ChannelNumber == 1)
	{
		wifi_Package.clear();//~ �ö�ά����wifi_Package�洢4*1000�����ݣ���ˣ���⵽ͨ����Ϊ1ʱ����Ҫ���wifi_Package�����ݣ�
		channelCount = 1;
		channelOneFlag = true;//~ ֻ�в�׽��ͨ������1�����ݰ����Ż�������ܺ�������ݰ�
		wifi_Package.push_back(temp);//~ ʹ��.push_back����ֱ�ӽ�temp�����ݼӵ�wifi_Package�У�.push_back�����ǻ������
		channelCount++;
	}
	if (channelOneFlag)
	{
		if (ChannelNumber == 2)
		{
			if (ChannelNumber == channelCount)//~ ˫���жϣ���֤��ǰ���յ������ݰ���ǰһ�����ݰ���ͨ���������ŵģ�
			{
				wifi_Package.push_back(temp);
				channelCount++;
			}
			else
			{
				channelOneFlag = false;
				channelCount = 1;
			}
		}

		else if (ChannelNumber == 3)
		{
			if (ChannelNumber == channelCount)
			{
				wifi_Package.push_back(temp);
				channelCount++;
			}
			else
			{
				channelOneFlag = false;
				channelCount = 1;
			}
		}

		else if (ChannelNumber == 4)
		{
			if (ChannelNumber == channelCount)
			{
				//channelOneFlag = false;//~ �����յ���4��ͨ���󣬻ص���ͷ
				//channelCount = 1;
				wifi_Package.push_back(temp);
				channelCount++;
			}
			else
			{
				channelOneFlag = false;
				channelCount = 1;
			}
		}

		else if (ChannelNumber == 5)
		{
			if (ChannelNumber == channelCount)
			{
				wifi_Package.push_back(temp);
				channelCount++;
			}
			else
			{
				channelOneFlag = false;
				channelCount = 1;
			}
		}

		else if (ChannelNumber == 6)
		{
			if (ChannelNumber == channelCount)
			{
				wifi_Package.push_back(temp);
				channelCount++;
			}
			else
			{
				channelOneFlag = false;
				channelCount = 1;
			}
		}

		else if (ChannelNumber == 7)
		{
			if (ChannelNumber == channelCount)
			{
				wifi_Package.push_back(temp);
				channelCount++;
			}
			else
			{
				channelOneFlag = false;
				channelCount = 1;
			}
		}

		else if (ChannelNumber == 8)
		{
			if (ChannelNumber == channelCount)
			{
				channelOneFlag = false;//~ �����յ���8��ͨ���󣬻ص���ͷ
				channelCount = 1;
				wifi_Package.push_back(temp);
			}
			else
			{
				channelOneFlag = false;
				channelCount = 1;
			}
		}
	}
	//Check package size
	if (wifi_Package.size() == 8)//~ ��wifi_Package�洢����̫�����ݵĴ�СΪ4*1000ʱ���������ݰ�����WIFI_DATA�����У������߳̽��ж�ȡ��������ʾ��
	{
		//�����ٽ籣����
		EnterCriticalSection(&g_cs2);//~ �����ٽ���
		//�޻���
		WIFI_DATA = wifi_Package;
		
		LeaveCriticalSection(&g_cs2);//~ �뿪�ٽ���
	}
}

bool thread_two::wifi_connect()
{
	if (wifi_isopen)
	{
		wifi_tcpClient->disconnectFromHost();//~ �Ͽ�WiFi����
		if (wifi_tcpClient->state() == QAbstractSocket::UnconnectedState \
			|| wifi_tcpClient->waitForDisconnected(1000))  //�ѶϿ����������if{}
		{
			wifi_isopen = false;
			return false;
		}
		return true;
	}
	else
	{
		Ipaddress = "192.168.4.1";//~ WiFi���ӵ�ַ������λ���������ж��壩
		portvalue = 4321;//~ WiFi���Ӷ˿ں�
		wifi_tcpClient->connectToHost(Ipaddress, portvalue);//~ ����WiFi
		if (wifi_tcpClient->waitForConnected(1000))  // ���ӳɹ������if{}
		{	
			wifi_isopen = true;
			return true;
		}
		return false;
	}

}

bool thread_two::copydata_wifi(std::vector<std::vector<double>>& x)//~ ��ȡ�洢��US_DATA1�����е����ݰ�8*1000
{
	//�����ٽ籣����
	//~ �����ȡ��������ֹ�����߳�֮��ͬʱ���ʸñ���������ݴ��ң�
	//EnterCriticalSection(&g_cs);
	if (WIFI_DATA.empty())
	{
		return false;
	}

	if (WIFI_DATA.size() == 8)
	{
		if (WIFI_DATA[7][1] = 8)//~ ����ά����US_DATA1�����һ�����ݰ��Ƿ�Ϊͨ��8
		{
			EnterCriticalSection(&g_cs2);
			x = WIFI_DATA;//~ ����̫���ɼ�����4*1000�����鸳ֵ��copydata1�����Ĳ�����ͬʱ�ú�������boolֵ
			WIFI_DATA.clear();
			LeaveCriticalSection(&g_cs2);
			return true;
		}
	}
	else
	{
		return false;
	}
}

void thread_two::wifi_stopThread()
{
	wifi_stop_flag = true;
}
