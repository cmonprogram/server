#include "instance.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

RESULT stage_init(server_params *server, server_settings *settings) {
  memset(server, 0, sizeof(*server));
  /*
  if (settings->protocol == TCP) {
    PRINT_ERROR("TCP not supported yet");
    return RESULT_EXIT;
  }
  */
  server->server_addr.sin_family = AF_INET;
  server->server_addr.sin_addr.s_addr = INADDR_ANY;
  server->server_addr.sin_port = htons(settings->port_no);
  return RESULT_SUCESS;
}

RESULT stage_create(server_params *server, server_settings *settings) {
  switch (settings->protocol) {
  case UDP:
    server->sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    break;
  case TCP:
    server->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    break;
  }
  return server->sock_fd >= 0 ? RESULT_SUCESS : RESULT_FAIL;
}

RESULT stage_bind(server_params *server, server_settings *settings) {
  if (bind(server->sock_fd, (const struct sockaddr *)&server->server_addr,
           sizeof(server->server_addr)) < 0) {
    perror("[bind error]");
    return 0;
  }
  return RESULT_SUCESS;
}

RESULT stage_close(server_params *server, server_settings *settings) {
  close(server->sock_fd);
  return RESULT_SUCESS;
}

RESULT stage_execute(server_params *server, server_settings *settings) {
  PRINT("[%s server started] prort:%d\n", settings->protocol == TCP ? "TCP" : "UDP"  ,settings->port_no);
  if (settings->protocol == TCP) {
    if ((listen(server->sock_fd, 5)) != 0) {
      printf("Listen failed...\n");
      return RESULT_FAIL;
    }
  }

  while (1) {
    RESULT res = start_instance(server, settings);
    switch (res) {
    case RESULT_SUCESS:
      break;
    case RESULT_FAIL:
      break;
    case RESULT_EXIT:
      return RESULT_EXIT;
      break;
    }
  }
  return RESULT_SUCESS;
}

RESULT server_run(server_settings *settings) {
  server_params server;
  EXECUTE_STAGE("init server", stage_init, &server, settings);
  EXECUTE_STAGE("create socket", stage_create, &server, settings);
  EXECUTE_STAGE("bind socket", stage_bind, &server, settings);
  EXECUTE_STAGE("finish server", stage_execute, &server, settings);
  return RESULT_SUCESS;
}

//need msg limit error
RESULT get_msg(server_params *server, server_settings *settings,
               request_instance *request) {
  if (settings->protocol == UDP) {
    request->in_buffer_len =
        recvfrom(server->sock_fd, (char *)request->in_buffer,
                 sizeof(request->in_buffer) - 1, MSG_WAITALL, // MSG_ZEROCOPY
                 (struct sockaddr *)&request->client_addr, &request->addr_len);
    if (request->in_buffer_len < 0) {
      PRINT_ERROR("[recvfrom failed]");
      return RESULT_FAIL;
    }
    request->in_buffer[request->in_buffer_len++] = '\0';
    return RESULT_SUCESS;
  } else if (settings->protocol == TCP) {
    request->in_buffer_len = read(request->client_fd, request->in_buffer,
                                  sizeof(request->in_buffer) - 1);
    if (request->in_buffer_len < 0) {
      perror("[read failed]");
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