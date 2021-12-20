/* 
 * Copyright (C) 2014 杭州交浦科技有限公司(Elonxi Ltd)
 *
 * Author:  Xingchen Yang
 * Email:   support@elonxi.cn
 * Website: www.elonxi.com 
 *
 * This program comes with ABSOLUTELY NO WARRANTY 
 */
#pragma once;

//#define WIN32_LEAN_AND_MEAN
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <process.h>
#include <windows.h>
#include<vector>
#include <queue>
#include "pcap.h"
//输入输出流
#include <iostream>
#include <fstream> 

class ELOXI_US_API
{
private:
public:
	ELOXI_US_API();
	~ELOXI_US_API();
	//int startcapture();
	bool opendevice();
	//unsigned int __stdcall capturethread(PVOID pM);
	bool copydata(std::vector<std::vector<double>> &x);
	bool copydata1(std::vector<std::vector<double>> &x);
};