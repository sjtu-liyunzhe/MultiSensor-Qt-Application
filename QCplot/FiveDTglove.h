//�����������ݲɼ�
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
	//�洢������������
	void getGloveData();
	//�����������ײ���ָ��
	fdGlove* getDataGlovePtr();
	//dataGlove
	fdGlove *pGlove;

	bool datagloveused;
private:
	//�ڹ��캯���д��������ף�������ָ���pGlove
	std::ofstream outfile;
};


#endif // _FiveDTglove_H_
