#ifndef SOCKET_H
#define SOCKET_H
#include "main.h"
RESULT stage_init(server_params *server, server_settings *settings);
RESULT stage_create(server_params *server, server_settings *settings);
RESULT stage_bind(server_params *server, server_settings *settings);
RESULT stage_close(server_params *server, server_settings *settings);
RESULT stage_execute(server_params *server, server_settings *settings);

RESULT server_run(server_settings *settings);
RESULT get_msg(server_params *server, server_settings *settings,
               request_instance *request);
RESULT send_msg(server_params *server, server_settings *settings,
                request_instance *request);
#endif