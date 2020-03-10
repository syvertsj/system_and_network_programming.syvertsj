/**
 * tcp_subscriber.c
 * 
 * syvertsj
 */ 

#include "common.h"
#include "tcpnitslib.h"

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
   /* process input */
   if ( (argc != 2) && (argc != 3) ) {
      fprintf (stderr, "Usage: %s [article name|QUIT|LIST] [ipv4 host num]\n", argv[0]);
      return EXIT_FAILURE;
   }

   /* convert hostname string to internet address */
   struct in_addr host;
   int locfd;

   if ( (0 != (strcmp("QUIT", argv[1]))) && (0 != strcmp("LIST", argv[1])) ) {

      if ( NULL != argv[2] ) { 
         if ( 0 == inet_aton(argv[2], &host) ) {
            perror("internet address conversion failure");
            return EXIT_FAILURE;
         }
      } else {
         if ( 0 == inet_aton("127.0.0.1", &host) ) {
            perror("internet address conversion failure");
            return EXIT_FAILURE;
         } 
      }
      printf("\n converted server string to network byte order");
   
      /* open specified file */
      if ( 0 > (locfd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0644)) ) {
         fprintf (stderr, "%s: failure opening file: %s\n", argv[0], argv[1]);
         return EXIT_FAILURE;
      }
      printf("\n file opened ");

   } 

   /* obtain subscriber socket */
   int connfd;
   if ( NITS_SOCKET_ERROR == ( connfd = setup_subscriber(&host, PORT) ) ) {
      fprintf (stderr, "%s: failure obtaining subscriber socket.\n", argv[0]);
      return EXIT_FAILURE;
   }
   printf("\n %s: setup_subscriber called ", argv[0]);

   /* request article name, LIST, or QUIT from publisher */
   if ( 0 > ( write(connfd, argv[1], strlen(argv[1])) ) ) {
      fprintf (stderr, "%s: failure obtaining subscriber socket.\n", argv[0]);
      return EXIT_FAILURE;
   }
   printf("\n %s: %s file requested \n", argv[0], argv[1]);

   /* read data from connected socket and write to file using local fd */
   int num;
   char buffer[BUFFERMAX];
   bzero(buffer, BUFFERMAX);

   while ( 0 < ( num = read(connfd, buffer, BUFFERMAX) ) ) {
      if ( 0 > ( write (locfd, buffer, num) ) ) {
         fprintf(stdout, "%s: file write failure.\n", argv[0]);
         exit (EXIT_FAILURE);
      }
   }

   return EXIT_SUCCESS;
}
