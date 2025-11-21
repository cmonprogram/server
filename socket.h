#ifndef SOCKET_H
#define SOCKET_H
#include "main.h"
int stage_init(server_params *server, server_settings *settings) ;
int stage_create(server_params *server, server_settings *settings);
int stage_bind(server_params *server, server_settings *settings) ;
int stage_close(server_params *server, server_settings *settings);
int stage_execute(server_params *server, server_settings *settings);

int server_run(server_settings* settings);
#endif