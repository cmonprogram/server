#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include "main.h"
#include "socket.h"

RESULT cmd_exit(server_params *server, server_settings *settings,
                request_instance *request) {
  if (stage_close(server, settings)) {
    return RESULT_EXIT;
  } else {
    return RESULT_FAIL;
  }
}

RESULT cmd_time(server_params *server, server_settings *settings,
                request_instance *request) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  long long milliseconds =
      (long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL;
  request->out_buffer_len = sprintf(request->out_buffer, "%lld", milliseconds);
  printf("[time] %s\n", request->out_buffer);
  return send_msg(server, settings, request);
}

//edit in server should be thread safe
RESULT cmd_test_start(server_params *server, server_settings *settings,
                    request_instance *request) {
  server->server_test_packages_number = 0;
  server->server_test_packages_number_end = 1000;
  PRINT("[test_start] on %d packages\n", server->server_test_packages_number_end);
  struct timeval tv;
  gettimeofday(&tv, NULL);
  server->server_test_start_milliseconds =
      (long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL;
  server->server_in_test = 1;
  return RESULT_SUCESS;
}

//edit in server should be thread safe
RESULT cmd_test_end(server_params *server, server_settings *settings,
                  request_instance *request) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  server->server_test_end_milliseconds =
      (long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL;
  PRINT("[test_end] %d packages by %dms\n",
        server->server_test_packages_number_end,
        (int)(server->server_test_end_milliseconds -
              server->server_test_start_milliseconds));
  server->server_in_test = 0;
    return RESULT_SUCESS;
}