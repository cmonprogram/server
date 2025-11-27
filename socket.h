#ifndef SOCKET_H
#define SOCKET_H
#include "main.h"
#include <sys/epoll.h>

RESULT stage_init(server_params *server, server_settings *settings);
RESULT stage_create(server_params *server, server_settings *settings);
RESULT stage_bind(server_params *server, server_settings *settings);
RESULT stage_close(server_params *server, server_settings *settings);
RESULT stage_execute(server_params *server, server_settings *settings);

RESULT server_run(server_settings *settings);
RESULT add_to_epoll(int epollfd, int sockfd, PROTOCOL type);
RESULT delete_from_epoll(struct epoll_event *event);
RESULT epoll_get_msg(struct epoll_event *event);
RESULT epoll_send_msg(struct epoll_event *event);
#endif