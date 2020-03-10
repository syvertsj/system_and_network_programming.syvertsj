/**
 * unixnitslib.h 
 * 
 * syvertsj
 */ 

#ifndef UNIXNITSLIB_H
#define UNIXNITSLIB_H

int setup_subscriber(char *server_path);
int setup_publisher(char *server_path);
int get_next_subscriber();

#endif /* UNIXNITSLIB_H */
