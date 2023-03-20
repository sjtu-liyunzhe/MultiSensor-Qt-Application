#include "qcplot.h"
#include "GlobalData.h"
#include "Feature.h"
//#include <QFileDialog>
#include <QMessageBox>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <numeric>
#include "engine.h" //~ 该文件包含了matlab计算引擎API函数的说明和所需数据结构的定义
#include "matrix.h"
#include <QMovie>
#include <QLabel>

#pragma comment(lib,"libmx.lib")
#pragma comment(lib,"libmat.lib")
#pragma comment(lib,"libeng.lib")

using namespace std;


//#include <windows.h>

/*~ 

这个版本的功能是实现“连续的腕/手同步控制”，腕部内外旋1个自由度，手部张合1个自由度，共2个自由度；

相比较于离散手势识别软件，这个软件（改自离散手势识别软件）的很多冗余函数还没有删掉，比如getTrainFeature和getTrainFeature1，而是单独写了一个getTrainFeatureC函数；
很多原先控件对应的函数也没有删除，而是单独命名一个控件，如训练的按钮，变为train_ectype，对应的函数为on_train_ectype_clicked()；关于信号和槽，qt中以"on_控件名__clicked()"可以直接实现动作，不需要connect来关联信号和槽
注：QT槽函数多次响应问题——如果使用官方的写法（如：on_btn_clicked()），可以不用写connect函数,会默认触发槽函数；

适用设备：8通道A超设备，以太网连接；
修改目的：适配4通道小型化A超设备（下位机代码是8通道），WiFi连接；

数据存储：晨博只存储了predict过程中，当前腕手关节角度和TAC目标手势关节角度，每一行（每一帧数据100ms）是四个角度数值，默认存储在Mat_file文件夹下

*/



const int feature_col_num = 384;//~ 提取的特征矩阵的列的数目，根据所提特征参数的数目决定
const int feature_row_num = 480;//~ 去掉手势切换过程中间2s后的特征矩阵列数，每一阶段去掉40帧，共去掉120帧
const int beta_row_num = 251;//~ 线性回归的参数矩阵的行数，比特征矩阵经过pca降维后的特征维度多1；
const int train_feature_row_num = 600;//~ 提取的特征矩阵的列的数目，根据训练时间决定，100ms一帧，三个阶段，600对应着每个阶段训练20s
const int switch_len = 20;//~ 剔除每一段训练数据的首尾的数据，消除手势切换过程的影响。例如设置为20，即剔除每一段训练数据的前后各2s
const int part_len = train_feature_row_num / 3;
vector<float> Handangle_last(3, 0);
vector<float> wristangle_last(3, 0);

//~ xsens变量声明
bool xsense_new_packet = false;

/******************************************************* xsens代码中需要提前声明的部分（开始）**********************************/

/*! \brief Stream insertion operator overload for XsPortInfo */
std::ostream& operator << (std::ostream& out, XsPortInfo const& p)
{
	out << "Port: " << std::setw(2) << std::right << p.portNumber() << " (" << p.portName().toStdString() << ") @ "
		<< std::setw(7) << p.baudrate() << " Bd"
		<< ", " << "ID: " << p.deviceId().toString().toStdString()
		;
	return out;
}

/*! \brief Stream insertion operator overload for XsDevice */
std::ostream& operator << (std::ostream& out, XsDevice const& d)
{
	out << "ID: " << d.deviceId().toString().toStdString() << " (" << d.productCode().toStdString() << ")";
	return out;
}

/*! \brief Given a list of update rates and a desired update rate, returns the closest update rate to the desired one */
int findClosestUpdateRate(const XsIntArray& supportedUpdateRates, const int desiredUpdateRate)
{
	if (supportedUpdateRates.empty())
	{
		return 0;
	}

	if (supportedUpdateRates.size() == 1)
	{
		return supportedUpdateRates[0];
	}

	int uRateDist = -1;
	int closestUpdateRate = -1;
	for (XsIntArray::const_iterator itUpRate = supportedUpdateRates.begin(); itUpRate != supportedUpdateRates.end(); ++itUpRate)
	{
		const int currDist = std::abs(*itUpRate - desiredUpdateRate);

		if ((uRateDist == -1) || (currDist < uRateDist))
		{
			uRateDist = currDist;
			closestUpdateRate = *itUpRate;
		}
	}
	return closestUpdateRate;
}


//----------------------------------------------------------------------
// Callback handler for wireless master
//----------------------------------------------------------------------
class WirelessMasterCallback : public XsCallback
{
public:
	typedef std::set<XsDevice*> XsDeviceSet;

	XsDeviceSet getWirelessMTWs() const
	{
		XsMutexLocker lock(m_mutex);
		return m_connectedMTWs;
	}

protected:
	virtual void onConnectivityChanged(XsDevice* dev, XsConnectivityState newState)
	{
		XsMutexLocker lock(m_mutex);
		switch (newState)
		{
		case XCS_Disconnected:		/*!< Device has disconnected, only limited informational functionality is available. */
			std::cout << "\nEVENT: MTW Disconnected -> " << *dev << std::endl;
			m_connectedMTWs.erase(dev);
			break;
		case XCS_Rejected:			/*!< Device has been rejected and is disconnected, only limited informational functionality is available. */
			std::cout << "\nEVENT: MTW Rejected -> " << *dev << std::endl;
			m_connectedMTWs.erase(dev);
			break;
		case XCS_PluggedIn:			/*!< Device is connected through a cable. */
			std::cout << "\nEVENT: MTW PluggedIn -> " << *dev << std::endl;
			m_connectedMTWs.erase(dev);
			break;
		case XCS_Wireless:			/*!< Device is connected wirelessly. */
			std::cout << "\nEVENT: MTW Connected -> " << *dev << std::endl;
			m_connectedMTWs.insert(dev);
			break;
		case XCS_File:				/*!< Device is reading from a file. */
			std::cout << "\nEVENT: MTW File -> " << *dev << std::endl;
			m_connectedMTWs.erase(dev);
			break;
		case XCS_Unknown:			/*!< Device is in an unknown state. */
			std::cout << "\nEVENT: MTW Unknown -> " << *dev << std::endl;
			m_connectedMTWs.erase(dev);
			break;
		default:
			std::cout << "\nEVENT: MTW Error -> " << *dev << std::endl;
			m_connectedMTWs.erase(dev);
			break;
		}
	}
private:
	mutable XsMutex m_mutex;
	XsDeviceSet m_connectedMTWs;
};

//----------------------------------------------------------------------
// Callback handler for MTw
// Handles onDataAvailable callbacks for MTW devices
//----------------------------------------------------------------------
class MtwCallback : public XsCallback
{
public:
	MtwCallback(int mtwIndex, XsDevice* device)
		:m_mtwIndex(mtwIndex)
		, m_device(device)
	{}

	bool dataAvailable() const
	{
		XsMutexLocker lock(m_mutex);
		return !m_packetBuffer.empty();
	}

	XsDataPacket const* getOldestPacket() const
	{
		XsMutexLocker lock(m_mutex);
		XsDataPacket const* packet = &m_packetBuffer.front();
		return packet;
	}

	XsDataPacket const* getNewestPacket() const
	{
		XsMutexLocker lock(m_mutex);
		XsDataPacket const* packet = &m_packetBuffer.back();
		return packet;
	}


	void deleteOldestPacket()
	{
		XsMutexLocker lock(m_mutex);
		m_packetBuffer.pop_front();
	}

	int getMtwIndex() const
	{
		return m_mtwIndex;
	}

	XsDevice const& device() const
	{
		assert(m_device != 0);
		return *m_device;
	}

protected:
	virtual void onLiveDataAvailable(XsDevice*, const XsDataPacket* packet)
	{//~ 有新的数据采集时，会不断删掉最老的数据，保证packetBuffer的数目保持在300，也即存储300个packet，新加入的packet会存储在末端back位置
		XsMutexLocker lock(m_mutex);
		// NOTE: Processing of packets should not be done in this thread.

		m_packetBuffer.push_back(*packet);
		if (m_packetBuffer.size() > 300)
		{
			//std::cout << std::endl;//~ 有新数据进来后，会造成“溢出”，在控制台中输出回车表示
			deleteOldestPacket();
		}
	}

private:
	mutable XsMutex m_mutex;
	std::list<XsDataPacket> m_packetBuffer;
	int m_mtwIndex;
	XsDevice* m_device;
};


//**********************************************~ xsens相关变量声明
const int desiredUpdateRate = 40;	// Use ? Hz update rate for MTWs
const int desiredRadioChannel = 20;	// Use radio channel 19 for wireless master.//~ 使用的channel号没有太大影响。一个station有23个channel，随便用哪个都可以的

WirelessMasterCallback wirelessMasterCallback; // Callback for wireless master//~ 这里的master对应的就是station
std::vector<MtwCallback*> mtwCallbacks; // Callbacks for mtw devices//~ mtw对应的就是IMU模块
std::ofstream outfile_xsens;//~ 创建txt文件，存储IMU数据
std::ofstream outfile_xsens_all;//~ 创建txt文件，存储IMU数据

FILE* xsens_data;
FILE* xsens_data_all;

XsControl* control = XsControl::construct();

bool xsensthread_flag = false;
bool xsens_is_ready = false;
float xsens_angle = 0;



//**********************************************~ xsens设备连接和关闭等操作函数
void read_xsens_alldata()
{//~ 40Hz
	
	//~ 存储各个IMU模块的数据到eulerData，可以输出或者存储
	std::vector<XsEuler> eulerData(mtwCallbacks.size()); // Room to store euler data for each mtw //~ eulerData的向量长度与IMU模块的数目相同，每次存储的数据都是会覆盖的
	XsTime::msleep(0);
	bool newDataAvailable = false;
	for (size_t i = 0; i < mtwCallbacks.size(); ++i)
	{
		if (mtwCallbacks[i]->dataAvailable())
		{
			newDataAvailable = true;
			XsDataPacket const* packet = mtwCallbacks[i]->getNewestPacket();
			eulerData[i] = packet->orientationEuler();//~ 获取角度数据			
		}
	}

	if (newDataAvailable)
	{//~ 分别输出每个IMU模块的三个角度数据
		//outfile_xsens_all << eulerData[0].roll() << " " << eulerData[0].pitch() << " " << eulerData[0].yaw() << std::endl;

		for (size_t i = 0; i < mtwCallbacks.size(); ++i)
		{
			
			if (i == mtwCallbacks.size() - 1)
			{
				fprintf(xsens_data_all, "%e,%e,%e\n", eulerData[i].roll(), eulerData[i].pitch(), eulerData[i].yaw());
			}
			else
			{
				fprintf(xsens_data_all, "%e,%e,%e,", eulerData[i].roll(), eulerData[i].pitch(), eulerData[i].yaw());
			}
		}

	}

}

void read_xsens_data()
{//~ 10Hz，与A超同频率

	//~ 存储各个IMU模块的数据到eulerData，可以输出或者存储
	std::vector<XsEuler> eulerData(mtwCallbacks.size()); // Room to store euler data for each mtw //~ eulerData的向量长度与IMU模块的数目相同，每次存储的数据都是会覆盖的
	XsTime::msleep(0);
	bool newDataAvailable = false;
	for (size_t i = 0; i < mtwCallbacks.size(); ++i)
	{
		if (mtwCallbacks[i]->dataAvailable())
		{
			newDataAvailable = true;
			XsDataPacket const* packet = mtwCallbacks[i]->getNewestPacket();
			eulerData[i] = packet->orientationEuler();//~ 获取角度数据
		}
	}

	if (newDataAvailable)
	{//~ 分别输出每个IMU模块的三个角度数据
		for (size_t i = 0; i < mtwCallbacks.size(); ++i)
		{
			/*std::cout << "[" << i << "]: ID: " << mtwCallbacks[i]->device().deviceId().toString().toStdString()
				<< ", Roll: " << std::setw(7) << std::fixed << std::setprecision(2) << eulerData[i].roll()
				<< ", Pitch: " << std::setw(7) << std::fixed << std::setprecision(2) << eulerData[i].pitch()
				<< ", Yaw: " << std::setw(7) << std::fixed << std::setprecision(2) << eulerData[i].yaw()
				<< "\n";*/

			
			if (i == mtwCallbacks.size() - 1)
			{
				fprintf(xsens_data, "%e,%e,%e\n", eulerData[i].roll(), eulerData[i].pitch(), eulerData[i].yaw());
			}
			else
			{
				fprintf(xsens_data, "%e,%e,%e,", eulerData[i].roll(), eulerData[i].pitch(), eulerData[i].yaw());
			}
		}
		
		//outfile_xsens << eulerData[0].roll() << " " << eulerData[0].pitch() << " " << eulerData[0].yaw() << std::endl;

		xsens_angle = eulerData[0].roll();
	}

}

void show_xsens_data()
{
	//~ 存储各个IMU模块的数据到eulerData，可以输出或者存储
	std::vector<XsEuler> eulerData(mtwCallbacks.size()); // Room to store euler data for each mtw //~ eulerData的向量长度与IMU模块的数目相同，每次存储的数据都是会覆盖的
	XsTime::msleep(0);
	bool newDataAvailable = false;
	for (size_t i = 0; i < mtwCallbacks.size(); ++i)
	{
		if (mtwCallbacks[i]->dataAvailable())
		{
			newDataAvailable = true;
			XsDataPacket const* packet = mtwCallbacks[i]->getNewestPacket();
			eulerData[i] = packet->orientationEuler();//~ 获取角度数据
		}
	}

	if (newDataAvailable)
	{//~ 分别输出每个IMU模块的三个角度数据
		for (size_t i = 0; i < mtwCallbacks.size(); ++i)
		{
			/*std::cout << "[" << i << "]: ID: " << mtwCallbacks[i]->device().deviceId().toString().toStdString()
				<< ", Roll: " << std::setw(7) << std::fixed << std::setprecision(2) << eulerData[i].roll()
				<< ", Pitch: " << std::setw(7) << std::fixed << std::setprecision(2) << eulerData[i].pitch()
				<< ", Yaw: " << std::setw(7) << std::fixed << std::setprecision(2) << eulerData[i].yaw()
				<< "\n";*/
		}
		xsens_angle = eulerData[0].roll();
	}

}

void close_xsens()
{

	//~ 关掉连接的配置状态
	//std::cout << "Closing XsControl..." << std::endl;
	control->close();

	//std::cout << "Deleting mtw callbacks..." << std::endl;
	for (std::vector<MtwCallback*>::iterator i = mtwCallbacks.begin(); i != mtwCallbacks.end(); ++i)
	{
		delete (*i);
	}

	std::cout << "\nDisconnect Xsens Success!\n" << std::endl;
	//std::cout << "Press [ENTER] to continue." << std::endl; std::cin.get();
}

/******************************************************* xsens代码中需要提前声明的部分（结束）**********************************/

/********************************************** 读取xsens数据的线程（开始）***************************************************/
class XsensThread : public QThread
{
	void run() // 线程入口函数，执行完就自动结束了这个线程，如果一直停在while循环内，就一直采集数据
	{
		while (xsensthread_flag)
		{
			read_xsens_alldata();	
			//this->msleep(25);//~ 对应40Hz的采样率
			this->msleep(24);//~ 实际测试采样率有点不一样
		}
	}
};

//~ 创建xsens线程对象
XsensThread* xsensthread = new XsensThread();//~ 创建连续采集xsens数据的子线程

/********************************************** 读取xsens数据的线程（结束）***************************************************/



/*********************************************** 主程序（入口）*****************************************************/
QCplot::QCplot(QWidget *parent)
	: QMainWindow(parent)
	, fileName(),stream(),savedata()
{
	p = new ELOXI_US_API();
	myPlayer = new Player();
	lda = new LDA_Bayesian();
	//paraDialog = new para();
	ui.setupUi(this);
	Data_x.resize(997);
	Data_y.resize(997);
	dataX.resize(997);
	dataY.resize(997);
	IMU_x.resize(100);
	IMU_y.resize(100);
	for (int i = 0; i != 997; ++i)
	{
		Amode_Data_x.push_back(i);
	}
	//for calibration
	capture = false;
	calibration = false;
	flag = false;
	start_save = false;
	save_flag = false;
	//declare array
	frameRate = 20;				
	holdTime = 5;

	
	//p->opendevice();

	//~ 创建wifi线程对象
	wifi_Thread = new thread_two();

	//~ TAC虚拟手
	TAC_is_ready = false;

	//parameter struct
	currentParametersSettings.channelNumber = 8;
	currentParametersSettings.windowLength = 20;
	currentParametersSettings.overlap = 0;
	currentParametersSettings.featureType = "mean and variance";
	
	initial();
	//train image directory
	image_dir = "gesture_robust";
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	initialSlots();

	/* //~ 虚拟手相关设置，虚拟串口，不影响腕手同步程序
	//virtual hand setting  
	//initialize queue and map!!!!
	gestureOrder = false;
	mCOMPort = new CSerialPort();
	nCOM = 4;
	if(mCOMPort->InitPort(nCOM)) 
	{
		printf("Classifier: COM%d initialization successful !\n",nCOM);

		if(mCOMPort->OpenListenThread())
			printf("Classifier: Open ListenThread successful !\n");
		else
			printf("Classifier: Open ListenThread failed !\n");
	}
	else
		printf("Classifier: COM%d initialization failed !\n",nCOM);

	*/

	gestureMap.insert(std::make_pair("0rest.jpg",'r'));//休息态
	gestureMap.insert(std::make_pair("2fist.jpg",'P'));//握拳
	gestureMap.insert(std::make_pair("3victory.jpg",'Q'));//胜利
	gestureMap.insert(std::make_pair("4pinch.jpg",'R'));//OK
	gestureMap.insert(std::make_pair("six.jpg",'S'));//手势6
	gestureMap.insert(std::make_pair("gun.jpg",'u'));//枪姿
	gestureMap.insert(std::make_pair("1index.jpg",'i'));//食指向前
	gestureMap.insert(std::make_pair("5triple.jpg",'n'));//三指捏
	gestureMap.insert(std::make_pair("neifan.jpg",'3'));//内翻
	gestureMap.insert(std::make_pair("waifan.jpg",'4'));//外翻
	gestureMap.insert(std::make_pair("shangqie.jpg",'6'));//上切
	gestureMap.insert(std::make_pair("xiaqie.jpg",'2'));//下切
	gestureMap.insert(std::make_pair("neifan.jpg",'1'));//内旋
	gestureMap.insert(std::make_pair("waifan.jpg",'5'));//外旋

	//5DT数据手套
	glo = new FiveDTglove();
	glo->pGlove = glo->getDataGlovePtr();

	//绘制数据手套角度		//~ 初始化控件
	setupRealTimeAngle();
	
	predict_Handangle = 0;
	predict_angle_plot = false;
	boundWrist[0] = 0, boundWrist[1] = 1;
	boundHand[0] = 0, boundHand[1] = 1;

	//Init_Socket(); //~ 连接虚拟手软件

	predictNum = 0;
	//std::ifstream inFile;
	//inFile.open("test.txt");
	//std::istream_iterator<float> begin(inFile);    
	//std::istream_iterator<float> end;          
	//vector<float> inData(begin,end);    
	//Mat tmpMat = Mat(inData);     
	//std::cout<<tmpMat<<' '<<tmpMat.rows;

	bool Handangle_flag = false;
	bool wristangle_flag = false;
	engine_is_open = false;
	predict_flag = false;
	predict_Wristangle = 0;
	predict_Handangle = 0;

	//~ delsys相关		
	Delsys_On_flag = false;
	delsys_sdk_thread = new Delsys_read_Thread();//~ 创建delsys sdk 读取数据的线程对象

	count_num = 0;

	/******************************************************* mode1相关 ************************************************/
	//~ mode1离散手势显示
	showTrainImage_mode1();
	PacketNumber_mode1 = 0;
	trialNum_mode1 = 5;//~ 设置mode1中的trail数目
	channelindex_mode1 = 1;//~ 主界面显示的通道信号
	//~ 初始化mode1训练进度条
	ui.train_progressBar_mode1->setMinimum(0);//~ 最小值
	ui.train_progressBar_mode1->setMaximum(frameRate* holdTime* trialNum_mode1* classNum_mode1);
	ui.train_progressBar_mode1->setValue(0);
	

	/******************************************************* mode2相关 ************************************************/
	//~ mode2离散手势显示
	showTrainImage_mode2();
	PacketNumber_mode2 = 0;
	trialNum_mode2 = 1;//~ 设置mode1中的trail数目
	channelindex_mode2 = 1;
	holdTime_mode2 = 50;
	//~ 初始化mode1训练进度条
	ui.train_progressBar_mode2->setMinimum(0);//~ 最小值
	ui.train_progressBar_mode2->setMaximum(frameRate* holdTime_mode2 * trialNum_mode2* classNum_mode2);
	ui.train_progressBar_mode2->setValue(0); 

	channelindex_mode2 = 1;//~ 主界面显示的通道信号

	/******************************************************* mode3相关(SPC) ************************************************/
	session_SPC = 1;
	//SPC训练时间,eg.60s = 20(eachMotion) * 3(session_SPC）
	eachMotion = 50;//~ 每个阶段需要做的动作周期数（单位是s），动作频率为1Hz

	//~ 初始化训练进度条
	ui.train_progressBar->setMinimum(0);//~ 最小值
	ui.train_progressBar->setMaximum(4 * eachMotion * frameRate * session_SPC);
	ui.train_progressBar->setValue(0);


	p->opendevice();

	// 串口
	serialPort = new QSerialPort();
	serialPortInfo = new QSerialPortInfo();
	serialPortInit();
	IMU_Thread = new IMUProcesser(serialPort);
	IMU_Datalength = 22;

	// EMG串口
	emgSerialPort = new QSerialPort();
	emgSerialPortInfo = new QSerialPortInfo();
	emgSerialPortInit();
	
	EMG_Thread = new EMGProcesser(emgSerialPort);
	EMG_Datalength = 8;
	
	isUpdatePlot = false;

	// disGesture显示手势
	showTrainImage_disGesture();
	packetNum_disGesture = 0;
	trialNum_disGesture = 2;
	holdTime_disGesture = 10;
	ui.disGesture_progressBar_1->setMinimum(0);
	ui.disGesture_progressBar_1->setMaximum(frameRate * holdTime_disGesture * trialNum_disGesture * classNum_disGesture);
	ui.disGesture_progressBar_1->setValue(0);

	// 参数设置窗口
	parameters_window_disGesture = new Parameters_dialog(this);
	parameters_window_disGesture->ui->parameters_trainTime->setText(QString::number(holdTime_disGesture));
	parameters_window_disGesture->holdTime = holdTime_disGesture;
	parameters_window_disGesture->ui->parameters_num->setText(QString::number(trialNum_disGesture));
	parameters_window_disGesture->trialNum = trialNum_disGesture;
} 


QCplot::~QCplot()
{
	delete lda;
	//delete paraDialog;
}
//~ 初始化信号和槽
void QCplot::initialSlots()
{	
	//调整为阻塞
	//QObject::connect(wifi_Thread->wifi_tcpClient, SIGNAL(readyRead()), this, SLOT(updatePlayerUI()), Qt::BlockingQueuedConnection);		// showImage是MyPlayer中的信号
	//QObject::connect(myPlayer,	SIGNAL(showImage()), this, SLOT(updatePlayerUI()),Qt::BlockingQueuedConnection);		// showImage是MyPlayer中的信号
	//QObject::connect(myPlayer,	SIGNAL(showImage(QImage)), this, SLOT(updatePlayerUI(QImage)),Qt::BlockingQueuedConnection);
	//QObject::connect(myPlayer,	SIGNAL(trainImage()), this, SLOT(getTrainFeatureC()),Qt::BlockingQueuedConnection);
	//QObject::connect(myPlayer,	SIGNAL(trainImage(QImage)), this, SLOT(getTrainFeatureC(QImage)),Qt::BlockingQueuedConnection);
	//QObject::connect(myPlayer,	SIGNAL(predictImage()), this, SLOT(getPredictFeatureC()),Qt::BlockingQueuedConnection);
	//QObject::connect(myPlayer,	SIGNAL(predictImage(QImage)),	this, SLOT(getPredictFeatureC(QImage)),Qt::BlockingQueuedConnection);
	
	//~ mode1
	QObject::connect(myPlayer, SIGNAL(trainImage_mode1()), this, SLOT(getTrainFeature_mode1()), Qt::BlockingQueuedConnection);
	//~ mode2
	//QObject::connect(myPlayer, SIGNAL(trainImage_mode2()), this, SLOT(getTrainFeature_mode2()), Qt::BlockingQueuedConnection);
	// disGesture
	//QObject::connect(myPlayer, SIGNAL(trainImage_disDesture()), this, SLOT(getTrainFeature_disGesture()), Qt::BlockingQueuedConnection);
	//QObject::connect(myPlayer, SIGNAL(trainImage_disDesture()), this, SLOT(getTrainFeature_disGesture_A_trigger()), Qt::BlockingQueuedConnection);

	QObject::connect(myPlayer, SIGNAL(predictImage_disDesture()), this, SLOT(getPredictFeature_disGesture()), Qt::BlockingQueuedConnection);
}

void QCplot::on_start_clicked()
{
	if (myPlayer->isStop())
	{
		myPlayer->threadStart();
		ui.start->setEnabled(false);
	}

	else
	{
		myPlayer->setStop();
		ui.start->setText("stop");
	}
}



void QCplot::on_stop_clicked()
{
	exit(0);
}

void QCplot::on_train_clicked()
{
	ui.train->setEnabled(false);
	showTrainImage();
	//按帧率来计算时间，不怕丢包
	ui.horizontalSlider->setMaximum(classNum*frameRate*holdTime);  
	ui.horizontalSlider->setValue(0);
	train_feature.release();										
	train_label.release();
	//训练时剔除瞬态数据使用
	packetNumber = 0;	
	//保存训练数据
	time_t  now_time = time(NULL);
	struct tm  *p;
	p = gmtime(&now_time);
	char filename[128] = {0};
	//存成.bin文件
	sprintf(fileName[0],"./train_pridict/train_%d-%d-%d-%d-%d_1.bin",1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min);
	sprintf(fileName[1],"./train_pridict/train_%d-%d-%d-%d-%d_2.bin",1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min);
	sprintf(fileName[2],"./train_pridict/train_%d-%d-%d-%d-%d_3.bin",1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min);
	sprintf(fileName[3],"./train_pridict/train_%d-%d-%d-%d-%d_4.bin",1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min);
	sprintf(fileName[4],"./train_pridict/train_%d-%d-%d-%d-%d_5.bin",1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min);
	sprintf(fileName[5],"./train_pridict/train_%d-%d-%d-%d-%d_6.bin",1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min);
	sprintf(fileName[6],"./train_pridict/train_%d-%d-%d-%d-%d_7.bin",1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min);
	sprintf(fileName[7],"./train_pridict/train_%d-%d-%d-%d-%d_8.bin",1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min);
	stream[0]=fopen(fileName[0],"ab+");
	stream[1]=fopen(fileName[1],"ab+");
	stream[2]=fopen(fileName[2],"ab+");
	stream[3]=fopen(fileName[3],"ab+");
	stream[4]=fopen(fileName[4],"ab+");
	stream[5]=fopen(fileName[5],"ab+");
	stream[6]=fopen(fileName[6],"ab+");
	stream[7]=fopen(fileName[7],"ab+");
	save_flag = true;
	//trainFeatureNormalize.release();		
	//配置完成，切换到训练模式
	myPlayer->threadTrain(); 

}

void QCplot::on_predict_clicked()
{	//预测动作
	//qDebug() << "*";
	predictTrial = 0;
	predict_feature.release();
	predict_label.release();
	//LDA和SVM同时预测
	predict_label_svm.release();
	predict_real_label.release();
	predictFeatureNormalize.release();
	Size frameSize(325,240);					
	//动作序号
	predictTrialOrder = trainTrialOrder;
	//用默认随机数打乱
	random_shuffle(predictTrialOrder.begin()+1, predictTrialOrder.end());
	//random_shuffle(predictTrialOrder.begin(), predictTrialOrder.end());
	for (int i = 0;i<(2*classNum -1);i++)
	{
		if (i%2 == 0)
		{
			predictTrialOrderNew.push_back(0);
			//predictTrialOrderNew[i] = 0;
		}
		else
		{
			predictTrialOrderNew.push_back(predictTrialOrder[i/2 + 1]);
		}
	}
	ui.predict->setEnabled(false);
	//隐藏进度条
	ui.horizontalSlider->close();
	myPlayer->threadPredict();	
	//for (int i = 0; i < predictTrialOrderNew.size(); ++i) qDebug() << predictTrialOrderNew[i] << "  ";
}

void QCplot::on_accuracy_clicked()
{
	calAccuracy();
	QString accurancy;
	accurancy = QString("%1").arg(predictAccurancy);
	ui.accuracy->setText(accurancy);
}

void QCplot::on_save_clicked()
{
	if (!flag)
	{
		gettime();
		flag= true;
		stream[0]=fopen(fileName[0],"ab+");
		stream[1]=fopen(fileName[1],"ab+");
		stream[2]=fopen(fileName[2],"ab+");
		stream[3]=fopen(fileName[3],"ab+");
		stream[4]=fopen(fileName[4],"ab+");
		stream[5]=fopen(fileName[5],"ab+");
		stream[6]=fopen(fileName[6],"ab+");
		stream[7]=fopen(fileName[7],"ab+");
		outfile.open(savedata);
	}
	else
	{
		flag= false;
		fclose(stream[0]);
		fclose(stream[1]);
		fclose(stream[2]);
		fclose(stream[3]);
		fclose(stream[4]);
		fclose(stream[5]);
		fclose(stream[6]);
		fclose(stream[7]);
		outfile.close();
	}
}

void QCplot::on_capture_clicked()
{
	capture = true;
	ui.capture->setEnabled(false);
}

void QCplot::on_calibration_clicked()

{
	if (calibration)
	{
		calibration = false;
		ui.calibration->setText(tr("calibration"));
	}else
	{
		calibration = true;
		ui.calibration->setText(tr("stopCali"));
		//FileStorage读取模板数据
		FileStorage fsRead("Template.xml", FileStorage::READ); 
		fsRead["Template"]>>calibrationImage; 
		fsRead.release(); 

	/*	FileStorage fsFeature("calibrationImage.xml", FileStorage::WRITE);
		fsFeature<<"calibrationImage"<<calibrationImage; 
		fsFeature.release();*/

	}

}
//~ 初始化信号通道显示控件
void QCplot::initial()
{
	// PR
	//ch1
	ui.customPlot->addGraph();		// 创建一个新的graph
	ui.customPlot->graph(0)->setPen(QPen(Qt::black));		// 绘制图形线
	for (int i=0; i<997; ++i)
	{
	  Data_x[i] = i;
	  Data_y[i] = 0;
	  dataX[i] = i;
	  dataY[i] = 0;
	}
	ui.customPlot->graph(0)->setData(Data_x, Data_y);
	ui.customPlot->graph(0)->rescaleAxes();
	// 设置图标的交互方式：可以鼠标拖动 | 可以滚轮缩放 | 可以选择plottable(图形、曲线)及数据
	ui.customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.customPlot->xAxis->setLabel("1/20 us");
	ui.customPlot->yAxis->setLabel("Amplitude");
	ui.customPlot->xAxis->setRange(0,997);
    ui.customPlot->yAxis->setRange(50,200);
	ui.customPlot->plotLayout()->insertRow(0);		// 在行索引0处插入一个单元格为空的新行
	ui.customPlot->plotLayout()->addElement(0, 0, new QCPTextElement(ui.customPlot, "CH_1"));		// 添加元素
	//ui.customPlot->graph(0)->setName("ch1");
	//ch2
	ui.customPlot1->addGraph();
	ui.customPlot1->graph(0)->setPen(QPen(Qt::black));
	for (int i=0; i<997; ++i)
	{
	  Data_x[i] = i;
	  Data_y[i] = 0; 
	}
	ui.customPlot1->graph(0)->setData(Data_x, Data_y);
	ui.customPlot1->graph(0)->rescaleAxes();
	ui.customPlot1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.customPlot1->xAxis->setLabel("1/20 us");
	ui.customPlot1->yAxis->setLabel("Amplitude");
	ui.customPlot1->xAxis->setRange(0,997);
    ui.customPlot1->yAxis->setRange(50,200);
	ui.customPlot1->plotLayout()->insertRow(0);
	ui.customPlot1->plotLayout()->addElement(0, 0, new QCPTextElement(ui.customPlot1, "CH_2")); 
	//ui.customPlot1->graph(0)->setName("ch2");
	//ch3
	ui.customPlot2->addGraph();
	ui.customPlot2->graph(0)->setPen(QPen(Qt::black));
	for (int i=0; i<997; ++i)
	{
	  Data_x[i] = i;
	  Data_y[i] = 0; 
	}
	ui.customPlot2->graph(0)->setData(Data_x, Data_y);
	ui.customPlot2->graph(0)->rescaleAxes();
	ui.customPlot2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.customPlot2->xAxis->setLabel("1/20 us");
	ui.customPlot2->yAxis->setLabel("Amplitude");
	ui.customPlot2->xAxis->setRange(0,997);
    ui.customPlot2->yAxis->setRange(50,200);
	ui.customPlot2->plotLayout()->insertRow(0);
	ui.customPlot2->plotLayout()->addElement(0, 0, new QCPTextElement(ui.customPlot2, "CH_3")); 
	//ui.customPlot2->graph(0)->setName("ch3");
	//ch4
	ui.customPlot3->addGraph();
	ui.customPlot3->graph(0)->setPen(QPen(Qt::black));
	for (int i=0; i<997; ++i)
	{
	  Data_x[i] = i;
	  Data_y[i] = 0; 
	}
	ui.customPlot3->graph(0)->setData(Data_x, Data_y);
	ui.customPlot3->graph(0)->rescaleAxes();
	ui.customPlot3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.customPlot3->xAxis->setLabel("1/20 us");
	ui.customPlot3->yAxis->setLabel("Amplitude");
	ui.customPlot3->xAxis->setRange(0,997);
    ui.customPlot3->yAxis->setRange(50,200);
	ui.customPlot3->plotLayout()->insertRow(0);
	ui.customPlot3->plotLayout()->addElement(0, 0, new QCPTextElement(ui.customPlot3, "CH_4")); 
	//ui.customPlot3->graph(0)->setName("ch4");


	//~ mode1 ch1
	ui.customPlot_mode1->addGraph();
	ui.customPlot_mode1->graph(0)->setPen(QPen(Qt::black));
	for (int i = 0; i < 997; ++i)
	{
		Data_x[i] = i;
		Data_y[i] = 0;
	}
	ui.customPlot_mode1->graph(0)->setData(Data_x, Data_y);
	ui.customPlot_mode1->graph(0)->rescaleAxes();
	ui.customPlot_mode1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.customPlot_mode1->xAxis->setLabel("1/20 us");
	ui.customPlot_mode1->yAxis->setLabel("Amplitude");
	ui.customPlot_mode1->xAxis->setRange(0, 997);
	ui.customPlot_mode1->yAxis->setRange(50, 200);
	ui.customPlot_mode1->plotLayout()->insertRow(0);
	ui.customPlot_mode1->plotLayout()->addElement(0, 0, new QCPTextElement(ui.customPlot_mode1, "CH_1"));
	//ui.customPlot_mode1->graph(0)->setName("ch4");

	//~ mode2 ch1
	ui.customPlot_mode2->addGraph();
	ui.customPlot_mode2->graph(0)->setPen(QPen(Qt::black));
	for (int i = 0; i < 997; ++i)
	{
		Data_x[i] = i;
		Data_y[i] = 0;
	}
	ui.customPlot_mode2->graph(0)->setData(Data_x, Data_y);
	ui.customPlot_mode2->graph(0)->rescaleAxes();
	ui.customPlot_mode2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.customPlot_mode2->xAxis->setLabel("1/20 us");
	ui.customPlot_mode2->yAxis->setLabel("Amplitude");
	ui.customPlot_mode2->xAxis->setRange(0, 997);
	ui.customPlot_mode2->yAxis->setRange(50, 200);
	ui.customPlot_mode2->plotLayout()->insertRow(0);
	ui.customPlot_mode2->plotLayout()->addElement(0, 0, new QCPTextElement(ui.customPlot_mode2, "CH_1"));
	//ui.customPlot_mode2->graph(0)->setName("ch4");

	// 串口
	for (int i = 0; i < IMU_x.size(); ++i)
	{
		IMU_x[i] = i;
		IMU_y[i] = 0;
	}
	QFont font;
	font.setPixelSize(12);
	font.setStyleStrategy(QFont::NoAntialias);

	ui.IMU_1_plot->legend->setFont(font);
	ui.IMU_1_plot->legend->setVisible(true);
	ui.IMU_1_plot->legend->setRowSpacing(-5);
	ui.IMU_1_plot->legend->setIconSize(12, 15);
	ui.IMU_1_plot->axisRect()->insetLayout()->setMargins(QMargins(0, 0, 0, 0));
	ui.IMU_1_plot->addGraph();
	ui.IMU_1_plot->graph(0)->setPen(QPen(Qt::black));
	//ui.IMU_1_plot->graph(0)->setData(IMU_x, IMU_y);
	ui.IMU_1_plot->graph(0)->rescaleAxes();
	ui.IMU_1_plot->graph(0)->setName("Accx");
	ui.IMU_1_plot->addGraph();
	ui.IMU_1_plot->graph(1)->setPen(QPen(Qt::blue));
	//ui.IMU_1_plot->graph(1)->setData(IMU_x, IMU_y);
	ui.IMU_1_plot->graph(1)->rescaleAxes();
	ui.IMU_1_plot->graph(1)->setName("Accy");
	ui.IMU_1_plot->addGraph();
	ui.IMU_1_plot->graph(2)->setPen(QPen(Qt::red));
	//ui.IMU_1_plot->graph(2)->setData(IMU_x, IMU_y);
	ui.IMU_1_plot->graph(2)->rescaleAxes();
	ui.IMU_1_plot->graph(2)->setName("Accz");

	ui.IMU_1_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	//ui.IMU_1_plot->xAxis->setLabel("1/20 us");
	ui.IMU_1_plot->yAxis->setLabel("Amplitude");
	ui.IMU_1_plot->xAxis->setRange(0, 1000);
	ui.IMU_1_plot->yAxis->setRange(-20, 20);
	//ui.IMU_1_plot->yAxis->setTickLabels(false);
	ui.IMU_1_plot->plotLayout()->insertRow(0);
	ui.IMU_1_plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui.IMU_1_plot, "CH_1"));

	ui.IMU_2_plot->legend->setFont(font);
	ui.IMU_2_plot->legend->setVisible(true);
	ui.IMU_2_plot->legend->setRowSpacing(-5);
	ui.IMU_2_plot->legend->setIconSize(12, 15);
	ui.IMU_2_plot->axisRect()->insetLayout()->setMargins(QMargins(0, 0, 0, 0));
	ui.IMU_2_plot->addGraph();
	ui.IMU_2_plot->graph(0)->setPen(QPen(Qt::black));
	//ui.IMU_2_plot->graph(0)->setData(IMU_x, IMU_y);
	ui.IMU_2_plot->graph(0)->rescaleAxes();
	ui.IMU_2_plot->graph(0)->setName("Accx");
	ui.IMU_2_plot->addGraph();
	ui.IMU_2_plot->graph(1)->setPen(QPen(Qt::blue));
	//ui.IMU_2_plot->graph(1)->setData(IMU_x, IMU_y);
	ui.IMU_2_plot->graph(1)->rescaleAxes();
	ui.IMU_2_plot->graph(1)->setName("Accy");
	ui.IMU_2_plot->addGraph();
	ui.IMU_2_plot->graph(2)->setPen(QPen(Qt::red));
	//ui.IMU_2_plot->graph(2)->setData(IMU_x, IMU_y);
	ui.IMU_2_plot->graph(2)->rescaleAxes();
	ui.IMU_2_plot->graph(2)->setName("Accz");

	ui.IMU_2_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	//ui.IMU_2_plot->xAxis->setLabel("1/20 us");
	ui.IMU_2_plot->yAxis->setLabel("Amplitude");
	ui.IMU_2_plot->xAxis->setRange(0, 1000);
	ui.IMU_2_plot->yAxis->setRange(-20, 20);
	//ui.IMU_2_plot->yAxis->setTickLabels(false);
	ui.IMU_2_plot->plotLayout()->insertRow(0);
	ui.IMU_2_plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui.IMU_2_plot, "CH_2"));

	ui.IMU_3_plot->legend->setFont(font);
	ui.IMU_3_plot->legend->setVisible(true);
	ui.IMU_3_plot->legend->setRowSpacing(-5);
	ui.IMU_3_plot->legend->setIconSize(12, 15);
	ui.IMU_3_plot->axisRect()->insetLayout()->setMargins(QMargins(0, 0, 0, 0));
	ui.IMU_3_plot->addGraph();
	ui.IMU_3_plot->graph(0)->setPen(QPen(Qt::black));
	//ui.IMU_3_plot->graph(0)->setData(IMU_x, IMU_y);
	ui.IMU_3_plot->graph(0)->rescaleAxes();
	ui.IMU_3_plot->graph(0)->setName("Accx");
	ui.IMU_3_plot->addGraph();
	ui.IMU_3_plot->graph(1)->setPen(QPen(Qt::blue));
	//ui.IMU_3_plot->graph(1)->setData(IMU_x, IMU_y);
	ui.IMU_3_plot->graph(1)->rescaleAxes();
	ui.IMU_3_plot->graph(1)->setName("Accy");
	ui.IMU_3_plot->addGraph();
	ui.IMU_3_plot->graph(2)->setPen(QPen(Qt::red));
	//ui.IMU_3_plot->graph(2)->setData(IMU_x, IMU_y);
	ui.IMU_3_plot->graph(2)->rescaleAxes();
	ui.IMU_3_plot->graph(2)->setName("Accz");

	ui.IMU_3_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	//ui.IMU_2_plot->xAxis->setLabel("1/20 us");
	ui.IMU_3_plot->yAxis->setLabel("Amplitude");
	ui.IMU_3_plot->xAxis->setRange(0, 1000);
	ui.IMU_3_plot->yAxis->setRange(-20, 20);
	//ui.IMU_3_plot->yAxis->setTickLabels(false);
	ui.IMU_3_plot->plotLayout()->insertRow(0);
	ui.IMU_3_plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui.IMU_3_plot, "CH_3"));

	ui.IMU_4_plot->legend->setFont(font);
	ui.IMU_4_plot->legend->setVisible(true);
	ui.IMU_4_plot->legend->setRowSpacing(-5);
	ui.IMU_4_plot->legend->setIconSize(12, 15);
	ui.IMU_4_plot->axisRect()->insetLayout()->setMargins(QMargins(0, 0, 0, 0));
	ui.IMU_4_plot->addGraph();
	ui.IMU_4_plot->graph(0)->setPen(QPen(Qt::black));
	//ui.IMU_4_plot->graph(0)->setData(IMU_x, IMU_y);
	ui.IMU_4_plot->graph(0)->rescaleAxes();
	ui.IMU_4_plot->graph(0)->setName("Accx");
	ui.IMU_4_plot->addGraph();
	ui.IMU_4_plot->graph(1)->setPen(QPen(Qt::blue));
	//ui.IMU_4_plot->graph(1)->setData(IMU_x, IMU_y);
	ui.IMU_4_plot->graph(1)->rescaleAxes();
	ui.IMU_4_plot->graph(1)->setName("Accy");
	ui.IMU_4_plot->addGraph();
	ui.IMU_4_plot->graph(2)->setPen(QPen(Qt::red));
	//ui.IMU_4_plot->graph(2)->setData(IMU_x, IMU_y);
	ui.IMU_4_plot->graph(2)->rescaleAxes();
	ui.IMU_4_plot->graph(2)->setName("Accz");

	ui.IMU_4_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	//ui.IMU_2_plot->xAxis->setLabel("1/20 us");
	ui.IMU_4_plot->yAxis->setLabel("Amplitude");
	ui.IMU_4_plot->xAxis->setRange(0, 1000);
	ui.IMU_4_plot->yAxis->setRange(-20, 20);
	//ui.IMU_4_plot->yAxis->setTickLabels(false);
	ui.IMU_4_plot->plotLayout()->insertRow(0);
	ui.IMU_4_plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui.IMU_4_plot, "CH_4"));
	
	// EMG
	ui.EMG_1_plot->legend->setFont(font);
	ui.EMG_1_plot->legend->setVisible(true);
	ui.EMG_1_plot->legend->setRowSpacing(-5);
	ui.EMG_1_plot->legend->setIconSize(12, 15);
	ui.EMG_1_plot->axisRect()->insetLayout()->setMargins(QMargins(0, 0, 0, 0));
	ui.EMG_1_plot->addGraph();
	ui.EMG_1_plot->graph(0)->setPen(QPen(Qt::black));
	ui.EMG_1_plot->graph(0)->rescaleAxes();
	ui.EMG_1_plot->graph(0)->setName("EMG");
	ui.EMG_1_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.EMG_1_plot->yAxis->setLabel("Amplitude");
	ui.EMG_1_plot->xAxis->setRange(0, 10000);
	ui.EMG_1_plot->yAxis->setRange(-5000, 5000);
	ui.EMG_1_plot->plotLayout()->insertRow(0);
	ui.EMG_1_plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui.EMG_1_plot, "CH_1"));

	ui.EMG_2_plot->legend->setFont(font);
	ui.EMG_2_plot->legend->setVisible(true);
	ui.EMG_2_plot->legend->setRowSpacing(-5);
	ui.EMG_2_plot->legend->setIconSize(12, 15);
	ui.EMG_2_plot->axisRect()->insetLayout()->setMargins(QMargins(0, 0, 0, 0));
	ui.EMG_2_plot->addGraph();
	ui.EMG_2_plot->graph(0)->setPen(QPen(Qt::black));
	ui.EMG_2_plot->graph(0)->rescaleAxes();
	ui.EMG_2_plot->graph(0)->setName("EMG");
	ui.EMG_2_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.EMG_2_plot->yAxis->setLabel("Amplitude");
	ui.EMG_2_plot->xAxis->setRange(0, 10000);
	ui.EMG_2_plot->yAxis->setRange(-5000, 5000);
	ui.EMG_2_plot->plotLayout()->insertRow(0);
	ui.EMG_2_plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui.EMG_2_plot, "CH_3"));

	ui.EMG_3_plot->legend->setFont(font);
	ui.EMG_3_plot->legend->setVisible(true);
	ui.EMG_3_plot->legend->setRowSpacing(-5);
	ui.EMG_3_plot->legend->setIconSize(12, 15);
	ui.EMG_3_plot->axisRect()->insetLayout()->setMargins(QMargins(0, 0, 0, 0));
	ui.EMG_3_plot->addGraph();
	ui.EMG_3_plot->graph(0)->setPen(QPen(Qt::black));
	ui.EMG_3_plot->graph(0)->rescaleAxes();
	ui.EMG_3_plot->graph(0)->setName("EMG");
	ui.EMG_3_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.EMG_3_plot->yAxis->setLabel("Amplitude");
	ui.EMG_3_plot->xAxis->setRange(0, 10000);
	ui.EMG_3_plot->yAxis->setRange(-5000, 5000);
	ui.EMG_3_plot->plotLayout()->insertRow(0);
	ui.EMG_3_plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui.EMG_3_plot, "CH_2"));

	ui.EMG_4_plot->legend->setFont(font);
	ui.EMG_4_plot->legend->setVisible(true);
	ui.EMG_4_plot->legend->setRowSpacing(-5);
	ui.EMG_4_plot->legend->setIconSize(12, 15);
	ui.EMG_4_plot->axisRect()->insetLayout()->setMargins(QMargins(0, 0, 0, 0));
	ui.EMG_4_plot->addGraph();
	ui.EMG_4_plot->graph(0)->setPen(QPen(Qt::black));
	ui.EMG_4_plot->graph(0)->rescaleAxes();
	ui.EMG_4_plot->graph(0)->setName("EMG");
	ui.EMG_4_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.EMG_4_plot->yAxis->setLabel("Amplitude");
	ui.EMG_4_plot->xAxis->setRange(0, 10000);
	ui.EMG_4_plot->yAxis->setRange(-5000, 5000);
	ui.EMG_4_plot->plotLayout()->insertRow(0);
	ui.EMG_4_plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui.EMG_4_plot, "CH_4"));
	
	ui.Amode_1_plot->legend->setFont(font);
	ui.Amode_1_plot->legend->setVisible(true);
	ui.Amode_1_plot->legend->setRowSpacing(-5);
	ui.Amode_1_plot->legend->setIconSize(12, 15);
	ui.Amode_1_plot->axisRect()->insetLayout()->setMargins(QMargins(0, 0, 0, 0));
	ui.Amode_1_plot->addGraph();
	ui.Amode_1_plot->graph(0)->setPen(QPen(Qt::black));
	ui.Amode_1_plot->graph(0)->rescaleAxes();
	ui.Amode_1_plot->graph(0)->setName("Amode");
	ui.Amode_1_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.Amode_1_plot->yAxis->setLabel("Amplitude");
	ui.Amode_1_plot->xAxis->setRange(0, 1000);
	ui.Amode_1_plot->yAxis->setRange(0, 200);
	ui.Amode_1_plot->plotLayout()->insertRow(0);
	ui.Amode_1_plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui.Amode_1_plot, "CH_1"));

	ui.Amode_2_plot->legend->setFont(font);
	ui.Amode_2_plot->legend->setVisible(true);
	ui.Amode_2_plot->legend->setRowSpacing(-5);
	ui.Amode_2_plot->legend->setIconSize(12, 15);
	ui.Amode_2_plot->axisRect()->insetLayout()->setMargins(QMargins(0, 0, 0, 0));
	ui.Amode_2_plot->addGraph();
	ui.Amode_2_plot->graph(0)->setPen(QPen(Qt::black));
	ui.Amode_2_plot->graph(0)->rescaleAxes();
	ui.Amode_2_plot->graph(0)->setName("Amode");
	ui.Amode_2_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.Amode_2_plot->yAxis->setLabel("Amplitude");
	ui.Amode_2_plot->xAxis->setRange(0, 1000);
	ui.Amode_2_plot->yAxis->setRange(0, 200);
	ui.Amode_2_plot->plotLayout()->insertRow(0);
	ui.Amode_2_plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui.Amode_2_plot, "CH_2"));

	ui.Amode_3_plot->legend->setFont(font);
	ui.Amode_3_plot->legend->setVisible(true);
	ui.Amode_3_plot->legend->setRowSpacing(-5);
	ui.Amode_3_plot->legend->setIconSize(12, 15);
	ui.Amode_3_plot->axisRect()->insetLayout()->setMargins(QMargins(0, 0, 0, 0));
	ui.Amode_3_plot->addGraph();
	ui.Amode_3_plot->graph(0)->setPen(QPen(Qt::black));
	ui.Amode_3_plot->graph(0)->rescaleAxes();
	ui.Amode_3_plot->graph(0)->setName("Amode");
	ui.Amode_3_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.Amode_3_plot->yAxis->setLabel("Amplitude");
	ui.Amode_3_plot->xAxis->setRange(0, 1000);
	ui.Amode_3_plot->yAxis->setRange(0, 200);
	ui.Amode_3_plot->plotLayout()->insertRow(0);
	ui.Amode_3_plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui.Amode_3_plot, "CH_3"));

	ui.Amode_4_plot->legend->setFont(font);
	ui.Amode_4_plot->legend->setVisible(true);
	ui.Amode_4_plot->legend->setRowSpacing(-5);
	ui.Amode_4_plot->legend->setIconSize(12, 15);
	ui.Amode_4_plot->axisRect()->insetLayout()->setMargins(QMargins(0, 0, 0, 0));
	ui.Amode_4_plot->addGraph();
	ui.Amode_4_plot->graph(0)->setPen(QPen(Qt::black));
	ui.Amode_4_plot->graph(0)->rescaleAxes();
	ui.Amode_4_plot->graph(0)->setName("Amode");
	ui.Amode_4_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.Amode_4_plot->yAxis->setLabel("Amplitude");
	ui.Amode_4_plot->xAxis->setRange(0, 1000);
	ui.Amode_4_plot->yAxis->setRange(0, 200);
	ui.Amode_4_plot->plotLayout()->insertRow(0);
	ui.Amode_4_plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui.Amode_4_plot, "CH_4"));

	// disGesture
	ui.disGesture_Amode_plot->legend->setFont(font);
	ui.disGesture_Amode_plot->legend->setVisible(true);
	ui.disGesture_Amode_plot->legend->setRowSpacing(-5);
	ui.disGesture_Amode_plot->legend->setIconSize(12, 15);
	ui.disGesture_Amode_plot->axisRect()->insetLayout()->setMargins(QMargins(0, 0, 0, 0));
	ui.disGesture_Amode_plot->addGraph();
	ui.disGesture_Amode_plot->graph(0)->setPen(QPen(Qt::black));
	ui.disGesture_Amode_plot->graph(0)->rescaleAxes();
	ui.disGesture_Amode_plot->graph(0)->setName("Amode");
	ui.disGesture_Amode_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.disGesture_Amode_plot->yAxis->setLabel("Amplitude");
	ui.disGesture_Amode_plot->xAxis->setRange(0, 1000);
	ui.disGesture_Amode_plot->yAxis->setRange(0, 200);
	ui.disGesture_Amode_plot->plotLayout()->insertRow(0);
	ui.disGesture_Amode_plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui.disGesture_Amode_plot, "CH_1"));

	ui.disGesture_EMG_plot->legend->setFont(font);
	ui.disGesture_EMG_plot->legend->setVisible(true);
	ui.disGesture_EMG_plot->legend->setRowSpacing(-5);
	ui.disGesture_EMG_plot->legend->setIconSize(12, 15);
	ui.disGesture_EMG_plot->axisRect()->insetLayout()->setMargins(QMargins(0, 0, 0, 0));
	ui.disGesture_EMG_plot->addGraph();
	ui.disGesture_EMG_plot->graph(0)->setPen(QPen(Qt::black));
	ui.disGesture_EMG_plot->graph(0)->rescaleAxes();
	ui.disGesture_EMG_plot->graph(0)->setName("EMG");
	ui.disGesture_EMG_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.disGesture_EMG_plot->yAxis->setLabel("Amplitude");
	ui.disGesture_EMG_plot->xAxis->setRange(0, 10000);
	ui.disGesture_EMG_plot->yAxis->setRange(-5000, 5000);
	ui.disGesture_EMG_plot->plotLayout()->insertRow(0);
	ui.disGesture_EMG_plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui.disGesture_EMG_plot, "CH_1"));

	ui.disGesture_IMU_plot->legend->setFont(font);
	ui.disGesture_IMU_plot->legend->setVisible(true);
	ui.disGesture_IMU_plot->legend->setRowSpacing(-5);
	ui.disGesture_IMU_plot->legend->setIconSize(12, 15);
	ui.disGesture_IMU_plot->axisRect()->insetLayout()->setMargins(QMargins(0, 0, 0, 0));
	ui.disGesture_IMU_plot->addGraph();
	ui.disGesture_IMU_plot->graph(0)->setPen(QPen(Qt::black));
	ui.disGesture_IMU_plot->graph(0)->rescaleAxes();
	ui.disGesture_IMU_plot->graph(0)->setName("Accx");
	ui.disGesture_IMU_plot->addGraph();
	ui.disGesture_IMU_plot->graph(1)->setPen(QPen(Qt::blue));
	ui.disGesture_IMU_plot->graph(1)->rescaleAxes();
	ui.disGesture_IMU_plot->graph(1)->setName("Accy");
	ui.disGesture_IMU_plot->addGraph();
	ui.disGesture_IMU_plot->graph(2)->setPen(QPen(Qt::red));
	ui.disGesture_IMU_plot->graph(2)->rescaleAxes();
	ui.disGesture_IMU_plot->graph(2)->setName("Accz");

	ui.disGesture_IMU_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	ui.disGesture_IMU_plot->yAxis->setLabel("Amplitude");
	ui.disGesture_IMU_plot->xAxis->setRange(0, 1000);
	ui.disGesture_IMU_plot->yAxis->setRange(-20, 20);
	ui.disGesture_IMU_plot->plotLayout()->insertRow(0);
	ui.disGesture_IMU_plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui.disGesture_IMU_plot, "CH_1"));

	// replot
	// 更新图形，必须调用
	ui.customPlot->replot();
	ui.customPlot1->replot();
	ui.customPlot2->replot();
	ui.customPlot3->replot();
	ui.customPlot_mode1->replot();

	ui.IMU_1_plot->replot();
	ui.IMU_2_plot->replot();
	ui.IMU_3_plot->replot();
	ui.IMU_4_plot->replot();

	ui.EMG_1_plot->replot();
	ui.EMG_2_plot->replot();
	ui.EMG_3_plot->replot();
	ui.EMG_4_plot->replot();

	ui.Amode_1_plot->replot();
	ui.Amode_2_plot->replot();
	ui.Amode_3_plot->replot();
	ui.Amode_4_plot->replot();

	ui.disGesture_Amode_plot->replot();
	ui.disGesture_EMG_plot->replot();
	ui.disGesture_IMU_plot->replot();
}
//~ 给需要存储数据的文件，以当前时间进行命名。如果文件被.open，则以gettime函数中读取的当前时间和对应格式命名该文件，并写入数据。文件被.close，则关闭当前文件，停止数据写入
void QCplot::gettime()
{
	char file_date[20];
	char buff[9];
	sprintf(file_date,"2008.12.26_09.32.27");					//LB 将file_data改成系统时间
	_strdate_s( buff, 9 );									//LB复制当前系统日期到缓冲区 _strdate_s( tmpbuf, 128 ); printf( "OS date:\t\t\t\t%s\n", tmpbuf ); OS date: 04/25/03  2003年4月25日
	file_date[2] = buff[6];
	file_date[3] = buff[7];
	file_date[5] = buff[0];
	file_date[6] = buff[1];
	file_date[8] = buff[3];
	file_date[9] = buff[4];
	_strtime_s( buff, 9 );									//LB写入时间到缓冲区      _strtime_s( tmpbuf, 128 );   printf( "OS time:\t\t\t\t%s\n", tmpbuf );    OS time: 13:51:23
	file_date[11] = buff[0];
	file_date[12] = buff[1];
	file_date[14] = buff[3];
	file_date[15] = buff[4];								//LB将file_date改为系统当前时间
	//时间精确到秒s
	file_date[17] = buff[6];
	file_date[18] = buff[7];
	//~ sprintf_s函数将数据格式化输出到字符串，sprintf_s()是sprintf()的安全版本，通过指定缓冲区长度来避免sprintf()存在的溢出风险。
	sprintf_s(fileName[0],"Mat_file//%s_1.bin",file_date);		//LBdata文件夹  “日期_label.txt”文件
	sprintf_s(fileName[1],"Mat_file//%s_2.bin",file_date); 
	sprintf_s(fileName[2],"Mat_file//%s_3.bin",file_date);
	sprintf_s(fileName[3],"Mat_file//%s_4.bin",file_date);
	sprintf_s(fileName[4],"Mat_file//%s_5.bin",file_date);
	sprintf_s(fileName[5],"Mat_file//%s_6.bin",file_date);
	sprintf_s(fileName[6],"Mat_file//%s_7.bin",file_date);
	sprintf_s(fileName[7],"Mat_file//%s_8.bin",file_date);
	//for data glove
	sprintf_s(savedata,"Mat_file//%s.txt",file_date);//~ 这里的savedata就是包含完整文件名的char数组
	sprintf_s(save_dataglove, "Mat_file//%s_dataglove.txt", file_date);//~ 存储dataglove的原始数据，也即训练过程中的train_label
	sprintf_s(savedata_xsens, "Mat_file//%s_Xsens.txt", file_date);//~ 保存xsens的数据，与A超的频率相同
	sprintf_s(savedata_xsens_all, "Mat_file//%s_Xsens_All.txt", file_date);//~ 保存全部的xsens数据

	sprintf_s(save_xsens, "Mat_file//%s_Xsens.csv", file_date);//~ 保存xsens的数据，与A超的频率相同
	sprintf_s(save_xsens_all, "Mat_file//%s_Xsens_All.csv", file_date);//~ 保存全部的xsens数据
}
//~ 显示A超信号和数据手套的角度信息
void QCplot::updatePlayerUI()
{
	if (xsens_is_ready)
	{
		show_xsens_data();
	}
	
	std::vector<std::vector<double>> CurrentPackage;
	if (p->copydata1(CurrentPackage) == true || wifi_Thread->copydata_wifi(CurrentPackage) == true)
	{
		showimage1(CurrentPackage);
		if (glo->datagloveused || xsens_is_ready)
		{
			float labelValue = fdGetSensorScaled(glo->pGlove, FD_MIDDLENEAR);
			showRealTimeAngle(labelValue, xsens_angle);
		}
	}
	
	//showIMUImage();
}

void QCplot::showIMUImage()
{
	/*IMU_y = QVector<double>::fromStdVector(IMU_1_Roll);
	ui.IMU_2_plot->graph(0)->setData(IMU_x, IMU_y);
	ui.IMU_2_plot->replot();*/
	//qDebug() << IMU_1_Roll[4];
	//qDebug() << IMU_y;
	int* plotSize = &(IMU_Thread->imuBuffer->bufferSize);
	std::queue<double>* tempRollQueue = &(IMU_Thread->imuBuffer->array[1][0]);
	std::vector<double>* tempRollVector = &(IMU_Thread->imuBuffer->vectorArray[1][0]);
	std::queue<double>* tempPitchQueue = &(IMU_Thread->imuBuffer->array[1][1]);
	std::queue<double>* tempYawQueue = &(IMU_Thread->imuBuffer->array[1][2]);
	/*if (!(*tempRollQueue).empty())
	{
		if ((*tempRollVector).size() < (*plotSize))
		{
			(*tempRollVector).push_back((*tempRollQueue).back());
			(*tempRollQueue).pop();
		}
		else
		{
			qDebug() << "in plot" << endl;
			IMU_y = QVector<double>::fromStdVector(*tempRollVector);
			ui.IMU_1_plot->graph(0)->setData(IMU_x, IMU_y);
			(*tempRollVector).clear();
		}
	}*/
	if (!(*tempRollQueue).empty() && !(*tempPitchQueue).empty() && !(*tempYawQueue).empty())
	{
		double addRollNum = (double)(*tempRollQueue).back();
		double addPitchNum = (double)(*tempPitchQueue).back();
		double addYawNum = (double)(*tempYawQueue).back();
		(*tempRollQueue).pop();
		(*tempPitchQueue).pop();
		(*tempYawQueue).pop();
		ui.IMU_1_plot->graph(0)->addData(key_IMU_1, addRollNum);
		ui.IMU_1_plot->graph(1)->addData(key_IMU_1, addPitchNum);
		ui.IMU_1_plot->graph(2)->addData(key_IMU_1, addYawNum);
	}
	key_IMU_1++;
	if (key_IMU_1 > 1000)
	{
		ui.IMU_1_plot->xAxis->setRange(key_IMU_1 - 999.9, key_IMU_1 + 0.1);
	}
	ui.IMU_1_plot->replot();
	
}
void QCplot::showIMUImage_2()
{
	int* plotSize = &(IMU_Thread->imuBuffer->bufferSize);
	std::queue<double>* tempRollQueue = &(IMU_Thread->imuBuffer->array[2][0]);
	std::queue<double>* tempPitchQueue = &(IMU_Thread->imuBuffer->array[2][1]);
	std::queue<double>* tempYawQueue = &(IMU_Thread->imuBuffer->array[2][2]);
	if (!(*tempRollQueue).empty() && !(*tempPitchQueue).empty() && !(*tempYawQueue).empty())
	{
		double addRollNum = (double)(*tempRollQueue).back();
		double addPitchNum = (double)(*tempPitchQueue).back();
		double addYawNum = (double)(*tempYawQueue).back();
		(*tempRollQueue).pop();
		(*tempPitchQueue).pop();
		(*tempYawQueue).pop();
		ui.IMU_2_plot->graph(0)->addData(key_IMU_2, addRollNum);
		ui.IMU_2_plot->graph(1)->addData(key_IMU_2, addPitchNum);
		ui.IMU_2_plot->graph(2)->addData(key_IMU_2, addYawNum);
	}
	key_IMU_2++;
	if (key_IMU_2 > 1000)
	{
		ui.IMU_2_plot->xAxis->setRange(key_IMU_2 - 999.9, key_IMU_2 + 0.1);
	}
	ui.IMU_2_plot->replot();
}

void QCplot::showIMUImage_3()
{
	//IMU_y = QVector<double>::fromStdVector(IMU_2_Roll);
	//ui.IMU_3_plot->graph(0)->setData(IMU_x, IMU_y);
	//ui.IMU_3_plot->replot();
	////qDebug() << IMU_1_Roll[4];
	////qDebug() << IMU_y;
	int* plotSize = &(IMU_Thread->imuBuffer->bufferSize);
	std::queue<double>* tempRollQueue = &(IMU_Thread->imuBuffer->array[3][0]);
	std::queue<double>* tempPitchQueue = &(IMU_Thread->imuBuffer->array[3][1]);
	std::queue<double>* tempYawQueue = &(IMU_Thread->imuBuffer->array[3][2]);
	if (!(*tempRollQueue).empty() && !(*tempPitchQueue).empty() && !(*tempYawQueue).empty())
	{
		double addRollNum = (double)(*tempRollQueue).back();
		double addPitchNum = (double)(*tempPitchQueue).back();
		double addYawNum = (double)(*tempYawQueue).back();
		(*tempRollQueue).pop();
		(*tempPitchQueue).pop();
		(*tempYawQueue).pop();
		ui.IMU_3_plot->graph(0)->addData(key_IMU_3, addRollNum);
		ui.IMU_3_plot->graph(1)->addData(key_IMU_3, addPitchNum);
		ui.IMU_3_plot->graph(2)->addData(key_IMU_3, addYawNum);
	}
	key_IMU_3++;
	if (key_IMU_3 > 1000)
	{
		ui.IMU_3_plot->xAxis->setRange(key_IMU_3 - 999.9, key_IMU_3 + 0.1);
	}
	ui.IMU_3_plot->replot();
}

void QCplot::showIMUImage_4()
{
	IMU_y = QVector<double>::fromStdVector(IMU_3_Roll);
	ui.IMU_4_plot->graph(0)->setData(IMU_x, IMU_y);
	ui.IMU_4_plot->replot();
	//qDebug() << IMU_1_Roll[4];
	//qDebug() << IMU_y;
}
void QCplot::showUnionIMUImage()
{
	vector<queue<double>*> tempRowQueue(3), tempPitchQueue(3), tempYawQueue(3);
	for (int i = 0; i != 3; ++i)
	{
		tempRowQueue[i] = &(EMG_Thread->imuBuffer->array[i][0]);
		tempPitchQueue[i] = &(EMG_Thread->imuBuffer->array[i][1]);
		tempYawQueue[i] = &(EMG_Thread->imuBuffer->array[i][2]);
	}
	bool isEmpty = false;
	if (!tempRowQueue[0]->empty())
	{
		ui.IMU_1_plot->graph(0)->addData(key_IMU_1, double(tempRowQueue[0]->back()));
		tempRowQueue[0]->pop();
	}
	if (!tempPitchQueue[0]->empty())
	{
		ui.IMU_1_plot->graph(1)->addData(key_IMU_1, double(tempPitchQueue[0]->back()));
		tempPitchQueue[0]->pop();
	}
	if (!tempYawQueue[0]->empty())
	{
		ui.IMU_1_plot->graph(2)->addData(key_IMU_1, double(tempYawQueue[0]->back()));
		tempYawQueue[0]->pop();
	}

	if (!tempRowQueue[1]->empty())
	{
		ui.IMU_2_plot->graph(0)->addData(key_IMU_1, double(tempRowQueue[1]->back()));
		tempRowQueue[1]->pop();
	}
	if (!tempPitchQueue[1]->empty())
	{
		ui.IMU_2_plot->graph(1)->addData(key_IMU_1, double(tempPitchQueue[1]->back()));
		tempPitchQueue[1]->pop();
	}
	if (!tempYawQueue[1]->empty())
	{
		ui.IMU_2_plot->graph(2)->addData(key_IMU_1, double(tempYawQueue[1]->back()));
		tempYawQueue[1]->pop();
	}

	if (!tempRowQueue[2]->empty())
	{
		ui.IMU_3_plot->graph(0)->addData(key_IMU_1, double(tempRowQueue[2]->back()));
		tempRowQueue[2]->pop();
	}
	if (!tempPitchQueue[2]->empty())
	{
		ui.IMU_3_plot->graph(1)->addData(key_IMU_1, double(tempPitchQueue[2]->back()));
		tempPitchQueue[2]->pop();
	}
	if (!tempYawQueue[2]->empty())
	{
		ui.IMU_3_plot->graph(2)->addData(key_IMU_1, double(tempYawQueue[2]->back()));
		tempYawQueue[2]->pop();
	}

	key_IMU_1++;
	if (key_IMU_1 > 1000)
	{
		ui.IMU_1_plot->xAxis->setRange(key_IMU_1 - 950, key_IMU_1 + 50);
		ui.IMU_2_plot->xAxis->setRange(key_IMU_1 - 950, key_IMU_1 + 50);
		ui.IMU_3_plot->xAxis->setRange(key_IMU_1 - 950, key_IMU_1 + 50);
		ui.IMU_4_plot->xAxis->setRange(key_IMU_1 - 950, key_IMU_1 + 50);
	}
	if (dataTabFlag)
	{
		ui.IMU_1_plot->replot();
		ui.IMU_2_plot->replot();
		ui.IMU_3_plot->replot();
		ui.IMU_4_plot->replot();
	}
}
void QCplot::showUnionIMUImage_2()
{
	vector<vector<double>> IMU_roll_array(4);
	vector<vector<double>> IMU_pitch_array(4);
	vector<vector<double>> IMU_yaw_array(4);
	vector<QVector<double>> IMU_roll_plot_y_array(4);
	vector<QVector<double>> IMU_pitch_plot_y_array(4);
	vector<QVector<double>> IMU_yaw_plot_y_array(4);
	for (int i = 0; i < 4; ++i)
	{
		auto IMU_Roll_Ptr = &(EMG_Thread->imuBuffer->deArray[i][0]);
		auto IMU_Pitch_Ptr = &(EMG_Thread->imuBuffer->deArray[i][1]);
		auto IMU_Yaw_Ptr = &(EMG_Thread->imuBuffer->deArray[i][2]);
		IMU_roll_array[i] = vector<double>(IMU_Roll_Ptr->begin(), IMU_Roll_Ptr->end());
		IMU_pitch_array[i] = vector<double>(IMU_Pitch_Ptr->begin(), IMU_Pitch_Ptr->end());
		IMU_yaw_array[i] = vector<double>(IMU_Yaw_Ptr->begin(), IMU_Yaw_Ptr->end());
		IMU_roll_plot_y_array[i] = QVector<double>::fromStdVector(IMU_roll_array[i]);
		IMU_pitch_plot_y_array[i] = QVector<double>::fromStdVector(IMU_pitch_array[i]);
		IMU_yaw_plot_y_array[i] = QVector<double>::fromStdVector(IMU_yaw_array[i]);
 	}
	QVector<double> IMU_plot_x = EMG_Thread->imuBuffer->Data_x;
	ui.IMU_1_plot->graph(0)->setData(IMU_plot_x, IMU_roll_plot_y_array[0]);
	ui.IMU_1_plot->graph(1)->setData(IMU_plot_x, IMU_pitch_plot_y_array[0]);
	ui.IMU_1_plot->graph(2)->setData(IMU_plot_x, IMU_yaw_plot_y_array[0]);

	ui.IMU_2_plot->graph(0)->setData(IMU_plot_x, IMU_roll_plot_y_array[1]);
	ui.IMU_2_plot->graph(1)->setData(IMU_plot_x, IMU_pitch_plot_y_array[1]);
	ui.IMU_2_plot->graph(2)->setData(IMU_plot_x, IMU_yaw_plot_y_array[1]);

	ui.IMU_3_plot->graph(0)->setData(IMU_plot_x, IMU_roll_plot_y_array[2]);
	ui.IMU_3_plot->graph(1)->setData(IMU_plot_x, IMU_pitch_plot_y_array[2]);
	ui.IMU_3_plot->graph(2)->setData(IMU_plot_x, IMU_yaw_plot_y_array[2]);

	ui.IMU_4_plot->graph(0)->setData(IMU_plot_x, IMU_roll_plot_y_array[3]);
	ui.IMU_4_plot->graph(1)->setData(IMU_plot_x, IMU_pitch_plot_y_array[3]);
	ui.IMU_4_plot->graph(2)->setData(IMU_plot_x, IMU_yaw_plot_y_array[3]);

	ui.IMU_1_plot->replot();
	ui.IMU_2_plot->replot();
	ui.IMU_3_plot->replot();
	ui.IMU_4_plot->replot();
}
void QCplot::showEMGImage_1()
{
	//std::queue<double>* c_1_tempQueue = &(EMG_Thread->emgBuffer->array[0]);
	std::vector<std::queue<double>*> tempQueue;
	tempQueue.resize(4);
	for (int i = 0; i < 4; i++)
	{
		tempQueue[i] = &(EMG_Thread->emgBuffer->array[i]);
	}
	bool isEmpty = false;
	//qDebug() << tempQueue[0] << endl;
	for (int i = 0; i < 4; i++)
	{
		if (tempQueue[i]->empty())
			isEmpty = true;
	}
	//qDebug() << isEmpty << endl;
	while (!isEmpty)
	{
		
		ui.EMG_1_plot->graph(0)->addData(key_EMG_1, double(tempQueue[0]->back()));
		//qDebug() << double(tempQueue[0]->back()) << endl;
		tempQueue[0]->pop();		// == (*(tempQueue[0])).pop()
		ui.EMG_2_plot->graph(0)->addData(key_EMG_1, double(tempQueue[1]->back()));
		tempQueue[1]->pop();
		ui.EMG_3_plot->graph(0)->addData(key_EMG_1, double(tempQueue[2]->back()));
		tempQueue[2]->pop();
		ui.EMG_4_plot->graph(0)->addData(key_EMG_1, double(tempQueue[3]->back()));
		tempQueue[3]->pop();
		for (int i = 0; i < 4; i++)
		{
			if (tempQueue[i]->empty())
				isEmpty = true;
		}
		key_EMG_1++;
		if (key_EMG_1 > 10000)
		{
			ui.EMG_1_plot->xAxis->setRange(key_EMG_1 - 9950, key_EMG_1 + 50);
			ui.EMG_2_plot->xAxis->setRange(key_EMG_1 - 9950, key_EMG_1 + 50);
			ui.EMG_3_plot->xAxis->setRange(key_EMG_1 - 9950, key_EMG_1 + 50);
			ui.EMG_4_plot->xAxis->setRange(key_EMG_1 - 9950, key_EMG_1 + 50);
		}
	}
	/*key_EMG_1++;
	ui.EMG_1_plot->replot();*/
	if (dataTabFlag)
	{
		ui.EMG_1_plot->replot();
		ui.EMG_2_plot->replot();
		ui.EMG_3_plot->replot();
		ui.EMG_4_plot->replot();
	}

}
void QCplot::showEMGImage_2()
{
	vector<vector<double>> EMGDataArray(4);
	vector<QVector<double>> EMG_plot_y_array(4);
	for (int i = 0; i < 4; ++i)
	{
		auto EMGPtr = &(EMG_Thread->emgBuffer->deArray[i]);
		EMGDataArray[i] = vector<double>(EMGPtr->begin(), EMGPtr->end());
		EMG_plot_y_array[i] = QVector<double>::fromStdVector(EMGDataArray[i]);
	}
	QVector<double> EMG_plot_x = EMG_Thread->emgBuffer->Data_x;
	ui.EMG_1_plot->graph(0)->setData(EMG_plot_x, EMG_plot_y_array[0]);
	ui.EMG_2_plot->graph(0)->setData(EMG_plot_x, EMG_plot_y_array[1]);
	ui.EMG_3_plot->graph(0)->setData(EMG_plot_x, EMG_plot_y_array[2]);
	ui.EMG_4_plot->graph(0)->setData(EMG_plot_x, EMG_plot_y_array[3]);

	if (dataTabFlag)
	{
		ui.EMG_1_plot->replot();
		ui.EMG_2_plot->replot();
		ui.EMG_3_plot->replot();
		ui.EMG_4_plot->replot();
	}
}

void QCplot::showAmodeImage_1(std::vector<std::vector<double>> CurrentPackage)
{
	uint8_t channelNums = 4;
	uint8_t channelNum = 0;
	for (int i = 0; i != channelNums; ++i)
	{
		std::vector<double> temp(CurrentPackage[i].begin() + 2, CurrentPackage[i].end() - 1);
		dataY = QVector<double>::fromStdVector(temp);
		channelNum = CurrentPackage[i][1];
		switch (channelNum)
		{
		case 1:
			ui.Amode_1_plot->graph(0)->setData(dataX, dataY);
			ui.Amode_1_plot->replot();
			break;
		case 2:
			ui.Amode_2_plot->graph(0)->setData(dataX, dataY);
			ui.Amode_2_plot->replot();
			break;
		case 3:
			ui.Amode_3_plot->graph(0)->setData(dataX, dataY);
			ui.Amode_3_plot->replot();
			break;
		case 4:
			ui.Amode_4_plot->graph(0)->setData(dataX, dataY);
			ui.Amode_4_plot->replot();
			break;
		default:
			break;
		}
	}
}

void QCplot::showDisGestureDataImage(const int& EMGChannel, const int& IMUChannel)
{
	
	int EMGIndex = EMGChannel - 1;
	int IMUIndex = IMUChannel - 1;
	// EMG Plot
	auto EMGPtr = &(EMG_Thread->emgBuffer->deArray[EMGIndex]);
	//qDebug() << "1: " << *(EMGPtr->end() - 1) << endl;
	vector<double> EMGData(EMGPtr->begin(), EMGPtr->end());
	//qDebug() << "2: " << *(EMGData.end() - 1) << endl;
	EMG_Data_x = EMG_Thread->emgBuffer->Data_x;
	EMG_Data_y = QVector<double>::fromStdVector(EMGData);
	//qDebug() << "3: " << *(EMG_Data_y.end() - 1) << endl;
	ui.disGesture_EMG_plot->graph(0)->setData(EMG_Data_x, EMG_Data_y);
	ui.disGesture_EMG_plot->replot();
	// IMU Plot
	auto IMU_Roll_Ptr = &(EMG_Thread->imuBuffer->deArray[IMUIndex][0]);
	auto IMU_Pitch_Ptr = &(EMG_Thread->imuBuffer->deArray[IMUIndex][1]);
	auto IMU_Yaw_Ptr = &(EMG_Thread->imuBuffer->deArray[IMUIndex][2]);
	vector<double> IMURollData(IMU_Roll_Ptr->begin(), IMU_Roll_Ptr->end());
	vector<double> IMUPitchData(IMU_Pitch_Ptr->begin(), IMU_Pitch_Ptr->end());
	vector<double> IMUYawData(IMU_Yaw_Ptr->begin(), IMU_Yaw_Ptr->end());
	IMU_Data_x = EMG_Thread->imuBuffer->Data_x;
	IMU_Roll_Data_y = QVector<double>::fromStdVector(IMURollData);
	IMU_Pitch_Data_y = QVector<double>::fromStdVector(IMUPitchData);
	IMU_Yaw_Data_y = QVector<double>::fromStdVector(IMUYawData);
	ui.disGesture_IMU_plot->graph(0)->setData(IMU_Data_x, IMU_Roll_Data_y);
	ui.disGesture_IMU_plot->graph(1)->setData(IMU_Data_x, IMU_Pitch_Data_y);
	ui.disGesture_IMU_plot->graph(2)->setData(IMU_Data_x, IMU_Yaw_Data_y);
	ui.disGesture_IMU_plot->replot();
}
void QCplot::showDisGestureAmodeImage(const int& AmodeChannel)
{
	int AmodeIndex = AmodeChannel - 1;
	vector<vector<double>> currentPackage;
	if (wifi_Thread->copydata_wifi(currentPackage))
	{
		vector<double> AmodeData(currentPackage[AmodeIndex].begin() + 2, currentPackage[AmodeIndex].end() - 1);
		int channelNum = currentPackage[AmodeIndex][1];
		if (channelNum != AmodeChannel)
			qDebug() << "wrong" << endl;
		Amode_Data_y = QVector<double>::fromStdVector(AmodeData);
		ui.disGesture_Amode_plot->graph(0)->setData(Amode_Data_x, Amode_Data_y);
		ui.disGesture_Amode_plot->replot();
	}
}

//~ 将A超信号实时刷新显示到控件上
void QCplot::showimage1(std::vector<std::vector<double>> CurrentPackage)
{
	//int chnum = 8;
	int chnum = 4;
	int sample_length = 1000;
	int channelnum = 0;
	//for (int i= 0; i<chnum; i+=2 )

	for (int i= 0; i<chnum; i++ )
	{
		std::vector<double> Temp(CurrentPackage[i].begin()+2, CurrentPackage[i].end()-1);		// 取A超数据，不包括包头、包尾和通道号
		Data_y = QVector<double>::fromStdVector(Temp);
		//for (int j= 0; j<sample_length - 3; j++)
		//{
		//	Data_y[j] = CurrentPackage[i][j + 2];
		//}
		channelnum = CurrentPackage[i][1];
		// plot //~ 这里只显示4个通道
		if(channelnum==1)
		{
			ui.customPlot->graph(0)->setData(Data_x, Data_y);
			ui.customPlot->replot();
		}
		else if(channelnum==2)
		{
			ui.customPlot1->graph(0)->setData(Data_x, Data_y);
			ui.customPlot1->replot();
		}
		else if(channelnum==3)
		{
			ui.customPlot2->graph(0)->setData(Data_x, Data_y);
			ui.customPlot2->replot();
		}
		else if(channelnum==4)
		{
			ui.customPlot3->graph(0)->setData(Data_x, Data_y);
			ui.customPlot3->replot();
		}

		//~ mode1
		if (channelnum == channelindex_mode1)
		{
			ui.customPlot_mode1->graph(0)->setData(Data_x, Data_y);
			ui.customPlot_mode1->replot();
		}

		//~ mode2
		if (channelnum == channelindex_mode2)
		{
			ui.customPlot_mode2->graph(0)->setData(Data_x, Data_y);
			ui.customPlot_mode2->replot();
		}
	}
}

//训练和预测时保存数据
void QCplot::saveimage(Mat Adata)
{
	if (save_flag)
	{
		int chnum = Adata.rows;
		int sample_length = Adata.cols;
		for (int i= 0; i<chnum; i++ )
		{
			uchar temp[1000] = {0}; 
			for (int j= 0; j<sample_length; j++)
			{
				temp[j] = Adata.at<uchar>(i,j);
			}
			fwrite(temp,sample_length,1,stream[i]);
		}
	}
}

//original code
void QCplot::getTrainFeature(QImage Aimg)
{
	//特征提取
	Mat matImage = Mat(Aimg.height(),Aimg.width(),CV_8UC1,(uchar*)Aimg.constBits(),Aimg.bytesPerLine());
	matImage.convertTo(matImage, CV_32FC1);
	showimage1(matImage);
	rawData = matImage;   
	//if(rawData.rows == 8)
	//可将后面的rawData直接替换成matImage
	//if((rawData.rows == 8)&&(rawData.at<float>(0,1) == 1))
	if ((rawData.rows == 4) && (rawData.at<float>(0, 1) == 1))
	{
			rawData.convertTo(rawData, CV_32FC1);
			//std::cout<< rawData.col(1);
			//std::cout<< sizeof(rawData.at<uchar>(1,1));
			Mat imageFeature;
			getCoefficientImageFeature(rawData,currentParametersSettings,imageFeature);
			//train_feature 为最终的特征矩阵
			//push_back 在原Mat中添加行
			train_feature.push_back(imageFeature);
			//贴标签
			float labelValue;
			labelValue = ((train_feature.rows-1)%(frameRate*holdTime*classNum))/(frameRate*holdTime);
			Mat label;
			label = Mat(1,1,CV_32FC1,labelValue);
			train_label.push_back(label);
			int step = train_feature.rows%(frameRate*holdTime*classNum);
			ui.horizontalSlider->setValue(step);
			if(train_feature.rows>=frameRate*holdTime*trialNum*classNum)
			{
				myPlayer->threadFree();
				trainModel();
				ui.train->setEnabled(true);
			}
	}


}

void QCplot::getTrainFeature1(QImage Aimg)
{

	Mat matImage = Mat(Aimg.height(),Aimg.width(),CV_8UC1,(uchar*)Aimg.constBits(),Aimg.bytesPerLine());
	//存储uchar型的数据
	Mat save_image = matImage;
	matImage.convertTo(matImage, CV_32FC1);
	showimage1(matImage);
	rawData = matImage;   
	//if((rawData.rows == 8)&&(rawData.at<float>(0,1) == 1))
	if ((rawData.rows == 4) && (rawData.at<float>(0, 1) == 1))
	{
		Mat imageFeature;

		if((packetNumber%(frameRate*holdTime*(classNum + 2)) < frameRate*holdTime*classNum))
		{

			//保存训练数据
			saveimage(save_image);
			//存储数据手套数据
			glo->getGloveData();
			//计算存储时间
			clock_t start0, end0;
			start0 = clock();
			getCoefficientImageFeature(rawData,currentParametersSettings,imageFeature);
			end0 = clock();
			double totaltime = (double)(end0-start0)/CLOCKS_PER_SEC;
			//std::cout<< totaltime <<std::endl;
			int step = packetNumber%(frameRate*holdTime*(classNum + 2));
			ui.horizontalSlider->setValue(step);
			//开始和结尾各去除1s
			if ((packetNumber%(frameRate*holdTime)> frameRate)&&(packetNumber%(frameRate*holdTime)<= frameRate*(holdTime - 1)))
			{
				train_feature.push_back(imageFeature);
				float labelValue;
				labelValue = ((train_feature.rows-1)%(frameRate*(holdTime- 2)*classNum))/(frameRate*(holdTime - 2));
				Mat label;
				label = Mat(1,1,CV_32FC1,labelValue);
				train_label.push_back(label);
			}
		}
		else 
		{
			//摘自预测
			String filename = "rest.jpg";
			Mat currentImage = imread(filename, CV_LOAD_IMAGE_COLOR);
			Mat currentQImage;
			cvtColor(currentImage,currentQImage,CV_BGR2RGB);
			QImage img = QImage((const unsigned char*)(currentQImage.data), currentQImage.cols,
				currentQImage.rows,currentQImage.step, QImage::Format_RGB888);
			QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
			ui.predictGesture->setScaledContents(true);
			ui.predictGesture->resize(ui.imageWidget->size());
			ui.predictGesture->setAutoFillBackground(false);
			ui.predictGesture->setStyleSheet(QStringLiteral("Background-color: #000;"));
			ui.predictGesture->setPixmap(QPixmap::fromImage(img));
			ui.predictGesture->show();
			if (packetNumber%(frameRate*holdTime*(classNum + 2)) > 0.9 * frameRate*holdTime*(classNum + 2))
			{
				ui.predictGesture->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 255, 0);");	 
			}
			if(packetNumber%(frameRate*holdTime*(classNum+2)) == (frameRate*holdTime*(classNum+2) - 1))
			{
		
				ui.predictGesture->clear();
				ui.predictGesture->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(0, 0, 0);");	

			}

		}
		packetNumber++;
		if(packetNumber>=frameRate*holdTime*trialNum*(classNum+2))
		{

			trainModel();
			ui.train->setEnabled(true);
			//停止存储
			save_flag = false;
			fclose(stream[0]);
			fclose(stream[1]);
			fclose(stream[2]);
			fclose(stream[3]);
			fclose(stream[4]);
			fclose(stream[5]);
			fclose(stream[6]);
			fclose(stream[7]);
			myPlayer->threadFree();
		}
	}


}

void QCplot::trainModel()
{	
	columnNormalize(train_feature, trainFeatureNormalize,maxCol,minCol,averageCol);
	CvSVMParams params;
	CvTermCriteria criteria;
	params.svm_type    = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 1000, FLT_EPSILON);					//1000是干啥的来着
	//注释掉PCA
	int number_principal_compent = 35;
	pca = new PCA(trainFeatureNormalize,Mat(),CV_PCA_DATA_AS_ROW, number_principal_compent);	// 每一行代表一个样本
	Mat projectedTrain = pca->project(trainFeatureNormalize);									// 投影到PCA的主成分空间中
	projectedTrain.convertTo(projectedTrain,CV_32FC1);
	//SVM
	/*
	SVMmodel.train_auto(projectedTrain,train_label,Mat(),Mat(),params);
	SVMmodel.save(".\\Mat_file\\SVM_model.xml");
	writeMatToFile(train_label, (".\\Mat_file\\" + currentParametersSettings.featureType.toStdString()+"train_label.txt").c_str());
	writeMatToFile(trainFeatureNormalize,(".\\Mat_file\\" + currentParametersSettings.featureType.toStdString()+"train_data.txt").c_str());
	*/
	//LDA
	lda->LDA_Bayesian::Bayestrain(projectedTrain,train_label);
	//LDA和SVM同时预测
	SVMmodel.train_auto(projectedTrain,train_label,Mat(),Mat(),params);
}
//~ 将mat数据存储为.bin文件，腕手同步识别没有用到
void QCplot::writeMatToFile(cv::Mat& matData, const char* filename)
{
	std::ofstream fout(filename,std::ios_base::out|std::ios_base::app);
	if(!fout)
	{
		std::cout<<"File Not Opened"<<endl;  
		return;
	}

	for(int i=0; i<matData.rows; i++)
	{
		for(int j=0; j<matData.cols; j++)
		{
			float signalData = matData.at<float>(i,j);	
			fout<<signalData<<'\t';	
		}
		fout<<std::endl;
	}
	fout.clear();
	fout.close();

}
//~ 数据归一化处理
void QCplot::columnNormalize(Mat& src, Mat& des,Mat& maxCol,Mat& minCol,Mat& averageCol)
{
	Mat maxMat,minMat,averageMat;
	for (int i = 0;i<src.cols;i++)
	{
		float sum=0;
		float maxValue = -std::numeric_limits<float>::max(),minValue=std::numeric_limits<float>::max();  //返回编译器允许的float类型的最大值
		//float maxValue = -1000, minValue = 1000;
		for (int j=0;j<src.rows;j++)
		{
			float tempVal = src.at<float>(j,i); //返回float类型

			if (tempVal>maxValue)
			{
				maxValue = tempVal;				//得到最大值
			}
			if (tempVal<minValue)
			{
				minValue = tempVal;				//得到最小值
			}
			sum += tempVal;						//得到对应列的行和
		}
		//cout<<"sucess"<<endl;
		float average = (float)(sum/src.rows);	//求出对应列的行平均值
		maxCol.push_back(maxValue);				//存入对应列的最大值
		minCol.push_back(minValue);				//存入对应列的最小值
		averageCol.push_back(average);			//存入对应列的平均值
	}
	repeat(maxCol.t(),src.rows,1,maxMat);		//.t（）转置
	repeat(minCol.t(),src.rows,1,minMat);
	repeat(averageCol.t(),src.rows,1,averageMat);
	//des = (src-averageMat)/(maxMat-minMat);		//归一化
	des = (src-minMat)/(maxMat-minMat);				//减去最小值
	//float test = des.at<float>(0,0);				
}


void QCplot::getPredictFeature(QImage Aimg)
{
	Mat matImage = Mat(Aimg.height(),Aimg.width(),CV_8UC1,(uchar*)Aimg.constBits(),Aimg.bytesPerLine());
	matImage.convertTo(matImage, CV_32FC1);
	showimage1(matImage);
	rawData = matImage;  
	//if( rawData.rows == 8)
	//if((rawData.rows == 8)&&(rawData.at<float>(0,1) == 1))
	if ((rawData.rows == 4) && (rawData.at<float>(0, 1) == 1))
	{
		rawData.convertTo(rawData, CV_32FC1);
		Mat imageFeature;
		getCoefficientImageFeature(rawData,currentParametersSettings,imageFeature);
		//predictFeatureNormalize = (imageFeature-averageCol.t())/(maxCol.t()-minCol.t());
		predictFeatureNormalize = (imageFeature-minCol.t())/(maxCol.t()-minCol.t());
		//投影到PCA主成分空间
		Mat projectedpredict = pca->project(predictFeatureNormalize);
		projectedpredict.convertTo(projectedpredict,CV_32FC1);
		//预测标签SVM
		//float predictLabel = SVMmodel.predict(projectedPredict);
		//预测标签LDA
		clock_t start0, end0, start1, end1;
		start0 = clock();
		float predictLabel = lda->LDA_Bayesian::Bayespredict(projectedpredict);
		end0 = clock();
		double totaltime = (double)(end0-start0)/CLOCKS_PER_SEC;
		//std::cout<< "LDA:"<<totaltime <<std::endl;
		start1 = clock();
		float predictLabel_svm  = SVMmodel.predict(projectedpredict);
		end1 = clock();
		totaltime = (double)(end1-start1)/CLOCKS_PER_SEC;
		//std::cout<< "SVM:"<<totaltime <<std::endl;
		//预测标签结束
		if(predict_label.rows%(frameRate*holdTime) == 0)
		{
			//predictTrail 指示当前动作，换成predictAction更好
			if (predictTrial == 0)
			{
				imageShowLabel[predictTrialOrderNew[predictTrial++]]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(255, 0, 0);");
			}else
			{
				 //YANG 去掉之前的框选（红）
				imageShowLabel[predictTrialOrderNew[predictTrial-1]]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				//YANG 框选当前动作（红）
				imageShowLabel[predictTrialOrderNew[predictTrial++]]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(255, 0, 0);"); 
			}

		}

		if(predict_label.rows%(frameRate*holdTime) >= frameRate*holdTime*0.8)
		{
			//动作标号小于6
			if(predictTrial<predictTrialOrderNew.size())
			{
				//YANG 提示下一个动作（绿）
				imageShowLabel[predictTrialOrderNew[predictTrial]]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 255, 0);");	 

			}	
		}
		//在HoldTime内，反复识别当前动作
		float predictRealLabel =predictTrialOrderNew[predictTrial-1];
		predict_feature.push_back(predictFeatureNormalize);
		predict_label.push_back(predictLabel);
		//SVM 和 LDA 同时预测
		predict_label_svm.push_back(predictLabel_svm);
		predict_real_label.push_back(predictRealLabel);
		//插入控制策略
		gestureOrder = true;
		if(gestureQueue.size()<3)
		{
			gestureQueue.push(predictLabel);
			gestureOrder = false;
		}
		else
		{
			int sameLabel = gestureQueue.front();
			int temp;
			//judge whether all the data in the queue are identical
			for(int i=0;i<gestureQueue.size();i++)
			{
				temp = gestureQueue.front();
				gestureQueue.pop();
				gestureQueue.push(temp);
				if(temp!=sameLabel)
				{
					gestureOrder = false;
				}
			}
			gestureQueue.push(predictLabel);
			gestureQueue.pop();
		}
		if(gestureOrder){
			unsigned char temp_label = gestureMap[gesturePath[gestureQueue.front()]];
			mCOMPort->WriteData(&temp_label, 1);
			//unity虚拟手控制
			int motion_num = gestureQueue.front();
			char motion_str[3];
			itoa(motion_num, motion_str, 10);
			char motion_name[9] = "Gesture_";
			strcat(motion_name, motion_str);
			std::cout << "sending: " << motion_name << std::endl;

		}
		//ending
		Mat currentImage = imread(gestureName[predictLabel]);
		Mat currentQImage;
		cvtColor(currentImage,currentQImage,CV_BGR2RGB);
		QImage img = QImage((const unsigned char*)(currentQImage.data), currentQImage.cols,
			currentQImage.rows,currentQImage.step, QImage::Format_RGB888);
		QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
		ui.predictGesture->setScaledContents(true);
		ui.predictGesture->resize(ui.imageWidget->size());
		ui.predictGesture->setAutoFillBackground(false);
		ui.predictGesture->setStyleSheet(QStringLiteral("Background-color: #000;"));
		ui.predictGesture->setPixmap(QPixmap::fromImage(img));
		ui.predictGesture->show();

		if(predict_feature.rows>=frameRate*holdTime*(2*classNum - 1))
		{	//去掉之前的提示框
			imageShowLabel[predictTrialOrderNew[predictTrial-1]]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);"); 
			myPlayer->threadFree();
			savePredict();
			ui.predict->setEnabled(true);
		}
	}
}
//~ 在腕手同步实验中没有用到，这部分是在离散手势识别中存储数据用
void QCplot::savePredict()
{
	writeMatToFile(predict_label,(".\\Mat_file\\" + currentParametersSettings.featureType.toStdString()+"predict_label.txt").c_str());
	writeMatToFile(predict_real_label,(".\\Mat_file\\" + currentParametersSettings.featureType.toStdString()+"predict_real_label.txt").c_str());
	//SVM和LDA同时预测
	writeMatToFile(predict_label_svm,(".\\Mat_file\\" + currentParametersSettings.featureType.toStdString()+"predict_label_svm.txt").c_str());
}

void QCplot::showTrainImage()
{
	//读取本地工程目录下的文件
	QDir dir(image_dir);
	
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Name | QDir::NoSort);
	QFileInfoList list = dir.entryInfoList();
	classNum = list.size() - 1;	                                        //classNum = 7??
	for (int i = 0; i < (list.size()-1); i++)
	{
		trainTrialOrder.push_back(i);
		QFileInfo fileInfo = list.at(i);
		String filename;
		filename.append(image_dir.toLatin1().data());
		filename.append("/");
		filename.append(fileInfo.fileName().toLatin1().data());		//由QString转到String
		gestureName.push_back(filename);
		//US_SHOW
		gesturePath.push_back(fileInfo.fileName().toLatin1().data());
		Mat currentImage = imread(filename, CV_LOAD_IMAGE_COLOR);
		Mat currentQImage;
		cvtColor(currentImage,currentQImage,CV_BGR2RGB);
		QImage img = QImage((const unsigned char*)(currentQImage.data), currentQImage.cols,
			currentQImage.rows,currentQImage.step, QImage::Format_RGB888); 

		QLabel *labelTrainImage = new QLabel(ui.centralWidget);
		QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
		ui.imageIn_2->addWidget(labelTrainImage);
		labelTrainImage->setScaledContents(true);
		labelTrainImage->resize(ui.imageWidget->size());
		labelTrainImage->setAutoFillBackground(false);
		labelTrainImage->setStyleSheet(QStringLiteral("Background-color: #000;"));
		labelTrainImage->setPixmap(QPixmap::fromImage(img));
		labelTrainImage->show();
		imageShowLabel.push_back(labelTrainImage);
	}
}

void QCplot::calAccuracy()
{
	int label_length = predict_label.rows >= train_label.rows ? train_label.rows:predict_label.rows;
	float sumNum = 0;
	for (int i = 0; i<label_length; i++)
	{
		if (predict_label.at<float>(i,0) == predict_real_label.at<float>(i,0))
		{
			sumNum += 1.0;
		}
	}
	predictAccurancy = 1.0*sumNum/label_length;
	//printf("%f ", predictAccurancy);
}

//void QCplot::on_actionParameter_triggered()
//{
//	if (paraDialog->exec() == QDialog::Accepted)
//	{
//		paraDialog->updateSettings();
//		para::featureSettings p = paraDialog->getfeatureSettings();
//		currentParametersSettings.windowLength =  p.windowLength.toInt();
//		currentParametersSettings.channelNumber =  p.channelNumber.toInt();
//		currentParametersSettings.overlap =  p.overlap.toInt();
//		currentParametersSettings.featureType =  p.featureType;
//		image_dir = p.imageDir;
//		trialNum =  p.trialNumber.toInt();
//	}
//}
//~ 设置customPlot_angle控件的一些参数
void QCplot::setupRealTimeAngle()
{
	ui.customPlot_angle->legend->setVisible(true);
	ui.customPlot_angle->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
	ui.customPlot_angle->legend->setFont(QFont("Helvetica",8));
	ui.customPlot_angle->addGraph();
	QPen pen;
	//Hand
	pen.setColor(QColor(0, 0, 0, 255));
	ui.customPlot_angle->graph(0)->setPen(pen);
	ui.customPlot_angle->graph(0)->setName("Hand");
	//PC1预测腕部数据
	ui.customPlot_angle->addGraph();
	pen.setColor(QColor(0, 0, 0, 255));
	ui.customPlot_angle->graph(1)->setPen(pen);
	ui.customPlot_angle->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 8));
	ui.customPlot_angle->graph(1)->setName("Wrist");
	ui.customPlot_angle->xAxis->setLabel("Time/s");
	ui.customPlot_angle->xAxis->setLabelFont(QFont("Helvetica",8));
	ui.customPlot_angle->yAxis->setLabel("Hand (deg)    	 Wrist(deg)");
	ui.customPlot_angle->yAxis->setLabelFont(QFont("Helvetica",8));
	QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
	timeTicker->setTimeFormat("%m:%s");
	ui.customPlot_angle->xAxis->setTicker(timeTicker);
	ui.customPlot_angle->xAxis->setTickLabelFont(QFont("Helvetica",8));
	ui.customPlot_angle->axisRect()->setupFullAxesBox();
	ui.customPlot_angle->xAxis->setRange(0,8);
	QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
	textTicker->addTick(0, "0");
	textTicker->addTick(0.5, "45");
	textTicker->addTick(1, "90");
	textTicker->addTick(1.1, "-80");
	textTicker->addTick(1.6, "0");
	textTicker->addTick(2.1, "80");
	ui.customPlot_angle->yAxis->setTicker(textTicker);
	ui.customPlot_angle->yAxis->setTickLabelFont(QFont("Helvetica", 6));
	ui.customPlot_angle->yAxis->setRange(0, 2.1); 
}

//~ 实时显示数据手套的手部张合角度
void QCplot::showRealTimeAngle(float handangle, float wristangle)
{
	//key的单位是秒
	static QTime timePlot(QTime::currentTime());
	double key = timePlot.elapsed()/1000.00;
	if (!predict_angle_plot)
	{//~ 非预测期间，显示dataglove的真实角度，graph(0)表示手部关节角，graph(1)表示腕部关节角
		ui.customPlot_angle->graph(1)->data()->removeBefore(key-6);
		ui.customPlot_angle->graph(1)->addData(key, handangle);
		ui.customPlot_angle->graph(1)->rescaleValueAxis(true);
		//ui.customPlot_angle->graph(1)->data()->clear();
		if (xsens_is_ready)
		{//~ 这里的wristangle由xsens测得
			ui.customPlot_angle->graph(0)->data()->removeBefore(key - 6);
			ui.customPlot_angle->graph(0)->addData(key, wristangle / 360 + 1.6);
			ui.customPlot_angle->graph(0)->rescaleValueAxis(true);
		}
		else
		{
			ui.customPlot_angle->graph(0)->data()->removeBefore(key - 6);
			ui.customPlot_angle->graph(0)->addData(key, 0 / 360 + 1.6);
			ui.customPlot_angle->graph(0)->rescaleValueAxis(true);
		}
		
	}
	else
	{//~ 预测期间，显示预测角度
		//时间重置为零
		if (updateTime)
		{
			timePlot.restart();
			updateTime = false;
			ui.customPlot_angle->graph(1)->data()->clear();
			ui.customPlot_angle->graph(0)->data()->clear();
		}
		ui.customPlot_angle->graph(1)->data()->removeBefore(key-5);
		ui.customPlot_angle->graph(0)->data()->removeBefore(key-5);
		//ui.customPlot_angle->graph(0)->data().data()->clear();
		//ui.customPlot_angle->graph(1)->data().data()->clear();
		ui.customPlot_angle->graph(1)->addData(key, predict_Handangle);
		ui.customPlot_angle->graph(1)->rescaleValueAxis(true);
		ui.customPlot_angle->graph(0)->addData(key, predict_Wristangle + 1.1);
		//ui.customPlot_angle->graph(1)->addData(key, filteredPC1+1.1);
		ui.customPlot_angle->graph(0)->rescaleValueAxis(true);
	}
	ui.customPlot_angle->xAxis->setRange(key, 8, Qt::AlignRight);
	ui.customPlot_angle->replot();
}

//~ 保存数据按钮
void QCplot::on_save_ectype_clicked()
{
	if (!flag)
	{
		ui.save_ectype->setText("stop");
		gettime();
		flag= true;
		stream[0]=fopen(fileName[0],"ab+");
		stream[1]=fopen(fileName[1],"ab+");
		stream[2]=fopen(fileName[2],"ab+");
		stream[3]=fopen(fileName[3],"ab+");
		stream[4]=fopen(fileName[4],"ab+");
		stream[5]=fopen(fileName[5],"ab+");
		stream[6]=fopen(fileName[6],"ab+");
		stream[7]=fopen(fileName[7],"ab+");
		outfile.open(savedata);
	}
	else
	{
		ui.save_ectype->setText("save");
		flag= false;
		fclose(stream[0]);
		fclose(stream[1]);
		fclose(stream[2]);
		fclose(stream[3]);
		fclose(stream[4]);
		fclose(stream[5]);
		fclose(stream[6]);
		fclose(stream[7]);
		outfile.close();
	}

}
//~ 开始按钮
void QCplot::on_start_ectype_clicked()
{
	
	if (myPlayer->isStop())
	{
		myPlayer->threadStart();
		ui.start_ectype->setText("Stop");
		ui.train_ectype->setEnabled(true);
	}

	else
	{
		myPlayer->setStop();
		ui.start_ectype->setText("Start");
		ui.train_ectype->setEnabled(false);
		ui.predict_ectype->setEnabled(false);
	}

}
//~ 训练按钮
void QCplot::on_train_ectype_clicked()
{
	ui.train_ectype->setEnabled(false);
	train_feature.release();				
	train_label.release();
	packetNumber = 0;
	gettime();
	//~ 创建文件，用于存储训练数据
	for (int i=0;i<8;i++)
	{
		stream[i]=fopen(fileName[i],"ab+");
	}

	//~ 训练时候记录手指位置,dataglove测得的真值
	if (glo->datagloveused)
	{
		outfile_dataglove.open(save_dataglove);
	}

	save_flag = true;

	
	//~ 如果xsens连接成功，则创建文件，用于采集数据
	if (xsens_is_ready)
	{
		////~ 创建xsens文件，不开线程，在getTrainFeatureC()中采集数据，保证和A超的采集存储频率相同
		gettime();
		//outfile_xsens.open(savedata_xsens);//~ 创建存储xsens数据的txt文档
		//outfile_xsens << "roll" << "	" << "pitch" << "	" << "yaw" << std::endl;
		xsens_data = fopen(save_xsens, "w");
		if (xsens_data == NULL)
		{
			printf("Can't open xsens data files.\n");
		}
		else
		{
			//Create headers for CSV files
			fprintf(xsens_data, "xsens1, , ,xsens2, , ,xsens3\n");
			fprintf(xsens_data, "roll,pitch,yaw,roll,pitch,yaw,roll,pitch,yaw\n");
		}


		//~ 创建并打开xsens文件，单独开一个线程，存储完整的xsens数据
		gettime();
		//outfile_xsens_all.open(savedata_xsens_all);//~ 创建存储xsens数据的txt文档
		//outfile_xsens_all << "roll" << "	" << "pitch" << "	" << "yaw" << std::endl;
		xsens_data_all = fopen(save_xsens_all, "w");
		if (xsens_data_all == NULL)
		{
			printf("Can't open xsens data files.\n");
		}
		else
		{
			//Create headers for CSV files
			fprintf(xsens_data_all, "xsens1, , ,xsens2, , ,xsens3\n");
			fprintf(xsens_data_all, "roll,pitch,yaw,roll,pitch,yaw,roll,pitch,yaw\n");
		}

		xsensthread_flag = true;
		xsensthread->start();
	}

	
	if (Delsys_On_flag)
	{//~ 调用delsys的c++ sdk，直接在上位机的一个线程内采集数据
		delsys_sdk_thread->create_delsys_file();
		delsys_sdk_thread->start_save();
	}


	myPlayer->threadTrain(); 
}
//~ 训练对应的线程
void QCplot::getTrainFeatureC()
{
	Mat rawData(8, 1000, CV_32FC1);
	std::vector<std::vector<double>> CurrentPackage;
	if (p->copydata1(CurrentPackage) == true || wifi_Thread->copydata_wifi(CurrentPackage) == true)//~ 读取A超设备传的数据
	{
		showimage1(CurrentPackage);//~ 显示当前信号到控件
		for (int i = 0; i<CurrentPackage.size();i++)
		{
			for (int j = 0; j<1000; j++)
			{
				rawData.at<float>(i, j) = float(CurrentPackage[i][j]);
			}
		}
		if((rawData.rows == 8)&&(rawData.at<float>(0,1) == 1))
		{
			//~ 训练过程中，保存A超原始数据
			Mat saveData;
			rawData.convertTo(saveData, CV_8UC1);
			saveimage(saveData);

			//~ 读取xsens的数据
			if (xsens_is_ready)
			{
				read_xsens_data();
			}

			if (glo->datagloveused || xsens_is_ready)
			{
				float labelValue = fdGetSensorScaled(glo->pGlove, FD_MIDDLENEAR);//~ 获取数据手套当前的手部张合角度值（0~90），这里取的是手套背部中间的一条传感器的数值
				outfile_dataglove << labelValue << std::endl;//~ 存储数据手套的数据
				showRealTimeAngle(labelValue, xsens_angle);//~ 显示数据手套的值到控件
			}
			

			//Mat label;
			//label = Mat(1,1,CV_32FC1,labelValue);
			//train_label.push_back(label);//~ train_label用于存放真实的手部张合角度值（dataglove测得），一列值
			
			if (engine_is_open)
			{
				//~ 调用MATLAB引擎，对rawData（8*1000）进行滤波和特征提取等操作，输出imageFeature（1*feature_col_num）
				Mat imageFeature(1, feature_col_num, CV_64FC1);
				double temp_imageFeature[1][feature_col_num];//~ 暂存imageFeature的数据
				double mat[8][1000];//~ 暂存rawData数据
				Mat temp_mat(8, 1000, CV_64FC1);
				rawData.convertTo(temp_mat, CV_64FC1);
				memcpy(mat, temp_mat.data, 8 * 1000 * sizeof(double));//~ 将cv::Mat类型转成double类型数组
				SetMat("rawData", (void*)mat, 8, 1000, ep);//~ 将c++中的mat矩阵写入到MATLAB工作区中的mat_matlab矩阵
				engEvalString(ep, "imageFeature=getCoefficientImageFeature(rawData)");//~ 提取A超信号特征参数到imageFeature，有48（子窗数）*1（特征参数个数）*8（通道数）=384个特征参数。是1*384的矩阵
				engEvalString(ep, "imageFeature = imageFeature'");
				GetMat("imageFeature", (void*)temp_imageFeature, 1, feature_col_num, ep);//~ 将MATLAB工作区中的mat_matlab矩阵赋值到c++中的mat矩阵，覆盖掉原先mat矩阵的值
				memcpy(imageFeature.data, temp_imageFeature, 1 * feature_col_num * sizeof(double));//~ 将double类型的mat矩阵数组转成cv::Mat类型，存储到Mat A(M, M, CV_64FC1)中
				imageFeature.convertTo(imageFeature, CV_32FC1);//~ 将64位存储类型的A矩阵转成32位（float）类型
				train_feature.push_back(imageFeature);//~ push_back是按行加在mat矩阵最后一行
			}
			
			
			packetNumber++;//~ 每一帧packetNumber就加一次

			//~ 用progressbar控件显示当前训练的进度
			ui.train_progressBar->setValue(packetNumber+1);
				

			int packetNumPerTrial = packetNumber % (4 * eachMotion * frameRate);//~ packetNumPerTrial用于判断当前所处的训练阶段，指示受试者进行对应的动作，总共有三个阶段
			if ((packetNumPerTrial>0)&&(packetNumPerTrial<=eachMotion*frameRate))
			{
				if (packetNumPerTrial==1)
				{
					//~ 红框进行提示
					ui.hand_close_gif->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(255, 0, 0);");
					ui.motion_prompt->setText("Hand Closing");
					//~ 显示动图
					movie_hand = new QMovie("Resources/HandClosing.gif");
					ui.hand_close_gif->setMovie(movie_hand);
					movie_hand->start();

				}
				
				//if (eachMotion * frameRate - packetNumPerTrial < 20)
				//{//~ 每一阶段的最后两秒开始提示下一阶段的信息
				//	ui.wrist_rotation_gif->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(255, 128, 0);");
				//}
				if (packetNumPerTrial == eachMotion * frameRate - 1)
				{
					//~ 提示休息10s
					ui.motion_prompt->setText("Rest 10s");					
				}
				else if (packetNumPerTrial == eachMotion * frameRate)
				{
					_sleep(10000);
				}

			}
			else if ((packetNumPerTrial>eachMotion*frameRate)&&(packetNumPerTrial<=2*eachMotion*frameRate))
			{
				if (packetNumPerTrial == eachMotion * frameRate+1)
				{
					//~ 去掉上一阶段的提示框和gif动图播放，更换当前阶段的提示框
					movie_hand->stop();
					ui.hand_close_gif->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
					ui.wrist_rotation_gif->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(255, 0, 0);");

					ui.motion_prompt->setText("Wrist Rotation");

					//~ 显示动图
					movie_wrist = new QMovie("Resources/WristRotation.gif");
					ui.wrist_rotation_gif->setMovie(movie_wrist);
					movie_wrist->start();

				}
				
				//if (2 * eachMotion * frameRate - packetNumPerTrial < 20)
				//{//~ 每一阶段的最后两秒开始提示下一阶段的信息
				//	ui.simultaneous_gif->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(255, 128, 0);");
				//}
				if (packetNumPerTrial == 2*eachMotion * frameRate - 1)
				{
					//~ 提示休息10s
					ui.motion_prompt->setText("Rest 10s");
				}
				else if (packetNumPerTrial == 2*eachMotion * frameRate)
				{
					_sleep(10000);
				}

			}
			else if ((packetNumPerTrial>2*eachMotion*frameRate)&&(packetNumPerTrial<=3*eachMotion*frameRate))
			{
				if (packetNumPerTrial == 2 * eachMotion * frameRate+1)
				{
					//~ 去掉上一阶段的提示框和gif动图播放，更换当前阶段的提示框
					movie_wrist->stop();
					ui.wrist_rotation_gif->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
					ui.simultaneous_gif->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(255, 0, 0);");

					ui.motion_prompt->setText("Simultaneous");

					//~ 显示动图
					movie_simultaneous = new QMovie("Resources/simultaneous.gif");
					ui.simultaneous_gif->setMovie(movie_simultaneous);
					movie_simultaneous->start();
				}

				if (packetNumPerTrial == 3*eachMotion * frameRate - 1)
				{
					//~ 提示休息10s
					ui.motion_prompt->setText("Rest 10s");
				}
				else if (packetNumPerTrial == 3*eachMotion * frameRate)
				{
					_sleep(10000);
				}

				
			}
			else if ((packetNumPerTrial > 3 * eachMotion * frameRate) && (packetNumPerTrial <= 4 * eachMotion * frameRate))
			{
				if (packetNumPerTrial == 3 * eachMotion * frameRate + 1)
				{
					//~ 去掉所有提示框信息和gif动图播放
					movie_simultaneous->stop();
					ui.simultaneous_gif->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
					
					ui.motion_prompt->setText("Unconstrained");

					
				}		
				if (packetNumPerTrial == 4 * eachMotion * frameRate - 1)
				{
					//~ 提示休息10s
					ui.motion_prompt->setText("Rest 10s");
				}
			

			}

			

			if (packetNumber == 4 * eachMotion * frameRate * session_SPC)
			{

				ui.motion_prompt->setText("End!");

				save_flag = false;
				for (int i = 0; i < 8; i++)
				{
					fclose(stream[i]);
				}

				if (glo->datagloveused)
				{//~ 停止数据手套的数据采集
					outfile_dataglove.close();//~ 关闭存储数据的文件。否则会导致内存溢出，MATLAB引擎调用也会失败
				}
						

				if (xsens_is_ready)
				{
					//~ 停止xsens_all采集数据的线程
					xsensthread_flag = false;
					//outfile_xsens_all.close();

					////~ 停止xsens数据采集
					//outfile_xsens.close();

					fclose(xsens_data);
					fclose(xsens_data_all);

				}

				if (Delsys_On_flag)
				{
					delsys_sdk_thread->stop_read_delsys();//~ 停止读取delsys数据的线程					
				}

				ui.train_ectype->setEnabled(true);
				ui.Train_Model->setEnabled(true);

				count_num = 0;
				
				myPlayer->threadFree();
			}
			else if (packetNumPerTrial == 0)//~ 用于判断每个session的末尾
			{
				//_sleep(10000);
				count_num++;
				packetNumber--;
				if (count_num == 100)
				{
					packetNumber++;
				}
			}
			
		}
	}
}
//~ 训练结束后，计算模型
void QCplot::trainModelC()
{	
	
	Mat train_feature_hc = train_feature.rowRange(switch_len, part_len - switch_len).clone();
	Mat train_feature_wr = train_feature.rowRange(part_len + switch_len, part_len * 2 - switch_len).clone();
	Mat train_feature_com = train_feature.rowRange(part_len * 2 + switch_len, part_len * 3 - switch_len).clone();

	train_feature_new.push_back(train_feature_hc);
	train_feature_new.push_back(train_feature_wr);
	train_feature_new.push_back(train_feature_com);


	//~ 注意，在Mat_file文件夹下面的.bin文件，只能有刚刚train过程产生的8个.bin文件，不能又其他.bin文件
	//~ 调用MATLAB计算引擎，读取工作区pc2new的数值。取代原先读取pc2new_label.txt文件的方法
	double mat_pc1new[feature_row_num][1];//~ 用于暂存pc1new_label的值
	double mat_pc2new[feature_row_num][1];//~ 用于暂存pc2new_label的值
	Mat temp_mat_pc1new(feature_row_num, 1, CV_64FC1);//~ 为了保证数据的一致性，只能和MATLAB中mxCreateDoubleMatrix都采用64位存储。最后转成32位
	Mat temp_mat_pc2new(feature_row_num, 1, CV_64FC1);

	static double mat_train_feature[train_feature_row_num][feature_col_num];
	Mat temp_mat_train_feature(train_feature_row_num, feature_col_num, CV_64FC1);
	train_feature.convertTo(temp_mat_train_feature, CV_64FC1);
	memcpy(mat_train_feature, temp_mat_train_feature.data, train_feature_row_num * feature_col_num * sizeof(double));//~ 将cv::Mat类型转成double类型数组
	SetMat("train_feature", (void*)mat_train_feature, train_feature_row_num, feature_col_num, ep);//~ 将c++中的train_feature矩阵写入到MATLAB工作区中

	engEvalString(ep, "[pc1new,pc2new] = pca_label()");//~ 调用MATLAB中的pc2new_label函数（自定义函数）
	GetMat("pc1new", (void*)mat_pc1new, feature_row_num, 1, ep);//~ 将MATLAB工作区中的mat_matlab矩阵赋值到c++中的mat矩阵，覆盖掉原先mat矩阵的值
	GetMat("pc2new", (void*)mat_pc2new, feature_row_num, 1, ep);

	memcpy(temp_mat_pc1new.data, mat_pc1new, feature_row_num * 1 * sizeof(double));
	temp_mat_pc1new.convertTo(temp_mat_pc1new, CV_32FC1);
	pc1new_label = temp_mat_pc1new.clone();

	memcpy(temp_mat_pc2new.data, mat_pc2new, feature_row_num * 1 * sizeof(double));//~ 将double类型的mat矩阵数组转成cv::Mat类型，存储到Mat A(M, M, CV_64FC1)中
	temp_mat_pc2new.convertTo(temp_mat_pc2new, CV_32FC1);//~ 将64位存储类型的A矩阵转成32位（float）类型
	pc2new_label = temp_mat_pc2new.clone();


	//pca = new PCA(train_feature,Mat(),CV_PCA_DATA_AS_ROW, number_principal_compent);	
	pca = new PCA(train_feature_new, Mat(), CV_PCA_DATA_AS_ROW, beta_row_num - 1);//~ 第三个参数表示按照行模式读取数据，第四个参数0.995是解释方差,也可以指定为多少个维数（ >1 ），参考：https://blog.csdn.net/guanyonglai/article/details/78418078?app_version=4.7.1
	//~ projectedTrain是A超特征参数集合train_feature投影到PCA的主成分空间（由train_feature经pca后张成的空间）中的数据集合，即经过PCA降维后的特征数据集合（也即一系列数据进行新的坐标变换）
	projectedTrain = pca->project(train_feature_new);
	//std::cout << "projectedTrain.cols" << projectedTrain.cols << std::endl;//~ 输出显示，经过PCA降维（投影）后，有多少个主成分被保留下来了
	projectedTrain.convertTo(projectedTrain, CV_32FC1);//~ 把降维后的数据集合，转换成mat中的灰度图形式

	//~ 注释于2021-05-30
	//Mat P1 = projectedTrain.col(0);//~ 降维后数据集（mat格式存储）的第一列，即第一主成分坐标轴上的所有数据点的投影数据集合，赋值到mat P1：               
	//MaxMinPC1Wrist(P1);//~ 找出PC1对应手腕的最大最小值赋值到mmaxMinPC1，具体哪个对应最大值并不一直确定

	//~ 后续用于输出角度值的归一化
	MaxMinWrist(pc1new_label);
	MaxMinHand(pc2new_label);


	//~ LR 线性回归模型，计算参数矩阵beta
	Mat onecolumn = Mat::ones(projectedTrain.rows, 1, CV_32FC1);
	Mat X;
	hconcat(projectedTrain, onecolumn, X);//~ 水平拼接，输出X
	Mat Y_pc1;
	Y_pc1 = pc1new_label;
	Mat Y_pc2;
	Y_pc2 = pc2new_label;
	//Y = train_label;//~ 无监督代码，不使用dataglove作为真值的label。
	//MaxMinHand(train_label);
	beta_pc1 = (X.t() * X).inv() * (X.t()) * Y_pc1;
	beta_pc2 = (X.t() * X).inv() * (X.t()) * Y_pc2;//~ .t是转置，inv是求逆矩阵


	////KRR //~ 注释于2021-05-17
	////lamd = 0.03, gamma = 0.002
	//Mat K = GaussianKernel(projectedTrain, projectedTrain, 0.002);
	////std::cout << " K：" << ' ' << K << std::endl;
	//Mat Eye = Mat::eye(K.rows, K.cols, CV_32FC1);
	//double lamd = 0.03;
	//beta = (K+lamd*Eye).inv()*train_label;
	////std::cout << " train_label：" << ' ' << train_label << std::endl;

	//CvSVMParams params;
	//CvTermCriteria criteria;
	//params.svm_type    = CvSVM::NU_SVR;
	//params.kernel_type = CvSVM::RBF;
	//params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 1000, FLT_EPSILON);	
	////params.nu = 0.2;
	//SVMmodel.train_auto(projectedTrain, train_label, Mat(), Mat(), params);

	
}

void QCplot::on_predict_ectype_clicked()
{
	if (predict_flag == false)
	{
		predict_flag = true;
		predict_gesture();
		ui.predict_ectype->setText("Stop_Predict");
	}

	else
	{
		myPlayer->threadStart();
		predict_flag = false;
		ui.predict_ectype->setText("Predict");
		outfile.close();
		predict_angle_plot = false;
		myPlayer->threadFree();
		
		
	}
}

//~ 测试阶段，随机指定下一个TAC手势
void QCplot::predict_gesture()
{
	predict_angle_plot = true;
	predict_feature.release();//~ 每次重新按下predict按钮，都会把之前一次predict过程中提取到的特征参数清除
	//线性回归预测LR
	predict_label.release();
	predict_real_label.release();
	updateTime = true;
	DwellingTime = 0;//~ 用于看手势在目标位置处的保持时间
	gettime();
	outfile.open(savedata);//~ savedata是char类型的数组，gettime函数中在savedata中存入了“当前时间.txt”（完整的文件名），这里.open完整的文件名，即创建了这个文件
	//Shuffle the SPC tasks //~ 打乱TAC目标手势位置，总共15个，5个手部旋转角度，3个手部张合角度
	const int TestNum = 15;
	srand(time(0));
	if (predictNum%TestNum==0)//~ 15个手势为一轮，下一轮再重新打乱
	{
		for (int i = 0; i<TestNum;i++)
		{
			SPCTasks[i]=i;
		}
		std::random_shuffle(SPCTasks, SPCTasks + TestNum);//~ random_shuffle函数，对一个元素序列进行随机排序得到新的SPCTasks序列
		/*for (int i = 0; i < TestNum; i++)
		{
			printf("%d", SPCTasks[i]);
		}*/
	}
	CurrentTask = SPCTasks[predictNum%TestNum];//~ CurrentTask为当前的随机数，从1~15慢慢数
	predictNum++;//~ predictNum是当前predict的trial数
	if (predictNum%TestNum==0)
	{
		//Prompt next section //~ 15个trial为一轮
		ui.trialno->setText("Predict Trial #"+QString::number(predictNum)+" Next Section!!!");
	}
	else
	{
		ui.trialno->setText("Predict Trial #"+QString::number(predictNum));
	}
	//Set Target Hand Position //~ 设置TAC目标手势，这里指令对应的手势具体看一下unity虚拟手软件
	const char* senddata;
	switch(CurrentTask)
	{
		case 0:
			senddata  = "500";
			break;
		case 1:
			senddata  = "501";
			break;
		case 2:
			senddata  = "502";
			break;
		case 3:
			senddata  = "503";
			break;
		case 4:
			senddata  = "504";
			break;
		case 5:
			senddata  = "505";
			break;
		case 6:
			senddata  = "506";
			break;
		case 7:
			senddata  = "507";
			break;
		case 8:
			senddata  = "508";
			break;
		case 9:
			senddata  = "509";
			break;
		case 10:
			senddata  = "510";
			break;
		case 11:
			senddata  = "511";
			break;
		case 12:
			senddata  = "512";
			break;
		case 13:
			senddata  = "513";
			break;
		case 14:
			senddata  = "514";
			break;
	}
	send(msockClient, senddata, strlen(senddata), 0);//~ 给虚拟手软件发送目标手势	
	
	myPlayer->threadPredict();
}
//~ 测试对应的线程
void QCplot::getPredictFeatureC()
{
	Mat rawData(8, 1000, CV_32FC1);
	std::vector<std::vector<double>> CurrentPackage;
	if (p->copydata1(CurrentPackage) == true || wifi_Thread->copydata_wifi(CurrentPackage) == true)
	{
		for (int i = 0; i<CurrentPackage.size();i++)
		{
			for (int j = 0; j<1000; j++)
			{
				rawData.at<float>(i, j) = float(CurrentPackage[i][j]);
			}
		}
		if((rawData.rows == 8)&&(rawData.at<float>(0,1) == 1))
		{
			if (engine_is_open)
			{
				//~ 调用MATLAB引擎，对rawData（8*1000）进行滤波和特征提取等操作，输出imageFeature（1*feature_col_num）
				Mat imageFeature(1, feature_col_num, CV_64FC1);
				double temp_imageFeature[1][feature_col_num];//~ 暂存imageFeature的数据
				double mat[8][1000];//~ 暂存rawData数据
				Mat temp_mat(8, 1000, CV_64FC1);
				rawData.convertTo(temp_mat, CV_64FC1);
				memcpy(mat, temp_mat.data, 8 * 1000 * sizeof(double));//~ 将cv::Mat类型转成double类型数组
				SetMat("rawData", (void*)mat, 8, 1000, ep);//~ 将c++中的mat矩阵写入到MATLAB工作区中的mat_matlab矩阵
				engEvalString(ep, "imageFeature=getCoefficientImageFeature(rawData)");//~ 调用MATLAB中的pc2new_label函数（自定义函数）
				GetMat("imageFeature", (void*)temp_imageFeature, 1, feature_col_num, ep);//~ 将MATLAB工作区中的mat_matlab矩阵赋值到c++中的mat矩阵，覆盖掉原先mat矩阵的值
				memcpy(imageFeature.data, temp_imageFeature, 1 * feature_col_num * sizeof(double));//~ 将double类型的mat矩阵数组转成cv::Mat类型，存储到Mat A(M, M, CV_64FC1)中
				imageFeature.convertTo(imageFeature, CV_32FC1);//~ 将64位存储类型的A矩阵转成32位（float）类型

				//Mat imageFeature;
				//getCoefficientImageFeature(rawData,currentParametersSettings,imageFeature);//~ 提取A超信号特征参数到imageFeature

				//投影到PCA主成分空间 //~ 得到的结果是一个经过pca主成分空间坐标变换后的A超特征参数数据集合，第一个即源高维数据点在P1坐标系上的投影，可以得到第一主成分PC1的值
				Mat projectedpredict = pca->project(imageFeature);//~ projectedTrain是这一帧的A超特征参数集合imageFeature投影（即坐标变化）到PCA的主成分空间中的数据集合（此处的pca主成分空间是在train_modleC中，由train_feature形成的。）
				projectedpredict.convertTo(projectedpredict, CV_32FC1);//~ 灰度值化，变成单一通道

				//每一个数据预测一次  //~ 取消注释于2021-05-17
				//LR
				Mat onecolumn = Mat::ones(1, 1, CV_32FC1);
				Mat X;
				hconcat(projectedpredict, onecolumn, X);//~ 将多个矩阵拼接起来，可用在增加某一矩阵的最前行和最后的行。这里是将projectedpredict, onecolumn水平拼接，然后输出X

				Mat linearpredict_wrist = X * beta_pc1;
				predict_Wristangle = linearpredict_wrist.at<float>(0, 0);
				Mat linearpredict_hand = X * beta_pc2;
				predict_Handangle = linearpredict_hand.at<float>(0, 0);
			}
			

			//KRR  //~ 注释于2021-05-17
			//Mat KRRpredict = beta.t()*GaussianKernel(projectedTrain, projectedpredict, 0.002);//~ 将predict过程中经过PCA降维后的特征参数集合projectedTrain与训练集projectedTrain放到高斯过程中，预测当前手部张合关节角
			//predict_angle = KRRpredict.at<float>(0, 0);//~ mat.at是取图像中的某一点的值；上面将一帧特征参数放到高斯回归中，输出的就是一个点的值
			
			////SVR
			//predict_angle = SVMmodel.predict(projectedpredict);

			//~ 腕部关节角输出角度归一化处理
			predict_Wristangle = (predict_Wristangle - mmaxMinWrist[1]) / (mmaxMinWrist[0] - mmaxMinWrist[1]);
			//~ 掌指关节角输出角度归一化处理
			predict_Handangle = (predict_Handangle - mmaxMinHand[1]) / (mmaxMinHand[0] - mmaxMinHand[1]);//~ 对计算得到的手部关节角进行归一化处理。这里进行归一化处理是与dataglove得到的真值进行比较


			//角度修正
			/*if (predict_Handangle > 0.6)
			{
				predict_Handangle = 1.25 * predict_Handangle;
			}*/

			
			//非线性映射
			predict_Handangle = predict_Handangle * (0.5 * predict_Handangle + 0.8);							//~ 这部分的作用？？？


			//动态边界修正 //~ 每一帧都在不断的校正边界值，避免出现受试者的实际关节角度达到边界后，仍然无法达到目标位置的情况
			/*BoundUpdate(predict_Handangle, boundHand, 0.005);*/
			/*BoundUpdate(predict_Handangle, boundHand, 0.01);
			predict_Handangle = (predict_Handangle-boundHand[0])/(boundHand[1]-boundHand[0]);
			BoundUpdate(predict_Wristangle, boundWrist, 0.008);
			predict_Wristangle = (predict_Wristangle - boundWrist[0]) / (boundWrist[1] - boundWrist[0]);*/

			int predictnum = predict_label.rows;//~ 后面可以看出，预测的手部张合角度predict_label是用于存储predict_angle，其实本质是用来计数（计时）的，与15s进行对比。所以这里赋值给了predictnum，就是predict线程被激活的次数，一帧数据激活一次
			if (predictnum<ORDER)//~ 前两帧的数据用来初始化，后面的数据就需要用实时的巴特沃斯滤波器进行处理。实时滤波器是和之前数据有关，所以前两帧数据不能滤波
			{
				mxBuf[predictnum] = predict_Handangle;
				myBuf[predictnum] = predict_Handangle;
				mzBuf[predictnum] = predict_Handangle;
				mxBufWrist[predictnum] = predict_Wristangle;
				myBufWrist[predictnum] = predict_Wristangle;
				mzBufWrist[predictnum] = predict_Wristangle;

			}
			else
			{
				//2Hz //~ 这里应该只是将输出数据进行平滑滤波，不会波动的太剧烈
				/*double SOS[ORDER][6] = { 1, 2, 1, 1, -0.4531, 0.4663, 1, 2, 1, 1, -0.329, 0.0646 };
				double Gain[ORDER] = { 0.2533, 0.1839 };*/
				
				////1.5Hz
				double SOS[ORDER][6] = { 1, 2, 1, 1, -0.8977, 0.5272, 1, 2, 1, 1, -0.6727, 0.1445 };
				double Gain[ORDER] = { 0.1574, 0.1179 };

				//1Hz
				//double SOS[ORDER][6] = { 1, 2, 1, 1, -1.3209, 0.6327, 1, 2, 1, 1, -1.0486, 0.2961 };
				//double Gain[ORDER] = { 0.0780, 0.0619 };

				predict_Handangle = RealTimeButterworth(predict_Handangle, SOS, Gain, mxBuf, myBuf, mzBuf);
				predict_Wristangle = RealTimeButterworth(predict_Wristangle, SOS, Gain, mxBufWrist, myBufWrist, mzBufWrist);
				
				
			}
			

			
			float labelValue;
			labelValue = fdGetSensorScaled(glo->pGlove,FD_MIDDLENEAR);//~ 这里读取dataglove的值
			showRealTimeAngle(labelValue, xsens_angle);//~ 这个函数显示预测的腕部和手部关节角度。这里的含义是，如果在预测期间，predict_angle_plot=true，不显示真实的dataglove值，而是显示预测值；如果不在预测期间，就是显示上一行读取到的dataglove的值
			
			wristangle = (predict_Wristangle - 0.5) * 2;

			//~ 输出控制虚拟手之前，进行多点平滑。大小为6的容器vector，存储最近的6次角度结果，取均值后输出为最终结果。不影响实时响应性能，每次都是输出当前值和前五个值的平均值
			//Handangle_last.insert(Handangle_last.begin(), predict_Handangle);
			//wristangle_last.insert(wristangle_last.begin(), wristangle);
			//			
			//float sum_hand = std::accumulate(std::begin(Handangle_last), std::end(Handangle_last), 0.0);
			//predict_Handangle = sum_hand / Handangle_last.size();
			//Handangle_last.pop_back();

			//float sum_wrist = std::accumulate(std::begin(wristangle_last), std::end(wristangle_last), 0.0);
			//wristangle = sum_wrist / wristangle_last.size();
			//wristangle_last.pop_back();

			//~ 控制虚拟手执行当前手势
			TACGestureControl(predict_Handangle, wristangle);			
			

			//TAC任务目标位置  //~ TACTargets[CurrentTask][1]是TAC的手部关节角（0、40、80）；TACTargets[CurrentTask][0]是TAC的腕部关节角（-60、-30、0、30、60）
			//int TACTargets[14][2] = {0, 35, 0, 70, -60, 0, -30, 0, 30, 0, 60, 0, -60, 35, -60, 70, 60, 35, 60, 70, -30, 35, -30, 70, 30, 35, 30, 70};
			//int TACTargets[14][2] = {0, 40, 0, 80, -60, 0, -30, 0, 30, 0, 60, 0, -60, 40, -60, 80, 60, 40, 60, 80, -30, 40, -30, 80, 30, 40, 30, 80};
			int TACTargets[15][2] = {0, 40, 0, 80, -60, 0, -30, 0, 30, 0, 60, 0, -60, 40, -60, 80, 60, 40, 60, 80, -30, 40, -30, 80, 30, 40, 30, 80, 0, 0};//~ 15种组合
			
			if ((abs( predict_Handangle*90-TACTargets[CurrentTask][1])<=10)&&(abs(wristangle *80-TACTargets[CurrentTask][0])<=10))
			{
				DwellingTime++;
			}
			else
			{
				DwellingTime=0;
			}
			outfile<<predict_Handangle*90<<" "<< wristangle *80<<" "<<TACTargets[CurrentTask][1]<<" "<<TACTargets[CurrentTask][0]<<std::endl;
			

			Mat temp = Mat(1,1,CV_32FC1,predict_Handangle);
			predict_label.push_back(temp);//~ 将手部关节角的预测值predict_angle不断加到predict_label中，用于在predict过程中计数。predict_label无实际计算或者比较作用，数据不存储
			//预测时间最多15秒，超过15s，则自动停止
			if((predict_label.rows>=15*frameRate)||(DwellingTime>=3))
			{//~ 8通道的话，一帧是100ms。此处用DwellingTime与3进行判断，说明只要在TAC目标位置范围（±10度）内保持300ms，即可认为是达到目标值
				myPlayer->threadFree();
				//ui.predict_ectype->setEnabled(true);
				predict_angle_plot = false;//~ 
				outfile.close();//~ 每一个predict产生一个file

				_sleep(2000);//~ 延时2000ms

				if (predict_flag == true)
				{
					predict_gesture();
				}
				

			}
		}
	}
}

//~ 找出PC1对应手腕的最大最小值
void QCplot::MaxMinPC1Wrist(Mat PC1)
{
	double minPC1 = 0.0, maxPC1 = 0.0;
	double* minp = &minPC1;
	double* maxp = &maxPC1;
	minMaxIdx(PC1,minp,maxp);//~ 找到第一主成分数据集P1中的最大最小值对应的index分别赋值到minp和maxp，此时minPC1和maxPC1的值也会发生变化，因为是通过minp和maxp进行索引的
	Mat initialPC1Mat = PC1.rowRange(0,10);//~ 取前10行的目的是什么？？？
	double initialPC1 = mean(initialPC1Mat)[0];
	mmaxMinPC1[1] = (fabs(minPC1-initialPC1)<fabs(maxPC1-initialPC1))? minPC1:maxPC1;//~ 这里是一个判断选择式，若条件满足，则为minPC1，否则吗，为maxPC1
	mmaxMinPC1[0] = (fabs(minPC1-initialPC1)>fabs(maxPC1-initialPC1))? minPC1:maxPC1;//~ mmaxMinPC1[0]/mmaxMinPC1[0]与minPC1/maxPC1的对应关系并不是一直确定的，这里只是分别赋值了

}

/************************************************* 虚拟手软件相关 *************************************************/
//~ 通过TCP连接TAC虚拟手软件
void QCplot::Init_Socket()
{
	SOCKADDR_IN addrServer;
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		QMessageBox::warning(this, tr("Winsock load"), tr("Failed to load Winsock"));
	}
	//Setup socket -- client
	msockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == msockClient)
	{
		printf("Socket() error: %d", WSAGetLastError());
	}
	addrServer.sin_family = AF_INET;
	addrServer.sin_addr.S_un.S_addr = inet_addr("127.0.1.1");	
	addrServer.sin_port = htons(9695);
	int ret = ::connect(msockClient, (struct sockaddr*) &addrServer, sizeof(addrServer));
	if (ret == 0)
	{
		ui.TAC_Connect->setText("TAC_Off");
		TAC_is_ready = true;
		printf("%s\n", "TCP connect success!\n"); 
	}
}
//~ 连接虚拟手软件
void QCplot::on_TAC_Connect_clicked()
{
	if (!TAC_is_ready)
	{
		Init_Socket();
	}
	else
	{

		int close_TAC = ::closesocket(msockClient);
		if (close_TAC==0)
		{
			ui.TAC_Connect->setText("TAC_On");
			printf("%s\n", "TCP disconnect!\n");
			TAC_is_ready = false;
		}
		
	}
	

}

//~ 控制当前虚拟手手势，即给虚拟手软件发送计算的腕部角度和手部张合角度
void QCplot::TACGestureControl(float handposition, float wristangle)
{
	std::string angle =std::to_string(static_cast<long double>(handposition))+ "," + std::to_string(static_cast<long double>(wristangle));
	const char* senddata  = angle.c_str();
	send(msockClient, senddata, strlen(senddata), 0);
	 
}

//~ 找出手部张合角度的最大最小值
void QCplot::MaxMinHand(Mat Label)
{//~ dataglove测的数据最小为0，最大为1，对应手部张合角度的0~90度
	double minHand = 0.0, maxHand = 0.0;
	double* minp = &minHand;
	double* maxp = &maxHand;
	minMaxIdx(Label,minp,maxp);//~ 利用minMaxIdx函数找到最大最小值对应的index
	mmaxMinHand[1] = minHand;
	mmaxMinHand[0] = maxHand;

}

void QCplot::MaxMinWrist(Mat Label)
{//~ 腕部关节角的预测，也采用线性回归的方式
	double minWrist = 0.0, maxWrist = 0.0;
	double* minp = &minWrist;
	double* maxp = &maxWrist;
	minMaxIdx(Label, minp, maxp);//~ 利用minMaxIdx函数找到最大最小值对应的index
	mmaxMinWrist[1] = minWrist;
	mmaxMinWrist[0] = maxWrist;
}

//~ 实时巴特沃斯滤波器，在线测试用
double QCplot::RealTimeButterworth(double x, double SOS[2][6], double Gain[2], double input[ORDER+1], double  inter[ORDER+1], double output[ORDER+1])
{
	for (int i=ORDER; i>0; i--)
	{
		input[i] = input[i-1];
		inter[i] = inter[i-1];
		output[i]=output[i-1];
	}
	input[0]= x; inter[0]=0; output[0]=0;
	//第一级二阶巴特沃斯
	for (int i=1; i<=ORDER; i++)
	{
		inter[0]+=Gain[0]*SOS[0][i]*input[i]-SOS[0][i+3]*inter[i];
	}
	inter[0]+=Gain[0]*SOS[0][0]*input[0];
	//第二级二阶巴特沃斯
	for (int i=1; i<=ORDER; i++)
	{
		output[0]+=Gain[1]*SOS[1][i]*inter[i]-SOS[1][i+3]*output[i];
	}
	output[0]+=Gain[1]*SOS[1][0]*inter[0];
	return output[0];
}
//~ 自适应边界更新
void QCplot::BoundUpdate(double data, double bound[2],double coeff)
{
	double lower = bound[0], upper = bound[1];
	if ((data>=lower)&&(data<=upper))
	{
		if (abs(data-lower)<abs(data-upper))
		{
			lower = coeff*data + (1-coeff)*lower;
		}
		else
		{
			upper = coeff*data + (1-coeff)*upper;
		}
	}
	else if (data<lower)
	{
		lower = (data + lower)/2;
	}
	else if (data>upper)
	{
		upper = (data+upper)/2;
	}
	bound[0] = lower; bound[1] = upper;
}
//~ 高斯核岭回归
Mat QCplot::GaussianKernel(Mat data1, Mat data2, double sigma)
{
	Mat data1square = data1.mul(data1);//~ mul函数是矩阵相乘，这里得到的是矩阵的平方
	Mat data2square = data2.mul(data2);
	Mat rowsum1, rowsum2;
	reduce(data1square, rowsum1, 1, CV_REDUCE_SUM);//~ reduce函数可以将二维数组转化为向量，第三个参数为1，意味着矩阵被处理成一列，第二个参数书处理输入矩阵data1square后得到的列向量，为原始矩阵每一行数据的求和
	reduce(data2square, rowsum2, 1, CV_REDUCE_SUM);
	Mat K1, K2;
	repeat(rowsum1, 1, data2.rows, K1);
	repeat(rowsum2.t(), data1.rows, 1, K2);
	Mat K0 = K1+K2-2*data1*data2.t();
	Mat K;
	exp(-sigma*K0, K);
	return K;
	                                                             
}


/************************************************** WiFi设置*****************************************************/

void QCplot::on_WifiMode_clicked()
{
	//ui.chooseInterface->setCurrentWidget(ui.WifiInterface);
	ui.start->setEnabled(true);
	ui.stop->setEnabled(false);
	myPlayer->setStop();
	ui.wifi_connect->setEnabled(true);
}
void QCplot::on_EthernetMode_clicked()
{
	//~ 关闭wifi
	ui.wifi_connect->setText(QString::fromLocal8Bit("Wifi_On"));
	wifi_Thread->wifi_stopThread();//~ 停止线程


	ui.start->setEnabled(true);
	ui.stop->setEnabled(false);

	p->opendevice();//~ 开启以太网读取数据的线程（线程的开启是封装在了以太网相关的dll文件中，内部调用了_beginthreadex函数）
}


void QCplot::on_wifi_connect_clicked()
{

	if (wifi_Thread->wifi_connect())//~ 返回值是bool值，用于判断是否完成连接
	{
		ui.wifi_connect->setText(QString::fromLocal8Bit("Wifi_Off"));
		ui.start->setEnabled(true);
		ui.stop->setEnabled(false);
		wifi_Thread->start();//~ 此时的状态是已经打开了wifi，所以需要对应开启接收wifi数据的线程
	}
	else
	{
		ui.wifi_connect->setText(QString::fromLocal8Bit("Wifi_On"));
		ui.start->setEnabled(false);
		ui.stop->setEnabled(true);
		wifi_Thread->wifi_stopThread();//~ 停止线程
	}
	
}



void QCplot::on_Train_Model_clicked()
{//~ 训练模型，用于预测
	if (engine_is_open)
	{
		trainModelC();
		ui.motion_prompt->setText("Model Trained");
		ui.train_ectype->setEnabled(true);
		ui.predict_ectype->setEnabled(true);

		std::cout << "\nTrain Model End!\n" << std::endl;
	}
	else
	{
		std::cout << "\n Matlab engine is not open! \n" << std::endl;
	}
	
}

/******************************************************** MATLAB相关（开始） ************************************************/
void QCplot::on_Run_matlab_clicked()
{
	//~ 调用MATLAB计算引擎
	if (!(ep = engOpen(NULL)))  //打开引擎 测试是否启动引擎成功
	{
		std::cout << "Can't start Matlab engine!" << std::endl;
		engine_is_open = false;
		return;
	}
	engine_is_open = true;
	std::cout << "Start Matlab Engine!" << std::endl;

	//~ 把mat_file文件夹内的数据移动到“备份”文件夹中
	engEvalString(ep, "files_backup()");
	std::cout << "back up files" << std::endl;

}

void QCplot::GetMat(char* VarName, void* mat, int M, int N, Engine* engine)
{//~ 从MATLAB工作区中获取变量的值，矩阵mxArray转为mat格式
	memcpy(mat, mxGetPr(engGetVariable(engine, VarName)), M * N * sizeof(double));
}

int QCplot::SetMat(char* VarName, void* mat, int M, int N, Engine* engine)
{//~ 将c++中定义的数组，赋值到mxArray，可以写入到MATLAB工作区，方便进一步做数据处理
	mxArray* matArray = mxCreateDoubleMatrix(M, N, mxREAL);
	memcpy(mxGetPr(matArray), mat, M * N * sizeof(double));
	int status = engPutVariable(engine, VarName, matArray);
	mxDestroyArray(matArray);
	return status;
}

void QCplot::close_engine()
{
	engine_is_open = false;
	engClose(ep);
	std::cout << "Close Matlab Engine!" << std::endl;
}
/******************************************************** MATLAB相关（结束） ************************************************/


/***************************************~ xsens_MTw相关代码（开始） ***********************************************************/
void QCplot::on_Xsens_Connect_clicked()
{	
	if (!xsens_is_ready)
	{
		//~ 开始进行设备扫描连接等操作
		std::cout << "\nSearching Xsens..." << std::endl;

		if (control == 0)
		{
			std::cout << "Failed to construct XsControl instance." << std::endl;
			xsens_is_ready = false;
			return;
		}

		try
		{
			//~ 先从设备管理器中读取串口
			//std::cout << "Scanning ports..." << std::endl;
			XsPortInfoArray detectedDevices = XsScanner::scanPorts();

			//~ 再从读取到的串口中，找到master设备（Xsens Awinda Station Virtual COM port）
			//std::cout << "Finding wireless master..." << std::endl;
			XsPortInfoArray::const_iterator wirelessMasterPort = detectedDevices.begin();
			while (wirelessMasterPort != detectedDevices.end() && !wirelessMasterPort->deviceId().isWirelessMaster())
			{
				++wirelessMasterPort;
			}
			if (wirelessMasterPort == detectedDevices.end())
			{
				throw std::runtime_error("No wireless masters found");
			}
			std::cout << "Wireless master found @ " << *wirelessMasterPort << std::endl;

			//~ 打开master设备，此时STAT指示灯会从红灯变为绿灯
			//std::cout << "Opening port..." << std::endl;
			if (!control->openPort(wirelessMasterPort->portName().toStdString(), wirelessMasterPort->baudrate()))
			{
				std::ostringstream error;
				error << "Failed to open port " << *wirelessMasterPort;
				throw std::runtime_error(error.str());
				xsens_is_ready = false;
				return;
			}

			//~ 这里的XsDevice instance是指IMU模块（在找xsens的实例）
			//std::cout << "Getting XsDevice instance for wireless master..." << std::endl;
			XsDevicePtr wirelessMasterDevice = control->device(wirelessMasterPort->deviceId());
			if (wirelessMasterDevice == 0)
			{
				std::ostringstream error;
				error << "Failed to construct XsDevice instance: " << *wirelessMasterPort;
				throw std::runtime_error(error.str());
				xsens_is_ready = false;
				return;
			}

			//std::cout << "XsDevice instance created @ " << *wirelessMasterDevice << std::endl;

			//~ 设置配置模式，应该对应于MT Manager中的wireless configration配置连接
			//std::cout << "Setting config mode..." << std::endl;
			if (!wirelessMasterDevice->gotoConfig())
			{
				std::ostringstream error;
				error << "Failed to goto config mode: " << *wirelessMasterDevice;
				throw std::runtime_error(error.str());
				xsens_is_ready = false;
				return;
			}

			//std::cout << "Attaching callback handler..." << std::endl;
			wirelessMasterDevice->addCallbackHandler(&wirelessMasterCallback);

			//~ 列出IMU所支持的采样频率
			std::cout << "Getting the list of the supported update rates..." << std::endl;
			const XsIntArray supportedUpdateRates = wirelessMasterDevice->supportedUpdateRates();

			std::cout << "Supported update rates: ";
			for (XsIntArray::const_iterator itUpRate = supportedUpdateRates.begin(); itUpRate != supportedUpdateRates.end(); ++itUpRate)
			{
				std::cout << *itUpRate << " ";
			}
			std::cout << std::endl;

			//~ 找到IMU模块支持的采样率和预设采样率最接近的一个值，即最终需要设置的采样率
			const int newUpdateRate = findClosestUpdateRate(supportedUpdateRates, desiredUpdateRate);

			std::cout << "Setting update rate to " << newUpdateRate << " Hz..." << std::endl;
			if (!wirelessMasterDevice->setUpdateRate(newUpdateRate))
			{
				std::ostringstream error;
				error << "Failed to set update rate: " << *wirelessMasterDevice;
				throw std::runtime_error(error.str());
				xsens_is_ready = false;
				return;
			}


			//std::cout << "Disabling radio channel if previously enabled..." << std::endl;
			if (wirelessMasterDevice->isRadioEnabled())
			{
				if (!wirelessMasterDevice->disableRadio())
				{
					std::ostringstream error;
					error << "Failed to disable radio channel: " << *wirelessMasterDevice;
					throw std::runtime_error(error.str());
					xsens_is_ready = false;
					return;
				}
			}

			//~ 设置radio channel，即IMU模块连接到station的其中一个channel，好像station总共有23个channel，即可同时连接23个IMU模块
			std::cout << "Setting radio channel to " << desiredRadioChannel << " and enabling radio..." << std::endl;
			if (!wirelessMasterDevice->enableRadio(desiredRadioChannel))
			{
				std::ostringstream error;
				error << "Failed to set radio channel: " << *wirelessMasterDevice;
				throw std::runtime_error(error.str());
				xsens_is_ready = false;
				return;
			}

			//~ 将IMU模块连接到master（station）
			std::cout << "Waiting for MTW to wirelessly connect...\n" << std::endl;

			bool waitForConnections = true;
			size_t connectedMTWCount = wirelessMasterCallback.getWirelessMTWs().size();
			/*do
			{
				XsTime::msleep(100);*/

			//while (true)
			//{
			//	XsTime::msleep(100);
			//	size_t nextCount = wirelessMasterCallback.getWirelessMTWs().size();
			//	if (nextCount != connectedMTWCount)
			//	{
			//		//std::cout << "Number of connected MTWs: " << nextCount << ". Press 'Y' to start measurement." << std::endl;
			//		std::cout << "Number of connected MTWs: " << nextCount << std::endl;
			//		connectedMTWCount = nextCount;
			//		break;
			//	}

			//}
			// 
			do
			{
				XsTime::msleep(100);

				while (true)
				{
					size_t nextCount = wirelessMasterCallback.getWirelessMTWs().size();
					if (nextCount != connectedMTWCount)
					{
						std::cout << "Number of connected MTWs: " << nextCount << ". Press 'Y' to start measurement." << std::endl;
						connectedMTWCount = nextCount;
					}
					else
					{
						break;
					}
				}
				if (_kbhit())
				{
					waitForConnections = (toupper((char)_getch()) != 'Y');
				}
			}
			//~ 如果一直没用点Y，就一直等待
			while (waitForConnections);
			//	if (_kbhit())
			//	{
			//		waitForConnections = (toupper((char)_getch()) != 'Y');
			//	}
			//}
			////~ 如果一直没用点Y，就一直等待
			//while (waitForConnections);
			//~ 开始采集IMU模块的数据
			std::cout << "Starting measurement..." << std::endl;
			if (!wirelessMasterDevice->gotoMeasurement())
			{
				std::ostringstream error;
				error << "Failed to goto measurement mode: " << *wirelessMasterDevice;
				throw std::runtime_error(error.str());
				xsens_is_ready = false;
				return;
			}
			//~ 分配之前创建的XsDevice instances给各个IMU模块
			//std::cout << "Getting XsDevice instances for all MTWs..." << std::endl;
			XsDeviceIdArray allDeviceIds = control->deviceIds();
			XsDeviceIdArray mtwDeviceIds;
			for (XsDeviceIdArray::const_iterator i = allDeviceIds.begin(); i != allDeviceIds.end(); ++i)
			{
				if (i->isMtw())
				{
					mtwDeviceIds.push_back(*i);
				}
			}
			XsDevicePtrArray mtwDevices;
			for (XsDeviceIdArray::const_iterator i = mtwDeviceIds.begin(); i != mtwDeviceIds.end(); ++i)
			{
				XsDevicePtr mtwDevice = control->device(*i);
				if (mtwDevice != 0)
				{
					mtwDevices.push_back(mtwDevice);
				}
				else
				{
					throw std::runtime_error("Failed to create an MTW XsDevice instance");
					xsens_is_ready = false;
					return;
				}
			}

			//~ 可以容纳多个IMU模块，对应于mtwCallbacks[i]
			//std::cout << "Attaching callback handlers to MTWs..." << std::endl;
			mtwCallbacks.resize(mtwDevices.size());
			for (int i = 0; i < (int)mtwDevices.size(); ++i)
			{
				mtwCallbacks[i] = new MtwCallback(i, mtwDevices[i]);
				mtwDevices[i]->addCallbackHandler(mtwCallbacks[i]);
			}

			//std::cout << "\nMain loop. Press any key to quit\n" << std::endl;
			std::cout << "Waiting for data available..." << std::endl;

			xsens_is_ready = true;
			ui.Xsens_Connect->setText("Xsens_Off");
		}
		catch (std::exception const& ex)
		{
			std::cout << ex.what() << std::endl;
			std::cout << "****ABORT****" << std::endl;
		}
		catch (...)
		{
			std::cout << "An unknown fatal error has occured. Aborting." << std::endl;
			std::cout << "****ABORT****" << std::endl;
		}
	}
	else
	{

		xsens_is_ready = false;
		//~ 关闭xsens连接
		close_xsens();
		ui.Xsens_Connect->setText("Xsens_On");

	}
	
	
};

/***************************************~ xsens_MTw相关代码（结束） ***********************************************************/


/*----------------------------------------------------- 分割线-----------------------------------------------------------------*/

/**************************************** 加入其它2个范式（mode-1和mode-2） ******************************************************************/
/*
目前只有对应范式的数据采集功能，没有加入离线模型算法和predict部分
*/

/*************************************** mode-1（开始） ***************************************************************/

void QCplot::on_mode1_start_clicked()
{

	if (myPlayer->isStop())
	{
		myPlayer->threadStart();
		ui.mode1_start->setText("Stop");
		ui.mode1_train->setEnabled(true);
	}

	else
	{
		myPlayer->setStop();
		ui.mode1_start->setText("Start");
		ui.mode1_train->setEnabled(false);
		ui.mode1_predict->setEnabled(false);
	}

}

//~ 训练按钮
void QCplot::on_mode1_train_clicked()
{
	ui.mode1_train->setEnabled(false);
	PacketNumber_mode1 = 0;
	gettime();
	//~ 创建文件，用于存储训练数据
	for (int i = 0; i < 8; i++)
	{
		stream[i] = fopen(fileName[i], "ab+");
	}

	//~ 训练时候记录手指位置,dataglove测得的真值
	if (glo->datagloveused)
	{
		outfile_dataglove.open(save_dataglove);
	}

	save_flag = true;


	//~ 如果xsens连接成功，则创建文件，用于采集数据
	if (xsens_is_ready)
	{
		////~ 创建xsens文件，不开线程，在getTrainFeatureC()中采集数据，保证和A超的采集存储频率相同
		gettime();
		//outfile_xsens.open(savedata_xsens);//~ 创建存储xsens数据的txt文档
		//outfile_xsens << "roll" << "	" << "pitch" << "	" << "yaw" << std::endl;
		xsens_data = fopen(save_xsens, "w");
		if (xsens_data == NULL)
		{
			printf("Can't open xsens data files.\n");
		}
		else
		{
			//Create headers for CSV files
			fprintf(xsens_data, "xsens1, , ,xsens2, , ,xsens3\n");
			fprintf(xsens_data, "roll,pitch,yaw,roll,pitch,yaw,roll,pitch,yaw\n");
		}


		//~ 创建并打开xsens文件，单独开一个线程，存储完整的xsens数据
		gettime();
		//outfile_xsens_all.open(savedata_xsens_all);//~ 创建存储xsens数据的txt文档
		//outfile_xsens_all << "roll" << "	" << "pitch" << "	" << "yaw" << std::endl;
		xsens_data_all = fopen(save_xsens_all, "w");
		if (xsens_data_all == NULL)
		{
			printf("Can't open xsens data files.\n");
		}
		else
		{
			//Create headers for CSV files
			fprintf(xsens_data_all, "xsens1, , ,xsens2, , ,xsens3\n");
			fprintf(xsens_data_all, "roll,pitch,yaw,roll,pitch,yaw,roll,pitch,yaw\n");
		}

		xsensthread_flag = true;
		xsensthread->start();
	}

	if (Delsys_On_flag)
	{//~ 调用delsys的c++ sdk，直接在上位机的一个线程内采集数据
		delsys_sdk_thread->create_delsys_file();
		delsys_sdk_thread->start_save();
	}

	myPlayer->threadTrain_mode1();
}

//~ 训练对应的线程
void QCplot::getTrainFeature_mode1()
{
	Mat rawData(8, 1000, CV_32FC1);
	std::vector<std::vector<double>> CurrentPackage;
	//QDateTime currentTime = QDateTime::currentDateTime();
	//QString currentTime_str = currentTime.toString("dd_hh_mm_ss.zzz");
	//qDebug() << currentTime_str << endl;
	if (p->copydata1(CurrentPackage) == true || wifi_Thread->copydata_wifi(CurrentPackage) == true)//~ 读取A超设备传的数据
	{
		//qDebug() << "*" << endl;
		QDateTime currentTime = QDateTime::currentDateTime();
		QString currentTime_str = currentTime.toString("dd_hh_mm_ss.zzz");
		qDebug() << currentTime_str << endl;
		showimage1(CurrentPackage);//~ 显示当前A超信号到控件
		for (int i = 0; i < CurrentPackage.size(); i++)
		{
			for (int j = 0; j < 1000; j++)

			{
				rawData.at<float>(i, j) = float(CurrentPackage[i][j]);
			}
		}
		if ((rawData.rows == 8) && (rawData.at<float>(0, 1) == 1))
		{
			//~ 训练过程中，保存A超原始数据
			Mat saveData;
			rawData.convertTo(saveData, CV_8UC1);
			saveimage(saveData);


			//~ 读取存储并显示xsens的数据
			if (xsens_is_ready)
			{
				read_xsens_data();
			}

			if (glo->datagloveused || xsens_is_ready)
			{
				float labelValue = fdGetSensorScaled(glo->pGlove, FD_MIDDLENEAR);//~ 获取数据手套当前的手部张合角度值（0~90），这里取的是手套背部中间的一条传感器的数值
				outfile_dataglove << labelValue << std::endl;//~ 存储数据手套的数据
				showRealTimeAngle(labelValue, xsens_angle);//~ 显示数据手套的值到控件
			}

			if (engine_is_open)
			{
				//~ 调用MATLAB引擎，对rawData（8*1000）进行滤波和特征提取等操作，输出imageFeature（1*feature_col_num）
				Mat imageFeature(1, feature_col_num, CV_64FC1);
				double temp_imageFeature[1][feature_col_num];//~ 暂存imageFeature的数据
				double mat[8][1000];//~ 暂存rawData数据
				Mat temp_mat(8, 1000, CV_64FC1);
				rawData.convertTo(temp_mat, CV_64FC1);
				memcpy(mat, temp_mat.data, 8 * 1000 * sizeof(double));//~ 将cv::Mat类型转成double类型数组
				int ans = SetMat("rawData", (void*)mat, 8, 1000, ep);//~ 将c++中的mat矩阵写入到MATLAB工作区中的mat_matlab矩阵
				
				engEvalString(ep, "imageFeature=getCoefficientImageFeature(rawData)");//~ 提取A超信号特征参数到imageFeature，有48（子窗数）*1（特征参数个数）*8（通道数）=384个特征参数。是1*384的矩阵
				qDebug() << ans;
				engEvalString(ep, "imageFeature = imageFeature'");
				GetMat("imageFeature", (void*)temp_imageFeature, 1, feature_col_num, ep);//~ 将MATLAB工作区中的mat_matlab矩阵赋值到c++中的mat矩阵，覆盖掉原先mat矩阵的值
				memcpy(imageFeature.data, temp_imageFeature, 1 * feature_col_num * sizeof(double));//~ 将double类型的mat矩阵数组转成cv::Mat类型，存储到Mat A(M, M, CV_64FC1)中
				imageFeature.convertTo(imageFeature, CV_32FC1);//~ 将64位存储类型的A矩阵转成32位（float）类型
				train_feature.push_back(imageFeature);//~ push_back是按行加在mat矩阵最后一行
			}

			PacketNumber_mode1++;

			//~ 用progressbar控件显示当前训练的进度
			ui.train_progressBar_mode1->setValue(PacketNumber_mode1+1);


			int step = PacketNumber_mode1 % (frameRate * holdTime * classNum_mode1);//~ %百分号是取模运算符，整除后的余数
			//红框提醒训练动作
			int temp = frameRate * holdTime;//~ 10*5；


			if ((step >= 0) && (step < temp))
			{//~ 去掉第6张手势的红色提醒边框，使能第一张手势的红色提醒边框；下面的顺序依次类推
				imageShowLabel_mode1[9]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_mode1[0]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");

				if (step > temp - frameRate)
				{
					imageShowLabel_mode1[1]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= temp) && (step < 2 * temp))
			{
				imageShowLabel_mode1[0]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_mode1[1]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step > 2 * temp - frameRate)
				{
					imageShowLabel_mode1[2]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= 2 * temp) && (step < 3 * temp))
			{
				imageShowLabel_mode1[1]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_mode1[2]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step > 3 * temp - frameRate)
				{
					imageShowLabel_mode1[3]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= 3 * temp) && (step < 4 * temp))
			{
				imageShowLabel_mode1[2]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_mode1[3]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step > 4 * temp - frameRate)
				{
					imageShowLabel_mode1[4]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= 4 * temp) && (step < 5 * temp))
			{
				imageShowLabel_mode1[3]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_mode1[4]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step > 5 * temp - frameRate)
				{
					imageShowLabel_mode1[5]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= 5 * temp) && (step < 6 * temp))
			{
				imageShowLabel_mode1[4]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_mode1[5]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step > 6 * temp - frameRate)
				{
					imageShowLabel_mode1[6]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= 6*temp) && (step < 7 * temp))
			{
				imageShowLabel_mode1[5]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_mode1[6]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step > 7 * temp - frameRate)
				{
					imageShowLabel_mode1[7]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= 7 * temp) && (step < 8 * temp))
			{
				imageShowLabel_mode1[6]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_mode1[7]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step > 8 * temp - frameRate)
				{
					imageShowLabel_mode1[8]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= 8 * temp) && (step < 9 * temp))
			{
				imageShowLabel_mode1[7]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_mode1[8]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step > 9 * temp - frameRate)
				{
					imageShowLabel_mode1[9]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= 9 * temp) && (step < 10 * temp))
			{
				imageShowLabel_mode1[8]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_mode1[9]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step == 10 * temp - 1)
				{
					//~ 提示休息10s
					ui.remind_mode1->setText("Rest 10s");
				}
			}
				


		
		
			if (PacketNumber_mode1 >= frameRate * holdTime * trialNum_mode1 * classNum_mode1)
			{
				imageShowLabel_mode1[9]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				
				ui.mode1_train->setEnabled(true);
				//释放存储指针
				for (int i = 0; i < 8; i++)
				{
					fclose(stream[i]);
				}
				
				save_flag = false;
				
				if (glo->datagloveused)
				{//~ 停止数据手套的数据采集
					outfile_dataglove.close();//~ 关闭存储数据的文件。否则会导致内存溢出，MATLAB引擎调用也会失败
				}

				if (xsens_is_ready)
				{
					//~ 停止xsens_all采集数据的线程
					xsensthread_flag = false;
					//outfile_xsens_all.close();

					////~ 停止xsens数据采集
					//outfile_xsens.close();

					fclose(xsens_data);
					fclose(xsens_data_all);

				}


				if (Delsys_On_flag)
				{
					delsys_sdk_thread->stop_read_delsys();//~ 停止读取delsys数据的线程					
				}

				ui.remind_mode1->setText("");

				myPlayer->threadFree();
			}
			else if (step == 0)
			{
				_sleep(10000);
				ui.remind_mode1->setText("");
			}


		}
	}
}

void QCplot::showTrainImage_mode1()//~ 读取gesture-mode1文件夹目录下的手势图片（训练用），显示在UI界面的imageIn_2控件上
{
	//读取本地工程目录下的文件
	//QDir dir("gesture");											
	QDir dir("gesture-mode1");
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Name | QDir::NoSort);
	QFileInfoList list = dir.entryInfoList();
	classNum_mode1 = list.size();	                                        //~ classNum_mode1 是gesture-mode1文件夹中手势类别数量？
	//std::cout<< classNum_mode1;
	for (int i = 0; i < (classNum_mode1); i++)//~ 修改前：for (int i = 0; i < (list.size()-1); i++)
	{
		trainTrialOrder.push_back(i);
		QFileInfo fileInfo = list.at(i);
		String filename;
		//filename.append("gesture/");
		filename.append("gesture-mode1/");
		filename.append(fileInfo.fileName().toLatin1().data());		//由QString转到String
		gestureName.push_back(filename);
		Mat currentImage = imread(filename, CV_LOAD_IMAGE_COLOR);
		Mat currentQImage;
		cvtColor(currentImage, currentQImage, CV_BGR2RGB);
		QImage img = QImage((const unsigned char*)(currentQImage.data), currentQImage.cols,
			currentQImage.rows, currentQImage.step, QImage::Format_RGB888);

		QLabel* labelTrainImage = new QLabel(ui.centralWidget);
		QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
		ui.motion_image_mode1->addWidget(labelTrainImage);
		labelTrainImage->setScaledContents(true);
		labelTrainImage->resize(ui.imageWidget->size());
		labelTrainImage->setAutoFillBackground(false);
		labelTrainImage->setStyleSheet(QStringLiteral("Background-color: #000;"));
		labelTrainImage->setPixmap(QPixmap::fromImage(img));
		labelTrainImage->show();
		imageShowLabel_mode1.push_back(labelTrainImage);
	}
}





/*************************************** mode-1（结束） ***************************************************************/



/*************************************** mode-2（开始） ***************************************************************/
void QCplot::on_mode2_start_clicked()
{

	if (myPlayer->isStop())
	{
		myPlayer->threadStart();
		ui.mode2_start->setText("Stop");
		ui.mode2_train->setEnabled(true);
	}

	else
	{
		myPlayer->setStop();
		ui.mode2_start->setText("Start");
		ui.mode2_train->setEnabled(false);
		ui.mode2_predict->setEnabled(false);
	}

}

//~ 训练按钮
void QCplot::on_mode2_train_clicked()
{
	ui.mode2_train->setEnabled(false);
	PacketNumber_mode2 = 0;
	gettime();
	//~ 创建文件，用于存储训练数据
	for (int i = 0; i < 8; i++)
	{
		stream[i] = fopen(fileName[i], "ab+");
	}

	//~ 训练时候记录手指位置,dataglove测得的真值
	if (glo->datagloveused)
	{
		outfile_dataglove.open(save_dataglove);
	}

	save_flag = true;


	//~ 如果xsens连接成功，则创建文件，用于采集数据
	if (xsens_is_ready)
	{
		////~ 创建xsens文件，不开线程，在getTrainFeatureC()中采集数据，保证和A超的采集存储频率相同
		gettime();
		//outfile_xsens.open(savedata_xsens);//~ 创建存储xsens数据的txt文档
		//outfile_xsens << "roll" << "	" << "pitch" << "	" << "yaw" << std::endl;
		xsens_data = fopen(save_xsens, "w");
		if (xsens_data == NULL)
		{
			printf("Can't open xsens data files.\n");
		}
		else
		{
			//Create headers for CSV files
			fprintf(xsens_data, "xsens1, , ,xsens2, , ,xsens3\n");
			fprintf(xsens_data, "roll,pitch,yaw,roll,pitch,yaw,roll,pitch,yaw\n");
		}


		//~ 创建并打开xsens文件，单独开一个线程，存储完整的xsens数据
		gettime();
		//outfile_xsens_all.open(savedata_xsens_all);//~ 创建存储xsens数据的txt文档
		//outfile_xsens_all << "roll" << "	" << "pitch" << "	" << "yaw" << std::endl;
		xsens_data_all = fopen(save_xsens_all, "w");
		if (xsens_data_all == NULL)
		{
			printf("Can't open xsens data files.\n");
		}
		else
		{
			//Create headers for CSV files
			fprintf(xsens_data_all, "xsens1, , ,xsens2, , ,xsens3\n");
			fprintf(xsens_data_all, "roll,pitch,yaw,roll,pitch,yaw,roll,pitch,yaw\n");
		}

		xsensthread_flag = true;
		xsensthread->start();
	}


	if (Delsys_On_flag)
	{//~ 调用delsys的c++ sdk，直接在上位机的一个线程内采集数据
		delsys_sdk_thread->create_delsys_file();
		delsys_sdk_thread->start_save();
	}

	myPlayer->threadTrain_mode2();
}



void QCplot::showTrainImage_mode2()//~ 读取gesture-mode1文件夹目录下的手势图片（训练用），显示在UI界面的imageIn_2控件上
{
	//读取本地工程目录下的文件
	//QDir dir("gesture");											
	QDir dir("gesture-mode2");
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Name | QDir::NoSort);
	QFileInfoList list = dir.entryInfoList();
	classNum_mode2 = list.size();	                                        //~ classNum 是gesture-mode2文件夹中手势类别数量？
	//std::cout<< classNum_mode2;
	for (int i = 0; i < (classNum_mode2); i++)//~ 修改前：for (int i = 0; i < (list.size()-1); i++)
	{
		trainTrialOrder.push_back(i);
		QFileInfo fileInfo = list.at(i);
		String filename;
		//filename.append("gesture/");
		filename.append("gesture-mode2/");
		filename.append(fileInfo.fileName().toLatin1().data());		//由QString转到String
		gestureName.push_back(filename);
		Mat currentImage = imread(filename, CV_LOAD_IMAGE_COLOR);
		Mat currentQImage;
		cvtColor(currentImage, currentQImage, CV_BGR2RGB);
		QImage img = QImage((const unsigned char*)(currentQImage.data), currentQImage.cols,
			currentQImage.rows, currentQImage.step, QImage::Format_RGB888);

		QLabel* labelTrainImage = new QLabel(ui.centralWidget);
		QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
		ui.motion_image_mode2->addWidget(labelTrainImage);
		labelTrainImage->setScaledContents(true);
		labelTrainImage->resize(ui.imageWidget->size());
		labelTrainImage->setAutoFillBackground(false);
		labelTrainImage->setStyleSheet(QStringLiteral("Background-color: #000;"));
		labelTrainImage->setPixmap(QPixmap::fromImage(img));
		labelTrainImage->show();
		imageShowLabel_mode2.push_back(labelTrainImage);
	}
}

//~ 训练对应的线程
void QCplot::getTrainFeature_mode2()
{
	Mat rawData(8, 1000, CV_32FC1);
	std::vector<std::vector<double>> CurrentPackage;
	if (p->copydata1(CurrentPackage) == true || wifi_Thread->copydata_wifi(CurrentPackage) == true)//~ 读取A超设备传的数据
	{
		showimage1(CurrentPackage);//~ 显示当前A超信号到控件
		for (int i = 0; i < CurrentPackage.size(); i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				rawData.at<float>(i, j) = float(CurrentPackage[i][j]);
			}
		}
		if ((rawData.rows == 8) && (rawData.at<float>(0, 1) == 1))
		{
			//~ 训练过程中，保存A超原始数据
			Mat saveData;
			rawData.convertTo(saveData, CV_8UC1);
			saveimage(saveData);


			//~ 读取存储并显示xsens的数据
			if (xsens_is_ready)
			{
				read_xsens_data();
			}

			if (glo->datagloveused || xsens_is_ready)
			{
				float labelValue = fdGetSensorScaled(glo->pGlove, FD_MIDDLENEAR);//~ 获取数据手套当前的手部张合角度值（0~90），这里取的是手套背部中间的一条传感器的数值
				outfile_dataglove << labelValue << std::endl;//~ 存储数据手套的数据
				showRealTimeAngle(labelValue, xsens_angle);//~ 显示数据手套的值到控件
			}

			if (engine_is_open)
			{
				//~ 调用MATLAB引擎，对rawData（8*1000）进行滤波和特征提取等操作，输出imageFeature（1*feature_col_num）
				Mat imageFeature(1, feature_col_num, CV_64FC1);
				double temp_imageFeature[1][feature_col_num];//~ 暂存imageFeature的数据
				double mat[8][1000];//~ 暂存rawData数据
				Mat temp_mat(8, 1000, CV_64FC1);
				rawData.convertTo(temp_mat, CV_64FC1);
				memcpy(mat, temp_mat.data, 8 * 1000 * sizeof(double));//~ 将cv::Mat类型转成double类型数组
				SetMat("rawData", (void*)mat, 8, 1000, ep);//~ 将c++中的mat矩阵写入到MATLAB工作区中的mat_matlab矩阵
				engEvalString(ep, "imageFeature=getCoefficientImageFeature(rawData)");//~ 提取A超信号特征参数到imageFeature，有48（子窗数）*1（特征参数个数）*8（通道数）=384个特征参数。是1*384的矩阵
				engEvalString(ep, "imageFeature = imageFeature'");
				GetMat("imageFeature", (void*)temp_imageFeature, 1, feature_col_num, ep);//~ 将MATLAB工作区中的mat_matlab矩阵赋值到c++中的mat矩阵，覆盖掉原先mat矩阵的值
				memcpy(imageFeature.data, temp_imageFeature, 1 * feature_col_num * sizeof(double));//~ 将double类型的mat矩阵数组转成cv::Mat类型，存储到Mat A(M, M, CV_64FC1)中
				imageFeature.convertTo(imageFeature, CV_32FC1);//~ 将64位存储类型的A矩阵转成32位（float）类型
				train_feature.push_back(imageFeature);//~ push_back是按行加在mat矩阵最后一行
			}

			PacketNumber_mode2++;

			//~ 用progressbar控件显示当前训练的进度
			ui.train_progressBar_mode2->setValue(PacketNumber_mode2+1);

			if (PacketNumber_mode2 == 1)
			{
				//~ 显示动图
				movie_wrist_mode2 = new QMovie("Resources/WristRotation.gif");
				ui.wrist_motion->setMovie(movie_wrist_mode2);
				movie_wrist_mode2->start();
			}


			int step = PacketNumber_mode2 % (frameRate * holdTime_mode2 * classNum_mode2);//~ %百分号是取模运算符，整除后的余数
			//红框提醒训练动作
			int temp = frameRate * holdTime_mode2;


			if ((step >= 0) && (step < temp))
			{//~ 去掉第6张手势的红色提醒边框，使能第一张手势的红色提醒边框；下面的顺序依次类推
				imageShowLabel_mode2[5]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_mode2[0]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				ui.remind_mode2->setText("");
				/*if (step > temp - frameRate)
				{
					imageShowLabel_mode2[1]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}*/
				if (step == temp - 2)
				{
					//~ 提示休息10s
					ui.remind_mode2->setText("Rest 10s");
				}
				else if (step == temp - 1)
				{
					_sleep(10000);
				}
			}
			else if ((step >= temp) && (step < 2 * temp))
			{
				imageShowLabel_mode2[0]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_mode2[1]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				ui.remind_mode2->setText("");
				/*if (step > 2 * temp - frameRate)
				{
					imageShowLabel_mode2[2]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}*/

				if (step == 2 * temp - 2)
				{
					//~ 提示休息10s
					ui.remind_mode2->setText("Rest 10s");
				}
				else if (step == 2 * temp - 1)
				{
					_sleep(10000);
				}
			}
			else if ((step >= 2 * temp) && (step < 3 * temp))
			{
				imageShowLabel_mode2[1]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_mode2[2]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				ui.remind_mode2->setText("");
				/*if (step > 3 * temp - frameRate)
				{
					imageShowLabel_mode2[3]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}*/

				if (step == 3 * temp - 2)
				{
					//~ 提示休息10s
					ui.remind_mode2->setText("Rest 10s");
				}
				else if (step == 3 * temp - 1)
				{
					_sleep(10000);
				}
			}
			else if ((step >= 3 * temp) && (step < 4 * temp))
			{
				imageShowLabel_mode2[2]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_mode2[3]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				ui.remind_mode2->setText("");
				/*if (step > 4 * temp - frameRate)
				{
					imageShowLabel_mode2[4]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}*/

				if (step == 4 * temp - 2)
				{
					//~ 提示休息10s
					ui.remind_mode2->setText("Rest 10s");
				}
				else if (step == 4 * temp - 1)
				{
					_sleep(10000);
				}
			}
			else if ((step >= 4 * temp) && (step < 5 * temp))
			{
				imageShowLabel_mode2[3]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_mode2[4]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				ui.remind_mode2->setText("");
				/*if (step > 5 * temp - frameRate)
				{
					imageShowLabel_mode2[5]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}*/

				if (step == 5 * temp - 2)
				{
					//~ 提示休息10s
					ui.remind_mode2->setText("Rest 10s");
				}
				else if (step == 5 * temp - 1)
				{
					_sleep(10000);
				}
			}
			else if ((step >= 5 * temp) && (step < 6 * temp))
			{
				if (step == 6 * temp - 1)
				{
					//~ 提示休息10s
					ui.remind_mode2->setText("Rest 10s");
				}

				imageShowLabel_mode2[4]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_mode2[5]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				ui.remind_mode2->setText("");
				
			}




			if (PacketNumber_mode2 >= frameRate * holdTime_mode2 * trialNum_mode2 * classNum_mode2)
			{
				imageShowLabel_mode2[5]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");

				movie_wrist_mode2->stop();

				ui.mode2_train->setEnabled(true);
				//释放存储指针
				for (int i = 0; i < 8; i++)
				{
					fclose(stream[i]);
				}

				save_flag = false;

				if (glo->datagloveused)
				{//~ 停止数据手套的数据采集
					outfile_dataglove.close();//~ 关闭存储数据的文件。否则会导致内存溢出，MATLAB引擎调用也会失败
				}

				if (xsens_is_ready)
				{
					//~ 停止xsens_all采集数据的线程
					xsensthread_flag = false;
					//outfile_xsens_all.close();

					////~ 停止xsens数据采集
					//outfile_xsens.close();

					fclose(xsens_data);
					fclose(xsens_data_all);

				}

				if (Delsys_On_flag)
				{
					delsys_sdk_thread->stop_read_delsys();//~ 停止读取delsys数据的线程					
				}

				ui.remind_mode2->setText("");

				myPlayer->threadFree();
			}
			else if (step == 0)
			{
				_sleep(10000);
				ui.remind_mode2->setText("");
			}

		}
	}
}
/*************************************** mode-2（结束） ***************************************************************/


/**************************************** 读取delsys数据，C++ SDK（开始） **********************************************/
void QCplot::on_Delsys_On_clicked()
{//~ 使用delsys的SDK，不是连接其他GUI，在c++中开一个线程采集数据
	if (!Delsys_On_flag)
	{
		int delsys_ready_flag = delsys_sdk_thread->Init_delsys_socket();//~ 初始化，根据IP地址连接到delsys sever
		if (delsys_ready_flag)
		{
			Delsys_On_flag = true;
			ui.Delsys_On->setText("Delsys_Off");
			delsys_sdk_thread->start();
		}
		
	}
	else
	{
		Delsys_On_flag = false;
		ui.Delsys_On->setText("Delsys_On");
		delsys_sdk_thread->close_delsys_socket();//~ 关闭delsys连接
	}

}


/**************************************** 读取delsys数据，C++ SDK（结束） **********************************************/

// 串口
void QCplot::serialPortInit()
{
	ui.start_serialPort_button->setEnabled(true);
	ui.stop_serialPort_button->setEnabled(false);
	getExistingSerialPort();
	baudMap.insert(pair<int, QSerialPort::BaudRate>(1200, QSerialPort::Baud1200));
	baudMap.insert(pair<int, QSerialPort::BaudRate>(2400, QSerialPort::Baud2400));
	baudMap.insert(pair<int, QSerialPort::BaudRate>(4800, QSerialPort::Baud4800));
	baudMap.insert(pair<int, QSerialPort::BaudRate>(9600, QSerialPort::Baud9600));
	baudMap.insert(pair<int, QSerialPort::BaudRate>(19200, QSerialPort::Baud19200));
	baudMap.insert(pair<int, QSerialPort::BaudRate>(38400, QSerialPort::Baud38400));
	baudMap.insert(pair<int, QSerialPort::BaudRate>(57600, QSerialPort::Baud57600));
	baudMap.insert(pair < int, QSerialPort::BaudRate>(115200, QSerialPort::Baud115200));

	parityMap.insert(pair<std::string, QSerialPort::Parity>("None", QSerialPort::NoParity));
	parityMap.insert(pair<std::string, QSerialPort::Parity>("Odd", QSerialPort::OddParity));
	parityMap.insert(pair<std::string, QSerialPort::Parity>("Even", QSerialPort::EvenParity));

	dataBitsMap.insert(pair<int, QSerialPort::DataBits>(5, QSerialPort::Data5));
	dataBitsMap.insert(pair<int, QSerialPort::DataBits>(6, QSerialPort::Data6));
	dataBitsMap.insert(pair<int, QSerialPort::DataBits>(7, QSerialPort::Data7));
	dataBitsMap.insert(pair<int, QSerialPort::DataBits>(8, QSerialPort::Data8));

	stopBitsMap.insert(pair<float, QSerialPort::StopBits>(1.0, QSerialPort::OneStop));
	stopBitsMap.insert(pair<float, QSerialPort::StopBits>(1.5, QSerialPort::OneAndHalfStop));
	stopBitsMap.insert(pair<float, QSerialPort::StopBits>(2.0, QSerialPort::TwoStop));

	baudHash.insert(1200, QSerialPort::Baud1200);
	baudHash.insert(2400, QSerialPort::Baud2400);
	baudHash.insert(4800, QSerialPort::Baud4800);
	baudHash.insert(9600, QSerialPort::Baud9600);
	baudHash.insert(19200, QSerialPort::Baud19200);
	baudHash.insert(38400, QSerialPort::Baud38400);
	baudHash.insert(57600, QSerialPort::Baud57600);
	baudHash.insert(115200, QSerialPort::Baud115200);
	isReceiveHex = ui.hex_checkBox->isChecked();

	IMUDataPackage.resize(4);
	for (int i = 0; i < 4; i++)
	{
		IMUDataPackage[i].resize(3);
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			IMUDataPackage[i][j].resize(1000);
		}
	}
	IMU_0_Roll.resize(100);
	IMU_0_Pitch.resize(100);
	IMU_0_Yaw.resize(100);

	IMU_1_Roll.resize(100);
	IMU_1_Pitch.resize(100);
	IMU_1_Yaw.resize(100);

	IMU_2_Roll.resize(100);
	IMU_2_Pitch.resize(100);
	IMU_2_Yaw.resize(100);

	IMU_3_Roll.resize(100);
	IMU_3_Pitch.resize(100);
	IMU_3_Yaw.resize(100);

	//connect(serialPort, SIGNAL(readyRead()), this, SLOT(readData()));
	connect(ui.hex_checkBox, SIGNAL(toggled(bool)), this, SLOT(receiveChangeState(bool)));
	connect(serialPort, SIGNAL(readyRead()), this, SLOT(dispalySerialData()));
	//connect(serialPort, SIGNAL(readyRead()), this, SLOT(updateIMUImage()));

	key_IMU_1 = 0;
	key_IMU_2 = 0;
	key_IMU_3 = 0;
	key_IMU_4 = 0;
}
void QCplot::emgSerialPortInit()
{
	ui.emg_start_serialPort_button->setEnabled(true);
	ui.emg_stop_serialPort_button->setEnabled(false);
	getEmgExistingSerialPort();
	connect(ui.emg_hex_checkBox, SIGNAL(toggled(bool)), this, SLOT(receiveChangeState(bool)));
	//connect(emgSerialPort, SIGNAL(readyRead()), this, SLOT(displayEMGSerialData()));
	connect(emgSerialPort, SIGNAL(readyRead()), this, SLOT(updateEMGImage()));
	QObject::connect(emgSerialPort, SIGNAL(readyRead()), this, SLOT(updateDisGetureDataImage()));
	//connect(emgSerialPort, SIGNAL(readyRead()), this, SLOT(updatePlayerUI()));
	// A mode
	QObject::connect(wifi_Thread->wifi_tcpClient, SIGNAL(readyRead()), this, SLOT(updateAmodeImage()));
	//QObject::connect(wifi_Thread->wifi_tcpClient, SIGNAL(readyRead()), this, SLOT(updateDisGestureAmodeImage()));
	key_EMG_1 = 0;
	key_EMG_1 = 0;
	showCount = 0;
	//ui.emg_clear_button
	//connect(&refreshTime, SIGNAL(timeout()), this, SLOT(updateEMGImage()));
	//connect(emgSerialPort, SIGNAL(updateEMGSignal()), this, SLOT(updateEMGImage()));
	refreshTime.start(0);
}
void QCplot::getExistingSerialPort()
{
	for (int i = 0; i < ui.serialPort_comboBox->count(); i++)
	{
		ui.serialPort_comboBox->removeItem(i);
	}
	serialList = serialPortInfo->availablePorts();
	int serialNum = serialList.length();
	for (int i = 0; i < serialNum; i++)
	{
		QString serialName = serialList[i].portName();
		ui.serialPort_comboBox->addItem(serialName);
	}
}
void QCplot::getEmgExistingSerialPort()
{
	ui.emg_serialPort_comboBox->setEnabled(true);
	qDebug() << ui.emg_serialPort_comboBox->count() << endl;
	for (int i = 0; i < ui.emg_serialPort_comboBox->count(); i++)
	{
		ui.emg_serialPort_comboBox->removeItem(i);
	}
	emgSerialList = emgSerialPortInfo->availablePorts();
	int emgSerialNum = emgSerialList.length();
	qDebug() << emgSerialNum << endl;
	for (int i = 0; i < emgSerialNum; i++)
	{
		QString emgSerialName = emgSerialList[i].portName();
		ui.emg_serialPort_comboBox->addItem(emgSerialName);
	}
}
void QCplot::on_start_serialPort_button_clicked()
{
	if (serialPort == NULL)
	{
		ui.receiveWindow->append(QString::fromLocal8Bit("SerialPort Error") + "\n");
	}
	this->setPortName();
	bool modeSet = serialPort->open(QIODevice::ReadWrite);
	bool baudSet = this->setBaudRate();
	bool dataBitsSet = this->setDataBits();
	bool paritySet = this->setParity();
	bool stopBitsSet = this->setStopBits();
	if (modeSet && baudSet && dataBitsSet && paritySet && stopBitsSet)
	{
		emit serialPortConnected();
		ui.start_serialPort_button->setEnabled(false);
		ui.stop_serialPort_button->setEnabled(true);
	}
	IMU_Thread->start();
}
void QCplot::on_emg_start_serialPort_button_clicked()
{
	if (emgSerialPort == NULL)
	{
		ui.emg_receiveWindow->append(QString::fromLocal8Bit("SerialPort Error"));
	}
	this->setEmgPortName();
	bool emgModeSet = emgSerialPort->open(QIODevice::ReadWrite);
	bool emgBaudSet = this->setEmgBaudRate();
	//bool emgBaudSet = this->setBaudRate();
	bool emgDataBitsSet = this->setEmgDataBits();
	bool emgParitySet = this->setEmgStopBits();
	if (emgModeSet && emgBaudSet && emgDataBitsSet && emgParitySet)
	{
		ui.emg_start_serialPort_button->setEnabled(false);
		ui.emg_stop_serialPort_button->setEnabled(true);
	}
	EMG_Thread->start();
}
void QCplot::on_stop_serialPort_button_clicked()
{
	serialPort->close();
	ui.start_serialPort_button->setEnabled(true);
	ui.stop_serialPort_button->setEnabled(false);
}
void QCplot::on_emg_stop_serialPort_button_clicked()
{
	emgSerialPort->close();
	ui.emg_start_serialPort_button->setEnabled(true);
	ui.emg_stop_serialPort_button->setEnabled(false);
}
void QCplot::setPortName()
{
	QVariant data = ui.serialPort_comboBox->currentText();
	ui.receiveWindow->append(data.toString() + "\n");
	serialPort->setPortName(data.toString());
}
void QCplot::setEmgPortName()
{
	QVariant data = ui.emg_serialPort_comboBox->currentText();
	ui.emg_receiveWindow->append(data.toString() + "\n");
	emgSerialPort->setPortName(data.toString());
}
bool QCplot::setBaudRate()
{
	QVariant data = ui.baud_comboBox->currentText();
	ui.receiveWindow->append(data.toString() + "\n");
	//QSerialPort::BaudRate baudRate = baudMap[data.toInt()];
	QSerialPort::BaudRate baudRate = baudHash[data.toInt()];
	//bool baudSet = serialPort->setBaudRate(baudRate);
	bool baudSet = serialPort->setBaudRate(data.toInt());
	if (baudSet == false)
	{
		ui.receiveWindow->append(QString::fromLocal8Bit("波特率设置失败") + "\n");
	}
	else
	{
		ui.receiveWindow->append(QString::fromLocal8Bit("波特率设置为") + data.toString() + "\n");
	}
	return baudSet;
}
bool QCplot::setEmgBaudRate()
{
	QVariant data = ui.emg_baud_comboBox->currentText();
	ui.emg_receiveWindow->append(data.toString() + "\n");
	bool baudSet = emgSerialPort->setBaudRate(data.toInt());
	if (baudSet == false)
		ui.emg_receiveWindow->append(QString::fromLocal8Bit("波特率失败") + "\n");
	else
		ui.emg_receiveWindow->append(QString::fromLocal8Bit("波特率设置为") + data.toString() + "\n");
	return baudSet;
}
bool QCplot::setDataBits()
{
	QVariant data = ui.dataBits_comboBox->currentText();
	QSerialPort::DataBits dataBits = dataBitsMap[data.toInt()];
	bool dataBitsSet = serialPort->setDataBits(dataBits);
	if (dataBitsSet == false)
	{
		ui.receiveWindow->append(QString::fromLocal8Bit("数据位设置失败") + "\n");
	}
	else
	{
		ui.receiveWindow->append(QString::fromLocal8Bit("数据位设置为") + data.toString() + "\n");
	}
	return dataBitsSet;
}
bool QCplot::setEmgDataBits()
{
	QVariant data = ui.emg_dataBits_comboBox->currentText();
	QSerialPort::DataBits emgDataBits = dataBitsMap[data.toInt()];
	qDebug() << emgDataBits << endl;
	bool emgDataBitsSet = emgSerialPort->setDataBits(emgDataBits);
	if (emgDataBitsSet == false)
		ui.emg_receiveWindow->append(QString::fromLocal8Bit("数据位设置失败") + "\n");
	else
		ui.emg_receiveWindow->append(QString::fromLocal8Bit("数据为设置为") + data.toString() + "\n");
	return emgDataBitsSet;
}
bool QCplot::setParity()
{
	QVariant data = ui.verify_comboBox->currentText();
	QSerialPort::Parity parity = parityMap[data.toString().toStdString()];
	bool paritySet = serialPort->setParity(parity);
	if (paritySet == false)
	{
		ui.receiveWindow->append(QString::fromLocal8Bit("奇偶校验位设置失败") + "\n");
	}
	else
	{
		ui.receiveWindow->append(QString::fromLocal8Bit("奇偶校验位设置为") + data.toString() + "\n");
	}
	return paritySet;
}
bool QCplot::setEmgParity()
{
	QVariant data = ui.emg_verify_comboBox->currentText();
	QSerialPort::Parity parity = parityMap[data.toString().toStdString()];
	bool emgParitySet = emgSerialPort->setParity(parity);
	if (emgParitySet == false)
		ui.emg_receiveWindow->append(QString::fromLocal8Bit("奇偶校验设置位失败") + "\n");
	else
		ui.emg_receiveWindow->append(QString::fromLocal8Bit("奇偶校验位设置为") + data.toString() + "\n");
	return emgParitySet;
}
bool QCplot::setStopBits()
{
	QVariant data = ui.stopBit_comboBox->currentText();
	QSerialPort::StopBits stopBits = stopBitsMap[data.toFloat()];
	bool stopBitsSet = serialPort->setStopBits(stopBits);
	if (stopBitsSet == false)
	{
		ui.receiveWindow->append(QString::fromLocal8Bit("停止位设置失败") + "\n");
	}
	else
	{
		ui.receiveWindow->append(QString::fromLocal8Bit("停止位设置为") + data.toString() + "\n");
	}
	return stopBitsSet;
}
bool QCplot::setEmgStopBits()
{
	QVariant data = ui.emg_stopBit_comboBox->currentText();
	QSerialPort::StopBits stopBits = stopBitsMap[data.toFloat()];
	bool emgStopBitsSet = emgSerialPort->setStopBits(stopBits);
	if (emgStopBitsSet == false)
		ui.emg_receiveWindow->append(QString::fromLocal8Bit("停止位设置失败") + "\n");
	else
		ui.emg_receiveWindow->append(QString::fromLocal8Bit("停止位设置为") + data.toString() + "\n");
	return emgStopBitsSet;
}
void QCplot::receiveChangeState(bool checkFlag)
{
	/*if (checkFlag)
		isReceiveHex = true;
	else
		isReceiveHex = false;*/
	isReceiveHex = ui.hex_checkBox->isChecked();
	qDebug() << isReceiveHex;
}
void QCplot::readData()
{
	int16_t t = 0;
	if (serialPort->bytesAvailable() < 0)
	{
		ui.receiveWindow->append("No Data");
		return;
	}
	//qDebug() << serialPort->bytesAvailable() << endl;
	
	serialPort->setReadBufferSize(22);
	int size = serialPort->readBufferSize();
	qDebug() << size;
	//serialPort->setReadBufferSize(1000);
	QByteArray buffer = serialPort->readAll();
	QString receiveMsg;
	if (isReceiveHex)
		receiveMsg = ByteArrayToHexStr(buffer);
	else
		receiveMsg = buffer;
	//receiveMsg = ByteArrayToHexStr(buffer);
	//qDebug() << receiveMsg.length();
	qDebug() << receiveMsg;
	ui.receiveWindow->append(receiveMsg);
	
	bool ok = 1;
	QByteArray tempBuffer = buffer.toHex();
	qDebug() << tempBuffer.length();
	IMUSerialPortBuffer.clear();
	for (int i = 0; i < tempBuffer.length(); i += 2)
	{
		IMUSerialPortBuffer += tempBuffer.mid(i, 2).toInt(&ok, 16);
	}
	qDebug() << IMUSerialPortBuffer.length() << endl;
	this->dataUnpackage();
	showIMUImage();
	showIMUImage_3();
	showIMUImage_4();
}
QByteArray QCplot::HexStrToByteArray(QString str)
{
	QByteArray senDdata;
	int hexData, lowHexdata;
	int hexDataLen = 0;
	int len = str.length();
	senDdata.resize(len / 2);
	char lstr, hstr;
	for (int i = 0; i < len; )
	{
		hstr = str[i].toLatin1();
		if (hstr == ' ')
		{
			i++;
			continue;
		}
		i++;
		if (i >= len)
			break;
		lstr = str[i].toLatin1();
		hexData = ConvertHexChar(hstr);
		lowHexdata = ConvertHexChar(lstr);
		if ((hexData == 16) || (lowHexdata == 16))
			break;
		else
			hexData = hexData * 16 + lowHexdata;
		i++;
		senDdata[hexDataLen] = hexData;
		hexDataLen++;
	}
	senDdata.resize(hexDataLen);
	return senDdata;
}
char QCplot::ConvertHexChar(char ch)
{
	if ((ch >= '0') && (ch <= '9'))
		return ch - 0x30;
	else if ((ch >= 'A') && (ch <= 'F'))
		return ch - 'A' + 10;
	else if ((ch >= 'a') && (ch <= 'f'))
		return ch - 'a' + 10;
	else return (-1);
}
QString QCplot::ByteArrayToHexStr(QByteArray data)
{
	QString temp = "";
	QString hex = data.toHex();
	for (int i = 0; i < hex.length(); i = i + 2)
	{
		temp += hex.mid(i, 2) + " ";
	}
	return temp.trimmed().toUpper();
}
void QCplot::dataUnpackage()
{
	if (IMU_1_Roll.size() > 1000)
	{
		IMU_1_Roll.clear();
	}
	if (IMU_2_Roll.size() > 1000)
	{
		IMU_2_Roll.clear();
	}
	if (IMU_3_Roll.size() > 1000)
	{
		IMU_3_Roll.clear();
	}
	int16_t tempRoll = 0, tempPitch = 0, tempYaw = 0;
	unsigned char rollHigh = 0, rollLow = 0, pitchHigh = 0, pitchLow = 0, yawHigh = 0, yawLow = 0;
	int channel = 0;
	if (IMUSerialPortBuffer[0].operator == (0x88))
	{
		qDebug() << "inn" << endl;
		unsigned char sum = 0;
		for (int i = 0; i < IMUSerialPortBuffer.length(); i++)
		{
			sum += IMUSerialPortBuffer[i];
		}
		if(IMUSerialPortBuffer[1].operator== (0xA1) || IMUSerialPortBuffer[1].operator== (0xA2) || IMUSerialPortBuffer[1].operator== (0xA3) || IMUSerialPortBuffer[1].operator== (0xA4))
		//if (IMUSerialPortBuffer[IMU_Datalength - 1].operator== (sum))
		{
			qDebug() << "in" << endl;
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
				IMU_0_Roll.push_back(tempRoll);
				IMU_0_Pitch.push_back(tempPitch);
				IMU_0_Yaw.push_back(tempYaw);
				qDebug() << "imu0roll";
				qDebug() << IMU_0_Roll[3];
			}
			else if (IMUSerialPortBuffer[1].operator== (0xA2))
			{
				channel = 1;
				IMU_1_Roll.push_back(tempRoll);
				IMU_1_Pitch.push_back(tempPitch);
				IMU_1_Yaw.push_back(tempYaw);
			}
			else if (IMUSerialPortBuffer[1].operator== (0xA3))
			{
				channel = 2;
				IMU_2_Roll.push_back(tempRoll);
				IMU_2_Pitch.push_back(tempPitch);
				IMU_2_Yaw.push_back(tempYaw);
			}
			else if (IMUSerialPortBuffer[1].operator== (0xA4))
			{
				channel = 3;
				IMU_3_Roll.push_back(tempRoll);
				IMU_3_Pitch.push_back(tempPitch);
				IMU_3_Yaw.push_back(tempYaw);
			}
			qDebug() << tempRoll;
			/*IMUDataPackage[channel][0].push_back(tempRoll);
			
			IMUDataPackage[channel][1].push_back(tempPitch);
			IMUDataPackage[channel][2].push_back(tempYaw);*/
			qDebug() << "stop" << endl;
		}
	}
}
void QCplot::dispalySerialData()
{
	QString receiveMsg;
	if (isReceiveHex)
		receiveMsg = ByteArrayToHexStr(IMU_Thread->receiveBuffer);
	else
		receiveMsg = IMU_Thread->receiveBuffer;
	ui.receiveWindow->append(receiveMsg);
}
void QCplot::displayEMGSerialData()
{
	QString receiveMsg;
	if (isReceiveHex)
		receiveMsg = ByteArrayToHexStr(EMG_Thread->receiveBuffer);
	else
		receiveMsg = EMG_Thread->receiveBuffer;
	ui.emg_receiveWindow->append(receiveMsg);
}
void QCplot::updateIMUImage()
{
	this->showIMUImage();
	this->showIMUImage_2();
	this->showIMUImage_3();
}
int updateEMGCount = 0;
void QCplot::updateEMGImage()
{
	/*if (updateEMGCount > 10)
	{
		this->showEMGImage_1();
		this->showUnionIMUImage();
		updateEMGCount = 0;
	}
	++updateEMGCount;*/
	// 旧版本更新图窗方式
	//this->showEMGImage_1();
	//this->showUnionIMUImage();
	// 改进
	if (isUpdatePlot)
	{
		showEMGImage_2();
		showUnionIMUImage_2();
	}
}
void QCplot::on_emg_clear_button_clicked()
{
	//updateEMGImage();
	;
}
void QCplot::updateAmodeImage()
{
	//QDateTime currentTime = QDateTime::currentDateTime();
	//QString currentTime_str = currentTime.toString("dd_hh_mm_ss.zzz");
	//qDebug() << currentTime_str << endl;
	//qDebug() << "update A" << endl;
	if (dataTabFlag && isUpdatePlot)
	{
		//qDebug() << "*";
		std::vector<std::vector<double>> CurrentPackage;
		if (wifi_Thread->copydata_wifi(CurrentPackage))
		{
			QDateTime currentTime = QDateTime::currentDateTime();
			QString currentTime_str = currentTime.toString("dd_hh_mm_ss.zzz");
			//qDebug() << currentTime_str << endl;
			//qDebug() << "*****" << endl;
			//qDebug() << CurrentPackage[0].size();
			showAmodeImage_1(CurrentPackage);
		}
	}
}
void QCplot::on_saveData_button_clicked()
{
	//vector<double> testVec(10, 0.4);
	auto ptr = &EMG_Thread->emgBuffer->vectorArray[1];
	auto ptr_pac = &EMG_Thread->emgBuffer->packageNumArray;
	auto ptr_ipa = &EMG_Thread->imuBuffer->packageNumArray;

	auto ptr_imu = &EMG_Thread->imuBuffer->vectorArray[0][0];

	QString filePath = "E:\\2021-2022\\Upper Software\\QT_2threads_36_wifi_SPC(WiFi)_1220\\";
	QDateTime currentTime = QDateTime::currentDateTime();
	QString currentTime_str = currentTime.toString("dd_hh_mm_ss");
	//qDebug() << currentTime_str << endl;
	QFile saveFile(filePath + "EMG " + currentTime_str + " Exp.txt");
	QFile savePackageFile(filePath + "PAC " + currentTime_str + " Exp.txt");
	QFile saveIMUPackageFile(filePath + "IPA " + currentTime_str + " Exp.txt");
	
	QFile saveIMUFile(filePath + "IMU " + currentTime_str + " Exp.txt");

	if (!saveFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text) ||
		!savePackageFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text) ||
		!saveIMUPackageFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text) ||
		!saveIMUFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)) {//追加写入 添加结束符\r\n
		QMessageBox::warning(this, tr("错误"), tr("打开文件失败,数据保存失败"));
		return;
	}
	else {
		QTextStream out(&saveFile);
		QTextStream out_pac(&savePackageFile);
		QTextStream out_ipa(&saveIMUPackageFile);

		QTextStream out_imu(&saveIMUFile);

		for (auto i = ptr->begin(); i != ptr->end(); ++i)
			out << *i << endl;
		for (auto i = ptr_pac->begin(); i != ptr_pac->end(); ++i)
			out_pac << *i << endl;
		for (auto i = ptr_ipa->begin(); i != ptr_ipa->end(); ++i)
			out_ipa << *i << endl;
		for (auto i = ptr_imu->begin(); i != ptr_imu->end(); ++i)
			out_imu << *i << endl;
	}
	saveFile.close();
	savePackageFile.close();
	saveIMUPackageFile.close();
	saveIMUFile.close();
	//ui.saveData_button->setEnabled(false);
}

void QCplot::saveAmodeData_disGesture(std::vector<std::vector<double>> CurrentPackage)
{
	int channelNums = 4;
	for (int i = 0; i != channelNums; ++i)
	{
		std::vector<double> temp(CurrentPackage[i].begin() + 2, CurrentPackage[i].end() - 1);
		//qDebug() << temp.size();
		int channelNum = CurrentPackage[i][1];
		//if (!qfSaveAmodeFile[i].open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text))
		//{
		//	QMessageBox::warning(this, tr("ERROR"), tr("Fail to open file"));
		//	return;
		//}
		if (channelNum == (i + 1))
		{
			QTextStream out(&qfSaveAmodeFile[i]);
			QTextStream out_time(&qfSaveAmodeTimeFile[i]);
			QDateTime currentTime = QDateTime::currentDateTime();
			QString strCurrentTime = currentTime.toString("dd_hh_mm_ss");
			out_time << strCurrentTime << endl;
			for (auto it = temp.begin(); it != temp.end(); ++it)
			{
				out << *it << " ";
			}
			out << endl;
		}
	}
}

void QCplot::saveEMGData_disGesture()
{
	for (int i = 0; i != 4; ++i)
	{
		auto ptr = &EMG_Thread->emgBuffer->vectorArray[i];
		auto label_ptr = &emgLabel[i];
		if (!qfSaveEMGFile[i].open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text) ||
			!qfSaveEMGLabelFile[i].open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text))
		{
			QMessageBox::warning(this, tr("错误"), tr("打开文件夹失败"));
			return;
		}
		else
		{
			QTextStream out(&qfSaveEMGFile[i]);
			for (auto it = ptr->begin(); it != ptr->end(); ++it)
			{
				out << *it << endl;
			}
			qfSaveEMGFile[i].close();
			QTextStream out_label(&qfSaveEMGLabelFile[i]);
			for (auto it = label_ptr->begin(); it != label_ptr->end(); ++it)
			{
				out_label << *it << endl;
			}
			qfSaveEMGLabelFile[i].close();
		}
	}
	auto ptr_pac = &EMG_Thread->emgBuffer->packageNumArray;
	QTextStream out_pac(&qfSaveEMGPacFile);
	for (auto it = ptr_pac->begin(); it != ptr_pac->end(); ++it)
	{
		out_pac << *it << endl;
	}
	qfSaveEMGPacFile.close();
}

void QCplot::saveIMUData_disGesture()
{
	for (int i = 0; i != 4; ++i)
	{
		auto ptr = &EMG_Thread->imuBuffer->vectorArray[i][0];
		auto ptr_1 = &EMG_Thread->imuBuffer->vectorArray[i][1];
		auto ptr_2 = &EMG_Thread->imuBuffer->vectorArray[i][2];
		auto label_ptr = &imuLabel[i];
		if (!qfSaveIMUFile[i].open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text) ||
			!qfSaveIMULabelFile[i].open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text))
		{
			QMessageBox::warning(this, tr("错误"), tr("打开文件夹失败"));
			return;
		}
		else
		{
			QTextStream out(&qfSaveIMUFile[i]);
			for (auto it = ptr->begin(), it_1 = ptr_1->begin(), it_2 = ptr_2->begin();
				it != ptr->end(); ++it, ++it_1, ++it_2)
			{
				out << *it << " " << *it_1 << " " << *it_2 << endl;
			}
			qfSaveIMUFile[i].close();
			QTextStream out_label(&qfSaveIMULabelFile[i]);
			for (auto it = label_ptr->begin(); it != label_ptr->end(); ++it)
			{
				out_label << *it << endl;
			}
			qfSaveIMULabelFile[i].close();
		}
	}
	auto ptr_pac = &EMG_Thread->imuBuffer->packageNumArray;
	QTextStream out_pac(&qfSaveIMUPacFile);
	for (auto it = ptr_pac->begin(); it != ptr_pac->end(); ++it)
	{
		out_pac << *it << endl;
	}
	qfSaveIMUPacFile.close();
}


void QCplot::on_clearData_button_clicked()
{
	EMG_Thread->emgBuffer->vectorArray[1].clear();
	EMG_Thread->emgBuffer->packageNumArray.clear();
	EMG_Thread->imuBuffer->vectorArray[0][0].clear();
	//ui.clearData_button->setEnabled(false);
}

// disGesture
void QCplot::showTrainImage_disGesture()
{
	dataTabFlag = true;
	channelMap.insert("A-mode Channel 1", 1);
	channelMap.insert("A-mode Channel 2", 2);
	vector<QHBoxLayout*> imageLaoutArray;
	imageLaoutArray.push_back(ui.disGesture_layout_1);
	imageLaoutArray.push_back(ui.disGesture_layout_2);
	imageLaoutArray.push_back(ui.disGesture_layout_3);
	imageLaoutArray.push_back(ui.disGesture_layout_4);
	imageLaoutArray.push_back(ui.disGesture_layout_5);
	imageLaoutArray.push_back(ui.disGesture_layout_6);
	imageLaoutArray.push_back(ui.disGesture_layout_8);
	imageLaoutArray.push_back(ui.disGesture_layout_9);
	imageLaoutArray.push_back(ui.disGesture_layout_10);
	imageLaoutArray.push_back(ui.disGesture_layout_11);

	//reverse(imageLaoutArray.begin(), imageLaoutArray.end());
	QDir dir("gesture-disGesture");
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Name | QDir::NoSort);
	QFileInfoList list = dir.entryInfoList();
	classNum_disGesture = list.size();
	for (int i = 0; i != classNum_disGesture; ++i)
	{
		disGestureTrainOrder.push_back(i);
		QFileInfo fileInfo = list.at(i);
		String filename;
		filename.append("gesture-disGesture/");
		filename.append(fileInfo.fileName().toLatin1().data());		//由QString转到String
		disGestureName.push_back(filename);
		cv::Mat currentImage = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
		cv::Mat currentQImage;
		cv::cvtColor(currentImage, currentQImage, CV_BGR2RGB);
		QImage img = QImage((const unsigned char*)(currentQImage.data), currentQImage.cols,
			currentQImage.rows, currentQImage.step, QImage::Format_RGB888);

		disImgArr.push_back(img);

		QLabel* labelTrainImage = new QLabel(ui.centralWidget);
		QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
		//ui
		imageLaoutArray[i]->addWidget(labelTrainImage);
		labelTrainImage->setScaledContents(true);
		//labelTrainImage->resize(ui.imageWidget->size());
		labelTrainImage->setAutoFillBackground(false);
		labelTrainImage->setStyleSheet(QStringLiteral("Background-color: #000;"));
		labelTrainImage->setPixmap(QPixmap::fromImage(img));
		labelTrainImage->show();
		imageShowLabel_disGesture.push_back(labelTrainImage);
	}
	//reverse(imageShowLabel_disGesture.begin(), imageShowLabel_disGesture.end());
}
//void QCplot::show(
void QCplot::on_disGesture_start_button_clicked()
{
	// 初始化标签vector
	emgLabel.resize(4);
	imuLabel.resize(4);
	AmodeLabel.resize(4);
	if (myPlayer->isStop())
	{
		myPlayer->threadStart();
		ui.disGesture_start_button->setEnabled(false);
	}

	else
	{
		myPlayer->setStop();
	}
	ui.disGesture_train_button->setEnabled(true);
	ui.disGesture_stop_button->setEnabled(true);

	//dataTabFlag = false;

	holdTime_disGesture = parameters_window_disGesture->holdTime;
	trialNum_disGesture = parameters_window_disGesture->trialNum;
	if(parameters_window_disGesture->dirName.size() > 0)
	fatherDirName_disGesture = parameters_window_disGesture->dirName;
}

void QCplot::on_disGesture_train_button_clicked()
{
	ui.disGesture_progressBar_1->setMinimum(0);
	ui.disGesture_progressBar_1->setMaximum(frameRate * holdTime_disGesture * trialNum_disGesture * classNum_disGesture);
	ui.disGesture_progressBar_1->setValue(0);

	ui.disGesture_train_button->setEnabled(false);
	// 创建文件夹及txt文件用于存储训练数据
	QDateTime currentTime = QDateTime::currentDateTime();
	QString strCurrentTime = currentTime.toString("dd_hh_mm_ss");
	QString strTrainDataPath = fatherDirName_disGesture + "/TrainData/" + strCurrentTime;
	trainFileName = strCurrentTime;
	QDir dir;
	if (!dir.exists(strTrainDataPath))
	{
		dir.mkpath(strTrainDataPath);
	}
	for (int i = 0; i != 4; ++i)
	{
		QString tempPath = strTrainDataPath + "/Channel_" + QString::number(i, 10);
		if (!dir.exists(tempPath))
		{
			dir.mkpath(tempPath);
		}
	}
	for (int i = 0; i != 4; ++i)
	{
		QString tempPath = strTrainDataPath + "/Channel_" + QString::number(i, 10);
		qfSaveAmodeFile[i].setFileName(tempPath + "/Amode " + strCurrentTime + " Exp.txt");
		qfSaveEMGFile[i].setFileName(tempPath + "/EMG " + strCurrentTime + " Exp.txt");
		qfSaveIMUFile[i].setFileName(tempPath + "/IMU " + strCurrentTime + " Exp.txt");

		qfSaveEMGLabelFile[i].setFileName(tempPath + "/EMG_Label " + strCurrentTime + " Exp.txt");
		qfSaveIMULabelFile[i].setFileName(tempPath + "/IMU_Label " + strCurrentTime + " Exp.txt");
		
		qfSaveAmodeTimeFile[i].setFileName(tempPath + "/Amode_Time " + strCurrentTime + " Exp.txt");
		qfSaveAmodeLabelFile[i].setFileName(tempPath + "/Amode_Label " + strCurrentTime + " Exp.txt");

		qfSaveAmodeFile[i].open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
		qfSaveAmodeTimeFile[i].open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
		//qfSaveEMGFile[i].open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
		//qfSaveIMUFile[i].open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
	
		EMG_Thread->emgBuffer->vectorArray[i].clear();

		EMG_Thread->imuBuffer->vectorArray[i][0].clear();
		EMG_Thread->imuBuffer->vectorArray[i][1].clear();
		EMG_Thread->imuBuffer->vectorArray[i][2].clear();
	}
	qfSaveAmodePacFile.setFileName(strTrainDataPath + "/APAC " + strCurrentTime + " Exp.txt");
	qfSaveEMGPacFile.setFileName(strTrainDataPath + "/EPAC " + strCurrentTime + " Exp.txt");
	qfSaveIMUPacFile.setFileName(strTrainDataPath + "/IPAC " + strCurrentTime + " Exp.txt");

	qfSaveAmodePacFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
	qfSaveEMGPacFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
	qfSaveIMUPacFile.open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text);
	
	packetNum_disGesture = 0;
	emgLabel.resize(4);
	imuLabel.resize(4);
	AmodeLabel.resize(4);
	for (int i = 0; i < 4; ++i)
	{
		emgLabel[i].clear();
		imuLabel[i].clear();
		AmodeLabel[i].clear();
	}
	//myPlayer->threadTrain_disDesture();
	QObject::connect(wifi_Thread->wifi_tcpClient, SIGNAL(readyRead()), this, SLOT(getTrainFeature_disGesture_A_trigger()));
}

void QCplot::on_disGesture_stop_button_clicked()
{
	myPlayer->threadFree();
	ui.disGesture_start_button->setEnabled(true);
	ui.disGesture_stop_button->setEnabled(false);
	dataTabFlag = true;
}

void QCplot::on_disGesture_save_button_clicked()
{
	saveEMGData_disGesture();
	saveIMUData_disGesture();
}

void QCplot::on_disGesture_clear_button_clicked()
{
	;
}

void QCplot::updateDisGetureDataImage()
{
	
	// char不能直接转int，ASCII码，要减去'0'
	
	string sEMG_channel = ui.disGesture_EMG_comboBox->currentText().toStdString();
	int iEMG_channel = static_cast<int>(*(sEMG_channel.end() - 1) - '0');
	string sIMU_channel = ui.disGesture_IMU_comboBox->currentText().toStdString();
	int iIMU_channel = static_cast<int>(*(sIMU_channel.end() - 1) - '0');
	if (!ui.disGesture_start_button->isEnabled())
	{
		if (iEMG_channel <= 4 && iIMU_channel <= 4)
		{
			showDisGestureDataImage(iEMG_channel, iIMU_channel);
		}
	}
}
void QCplot::updateDisGestureAmodeImage()
{
	string sAmode_channel = ui.disGesture_Amode_comboBox->currentText().toStdString();
	int iAmode_channel = static_cast<int>(*(sAmode_channel.end() - 1) - '0');
	if (!ui.disGesture_start_button->isEnabled())
	{
		if (iAmode_channel <= 4)
		{
			showDisGestureAmodeImage(iAmode_channel);
		}
	}
}
void QCplot::getTrainFeature_disGesture()
{
	std::vector<std::vector<double>> CurrentPackage;
	if (wifi_Thread->copydata_wifi(CurrentPackage) == true)
	{
		saveAmodeData_disGesture(CurrentPackage);
	}
	packetNum_disGesture++;
	ui.disGesture_progressBar_1->setValue(packetNum_disGesture + 1);

	int step = packetNum_disGesture % (frameRate * holdTime_disGesture * classNum_disGesture);
	int temp = frameRate * holdTime_disGesture;

	if ((step >= 0) && (step < temp))
	{
		if (step == 1)
		{
			for (auto i = 0; i != 4; ++i)
			{
				emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
				imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
				AmodeLabel[i].push_back(packetNum_disGesture);
			}
		}
		imageShowLabel_disGesture[9]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
		imageShowLabel_disGesture[0]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");

		if (step > temp - frameRate)
		{
			imageShowLabel_disGesture[1]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
		}
	}
	else if ((step >= temp) && (step < 2 * temp))
	{
		if (step == temp)
		{
			for (auto i = 0; i != 4; ++i)
			{
				emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
				imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
				AmodeLabel[i].push_back(packetNum_disGesture);
			}
		}
		imageShowLabel_disGesture[0]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
		imageShowLabel_disGesture[1]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
		if (step > 2 * temp - frameRate)
		{
			imageShowLabel_disGesture[2]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
		}
	}
	else if ((step >= 2 * temp) && (step < 3 * temp))
	{
		if (step == 2 * temp)
		{
			for (auto i = 0; i != 4; ++i)
			{
				emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
				imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
				AmodeLabel[i].push_back(packetNum_disGesture);
			}
		}
		imageShowLabel_disGesture[1]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
		imageShowLabel_disGesture[2]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
		if (step > 3 * temp - frameRate)
		{
			imageShowLabel_disGesture[3]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
		}
	}
	else if ((step >= 3 * temp) && (step < 4 * temp))
	{
		if (step == 3 * temp)
		{
			for (auto i = 0; i != 4; ++i)
			{
				emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
				imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
				AmodeLabel[i].push_back(packetNum_disGesture);
			}
		}
		imageShowLabel_disGesture[2]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
		imageShowLabel_disGesture[3]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
		if (step > 4 * temp - frameRate)
		{
			imageShowLabel_disGesture[4]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
		}
	}
	else if ((step >= 4 * temp) && (step < 5 * temp))
	{
		if (step == 4 * temp)
		{
			for (auto i = 0; i != 4; ++i)
			{
				emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
				imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
				AmodeLabel[i].push_back(packetNum_disGesture);
			}
		}
		imageShowLabel_disGesture[3]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
		imageShowLabel_disGesture[4]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
		if (step > 5 * temp - frameRate)
		{
			imageShowLabel_disGesture[5]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
		}
	}
	else if ((step >= 5 * temp) && (step < 6 * temp))
	{
		if (step == 5 * temp)
		{
			for (auto i = 0; i != 4; ++i)
			{
				emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
				imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
				AmodeLabel[i].push_back(packetNum_disGesture);
			}
		}
		imageShowLabel_disGesture[4]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
		imageShowLabel_disGesture[5]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
		if (step > 6 * temp - frameRate)
		{
			imageShowLabel_disGesture[6]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
		}
	}
	else if ((step >= 6 * temp) && (step < 7 * temp))
	{
		if (step == 6 * temp)
		{
			for (auto i = 0; i != 4; ++i)
			{
				emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
				imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
				AmodeLabel[i].push_back(packetNum_disGesture);
			}
		}
		imageShowLabel_disGesture[5]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
		imageShowLabel_disGesture[6]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
		if (step > 7 * temp - frameRate)
		{
			imageShowLabel_disGesture[7]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
		}
	}
	else if ((step >= 7 * temp) && (step < 8 * temp))
	{
		if (step == 7 * temp)
		{
			for (auto i = 0; i != 4; ++i)
			{
				emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
				imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
				AmodeLabel[i].push_back(packetNum_disGesture);
			}
		}
		imageShowLabel_disGesture[6]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
		imageShowLabel_disGesture[7]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
		if (step > 8 * temp - frameRate)
		{
			imageShowLabel_disGesture[8]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
		}
	}
	else if ((step >= 8 * temp) && (step < 9 * temp))
	{
		if (step == 8 * temp)
		{
			for (auto i = 0; i != 4; ++i)
			{
				emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
				imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
				AmodeLabel[i].push_back(packetNum_disGesture);
			}
		}
		imageShowLabel_disGesture[7]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
		imageShowLabel_disGesture[8]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
		if (step > 9 * temp - frameRate)
		{
			imageShowLabel_disGesture[9]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
		}
	}
	else if ((step >= 9 * temp) && (step < 10 * temp))
	{
		if (step == 9 * temp)
		{
			for (auto i = 0; i != 4; ++i)
			{
				emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
				imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
				AmodeLabel[i].push_back(packetNum_disGesture);
			}
		}
		imageShowLabel_disGesture[8]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
		imageShowLabel_disGesture[9]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
		if (step == 10 * temp - 1)
		{
			//~ 提示休息10s
			ui.textBrowser->setText("Rest 10s");
			for (auto i = 0; i != 4; ++i)
			{
				emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
				imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
				AmodeLabel[i].push_back(packetNum_disGesture);
			}
		}
	}
	
	if (packetNum_disGesture >= frameRate * holdTime_disGesture * trialNum_disGesture * classNum_disGesture)
	{
		imageShowLabel_disGesture[9]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
		imageShowLabel_disGesture[0]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
		ui.disGesture_train_button->setEnabled(true);

		ui.textBrowser->setText("");

		for (int i = 0; i != 4; ++i)
		{
			qfSaveAmodeFile[i].close();
		}
		saveEMGData_disGesture();
		saveIMUData_disGesture();



		// kNN分类训练
		//ui.textBrowser->setText("Start Trainning!");
		//EMGTrain* train = new EMGTrain();
		//train->readData(trainFileName);
		//train->computeFeature();
		//KNN_model = train->myKNNTrain();
		//ui.textBrowser->setText("Complete!");


		myPlayer->threadFree();
		ui.disGesture_predict_button->setEnabled(true);
	}
	else if (step == 0)
	{
		_sleep(10000);
		ui.textBrowser->setText("");
	}
}

void QCplot::on_disGesture_predict_button_clicked()
{
	time_disGesturePredict = 5;
	num_disGesturePredict = 0;
	disGesturePredictTrial = 0;
	vector<int> tempOrder = disGestureTrainOrder;
	random_shuffle(tempOrder.begin(), tempOrder.end());
	for (int i = 0; i < classNum_disGesture; ++i)
	{
		disGesturePredictOrder.push_back(tempOrder[i]);
	}
	for (int i = 0; i < classNum_disGesture; ++i)
	{
		disGesturePredictOrder.push_back(tempOrder[i]);
	}
	ui.disGesture_predict_button->setEnabled(false);
	ui.disGesture_progressBar_1->close();

	for (int i = 0; i < 4; ++i)
	{
		EMG_Thread->emgBuffer->vectorArray[i].clear();
	}
	EMG_predictData.resize(4);
	timeWindow = 100;
	strideWindow = 150;
	EMGrealLabel = 0;
	EMGpredictLabel = 0;
	myPlayer->threadPredict_disDesture();
}

void QCplot::getPredictFeature_disGesture()
{
	// timeWindow = 100, strideWindow = 150;
	if (num_disGesturePredict % (frameRate * time_disGesturePredict) == 0)
	{
		EMGrealLabel = disGesturePredictOrder[disGesturePredictTrial] + 1;
		if (disGesturePredictTrial == 0)
		{
			imageShowLabel_disGesture[disGesturePredictOrder[disGesturePredictTrial++]]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(255, 0, 0);");
		}
		else
		{
			imageShowLabel_disGesture[disGesturePredictOrder[disGesturePredictTrial - 1]]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
			imageShowLabel_disGesture[disGesturePredictOrder[disGesturePredictTrial++]]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(255, 0, 0);");
		}
	}
	if (num_disGesturePredict % (frameRate * time_disGesturePredict) >= frameRate * time_disGesturePredict * 0.8)
	{
		if (disGesturePredictTrial < disGesturePredictOrder.size())
		{
			imageShowLabel_disGesture[disGesturePredictOrder[disGesturePredictTrial]]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
		}
	}
	num_disGesturePredict++;
	if (num_disGesturePredict >= frameRate * time_disGesturePredict * (2 * classNum_disGesture - 1))
	{	//去掉之前的提示框
		imageShowLabel_disGesture[disGesturePredictOrder[disGesturePredictTrial - 1]]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
		myPlayer->threadFree();
		ui.disGesture_predict_button->setEnabled(true);
	}
	// 预测
	bool predictFlag = true;
	for (int i = 0; i < 4; ++i)
	{
		if (EMG_Thread->emgBuffer->vectorArray[i].size() < strideWindow)
			predictFlag = false;
	}
	if (predictFlag)
	{
		EMG_predictData.clear();
		for (int i = 0; i < 4; ++i)
		{
			const auto& cend = EMG_Thread->emgBuffer->vectorArray[i].cend();
			EMG_predictData[i] = vector<double>(cend - strideWindow, cend);
		}
		EMGPredict* predict = new EMGPredict(EMG_predictData, KNN_model, EMGrealLabel);
		predict->computeFeature();
		int nowLabel = predict->KNNPredict(5);
		EMGpredictQue.push_back(nowLabel);
		if (EMGpredictQue.size() >= 3) EMGpredictQue.pop_front();
		bool same = true;
		for (int i = 0; i < 2; ++i)
			if (EMGpredictQue[i] != EMGpredictQue[i + 1]) same = false;
		if (same)
			EMGpredictLabel = EMGpredictQue.front();
		if (EMGpredictLabel)
		{
			showEMGpredictImg(EMGpredictLabel - 1);
		}
	}
}

void QCplot::showEMGpredictImg(int index)
{

	//QLabel* labelPredictImage = new QLabel(ui.centralWidget);
	//QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	////ui
	//ui.disGesture_layout_result->addWidget(labelPredictImage);
	//labelPredictImage->setScaledContents(true);
	////labelPredictImage->resize(ui.imageWidget->size());
	//labelPredictImage->setAutoFillBackground(false);
	//labelPredictImage->setStyleSheet(QStringLiteral("Background-color: #000;"));
	//labelPredictImage->setPixmap(QPixmap::fromImage(disImgArr[index]));
	//labelPredictImage->show();
	QWidget* p = nullptr;
	if(ui.disGesture_layout_result->itemAt(0))
		p = ui.disGesture_layout_result->itemAt(0)->widget();
	ui.disGesture_layout_result->removeWidget(p);

	QDir dir("gesture-disGesture");
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Name | QDir::NoSort);
	QFileInfoList list = dir.entryInfoList();
	int i = index;

	disGestureTrainOrder.push_back(i);
	QFileInfo fileInfo = list.at(i);
	String filename;
	filename.append("gesture-disGesture/");
	filename.append(fileInfo.fileName().toLatin1().data());		//由QString转到String
	disGestureName.push_back(filename);
	cv::Mat currentImage = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
	cv::Mat currentQImage;
	cv::cvtColor(currentImage, currentQImage, CV_BGR2RGB);
	QImage img = QImage((const unsigned char*)(currentQImage.data), currentQImage.cols,
		currentQImage.rows, currentQImage.step, QImage::Format_RGB888);

	QLabel* labelTrainImage = new QLabel(ui.centralWidget);
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	//ui
	ui.disGesture_layout_result->addWidget(labelTrainImage);
	labelTrainImage->setScaledContents(true);
	//labelTrainImage->resize(ui.imageWidget->size());
	labelTrainImage->setAutoFillBackground(false);
	labelTrainImage->setStyleSheet(QStringLiteral("Background-color: #000;"));
	labelTrainImage->setPixmap(QPixmap::fromImage(img));
	labelTrainImage->show();
	imageShowLabel_disGesture.push_back(labelTrainImage);
}
void QCplot::on_disGesture_parameters_button_clicked()
{
	parameters_window_disGesture->show();
}
void QCplot::on_multiData_start_button_clicked()
{
	isUpdatePlot = true;
	ui.multiData_start_button->setEnabled(false);
	ui.multiData_stop_button->setEnabled(true);
}
void QCplot::on_multiData_stop_button_clicked()
{
	isUpdatePlot = false;
	ui.multiData_start_button->setEnabled(true);
	ui.multiData_stop_button->setEnabled(false);
}

void QCplot::on_multiData_capture_button_clicked()
{
	saveEMGData_disGesture();
	saveIMUData_disGesture();
}

void QCplot::getTrainFeature_disGesture_A_trigger()
{
	//string sAmode_channel = ui.disGesture_Amode_comboBox->currentText().toStdString();
	//int iAmode_channel = static_cast<int>(*(sAmode_channel.end() - 1) - '0');
	//if (!ui.disGesture_start_button->isEnabled())
	//{
	//	if (iAmode_channel <= 4)
	//	{
	//		showDisGestureAmodeImage(iAmode_channel);
	//	}
	//}555  0

	Mat rawData(8, 1000, CV_32FC1);
	std::vector<std::vector<double>> CurrentPackage;
	//QDateTime currentTime = QDateTime::currentDateTime();
	//QString currentTime_str = currentTime.toString("dd_hh_mm_ss.zzz");
	//qDebug() << currentTime_str << endl;
	if (p->copydata1(CurrentPackage) == true || wifi_Thread->copydata_wifi(CurrentPackage) == true)//~ 读取A超设备传的数据
	{
		//qDebug() << "*" << endl;
		//QDateTime currentTime = QDateTime::currentDateTime();
		//QString currentTime_str = currentTime.toString("dd_hh_mm_ss.zzz");
		//qDebug() << currentTime_str << endl;

		for (int i = 0; i < CurrentPackage.size(); i++)
		{
			for (int j = 0; j < 1000; j++)

			{
				rawData.at<float>(i, j) = float(CurrentPackage[i][j]);
			}
		}
		if ((rawData.rows == 8) && (rawData.at<float>(0, 1) == 1))
		{
			//~ 训练过程中，保存A超原始数据
			Mat saveData;
			rawData.convertTo(saveData, CV_8UC1);
			//saveimage(saveData);
			saveAmodeData_disGesture(CurrentPackage);

			if (engine_is_open)
			{
				//~ 调用MATLAB引擎，对rawData（8*1000）进行滤波和特征提取等操作，输出imageFeature（1*feature_col_num）
				Mat imageFeature(1, feature_col_num, CV_64FC1);
				double temp_imageFeature[1][feature_col_num];//~ 暂存imageFeature的数据
				double mat[8][1000];//~ 暂存rawData数据
				Mat temp_mat(8, 1000, CV_64FC1);
				rawData.convertTo(temp_mat, CV_64FC1);
				memcpy(mat, temp_mat.data, 8 * 1000 * sizeof(double));//~ 将cv::Mat类型转成double类型数组
				int ans = SetMat("rawData", (void*)mat, 8, 1000, ep);//~ 将c++中的mat矩阵写入到MATLAB工作区中的mat_matlab矩阵

				engEvalString(ep, "imageFeature=getCoefficientImageFeature(rawData)");//~ 提取A超信号特征参数到imageFeature，有48（子窗数）*1（特征参数个数）*8（通道数）=384个特征参数。是1*384的矩阵
				//qDebug() << ans;
				engEvalString(ep, "imageFeature = imageFeature'");
				GetMat("imageFeature", (void*)temp_imageFeature, 1, feature_col_num, ep);//~ 将MATLAB工作区中的mat_matlab矩阵赋值到c++中的mat矩阵，覆盖掉原先mat矩阵的值
				memcpy(imageFeature.data, temp_imageFeature, 1 * feature_col_num * sizeof(double));//~ 将double类型的mat矩阵数组转成cv::Mat类型，存储到Mat A(M, M, CV_64FC1)中
				imageFeature.convertTo(imageFeature, CV_32FC1);//~ 将64位存储类型的A矩阵转成32位（float）类型
				train_feature.push_back(imageFeature);//~ push_back是按行加在mat矩阵最后一行
			}

			packetNum_disGesture++;
			ui.disGesture_progressBar_1->setValue(packetNum_disGesture + 1);

			int step = packetNum_disGesture % (frameRate * holdTime_disGesture * classNum_disGesture);
			int temp = frameRate * holdTime_disGesture;

			if ((step >= 0) && (step < temp))
			{
				if (step == 1)
				{
					for (auto i = 0; i != 4; ++i)
					{
						emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
						imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
						AmodeLabel[i].push_back(packetNum_disGesture);
					}
				}
				imageShowLabel_disGesture[9]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_disGesture[0]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");

				if (step > temp - frameRate)
				{
					imageShowLabel_disGesture[1]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= temp) && (step < 2 * temp))
			{
				if (step == temp)
				{
					for (auto i = 0; i != 4; ++i)
					{
						emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
						imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
						AmodeLabel[i].push_back(packetNum_disGesture);
					}
				}
				imageShowLabel_disGesture[0]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_disGesture[1]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step > 2 * temp - frameRate)
				{
					imageShowLabel_disGesture[2]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= 2 * temp) && (step < 3 * temp))
			{
				if (step == 2 * temp)
				{
					for (auto i = 0; i != 4; ++i)
					{
						emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
						imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
						AmodeLabel[i].push_back(packetNum_disGesture);
					}
				}
				imageShowLabel_disGesture[1]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_disGesture[2]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step > 3 * temp - frameRate)
				{
					imageShowLabel_disGesture[3]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= 3 * temp) && (step < 4 * temp))
			{
				if (step == 3 * temp)
				{
					for (auto i = 0; i != 4; ++i)
					{
						emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
						imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
						AmodeLabel[i].push_back(packetNum_disGesture);
					}
				}
				imageShowLabel_disGesture[2]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_disGesture[3]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step > 4 * temp - frameRate)
				{
					imageShowLabel_disGesture[4]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= 4 * temp) && (step < 5 * temp))
			{
				if (step == 4 * temp)
				{
					for (auto i = 0; i != 4; ++i)
					{
						emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
						imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
						AmodeLabel[i].push_back(packetNum_disGesture);
					}
				}
				imageShowLabel_disGesture[3]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_disGesture[4]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step > 5 * temp - frameRate)
				{
					imageShowLabel_disGesture[5]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= 5 * temp) && (step < 6 * temp))
			{
				if (step == 5 * temp)
				{
					for (auto i = 0; i != 4; ++i)
					{
						emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
						imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
						AmodeLabel[i].push_back(packetNum_disGesture);
					}
				}
				imageShowLabel_disGesture[4]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_disGesture[5]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step > 6 * temp - frameRate)
				{
					imageShowLabel_disGesture[6]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= 6 * temp) && (step < 7 * temp))
			{
				if (step == 6 * temp)
				{
					for (auto i = 0; i != 4; ++i)
					{
						emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
						imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
						AmodeLabel[i].push_back(packetNum_disGesture);
					}
				}
				imageShowLabel_disGesture[5]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_disGesture[6]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step > 7 * temp - frameRate)
				{
					imageShowLabel_disGesture[7]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= 7 * temp) && (step < 8 * temp))
			{
				if (step == 7 * temp)
				{
					for (auto i = 0; i != 4; ++i)
					{
						emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
						imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
						AmodeLabel[i].push_back(packetNum_disGesture);
					}
				}
				imageShowLabel_disGesture[6]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_disGesture[7]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step > 8 * temp - frameRate)
				{
					imageShowLabel_disGesture[8]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= 8 * temp) && (step < 9 * temp))
			{
				if (step == 8 * temp)
				{
					for (auto i = 0; i != 4; ++i)
					{
						emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
						imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
						AmodeLabel[i].push_back(packetNum_disGesture);
					}
				}
				imageShowLabel_disGesture[7]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_disGesture[8]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step > 9 * temp - frameRate)
				{
					imageShowLabel_disGesture[9]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(0, 128, 0);");
				}
			}
			else if ((step >= 9 * temp) && (step < 10 * temp))
			{
				if (step == 9 * temp)
				{
					for (auto i = 0; i != 4; ++i)
					{
						emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
						imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
						AmodeLabel[i].push_back(packetNum_disGesture);
					}
				}
				imageShowLabel_disGesture[8]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_disGesture[9]->setStyleSheet("border-width: 5px;border-style: solid;border-color: rgb(250, 128, 10);");
				if (step == 10 * temp - 1)
				{
					//~ 提示休息10s
					ui.textBrowser->setText("Rest 10s");
					for (auto i = 0; i != 4; ++i)
					{
						emgLabel[i].push_back((long long)EMG_Thread->emgBuffer->vectorArray[i].size() - 1);
						imuLabel[i].push_back((long long)EMG_Thread->imuBuffer->vectorArray[i][0].size() - 1);
						AmodeLabel[i].push_back(packetNum_disGesture);
					}
				}
			}

			if (packetNum_disGesture >= frameRate * holdTime_disGesture * trialNum_disGesture * classNum_disGesture)
			{
				imageShowLabel_disGesture[9]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				imageShowLabel_disGesture[0]->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 0, 0);");
				ui.disGesture_train_button->setEnabled(true);

				ui.textBrowser->setText("");

				saveEMGData_disGesture();
				saveIMUData_disGesture();

				for (int i = 0; i != 4; ++i)
				{
					auto label_ptr = &AmodeLabel[i];
					if (!qfSaveAmodeLabelFile[i].open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text))
					{
						QMessageBox::warning(this, tr("Error"), tr("Open failed"));
						return;
					}
					QTextStream out_label(&qfSaveAmodeLabelFile[i]);
					for (auto it = label_ptr->begin(); it != label_ptr->end(); ++it)
					{
						out_label << *it << endl;
					}
					qfSaveAmodeLabelFile[i].close();

					qfSaveAmodeFile[i].close();
					qfSaveAmodeTimeFile[i].close();
				}
				

				myPlayer->threadFree();
				ui.disGesture_predict_button->setEnabled(true);
				QObject::disconnect(wifi_Thread->wifi_tcpClient, SIGNAL(readyRead()), this, SLOT(getTrainFeature_disGesture_A_trigger()));
			}
			else if (step == 0)
			{
				_sleep(10000);
				ui.textBrowser->setText("");
			}
		}
	}
}