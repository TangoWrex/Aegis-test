#include <stdlib.h> // calloc
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <netpacket/packet.h>
#include <signal.h>


#define PCKT_LEN 2048
#define MAX_LIMIT 40
#define FILE_SIZE 2048


extern volatile sig_atomic_t shutdown_flag;

