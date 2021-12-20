#include "FiveDTglove.h"

FiveDTglove::FiveDTglove()
{
	pGlove = NULL;
	//outfile.open("Mat_file//ScaledData.txt");
}

FiveDTglove::FiveDTglove(const FiveDTglove& _DG)
{
	pGlove = _DG.pGlove;
}

FiveDTglove::~FiveDTglove()
{
	if (pGlove != NULL)
	{
		fdClose(pGlove);
	}
	//outfile.close();
}

fdGlove* FiveDTglove::getDataGlovePtr()
{
	char    *szPort    = NULL;
	const int PortToOpenSize = 6;
	char	szPortToOpen[PortToOpenSize];
	fdGlove *pGlove    = NULL;
	bool     showraw   = false;
	int      glovetype = FD_GLOVENONE;

	if (!szPort)
	{
#ifdef WIN32
		szPort = "USB";
#else
		szPort = "/dev/fglove";
#endif
	}
	strcpy_s(szPortToOpen, PortToOpenSize, szPort);
	if (strcmp(szPort,"USB") == 0)
	{
		unsigned short aPID[5];
		int nNumFound = 5;
		int nChosen = 0;
		fdScanUSB(aPID,nNumFound);
		for (int c = 0; c < nNumFound; c++)
		{
			printf("Available USB Gloves:\n");
			printf("%i - ",c);
			switch (aPID[c])
			{
			case DG14U_R:
				printf("Data Glove 14 Ultra Right\n");
				break;
			case DG14U_L:
				printf("Data Glove 14 Ultra Left\n");
				break;
			case DG5U_R:
				printf("Data Glove 5 Ultra Right\n");
				break;
			case DG5U_L:
				printf("Data Glove 5 Ultra Left\n");
				break;
			default:
				printf("Unknown\n");
			}
		}
		printf("Please enter the number to select Data Glove:");
		//scanf_s("%i",&nChosen,2); // 输入缓冲区大小为2字节，第二个字节用于存放'\0'
		nChosen=0;
		sprintf_s(szPortToOpen, PortToOpenSize,"USB%i",nChosen);
		fdOpen(szPortToOpen);
	}

	// Initialize glove
	printf( "Attempting to open glove on %s .. \n", szPortToOpen );
	if (NULL == (pGlove = fdOpen( szPortToOpen )))
	{
		printf( "failed.\n" );

		datagloveused = false;

		//return -1;
		return NULL;  //原来的函数返回整型，现在返回指针，所以如此修改
	}

	printf( "+-------------------------------+\n" );
	printf( "+ Data glove linking succeeded. +\n" );
	printf( "+-------------------------------+\n" );

	char *szType = "?";
	glovetype = fdGetGloveType(pGlove);
	switch (glovetype)
	{
	case FD_GLOVENONE: szType = "None"; break;
	case FD_GLOVE7:    szType = "Glove7"; break;
	case FD_GLOVE7W:   szType = "Glove7W"; break;
	case FD_GLOVE16:   szType = "Glove16"; break;
	case FD_GLOVE16W:  szType = "Glove16W"; break;
	case FD_GLOVE5U: szType = "DG5 Ultra serial"; break;
	case FD_GLOVE5UW: szType = "DG5 Ultra serial, wireless"; break;
	case FD_GLOVE5U_USB: szType = "DG5 Ultra USB"; break;
	case FD_GLOVE14U: szType = "DG14 Ultra serial"; break;
	case FD_GLOVE14UW: szType = "DG14 Ultra serial, wireless"; break;
	case FD_GLOVE14U_USB: szType = "DG14 Ultra USB"; break;
	}
	printf( "glove type: %s\n", szType );
	printf( "glove handedness: %s\n", fdGetGloveHand(pGlove)==FD_HAND_RIGHT?"Right":"Left" );
	int iNumSensors = fdGetNumSensors(pGlove);
	printf( "glove num sensors: %d\n", iNumSensors );
	//fdSetAutoCalibrate(pGlove, true);
	datagloveused = true;
	return pGlove;
}

void FiveDTglove::getGloveData()
{
	//outfile<< fdGetSensorScaled(pGlove,FD_THUMBNEAR) << " "<< fdGetSensorScaled(pGlove,FD_INDEXNEAR) << " "
	//	<< fdGetSensorScaled(pGlove,FD_MIDDLENEAR) << " "<< fdGetSensorScaled(pGlove,FD_RINGNEAR) << " "<<fdGetSensorScaled(pGlove,FD_LITTLENEAR)<< " " <<std::endl;
}