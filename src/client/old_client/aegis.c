#include "client.h"
#include "signal_handle.h"
#include <stdlib.h> // calloc
#include <stdio.h>
#include <string.h>

#include <netdb.h>
#include <unistd.h>

#include <ifaddrs.h>
#include <netpacket/packet.h>

#define PCKT_LEN 2048

volatile sig_atomic_t shutdown_flag = false;

int main(int argc, char * argv[])
{

    link_signal(SIGINT, sigint_handler, true);
    char *       ip   = "127.0.0.1";
    const char * port = "5003";
    char *       mac  = "00:x7:z0:0c:32:zz";

    int sock = tcp_socket_setup(port, ip);
    if (sock < 0)
    {
        perror("socket setup failed");
        return 0;
    }

    // get the mac

    // send the mac address
    send_mac(sock, mac);

    // 4. wait for the server to send an authentication response
    // for the the server to inform us we are registered and wait for a job from the
    // server

    // 5. wait for a job from the server

    char job[2000];
    int  read_size;
    char ack_msg[2000];
    char checkin_message[8];
    sprintf(checkin_message, "%s", "check in");

    // TODO: does this sleep need to be usleep? it's technically a python thread so are C
    // thread safe not required? Sleep allows the server time to check and add the file to
    // the database
    sleep(1);
    int bytes_read = recv(sock, ack_msg, PCKT_LEN, 0);
    if (bytes_read < 0)
    {
        perror("recv failed");
        return 0;
    }
    ack_msg[bytes_read] = '\0';
    printf("ask_msg: %s", ack_msg);

    if (strcmp(ack_msg, "ACK") == 0)
    {
        // if we recieved an ACK from the server we can go request them to send a job
        while (!shutdown_flag)
        {
            // send a checkin message to the server
            // we don't want to bombard the server. sleep for 5 seconds between checkings
            sleep(5);
            send(sock, checkin_message, strlen(checkin_message), 0);
            printf("sending checkin message\n");
            // wait for a job from the server
            sleep(2);
            read_size = recv(sock, job, PCKT_LEN, 0);
            printf("readsize: %d", read_size);
            if (read_size > 0)
            {
                printf("job received: %s\n", job);
                job[read_size]   = '\0';
                bool job_success = process_job(job, sock);
                // if (!job_success){
                // 	printf("job failed\n");
                // }
                // else{
                // 	printf("job success\n");
                // }
                // clear the buffer
                memset(job, 0, sizeof(job));
            }
            else
            {
                printf("no job received\n");
            }
        }
    }
    else
    {
        printf("ack not received\n");
        return 0;
    }

    printf("job: %s", job);

    close_socket(sock);
}