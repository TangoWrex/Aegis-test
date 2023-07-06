/** @file client.c
 *
 * @brief
 *
 */

#include "client.h"

extern volatile sig_atomic_t shutdown_flag;

int run_aegis_client(int argc, char * argv[])
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

int tcp_socket_setup(const char * port, char * ip)
{
    if (NULL == port)
    {
        fprintf(stderr, "Invalid Port\n");
        return 0;
    }
    if (NULL == ip)
    {
        fprintf(stderr, "invalid IP\n");
    }
    struct addrinfo * results;
    int               err = getaddrinfo(ip, port, NULL, &results);
    if (0 != err)
    {
        fprintf(stderr, "Cannot get address: %s\n", gai_strerror(err));
        return EX_NOHOST;
    }

    int sd = socket(results->ai_family, SOCK_STREAM, 0);
    if (sd < 0)
    {
        fprintf(stderr, "Could not create socket");
        freeaddrinfo(results);
        return EX_OSERR;
    }

    err = connect(sd, results->ai_addr, results->ai_addrlen);
    if (err < 0)
    {
        perror("Could not connect to remote");
        close(sd);
        freeaddrinfo(results);
        return EX_UNAVAILABLE;
    }

    return sd;
}

void close_socket(int sock)
{
    if (0 == sock)
    {
        fprintf(stderr, "Socket not specified");
        return;
    }
    close(sock);
}

static void send_msg(int sock, void * msg, FILE * fp, int read_size)
{
    if (0 == sock)
    {
        fprintf(stderr, "socket is null");
        return;
    }
    if (NULL == msg)
    {
        fprintf(stderr, "message is null");
        return;
    }
    if (0 == read_size)
    {
        fprintf(stderr, "read size is null");
        return;
    }
    fp = popen(msg, "r");
    while ((read_size = fread(msg, 1, PCKT_LEN, fp)))
    {
        send(sock, msg, read_size, 0);
    }
    pclose(fp);
}

char * get_mac_address(unsigned char MAC_str[13])
{
    // reference: https://community.onion.io/topic/2441/obtain-the-mac-address-in-c-code/3
    // https://gist.github.com/evanslai/3711349
    return 0;
}

/**
 * @brief handle the client
 * @param void struct
 * @return none
 */
void send_mac(int socket, char * mac)
{
    if (!socket)
    {
        perror("ip not received\n");
        return;
    }
    if (!mac)
    {
        perror("mac not received\n");
        return;
    }
    char * mac_addr = calloc(1, 20);
    strcpy(mac_addr, mac);

    send(socket, mac_addr, strlen(mac_addr), 0);

    free(mac_addr);
}

const char * get_mac()
{
    // https://stackoverflow.com/questions/1779715/how-to-get-mac-address-of-your-machine-using-a-c-program#:~:text=the%20file%20%2Fsys%2Fclass%2F,sys%2Fclass%2Fnet%2F%20.
    struct ifaddrs * ifaddr = NULL;
    struct ifaddrs * ifa    = NULL;
    int              i      = 0;

    const char * hold_mac = NULL;

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
    }
    else
    {
        for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
        {
            if ((ifa->ifa_addr) && (ifa->ifa_addr->sa_family == AF_PACKET))
            {
                struct sockaddr_ll * s = (struct sockaddr_ll *)ifa->ifa_addr;
                printf("%-8s ", ifa->ifa_name);
                for (i = 0; i < s->sll_halen; i++)
                {
                    //   printf("%02x%c", (s->sll_addr[i]), (i+1!=s->sll_halen)?':':'\n');
                    printf(
                        "%02x%c", (s->sll_addr[i]), (i + 1 != s->sll_halen) ? ':' : '\n');
                    hold_mac = (char *)s->sll_addr;
                    //   printf("%02x", (s->sll_addr[i]));
                }
                printf("\n");
            }
        }
        freeifaddrs(ifaddr);
    }
    return hold_mac;
}

bool send_file_over_netcat(char * ip, char * port, char * filename)
{
    if (!ip)
    {
        perror("ip not received\n");
        return false;
    }
    if (!filename)
    {
        perror("filename not received\n");
        return false;
    }
    char command[256] = { 0 };
    // sprintf(command, "ncat --ssl -w 3 127.0.0.1 15051 < wrx.jpg");
    // TODO: where does this file need ot be located? can we pass a location?
    sprintf(command, "ncat --ssl -w 3 %s %s < %s", ip, port, filename);
    sleep(1);
    system(command);

    return true;
}

bool send_file(int sock, FILE * fp)
{
    if (sock == 0)
    {
        perror("socket is null");
        return false;
    }
    if (fp == NULL)
    {
        perror("file is null");
        return false;
    }
    // https://superuser.com/questions/1368666/receiving-multiple-files-at-once-in-netcat-without-overwriting-last-file
    char               ip[INET_ADDRSTRLEN];
    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    socklen_t addr_size = sizeof(my_addr);
    getsockname(sock, (struct sockaddr *)&my_addr, &addr_size);
    inet_ntop(AF_INET, &my_addr.sin_addr, ip, INET_ADDRSTRLEN);

    unsigned int myport = ntohs(my_addr.sin_port);
    printf("my ip: %s", ip);
    printf("my port: %d", myport);

    char command[256] = { 0 };
    sprintf(command, "ncat --ssl -w 3 127.0.0.1 15051 < wrx.jpg");
    // sprintf(command, "ncat --ssl -w 3 %s 15051 < %s", ip, filename);
    sleep(1);
    system(command);

    // fclose(fp);
    return true;
}

bool process_job(char * job, int socket)
{
    if (NULL == job)
    {
        perror("job not received");
        return false;
    }
    char change_dir[5];
    sprintf(change_dir, "%s", "cd /");

    printf("executing task: %s\n", job);
    return true;

    // char send_file_test[9];
    // sprintf(send_file_test, "%s", "sendfile");

    // if (strncmp(job, "sdrforspace", 11) == 0){
    //     // program should be ran from root, but if not we'll go to the dir anyway
    //     // TODO: pull in the bash script off the pi that we built
    //     char sdrforspace_test[87] = "sudo ./usr/src/SDR4space/sdrvm -d
    //     DragonOS/RX/spectrum/1_wide_spectrum/ -f spectrum.js"; printf("executing task:
    //     sdrforspace\n");

    //     FILE *fp;
    //     fp = popen(change_dir, "r");
    //     pclose(fp);

    //     // fp = popen(sdrforspace_test, "r");
    //     // pclose(fp);
    //     return true;
    // }

    // if (strncmp(job, send_file_test, 8) == 0){
    //     // program should be ran from root, but if not we'll go to the dir anyway
    //     printf("execute send job command\n");

    //     int read_size = 1024;
    //     FILE *fp;
    //     fp = fopen("wrx.jpg", "r");
    //     bool return_from_send = send_file(socket, fp);
    //     pclose(fp);
    //     return true;
    //     }

    return false;
}
