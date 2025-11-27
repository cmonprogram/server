#ifndef SERVER_H
#define SERVER_H
#include "main.h"

RESULT stage_init(server_params *server, server_settings *settings);
RESULT stage_create(server_params *server, server_settings *settings);
RESULT stage_bind(server_params *server, server_settings *settings);
RESULT stage_close(server_params *server, server_settings *settings);
RESULT stage_execute(server_params *server, server_settings *settings);
RESULT server_run(server_settings *settings);
#endif
/*
RESULT test_start(){}
RESULT test_end(){}
RESULT exit(){}
*/