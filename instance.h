#ifndef INSTANCE_H
#define INSTANCE_H
#include "main.h"

RESULT test_section(server_params *server, struct epoll_event *event);
RESULT html_section(server_params *server, struct epoll_event *event);
RESULT parce_section(server_params *server, struct epoll_event *event);
RESULT command_section(server_params *server, struct epoll_event *event);
RESULT start_instance(thread_context *server);
#endif