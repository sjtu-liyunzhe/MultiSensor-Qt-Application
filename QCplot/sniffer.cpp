
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "pcap.h"  

using namespace std;
#define LINE_LEN 16
int count=0;// 11.16 debug
/* prototype of the packet handler */
//void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

void packet_handler(u_char *dumpfile, const struct pcap_pkthdr *header, const u_char *pkt_data);
int openNet(); // 11.21debug
void main()
{
	openNet();
}
//int main(int argc, _TCHAR* argv[]) 
int openNet()
{
	
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int inum;
	int i=0;
	int res;
	pcap_t *adhandle;
	pcap_dumper_t *dumpfile;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
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
	scanf("%d", &inum);
	
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
	if ((adhandle= pcap_open_live(d->name,	// name of the device
							 65536,			// portion of the packet to capture. 
											// 65536 grants that the whole packet will be captured on all the MACs.
							 1,				// promiscuous mode (nonzero means promiscuous)
							 1000,			// read timeout
							 errbuf			// error buffer
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
	
	// open a file
	dumpfile= pcap_dump_open(adhandle, "H:\\debug.txt");

	if (dumpfile == NULL)
	{
		fprintf(stderr,"\nError opening output file\n");
		cout << pcap_geterr(adhandle);
		pcap_close(adhandle);
		return -5;
	}

	pcap_loop(adhandle, 0, packet_handler, (unsigned char *)dumpfile);
	pcap_close(adhandle);
	pcap_dump_close(dumpfile);
	return 0;
}


/* Callback function invoked by libpcap for every incoming packet */
void packet_handler(u_char *dumpfile, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
    /* save the packet on the dump file */
	if(header->caplen==1000)
	{	
		printf("%.2x ", pkt_data[1]);
		count++;
		if(count%8==0)
			printf("\n");
		register FILE*stream;
		stream=fopen("data.txt","ab+");
		fwrite(pkt_data,header->caplen,1,stream);
		fclose(stream);
	}
}




