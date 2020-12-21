// daemon to serve uds to live pcap clients 

#include "socklib.h"
#include "extractor.h"

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

   /* our process ID and Session ID */
   pid_t pid, sid;

   /* fork off the parent process */
   if ( 0 > (pid = fork()) ) {
      perror("fork failure");
      exit(errno);
   }
   /* if we got a good PID, then
      we can exit the parent process. */
   if ( 0 < pid ) {
      exit(EXIT_SUCCESS);
   }

   /* change the file mode mask */
   umask(0);

   /* open any logs here */        

   /* create a new SID for the child process */
   if ( 0 > (sid = setsid()) ) {
      /* Log the failure */
      perror("session creation failure");
      exit(errno);
   }

   /* change the current working directory */
   if ( 0 > (chdir("/")) ) {
      /* Log the failure */
      perror("failure changing to safe directory");
      exit(errno);
   }

   /* close out the standard file descriptors */
   //close(STDIN_FILENO);
   //close(STDOUT_FILENO);
   //close(STDERR_FILENO);

   /* daemon-specific initialization goes here */

   int listenfd; /* listening socket */

   /* set up tcp service port */
   if ( SOCKET_ERROR == ( listenfd = setup_tcp_server(PORT) ) ) {
      perror("failure setting up service port");
      exit(errno); 
   }

   int        connfd;   /* connected socket */
   struct     sockaddr_in cliaddr;
   socklen_t  clilen;

   /* run server loop */
   do {

      /* accept tcp connections */
      if ( 0 > ( connfd = accept(listenfd, (struct sockaddr *) &cliaddr, 
                  &clilen) ) ) {
         perror("service port connection error");
         exit(errno);
      }
      printf("\nconnection accepted");

      char buffer[BUFFERMAX];
      bzero(buffer, BUFFERMAX);
      int n;

      /* read from socket */
      if ( 0 > ( n = read(connfd, buffer, BUFFERMAX-1) ) ) {
         perror("socket read error");
         exit(errno);
      }
      printf("\nreading from connected socket"); 

      /* reply with path to unix domain socket */
      sleep(3);
      char uds_path[20];
      char portstring[10];
      strcpy(uds_path, "/tmp/req_"); 
      strcat(uds_path, buffer);
      printf("\nextracd unix socket: %s", uds_path);

      if ( 0 > ( n = write(connfd, uds_path, strlen(uds_path)) ) ) {
         perror("socket write error");
         exit(errno);
      }
      printf("\nwriting to connected socket"); 

      /* fork extraction controller */
      pid_t ecpid;
      if ( 0 > (ecpid = fork()) ) {
         perror("extraction controller fork failure");
         exit(errno);
      }

      /* run with child */
      if ( 0 == pid ) {
         extraction(uds_path, 0);  // file data with unix domain socket
         //extraction(uds_path, 1);  // live pcap stats with unix domain socket
      }

   } while (1); 

   exit(EXIT_SUCCESS);
}

