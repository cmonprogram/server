
#include "instance.h"
#include "arg.h"
#include "cmd.h"
#include "main.h"
#include "socket.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define CLOSE                                                                  \
  if (server->settings.protocol == TCP_SERVER)                                 \
    close(request.client_fd);
// #define return if (settings->protocol == TCP_SERVER)
// close(request.client_fd); return

RESULT test_section(server_params *server, struct epoll_event *event) {
  epoll_handler *request = (epoll_handler *)event->data.ptr;
  // need better alternative for end
  if (server->server_in_test && strcmp(request->in_buffer, "test_end") == 0) {
    cmd_test_end(server, event);
    return RESULT_SUCESS;
  } else if (server->server_in_test &&
             server->server_test_packages_number <
                 server->server_test_packages_number_end) {
    PRINT("[get %d/%d] %s\n", server->server_test_packages_number + 1,
          server->server_test_packages_number_end, request->in_buffer);
    if (++server->server_test_packages_number ==
        server->server_test_packages_number_end) {
      cmd_test_end(server, event);
    }
    return RESULT_SUCESS;
  } else if (server->server_in_test) {
    cmd_test_end(server, event);
    return RESULT_SUCESS;
  }
  return RESULT_FAIL;
}

RESULT html_section(server_params *server, struct epoll_event *event) {
  epoll_handler *request = (epoll_handler *)event->data.ptr;
  char HTML_HEADER[] = "GET / HTTP";
  if (request->in_buffer_len > sizeof(HTML_HEADER) - 1) {
    if (strncmp(HTML_HEADER, request->in_buffer, sizeof(HTML_HEADER) - 1) ==
        0) {
      PRINT("[get html]\n");
      return cmd_html(server, event);
    }
  }
  return RESULT_FAIL;
}

RESULT parce_section(server_params *server, struct epoll_event *event) {
  epoll_handler *request = (epoll_handler *)event->data.ptr;
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
RESULT command_section(server_params *server, struct epoll_event *event) {
  epoll_handler *request = (epoll_handler *)event->data.ptr;
  if (strcmp(request->args[0], "exit") == 0) {
    return cmd_exit(server, event);
  } else if (strcmp(request->args[0], "time") == 0) {
    return cmd_time(server, event);
  } else if (strcmp(request->args[0], "test_start") == 0) {
    return cmd_test_start(server, event);
  } else {
    PRINT_ERROR("wrong command");
    return RESULT_FAIL;
  }
  // moved to tets section
  /*else if (strcmp(request->args[0], "test_end") == 0) {
    return cmd_test_end(server, settings, request);
  } */
}

RESULT start_instance(server_params *server) {
  int epoll_result =
      epoll_wait(server->epollfd, server->events, MAX_EVENTS, -1);
  if (epoll_result == -1) {
    perror("[epoll_wait error]");
    return RESULT_FAIL;
  } else if (epoll_result == 0) {
    return RESULT_FAIL;
  }

  for (int i = 0; i < epoll_result; ++i) {
    epoll_handler *request = (epoll_handler *)server->events[i].data.ptr;
    // printf("Event on fd %d; events: %u\n", request->client_fd,
    // server->events[i].events);
    RESULT msg_res = epoll_get_msg(&server->events[i]);
    if (msg_res == RESULT_SUCESS) {
      if (test_section(server, &server->events[i])) {
        delete_from_epoll(&server->events[i]);
        continue;
      }
      if (html_section(server, &server->events[i])) {
        delete_from_epoll(&server->events[i]);
        continue;
      }
      if (!parce_section(server, &server->events[i])) {
        delete_from_epoll(&server->events[i]);
        continue;
      }
      RESULT cmd_res = command_section(server, &server->events[i]);
      if (cmd_res == RESULT_EXIT) {
        delete_from_epoll(&server->events[i]);
        return cmd_res;
      }
    } else if (msg_res == RESULT_SKIP) {
      continue;
    } else if (msg_res == RESULT_FAIL && request->socket_type == TCP) {
      // PRINT_ERROR("read fail");
      delete_from_epoll(&server->events[i]);
      continue;
    }

    if (server->events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
      delete_from_epoll(&server->events[i]);
      continue;
    }
  }

  return RESULT_SUCESS;
}