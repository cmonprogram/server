#include "main.h"
#include "socket.h"
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <unistd.h>

#include <sys/epoll.h>


RESULT cmd_exit(server_params *server, server_settings *settings,
                struct epoll_event *event) {
   epoll_handler *request = (epoll_handler *)event->data.ptr;
  if (stage_close(server, settings)) {
    return RESULT_EXIT;
  } else {
    return RESULT_FAIL;
  }
}

RESULT cmd_time(server_params *server, server_settings *settings,
                struct epoll_event *event) {
   epoll_handler *request = (epoll_handler *)event->data.ptr;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  long long milliseconds =
      (long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL;
  request->out_buffer_len = sprintf(request->out_buffer, "%lld", milliseconds);
  printf("[time] %s\n", request->out_buffer);
  return epoll_send_msg(event);
}

// edit in server should be thread safe
RESULT cmd_test_start(server_params *server, server_settings *settings,
                struct epoll_event *event) {
   epoll_handler *request = (epoll_handler *)event->data.ptr;
  server->server_test_packages_number = 0;
  server->server_test_packages_number_end = 1000;
  request->out_buffer_len =
      sprintf(request->out_buffer, "[test_start] on %d packages\n",
              server->server_test_packages_number_end);
  PRINT("%s", request->out_buffer);
  struct timeval tv;
  gettimeofday(&tv, NULL);
  server->server_test_start_milliseconds =
      (long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL;
  server->server_in_test = 1;
  // return send_msg(server, settings, request);
  return RESULT_SUCESS;
}

// edit in server should be thread safe
RESULT cmd_test_end(server_params *server, server_settings *settings,
                struct epoll_event *event) {
   epoll_handler *request = (epoll_handler *)event->data.ptr;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  server->server_test_end_milliseconds =
      (long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL;
  server->server_in_test = 0;
  request->out_buffer_len =
      sprintf(request->out_buffer, "[test_end] %d packages by %dms\n",
              server->server_test_packages_number,
              (int)(server->server_test_end_milliseconds -
                    server->server_test_start_milliseconds));
  PRINT("%s", request->out_buffer);
  return epoll_send_msg(event);
}

RESULT cmd_html(server_params *server, server_settings *settings,
                struct epoll_event *event) {
   epoll_handler *request = (epoll_handler *)event->data.ptr;
  char response[] =
      "HTTP/1.1 200 OK \n Content-Type: text/xml;charset=utf-8 \n "
      "Content-Length: 256 \n\n"
      "<!doctype html>"
      "<html lang=\"en\">"
      "<head>"
      "<meta charset=\"UTF-8\">"
      "<title>Server</title>"
      "</head>"
      "<body><a "
      "href=\"https://github.com/cmonprogram/server\">https://github.com/"
      "cmonprogram/server</a>"
      "</body>"
      "</html>\n";
  request->out_buffer_len = sprintf(request->out_buffer, "%s", response);
  return epoll_send_msg(event);
}