//数据手套数据采集
#ifndef	_FiveDTglove_H_
#define _FiveDTglove_H_

#include "fglove.h"
#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <windows.h> // for Sleep
#else
#include <unistd.h>  // for usleep
#endif
#include <iostream>
#include <fstream>

class FiveDTglove
{
public:
	FiveDTglove();
	FiveDTglove(const FiveDTglove& _DG);
	~FiveDTglove();
	//存储数据手套数据
	void getGloveData();
	//返回数据手套操作指针
	fdGlove* getDataGlovePtr();
	//dataGlove
	fdGlove *pGlove;

	bool datagloveused;
private:
	//在构造函数中打开数据手套，并返回指针给pGlove
	std::ofstream outfile;
};


#endif // _FiveDTglove_H_
