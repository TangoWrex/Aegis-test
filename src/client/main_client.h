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
#define MAX_USERNAME       50
#define MAX_PASSWORD       50
#define SUCCESS_CODE       1    // success return code
#define FAIL_CODE          -1   // failure return code
#define MAX_DIR_NAME       40   // MAX Length for directory name
#define SOCK_MAX           1024 // Max number for fd socket
#define SOCK_MIN           0    // Min number for fd socket
#define ADMIN_PRIVILEGE    1
#define MAX_CONNECTIONS    8     // max threadpool size
#define MAX_PORT           65535 // max port number
#define MIN_PORT           1024  // min port number
#define MAX_PORT_LEN       5     // used for checking port length
#define REQ_ARGS           5     // 4 args + 1 for the program name




extern volatile sig_atomic_t shutdown_flag;

#endif