#include "socket.h"
#include "main.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

RESULT add_to_epoll(server_params *server, int sockfd, PROTOCOL type) {
  /* 
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK) == -1) {
      perror("[setnonblocking]");
      return RESULT_FAIL;
    }
 */
  epoll_handler *handler = calloc(1, sizeof(epoll_handler));
  handler->server = server;
  handler->socket_type = type;
  handler->addr_len = sizeof(handler->client_addr);
  handler->client_fd = sockfd;

  struct epoll_event ev;
  ev.data.fd = sockfd;
  ev.events = EPOLLIN; // | EPOLLET;  //EPOLLONESHOT;
  ev.data.ptr = handler;
  if (epoll_ctl(server->epollfd, EPOLL_CTL_ADD, handler->client_fd, &ev) ==
      -1) {
    perror("[epoll_ctl error]");
    free(handler);
    return RESULT_FAIL;
  }

  return RESULT_SUCESS;
}

RESULT delete_from_epoll(struct epoll_event *event) {
  epoll_handler *request = (epoll_handler *)event->data.ptr;
  epoll_ctl(request->server->epollfd, EPOLL_CTL_DEL, request->client_fd, NULL);
  close(request->client_fd);
  free(request);
  return RESULT_SUCESS;
}

// need msg limit error
RESULT epoll_get_msg(struct epoll_event *event) {
  epoll_handler *handler = (epoll_handler *)event->data.ptr;
  if (handler->socket_type == UDP_SERVER) {
    handler->in_buffer_len =
        recvfrom(handler->client_fd, (char *)handler->in_buffer,
                 sizeof(handler->in_buffer) - 1, MSG_WAITALL, // MSG_ZEROCOPY
                 (struct sockaddr *)&handler->client_addr, &handler->addr_len);
    if (handler->in_buffer_len < 0 &&
        (errno == EAGAIN || errno == EWOULDBLOCK)) {
      return RESULT_SKIP;
    } else if (handler->in_buffer_len > 0) {
      handler->in_buffer[handler->in_buffer_len++] = '\0';
      return RESULT_SUCESS;
    }
    return RESULT_FAIL;
  } else if (handler->socket_type == TCP) {
    handler->in_buffer_len = read(handler->client_fd, handler->in_buffer,
                                  sizeof(handler->in_buffer) - 1);
    if (handler->in_buffer_len < 0 &&
        (errno == EAGAIN || errno == EWOULDBLOCK)) {
      return RESULT_SKIP;
    } else if (handler->in_buffer_len > 0) {
      handler->in_buffer[handler->in_buffer_len++] = '\0';
      return RESULT_SUCESS;
    }
    return RESULT_FAIL;
  } else if (handler->socket_type == TCP_SERVER) {
    int new_fd;
    if ((new_fd = accept(handler->client_fd,
                         (struct sockaddr *)&handler->client_addr,
                         (socklen_t *)&handler->addr_len)) < 0) {
      perror("[accept failed]");
      return RESULT_FAIL;
    }
    add_to_epoll(handler->server, new_fd, TCP);
  }
  return RESULT_FAIL;
}

RESULT epoll_send_msg(struct epoll_event *event) {
  epoll_handler *handler = (epoll_handler *)event->data.ptr;
  if (handler->socket_type == UDP_SERVER) {
    if (sendto(handler->client_fd, (char *)handler->out_buffer,
               handler->out_buffer_len, 0,
               (struct sockaddr *)&handler->client_addr, handler->addr_len)) {
      return RESULT_SUCESS;
    }
  } else if (handler->socket_type == TCP) {
    if (send(handler->client_fd, handler->out_buffer, handler->out_buffer_len,
             0)) {
      return RESULT_SUCESS;
    }
  }
  return RESULT_FAIL;
}