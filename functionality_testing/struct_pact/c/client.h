#include<stdio.h> // FILE
#include<stdbool.h> // bool
#include<gps.h> // gps


typedef struct sensor_struct{
	char* sensor_id;
	char* mac_addr;
	char* port;
    int port_len;
	char* latitude;
	char* longitude;
	uint16_t data_len;	
} sensor_struct;

/** 
 * @brief create tcp socket as our admin channel
 * @param const char* port
 * @param char * ip
 * @return socket 
 */
int tcp_socket_setup(const char* port,  char *ip);

/** 
 * @brief close the tcp socket 
 * @param int socket to close 
 * @return none
 */
void close_socket(int sock);

/** 
 * @brief send_msg is responsible for sending the appropriate message back to our client 
 * based on the users request
 * @param int socket to send the data to 
 * @param FILE *fp file pointer for popen
 * @param char *msg received from the client
 * @param read_size size of data sent from the client
 * @return 
 */
static void send_msg(int sock, void* msg, FILE *fp, int read_size);


/** 
 * @brief send_file is responsible for sending the file back to the client
 * @param int socket to send the data on
 * @param FILE *fp file pointer for popen
 * @param char *msg received from the client
 * @param read_size size of data sent from the client
 * @return 
 */
bool send_file(int sock, FILE *fp);

/** 
 * @brief The first message sent from the client to the server is the mac address
 * @return 
 */
const char *get_mac();

/** 
 * @brief The first message sent from the client to the server is the mac address
 * @return 
 */
void send_mac(int socket, char * mac);

/** 
 * @brief pack our data in a struct to be sent over the wire. Server will use unpack to get the data
 * @return 
 */
void send_struct(int socket, sensor_struct *sensor);



/** 
 * @brief The first m   essage sent from the client to the server is the mac address
 * @return 
 */
const char * get_mac();

/** 
 * @brief The first m   essage sent from the client to the server is the mac address
 * @return 
 */
bool process_job(char *job, int socket);


/** 
 * @brief 
 * @return 
 */

void get_gps();
// https://stackoverflow.com/questions/32000328/how-can-i-implement-a-gpsd-client-in-c-to-get-latitude-longitude-and-altitude

//
