#include "cmd.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

RESULT start_instance(server_params *server, server_settings *settings) {
  request_instance request;
  request.addr_len = sizeof(request.client_addr);
  if (settings->protocol == TCP) {
    if ((request.client_fd =
             accept(server->sock_fd, (struct sockaddr *)&request.client_addr,
                    (socklen_t *)&request.addr_len)) < 0) {
      perror("accept failed");
      return RESULT_FAIL;
    }
  }
  if (get_msg(server, settings, &request)) {
    PRINT("[get] %s\n", request.in_buffer);
  }

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
