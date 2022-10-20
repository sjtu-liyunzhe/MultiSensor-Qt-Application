#include "Player.h"
#include "GlobalData.h"

Player::Player(QObject *parent)
	: QThread(parent)
{
	stop = true;
}

//这里面内容不是很懂，在STACKOVERFLOW
//上拿下来的，貌似要在多线程的时候有用
//后续再研究
Player::~Player()
{
	mutex.lock();
	stop = true;
	condition.wakeOne();				//唤醒一个线程
	mutex.unlock();
	wait();
}

// 由外部按钮触发，启动线程
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
	{																			//设置一个标志来控制死循环的退出

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
			//this->msleep(100);		// 为了适应肌电分窗设置的时间
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
