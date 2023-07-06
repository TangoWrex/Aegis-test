/** @file main_client.h
 *
 * @brief
 *
 */


#ifndef MAIN_CLIENT_H
#define MAIN_CLIENT_H


#include <stdlib.h> // calloc
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <netpacket/packet.h>
#include <signal.h>
#include<stdbool.h> // bool
#include<gps.h> // gps

//#include <netdb.h>
#include <netinet/in.h>
// #include <stdio.h>
// #include <string.h>
#include <sysexits.h>
// #include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
// #include <stdlib.h> // calloc

#include <sys/ioctl.h>
#include <net/if.h>

// #include <ifaddrs.h>          // getifaddrs
// #include <netpacket/packet.h> // sockaddr_ll

// #include <stdbool.h> // bool
// #include <signal.h>  // signal
// #include <arpa/inet.h>
//


#define PCKT_LEN 2048
#define MAX_LIMIT 40
#define FILE_SIZE 2048


extern volatile sig_atomic_t shutdown_flag;

#endif