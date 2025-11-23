#ifndef INSTANCE_H
#define INSTANCE_H
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "cmd.h"
#include "main.h"

int start_instance(server_params *server, server_settings *settings) {
  request_instance request;
  request.addr_len = sizeof(request.client_addr);
  int n_bytes =
      recvfrom(server->sock_fd, (char *)request.in_buffer,
               sizeof(request.in_buffer) - 1, MSG_WAITALL, // MSG_ZEROCOPY
               (struct sockaddr *)&request.client_addr, &request.addr_len);
  if (n_bytes < 0) {
    PRINT_ERROR("recvfrom failed");
    return RESULT_FAIL;
  }
  request.in_buffer[n_bytes] = '\0';
 PRINT("[get] %s\n", request.in_buffer);
  if (strcmp(request.in_buffer, "exit") == 0) {
    return cmd_exit(server, settings, &request);
  } else if (strcmp(request.in_buffer, "time") == 0) {
    cmd_time(server, settings, &request);
  } else if (strcmp(request.in_buffer, "start_test") == 0) {
    cmd_test_start(server, settings, &request);
  } else if (strcmp(request.in_buffer, "end_test") == 0) {
    cmd_test_end(server, settings, &request);
  }
  return RESULT_SUCESS;
}

#endif