#include "server.h"
#include "instance.h"
#include "socket.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

RESULT stage_init(server_params *server, server_settings *settings) {
  memset(server, 0, sizeof(*server));
  server->settings = *settings;
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

RESULT stage_create(server_params *server) {
  switch (server->settings.protocol) {
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

RESULT stage_bind(server_params *server) {
  if (bind(server->sock_fd, (const struct sockaddr *)&server->server_addr,
           sizeof(server->server_addr)) < 0) {
    perror("[bind error]");
    return 0;
  }
  if (add_to_epoll(server->epollfd, server->sock_fd,
                   server->settings.protocol) == RESULT_FAIL)
    return RESULT_FAIL;

  return RESULT_SUCESS;
}

RESULT stage_close(server_params *server) {
  close(server->sock_fd);
  return RESULT_SUCESS;
}

RESULT stage_execute(server_params *server) {
  PRINT("[%s server started] prort:%d\n",
        server->settings.protocol == UDP_SERVER ? "UDP" : "TCP",
        server->settings.port_no);

  if (server->settings.protocol == TCP_SERVER) {
    if ((listen(server->sock_fd, 5)) != 0) {
      printf("Listen failed...\n");
      return RESULT_FAIL;
    }
  }

  while (1) {
    RESULT res = start_instance(server);
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
  EXECUTE_STAGE("create socket", stage_create, &server);
  EXECUTE_STAGE("bind socket", stage_bind, &server);
  EXECUTE_STAGE("finish server", stage_execute, &server);
  return RESULT_SUCESS;
}