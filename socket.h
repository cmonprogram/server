#ifndef SOCKET_H
#define SOCKET_H
#include "main.h"
#include <sys/epoll.h>

RESULT add_to_epoll(server_params* server, int sockfd, PROTOCOL type);
RESULT delete_from_epoll(struct epoll_event *event);

RESULT epoll_get_msg(struct epoll_event *event);
RESULT epoll_send_msg(struct epoll_event *event);
#endif