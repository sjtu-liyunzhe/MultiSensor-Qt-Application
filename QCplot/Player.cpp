#include "Player.h"
#include "GlobalData.h"

Player::Player(QObject *parent)
	: QThread(parent)
{
	stop = true;
}

//���������ݲ��Ǻܶ�����STACKOVERFLOW
//���������ģ�ò��Ҫ�ڶ��̵߳�ʱ������
//�������о�
Player::~Player()
{
	mutex.lock();
	stop = true;
	condition.wakeOne();				//����һ���߳�
	mutex.unlock();
	wait();
}

// ���ⲿ��ť�����������߳�
void Player::threadStart()
{
	if (!isRunning())
	{
		if (isStop())
		{
			stop = false;
			pstate = FREE;
		}
		start(LowPriority);
	}
}

void Player::setStop()
{
	stop = true;
}


bool Player::isStop() const
{
	return this->stop;
}

void Player::run()
{
	while(!stop)
	{																			//����һ����־��������ѭ�����˳�

		while (1)
		{
			if (pstate == FREE)
			{
				emit showImage();									
			}																
			else if (pstate == TRAIN)
			{
				emit trainImage();
			} 
			else if (pstate == TRAIN_mode1)
			{
				emit trainImage_mode1();
			}
			else if (pstate == TRAIN_mode2)
			{
				emit trainImage_mode2();
			}
			else if (pstate == TRAIN_DisGesture)
			{
				emit trainImage_disDesture();
			}
			else if (pstate == PREDICT)
			{
				emit predictImage();
			}
			else if (pstate == PREDICT_DisGesture)
			{
				emit predictImage_disDesture();
			}
			//this->msleep(100);		// Ϊ����Ӧ����ִ����õ�ʱ��
			this->msleep(10);
			if (stop)
			{
				break;
			}
		}

	}
}

void Player::threadPredict()
{
	 //Predict
	pstate = PREDICT;
}

void Player::threadTrain()
{
	//Train
	pstate = TRAIN; 
}

void Player::threadTrain_mode1()
{
	//Train
	pstate = TRAIN_mode1;
}

void Player::threadTrain_mode2()
{
	//Train
	pstate = TRAIN_mode2;
}

void Player::threadTrain_disDesture()
{
	pstate = TRAIN_DisGesture;
}

void Player::threadPredict_disDesture()
{
	pstate = PREDICT_DisGesture;
}

Player::processstate Player::getState()  
{
	return pstate;
}

void Player::threadFree()
{
	//Free
	pstate = FREE; 
}
