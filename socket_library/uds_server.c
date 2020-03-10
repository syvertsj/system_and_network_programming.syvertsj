// uds server using socket library 

#include "socklib.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

static void signal_handler(int signal)
{
   if ( signal == SIGHUP )  { printf("\nreceived SIGHUP\n"); }
   if ( signal == SIGTERM ) { printf("\nreceived SIGTERM\n"); }
   if ( signal == SIGINT )  { printf("\nreceived SIGINT\n"); }
   unlink("./unix_socket");
   exit(signal);
}

int main(int argc, char* argv[]) {

   /* install signal handler(s) */
   if ( SIG_ERR == signal(SIGHUP, signal_handler) ) {
      perror("\n signal(SIGHUP) error\n");
   }
   if ( SIG_ERR == signal(SIGTERM, signal_handler) ) {
      perror("\n signal(SIGTERM) error\n");
   }
   if ( SIG_ERR == signal(SIGINT, signal_handler) ) {
      perror("\n signal(SIGINT) error\n");
   }

   int listenfd; /* listening socket */

   /* set up service port */
   if ( SOCKET_ERROR == ( listenfd = setup_unix_server( (char *)"./unix_socket") ) ) {
      perror("failure setting up service port");
      exit(errno); 
   }

   int        connfd;   /* connected socket */
   struct     sockaddr_in cliaddr;
   socklen_t  clilen;

   /* run server loop */
   do {

      /* accept connections */
      if ( 0 > ( connfd = accept(listenfd, (struct sockaddr *) &cliaddr, 
                  &clilen) ) ) {
         perror("service port connection error");
         exit(errno);
      }
      printf("\n%s: connection accepted", argv[0]);

      /* read from socket */
      char buffer[BUFFERMAX];
      bzero(buffer, BUFFERMAX);
      int n;

      if ( 0 > ( n = read(connfd, buffer, BUFFERMAX-1) ) ) {
         perror("socket read error");
         exit(errno);
      }
      printf("\n%s: read: %s from connected socket", argv[0], buffer); 

      /* respond with simple echo */
      sleep(3);
      if ( 0 > ( n = write(connfd, buffer, strlen(buffer)) ) ) {
         perror("socket write error");
         exit(errno);
      }

   } while (1); 

   unlink("./unix_socket");

   exit(EXIT_SUCCESS);
}

