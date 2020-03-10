/**
 * tcpnitslib.h 
 * 
 * syvertsj
 */ 

#ifndef TCPNITSLIB_H
#define TCPNITSLIB_H

#include <netinet/in.h>

int setup_subscriber(struct in_addr *pub_host, int pub_port);
int setup_publisher(int pub_port);
int get_next_subscriber();

#endif /* TCPNITSLIB_H */

