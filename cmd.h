#include "main.h"
#include "socket.h"
#include "sys/time.h"
#include <stdio.h>

int  cmd_exit(server_params *server, server_settings *settings) {
    return stage_close(server, settings);
}

int cmd_time(server_params *server, server_settings *settings) {
      struct timeval tv;
      gettimeofday(&tv, NULL);
      long long milliseconds =
          (long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL;
      int out_buffer_len = sprintf(server->out_buffer, "%lld", milliseconds);
      printf("[time] %s\n", server->out_buffer);
      sendto(server->sock_fd, (char *)server->out_buffer, out_buffer_len, 0,
             (struct sockaddr *)&server->client_addr, server->addr_len);
     return RESULT_SUCESS;
}

void cmd_test_start() {}
void cmd_test_end() {}