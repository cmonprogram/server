#ifndef CMD_H
#define CMD_H
#include "main.h"
#include <sys/epoll.h>

RESULT cmd_exit(server_params *serve, epoll_handler *request);
RESULT cmd_time(server_params *server, epoll_handler *request);
RESULT cmd_test_start(server_params *server, epoll_handler *request);
RESULT cmd_test_end(server_params *server, epoll_handler *request);
RESULT cmd_html(server_params *server, epoll_handler *request);
#endif