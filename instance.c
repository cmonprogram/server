
#include "arg.h"
#include "cmd.h"
#include "main.h"
#include "socket.h"
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
    int len = parce_args(request.in_buffer, request.in_buffer_len, request.args,
                         sizeof(request.args) / sizeof(request.args[0]));
    if (len) {
      //PRINT("[get][%d] %s\n", len, request.in_buffer);
    } else {
      PRINT_ERROR( "wrong command");
      return RESULT_FAIL;
    }
    /*
    printf("[args] %d\n", len);
    for (int i = 0; i < len; ++i) {
      printf("[arg%d]%s\n", i, args_array[i]);
    }
    */
  }

  if (strcmp(request.args[0], "exit") == 0) {
    return cmd_exit(server, settings, &request);
  } else if (strcmp(request.args[0], "time") == 0) {
    cmd_time(server, settings, &request);
  } else if (strcmp(request.args[0], "start_test") == 0) {
    cmd_test_start(server, settings, &request);
  } else if (strcmp(request.args[0], "end_test") == 0) {
    cmd_test_end(server, settings, &request);
  }else{
    PRINT_ERROR( "wrong command");
  }
  return RESULT_SUCESS;
}