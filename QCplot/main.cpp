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
	//	w->close_engine();//~ 在上位机软件关闭时，也把MATLAB计算引擎关闭
	//}
	
	delete w;
	delete myPlayer;
	return 0;
}

