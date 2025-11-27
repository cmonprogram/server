#include "socket.h"
#include "instance.h"
#include "main.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

RESULT stage_init(server_params *server, server_settings *settings) {
  memset(server, 0, sizeof(*server));
  server->server_addr.sin_family = AF_INET;
  server->server_addr.sin_addr.s_addr = INADDR_ANY;
  server->server_addr.sin_port = htons(settings->port_no);
  server->epollfd = epoll_create1(0);
  if (server->epollfd == -1) {
    perror("[epoll error]");
    return RESULT_FAIL;
  }
  return RESULT_SUCESS;
}

RESULT stage_create(server_params *server, server_settings *settings) {
  switch (settings->protocol) {
  case UDP_SERVER:
    server->sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    break;
  case TCP_SERVER:
    server->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    break;
  default:
    return RESULT_FAIL;
  }
  return (server->sock_fd < 0) ? RESULT_FAIL : RESULT_SUCESS;
}

RESULT stage_bind(server_params *server, server_settings *settings) {
  if (bind(server->sock_fd, (const struct sockaddr *)&server->server_addr,
           sizeof(server->server_addr)) < 0) {
    perror("[bind error]");
    return 0;
  }
  if (add_to_epoll(server->epollfd, server->sock_fd, settings->protocol) ==
      RESULT_FAIL)
    return RESULT_FAIL;

  return RESULT_SUCESS;
}

RESULT stage_close(server_params *server, server_settings *settings) {
  close(server->sock_fd);
  return RESULT_SUCESS;
}

RESULT stage_execute(server_params *server, server_settings *settings) {
  PRINT("[%s server started] prort:%d\n",
        settings->protocol == UDP_SERVER ? "UDP" : "TCP", settings->port_no);

  if (settings->protocol == TCP_SERVER) {
    if ((listen(server->sock_fd, 5)) != 0) {
      printf("Listen failed...\n");
      return RESULT_FAIL;
    }
  }

  while (1) {
    RESULT res = start_instance(server, settings);
    switch (res) {
    case RESULT_SUCESS:
      break;
    case RESULT_FAIL:
      break;
    case RESULT_SKIP:
      break;
    case RESULT_EXIT:
      return RESULT_EXIT;
      break;
    }
  }
  return RESULT_SUCESS;
}

RESULT server_run(server_settings *settings) {
  server_params server;
  EXECUTE_STAGE("init server", stage_init, &server, settings);
  EXECUTE_STAGE("create socket", stage_create, &server, settings);
  EXECUTE_STAGE("bind socket", stage_bind, &server, settings);
  EXECUTE_STAGE("finish server", stage_execute, &server, settings);
  return RESULT_SUCESS;
}

RESULT add_to_epoll(int epollfd, int sockfd, PROTOCOL type) {
  /*
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK) == -1) {
      perror("[setnonblocking]");
      return RESULT_FAIL;
    }
  */
  struct epoll_event ev;
  ev.data.fd = sockfd;
  ev.events = EPOLLIN; // | EPOLLET;
  epoll_handler *handler = calloc(1, sizeof(epoll_handler));
  handler->socket_type = type;
  handler->addr_len = sizeof(handler->client_addr);
  handler->epollfd = epollfd;
  handler->client_fd = sockfd;
  ev.data.ptr = handler;
  if (epoll_ctl(handler->epollfd, EPOLL_CTL_ADD, handler->client_fd, &ev) ==
      -1) {
    perror("[epoll_ctl error]");
    free(handler);
    return RESULT_FAIL;
  }
  return RESULT_SUCESS;
}

RESULT delete_from_epoll(struct epoll_event *event) {
  epoll_handler *request = (epoll_handler *)event->data.ptr;
  epoll_ctl(request->epollfd, EPOLL_CTL_DEL, request->client_fd, NULL);
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
    add_to_epoll(handler->epollfd, new_fd, TCP);
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