#include "main.h"
#include "socket.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

RESULT get_msg(server_params *server, server_settings *settings,
               request_instance *request) {
  if (settings->protocol == UDP) {
    request->in_buffer_len =
        recvfrom(server->sock_fd, (char *)request->in_buffer,
                 sizeof(request->in_buffer) - 1, MSG_WAITALL, // MSG_ZEROCOPY
                 (struct sockaddr *)&request->client_addr, &request->addr_len);
    if (request->in_buffer_len < 0) {
      PRINT_ERROR("recvfrom failed");
      return RESULT_FAIL;
    }
    request->in_buffer[request->in_buffer_len++] = '\0';
    return RESULT_SUCESS;
  } else if (settings->protocol == TCP) {
    request->in_buffer_len = read(request->client_fd, request->in_buffer,
                                  sizeof(request->in_buffer) - 1);
    if (request->in_buffer_len < 0) {
      perror("read failed");
      return RESULT_FAIL;
    }
    request->in_buffer[request->in_buffer_len++] = '\0';
    return RESULT_SUCESS;
  }
  return RESULT_FAIL;
}

RESULT send_msg(server_params *server, server_settings *settings,
                request_instance *request) {
  if (settings->protocol == UDP) {
    if (sendto(server->sock_fd, (char *)request->out_buffer,
               request->out_buffer_len, 0,
               (struct sockaddr *)&request->client_addr, request->addr_len)) {
      return RESULT_SUCESS;
    }
  } else {
    if (send(request->client_fd, request->out_buffer, request->out_buffer_len,
             0)) {
      return RESULT_SUCESS;
    }
  }
  return RESULT_FAIL;
}

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

void cmd_test_start(server_params *server, server_settings *settings,
                    request_instance *request) {}
void cmd_test_end(server_params *server, server_settings *settings,
                  request_instance *request) {}