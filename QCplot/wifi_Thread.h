#ifndef THREAD_TWO_H
#define THREAD_TWO_H
///*WIFI����ӵ�ͷ�ļ�*////
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
	bool thread_two::copydata_wifi(std::vector<std::vector<double>>& x);//~ ��Ҫ����public�£�������protected�У����߳��޷�����
	void thread_two::wifi_stopThread();
	QTcpSocket* wifi_tcpClient;
protected:
    void run();//���߳�ִ�е����ݽ�ͨ�����¸��麯��ʵ��
	

private:
	/////*WIFI����ӵı���*/////
	bool wifi_isopen;
	QString Ipaddress;
	quint16 portvalue;
	/*QTcpSocket *wifi_tcpClient;*/
	QByteArray buffer;
	std::vector<double> Data_tmp;          //~ ��������з�װ����������count������֪����Ԫ���������ҿ��Բ�Ԥ�ȶ��������С
	std::vector<std::vector<double>> wifi_Package;//~ �ݴ��������ݰ�8*1000
	std::vector<std::vector<double>> WIFI_DATA;//~ �洢���մ������߳��н��д�������ݰ�8*1000��
	int channelCount;
	bool channelOneFlag;
	bool wifi_stop_flag;

	
signals:
    
public slots:
	void onReadyRead();
	
};

#endif 



