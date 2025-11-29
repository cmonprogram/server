#ifndef INSTANCE_H
#define INSTANCE_H
#include "main.h"

RESULT test_section(server_params *server, epoll_handler *event );
RESULT html_section(server_params *server, epoll_handler *event );
RESULT parce_section(server_params *server, epoll_handler *event );
RESULT command_section(server_params *server, epoll_handler *event );
RESULT start_instance(thread_context *server);
#endif