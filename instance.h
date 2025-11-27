#ifndef INSTANCE_H
#define INSTANCE_H
#include "main.h"

RESULT test_section(server_params *server, server_settings *settings,
                    struct epoll_event *event);
RESULT html_section(server_params *server, server_settings *settings,
                    struct epoll_event *event);
RESULT parce_section(server_params *server, server_settings *settings,
                     struct epoll_event *event);
RESULT command_section(server_params *server, server_settings *settings,
                       struct epoll_event *event);
RESULT start_instance(server_params *server, server_settings *settings);
#endif