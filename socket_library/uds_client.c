// uds client using socket library

#include "socklib.h"

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
   /* convert hostname string to internet address */
   struct in_addr host;
   int locfd;

   if ( NULL != argv[1] ) { 
      if ( 0 == inet_aton(argv[1], &host) ) {
         perror("internet address conversion failure");
         return EXIT_FAILURE;
      }
   } else {
      if ( 0 == inet_aton("127.0.0.1", &host) ) {
         perror("internet address conversion failure");
         return EXIT_FAILURE;
      } 
   }
   printf("\n%s: converted server string to network byte order", argv[0]);

   /* obtain connected subscriber socket */
   int connfd;
   if ( SOCKET_ERROR == ( connfd = setup_unix_client( (char *) "./unix_socket") ) ) {
      fprintf (stderr, "%s: failure obtaining subscriber socket.\n", argv[0]);
      return EXIT_FAILURE;
   }
   printf("\n %s: obtained client uds socket \n", argv[0]);

   /* get request */
   char buffer[BUFFERMAX];
   printf("%s: enter message: ", argv[0]);
   fgets(buffer, BUFFERMAX-1, stdin);

   /* write to socket */
   if ( 0 > ( write(connfd, buffer, strlen(buffer)) ) ) {
      fprintf (stderr, "%s: failure obtaining subscriber socket.\n", argv[0]);
      return EXIT_FAILURE;
   }

   /* read reply from connected socket */
   int num;
   bzero(buffer, BUFFERMAX);

   if ( 0 > ( num = read(connfd, buffer, BUFFERMAX-1) ) ) {
      fprintf(stderr, "%s: file read failure.\n", argv[0]);
      return EXIT_FAILURE;
   }
   printf("\n%s received response: %s \n", argv[0], buffer);
   close(connfd);

   return EXIT_SUCCESS;
}
