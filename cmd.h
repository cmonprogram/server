#include "main.h"
#include "socket.h"
#include "sys/time.h"
#include <stdio.h>

RESULT cmd_exit(server_params *server, server_settings *settings, request_instance* request) {
    if(stage_close(server, settings)){
        return RESULT_EXIT;
    }else{
        return RESULT_FAIL;
    }
}

RESULT cmd_time(server_params *server, server_settings *settings, request_instance* request) {
      struct timeval tv;
      gettimeofday(&tv, NULL);
      long long milliseconds =
          (long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL;
      int out_buffer_len = sprintf(request->out_buffer, "%lld", milliseconds);
      printf("[time] %s\n", request->out_buffer);
      sendto(server->sock_fd, (char *)request->out_buffer, out_buffer_len, 0,
             (struct sockaddr *)&request->client_addr, request->addr_len);
     return RESULT_SUCESS;
}

void cmd_test_start(server_params *server, server_settings *settings, request_instance* request)  {

}
void cmd_test_end(server_params *server, server_settings *settings, request_instance* request) {
    
}