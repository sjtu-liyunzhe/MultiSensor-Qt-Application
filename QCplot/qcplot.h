#ifndef QCPLOT_H
#define QCPLOT_H

#include <QtWidgets/QMainWindow>
#include <iostream>
#include <fstream>
#include <stdio.h> 
#include <Player.h>
#include "LDA_Bayesian.h"
#include "ui_qcplot.h"
//opencv
#include <ml.h>
#include <time.h>
#include <contrib/contrib.hpp>
#include <cxcore.hpp>
#include <core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <algorithm>
#include <vector>
#include <opencv2/opencv.hpp>
//virtual hand control
#include "SerialPort.h"
#include <map>
#include <queue>
//ultrasound API
#include"ElonxiUSDevice.h"
//five DT dataglove
#include "FiveDTglove.h"
//TCP
#include <Winsock2.h>
#include <WS2tcpip.h>
//~ MATLAB
#include "engine.h" //~ 该文件包含了matlab计算引擎API函数的说明和所需数据结构的定义


#pragma comment(lib, "ws2_32.lib")

#define ORDER 2

//~ wifi通讯相关
#include "wifi_Thread.h"

//~ xsens
//#include "xsens.hpp"
#include <xsensdeviceapi.h> // The Xsens device API header
#include "conio.h"			// For non ANSI _kbhit() and _getch()

#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <utility>
#include <fstream>
#include <xsens/xsmutex.h>

//~ delsys sdk
#include "Delsys_SDK.h"


class QCplot : public QMainWindow //,public Ui_QCplotClass
{
	Q_OBJECT		// moc

private:
	char fileName[8][100];
	FILE *stream[8];
	QVector<double>Data_x,Data_y;
	bool flag;
	bool start_save; 
	int classNum;
	int frameRate;
	int holdTime;
	int trialNum;
	Mat train_feature;
	Mat train_feature_new;
	Mat train_label;
	

	Mat rawData;
	//normailization
	Mat trainFeatureNormalize;
	Mat maxCol;
	Mat minCol;
	Mat averageCol;
	Mat predict_feature;
	Mat predict_label_svm;
	Mat predict_label;
	Mat predict_real_label;
	Mat predictFeatureNormalize; 
	vector<int> trainTrialOrder;
	vector<int> predictTrialOrder;
	vector<int> predictTrialOrderNew;
	vector<String> gestureName;
	vector<String> gesturePath;
	vector<QLabel*> imageShowLabel;
	int predictTrial;
	PCA* pca;
	CvSVM SVMmodel;
	float predictAccurancy;
	int packetNumber;	
	LDA_Bayesian *lda;
	Player* myPlayer;
	ELOXI_US_API *p;
	//calibration
	bool capture;
	bool calibration;
	bool save_flag;
	Mat calibrationImage;
	//para *paraDialog;
	//virtual hand control
	CSerialPort* mCOMPort;
	int nCOM;
	std::map<String, unsigned char> gestureMap;
	std::queue<int> gestureQueue;
	bool gestureOrder;
	//load gesture images
	QString image_dir;
	//five DT data glove
	FiveDTglove* glo;
	std::ofstream outfile;
	std::ofstream outfile_dataglove;
	
	char savedata[100];
	char save_dataglove[100];

	
	//LR coefficient
	Mat beta_pc1;
	Mat beta_pc2;
	float predict_Handangle;//~ 最终要输出的手部张合角度
	float predict_Wristangle;//~ 腕部角度的中间过程
	float wristangle;//~ 最终要输出的腕部旋转角度
	
	bool Handangle_flag;
	bool wristangle_flag;


	float filteredPC1;
	//plot prediction results
	bool predict_angle_plot;
	double mxBuf[ORDER+1],myBuf[ORDER+1],mzBuf[ORDER+1];
	double mxBufWrist[ORDER+1],myBufWrist[ORDER+1],mzBufWrist[ORDER+1];
	double mmaxMinPC1[2];
	double mmaxMinHand[2];
	double mmaxMinWrist[2];
	double boundWrist[2], boundHand[2];
	bool updateTime;
	// TCP
	SOCKET msockClient;
	void Init_Socket();
	// TAC
	void TACGestureControl(float, float);
	int predictNum;
	int CurrentTask;
	int SPCTasks[14];
	int DwellingTime;
	Mat projectedTrain;
	bool TAC_is_ready;

	//~ wifi
	thread_two* wifi_Thread;

	QMovie* movie_hand;
	QMovie* movie_wrist;
	QMovie* movie_simultaneous;
	int eachMotion;

	bool predict_flag;//~ 决定当前是否要进行预测
	void predict_gesture();

	int count_num;

	//~ mode1
	void showTrainImage_mode1();
	int PacketNumber_mode1;
	int	trialNum_mode1;
	vector<QLabel*> imageShowLabel_mode1;
	int channelindex_mode1;
	int classNum_mode1;

	//~ mode2
	int channelindex_mode2;
	void showTrainImage_mode2();
	int	trialNum_mode2;
	int classNum_mode2;
	vector<QLabel*> imageShowLabel_mode2;
	int PacketNumber_mode2;
	int holdTime_mode2;
	QMovie* movie_wrist_mode2;

	//~ mode3 SPC
	int session_SPC;

	//~ delsys sdk	
	bool Delsys_On_flag;
	Delsys_read_Thread* delsys_sdk_thread;

	//~ xsens
	char savedata_xsens[100];
	char savedata_xsens_all[100];
	char save_xsens[100];
	char save_xsens_all[100];
	
public:
	void initial();
	void gettime();
	void initialSlots();	
	void showimage1(std::vector<std::vector<double>>);
	void saveimage(Mat);
	void showTrainImage();
	void trainModel();
	void trainModelC();
	void columnNormalize(Mat& src, Mat& des,Mat& maxCol,Mat& minCol,Mat& averageCol);
	void writeMatToFile(cv::Mat& m, const char* filename);
	void savePredict();
	void calAccuracy();
	QCplot(QWidget *parent = 0);
	~QCplot();
	// 参数设置结构体
	struct parametersSettings 
	{
		QString featureType;
		int channelNumber;
		int windowLength;
		int overlap;
	};
	void setupRealTimeAngle();
	void showRealTimeAngle(float, float);
	void MaxMinPC1Wrist(Mat );
	void MaxMinHand(Mat );
	void MaxMinWrist(Mat);
	double RealTimeButterworth(double x, double SOS[ORDER][6], double Gain[ORDER], double input[ORDER+1], double  inter[ORDER+1], double output[ORDER+1]);
	void BoundUpdate(double data, double bound[2], double coeff);
	Mat GaussianKernel(Mat, Mat, double);

	//~ matlab交互
	void GetMat(char* VarName, void* mat, int M, int N, Engine* engine);
	int SetMat(char* VarName, void* mat, int M, int N, Engine* engine);
	Mat pc1new_label;
	Mat pc2new_label;
	Engine* ep;  //定义matlab引擎指针
	void close_engine();
	bool engine_is_open;

		

	
private:
	parametersSettings currentParametersSettings;
	Ui::QCplotClass ui;   //namespace

private slots:
	void on_start_clicked();		// 这些clicked槽函数由click事件自动触发，QT内部已经connect了
	void on_stop_clicked();
	void on_save_clicked();
	void on_train_clicked();
	void on_predict_clicked();
	void on_accuracy_clicked();
	void on_capture_clicked();
	void on_calibration_clicked();
	void updatePlayerUI();
	void getTrainFeature(QImage);
	void getTrainFeature1(QImage);	
	void getTrainFeatureC();
	void getPredictFeature(QImage);
	void getPredictFeatureC();
	//void on_actionParameter_triggered();
	void on_save_ectype_clicked();
	void on_start_ectype_clicked();
	void on_train_ectype_clicked();
	void on_predict_ectype_clicked();

	//~ 以太网模块相关
	void on_EthernetMode_clicked();

	//~* WiFi模块相关 *~//
	void on_WifiMode_clicked();

	void on_wifi_connect_clicked();

	//~ TAC 虚拟假肢手
	void on_TAC_Connect_clicked();

	//~ 与MATLAB交互
	void on_Train_Model_clicked();
	//void on_Read_Label_clicked();
	void on_Run_matlab_clicked();

	//~ xsens相关
	void on_Xsens_Connect_clicked();

	//~ mode-1
	void on_mode1_start_clicked();
	void on_mode1_train_clicked();
	void getTrainFeature_mode1();

	//~ mode-2
	void on_mode2_start_clicked();
	void on_mode2_train_clicked();
	void getTrainFeature_mode2();

	//~ delsys相关
	void on_Delsys_On_clicked();


};

#endif // QCPLOT_H
