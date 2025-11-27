
#include "arg.h"
#include "cmd.h"
#include "main.h"
#include "socket.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define CLOSE_TCP                                                              \
  if (settings->protocol == TCP)                                               \
    close(request.client_fd);
// #define return if (settings->protocol == TCP) close(request.client_fd);
// return

RESULT test_section(server_params *server, server_settings *settings,
                    request_instance *request) {
  // need better alternative for end
  if (server->server_in_test && strcmp(request->in_buffer, "test_end") == 0) {
    cmd_test_end(server, settings, request);
    return RESULT_SUCESS;
  } else if (server->server_in_test &&
             server->server_test_packages_number <
                 server->server_test_packages_number_end) {
    PRINT("[get %d/%d] %s\n", server->server_test_packages_number + 1,
          server->server_test_packages_number_end, request->in_buffer);
    if (++server->server_test_packages_number ==
        server->server_test_packages_number_end) {
      cmd_test_end(server, settings, request);
    }
    return RESULT_SUCESS;
  } else if (server->server_in_test) {
    cmd_test_end(server, settings, request);
    return RESULT_SUCESS;
  }
  return RESULT_FAIL;
}

RESULT html_section(server_params *server, server_settings *settings,
                    request_instance *request) {
  char HTML_HEADER[] = "GET / HTTP";
  if (request->in_buffer_len > sizeof(HTML_HEADER)) {
    if (strncmp(HTML_HEADER, request->in_buffer, sizeof(HTML_HEADER))) {
      PRINT("[get html]\n");
      return cmd_html(server, settings, request);
    }
  }
  return RESULT_FAIL;
}

RESULT parce_section(server_params *server, server_settings *settings,
                     request_instance *request) {
  PRINT("[get] %s\n", request->in_buffer);
  int len =
      parce_args(request->in_buffer, request->in_buffer_len, request->args,
                 sizeof(request->args) / sizeof(request->args[0]));
  if (len) {
    // PRINT("[get][%d] %s\n", len, request.in_buffer);
    /*
      printf("[args] %d\n", len);
      for (int i = 0; i < len; ++i) {
        printf("[arg%d]%s\n", i, args_array[i]);
      }
    */
    return RESULT_SUCESS;
  } else {
    PRINT_ERROR("wrong command");
    return RESULT_FAIL;
  }
}
RESULT command_section(server_params *server, server_settings *settings,
                       request_instance *request) {
  if (strcmp(request->args[0], "exit") == 0) {
    return cmd_exit(server, settings, request);
  } else if (strcmp(request->args[0], "time") == 0) {
    return cmd_time(server, settings, request);
  } else if (strcmp(request->args[0], "test_start") == 0) {
    return cmd_test_start(server, settings, request);
  } else {
    PRINT_ERROR("wrong command");
    return RESULT_FAIL;
  }
  // moved to tets section
  /*else if (strcmp(request->args[0], "test_end") == 0) {
    return cmd_test_end(server, settings, request);
  } */
}

RESULT start_instance(server_params *server, server_settings *settings) {
  request_instance request;
  request.addr_len = sizeof(request.client_addr);
  if (settings->protocol == TCP) {
    if ((request.client_fd =
             accept(server->sock_fd, (struct sockaddr *)&request.client_addr,
                    (socklen_t *)&request.addr_len)) < 0) {
      perror("accept failed");
      CLOSE_TCP;
      return RESULT_FAIL;
    }
  }
  if (get_msg(server, settings, &request)) {

    if (test_section(server, settings, &request)) {
      CLOSE_TCP;
      return RESULT_SUCESS;
    }

    if (html_section(server, settings, &request)) {
      CLOSE_TCP;
      return RESULT_SUCESS;
    }

    if (!parce_section(server, settings, &request)) {
      CLOSE_TCP;
      return RESULT_FAIL;
    }

    RESULT res = command_section(server, settings, &request);
    CLOSE_TCP;
    return res;
  }
  CLOSE_TCP;
  return RESULT_FAIL;
}