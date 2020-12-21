
#ifndef SOCKLIB_H
#define SOCKLIB_H

#include <netinet/in.h>

#define PORT 2112
#define SOCKET_OK 0
#define SOCKET_ERROR -1
#define BUFFERMAX 256

int setup_tcp_server(int pub_port);
int setup_tcp_client(int pub_port);

int setup_unix_server(char *uds_path);
int setup_unix_client(char *uds_path);

#endif /* SOCKLIB_H */

