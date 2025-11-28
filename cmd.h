#ifndef CMD_H
#define CMD_H
#include "main.h"
#include <sys/epoll.h>

RESULT cmd_exit(server_params *server, struct epoll_event *event);
RESULT cmd_time(server_params *server, struct epoll_event *event);
RESULT cmd_test_start(server_params *server, struct epoll_event *event);
RESULT cmd_test_end(server_params *server, struct epoll_event *event);
RESULT cmd_html(server_params *server, struct epoll_event *event);
#endif