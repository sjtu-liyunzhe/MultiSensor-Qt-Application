#include "Player.h"
#include <QtWidgets/QApplication>
#include "GlobalData.h"
#include "qcplot.h"


int GlobalData::count2=0;
int GlobalData::state = 0;
Player *myPlayer;

int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
	myPlayer = new Player();
	QCplot *w;
	w = new QCplot();
	w->show();
	a.exec();
	//if (w->engine_isopen == true)
	//{
	//	std::cout << "close engine" << std::endl;
	//	w->close_engine();//~ ����λ������ر�ʱ��Ҳ��MATLAB��������ر�
	//}
	
	delete w;
	delete myPlayer;
	return 0;
}

