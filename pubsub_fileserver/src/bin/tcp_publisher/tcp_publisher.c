/**
 * tcp_publisher.c
 * 
 * syvertsj
 */ 

#include "common.h"
#include "tcpnitslib.h"

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <signal.h>

static void signal_handler(int signal)
{
   if ( signal == SIGINT ) {
      printf("\nreceived SIGINT\n");
   }
   if ( signal == SIGHUP ) {
      printf("\nreceived SIGHUP\n");
   }

	printf("\n exiting...\n");
	exit(signal);
}

void list_articles()
{
   DIR *dir;
   struct dirent *ent;
   if ( NULL != (dir = opendir ("Articles")) ) {
      while ( NULL != (ent = readdir(dir)) ) {
         printf ("%s\n", ent->d_name);
      }
      closedir (dir);
   } else {
      perror ("unable to list articles directory");
   }
}

int main(int argc, char *argv[])
{

   /* Install signal handlers */
   signal(SIGINT, signal_handler);
   signal(SIGHUP, signal_handler);

   /* set up publisher */
   if ( NITS_SOCKET_ERROR == setup_publisher(PORT) ) {
      fprintf (stderr, "%s: failure setting up publisher.\n", argv[0]);
      return EXIT_FAILURE; 
   }
   printf("\n tcp publisher set up");

   int connfd;   /* connected socket */

   /* run server loop */ 
   do {

      /* get connected socket for subscriber */
      if ( NITS_SOCKET_ERROR == ( connfd = get_next_subscriber() ) ) {
         fprintf (stderr, "%s: failure getting subscriber.\n", argv[0]);
         return EXIT_FAILURE; 
      }
      printf("\n %s: get_next_subscriber called ", argv[0]);
   
      /* read name of article from socket */
      char buffer[BUFFERMAX];
      bzero(buffer, BUFFERMAX);
      int n;

      if ( 0 > ( n = read(connfd, buffer, BUFFERMAX-1) ) ) {
         fprintf (stderr, "%s: socket read error: %d.\n", argv[0], errno);
         exit (EXIT_FAILURE);
      }

      if ( 0 == (strcmp("QUIT", buffer) ) ) {
         printf("\n %s: received QUIT and will now exit \n", argv[0]);
         exit (EXIT_SUCCESS);
      } else if ( 0 == (strcmp("LIST", buffer) ) ) {
         printf("\n %s: received LIST \n", argv[0]);
         list_articles();
      } else {
         printf("\n %s: received name of article: %s \n", argv[0], buffer);
      }

      /* open file */
      //char filename[BUFFERMAX];
      char *filename;
      strcpy(filename, "Articles/"); 
      strcat(filename, buffer);
      int locfd;

      printf("\n %s: opening file: %s \n", argv[0], filename);

      if ( 0 > (locfd = open(filename, O_RDONLY, 0644)) ) {
         fprintf(stdout, "%s: file open failure.\n", argv[0]);
         exit (EXIT_FAILURE);
      }
      printf("\n %s: file: %s opened \n", argv[0], filename);

      /* read file from local fd and write to socket */
      int num; 
      //bzero(buffer, sizeof(buffer));
      bzero(buffer, BUFFERMAX);

      while ( 0 < ( num = read(locfd, buffer, BUFFERMAX) ) ) {
         if ( 0 > ( write (connfd, buffer, num) ) ) {
            fprintf(stdout, "%s: file write failure.\n", argv[0]);
            exit (EXIT_FAILURE);
         }
      }
      fprintf(stdout, "%s: file: %s written \n", argv[0], buffer);
      close(locfd);  /* close local file descriptor */
      close(connfd); /* close connected socket */

   } while (1); 

   return EXIT_SUCCESS;
}
