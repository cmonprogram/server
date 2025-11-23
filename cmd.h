#ifndef CMD_H
#define CMD_H
#include "main.h"
RESULT get_msg(server_params *server, server_settings *settings,
               request_instance *request);
RESULT send_msg(server_params *server, server_settings *settings,
                request_instance *request);
RESULT cmd_exit(server_params *server, server_settings *settings,
                request_instance *request);
RESULT cmd_time(server_params *server, server_settings *settings,
                request_instance *request);
void cmd_test_start(server_params *server, server_settings *settings,
                    request_instance *request);
void cmd_test_end(server_params *server, server_settings *settings,
                  request_instance *request);
#endif