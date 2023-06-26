#ifndef SIG_HDL_H
#define SIG_HDL_H

#include <signal.h>
#include <stdbool.h>
typedef void (*signal_f)(int);

/**
 * @brief Handles SIGINT, changes the value of the global variable to stop the while loop in main
 * @param int sig
 * @return void
 */
void sigint_handler(int);

/**
 * @brief Handles SIGCHLD
 * @param int sig
 * @return void
 */
void sigint_child_handler(int);

/**
 * @brief Function used for the signal handling of any signal my program needs to handle
 * @param int sig, the signal that was sent, flag: used to determine what to do
 * @return void
 */
void link_signal(int, signal_f, bool);

#endif
