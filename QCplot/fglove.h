/*--------------------------------------------------------------------------*/
// fglove.h
//
// 5DT Data Glove driver SDK
// Version 2.1
//
// Copyright (C) 2000-2005, 5DT <Fifth Dimension Technologies>
// http://www.5dt.com/
/*--------------------------------------------------------------------------*/

//#pragma once
#ifndef DATA_GLOVE_FGLOVE_H_
#define DATA_GLOVE_FGLOVE_H_

/*--------------------------------------------------------------------------*/


#include <string>

#ifdef WIN32
#define FGLOVEAPI __declspec( dllexport )
#else
#define FGLOVEAPI
#endif

#ifndef LPVOIDn             //ԭ����LPVOID����ͷ�ļ��й�������
#define LPVOIDn void*
#endif

//#ifndef SOCKET
//#define SOCKET unsigned int
//#endif
//
//#ifndef HANDLE
//#define HANDLE void*
//#endif

// Uncomment the debug level you want -
#define USER_DEBUG 0	// level 0 = user debug
#define DEV_DEBUG 1 	// level 1 = development debug

#define DEBUG_LEVEL USER_DEBUG
//#define DEBUG_LEVEL DEV_DEBUG

#define VENDOR_ID_5DT 0x5d70

// Glove type / capability defines for DG14 Ultra
#define DG14U_R			0x00
#define DG14U_L			0x01
#define DG14U_RW		0x02
#define DG14U_LW		0x03
#define DG14U_RW_SLAVE	0x06
#define DG14U_LW_SLAVE	0x07
#define DG5U_R			0x10
#define DG5U_L			0x11
#define DG5U_RW			0x12
#define DG5U_LW			0x13
#define DG5U_RW_SLAVE	0x16
#define DG5U_LW_SLAVE	0x17
#define DG14U_INFO		0x80		// Mask for info packet
#define MASK_WIRELESS	0x02

enum EfdRecvState
{
	STATE_REQUESTNONE,			// No requests (used only at init)
	STATE_REQUESTSETUP,			// Request for a glove initial setup packet
	STATE_REQUESTDATA,			// A data packet request state.
	STATE_REQUESTDISCONNECT,	// Request for disconnect
};

/*--------------------------------------------------------------------------*/
//--8<--- SNIP END

enum EfdGloveHand
{
	FD_HAND_LEFT,   // left-handed glove
	FD_HAND_RIGHT   // right-handed glove
};

enum EfdGloveTypes
{
	FD_GLOVENONE,   // no glove
	FD_GLOVE5U,		// DG5 Ultra serial
	FD_GLOVE5UW,	// DG5 Ultra serial, wireless
	FD_GLOVE5U_USB,	// DG5 Ultra USB
	FD_GLOVE7,      // 7-sensor
	FD_GLOVE7W,     // 7-sensor, wireless
	FD_GLOVE16,     // 16-sensor
	FD_GLOVE16W,    // 16-sensor, wireless
	FD_GLOVE14U,	// DG14 Ultra serial
	FD_GLOVE14UW,	// DG14 Ultra serial, wireless
	FD_GLOVE14U_USB	// DG14 Ultra USB
};

enum EfdSensors
{
	FD_THUMBNEAR=0,
	FD_THUMBFAR,
	FD_THUMBINDEX,
	FD_INDEXNEAR,
	FD_INDEXFAR,
	FD_INDEXMIDDLE,
	FD_MIDDLENEAR,
	FD_MIDDLEFAR,
	FD_MIDDLERING,
	FD_RINGNEAR,
	FD_RINGFAR,
	FD_RINGLITTLE,
	FD_LITTLENEAR,
	FD_LITTLEFAR,
	FD_THUMBPALM,
	FD_WRISTBEND,
	FD_PITCH,
	FD_ROLL
};

/*--------------------------------------------------------------------------*/

typedef struct
{
	// The contents of this struct are platform-dependent and subject to
	// change. You should not manipulate the contents of this struct directly.

	/*New stuff*/
//	HWND			m_hWnd;
//	HDEVNOTIFY		m_hDevNotify;

} fdGlove;

/*--------------------------------------------------------------------------*/

FGLOVEAPI fdGlove *fdOpen(char *pPort, bool bOnlyAllowSingleConnection = false);	
FGLOVEAPI int   fdClose(fdGlove *pFG);
FGLOVEAPI int   fdGetGloveHand(fdGlove *pFG);
FGLOVEAPI int   fdGetGloveType(fdGlove *pFG);
FGLOVEAPI int   fdGetNumSensors(fdGlove *pFG);
FGLOVEAPI void  fdGetSensorRawAll(fdGlove *pFG, unsigned short *pData);
FGLOVEAPI unsigned short fdGetSensorRaw(fdGlove *pFG, int nSensor);
FGLOVEAPI void  fdSetSensorRawAll(fdGlove *pFG, unsigned short *pData);
FGLOVEAPI void  fdSetSensorRaw(fdGlove *pFG, int nSensor, unsigned short nRaw);
FGLOVEAPI void  fdGetSensorScaledAll(fdGlove *pFG, float *pData);
FGLOVEAPI float fdGetSensorScaled(fdGlove *pFG, int nSensor);
FGLOVEAPI int   fdGetNumGestures(fdGlove *pFG);
FGLOVEAPI int   fdGetGesture(fdGlove *pFG);
FGLOVEAPI void  fdGetCalibrationAll(fdGlove *pFG, unsigned short *pUpper, unsigned short *pLower);
FGLOVEAPI void  fdGetCalibration(fdGlove *pFG, int nSensor, unsigned short *pUpper, unsigned short *pLower);
FGLOVEAPI void  fdSetCalibrationAll(fdGlove *pFG, unsigned short *pUpper, unsigned short *pLower);
FGLOVEAPI void  fdSetCalibration(fdGlove *pFG, int nSensor, unsigned short nUpper, unsigned short nLower);
FGLOVEAPI void  fdResetCalibration(fdGlove *pFG, int nSensor);
FGLOVEAPI void  fdResetCalibration(fdGlove *pFG);
FGLOVEAPI void  fdGetSensorMaxAll(fdGlove *pFG, float *pMax);
FGLOVEAPI float fdGetSensorMax(fdGlove *pFG, int nSensor);
FGLOVEAPI void  fdSetSensorMaxAll(fdGlove *pFG, float *pMax);
FGLOVEAPI void  fdSetSensorMax(fdGlove *pFG, int nSensor, float fMax);
FGLOVEAPI void  fdGetThresholdAll(fdGlove *pFG, float *pUpper, float *pLower);
FGLOVEAPI void  fdGetThreshold(fdGlove *pFG, int nSensor, float *pUpper, float *pLower);
FGLOVEAPI void  fdSetThresholdAll(fdGlove *pFG, float *pUpper, float *pLower);
FGLOVEAPI void  fdSetThreshold(fdGlove *pFG, int nSensor, float fUpper, float fLower);
FGLOVEAPI void  fdGetGloveInfo(fdGlove *pFG, unsigned char *pData);
FGLOVEAPI void  fdGetDriverInfo(fdGlove *pFG, unsigned char *pData);
FGLOVEAPI void	fdSetCallback(fdGlove *pFG,void *pFunc, LPVOIDn param);
FGLOVEAPI void	fdRemoveCallback(fdGlove *pFG);
FGLOVEAPI int	fdGetPacketRate(fdGlove *pFG);
FGLOVEAPI bool	fdNewData(fdGlove *pFG);
FGLOVEAPI int	fdGetFWVersionMajor(fdGlove *pFG);
FGLOVEAPI int	fdGetFWVersionMinor(fdGlove *pFG);
FGLOVEAPI bool	fdGetAutoCalibrate(fdGlove *pFG);
FGLOVEAPI bool	fdSetAutoCalibrate(fdGlove *pFG, bool bAutoCalibrate);
FGLOVEAPI bool	fdSaveCalibration(fdGlove *pFG, const char *pFileName);
FGLOVEAPI bool	fdLoadCalibration(fdGlove *pFG, const char *pFileName);
FGLOVEAPI int	fdScanUSB(unsigned short *aPID, int &nNumMax);
FGLOVEAPI int	fdScanUSB();

#endif //DATA_GLOVE_FGLOVE_H_
