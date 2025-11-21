#include "main.h"
#include "arg.h"
#include "socket.h"

int main(int argc, char** argv){
    server_settings settings;
    if(!parce_settings(&settings, argc, argv)){
        return 1;
    }
    
     return server_run(&settings) ? 0 : 1;
}