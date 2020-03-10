/**
 * unix_subscriber.c
 * 
 * syvertsj
 */ 

#include "common.h"
#include "unixnitslib.h"

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

   /* process input */
   if ( 2 != argc ) {
      fprintf(stderr, "\nUsage: %s [article name] \n\n", argv[0]);
      return EXIT_FAILURE;
   }
   
   /* open specified file */
   int locfd;

   if ( (0 != (strcmp("QUIT", argv[1]))) && (0 != strcmp("LIST", argv[1])) ) {

      /* open specified file */
      if ( 0 > (locfd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0644)) ) {
         fprintf (stderr, "%s: failure opening file: %s\n", argv[0], argv[1]);
         return EXIT_FAILURE;
      }
      printf("\n local file opened ");
   }

   /* obtain subscriber socket */
   int connfd;

   //if ( NITS_SOCKET_ERROR == ( connfd = setup_subscriber(argv[1]) ) ) {
   if ( NITS_SOCKET_ERROR == ( connfd = setup_subscriber( (char *) "/tmp/port7406") ) ) {
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

   /* read data from socket and write to file */
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
