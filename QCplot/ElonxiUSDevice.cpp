#include "ElonxiUSDevice.h"
//function prototype
void packet_handler(u_char *dumpfile, const struct pcap_pkthdr *header, const u_char *pkt_data);

// global variables
std::vector<std::vector<double>> OnePackage;		// ��ά���飬��8��ͨ��������
std::queue<std::vector<std::vector<double>>> US_DATA;
std::vector<std::vector<double>> US_DATA1;
//�����ٽ籣��������
CRITICAL_SECTION g_cs;

int startcapture()
{
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int inum;
	int i=0;
	pcap_t *adhandle;
	pcap_dumper_t *dumpfile;
	char errbuf[PCAP_ERRBUF_SIZE]; 
	/* Retrieve the device list */
	if(pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}

	/* Print the list */
	for(d=alldevs; d; d=d->next)
	{
		printf("%d. %s", ++i, d->name);
		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}

	if(i==0)
	{
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return -1;
	}

	printf("Enter the interface number (1-%d):",i);
	//choose netcard number
	scanf_s("%d", &inum);
	//fix netcard number
	//inum =1;
	if(inum < 1 || inum > i)
	{
		printf("\nInterface number out of range.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	/* Jump to the selected adapter */
	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);
	/* Open the device */
	/* Open the adapter */
	if ((adhandle= pcap_open_live(d->name,		// name of the device
		65536,									// portion of the packet to capture. 
		// 65536 grants that the whole packet will be captured on all the MACs.
		1,										// promiscuous mode (nonzero means promiscuous)
		1000,									// read timeout
		errbuf									// error buffer
		)) == NULL)
	{
		fprintf(stderr,"\nUnable to open the adapter. %s is not supported by WinPcap\n", d->name);
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	printf("\nlistening on %s...\n", d->description);
	/* At this point, we don't need any more the device list. Free it */
	pcap_freealldevs(alldevs);
	/*open a file*/
	dumpfile= pcap_dump_open(adhandle, "dumpfile.txt");

	if (dumpfile == NULL)
	{
		fprintf(stderr,"\nError opening output file\n");
		std::cout << pcap_geterr(adhandle);
		pcap_close(adhandle);
		return -5;
	}

	pcap_loop(adhandle, 0, packet_handler, (unsigned char *)dumpfile);
	pcap_dump_close(dumpfile);
	pcap_close(adhandle);
	return 0;
};

//global variables
int channelCount = 1;
bool channelOneFlag = false;

void packet_handler(u_char *dumpfile, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	if(header->caplen==1000)
	{	
		std::vector<double> temp(1000, 0);
		for (int i=0; i<1000; i++)
		{
			temp[i] = pkt_data[i];
		}
		double ChannelNumber = temp[1];		// ���ݰ��ڶ�����ͨ����
		// combine 8 channels
		if (ChannelNumber ==1)
		{
			OnePackage.clear();
			channelCount = 1;
			channelOneFlag = true;
			OnePackage.push_back(temp);
			channelCount++;
		}
		if(channelOneFlag)
		{
			if (ChannelNumber ==2)
			{
				if (ChannelNumber == channelCount)
				{
					OnePackage.push_back(temp);
					channelCount++;
				}
				else
				{
					channelOneFlag =false;
					channelCount =1;
				}
			}

			else if(ChannelNumber ==3)
			{
				if (ChannelNumber == channelCount)
				{
					OnePackage.push_back(temp);
					channelCount++;
				}
				else
				{
					channelOneFlag =false;
					channelCount =1;
				}
			}

			else if(ChannelNumber ==4)
			{
				if (ChannelNumber == channelCount)
				{
					OnePackage.push_back(temp);
					channelCount++;
				}
				else
				{
					channelOneFlag =false;
					channelCount =1;
				}
			}

			else if(ChannelNumber ==5)
			{
				if (ChannelNumber == channelCount)
				{
					OnePackage.push_back(temp);
					channelCount++;
				}
				else
				{
					channelOneFlag =false;
					channelCount =1;
				}
			}

			else if(ChannelNumber ==6)
			{
				if (ChannelNumber == channelCount)
				{
					OnePackage.push_back(temp);
					channelCount++;
				}
				else
				{
					channelOneFlag =false;
					channelCount =1;
				}
			}

			else if(ChannelNumber ==7)
			{
				if (ChannelNumber == channelCount)
				{
					OnePackage.push_back(temp);
					channelCount++;
				}
				else
				{
					channelOneFlag =false;
					channelCount =1;
				}
			}

			else if(ChannelNumber ==8)
			{
				if (ChannelNumber == channelCount)
				{
					channelOneFlag = false;
					channelCount = 1;
					OnePackage.push_back(temp);
				}
				else
				{
					channelOneFlag =false;
					channelCount =1;
				}
			}
		}
	}
	//Check package size
	if( OnePackage.size() == 8)
	{	
		//�����ٽ籣����
		EnterCriticalSection(&g_cs);
		//�޻���
		US_DATA1 = OnePackage;
		LeaveCriticalSection(&g_cs);
	}
}
//Ϊʲô���ܷŵ�������
unsigned int __stdcall capturethread(PVOID pM)
{
	startcapture();
	return 0;
}


bool ELOXI_US_API::opendevice()
{

	printf("create capture thread \n");
	HANDLE handle;
	handle = (HANDLE)_beginthreadex(NULL, 0, capturethread, NULL, 0, NULL);
	//��ʼ���ٽ籣��������
	InitializeCriticalSection(&g_cs);
	return true;
}

ELOXI_US_API::ELOXI_US_API()
{

}
ELOXI_US_API::~ELOXI_US_API()
{

}

// Ϊʲôû���������
bool ELOXI_US_API::copydata(std::vector<std::vector<double>> &x)
{
	if(!US_DATA.empty())
	{	
		EnterCriticalSection(&g_cs);
		x = US_DATA.front();
		US_DATA.pop();
		LeaveCriticalSection(&g_cs);
		return true;
	}
	else
	{
		return false;
	}
}

bool ELOXI_US_API::copydata1(std::vector<std::vector<double>> &x)
{
	if(US_DATA1.empty())
	{
		return false;
	}
	// ������ݰ���С
	if(US_DATA1.size() == 8)
	{	
		if (US_DATA1[7][1] = 8)		// ������ݰ���ʽ�Ƿ���ȷ
		{
			EnterCriticalSection(&g_cs);
			x = US_DATA1;
			US_DATA1.clear();
			LeaveCriticalSection(&g_cs);
			return true;
		}

	}
	else
	{
		return false;
	}
}