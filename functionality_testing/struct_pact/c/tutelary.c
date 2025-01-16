#include "client.h"
#include "signal_handle.h"
// #include <sys/socket.h>
// #include <sys/types.h>
#include <stdlib.h> // calloc
#include <stdio.h>
#include <string.h>

#include <netdb.h>
// #include <stdio.h>
// #include <string.h>
// #include <sysexits.h>
#include <unistd.h>

#include <ifaddrs.h>
#include <netpacket/packet.h>

#define PCKT_LEN 2048

volatile sig_atomic_t shutdown_flag = false;



int main(int argc, char *argv[])
{

    link_signal(SIGINT, sigint_handler, true);

	// create our sensor struct 
	sensor_struct *sensor = calloc(1, sizeof(sensor_struct));


	char* ip = "127.0.0.1";

	char* sensor_id = "dragon01";
    char* mac = "21:e9:10:6p:87:mq";
    char* port = "5500";
	char* lat = "40.12345";
	char* lon = "40.54321";
	
	


	sensor->sensor_id = sensor_id;
	sensor->mac_addr = mac;
	sensor->port = port;
	sensor->latitude = lat;
	sensor->longitude = lon;
	sensor->data_len = 0;

	// get the size of all data
	sensor->data_len = strlen(sensor->sensor_id) + strlen(sensor->mac_addr) + strlen(sensor->port) + strlen(sensor->latitude) + strlen(sensor->longitude);

	
	printf("sensor id: %s\n", sensor->sensor_id);
	printf("mac addr: %s\n", sensor->mac_addr);
	printf("port: %s\n", sensor->port);
	printf("latitude: %s\n", sensor->latitude);
	printf("longitude: %s\n", sensor->longitude);
	printf("data len: %d\n", sensor->data_len);


	printf("\n");



	// if (argc != 3)
	// {
    //     perror("Usage: ./client <dest-ip> <port>\n");
	// 	return 0;
	// }

	// int sock = tcp_socket_setup(argv[1], argv[2]);
	int sock = tcp_socket_setup(port, ip);
	if(sock < 0){
		perror("socket setup failed");
		return 0;
	}


	

	// get the mac 

	// send the mac address
	send_struct(sock, sensor);


	close_socket(sock);

}