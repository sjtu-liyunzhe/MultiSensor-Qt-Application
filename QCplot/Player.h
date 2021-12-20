#ifndef PLAYER_H
#define PLAYER_H
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <iostream>
#include <fstream>
#include<time.h>

/*******************************************
此类是一个线程，用于从B超设备中实时地
提取图像，并且将图像实时发送给后续处理
环节
********************************************/
class Player : public QThread
{
	Q_OBJECT

	
public:
	Player(QObject *parent = 0);
	~Player();
	void threadStart();														//用于设置处理状态
	void threadTrain();
	void threadPredict();
	void threadFree();
	void setStop();														
	bool isStop() const;
	enum processstate {
		TRAIN, PREDICT, FREE, REST, TRAIN_mode1, TRAIN_mode2
	};
	processstate pstate;
	processstate getState(); 

	//~ mode1
	void threadTrain_mode1();
	//~ mode2
	void threadTrain_mode2();

private:
	bool stop;
	QMutex mutex;
	QWaitCondition condition;

signals:
																				//定义三个信号，在不同处理阶段给主程序发送信号
	void showImage();
	void trainImage();
	void predictImage();

	//~ mode1
	void trainImage_mode1();

	//~ mode2
	void trainImage_mode2();

protected:
	void run();
	
};

#endif // PLAYER_H
