/**
 * tcpnitslib.c
 * 
 * syvertsj
 */ 

#include "common.h"
#include "tcpnitslib.h"

#include <unistd.h>
#include <strings.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BACKLOG 5
#define PUBSERV_UP 1
#define PUBSERV_DOWN 0

static int __publisher_service = PUBSERV_DOWN;
static int __listenfd; 


/* return socket file descriptor if success, or error otherwise 
 */
int setup_subscriber(struct in_addr *pub_host, int pub_port)
{
   int sockfd;
   int attempt; 
   struct sockaddr_in servaddr;

   /* open client socket file descriptor */
   if ( 0 > (sockfd = socket(AF_INET, SOCK_STREAM, 0)) ) {
      perror("error opening tcp subscriber socket");
   }
   printf("\n tcp setup_subscriber: client socket created ");

   /* initialize server address */
   bzero(&servaddr, sizeof(servaddr));                 /* initialize address string to zeros */
   servaddr.sin_family      = AF_INET;                 /* set address family */
   //servaddr.sin_addr.s_addr = htonl(pub_host->s_addr); /* host-to-network byte order */
   servaddr.sin_addr.s_addr = pub_host->s_addr;
   //servaddr.sin_addr.s_addr = INADDR_ANY;
   servaddr.sin_port        = htons(pub_port);

   printf("\n tcp setup_subscriber: client address initialized");

   /* attempt to connect to publisher listening on pub_port|pub_host (5 max) */
   for ( attempt = 0; attempt < 5; attempt++ ) {

      printf("\n subscriber connection attempt: %d", attempt);

      /* try to connect to listening socket */
      if ( 0 > connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) ) {
         sleep(1);
      } else {
         printf("\n tcp subscriber connected");
         return sockfd;
      }
   }
   printf("\n tcp setup_subscriber socket error");
   
   return NITS_SOCKET_ERROR;
}

/* set up listening socket on localhost and port
 */
int setup_publisher(int pub_port)
{
   struct sockaddr_in servaddr;

   /* create tcp listening socket */
   if ( 0 > ( __listenfd = socket(AF_INET, SOCK_STREAM, 0)) ) {
      perror("error opening tcp publisher socket");
   }
   printf("\n tcp setup_publisher: server listening socket created");

   /* initialize server address */
   bzero(&servaddr, sizeof(servaddr));            /* initialize address string to zeros */
   servaddr.sin_family      = AF_INET;            /* set address family */
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  /* use any interfaces */
   servaddr.sin_port        = htons(pub_port);    /* use known port */
   //
   printf("\n tcp setup_publisher: server address initialized");

   /* bind listening socket file descriptor to address */
   if ( 0 > ( bind(__listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) ) {
      perror("error binding publisher socket");
      return NITS_SOCKET_ERROR;
   }
   printf("\n tcp setup_publisher: listening socket bound");

   /* listen */
   if ( 0 > ( listen(__listenfd, BACKLOG)) ) {
      perror("error socket");
      return NITS_SOCKET_ERROR;
   }
   printf("\n tcp setup_publisher: server listening socket ready");

   __publisher_service = PUBSERV_UP;

   return NITS_SOCKET_OK;
}

/* accept connection from subscriber 
 * return socket file descriptor to connected tcp/ip stream socket
 */
int get_next_subscriber()
{
   struct     sockaddr_in cliaddr;
   int        connfd;
   socklen_t  clilen;

   /* verify publisher service */
   if ( PUBSERV_DOWN == __publisher_service ) { 
      return NITS_SOCKET_ERROR; 
   }
   printf("\n tcp get_next_subscriber: publisher service is not down");

   /* set up connection using existing listening socket fd */
   clilen = sizeof(cliaddr);
   if ( 0 > ( connfd = accept(__listenfd, (struct sockaddr *) &cliaddr, 
                                                              &clilen) ) ) {
      perror("tcp connection error");
      return NITS_SOCKET_ERROR; 
   }
   printf("\n tcp get_next_subscriber: connection accepted");

   return connfd;
}

