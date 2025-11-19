#include "main.h"
#include "stdio.h"

#include "sys/socket.h"
#include <string.h>
#include <unistd.h>

int stage_init(server_params *server, server_settings *settings) {
  memset(server, 0, sizeof(*server));
  server->server_addr.sin_family = AF_INET;
  server->server_addr.sin_addr.s_addr = INADDR_ANY;
  server->server_addr.sin_port = htons(settings->port_no);
  return 1;
}

int stage_create(server_params *server, server_settings *settings) {
  if (settings->protocol == UDP) {
    server->sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    return server->sock_fd >= 0;
  }
  return 0;
}

int stage_bind(server_params *server, server_settings *settings) {
  if (bind(server->sock_fd, (const struct sockaddr *)&server->server_addr,
           sizeof(server->server_addr)) < 0) {
    perror("[bind error]");
    return 0;
  }
  return 1;
}

int stage_close(server_params *server, server_settings *settings) {
  close(server->sock_fd);
  return 1;
}

int stage_execute(server_params *server, server_settings *settings) {
  PRINT("[server started] prort:%d\n", settings->port_no);
  char buffer[1000];
  socklen_t addr_len = sizeof(server->client_addr);
  while (1) {
    int n_bytes =
        recvfrom(server->sock_fd, (char *)buffer, sizeof(buffer), MSG_WAITALL,
                 (struct sockaddr *)&server->client_addr, &addr_len);

    if (n_bytes < 0) {
      perror("recvfrom failed");
      continue;
    }
    buffer[n_bytes] = '\0';
    printf("[get] %s\n", buffer);

    if (strcmp(buffer, "exit\n") == 0) {
      EXECUTE_STAGE("server close", stage_close, server, settings);
      return 1;
    }
  }
  return 0;
}
