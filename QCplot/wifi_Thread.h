#ifndef THREAD_TWO_H
#define THREAD_TWO_H
///*WIFI新添加的头文件*////
#include <qtcpsocket.h>
#include <qabstractsocket.h>
#include <qhostaddress.h>
#include <QThread>
#include <queue>


#ifndef _AFXDLL 
#define _AFXDLL             
#endif

class thread_two : public QThread
{
    Q_OBJECT

public:
    explicit thread_two(QObject* parent = nullptr);
	bool wifi_connect();
	bool thread_two::copydata_wifi(std::vector<std::vector<double>>& x);//~ 需要放在public下；若放在protected中，主线程无法访问
	void thread_two::wifi_stopThread();
	QTcpSocket* wifi_tcpClient;
protected:
    void run();//多线程执行的内容将通过重新该虚函数实现
	

private:
	/////*WIFI新添加的变量*/////
	bool wifi_isopen;
	QString Ipaddress;
	quint16 portvalue;
	/*QTcpSocket *wifi_tcpClient;*/
	QByteArray buffer;
	std::vector<double> Data_tmp;          //~ 对数组进行封装，可以利用count函数获知数组元素量，并且可以不预先定义数组大小
	std::vector<std::vector<double>> wifi_Package;//~ 暂存整合数据包8*1000
	std::vector<std::vector<double>> WIFI_DATA;//~ 存储最终传到主线程中进行处理的数据包8*1000；
	int channelCount;
	bool channelOneFlag;
	bool wifi_stop_flag;

	
signals:
    
public slots:
	void onReadyRead();
	
};

#endif 



