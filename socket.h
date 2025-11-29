#ifndef SOCKET_H
#define SOCKET_H
#include "main.h"
#include <sys/epoll.h>

RESULT add_to_epoll(int epollfd, int sockfd, PROTOCOL type);
RESULT delete_from_epoll(int epollfd, int sockfd);

RESULT epoll_get_msg(epoll_handler *handler);
RESULT epoll_send_msg(epoll_handler *handler);
#endif