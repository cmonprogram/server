#include "instance.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

RESULT stage_init(server_params *server, server_settings *settings) {
  if (settings->protocol == TCP) {
    PRINT_ERROR("TCP not supported yet");
    return RESULT_EXIT;
  }
  memset(server, 0, sizeof(*server));
  server->server_addr.sin_family = AF_INET;
  server->server_addr.sin_addr.s_addr = INADDR_ANY;
  server->server_addr.sin_port = htons(settings->port_no);
  return RESULT_SUCESS;
}

RESULT stage_create(server_params *server, server_settings *settings) {
  if (settings->protocol == UDP) {
    server->sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    return server->sock_fd >= 0;
  }
  return RESULT_SUCESS;
}

RESULT stage_bind(server_params *server, server_settings *settings) {
  if (bind(server->sock_fd, (const struct sockaddr *)&server->server_addr,
           sizeof(server->server_addr)) < 0) {
    perror("[bind error]");
    return 0;
  }
  return RESULT_SUCESS;
}

RESULT stage_close(server_params *server, server_settings *settings) {
  close(server->sock_fd);
  return RESULT_SUCESS;
}

RESULT stage_execute(server_params *server, server_settings *settings) {
  PRINT("[server started] prort:%d\n", settings->port_no);
  while (1) {
    RESULT res = start_instance(server, settings);
    switch (res) {
    case RESULT_SUCESS:
      break;
    case RESULT_FAIL:
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
