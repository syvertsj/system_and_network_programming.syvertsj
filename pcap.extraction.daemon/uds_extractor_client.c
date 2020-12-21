// uds packet capture client using socket library

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

   if ( argc != 2 ) { 
      printf("\nusage: %s [uds socket]", argv[0]);
      return EXIT_FAILURE;
   }

   if ( 0 == inet_aton("127.0.0.1", &host) ) {
      perror("internet address conversion failure");
      return EXIT_FAILURE;
   } 
   printf("\n%s: converted server string to network byte order", argv[0]);

   /* obtain connected subscriber socket */
   int connfd;
   if ( SOCKET_ERROR == ( connfd = setup_unix_client( argv[1]) ) ) {
      fprintf (stderr, "%s: failure obtaining subscriber socket.\n", argv[0]);
      return EXIT_FAILURE;
   }
   printf("\n %s: obtained client uds socket \n", argv[0]);

   /* read data from connected socket */
   int num;
   char buffer[BUFFERMAX];
   bzero(buffer, BUFFERMAX);

   while ( 0 < ( num = read(connfd, buffer, BUFFERMAX-1) ) ) {
      printf("\n%s received: %s \n", argv[0], buffer);
   }

   close(connfd);

   return EXIT_SUCCESS;
}
