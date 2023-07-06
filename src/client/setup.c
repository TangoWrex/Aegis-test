/** @file setup.c
 *
 * @brief
 *
 */

#include "setup.h"

/**
 * @brief Check the port number provided by the client
 *
 * @param char * port
 * @return SUCCESS_CODE on success
 * @return FAILURE_CODE on failure
 */
int check_port(char * port);

/**
 * @brief listen for and accept a connection from the client
 *
 * @param char *port
 * @param char *ipaddr
 * @return SUCCESS_CODE on success
 * @return FAILURE_CODE on failure
 */
int accept_client(char * port, char * ipaddr);

/**
 * @brief typedef for signal handling link_signal function to accept a function
 * pointer
 *
 */
typedef void (*signal_f)(int);

/**
 * @brief Handles SIGINT, changes the value of the global variable to stop the
 * while loop in main
 *
 * @param int signal
 * @return void
 */
void sigint_handler(int signal);

/**
 * @brief Function used for the signal handling of any signal my program needs
 * to handle
 * @param int sig, the signal that was sent,
 * @param signal_f signal_function, the function that will handle the signal
 * @return SUCCESS_CODE on success
 * @return FAILURE_CODE on failure
 */
int link_signal(int signal, signal_f signal_function);

int run(int argc, char * argv[])
{
    int    ret_code = FAIL_CODE;
    char * p_port   = NULL;
    int    opt      = 0;

    // ensure the argument count is correct
    int arg_count = check_args(argc);
    if (FAIL_CODE == arg_count)
    {
        fprintf(stderr, "Invalid argument count\n");
        goto EXIT;
    }

    // set up signal handling to catch SIGINT
    int signal_success = link_signal(SIGINT, sigint_handler);
    if (FAIL_CODE == signal_success)
    {
        fprintf(stderr, "Could not link signal handler\n");
        goto EXIT;
    }

    // getopt
    while ((opt = getopt(argc, argv, "p:d:t:")) != -1)
    {
        switch (opt)
        {
            case 'p': // port
                p_port = optarg;
                break;
            case ':':
                fprintf(stderr, "Usage: ./server -p port -d <directory> -t <timeout>\n");
                goto EXIT;
            default:
                fprintf(stderr, "Usage: ./server -p port -d <directory> -t <timeout>\n");
                goto EXIT;
        }
    }

    // Check for a valid port range
    int valid_port = check_port(p_port);
    if (FAIL_CODE == valid_port)
    {
        fprintf(stderr, "Invalid port\n");
        goto EXIT;
    }

    // allow our socket to use any ip address
    char ip_array[INET6_ADDRSTRLEN];
    snprintf(ip_array, INET6_ADDRSTRLEN, "%s", "0.0.0.0");
    char * ip_addr = ip_array;

    threadpool_t * t_pool = thpool_init(MAX_CONNECTIONS);
    if (NULL == t_pool)
    {
        fprintf(stderr, "Could not initialize threadpool\n");
        goto EXIT;
    }

    // Create the admin client for testing
    char username[ADMIN_NAME_LEN] = "admin";
    char password[ADMIN_PASS_LEN] = "password";
    int  client_added             = llist_add_client(t_pool->client_llist,
                                        username,
                                        password,
                                        DEFAULT_SESSION_ID,
                                        ADMIN,
                                        ADMIN,
                                        SOCK_MIN);

    if (FAIL_CODE == client_added)
    {
        fprintf(stderr, "Could not add admin client to llist\n");
        goto SHUTDOWN;
    }

    while (!shutdown_flag)
    {
        if (shutdown_flag)
        {
            goto SHUTDOWN;
        }
        int client_sock = accept_client(p_port, ip_addr);
        if (0 > client_sock)
        {
            fprintf(stderr, "accept_client failed");
            goto SHUTDOWN;
        }
        if (0 < client_sock)
        {

            int enq_success = enqueue_job(t_pool, client_sock);
            if (FAIL_CODE == enq_success)
            {
                fprintf(stderr, "Could not enqueue job\n");
            }
        }
        if (shutdown_flag)
        {
            goto SHUTDOWN;
        }
    }
SHUTDOWN:
    thpool_destroy(t_pool);
    ret_code = SUCCESS_CODE;
EXIT:
    return ret_code;
} /* run() */

int check_args(int argc)
{
    int ret_code = FAIL_CODE;

    if ((MIN_ARGC > argc) || (MAX_ARGC < argc))
    {
        fprintf(stderr,
                "Usage: ./server -p <port> -d <optional> <directory> -t <optional: "
                "timeout>\n");
        goto EXIT;
    }
    ret_code = SUCCESS_CODE;
EXIT:
    return ret_code;
} /* check_args() */

int check_port(char * p_port)
{
    int     ret_code    = FAIL_CODE;
    char ** pp_null_ptr = NULL;
    if (NULL == p_port)
    {
        fprintf(stderr, "Invalid port\n");
        goto EXIT;
    }

    // check the port for alphanumeric characters
    long port_num = strtol(p_port, pp_null_ptr, NUM_BASE);
    if (pp_null_ptr != NULL)
    {
        fprintf(stderr, "Invalid port\n");
        goto EXIT;
    }
    if ((MIN_PORT > port_num) || (MAX_PORT < port_num))
    {
        fprintf(stderr, "Invalid port: enter port between %d - %d\n", MIN_PORT, MAX_PORT);
        goto EXIT;
    }

    ret_code = SUCCESS_CODE;
EXIT:
    return ret_code;
} /* check_port() */

int accept_client(char * p_port, char * p_ipaddr)
{
    int sock_created = -1;
    if ((NULL == p_port))
    {
        fprintf(stderr, "Port error");
        sock_created = -1;
        goto ERROR;
    }
    if (NULL == p_ipaddr)
    {
        fprintf(stderr, "IP Address error");
        sock_created = -1;
        goto ERROR;
    }
    struct sockaddr_storage their_addr = { 0 };
    socklen_t               addr_size  = 0;
    struct addrinfo         hints      = { 0 };
    struct addrinfo *       res        = NULL;
    int                     status     = 0;
    int                     yes        = 1;

    int sock   = 0;
    int new_fd = 0; // listen on sock, new connection on new_fd

    // set hints to zero and set configuration info for the socket
    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;   // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags    = AI_PASSIVE;  // any ip address

    // get address info for the socket
    status = getaddrinfo(NULL, p_port, &hints, &res);
    if (0 != status)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        sock_created = -1;
        goto ERROR;
    }

    // create the socket
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    // set socket options to reuse the address and port
    int setsockopt_success = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
    if (-1 == setsockopt_success)
    {
        fprintf(stderr, "setsockopt");
        sock_created = -1;
        goto EXIT;
    }

    // bind the socket to the port
    int bind_success = bind(sock, res->ai_addr, res->ai_addrlen);
    if (-1 == bind_success)
    {
        fprintf(stderr, "bind failed. Error");
        sock_created = -1;
        goto EXIT;
    }

    while (1)
    {
        listen(sock, MAX_CONNECTIONS);
        addr_size = sizeof their_addr;
        new_fd    = accept(sock, (struct sockaddr *)&their_addr, &addr_size);

        // check if a valid file descriptor is returned
        if (0 < new_fd)
        {
            // I am breaking here because we recieved a valid file descriptor
            break;
        }

        // check if accept returned an error
        if (-1 == new_fd)
        {
            fprintf(stderr, "accept failed. Error: %s\n", strerror(errno));
            goto EXIT;
        }

        // check if SIGINT Recieved
        if (false == shutdown_flag)
        {
            goto EXIT;
        }
    }
    // each time we get a new connection we assign it to a new file descriptor
    sock_created = new_fd;
EXIT:
    if (NULL != res)
    {
        freeaddrinfo(res);
        res = NULL;
    }

    close(sock);

ERROR:
    return sock_created;
} /* accept_client() */

void sigint_handler(int sig)
{
    // unused variable error produced if (void)sig not used
    (void)sig;
    shutdown_flag = 1;
    return;
} /* sigint_handler() */

int link_signal(int sig, signal_f handler)
{
    int              ret_code = FAIL_CODE;
    struct sigaction sa       = { 0 };
    sa.sa_handler             = handler;
    sa.sa_flags               = 0;

    sigemptyset(&sa.sa_mask);

    int sig_check = sigaction(sig, &sa, NULL);
    if (-1 == sig_check)
    {
        fprintf(stderr, "sigaction");
        goto EXIT;
    }
    ret_code = SUCCESS_CODE;
EXIT:
    return ret_code;
} /* link_signal() */
