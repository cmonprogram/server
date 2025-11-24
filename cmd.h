#ifndef CMD_H
#define CMD_H
#include "main.h"
RESULT cmd_exit(server_params *server, server_settings *settings,
                request_instance *request);
RESULT cmd_time(server_params *server, server_settings *settings,
                request_instance *request);
RESULT cmd_test_start(server_params *server, server_settings *settings,
                    request_instance *request);
RESULT cmd_test_end(server_params *server, server_settings *settings,
                  request_instance *request);

#endif