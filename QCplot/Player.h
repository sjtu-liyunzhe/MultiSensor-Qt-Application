#ifndef PLAYER_H
#define PLAYER_H
#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <iostream>
#include <fstream>
#include<time.h>

#include <QTimer>
#include <QDateTime>
#include <QDebug>

// �����豸ʹ��״̬�����߳�
// ���ݲ�ͬ״ִ̬�в�ͬ�׶εĳ���
class Player : public QThread
{
	Q_OBJECT

	
public:
	Player(QObject *parent = 0);
	~Player();
	void threadStart();														//�������ô���״̬
	void threadTrain();
	void threadPredict();
	void threadFree();
	void setStop();														
	bool isStop() const;
	enum processstate {
		TRAIN,
		PREDICT,
		FREE,
		REST,
		TRAIN_mode1,
		TRAIN_mode2,
		TRAIN_DisGesture,
		PREDICT_DisGesture
	};
	processstate pstate;
	processstate getState(); 

	//~ mode1
	void threadTrain_mode1();
	//~ mode2
	void threadTrain_mode2();
	// disGesture
	void threadTrain_disDesture();
	void threadPredict_disDesture();

private:
	bool stop;
	QMutex mutex;
	QWaitCondition condition;

signals:
																				//���������źţ��ڲ�ͬ����׶θ����������ź�
	void showImage();
	void trainImage();
	void predictImage();

	//~ mode1
	void trainImage_mode1();

	//~ mode2
	void trainImage_mode2();

	// disGesture
	void trainImage_disDesture();
	void predictImage_disDesture();

protected:
	void run();
	
};

#endif // PLAYER_H
