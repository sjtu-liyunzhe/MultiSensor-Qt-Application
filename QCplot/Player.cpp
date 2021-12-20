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
			else if (pstate == PREDICT)
			{
				emit predictImage();
			}
			this->msleep(80);														
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

Player::processstate Player::getState()  
{
	return pstate;
}

void Player::threadFree()
{
	//Free
	pstate = FREE; 
}
