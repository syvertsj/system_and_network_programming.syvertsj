
#include "socklib.h"

#include <unistd.h>
#include <strings.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define BACKLOG 5


/* set up listening socket on localhost and port
 */
int setup_tcp_server(int pub_port)
{
   struct sockaddr_in servaddr;
   int listenfd;

   /* create tcp listening socket */
   if ( 0 > ( listenfd = socket(AF_INET, SOCK_STREAM, 0)) ) {
      perror("error opening tcp listener socket");
      return SOCKET_ERROR;
   }

   /* initialize server address */
   bzero(&servaddr, sizeof(servaddr));            /* initialize address string to zeros */
   servaddr.sin_family      = AF_INET;            /* set address family */
   servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);  /* use loopback */
   servaddr.sin_port        = htons(pub_port);    /* use known port */

   /* bind listening socket file descriptor to address */
   if ( 0 > ( bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) ) {
      perror("error binding publisher socket");
      return SOCKET_ERROR;
   }

   /* listen */
   if ( 0 > ( listen(listenfd, BACKLOG)) ) {
      perror("error socket");
      return SOCKET_ERROR;
   }
   printf("\n server listening socket ready");

   return listenfd;
}


/* return socket file descriptor if success, or error otherwise 
 */
int setup_tcp_client(int pub_port)
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
   servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);         /* use loopback of localhost */
   servaddr.sin_port        = htons(pub_port);
   //
   printf("\n tcp setup_subscriber: client address initialized");

   /* attempt to connect to publisher listening on pub_port|pub_host (5 max) */
   for ( attempt = 0; attempt < 5; attempt++ ) {

      printf("\n subscriber connection attempt: %d", attempt);

      /* try to connect to listening socket */
      if ( 0 > connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) ) {
         sleep(1);
      } else {
         //
         printf("\n subscriber connected");
         return sockfd;
      }
   }
   printf("\n setup_subscriber socket error");
   
   return SOCKET_ERROR;
}

/* set up unix socket on localhost 
 */
int setup_unix_server(char *uds_path)
{
   struct sockaddr_un  servaddr; /* (path on local filesystem) */
   int listenfd;

   /* create unix socket */
   if ( 0 > ( listenfd = socket(AF_UNIX, SOCK_STREAM, 0)) ) {
      perror("error opening unix socket");
      return SOCKET_ERROR;
   }

   /* initialize server address */
   unlink(uds_path);                      /* remove old name and socket */
   bzero(&servaddr, sizeof(servaddr));
   servaddr.sun_family = AF_UNIX;
   strcpy(servaddr.sun_path, uds_path);
   //
   printf("\n unix server address initialized");

   /* bind listening socket file descriptor to address */
   if ( 0 > ( bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) ) {
      perror("error binding publisher socket");
      return SOCKET_ERROR;
   }
   printf("\n unix listening socket bound");

   /* listen */
   if ( 0 > ( listen(listenfd, BACKLOG)) ) {
      perror("error socket");
      return SOCKET_ERROR;
   }
   printf("\n unix listening socket ready");

   return listenfd;
}

/* return socket file descriptor if success, or error otherwise 
 */
int setup_unix_client(char *uds_path) 
{
   int sockfd;
   struct sockaddr_un  servaddr; /* (path on local filesystem) */
   int attempt; 

   /* open client socket file descriptor */
   if ( 0 > (sockfd = socket(AF_UNIX, SOCK_STREAM, 0) ) ) { /* AF_UNIX or AF_LOCAL */
      perror("error opening unix subscriber socket");
      return SOCKET_ERROR;
   }

   /* initialize server address */
   bzero(&servaddr, sizeof(servaddr));                 
   servaddr.sun_family = AF_UNIX;  
   strcpy(servaddr.sun_path, uds_path);

   printf("\n unix client address initialized");

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

   return SOCKET_ERROR;
}
