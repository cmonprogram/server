#include "socket.h"
#include "main.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

RESULT add_to_epoll(int epollfd, int sockfd, PROTOCOL type) {
  /*
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK) == -1) {
      perror("[setnonblocking]");
      return RESULT_FAIL;
    }
 */
  struct epoll_event ev;
  ev.data.fd = sockfd;
  ev.events = EPOLLIN; // | EPOLLET;  //EPOLLONESHOT;
  ev.data.fd = sockfd;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
    perror("[epoll_ctl add error]");
    return RESULT_FAIL;
  }
  return RESULT_SUCESS;
}

RESULT delete_from_epoll(int epollfd, int sockfd) {
  if (epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, NULL) == -1) {
    perror("[epoll_ctl del error]");
    return RESULT_FAIL;
  }
  close(sockfd);
  return RESULT_SUCESS;
}

// need msg limit error
RESULT epoll_get_msg(epoll_handler *handler) {
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
  } 
  return RESULT_FAIL;
}

RESULT epoll_send_msg(epoll_handler *handler) {
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