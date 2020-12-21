// provide live pcap stats or canned data to extractd uds client

#include "extractor.h"
#include "socklib.h"

#include <pcap.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/un.h>
#include <errno.h>

   
int get_packets(int connfd, int limit)
{
    char *dev, errbuf[PCAP_ERRBUF_SIZE];

    /* find default device */
    dev = pcap_lookupdev(errbuf);
    if ( dev == NULL ) {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
        strcpy(dev, "wlan0");
    }
    printf("using device: %s\n", dev);

    /* open device */
    pcap_t *handle;  // session handle 
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);

    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return 2;

    }
    printf("device: %s opened!\n", dev);

    /* packet capture */
    struct pcap_pkthdr header;	// the header that pcap gives us 
    const u_char *packet;       // the actual packet 
    char buffer[1024];
    int i = 0;

    do { 
        packet = pcap_next(handle, &header); // single packet
        printf("captured a packet with length of [%d]\n", header.len);
        sprintf(buffer, "captured a packet with length of [%d]\n", header.len);
        send(connfd, buffer, header.len, 0);
    } while( limit > i++ ); 

    /* close session */
    pcap_close(handle);
}
   
void extraction(char *uds_path, int livedata) 
{
   int listenfd;

   /* get unix listening socket */
   if ( SOCKET_ERROR == ( listenfd = setup_unix_server(uds_path) ) ) {
      perror("failure setting up unix server socket");
      exit(errno); 
   }

   struct     sockaddr_un unixaddr;
   int        connfd;
   socklen_t  socklen;

   socklen = sizeof(unixaddr);
   if ( 0 > ( connfd = accept(listenfd, (struct sockaddr *) &unixaddr, 
                                                            &socklen) ) ) {
      perror("unix socket connection error");
   }
   printf("\n unix socket connection accepted");

   if ( livedata ) {  // write live pcap stats to uds socket
       int limit = 10; 
       get_packets(connfd, limit);
   } else {           // read and write data file to uds socket 
       char filename[256];
       strcpy(filename, "/tmp/extraction.data");  // (for now, put under /tmp)
       int locfd;

       printf("\n extractor_controller writing to uds: %s \n", uds_path);

       FILE *file = fopen(filename, "r");
       char buffer[1024];
       while ( !feof(file) ) {
          int rval = fread(buffer, 1, sizeof(buffer), file);
          send(connfd, buffer, rval, 0);
       }
   }
}
