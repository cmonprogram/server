#include "main.h"
#include "stdio.h"
#include "sys/socket.h"
#include <string.h>
#include <unistd.h>
#include "cmd.h"

int stage_init(server_params *server, server_settings *settings) {
    if(settings->protocol == TCP){
            PRINT("TCP not supported yet\n");
            return RESULT_FAIL;
    }
  memset(server, 0, sizeof(*server));
  server->server_addr.sin_family = AF_INET;
  server->server_addr.sin_addr.s_addr = INADDR_ANY;
  server->server_addr.sin_port = htons(settings->port_no);
  server->addr_len = sizeof(server->client_addr);
  return RESULT_SUCESS;
}

int stage_create(server_params *server, server_settings *settings) {
  if (settings->protocol == UDP) {
    server->sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    return server->sock_fd >= 0;
  }
  return RESULT_SUCESS;
}

int stage_bind(server_params *server, server_settings *settings) {
  if (bind(server->sock_fd, (const struct sockaddr *)&server->server_addr,
           sizeof(server->server_addr)) < 0) {
    perror("[bind error]");
    return 0;
  }
  return RESULT_SUCESS;
}

int stage_close(server_params *server, server_settings *settings) {
  close(server->sock_fd);
  return RESULT_SUCESS;
}

int stage_execute(server_params *server, server_settings *settings) {
  PRINT("[server started] prort:%d\n", settings->port_no);
  while (1) {
    int n_bytes =
        recvfrom(server->sock_fd, (char *)server->in_buffer, sizeof(server->in_buffer), MSG_WAITALL,
                 (struct sockaddr *)&server->client_addr, &server->addr_len);

    if (n_bytes < 0) {
      perror("recvfrom failed");
      continue;
    }
    server->in_buffer[n_bytes] = '\0';
    PRINT("[get] %s\n", server->in_buffer);

    if (strcmp(server->in_buffer, "exit") == 0) {
      return cmd_exit(server,settings);
    } else if (strcmp(server->in_buffer, "time") == 0) {
      cmd_time(server,settings);
    }
  }
  return RESULT_SUCESS;
}

int server_run(server_settings* settings){
    server_params server;
    EXECUTE_STAGE("init server", stage_init, &server, settings);
    EXECUTE_STAGE("create socket", stage_create, &server, settings);
    EXECUTE_STAGE("bind socket", stage_bind, &server, settings);
    EXECUTE_STAGE("finish server", stage_execute, &server, settings);
    return RESULT_SUCESS;
}
