/**
 * unixnitslib.c
 * 
 * syvertsj
 */ 

#include "common.h"
#include "unixnitslib.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>

#define BACKLOG 5
#define PUBSERV_UP 1
#define PUBSERV_DOWN 0

static int __publisher_service = PUBSERV_DOWN;
static int __listenfd; 


/* return socket file descriptor if success, or error otherwise 
 */
int setup_subscriber(char *server_path) 
{
   int sockfd;
   struct sockaddr_un  servaddr; /* (path on local filesystem) */
   int attempt; 

   /* open client socket file descriptor */
   if ( 0 > (sockfd = socket(AF_UNIX, SOCK_STREAM, 0) ) ) { /* AF_UNIX or AF_LOCAL */
      perror("error opening unix subscriber socket");
   }

   /* initialize server address */
   //unlink(server_path);                                /* remove old name and socket */
   bzero(&servaddr, sizeof(servaddr));                 /* initialize address string to zeros */
   servaddr.sun_family = AF_UNIX;  /* (or AF_LOCAL) */
   strcpy(servaddr.sun_path, server_path);

   printf("\n unix setup_subscriber: client address initialized");

   /* attempt to connect to publisher (5 max) */
   for ( attempt = 0; attempt < 5; attempt++ ) {

      printf("\n subscriber connection attempt: %d", attempt);

      /* try to connect to unix socket */
      if ( 0 > connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) ) {
         sleep(1);
      } else {
         printf("\n unix subscriber connected");
         return sockfd;
      }
   }
   printf("\n unix setup_subscriber socket error");

   return NITS_SOCKET_ERROR;
}

/* set up listening socket on localhost, associated with the path name
 */
int setup_publisher(char *server_path) 
{
   struct sockaddr_un  servaddr; /* (path on local filesystem) */

   /* create tcp listening socket */
   if ( 0 > ( __listenfd = socket(AF_UNIX, SOCK_STREAM, 0)) ) {
      perror("error opening unix publisher socket");
   }

   /* initialize server address */
   unlink(server_path);                                /* remove old name and socket */
   bzero(&servaddr, sizeof(servaddr));
   servaddr.sun_family = AF_UNIX;
   strcpy(servaddr.sun_path, server_path);

   /* bind listening socket file descriptor to address */
   if ( 0 > ( bind(__listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) ) {
      perror("error binding publisher socket");
      return NITS_SOCKET_ERROR;
   }
   printf("\n unix setup_publisher: listening socket bound");

   /* listen */
   if ( 0 > ( listen(__listenfd, BACKLOG)) ) {
      perror("error socket");
      return NITS_SOCKET_ERROR;
   }
   printf("\n unix setup_publisher: server listening socket ready");

   __publisher_service = PUBSERV_UP;

   return NITS_SOCKET_OK;
}

/* accept connection from subscriber and return socket file descriptor to tcp/ip stream socket
 */
int get_next_subscriber()
{
   struct     sockaddr_un cliaddr;
   int        connfd;
   socklen_t  clilen;

   /* verify publisher service */
   if ( PUBSERV_DOWN == __publisher_service ) { 
      return NITS_SOCKET_ERROR; 
   }
   printf("\n unix get_next_subscriber: publisher service is not down");

   /* set up connection using existing listening socket fd */
   clilen = sizeof(cliaddr);
   if ( 0 > ( connfd = accept(__listenfd, (struct sockaddr *) &cliaddr, 
                                                              &clilen) ) ) {
      perror("unix connection error");
      return NITS_SOCKET_ERROR; 
   }
   printf("\n unix get_next_subscriber: connection accepted");

   return connfd;
}

