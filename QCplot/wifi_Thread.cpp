#include "afxwin.h" //~ 需要放在最开头的位置！！
//~ 添加全局变量
#include "GlobalData.h"
#include "wifi_Thread.h"

//创建临界保护区对象
CRITICAL_SECTION g_cs2;

thread_two::thread_two(QObject* parent) : QThread(parent)
{
	wifi_tcpClient = new QTcpSocket();
	Data_tmp.resize(1000);//~ 这一句必须加，否则无法直接对数组的某个值进行赋值
	wifi_isopen = false;	
	channelOneFlag = false;
	channelCount = 1;
}

void thread_two::run()//~ 开启多线程（wifi_Tread->start()）后执行该函数
{
	//~ wifi模块部分
	QObject::connect(wifi_tcpClient, SIGNAL(readyRead()), this, SLOT(onReadyRead()));//~ 当WiFi缓冲区产生新的数据时会触发槽函数
	//初始化临界保护区对象
	InitializeCriticalSection(&g_cs2);
	wifi_stop_flag = false;
	while (!wifi_stop_flag)//~ 开启线程后，在这个函数内进行死循环不断接收wifi数据；若不用死循环的话，run函数执行完会自动关闭线程，就需要手动再次开启或者使用定时器开启
	{
		
	}
	
}

void thread_two::onReadyRead()
{		
	int chnum = 8;
	int sample_length = 1000;
	char par[1000];//~ 临时存储1*1000的WiFi数据
	int _gain = 1; //~显示增益
	wifi_tcpClient->setReadBufferSize(1000);//~ 设置wifi传输缓冲区的大小
	int receiveresult = wifi_tcpClient->read(par, 1000);//~ 读取wifi数据（法1）
	
	//QByteArray wifi_buffer= wifi_tcpClient->readAll();//~ 读取wifi数据（法2）


	//~ 整合WiFi数据包成4*1000
	std::vector<double> temp(1000, 0);//~ 初始化temp向量为1*1000的全0向量
	for (int i = 0; i < 1000; i++)
	{
		temp[i] = unsigned char(par[i]);//~ 读取wifi数据（法1）
		//temp[i] = wifi_buffer.data()[i];//~ 读取wifi数据（法2）
	}
	double ChannelNumber = temp[1];//~ 根据通讯协议，获取到的数据包1*1000中，第二个数据是通道号（temp[0]=0x00）
	
	if (ChannelNumber == 1)
	{
		wifi_Package.clear();//~ 用二维数组wifi_Package存储4*1000的数据；因此，检测到通道号为1时，需要清空wifi_Package的数据；
		channelCount = 1;
		channelOneFlag = true;//~ 只有捕捉到通道号是1的数据包，才会继续接受后面的数据包
		wifi_Package.push_back(temp);//~ 使用.push_back可以直接将temp的数据加到wifi_Package中，.push_back方法是换行添加
		channelCount++;
	}
	if (channelOneFlag)
	{
		if (ChannelNumber == 2)
		{
			if (ChannelNumber == channelCount)//~ 双重判断，保证当前接收到的数据包与前一个数据包的通道号是连着的；
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
				//channelOneFlag = false;//~ 当接收到第4个通道后，回到开头
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
				channelOneFlag = false;//~ 当接收到第8个通道后，回到开头
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
	if (wifi_Package.size() == 8)//~ 当wifi_Package存储的以太网数据的大小为4*1000时，将该数据包传到WIFI_DATA变量中，供主线程进行读取并计算显示等
	{
		//创建临界保护区
		EnterCriticalSection(&g_cs2);//~ 进入临界区
		//无缓存
		WIFI_DATA = wifi_Package;
		
		LeaveCriticalSection(&g_cs2);//~ 离开临界区
	}
}

bool thread_two::wifi_connect()
{
	if (wifi_isopen)
	{
		wifi_tcpClient->disconnectFromHost();//~ 断开WiFi连接
		if (wifi_tcpClient->state() == QAbstractSocket::UnconnectedState \
			|| wifi_tcpClient->waitForDisconnected(1000))  //已断开连接则进入if{}
		{
			wifi_isopen = false;
			return false;
		}
		return true;
	}
	else
	{
		Ipaddress = "192.168.4.1";//~ WiFi连接地址（在下位机代码中有定义）
		portvalue = 4321;//~ WiFi连接端口号
		wifi_tcpClient->connectToHost(Ipaddress, portvalue);//~ 连接WiFi
		if (wifi_tcpClient->waitForConnected(1000))  // 连接成功则进入if{}
		{	
			wifi_isopen = true;
			return true;
		}
		return false;
	}

}

bool thread_two::copydata_wifi(std::vector<std::vector<double>>& x)//~ 读取存储在US_DATA1变量中的数据包8*1000
{
	//创建临界保护区
	//~ 互斥读取变量，防止各个线程之间同时访问该变量造成数据错乱；
	//EnterCriticalSection(&g_cs);
	if (WIFI_DATA.empty())
	{
		return false;
	}

	if (WIFI_DATA.size() == 8)
	{
		if (WIFI_DATA[7][1] = 8)//~ 检查二维数组US_DATA1的最后一个数据包是否为通道8
		{
			EnterCriticalSection(&g_cs2);
			x = WIFI_DATA;//~ 把以太网采集到的4*1000的数组赋值给copydata1函数的参数，同时该函数返回bool值
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
