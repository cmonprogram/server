#include "main.h"
#include "arg.h"
#include "stdio.h"
#include "socket.h"

int main(int argc, char** argv){
    server_settings settings;
    if(!parce_settings(&settings, argc, argv)){
        return 1;
    }

    server_params server;
    EXECUTE_STAGE("init server", stage_init, &server, &settings);
    EXECUTE_STAGE("create socket", stage_create, &server, &settings);
    EXECUTE_STAGE("bind socket", stage_bind, &server, &settings);
    EXECUTE_STAGE("finish server", stage_execute, &server, &settings);

}